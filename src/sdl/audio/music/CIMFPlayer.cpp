/*
 * CIMFPlayer.cpp
 *
 *  Created on: 17.02.2011
 *      Author: gerstrong
 *
 *  Player which reproduces the IMF Data
 */

#include "CIMFPlayer.h"
#include "fileio/ResourceMgmt.h"
#include "fileio/compression/CHuffman.h"
#include "fileio/KeenFiles.h"
#include <base/utils/FindFile.h>
#include <base/GsLogging.h>
#include <fstream>
#include <string>
#include <cassert>


CIMFPlayer::CIMFPlayer( const SDL_AudioSpec& AudioSpec, COPLEmulator& opl_emulator ) :
m_AudioDevSpec(AudioSpec),
m_opl_emulator(opl_emulator),
m_numreadysamples(0),
m_samplesPerMusicTick(m_AudioDevSpec.freq / m_opl_emulator.getIMFClockRate()),
m_IMFDelay(0),
mMixBuffer(m_AudioDevSpec.samples, 0)
{}


bool CIMFPlayer::loadMusicFromFile(const std::string& filename)
{
    // Open the IMF File
    FILE *fp;
    word data_size;
    bool ok = false;
    
    if( ( fp = OpenGameFile(filename, "rb") ) == NULL )
        return false;
    
    int read_first = fread( &data_size, sizeof(word), 1, fp);
    
    if( read_first == 0)
        return false;

    if (data_size == 0) // Is the IMF file of Type-0?
    {
        fseek(fp, 0, SEEK_END);
        data_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
    }
    
    
    if(!m_IMF_Data.empty())
	m_IMF_Data.clear();
    const word imf_chunks = (data_size/sizeof(IMFChunkType));
    m_IMF_Data.reserve(imf_chunks);
    
    if( imf_chunks != fread( m_IMF_Data.getStartPtr(), sizeof(IMFChunkType), imf_chunks, fp ) )
	gLogging.textOut("The IMF-File seems to be corrupt.");
    else
	ok = true;
    
    fclose(fp);
    
    return ok;
}

void CIMFPlayer::swapRing(RingBuffer<IMFChunkType> &&ring)
{
    m_IMF_Data = std::move(ring);
}





bool CIMFPlayer::open()
{
	m_numreadysamples = m_IMFDelay = 0;
	m_samplesPerMusicTick = m_AudioDevSpec.freq / m_opl_emulator.getIMFClockRate();
	
	m_opl_emulator.setup();
	
	return !m_IMF_Data.empty();
}

void CIMFPlayer::close()
{
	play(false);
	m_IMF_Data.gotoStart();	
	m_numreadysamples = m_IMFDelay = 0;	
	m_opl_emulator.ShutAL();
	m_opl_emulator.shutdown();

	return;
}


#include <fstream>


void CIMFPlayer::OPLUpdate(byte *buffer, const unsigned int length)
{
    if(mMixBuffer.empty())
    {
        gLogging.textOut("Warning OPL Buffer is empty!");
        return;
    }

    m_opl_emulator.Chip__GenerateBlock2( length, mMixBuffer.data() );

    // Mix into the destination buffer, doubling up into stereo.
	if(m_AudioDevSpec.format == AUDIO_S16)
	{
		Sint16 *buf16 = (Sint16*) (void*) buffer;
		for (unsigned int i=0; i<length; ++i)
		{
		    Sint32 mix = mMixBuffer[i];
		    
		    if(mix > 32767)
			    mix = 32767;
		    else if(mix < -32768)
			    mix = -32768;
		    
			for (unsigned int j=0; j<m_AudioDevSpec.channels; j++)
			{
				*buf16 = mix + m_AudioDevSpec.silence;
				buf16++;
			}
		}
	}
	else if(m_AudioDevSpec.format == AUDIO_U8)
	{
		for (unsigned int i=0; i<length; ++i)
		{
		    Sint32 mix = mMixBuffer[i]>>8;
		    
		    if(mix > 255)
			    mix = 255;
		    else if(mix < 0)
			    mix = 0;
		    
			for (unsigned int j=0; j<m_AudioDevSpec.channels; j++)
			{
				*buffer = mix + m_AudioDevSpec.silence;
				buffer++;
			}
		}
	}
}

void CIMFPlayer::readBuffer(Uint8* buffer, Uint32 length)
{
    if(!m_playing)
        return;
    
    /// if a delay of the instruments is pending, play it
	Uint32 sampleslen = m_AudioDevSpec.samples;
	Uint32 sample_mult = m_AudioDevSpec.channels;
	sample_mult = (m_AudioDevSpec.format == AUDIO_S16) ? sample_mult*sizeof(Sint16) : sample_mult*sizeof(Uint8) ;
	
	// while the waveform is not filled
    while(1)
    {
        while( m_IMFDelay == 0 )
        {
            //read next IMF event
            const IMFChunkType Chunk = m_IMF_Data.getNextElement();
            m_IMFDelay = Chunk.Delay;

            //write reg+val to opl chip
            m_opl_emulator.Chip__WriteReg( Chunk.al_reg, Chunk.al_dat );
            m_numreadysamples = m_samplesPerMusicTick*m_IMFDelay;
        }

        //generate <delay> ticks of audio
        if(m_numreadysamples < sampleslen)
        {
            // Every time a tune has been played call this.
            OPLUpdate( buffer, m_numreadysamples );
            buffer += m_numreadysamples*sample_mult;
            sampleslen -= m_numreadysamples;
            m_IMFDelay = 0;
        }
        else
        {
            // Read the last stuff left in the emulators buffer. At this point the stream buffer is nearly full
            OPLUpdate( buffer, sampleslen );
            m_numreadysamples -= sampleslen;
            break;
        }
    }
}
