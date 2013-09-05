#include "MainMenuScene.h"
#include "Block.h"

MainMenuScene::MainMenuScene(void)
{
}


MainMenuScene::~MainMenuScene(void)
{
}


bool MainMenuScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	CCSprite* fg = CCSprite::create("fade.png");
	fg->setAnchorPoint(ccp(0,0));
	this->addChild(fg);
	return true;
}

CCScene* MainScene::scene()
{
	CCScene* scene = CCScene::create();
	MainMenuBGLayer* bgLayer = MainMenuBGLayer::create();
	MainMenuScene* fgLayer = MainMenuScene::create();
	scene->addChild(bgLayer);
	//scene->addChild(fgLayer);
	return scene;
}

bool MainMenuBGLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	CCSprite* bgSprite = CCSprite::create("background.png");
	bgSprite->setAnchorPoint(ccp(0.0f,0.0f));
	mBgEffectLayer = createBGEffect();
	mBgEffectLayer->setContentSize(CCSizeMake(640,1136));
	this->addChild(bgSprite);
	this->addChild(mBgEffectLayer,1);
	this->schedule(schedule_selector(MainMenuBGLayer::blockCreator));
	return true;
}
void MainMenuBGLayer::blockCreator(float delta)
{
	if(!mBgEffectLayer)
		return;
	if (mBgEffectLayer->getChildrenCount()<MAX_MENU_BG_STAR_NUM)
	{
		mBgEffectLayer->addChild(createOneStar(),10);
	}
}
CCLayer* MainMenuBGLayer::createBGEffect()
{
	CCLayer* effect = CCLayer::create();
	//time_t* t = new time_t;
	srand(time(NULL)*time(NULL));
	//for(int i =0;i<MAX_MENU_BG_STAR_NUM;i++)
	{
		effect->addChild(createOneStar(),10);
	}
	return effect;
}

Block* MainMenuBGLayer::createOneStar()
{
	int startX,endX,
		startY,endY,
		startLen,endLen,
		startTime,endTime;
	float startScale,endScale;
	startX = 40;
	endX = 640-40;
	startY = 1200;
	endY = 1200;
	startTime = 6;
	endTime = 10;
	startLen = -200;
	endLen = 200;
	startScale = 0.2f;
	endScale = 0.5f;
	int posX = CCRANDOM_0_1()*endX+startX;
	int posY = startY;
	CCLOG("%d,%d",posX,posY);
	int len =  CCRANDOM_0_1()*endLen +startLen;
	float time =  CCRANDOM_0_1()*endTime+startTime;
	float scale =  CCRANDOM_0_1()*endScale+startScale;
	Block* block = Block::create(0,0,0);
	block->setPosition(ccp(posX,posY));
	block->setTouchEnabled(false);
	block->setScale(scale);
	block->runAction(
		CCSequence::create(
			CCMoveTo::create(time,ccp(posX,0)),
			CCCallFunc::create(block,callfunc_selector(Block::removeFromParent)),
			NULL)
	);
	return block;
}