#include "MainScene.h"
#include "GameScene.h"
#include "RankingsScene.h"
#include "SettingScene.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d;

Scene* MainScene::scene()
{
    Scene* scene = Scene::create();

    MainScene* layer = MainScene::create();

    scene->addChild(layer);
    return scene;
}

bool MainScene::init() {

    if (!Layer::init())
    {
        return false;
    }

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	auto origin = director->getVisibleOrigin();

	auto background = Sprite::create("MainSceneBG.png");
	background->setAnchorPoint(Vec2(0, 0));
	background->setPosition(Point(0, 0));

	//自适应大小
	float x = visibleSize.width / background->getContentSize().width;
	float y = visibleSize.height / background->getContentSize().height;
	background->setScale(x, y);

	this->addChild(background);

	Label* logo = Label::createWithTTF("2048", "fonts/tahomabd.ttf", 150);
	logo->setColor(Color3B(119, 110, 101));
	logo->setPosition(visibleSize.width / 2, visibleSize.height/5*4);
	this->addChild(logo);

	Sprite* newGameNormal = Sprite::create("NewGameButton.png");

	Sprite* gameSettingsNormal = Sprite::create("SettingsButton.png");

	Sprite* RankingsNormal = Sprite::create("RankingButton.png");

	MenuItemSprite* newGame = MenuItemSprite::create(newGameNormal,nullptr,
		CC_CALLBACK_1(MainScene::onNewGame, this));

	MenuItemSprite* gameSetting = MenuItemSprite::create(gameSettingsNormal, nullptr,
		CC_CALLBACK_1(MainScene::onSettings, this));

	MenuItemSprite* Rankings = MenuItemSprite::create(RankingsNormal, nullptr,
		CC_CALLBACK_1(MainScene::onRankings, this));

	newGame->setScale(2.0f);
	gameSetting->setScale(2.0f);
	Rankings->setScale(2.0f);

	Menu* menu = Menu::create(newGame, gameSetting, Rankings, NULL);
	menu->alignItemsVerticallyWithPadding(15);
	menu->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 140));
	this->addChild(menu);

	if (IsMusicOn)
	{
		AudioEngine::pauseAll();
		auto mainMusicAudio = AudioEngine::play2d("BGM.mp3");
		AudioEngine::setVolume(mainMusicAudio, 0.3f);
	}

	return true;
}

void MainScene::onNewGame(Ref* pSender)
{
	Scene* scene = Scene::create();
	scene->addChild(GameScene::create());
	Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
}

void MainScene::onSettings(Ref* pSender)
{
	Scene* scene = SettingScene::scene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
}

void MainScene::onRankings(Ref* pSender)
{
	Scene* scene = RankingsScene::scene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
}