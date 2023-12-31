#include "SettingScene.h"
#include "MainScene.h"
#include "audio/include/AudioEngine.h"

bool IsMusicOn = true;

Scene* SettingScene::scene()
{
    Scene* scene = Scene::create();
    SettingScene* layer = SettingScene::create();

    scene->addChild(layer);
    return scene;
}

bool SettingScene::init() {
    if (!Layer::init()) {
        return false;
    }

    auto director = Director::getInstance();
    auto visibleSize = director->getVisibleSize();

    auto background = Sprite::create("MainSceneBG.png");
    background->setAnchorPoint(Vec2(0, 0));
    background->setPosition(Point(0, 0));

    //自适应大小
    float x = visibleSize.width / background->getContentSize().width;
    float y = visibleSize.height / background->getContentSize().height;
    background->setScale(x, y);
    this->addChild(background);


    Label* logo = Label::createWithTTF("Settings", "fonts/tahomabd.ttf", 50);
    logo->setColor(Color3B(119, 110, 101));
    logo->setPosition(visibleSize.width / 2, visibleSize.height - 50);
    this->addChild(logo);

    Sprite* BackButtonNor = Sprite::create("BackButton.png");
    MenuItemSprite* BackButton = MenuItemSprite::create(BackButtonNor, nullptr,
        CC_CALLBACK_1(SettingScene::BacktoMain, this));
    BackButton->setScale(1.5f);
    BackButton->setPosition(Vec2(70, visibleSize.height - 50));

    Sprite* MusicIcon = Sprite::create("MusicIcon.png");
    MusicIcon->setScale(1.5f);
    MusicIcon->setPosition(Vec2(visibleSize.width / 2 - 40, visibleSize.height / 2));
    this->addChild(MusicIcon);

    //音乐控制按钮
    Sprite* Music;
    if (IsMusicOn) 
        Music = Sprite::create("PauseNor.png");
    else
        Music = Sprite::create("PauseSel.png");

    MusicButton = MenuItemSprite::create(Music, nullptr,
        CC_CALLBACK_1(SettingScene::onSoundControl, this));
    MusicButton->setScale(1.5f);
    MusicButton->setPosition(Vec2(visibleSize.width / 2 + 40, visibleSize.height / 2));


    auto menu = Menu::create(BackButton,MusicButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);

    return true;
}

void SettingScene::onSoundControl(Ref* pSender)
{
    IsMusicOn = !IsMusicOn;

    if (IsMusicOn) {//开始音乐
        Sprite* newSprite = Sprite::create("PauseNor.png");
        MusicButton->setNormalImage(newSprite);
        AudioEngine::resumeAll();
    }
    else {//暂停音乐
        Sprite* newSprite = Sprite::create("PauseSel.png");
        MusicButton->setNormalImage(newSprite);
        AudioEngine::pauseAll();
    }
}

void SettingScene::BacktoMain(Ref* pSender)
{
    Scene* scene = MainScene::scene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.3f, scene));
}