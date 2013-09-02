#pragma once
#include "cocos2d.h"
USING_NS_CC;
class Block : public CCLayer
{
public:
	bool isSelected;
	int blockX,blockY;
	int blockType;
	CCSprite* bgSprite;
	CCSprite* block;
	bool m_bTouchEnabled;
public:
	Block();
	~Block();
	static Block* create(CCSprite* bg,CCSprite* block,int x,int y);
	void registerWithTouchDispatcher();
	void setBlockPos(int x,int y);
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void customInit(CCSprite* bg,CCSprite* block);
	void blockRemove();
	void refreshTxt();
};
