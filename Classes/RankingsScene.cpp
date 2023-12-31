#include "RankingsScene.h"
#include "MainScene.h"

Scene* RankingsScene::scene()
{
    Scene* scene = Scene::create();
    RankingsScene* layer = RankingsScene::create();

    scene->addChild(layer);
    return scene;
}

bool RankingsScene::init() {
    if (!Layer::init()) {
        return false;
    }

    auto director = Director::getInstance();
    auto visibleSize = director->getVisibleSize();

    auto background = Sprite::create("MainSceneBG.png");
    background->setAnchorPoint(Vec2(0, 0));
    background->setPosition(Point(0, 0));

    //����Ӧ��С
    float x = visibleSize.width / background->getContentSize().width;
    float y = visibleSize.height / background->getContentSize().height;
    background->setScale(x, y);
    this->addChild(background);


    Label* logo = Label::createWithTTF("Rankings", "fonts/tahomabd.ttf", 50);
    logo->setColor(Color3B(119, 110, 101));
    logo->setPosition(visibleSize.width / 2, visibleSize.height -50);
    this->addChild(logo);

    Sprite* BackButtonNor = Sprite::create("BackButton.png");
    MenuItemSprite* BackButton = MenuItemSprite::create(BackButtonNor, nullptr,
        CC_CALLBACK_1(RankingsScene::BacktoMain, this));
    BackButton->setScale(1.5f);
    Menu* Menu = Menu::create(BackButton, NULL);
    Menu->setPosition(Vec2(70, visibleSize.height-50));
    this->addChild(Menu);

    //��ȡ���а񣬴�������
    int t;
    std::vector<int> AllScores;
    UserDefault* userDefault = UserDefault::getInstance();
    for (int i = 1; i <= 10; i++) {//��ȡǰʮ�����趨ֻ��¼ǰʮ�����а�
        std::string Rank = "Rank" + std::to_string(i);
        t = userDefault->getIntegerForKey(Rank.c_str(), -1);
        if (t == -1)
            break;//�洢����ʮ��
        AllScores.push_back(t);
    }

    for (int j = 0; j < AllScores.size(); j++) {
        std::string Rank = "Rank" + std::to_string(j + 1);
        Label* RANK = Label::createWithTTF(Rank, "fonts/tahomabd.ttf", 40);
        RANK->setColor(Color3B(187, 173, 161));
        RANK->setPosition(visibleSize.width / 2-90, visibleSize.height - 130 - 50 * j);
        this->addChild(RANK);

        Label* SCORE = Label::createWithTTF(std::to_string(AllScores[j]), "fonts/tahomabd.ttf", 40);
        SCORE->setColor(Color3B(187, 173, 161));
        SCORE->setPosition(visibleSize.width / 2+90, visibleSize.height - 130 - 50 * j);
        this->addChild(SCORE);
    }

    return true;
}

void RankingsScene::BacktoMain(Ref* pSender)
{
    Scene* scene = MainScene::scene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.3f, scene));
}