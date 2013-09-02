#include "Utils.h"

CCRect Utils::getRect(CCNode* pNode) 
{ 
	CCRect rc; 
	rc.origin = pNode->getPosition(); 
	rc.size = pNode->getContentSize(); 
	rc.origin.x -= rc.size.width*0.5; 
	rc.origin.y -= rc.size.height*0.5; 
	return rc; 
} 