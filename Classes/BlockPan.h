#pragma once
#include "cocos2d.h"
#include "Block.h"
USING_NS_CC;
class BlockPan :
	public CCLayerColor
{
public:
	int mBlockLeft;
	CCString* mCurSelectType;
	bool isBeginMove;
	CCLayer* mGameLayer;
	int mapW;
	int mapH;
public:
	BlockPan(void);
	~BlockPan(void);
	CREATE_FUNC(BlockPan);
	virtual bool init();
	void createBlockPan();
	void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void blockFallDown( int index, Block *block );
	int findBlockIDByPos(int x,int y);
};

