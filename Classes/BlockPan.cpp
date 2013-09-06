#include "BlockPan.h"
#include "Block.h"
#include "GameScene.h"
#include "Utils.h"
#include "GameData.h"
int BlockPan::mCurSelectType = -1;
BlockPan::BlockPan(void)
{
	isFallDown = false;
}


BlockPan::~BlockPan(void)
{
}

bool BlockPan::init()
{
	if (!CCLayerColor::init())
	{
		return false;
	}
	createBlockPan();
	//this->setOpacity(150);
	//this->setColor(ccc3(0,0,0));
	this->setTouchEnabled(true);
	return true;
}

void BlockPan::createBlockPan()
{
	mBlockLeft = 0;
	GameData::shareData()->setScore(0);
	BlockPan::mCurSelectType = -1;
	selectBlock = CCArray::create();
	selectBlock->retain();
	mGameLayer = CCLayerColor::create();
	mGameLayerBG = CCLayerColor::create();
	CCSize size = CCSizeMake(630.0f,900.0f);
	this->setContentSize(size);
	mGameLayerBG->setContentSize(size);
	mGameLayer->setContentSize(size);
	CCClippingNode* clipper = CCClippingNode::create();
	clipper->addChild(mGameLayerBG);
	clipper->setContentSize(size);
	clipper->addChild(mGameLayer);
	clipper->setStencil(mGameLayerBG);
	this->addChild(clipper);
	//mGameLayerBG->setContentSize(CCSizeMake(600.0f,700.0f));
	//mGameLayer->setContentSize(CCSizeMake(600.0f,700.0f));
	CCSize winSize = mGameLayer->getContentSize();
	float x = mGameLayer->getPositionX();
	float y = mGameLayer->getPositionY();
	float w = winSize.width;
	float h = winSize.height;
	CCSprite* spriteBg = CCSprite::create("blocktiles.png");
	offX = spriteBg->getContentSize().width+spriteBg->getContentSize().width/2;//w/GAME_LAYER_SIZE_W;
	offY = spriteBg->getContentSize().height/2;//h/GAME_LAYER_SIZE_H;
	mapW = (w+spriteBg->getContentSize().width)/offX;
	mapH = (h-spriteBg->getContentSize().height/2)/offY;
	spriteBg->release();
	//        mGameLayer->removeAllChildren();
	/*if(mapH%2==0)
		mapH = mapH-1;*/
	int blockId = 0;
	srand((unsigned)time(new time_t()));//重置随机种子
	for(int i =0;i<mapH;i++)
	{
		int maxW = mapW;//-(i%2==0?0:1);
		int col =0;
		if(i%2!=0)
			col = 1;
		for (int j =0; j<maxW; j++) {
			CCSprite* spriteBg = CCSprite::create("blocktiles.png");
			Block* sprite = createNewBlock(j,i,col);
			spriteBg->setPosition(sprite->getPosition());
			spriteBg->setOpacity(100);
			sprite->setPosition(ccp(sprite->getPositionX(),900+80));
			CCMoveTo* moveTo = CCMoveTo::create(0.5f,spriteBg->getPosition());
			CCEaseSineInOut * easeIn = CCEaseSineInOut::create(moveTo);
			sprite->runAction(CCSequence::create(CCDelayTime::create(0.1f*i),easeIn,NULL));
			mGameLayerBG->addChild(spriteBg,0,0);
			mGameLayer->addChild(sprite,1);
			mBlockLeft++;
			col+=2;
		}
	}
}

void BlockPan::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-1,false);
}

bool BlockPan::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(isFallDown) return false;
	if(selectBlock)
		selectBlock->removeAllObjects();
	return true;
}
void BlockPan::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if(isFallDown) return;
	CCObject* obj = NULL;
	CCArray* blocks = mGameLayer->getChildren();
	/*CCARRAY_FOREACH(blocks,obj)
	{
		Block* block = (Block*)obj;
		if (selectBlock->containsObject(block))
		{
			return;
		}
		if (block!=NULL && block->isSelected && !block->isRemoved)
		{
			selectBlock->addObject(block);
		}
	}*/
}
void BlockPan::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	CCObject* obj = NULL;
	CCArray* blocks = mGameLayer->getChildren();
	selectBlock->removeAllObjects();
	CCARRAY_FOREACH(blocks,obj)
	{
		Block* block = (Block*)obj;
		if (selectBlock->containsObject(block))
		{
			return;
		}
		if (block!=NULL && block->isSelected && !block->isRemoved)
		{
			selectBlock->addObject(block);
			int temp = PER_BLOCK_SCORE*selectBlock->count();
			GameData::shareData()->addScore(temp);
		}
	}
	BlockPan::mCurSelectType = -1;
	if(selectBlock->count()<3)
	{
		CCARRAY_FOREACH(selectBlock,obj)
		{
			Block* block = (Block*)obj;
			block->block->setOpacity(255);
			block->isSelected = false;
		}
		isFallDown = false;
		return;
	}
	isFallDown = true;
	CCLOG("block %d is select",selectBlock->count());
	obj = NULL;
	CCARRAY_FOREACH(selectBlock,obj)
	{
		Block* block = (Block*)obj;	
		int lastLine = findLastLineByCol(block->col);
		Block* newBlock = createNewBlock(block->blockX,lastLine+2,block->col);
		mGameLayer->addChild(newBlock,1);
	}
	blocksRemove();
}
Block* BlockPan::createNewBlock(int x,int y,int col)
{
	BlockPos* blockPos = ccb(x,y);
	int i = blockPos->y;
	int j = blockPos->x;
	CCPoint pos;
	if(i%2!=0)
	{
		pos = (ccp(offX*j+offX/2+offX/2, offY*i+offY*3/2));
	}
	else
	{
		pos = (ccp(offX*j+offX/2, offY*i+offY*3/2));
	}
	Block* newBlock = Block::create(j,i,col);
	newBlock->setPosition(pos);
	return newBlock;
}
int BlockPan::findLastLineByCol(int col)
{
	int line = 0;
	CCArray* blocks = mGameLayer->getChildren();
	CCObject* obj = NULL;
	CCARRAY_FOREACH(blocks,obj)
	{
		Block* block = (Block*)obj;
		if (block->col==col)
		{
			if (line<block->blockY)
			{
				line = block->blockY;
			}
		}
	}
	return line;
}

Block* BlockPan::findBlockByPos(int x,int y)
{
	int index = 0;
	CCArray* blocks = mGameLayer->getChildren();
	CCObject* obj = NULL;
	CCARRAY_FOREACH(blocks,obj)
	{
		Block* block = (Block*)obj;
		if (block->blockX==x && block->blockY==y)
		{
			return block;
		}
	}
	return NULL;
}
void BlockPan::blocksRemove()
{
	CCObject* obj = NULL;
	CCArray* spwArray = CCArray::create();
	CCARRAY_FOREACH(selectBlock,obj)
	{
		Block* block = (Block*)obj;

		CCParticleSystem *meteor=CCParticleSystemQuad::create("particles/taken-gem.plist");
		this->addChild(meteor);
		meteor->setScale(2);
		meteor->setPosition(block->getPosition());
		block->blockRemove();
		if(block)
			blockFallDown(block);
	}

}
void BlockPan::blockFallDown( CCObject *obj )
{
	setTouchEnabled(false);
	Block* block = (Block*)obj;
	CCLOG("fall block is %i,%i",block->blockX,block->blockY);
	int lastLine = findLastLineByCol(block->col);
	int line = block->blockY+2;
	CCArray* tFallDown = CCArray::create();
	Block* blockBase = block; 
	for(unsigned int i = line;i<=lastLine;i+=2)
	{
		Block* blockAbove =(Block*)findBlockByPos(block->blockX,i);
		if(blockAbove !=NULL && !blockAbove->isRemoved)
		{
			tFallDown->addObject(blockAbove);

			//blockAbove->runAction(CCPlace::create(blockBase->getPosition()));
			blockBase = blockAbove;
		}
	}
	blockBase = block;
	CCObject* mBlockAbove = NULL;
	CCARRAY_FOREACH(tFallDown,mBlockAbove)
	{
		Block* blockAbove= (Block*)mBlockAbove;
		CCMoveTo* moveTo = CCMoveTo::create(0.1f,blockBase->getPosition());
		CCEaseSineInOut * easeIn = CCEaseSineInOut::create(moveTo);
		BlockPos* pos = ccb(blockAbove->blockX,blockAbove->blockY-2);
		blockAbove->setBlockPos(blockAbove->blockX,blockAbove->blockY-2);
		CCCallFunc* callF = CCCallFunc::create(this,callfunc_selector(BlockPan::moveIsDone));
		CCSequence* seq = CCSequence::create(CCDelayTime::create(0.2f),easeIn,callF,NULL);
		blockAbove->runAction(seq);
		//blockAbove->setBlockPos(blockAbove->blockX,blockAbove->blockY-2);
		//blockAbove->refreshTxt();
		blockBase = blockAbove;
	}
}
void BlockPan::moveIsDone()
{
	isFallDown = false;
	BlockPan::mCurSelectType = -1;
	setTouchEnabled(true);
	GameScene* hello = (GameScene*)getParent();
	hello->updateScore(0);
}

void BlockPan::setTouchEnabled( bool value )
{
	CCLayerColor::setTouchEnabled(value);
	CCObject* obj = NULL;
	if(mGameLayer)
	{
		CCArray* blocks = mGameLayer->getChildren();
		CCARRAY_FOREACH(blocks,obj)
		{
			Block* block = (Block*)obj;
			block->setTouchEnabled(value);
		}
	}
}

void BlockPan::showGameEnd()
{
	if(mGameLayer)
	{
		CCObject* obj = NULL;
		CCArray* blocks = mGameLayer->getChildren();
		srand(time(new time_t));
		CCARRAY_FOREACH(blocks,obj)
		{
			Block* block = (Block*)obj;
			block->setTouchEnabled(false);
			float offBlockX = CCRANDOM_MINUS1_1()*320.0f;
			float len = CCRANDOM_0_1()*200.0f;
			float time = CCRANDOM_0_1()+0.6;
			ccBezierConfig bezier;
			bezier.controlPoint_1 = block->getPosition();
			int targetX = block->getPositionX()+offBlockX;
			CCLOG("offx = %f",offBlockX);
			bezier.controlPoint_2 = ccp(block->getPositionX()+(targetX-block->getPositionX())/2,block->getPositionY()+len);
			bezier.endPosition = ccp(targetX,block->getPositionY()-1136);
			CCBezierTo* bez = CCBezierTo::create(time,bezier);
			//CCEaseSineIn * eff = CCEaseSineIn ::create(bez);
			CCJumpTo* eff = CCJumpTo::create(time,ccp(targetX,block->getPositionY()-1136),500.0f,1);
			block->runAction(CCSequence::create(eff,NULL));
		}
	}
}
void BlockPan::update(float delta)
{

}
void BlockPan::setStatus(int status)
{
	mStatus = status;
}
void BlockPan::setStatusToInit()
{
	setStatus(BLOCK_PAN_STATUS_INIT);
}

void BlockPan::setStatusToRun()
{
	setStatus(BLOCK_PAN_STATUS_RUN);
}
void BlockPan::setStatusToShowEnd()
{
	setStatus(BLOCK_PAN_STATUS_SHOW_END);
}
void BlockPan::setStatusToShowEndOver()
{
	setStatus(BLOCK_PAN_STATUS_SHOW_END_OVER);
}