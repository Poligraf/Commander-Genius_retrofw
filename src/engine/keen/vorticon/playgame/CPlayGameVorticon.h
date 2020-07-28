/*
 * CPlayGameVorticon.h
 *
 *  Created on: 03.10.2009
 *      Author: gerstrong
 *
 */


#ifndef CPlayGameVorticon_H_
#define CPlayGameVorticon_H_

#include "graphics/GsGraphics.h"
#include "fileio/CSaveGameController.h"
#include "../ai/CPlayer.h"
#include "fileio/CTileProperties.h"
#include "../CVorticonSpriteObject.h"
#include "engine/core/CMap.h"
#include "engine/core/options.h"
#include "engine/core/CHUD.h"
#include "../ai/CVorticonSpriteObjectAI.h"
#include "../ai/CEGABitmap.h"
#include "../dialog/CMessageBoxVort.h"
#include "sdl/audio/music/CMusic.h"
#include "../CHighScores.h"
#include "engine/core/mode/CPlayGame.h"
#include "../finale/CFinale.h"
#include <SDL.h>
#include <string>
#include <vector>
#include <list>
#include <memory>

#include "engine/core/mode/CGameMode.h"

const int WORLD_MAP_LEVEL_VORTICON = 80;

enum door_colours{
DOOR_YELLOW = 2,
DOOR_RED = 3,
DOOR_GREEN = 4,
DOOR_BLUE =  5
};

class CPlayGameVorticon : public CPlayGame
{
public:

    CPlayGameVorticon(const int startlevel = 0);

	void setupPlayers();
    bool init();

	// Game states
	bool loadGameState();
	bool saveGameState();

    bool loadXMLGameState();
    bool saveXMLGameState();

    void pumpEvent(const CEvent *evPtr);

    void ponder(const float deltaT);
	void processOnWorldMap();
	void processInLevel();

    void render();

	void showKeensLeft();
	void goBacktoMap();
	void YourShipNeedsTheseParts();
	void ShipEp3();
	void processObjectsAI();
	void drawObjects();
	void drawAllElements();
	void cycleCamLead();
	void handleFKeys();
	void verifyFinales();
	void collectHighScoreInfo(CHighScores &highScores);
	void createFinale();

	// Collision stuff stored in CPlayGameVorticonCollision.cpp
	void checkPlayerCollisions(CPlayer *p_player);
	bool checkisSolidr(CPlayer *p_player);
	bool checkisSolidl(CPlayer *p_player);
	bool checkisSolidd(CPlayer *p_player);
	bool checkisSolidu(CPlayer *p_player);
	void checkStandingOnIce(CPlayer &player);
	char checkDoorBlock(int t, CPlayer *p_player, int which);
	void processPlayerfallings(CPlayer *p_player);
	void processLevelTrigger(int trigger);
	void losePlayer(CPlayer *p_player);

	// Dialog processes stored in CPlayGameVorticonDialogs.cpp
	void processPauseDialogs();
    void renderPauseDialogs();
	void showPausedGameDlg();

	// Teleportation Stuff
	int getTeleporterInfo(int objectID);
	void teleportPlayer(int objectID, CPlayer &player);
	void teleportPlayerFromLevel( CPlayer &player, int origx, int origy);
	void readTeleportDestCoordinatesEP1(int objectID, int &destx, int &desty);
	void readTeleportDestCoordinatesEP3(int objectID, int &destx, int &desty);

	void cleanup();

private:

	// Tell whether any of the Players' status screen is open
	bool StatusScreenOpen();

	bool mpLevelCompleted[MAX_LEVELS_VORTICON];

	std::shared_ptr<CMap> mMap;
	std::vector<CPlayer> m_Player;
	std::shared_ptr<CVorticonSpriteObjectAI> mpObjectAI;
	std::unique_ptr<CFinale> mpFinale;
	std::unique_ptr<CEGABitmap> mpGameoverBmp;
	std::list< std::shared_ptr<CMessageBoxVort> > mMessageBoxes;
	bool m_showKeensLeft;
	std::vector< std::unique_ptr<CVorticonSpriteObject> > mSpriteObjectContainer;
    std::vector< std::unique_ptr<CHUD> > mpHUDVec;
	std::shared_ptr<SDL_Surface> mpKeenLeftSfc;
	SDL_Rect keenleft_rect;
};
#endif /* CPlayGameVorticon_H_ */
