#ifndef __SettingScene_H__
#define __SettingScene_H__


#include "cocos2d.h"

USING_NS_CC;

extern bool IsMusicOn;

using namespace std;
class SettingScene : public cocos2d::Layer
{
private:
	MenuItemSprite* MusicButton;
public:

	static cocos2d::Scene* scene();
	virtual bool init();

	void onSoundControl(Ref* pSender);
	void BacktoMain(Ref* pSender);

	CREATE_FUNC(SettingScene);
};


#endif