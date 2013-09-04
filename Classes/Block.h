#pragma once
#define BLOCK_LABEL_TAG 10
#define PER_BLOCK_SCORE 100
#define ccb(__X__,__Y__) BlockPos::create(ccp((float)(__X__), (float)(__Y__)))
//#include "BlockPan.h"
#include "cocos2d.h"
USING_NS_CC;


class Block : public CCLayer
{
public:
	bool isSelected;
	bool isRemoved;
	int blockX,blockY;
	int col;
	int blockType;
	CCSprite* block;
	bool m_bTouchEnabled;
	CCLabelTTF* mLabel;
public:
	Block();
	~Block();
	static Block* create(int x,int y,int col);
	void registerWithTouchDispatcher();
	void setBlockPos(int x,int y);
	void setBlockPos(CCObject* point);
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void customInit(int x,int y,int col);
	void blockRemove();
	void refreshTxt();
};
class BlockPos : public CCObject
{
public:
	int x,y;
public:
	static BlockPos* create(CCPoint p);
};