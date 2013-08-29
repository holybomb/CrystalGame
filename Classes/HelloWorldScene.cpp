#include "HelloWorldScene.h"

USING_NS_CC;

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
	CCSprite* bgSprite = CCSprite::create("background.png");
	bgSprite->setAnchorPoint(ccp(0.0f,0.0f));
    this->addChild(bgSprite);
	CCLayer* gameLayer = CCLayer::create();
	addChild(genCrystalPad());
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

CCLayer* HelloWorld::genCrystalPad()
{
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	CCLayer* mGameLayer = CCLayer::create();
	mGameLayer->ignoreAnchorPointForPosition(true);
	mGameLayer->setPosition(0,0);
	mGameLayer->setContentSize(CCSizeMake(640.0f,600.0f));
	winSize = mGameLayer->getContentSize();
	float x = mGameLayer->getPositionX();
	float y = mGameLayer->getPositionY();
	float w = winSize.width;
	float h = winSize.height;
	CCSprite* spriteBg = CCSprite::create("blocktiles.png");
	float offX = spriteBg->getContentSize().width+spriteBg->getContentSize().width/2;//w/GAME_LAYER_SIZE_W;
	float offY = spriteBg->getContentSize().height/2;//h/GAME_LAYER_SIZE_H;
	int mapW = (w+offX)/offX;
	int mapH = (h-offY)/offY;
	spriteBg->release();
	//        mGameLayer->removeAllChildren();
	if(mapH%2==0)
		mapH = mapH-1;
	for(int i =0;i<mapH;i++)
	{
		int maxW = mapW-(i%2==0?0:1);
		for (int j =0; j<maxW; j++) {
			CCSprite* spriteBg = CCSprite::create("blocktiles.png");
			spriteBg->setAnchorPoint(ccp(0.0f, 0.0f));

			int start = 0;
			int end = 5;
			int index = CCRANDOM_0_1()*end+start;
			CCString* file = CCString::createWithFormat("%i.png",index);
			CCSprite* sprite = CCSprite::create(file->getCString());
			sprite->setAnchorPoint(ccp(0.5f, 0.5f));
			int sw = spriteBg->getContentSize().width;
			int sh = spriteBg->getContentSize().height;

			//                CCLOG("block %d is add",j+mapW*i);
			if(i%2!=0)
			{
				spriteBg->setPosition(ccp(offX*j+offX/2, offY*i));
				sprite->setPosition(ccp(offX*j+offX/2+sw/2,offY*i+sh/2));
			}
			else
			{
				spriteBg->setPosition(ccp(offX*j, offY*i));
				sprite->setPosition(ccp(offX*j+sw/2,offY*i+sh/2));
			}
			mGameLayer->addChild(spriteBg,0,-1);
			mGameLayer->addChild(sprite,1,(j+mapH*i));
		}
	}
	return mGameLayer;
}
