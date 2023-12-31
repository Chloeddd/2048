#ifndef __GameOver_H__
#define __GameOver_H__

#include "cocos2d.h"

USING_NS_CC;

using namespace std;
class GameOver : public cocos2d::Layer
{
private:
public:

	virtual bool init();
	static cocos2d::Scene* scene();

	void onNewGame(Ref* pSender);
	void BacktoMain(Ref* pSender);

	CREATE_FUNC(GameOver);
};


#endif