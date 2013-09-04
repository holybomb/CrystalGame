#include "BlockPan.h"
#include "Block.h"
#include "Utils.h"
int BlockPan::mCurSelectType = -1;
CCArray* selectBlock = NULL;
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
	this->setOpacity(150);
	this->setColor(ccc3(0,0,0));
	this->setTouchEnabled(true);
	return true;
}

void BlockPan::createBlockPan()
{
	mBlockLeft = 0;
	BlockPan::mCurSelectType = -1;
	selectBlock = CCArray::create();
	selectBlock->retain();
	mGameLayer = CCLayerColor::create();
	CCLayerColor* mGameLayerBG = CCLayerColor::create();
	this->setContentSize(CCSizeMake(600.0f,700.0f));
	mGameLayerBG->setContentSize(this->getContentSize());
	mGameLayer->setContentSize(this->getContentSize());
	this->addChild(mGameLayerBG);
	this->addChild(mGameLayer);
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
		int maxW = mapW-(i%2==0?0:1);
		int col =0;
		if(i%2!=0)
			col = 1;
		for (int j =0; j<maxW; j++) {
			CCSprite* spriteBg = CCSprite::create("blocktiles.png");
			Block* sprite = createNewBlock(j,i,col);
			spriteBg->setPosition(sprite->getPosition());
			sprite->setPosition(ccp(sprite->getPositionX(),sprite->getPositionY()+80));
			CCMoveTo* moveTo = CCMoveTo::create(0.5f,spriteBg->getPosition());
			CCEaseSineInOut * easeIn = CCEaseSineInOut::create(moveTo);
			sprite->runAction(easeIn);
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
}