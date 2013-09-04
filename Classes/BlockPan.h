#pragma once
#include "cocos2d.h"
#include "Block.h"
USING_NS_CC;
class BlockPan :
	public CCLayerColor
{
public:
	static int mCurSelectType;
public:
	CCArray* selectBlock;
	int mBlockLeft;
	bool isFallDown;
	bool isBeginMove;
	CCLayer* mGameLayer;
	int mapW;
	int mapH;
	int offX;
	int offY;
	CCSpawn* blockSpawn;
public:
	BlockPan(void);
	~BlockPan(void);
	CREATE_FUNC(BlockPan);
	virtual bool init();
	void createBlockPan();
	void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void blockFallDown( CCObject *block );
	int findBlockIDByPos(int x,int y);
	Block* findBlockByPos(int x,int y);
	int findLastLineByCol(int x);
	void moveIsDone();
	Block* createNewBlock(int x,int y,int col);
	void blocksRemove();
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	bool isSameTypeBlock(Block* pBlock);
};