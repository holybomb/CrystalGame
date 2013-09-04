#pragma once
#include "cocos2d.h"
USING_NS_CC;
class GameData
{
private:
	int mScore;
public:
	GameData(void);
	~GameData(void);
	static GameData* shareData();
	void init();
	int getScore();
	void setScore(int score);
	void addScore(int score);
	const char* getScoreString();
};

