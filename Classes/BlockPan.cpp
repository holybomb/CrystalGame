#include "BlockPan.h"
#include "Block.h"

BlockPan::BlockPan(void)
{
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
	mGameLayer = this;
	mGameLayer->setContentSize(CCSizeMake(600.0f,700.0f));
	CCSize winSize = mGameLayer->getContentSize();
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
	srand((unsigned)time(new time_t()));//重置随机种子
	for(int i =0;i<mapH;i++)
	{
		int maxW = mapW-(i%2==0?0:1);
		for (int j =0; j<maxW; j++) {
			CCSprite* spriteBg = CCSprite::create("blocktiles.png");
			
			int start = 0;
			int end = 5;
			float rnd = CCRANDOM_0_1();
			int index = rnd*end+start;
			CCString* file = CCString::createWithFormat("%i.png",index);
			CCSprite* block = CCSprite::create(file->getCString());
			Block* sprite = Block::create(spriteBg,block,j,i);
			CCLOG("block %d is add",blockId);
			if(i%2!=0)
			{
				spriteBg->setPosition(ccp(offX*j+offX/2+offX/2, offY*i+offY*3/2));
				sprite->setPosition(ccp(offX*j+offX/2+offX/2, offY*i+offY*3/2));
			}
			else
			{
				spriteBg->setPosition(ccp(offX*j+offX/2, offY*i+offY*3/2));
				sprite->setPosition(ccp(offX*j+offX/2, offY*i+offY*3/2));
			}
			mGameLayer->addChild(spriteBg,0,blockId++);
			mGameLayer->addChild(sprite,1);
			sprite->setTag(blockId++);
			sprite->refreshTxt();
			sprite->setUserData(file);
			file->retain();
			mBlockLeft++;
		}
	}
}

void BlockPan::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-1,false);
}

bool BlockPan::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	return true;
}

void BlockPan::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	CCArray* selectBlock = CCArray::create();
	for(unsigned int i =0;i<mGameLayer->getChildrenCount();i+=2)
	{
		Block* block = (Block*)mGameLayer->getChildByTag(i+1);

		if (block!=NULL && block->isSelected)
		{
			block->blockRemove();
			selectBlock->addObject(block);
		}
	}
	CCArray* startMoveBlock = CCArray::create();
	for(unsigned int i =0;i<selectBlock->count();i++)
	{
		Block* block = (Block*)selectBlock->objectAtIndex(i);
		int index = findBlockIDByPos(block->blockX,block->blockY+2);
		if (index<mGameLayer->getChildrenCount())
		{
			blockFallDown(index,block);
			//block->removeFromParent();
		}
	}
}
int BlockPan::findBlockIDByPos(int x,int y)
{
	int index = 0;
	CCLOG("x==%i , y==%i",x,y);
	for(int i =0;i<mapH;i++)
	{
		int maxW = mapW-(i%2==0?0:1);
		for (int j =0; j<maxW; j++) {
			CCLOG("x==%i , y==%i",j,i);
			if (x==j&&y==i)
			{
				return index;
			}
			else
			{
				index+=2;
			}
		}
	}
	return index;
}

void BlockPan::blockFallDown( int index, Block *block )
{
	int line = block->blockY;
	CCArray* tFallDown = CCArray::create();
	Block* blockBase = block; 
	for(int i = line;i<mapH;i+=2)
	{
		Block* blockAbove =(Block*)mGameLayer->getChildByTag(index+1);
		if(blockAbove !=NULL)
		{
			tFallDown->addObject(blockBase);
			//blockAbove->runAction(CCPlace::create(blockBase->getPosition()));
			/*blockAbove->setTag(blockBase->getTag());
			blockAbove->setBlockPos(blockBase->blockX,blockBase->blockY);
			blockAbove->refreshTxt();*/
			index = findBlockIDByPos(blockBase->blockX,i);
			blockBase = blockAbove;
		}
	}
	blockBase = block;
	CCObject* mBlockAbove = NULL;
	CCARRAY_FOREACH(tFallDown,mBlockAbove)
	{
		Block* blockAbove= (Block*)mBlockAbove;
		blockAbove->runAction(CCMoveTo::create(0.3f,blockBase->getPosition()));
		/*blockAbove->setTag(blockBase->getTag());
		blockAbove->setBlockPos(blockBase->blockX,blockBase->blockY);
		blockAbove->refreshTxt();*/
		blockBase = blockAbove;
	}
}
