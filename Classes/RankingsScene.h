#ifndef __RankingsScene_H__
#define __RankingsScene_H__


#include "cocos2d.h"

USING_NS_CC;

using namespace std;
class RankingsScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* scene();
	virtual bool init();

	void BacktoMain(Ref* pSender);

	CREATE_FUNC(RankingsScene);
};


#endif