#include "GameData.h"
#include "cocos2d.h"
USING_NS_CC;
static GameData* data = NULL;

GameData::GameData(void)
{
}
void GameData::init()
{
	
}
GameData* GameData::shareData()
{
	if ( !data )
	{
		data = new GameData();
		data->init();
	}

	return data;
}


GameData::~GameData(void)
{
	delete data;
}

int GameData::getScore()
{
	return mScore;
}

void GameData::setScore( int score )
{
	mScore = score;
}

void GameData::addScore( int score )
{
	mScore+=score;
}

const char* GameData::getScoreString()
{
	return (CCString::createWithFormat("%d",mScore)->getCString());
}
