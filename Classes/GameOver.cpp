#include "GameOver.h"
#include "MainScene.h"
#include "GameScene.h"

using namespace cocos2d;

Scene* GameOver::scene()
{
    Scene* scene = Scene::create();
    GameOver* layer = GameOver::create();

    scene->addChild(layer);
    return scene;
}

bool GameOver::init() {

    if (!Layer::init())
    {
        return false;
    }

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();

	auto background = Sprite::create("GameOverBG.png");
	background->setAnchorPoint(Vec2(0, 0));
	background->setPosition(Point(0, 0));

	//自适应大小
	float x = visibleSize.width / background->getContentSize().width;
	float y = visibleSize.height / background->getContentSize().height;
	background->setScale(x, y);

	this->addChild(background);

	Label* Score = Label::createWithTTF(StringUtils::format("Your Score: %d", GameOverScore), "fonts/tahomabd.ttf", 50);
	Score->setColor(Color3B(255, 255, 247));
	Score->setPosition(visibleSize.width / 2, visibleSize.height / 5 * 2);
	this->addChild(Score);

	Sprite* TryAgainNormal = Sprite::create("TryAgainButton.png");

	Sprite* BackNormal = Sprite::create("BackButton.png");

	MenuItemSprite* TryAgain = MenuItemSprite::create(TryAgainNormal, nullptr,
		CC_CALLBACK_1(GameOver::onNewGame, this));

	MenuItemSprite* Back = MenuItemSprite::create(BackNormal, nullptr,
		CC_CALLBACK_1(GameOver::BacktoMain, this));

	TryAgain->setScale(2.0f);
	Back->setScale(2.0f);

	Menu* menu = Menu::create(TryAgain, Back, NULL);
	menu->alignItemsVerticallyWithPadding(15);
	menu->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 240));
	this->addChild(menu);

	return true;
}

void GameOver::onNewGame(Ref* pSender)
{
	Scene* scene = Scene::create();
	scene->addChild(GameScene::create());
	Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
}

void GameOver::BacktoMain(Ref* pSender)
{
	Scene* scene = MainScene::scene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.3f, scene));
}