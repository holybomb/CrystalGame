#include "HelloWorldScene.h"
#include "BlockPan.h"
USING_NS_CC;
USING_NS_CC_EXT;
CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	mSelectBlock = NULL;
	CCSprite* bgSprite = CCSprite::create("background.png");
	bgSprite->setAnchorPoint(ccp(0.0f,0.0f));
    this->addChild(bgSprite);
	BlockPan* gameLayer = BlockPan::create();
	gameLayer->ignoreAnchorPointForPosition(true);
	int w,h;
	w = gameLayer->getContentSize().width;
	gameLayer->setPosition(ccp(320-w/2,40));
	addChild(gameLayer,0,0);
	CCSprite* logoSprite = CCSprite::create("logo.png");
	CCRotateBy* roateAnim = CCRotateBy::create(0.5f,720.0f);
	CCScaleTo* scaleAnim = CCScaleTo::create(0.5f,1.0f);
	logoSprite->setScale(0.1f);
	CCSpawn* spw = CCSpawn::create(roateAnim,scaleAnim,NULL);
	logoSprite->runAction(spw);
	logoSprite->setPosition(ccp(320,900));
	addChild(logoSprite);
	CCMenuItemImage* imageItem = CCMenuItemImage::create("CloseNormal.png","CLoseSelected.png", this,menu_selector(HelloWorld::restartScene));
	CCMenu* menu = CCMenu::create(imageItem,NULL);
	menu->setAnchorPoint(ccp(0,0));
	menu->setPosition(ccp(120,1100));
	addChild(menu);
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void HelloWorld::restartScene( CCObject* pSender )
{
	CCDirector::sharedDirector()->sharedDirector()->replaceScene(HelloWorld::scene());
}


void HelloWorld::showGameEnd()
{
	CCLayer* gameEndLayer = CCLayer::create();
	CCSprite* bgSprite = CCSprite::create("fade.png");
	bgSprite->setAnchorPoint(ccp(0,0));
	gameEndLayer->addChild(bgSprite);
	CCLabelTTF* txt = CCLabelTTF::create("Game Win","fonts/Marker Felt.ttf",40.0f);
	txt->setPosition(ccp(320,568));
	gameEndLayer->addChild(txt);
	this->addChild(gameEndLayer,100,this->getChildrenCount());
}
