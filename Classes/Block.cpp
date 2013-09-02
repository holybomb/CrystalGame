#include "Block.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Utils.h"
USING_NS_CC_EXT;
void Block::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-12,false);
}

bool Block::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(!block->isVisible()) return false;
	CCPoint touchLocation = convertTouchToNodeSpace(pTouch); 
	if(Utils::getRect(block).containsPoint(touchLocation)) 
	{ 
		CCLOG("我被点中了：%d",this->getTag());
		block->setOpacity(150);
	} 

	return true;
}

Block* Block::create(CCSprite* bg,CCSprite* block,int x,int y)
{
	Block *testSprite= new Block();
	testSprite->blockX = x;
	testSprite->blockY = y;
	if(testSprite&&testSprite->init()){

		testSprite->autorelease();

		testSprite->customInit(bg,block);//

		return testSprite;

	}

	CC_SAFE_DELETE(testSprite);

	return testSprite;


}

void Block::customInit(CCSprite* pBgSprite,CCSprite* pBlock)
{
	int sw = pBgSprite->getContentSize().width;
	int sh = pBgSprite->getContentSize().height;
	this->bgSprite = pBgSprite;
	this->block = pBlock;
//	this->addChild(pBgSprite,-1,0);
//	pBlock->setPosition(ccp(sw/2,sh/2));
	this->addChild(pBlock,11,0);
	block->setPosition(ccp(0,400));
	CCMoveTo* moveTo = CCMoveTo::create(0.5f,CCPointZero);
	CCEaseSineInOut * easeIn = CCEaseSineInOut::create(moveTo);
	block->runAction(easeIn);
	CCString* txt = CCString::createWithFormat("%i,%i,%i",blockX,blockY,getTag());
	CCLabelTTF* label = CCLabelTTF::create(txt->getCString(),"",20);
	label->setColor(ccc3(0,0,0));
	this->addChild(label,12,10);
	setTouchEnabled(true);
}

Block::Block()
{
	isSelected = false;
}

Block::~Block()
{

}

void Block::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	if(!block->isVisible()) return;
	if(block->getOpacity()<255) 
	{ 
		CCLOG("我被点中了：%d",this->getTag());
		isSelected = true;
	}
}
void Block::blockRemove()
{
	if(block->isVisible())
		block->runAction(CCSequence::create(CCBlink::create(0.5f,5),CCToggleVisibility::create(),NULL));
}
void Block::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if(!block->isVisible()) return;
	CCPoint touchLocation = convertTouchToNodeSpace(pTouch); 
	if(Utils::getRect(block).containsPoint(touchLocation)) 
	{ 
		CCLOG("我被点中了：%d",this->getTag());
		block->setOpacity(150);
	} 

}

void Block::refreshTxt()
{
	CCString* txt = CCString::createWithFormat("%i,%i,%i",blockX,blockY,getTag());
	CCLabelTTF* label = (CCLabelTTF*)getChildByTag(10);
	label->setString(txt->getCString());
}

void Block::setBlockPos( int x,int y )
{
	blockX = x;
	blockY = y;
}


