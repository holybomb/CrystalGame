#include "Block.h"
#include "BlockPan.h"
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
		if (BlockPan::mCurSelectType<0)
		{
			BlockPan::mCurSelectType = blockType;
		}
		CCLOG("began ：%d",this->getTag());
		block->setOpacity(150);
		isSelected = true;
	} 

	return true;
}

Block* Block::create(int x,int y,int col)
{
	Block *testSprite= new Block();
	if(testSprite&&testSprite->init()){

		testSprite->autorelease();

		testSprite->customInit(x,y,col);//
		return testSprite;

	}

	CC_SAFE_DELETE(testSprite);

	return testSprite;


}

void Block::customInit(int x,int y,int col)
{
	int start = 0;
	int end = 4;
	float rnd = CCRANDOM_0_1();
	int index = rnd*end+start;
	CCString* file = CCString::createWithFormat("%i.png",index);
	blockType = index;
	block = CCSprite::create(file->getCString());
	blockX = x;
	blockY = y;
	this->col = col;
//	this->addChild(pBgSprite,-1,0);
	this->setContentSize(block->getContentSize());
	this->addChild(block,11,0);
	CCString* txt = CCString::createWithFormat("%i,%i,%i",blockX,blockY,col);
	mLabel = CCLabelTTF::create(txt->getCString(),"",20);
	mLabel->setColor(ccc3(0,0,0));
	if(	CCDirector::sharedDirector()->isDisplayStats())
		this->addChild(mLabel,12,BLOCK_LABEL_TAG);
	setTouchEnabled(true);
}

Block::Block()
{
	isSelected = false;
	isRemoved = false;
}

Block::~Block()
{

}

void Block::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	if(isRemoved) return;
	if(block->getOpacity()<255) 
	{ 
		CCLOG("我被点中了：%d",this->getTag());
		
	}
}
void Block::blockRemove()
{
	if(!isRemoved)
	{
		//BlockPan* pan = (BlockPan*)getParent();
		//if(pan)
		//CCParticleSystem *meteor=CCParticleSystemQuad::create("particles/taken-gem.plist
		{
			this->removeFromParentAndCleanup(true);
			//block->runAction(CCCallFunc::create(this,callfunc_selector(Block::removeFromParent)));
		}
		isRemoved = true;
	}
}
void Block::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if(isRemoved) return;
	CCPoint touchLocation = convertTouchToNodeSpace(pTouch); 
	if (BlockPan::mCurSelectType != blockType)
	{
		block->setOpacity(255);
		return;
	}
	BlockPan* pan = (BlockPan*)(getParent()->getParent()->getParent());
	if(pan)
	{
		if(pan->selectBlock->containsObject(this))
			return;
		if( pan->selectBlock &&  pan->selectBlock->count()>0)
		{
			Block* temp = (Block*) pan->selectBlock->lastObject();
			if(abs(temp->col-this->col)>1 || abs(temp->blockY-this->blockY)>2)
			{
				block->setOpacity(255);
				return;
			}
		}
	}
	if(Utils::getRect(block).containsPoint(touchLocation)) 
	{ 
		CCLOG("be choose ：%d",this->getTag());
		block->setOpacity(150);
		pan->selectBlock->addObject(this);
		isSelected = true;
	}
}

void Block::refreshTxt()
{
	CCString* txt = CCString::createWithFormat("%i,%i,%i",blockX,blockY,col);
	if(	!CCDirector::sharedDirector()->isDisplayStats())
		return;
	if(mLabel!=NULL && mLabel->getParent())
		mLabel->setString(txt->getCString());
}

void Block::setBlockPos( int x,int y )
{
	blockX = x;
	blockY = y;
	refreshTxt();
}

void Block::setBlockPos(CCObject* obj)
{
	BlockPos* p = (BlockPos*)obj;
	setBlockPos(p->x,p->y);
}


BlockPos* BlockPos::create( CCPoint p )
{
	BlockPos* pos = new BlockPos();
	if(pos){
		pos->autorelease();
		pos->x = p.x;
		pos->y = p.y;
		return pos;
	}
	CC_SAFE_DELETE(pos);

	return pos;

}
