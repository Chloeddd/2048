#ifndef __MainScene_H__
#define __MainScene_H__


#include "cocos2d.h"

USING_NS_CC;

using namespace std;
class MainScene : public cocos2d::Layer
{
public:

	virtual bool init();
	static cocos2d::Scene* scene();

	void onNewGame(Ref* pSender);
	void onSettings(Ref* pSender);
	void onRankings(Ref* pSender);

	CREATE_FUNC(MainScene);
};


#endif