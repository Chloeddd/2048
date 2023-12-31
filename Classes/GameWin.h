#ifndef __GameWin_H__
#define __GameWin_H__

#include "cocos2d.h"

USING_NS_CC;

using namespace std;
class GameWin : public cocos2d::Layer
{
public:

	virtual bool init();
	static cocos2d::Scene* scene();

	void onNewGame(Ref* pSender);
	void BacktoMain(Ref* pSender);

	CREATE_FUNC(GameWin);
};


#endif