#include "HelloWorldScene.h"

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
	CCLayer* gameLayer = genCrystalPad();
	gameLayer->setPosition(ccp(50,50));
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
	setTouchEnabled(true);
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
	mBlockLeft = 0;
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	CCLayerRGBA* mGameLayer = CCLayerRGBA::create();
	mGameLayer->setColor(ccc3(20,40,0xa0));
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
	mapW = (w+spriteBg->getContentSize().width)/offX;
	mapH = (h-spriteBg->getContentSize().height/2)/offY;
	spriteBg->release();
	//        mGameLayer->removeAllChildren();
	/*if(mapH%2==0)
		mapH = mapH-1;*/
	int blockId = 0;
	for(int i =0;i<mapH;i++)
	{
		int maxW = mapW-(i%2==0?0:1);
		for (int j =0; j<maxW; j++) {
			CCSprite* spriteBg = CCSprite::create("blocktiles.png");
			spriteBg->setAnchorPoint(ccp(0.0f, 0.0f));

			int start = 0;
			int end = 5;
			float rnd = CCRANDOM_0_1();
			int index = rnd*end+start;
			CCString* file = CCString::createWithFormat("%i.png",index);
			CCSprite* sprite = CCSprite::create(file->getCString());
			sprite->setAnchorPoint(ccp(0.5f, 0.5f));
			int sw = spriteBg->getContentSize().width;
			int sh = spriteBg->getContentSize().height;

			CCLOG("block %d is add",blockId);
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
			mGameLayer->addChild(spriteBg,0,blockId++);
			mGameLayer->addChild(sprite,1,blockId++);
			sprite->setUserData(file);
			file->retain();
			mBlockLeft++;
		}
	}
	return mGameLayer;
}

void HelloWorld::registerWithTouchDispatcher()
{
	 CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-10,true);
}

bool HelloWorld::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if (mBlockLeft<=0)
	{
		CCDirector::sharedDirector()->sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0f,HelloWorld::scene(),false));
	}
	CCLayer* mGameLayer = (CCLayer*)getChildByTag(0);
	if (mGameLayer==NULL)
	{
		return false;
	}
	mSelectBlock = CCArray::createWithCapacity(mGameLayer->getChildrenCount());
	mSelectBlock->retain();

	mSelectBlock = new CCArray(mGameLayer->getChildrenCount());


	for(unsigned int i =1;i < (mGameLayer->getChildrenCount());i=i+2)
	{
		CCSprite* block = (CCSprite*)mGameLayer->getChildByTag(i);
		if (block==NULL) {
			CCLOG("block %d is null",i);
			continue;
		}
		CCPoint winPoint = mGameLayer->convertToNodeSpace(block->getPosition());
		CCRect blockRc = CCRectMake(winPoint.x,winPoint.y, block->getContentSize().width, block->getContentSize().height);
		CCPoint touchPoint = mGameLayer->convertTouchToNodeSpace(pTouch);
		//        CCRect blockRc = CC_RECT_POINTS_TO_PIXELS(block->getPosition());
		if(blockRc.containsPoint(touchPoint))
		{
			if (!block->isVisible())
			{
				CCLOG("block %d is inVisible",i);
				ccTouchEnded(pTouch,pEvent);
				isBeginMove = false;
				return false;
			}
			block->setOpacity(150);
			mCurSelectType = (CCString*)block->getUserData();
			mSelectBlock->addObject(block);
			break;
		}
	}
	isBeginMove = true;
	return true;
}

void HelloWorld::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if(!isBeginMove)
		return;
	CCLayer* mGameLayer = (CCLayer*)getChildByTag(0);
	if (mGameLayer==NULL)
	{
		return;
	}
	CCRect layerRc = CCRectMake(0,0, mGameLayer->getContentSize().width, mGameLayer->getContentSize().height);
	CCPoint touchPoint = mGameLayer->convertTouchToNodeSpace(pTouch);
	if (!layerRc.containsPoint(touchPoint))
	{
		ccTouchEnded(pTouch,pEvent);
		return;
	}
	
	for(unsigned int i =1;i< (mGameLayer->getChildrenCount());i=i+2)
	{
		CCSprite* block = (CCSprite*)mGameLayer->getChildByTag(i);
		//        CCRect blockRc = CC_RECT_POINTS_TO_PIXELS(block->getPosition());
		if (block==NULL) {
			CCLOG("block %d is null",i);
			continue;
		}
		CCPoint winPoint = mGameLayer->convertToNodeSpace(block->getPosition());
		CCRect blockRc = CCRectMake(winPoint.x,winPoint.y, block->getContentSize().width, block->getContentSize().height);

		if(blockRc.containsPoint(touchPoint))
		{
			if (mSelectBlock->containsObject(block))
				return;
			CCString* tempType = (CCString*)block->getUserData();
			if (!block->isVisible()|| tempType->compare(mCurSelectType->getCString()))
			{
				CCLOG("block %d is inVisible",i);
				ccTouchEnded(pTouch,pEvent);
				break;
			}
			if (!mSelectBlock->containsObject(block))
			{
				block->setOpacity(150);
				mSelectBlock->addObject(block);
			}
			//block->setVisible(false);
			/*mBlockLeft--;
			if (mBlockLeft<=0)
			{
			showGameEnd();
			}
			break;*/
		}
	}
}

void HelloWorld::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	if(!isBeginMove)
		return;
	CCObject* block = NULL;
	if(mSelectBlock->count()<3)
	{
		CCARRAY_FOREACH(mSelectBlock, block) // 正向遍历
		{
			((CCSprite*)block)->setOpacity(255);
			// todo with _bullet
		};
		mSelectBlock->removeAllObjects();
		mSelectBlock->release();
		isBeginMove = false;
		return;
	}
	
	CCARRAY_FOREACH(mSelectBlock, block) // 正向遍历
	{
		((CCSprite*)block)->setVisible(false);
		//((CCSprite*)block)->removeFromParent();
		mBlockLeft--;
		// todo with _bullet....

	};
	if (mBlockLeft<=0)
	{
		showGameEnd();
	}
	searchForEmptyNode(mSelectBlock);
	mSelectBlock->removeAllObjects();
	mSelectBlock->release();
	isBeginMove = false;
	return;
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

void HelloWorld::restartScene( CCObject* pSender )
{
	CCDirector::sharedDirector()->sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0f,HelloWorld::scene(),false));
}
CCSprite* HelloWorld::findBlockByPos(float x,float y)
{
	CCLayer* mGameLayer = (CCLayer*)getChildByTag(0);
	CCSprite* block;
	CCPoint tPoint = ccp(x,y);
	for(unsigned int i =1;i< (mGameLayer->getChildrenCount());i=i+2)
	{
		CCSprite* tblock = (CCSprite*)mGameLayer->getChildByTag(i);
		if (block==NULL) {
			CCLOG("block %d is null",i);
			continue;
		}
		CCPoint winPoint = mGameLayer->convertToNodeSpace(block->getPosition());
		CCRect blockRc = CCRectMake(winPoint.x,winPoint.y, block->getContentSize().width, block->getContentSize().height);

		if(blockRc.containsPoint(tPoint))
		{
			block = tblock;
		}
	}
	return block;
}
void HelloWorld::searchForEmptyNode(CCArray* mBlockLeft)
{
	CCLayer* mGameLayer = (CCLayer*)getChildByTag(0);
	CCObject* obj = NULL;
	/*CCARRAY_FOREACH(mSelectBlock,obj)*/
	CCArray* moveBlocks = CCArray::create();
	CCSprite* blockBg = (CCSprite*)mGameLayer->getChildByTag(0);
	int blockH = blockBg->getContentSize().height;
	int moveH = blockH;
	for(int i =0;i<mSelectBlock->count();i++)
	{
		CCSprite* block = (CCSprite*)mSelectBlock->objectAtIndex(i);
		int aboveIndex = block->getTag()+(mapW+mapW-1)*2;
		if (aboveIndex<mGameLayer->getChildrenCount())
		{
			CCSprite* aboveBlock = (CCSprite*) mGameLayer->getChildByTag(aboveIndex);
			if(aboveBlock->isVisible())
			{
				moveBlocks->addObject(block);
				moveBlocks->addObject(aboveBlock);
				CCMoveBy* moveTo = CCMoveBy::create(0.3f,ccp(0.0f,-moveH));
				aboveBlock->runAction(moveTo);
			}
			else
				moveH+=moveH;
		}
	}
}

