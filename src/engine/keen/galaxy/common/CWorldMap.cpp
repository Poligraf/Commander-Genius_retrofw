/*
 * CWorldMap.cpp
 *
 *  Created on: 14.07.2010
 *      Author: gerstrong
 */

#include "CWorldMap.h"
#include "fileio/CSaveGameController.h"
#include "graphics/effects/CColorMerge.h"
#include "engine/core/VGamepads/vgamepadsimple.h"


#include "../GalaxyEngine.h"
#include "dialog/CMessageBoxBitmapGalaxy.h"

namespace galaxy {

CWorldMap::CWorldMap(std::vector<CInventory> &inventoryVec,
					stCheat &Cheatmode):
CMapPlayGalaxy(inventoryVec, Cheatmode)
{}

void CWorldMap::init()
{
	// Load the World map level.
	std::unique_ptr<CMapLoaderGalaxy> MapLoader;

	if(g_pBehaviorEngine->getEpisode() == 4)
        MapLoader.reset( new CMapLoaderGalaxyEp4( mObjectPtr, mInventoryVec, mCheatmode) );
	else if(g_pBehaviorEngine->getEpisode() == 5)
        MapLoader.reset( new CMapLoaderGalaxyEp5( mObjectPtr, mInventoryVec, mCheatmode) );
	else if(g_pBehaviorEngine->getEpisode() == 6)
        MapLoader.reset( new CMapLoaderGalaxyEp6( mObjectPtr, mInventoryVec, mCheatmode) );

	MapLoader->loadMap( mMap, 0 );
	g_pBehaviorEngine->mapLevelName = MapLoader->getLevelName();

	const std::string loading_text = g_pBehaviorEngine->getString("WORLDMAP_LOAD_TEXT");

    gEffectController.setupEffect(new CColorMerge(8));
	
	if(!gpSaveGameController->busy())
	{
        showMsgWithBmp(loading_text, "KEENTHUMBSUP", LEFT);
	}

	mMap.drawAll();
}


/**
 * The map is not loaded again after the game started. But we have to reload its song.
 */
void CWorldMap::loadAndPlayMusic()
{
	g_pMusicPlayer->stop();
    if(loadLevelMusic(0))
		g_pMusicPlayer->play();
}

void CWorldMap::ponder(const float deltaT)
{
#ifdef TOUCHCONTROLS
    VirtualKeenControl *vkc = dynamic_cast<VirtualKeenControl*>(gInput.mpVirtPad.get());
    assert(vkc);
    vkc->mButtonMode = VirtualKeenControl::WMAP;
    vkc->mHideEnterButton = true;
#endif

    ponderBase(deltaT);
}


}
