#pragma once
#include "cocos2d.h"
USING_NS_CC;
class Block : public CCObject
{
public:
	int blockType;
	CCSprite* blockSprite;
public:
	Block(void);
	~Block(void);
	bool init();
	CREATE_FUNC(Block);
};

