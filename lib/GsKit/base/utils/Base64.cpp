#include "Base64.h"

#include <stdexcept>

#ifndef TEXT
    #define TEXT(x) x
#endif

//Lookup table for encoding
//If you want to use an alternate alphabet, change the characters here
const static TCHAR encodeLookup[] = TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
const static TCHAR padCharacter = TEXT('=');

#ifndef DWORD
    #define DWORD long
#endif

std::basic_string<TCHAR> base64Encode(const std::vector<BYTE> &inputBuffer)
{
        std::basic_string<TCHAR> encodedString;
        encodedString.reserve(((inputBuffer.size()/3) + (inputBuffer.size() % 3 > 0)) * 4);
        DWORD temp;
        auto cursor = inputBuffer.begin();

        for(size_t idx = 0; idx < inputBuffer.size()/3; idx++)
        {
                temp  = (*cursor++) << 16; //Convert to big endian
                temp += (*cursor++) << 8;
                temp += (*cursor++);
                encodedString.append(1,encodeLookup[(temp & 0x00FC0000) >> 18]);
                encodedString.append(1,encodeLookup[(temp & 0x0003F000) >> 12]);
                encodedString.append(1,encodeLookup[(temp & 0x00000FC0) >> 6 ]);
                encodedString.append(1,encodeLookup[(temp & 0x0000003F)      ]);
        }

        switch(inputBuffer.size() % 3)
        {
        case 1:
                temp  = (*cursor++) << 16; //Convert to big endian
                encodedString.append(1,encodeLookup[(temp & 0x00FC0000) >> 18]);
                encodedString.append(1,encodeLookup[(temp & 0x0003F000) >> 12]);
                encodedString.append(2,padCharacter);
                break;
        case 2:
                temp  = (*cursor++) << 16; //Convert to big endian
                temp += (*cursor++) << 8;
                encodedString.append(1,encodeLookup[(temp & 0x00FC0000) >> 18]);
                encodedString.append(1,encodeLookup[(temp & 0x0003F000) >> 12]);
                encodedString.append(1,encodeLookup[(temp & 0x00000FC0) >> 6 ]);
                encodedString.append(1,padCharacter);
                break;
        }
        return encodedString;
}




std::basic_string<TCHAR> base64Encode(BYTE *inputBuffer, const size_t size)
{
   std::vector<BYTE> inputBufferVec;

   for( size_t i = 0 ; i<size ; i++ )
   {
       inputBufferVec.push_back(inputBuffer[i]);
   }

   return base64Encode(inputBufferVec);
}





std::vector<BYTE> base64Decode(const std::basic_string<TCHAR>& input)
{
        if (input.length() % 4) //Sanity check
                throw std::runtime_error("Non-Valid base64!");
        size_t padding = 0;
        if (input.length())
        {
                if (input[input.length()-1] == padCharacter)
                        padding++;
                if (input[input.length()-2] == padCharacter)
                        padding++;
        }
        //Setup a vector to hold the result
        std::vector<BYTE> decodedBytes;
        decodedBytes.reserve(((input.length()/4)*3) - padding);
        DWORD temp=0; //Holds decoded quanta
        std::basic_string<TCHAR>::const_iterator cursor = input.begin();
        while (cursor < input.end())
        {
                for (size_t quantumPosition = 0; quantumPosition < 4; quantumPosition++)
                {
                        temp <<= 6;
                        if       (*cursor >= 0x41 && *cursor <= 0x5A) // This area will need tweaking if
                                temp |= *cursor - 0x41;                       // you are using an alternate alphabet
                        else if  (*cursor >= 0x61 && *cursor <= 0x7A)
                                temp |= *cursor - 0x47;
                        else if  (*cursor >= 0x30 && *cursor <= 0x39)
                                temp |= *cursor + 0x04;
                        else if  (*cursor == 0x2B)
                                temp |= 0x3E; //change to 0x2D for URL alphabet
                        else if  (*cursor == 0x2F)
                                temp |= 0x3F; //change to 0x5F for URL alphabet
                        else if  (*cursor == padCharacter) //pad
                        {
                                switch( input.end() - cursor )
                                {
                                case 1: //One pad character
                                        decodedBytes.push_back((temp >> 16) & 0x000000FF);
                                        decodedBytes.push_back((temp >> 8 ) & 0x000000FF);
                                        return decodedBytes;
                                case 2: //Two pad characters
                                        decodedBytes.push_back((temp >> 10) & 0x000000FF);
                                        return decodedBytes;
                                default:
                                        throw std::runtime_error("Invalid Padding in Base 64!");
                                }
                        }  else
                                throw std::runtime_error("Non-Valid Character in Base 64!");
                        cursor++;
                }
                decodedBytes.push_back((temp >> 16) & 0x000000FF);
                decodedBytes.push_back((temp >> 8 ) & 0x000000FF);
                decodedBytes.push_back((temp      ) & 0x000000FF);
        }
        return decodedBytes;
}

void base64Decode(BYTE *outBuffer, const std::basic_string<char> &inBuffer)
{
   std::vector<BYTE> outputBufferVec = base64Decode(inBuffer);

   int i=0;
   for( auto &item : outputBufferVec )
   {
       outBuffer[i++] = item;
   }
}
