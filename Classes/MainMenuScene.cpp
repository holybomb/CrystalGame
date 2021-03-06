#include "MainMenuScene.h"
#include "Block.h"
#include "GameScene.h"

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
	//this->addChild(fg);
	logo = CCSprite::create("logo.png");
	logo->setPosition(ccp(320,900));
	CCScaleTo* scaleBig = CCScaleTo::create(0.5f,1);
	CCScaleTo* scaleSmall = CCScaleTo::create(0.0001f,0);
	CCEaseBounceOut* eff = CCEaseBounceOut::create(scaleBig);
	eff->bounceTime(3);
	CCSequence* act = CCSequence::create(CCHide::create(),scaleSmall,CCShow::create(),eff,NULL);
	logo->setVisible(false);
	logo->runAction(act);

	CCMenuItemImage* playBtn = CCMenuItemImage::create("btn-play.png","btn-play-down.png",this,menu_selector(MainMenuScene::onPlayBtn));
	CCMenuItemImage* aboutBtn = CCMenuItemImage::create("btn-about.png","btn-about-down.png",this,menu_selector(MainMenuScene::onAboutBtn));
	CCMenuItemImage* doneBtn = CCMenuItemImage::create("btn-done.png","btn-done-down.png",this,menu_selector(MainMenuScene::onDoneBtn));
	mainMenu = CCMenu::create(playBtn,aboutBtn,/*doneBtn,*/NULL);;
	int offY = 0;

	CCObject* obj = NULL;
	int i =1;
	CCARRAY_FOREACH(mainMenu->getChildren(),obj)
	{
		((CCNode* )obj)->setVisible(false);
		((CCNode* )obj)->runAction(CCSequence::create(CCDelayTime::create(0.2*i),(CCAction*)act->copy(),NULL));
		i++;
	}
	
	addChild(mainMenu);
	mainMenu->alignItemsVerticallyWithPadding(offY);
	mainMenu->setAnchorPoint(ccp(0.5,0.5));
	mainMenu->setPosition(ccp(320,450));
	addChild(logo);
	CCLabelBMFont* highScoreTxt = CCLabelBMFont::create(GameData::shareData()->getHighScoreString(),"fonts/scorefont.fnt");
	highScoreTxt->setPosition(ccp(320,100));
	addChild(highScoreTxt);
	CCLabelTTF* highScoreHint = CCLabelTTF::create("High Socre","",30);
	addChild(highScoreHint);
	highScoreHint->setPosition(ccp(320,50));
	return true;
}
void MainMenuScene::onPlayBtn(CCObject* pSender)
{
	CCObject* obj = NULL;
	int i =1;
	CCScaleTo* act = CCScaleTo::create(0.3f,0);
	logo->runAction(act);
	CCARRAY_FOREACH(mainMenu->getChildren(),obj)
	{
		((CCNode* )obj)->runAction((CCAction*)act->copy());
	}
	this->runAction(
		CCSequence::create(
			CCDelayTime::create(0.3f),
			CCCallFunc::create(
				this,
				callfunc_selector(MainMenuScene::startPlay)
			),
			NULL
		)
	);
}

void MainMenuScene::startPlay()
{
	CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}
void MainMenuScene::onDoneBtn( CCObject* pSender )
{
	CCDirector::sharedDirector()->end();
	exit(0);
}

void MainMenuScene::onAboutBtn( CCObject* pSender )
{
	mainMenu->setEnabled(false);
	CCScaleTo* scaleBig = CCScaleTo::create(0.5f,1);
	CCScaleTo* scaleSmall = CCScaleTo::create(0.0001f,0);
	CCEaseBounceOut* eff = CCEaseBounceOut::create(scaleBig);
	eff->bounceTime(3);
	CCSequence* act = CCSequence::create(CCHide::create(),scaleSmall,CCShow::create(),eff,NULL);
	aboutLayer = showAboutLayer();
	aboutLayer->setVisible(false);
	aboutLayer->runAction(act);
	addChild(aboutLayer);
}
CCLayer* MainMenuScene::showAboutLayer()
{
	CCLayer* about = CCLayer::create();
	CCSprite*bg = CCSprite::create("about-bg.png");
	CCLabelTTF* aboutTxt = CCLabelTTF::create("Crystal Craze is created with CocosBuilder and cocos2d. It is released as open source under MIT license. Download the full source:\n https://github.com/vlidholt/CrystalCraze \nDownload CocosBuilder from: \nhttp://cocosbuilder.com \nBackground image is creative commons attribution: NASA, ESA, R. O'Connell (University of Virginia), F. Paresce (National Institute for Astrophysics, Bologna, Italy), E. Young (Universities Space Research Association/Ames Research Center), the WFC3 Science Oversight Committee, and the Hubble Heritage Team (STScI/AURA ","font/Marker Felt.ttf",30);
	aboutTxt->setPosition(ccp(40,690));
	aboutTxt->setHorizontalAlignment(kCCTextAlignmentLeft);
	aboutTxt->setVerticalAlignment(kCCVerticalTextAlignmentTop);
	aboutTxt->setAnchorPoint(ccp(0,1));
	aboutTxt->setColor(ccc3(255,255,255));
	aboutTxt->setDimensions(CCSizeMake(440,750));
	bg->addChild(aboutTxt);
	bg->setPosition(ccp(320,568));
	about->addChild(bg);
	CCMenuItemImage* doneBtn = CCMenuItemImage::create("btn-done.png","btn-done-down.png",this,menu_selector(MainMenuScene::onAboutDoneBtn));
	CCMenu* menu = CCMenu::create(doneBtn,NULL);
	menu->setPosition(ccp(320,230));
	menu->alignItemsVertically();
	about->addChild(menu);
	return about;
}
void MainMenuScene::onAboutDoneBtn(CCObject* pSender)
{
	CCScaleTo* scaleSmall = CCScaleTo::create(0.2f,0);
	CCSequence* act = CCSequence::create(scaleSmall,CCRemoveSelf::create(true),NULL);
	aboutLayer->runAction(scaleSmall);
	mainMenu->setEnabled(true);
}
CCScene* MainScene::scene()
{
	CCScene* scene = CCScene::create();
	MainMenuBGLayer* bgLayer = MainMenuBGLayer::create();
	MainMenuScene* fgLayer = MainMenuScene::create();
	scene->addChild(bgLayer);
	scene->addChild(fgLayer);
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
	mBgParticleLayer = CCLayer::create();
	CCParticleSystem* tempSystem = CCParticleSystemQuad::create("particles/bg-stars.plist");
	int posX = CCRANDOM_0_1()*640;
	int posY = CCRANDOM_0_1()*1136;
	tempSystem->setPosition(ccp(posX,posY));
	tempSystem->setScale(2);
	tempSystem->setAutoRemoveOnFinish(true);
	mBgParticleLayer->addChild(tempSystem);
	this->addChild(mBgParticleLayer,1);
	this->addChild(mBgEffectLayer,1);
	this->schedule(schedule_selector(MainMenuBGLayer::blockCreator),1);
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
	startTime = 7;
	endTime = 10;
	startLen = -200;
	endLen = 200;
	startScale = 0.2f;
	endScale = 0.5f;
	int posX = CCRANDOM_0_1()*endX+startX;
	int posY = startY;
	//CCLOG("%d,%d",posX,posY);
	int len =  CCRANDOM_0_1()*endLen +startLen;
	float time =  CCRANDOM_0_1()*endTime+startTime;
	float scale =  CCRANDOM_0_1()*endScale+startScale;
	Block* block = Block::create(0,0,0);
	block->setPosition(ccp(posX,posY));
	block->setTouchEnabled(false);
	block->setAnchorPoint(ccp(0,0));
	block->setScale(scale);
	block->runAction(
		CCSequence::create(
			CCMoveTo::create(time,ccp(posX,0)),
			CCCallFunc::create(block,callfunc_selector(Block::removeFromParent)),
			NULL)
	);
	return block;
}