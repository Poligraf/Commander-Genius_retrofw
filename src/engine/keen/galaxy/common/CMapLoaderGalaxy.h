/*
 * CVorticonMapLoaderGalaxy.h
 *
 *  Created on: 29.05.2010
 *      Author: gerstrong
 */

#ifndef CVorticonMapLoaderGALAXY_H_
#define CVorticonMapLoaderGALAXY_H_

#include <memory>
#include <vector>
#include <string>
#include <SDL.h>

#include <base/TypeDefinitions.h>
#include "engine/core/CMap.h"
#include "engine/core/Cheat.h"
#include "CInventory.h"
#include "CGalaxySpriteObject.h"

namespace galaxy
{

class CMapLoaderGalaxy
{
public:
    CMapLoaderGalaxy(std::vector< std::shared_ptr<CGalaxySpriteObject> > &ObjectPtr,
            std::vector<CInventory> &inventoryVec, stCheat &Cheatmode);
	
	size_t getMapheadOffset();
	bool gotoNextSignature(std::ifstream &MapFile);
	bool loadMap(CMap &Map, Uint8 level);
	void spawnFoes(CMap &Map);
	
	std::string getLevelName()
	{ return mLevelName; }
	
	
    virtual bool isKeenPlayer(const int foeID) = 0;

	virtual CGalaxySpriteObject* addFoe(CMap &Map, word foe, size_t x, size_t y) = 0;
	virtual ~CMapLoaderGalaxy() {}

    int numLoadedPlayers()
    {
        return mNumLoadedPlayers;
    }

    void resetNumLoadedPlayers()
    {
        mNumLoadedPlayers = 0;
    }

protected:
    void unpackPlaneData(std::ifstream &MapFile,
            CMap &Map, const size_t planeNumber,
            longword offset, longword length,
            word magic_word);

	std::vector< std::shared_ptr<CGalaxySpriteObject> > &m_ObjectPtr;
    std::vector<CInventory> &mInventoryVec;
	stCheat &m_Cheatmode;
	std::string mLevelName;
    int mNumLoadedPlayers;
};

}

#endif /* CVorticonMapLoaderGALAXY_H_ */
