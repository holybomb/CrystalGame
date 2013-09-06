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
	int mStatus;
	CCArray* selectBlock;
	int mBlockLeft;
	bool isFallDown;
	bool isBeginMove;
	CCLayerColor* mGameLayer;
	int mapW;
	int mapH;
	int offX;
	int offY;
	CCSpawn* blockSpawn;
	CCLayerColor*  mGameLayerBG;
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
	void setTouchEnabled(bool value);
	void showGameEnd();

	void setStatusToInit();
	void setStatusToRun();
	void setStatusToShowEnd();
	void setStatusToShowEndOver();
	void setStatus(int status);
	void update(float delta);
};