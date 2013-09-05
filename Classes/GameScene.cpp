#include "GameScene.h"
#include "BlockPan.h"
#include "GameData.h"
USING_NS_CC;
USING_NS_CC_EXT;
CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	mTime = -3;
	mSelectBlock = NULL;
	CCSprite* bgSprite = CCSprite::create("background.png");
	bgSprite->setAnchorPoint(ccp(0.0f,0.0f));
    this->addChild(bgSprite);
	gameLayer = BlockPan::create();
	gameLayer->ignoreAnchorPointForPosition(true);
	int w,h;
	w = gameLayer->getContentSize().width;
	gameLayer->setAnchorPoint(ccp(0.5f,0));
	gameLayer->setPosition(ccp(0,0));
	addChild(gameLayer,0,0);
	timeBoard = showTimerBoarder();
	timeBoard->setPosition(ccp(0,1136+158));
	timeBoard->runAction(CCMoveTo::create(0.3f,ccp(0,1136-158)));
	addChild(timeBoard);
    return true;
}
void GameScene::countTime(float dt)
{
	mTime++;
	CCLOG("countTime = %d",mTime);
	if(mTime==0)
	{
		pr->setPercentage(0);
		CCProgressTo* to = CCProgressTo::create(TOTAL_GAME_TIME,100);
		pr->setReverseProgress(true);
		pr->runAction(to);
	}
	if (mTime>=TOTAL_GAME_TIME*80/100)
	{
		pr->runAction(CCBlink::create(0.3f,4));
	}
	if (mTime>=TOTAL_GAME_TIME)
	{
		showGameEnd();
		unschedule(schedule_selector(GameScene::countTime));
		gameLayer->setTouchEnabled(false);
	}
}

void GameScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void GameScene::restartScene( CCObject* pSender )
{
	CCDirector::sharedDirector()->sharedDirector()->replaceScene(GameScene::scene());
}
CCLayer* GameScene::showTimerBoarder()
{
	CCLayer* board = CCLayer::create();
	CCSprite* boardBG = CCSprite::create("header.png");
	boardBG->setPosition(ccp(320,0));
	boardBG->setAnchorPoint(ccp(0.5f,0));
	board->addChild(boardBG);
	CCMenuItemImage* imageItem = CCMenuItemImage::create("btn-pause.png","btn-pause-down.png", this,menu_selector(GameScene::restartScene));
	imageItem->setAnchorPoint(ccp(0,1));
	CCMenu* menu = CCMenu::create(imageItem,NULL);
	menu->setAnchorPoint(ccp(0,1));
	menu->setPosition(ccp(0,158));
	pr = CCProgressTimer::create(CCSprite::create("timer.png"));
	pr->setPosition(ccp(320,16));
	pr->setAnchorPoint(ccp(0.5,0));
	pr->setPercentage(100);
	pr->setType(kCCProgressTimerTypeBar);
	pr->setMidpoint(ccp(1,0));
	pr->setBarChangeRate(ccp(1,0));
	this->schedule(schedule_selector(GameScene::countTime),1.0f);

	scoreTxt = CCLabelBMFont::create("0","fonts/scorefont.fnt");
	scoreTxt->setAnchorPoint(ccp(1,0.5f));
	scoreTxt->setScale(2.0f);
	scoreTxt->setAlignment(kCCTextAlignmentRight);
	scoreTxt->setPosition(ccp(640,110));
	board->addChild(scoreTxt,10);
	board->addChild(pr);
	board->addChild(menu);
	return board;
}
void GameScene::showGameEnd()
{
	timeBoard->runAction(CCMoveTo::create(0.3f,ccp(0,1136+158)));
	CCLayer* gameEndLayer = CCLayer::create();
	CCSprite* bgSprite = CCSprite::create("fade.png");
	scoreTxt = CCLabelBMFont::create(GameData::shareData()->getScoreString(),"fonts/scorefont.fnt");
	scoreTxt->setPosition(ccp(320,700));
	scoreTxt->setScale(0.0f);
	CCScaleTo* scaleTo = CCScaleTo::create(0.9f,5);
	CCEaseElasticInOut* scoreEff = CCEaseElasticInOut::create(scaleTo);
	scoreTxt->runAction(scoreEff);
	if(!scoreTxt->getParent())
		gameEndLayer->addChild(scoreTxt);
	bgSprite->setAnchorPoint(ccp(0,0));
	gameEndLayer->addChild(bgSprite);
	CCLabelTTF* txt = CCLabelTTF::create("Game End","fonts/Marker Felt.ttf",40.0f);
	txt->setPosition(ccp(320,568));

	gameEndLayer->addChild(txt);
	CCMenuItemImage* imageItem = CCMenuItemImage::create("CloseNormal.png","CLoseSelected.png", this,menu_selector(GameScene::restartScene));
	CCMenu* menu = CCMenu::create(imageItem,NULL);
	menu->setAnchorPoint(ccp(0,0));
	menu->setPosition(ccp(320,300));
	imageItem->setScale(3.0f);
	gameLayer->runAction(CCScaleTo::create(0.5,0));
	gameEndLayer->addChild(menu);
	this->addChild(gameEndLayer,100,this->getChildrenCount());
}
void GameScene::updateScore(float dt)
{
	scoreTxt->setCString(CCString::createWithFormat("%d",GameData::shareData()->getScore())->getCString());
}
GameScene::GameScene()
{

}
