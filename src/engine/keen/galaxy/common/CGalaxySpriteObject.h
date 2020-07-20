/*
 * CGalaxySpriteObject.h
 *
 *  Created on: 21.06.2012
 *      Author: gerstrong
 */

#ifndef CGALAXYSPRITEOBJECT_H_
#define CGALAXYSPRITEOBJECT_H_

#include <fileio/CSaveGameController.h>

#include "engine/core/CSpriteObject.h"


class CGalaxySpriteObject: public CSpriteObject
{
public:
    CGalaxySpriteObject(CMap *pmap, const Uint16 foeID, Uint32 x, Uint32 y, const int sprVar);

	void setupGalaxyObjectOnMap(const size_t ActionBaseOffset = 0x0,
				    const size_t ActionNumber = 0x0 );

	void performPhysAccelHor( const int accX, const int velLimit );
	void performPhysDampHorz();
	void performGravityLow();
	void performGravityMid();
	void performGravityHigh();
	void performInverseGravityHigh();

	void processFalling();

	int checkSolidU(int x1, int x2, int y1, const bool push_mode=false );
	int checkSolidD(int x1, int x2, int y2, const bool push_mode=false );

	bool getActionNumber(int16_t ActionNumber);
	virtual bool getActionStatus(int16_t ActionNumber);
	int16_t getActionNumber();
	virtual void setActionForce(const size_t ActionNumber);
	void setAction(size_t ActionNumber);
	void setActionSprite();
	virtual bool processActionRoutine();
	
	
	// For saving and loading...
	virtual void deserialize(CSaveGameController &savedGame) {}
	virtual void serialize(CSaveGameController &savedGame) {}

    virtual void serialize(boost::property_tree::ptree &node) {}
    virtual void deserialize(boost::property_tree::ptree &node) {}

	// This is used for action format only
	uint16_t mFoeID;
	uint16_t m_ActionTicker;
	uint16_t m_ActionNumber;
	//Vector2D<int> next;
	size_t m_ActionBaseOffset;

	int nextX;
	int nextY;

	//TileInfo for surrounding tiles.
	int topTI, bottomTI, leftTI, rightTI;


	int user1;
	int user2;
	int user3;
	int user4;

	bool m_jumped;
	bool m_climbing;
	bool mClipped;
	bool m_jumpdown;
	bool mEndOfAction;
};

#endif /* CGALAXYSPRITEOBJECT_H_ */
