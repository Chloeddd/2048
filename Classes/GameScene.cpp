#include "GameScene.h"
#include "MainScene.h"
#include "GameOver.h"
#include "GameWin.h"

int GameOverScore;

Scene* GameScene::scene()
{
    Scene* scene = Scene::create();

    GameScene* layer = GameScene::create();

    scene->addChild(layer);
    return scene;
}

bool GameScene::init() {

    if (!Layer::init())
    {
        return false;
    }

    NowScore = 0;

    auto director = Director::getInstance();
    auto visibleSize = director->getVisibleSize();
    auto origin = director->getVisibleOrigin();

    //��Ϸ����
    auto GameBG = Sprite::create("GameBG.png");
    GameBG->setAnchorPoint(Vec2(0, 0));
    GameBG->setPosition(Vec2(0, 0));
    //����Ӧ��С
    float x = visibleSize.width / GameBG->getContentSize().width;
    float y = visibleSize.height / GameBG->getContentSize().height;
    GameBG->setScale(x, y);
    this->addChild(GameBG);

    //������־����
    Sprite* ScoreBG = Sprite::create("score.png");
    this->addChild(ScoreBG);
    ScoreBG->setScale(2.0f);

    //��������
    scoreLabel = Label::createWithTTF(StringUtils::format("%d", NowScore), "fonts/tahomabd.ttf", 15);
    scoreLabel->setColor(Color3B(255, 255, 255));
    scoreLabel->setTag(SCORE);

    //���������뱳�����
    ScoreBG->addChild(scoreLabel);
    scoreLabel->setPosition(Vec2(ScoreBG->getContentSize().width / 2, ScoreBG->getContentSize().height / 4+3));
    ScoreBG->setPosition(Vec2(510, 743));

    //�������������ͣ��ť
    Sprite* BackButtonNor = Sprite::create("BackButton.png");
    MenuItemSprite* BackButton = MenuItemSprite::create(BackButtonNor, nullptr,
        CC_CALLBACK_1(GameScene::BacktoMain, this));

    Sprite* PauseButtonNor = Sprite::create("PauseButton.png");
    MenuItemSprite* PauseButton = MenuItemSprite::create(PauseButtonNor, nullptr,
        CC_CALLBACK_1(GameScene::PauseGame, this));

    BackButton->setScale(1.5f);
    PauseButton->setScale(1.5f);

    Menu* GameMenu = Menu::create(BackButton, PauseButton, NULL);
    GameMenu->alignItemsVerticallyWithPadding(15);
    GameMenu->setPosition(Vec2(510, 650));
    this->addChild(GameMenu);

    //���̱���
    Sprite* BoardBG = Sprite::create("BoardBG.png");
    BoardBG->setAnchorPoint(Vec2(0.5, 0));
    BoardBG->setPosition(Vec2(visibleSize.width / 2, 10));
    BoardBG->setScale(1.9f);
    this->addChild(BoardBG);

    //��ʼ������
	//���Ʒ����λ��˳���ʼ����������
	for (int i = 0; i < BoardSize; i++) {
		for (int j = 0; j < BoardSize; j++) {
			auto EmptyBlock = LayerColor::create(Color4B(205, 193, 180, 255), BlockSize, BlockSize);
			EmptyBlock->setPosition(Vec2(BOARD_X + i * (BlockSize + 15.0f), BOARD_Y + j * (BlockSize + 15.0f)));
			this->addChild(EmptyBlock);
            BoardNumMap[i][j] = 0;
		}
	}

    //��ʼ���������������
    CreateBlock();
    CreateBlock();

    //��껬��������
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    //�����������Ҽ�����
    keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    return true;
}

//��¼��ʼ��λ��
bool GameScene::onTouchBegan(Touch* touch, Event* event) {
    touchStartPos = touch->getLocation();
    return true;
}

//���㻬������
void GameScene::onTouchMoved(Touch* touch, Event* event) {
    //��껬����ƫ����
    Vec2 delta = touch->getDelta();

    //�жϻ�������
    if (fabs(delta.x) > fabs(delta.y)) {//ˮƽ���򻬶������ж�Ϊˮƽ����
		if (delta.x > 10)
			moveDir = MOVE_RIGHT;
		else if (delta.x < -10)
			moveDir = MOVE_LEFT;
	}
	else {//��ֱ���򻬶�
		if (delta.y > 10)
			moveDir = MOVE_UP;
		else if (delta.y < -10)
			moveDir = MOVE_DOWN;
	}

    isMoved = 0;//��ʼ��Ϊ0����δ�ƶ�
    MoveAllBlocks();

    if (isMoved) {//��������ɹ��������·���
        CreateBlock();
    }
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            moveDir = MOVE_UP;
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            moveDir = MOVE_DOWN;
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            moveDir = MOVE_LEFT;
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            moveDir = MOVE_RIGHT;
            break;
        default:
            break;
    }

    isMoved = 0;//��ʼ��Ϊ0����δ�ƶ�
    MoveAllBlocks();

    if (isMoved) {//��������ɹ�������Ƿ�ʤ������δʤ���������·���
        if (isWin()) {
            GameWin();
        }
        else {
            CreateBlock();
        }
    }
}

void GameScene::CreateBlock()
{
    int row, col;
    std::vector<int> emptyBlock;

    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            if (BoardNumMap[i][j] == 0) {
                emptyBlock.push_back(i);
                emptyBlock.push_back(j);
            }
        }
    }

    if(emptyBlock.size()>0){
        srand(int(time(0)));
        int k = rand() % (emptyBlock.size() / 2);
        row = emptyBlock[2 * k];
        col = emptyBlock[2 * k + 1];

        auto block = Block::create();
        BoardMap[row][col] = block;
        BoardNumMap[row][col] = 1;

        //�������ɶ���
        block->setPosition(Vec2(BOARD_X + col * (BlockSize + 15.0f), BOARD_Y + (3 - row) * (BlockSize + 15.0f)));
        block->setAnchorPoint(Vec2(0.5f, 0.5f));
        this->addChild(block);
        auto scaleToNormal = ScaleTo::create(0.1f, 1.0f);
        block->setScale(0.2f);
        block->runAction(scaleToNormal);
    }

    //���ɷ�������Ƿ���Ϸ����
    if (isOver()) {
        GameOver();
    }
}

void GameScene::MoveAllBlocks()
{
    if (moveDir == MOVE_UP)
        MoveUp();
    else if (moveDir == MOVE_DOWN)
        MoveDown();
    else if (moveDir == MOVE_LEFT)
        MoveLeft();
    else if (moveDir == MOVE_RIGHT)
        MoveRight();
}

void GameScene::MoveUp()//�������¼��������Ƶķ���
{
    for (int col = 0; col < BoardSize; col++) {
        for (int row = 1; row < BoardSize; row++) {
            if (BoardNumMap[row][col] != 0) {
                for (int i = row - 1; i >= 0; i--) {
                    if (BoardNumMap[i][col] == 0) {//�Ϸ�Ϊ�գ������ƶ�һ��
                        BoardMap[i+1][col]->MoveBlock(i,col);//(i,col)ΪĿ�����꣬(i+1,col)��Ϊ��ǰ�ķ��龫��
                        //���ֱ�Ǹ���
                        BoardNumMap[i][col] = 1;
                        BoardNumMap[i + 1][col] = 0;
                        //�������̸���
                        BoardMap[i][col] = BoardMap[i + 1][col];
                        BoardMap[i + 1][col] = nullptr;

                        isMoved = 1;
                    }
                    else {//�Ϸ��з���
                        if (BoardMap[i + 1][col]->getNum() == BoardMap[i][col]->getNum()) {//�����������ͬ���ϲ�
                            BoardMap[i + 1][col]->MoveBlock(i, col);//(i,col)ΪĿ�����꣬(i+1,col)��Ϊ��ǰ�ķ��龫��
                            //�÷��龫�����ƺ��Ƴ������궯��Ч����ɾ�����������Ϸ����龫��ϲ���
                            BoardMap[i + 1][col]->removeFromParent();
                            BoardNumMap[i + 1][col] = 0;
                            //�Ϸ����龫��*2
                            BoardMap[i][col]->ChangeBlock();
                            //��������
                            NowScore += BoardMap[i][col]->getNum();

                            scoreLabel->setString(StringUtils::format("%d", NowScore));

                            isMoved = 1;
                            break;
                        }
                        else
                            break;
                    }
                }
            }
        }
    }
}

void GameScene::MoveDown()//�������ϼ��������Ƶķ���
{
    for (int col = 0; col < BoardSize; col++) {
        for (int row = 2; row >= 0; row--) {
            if (BoardNumMap[row][col] != 0) {
                for (int i = row + 1; i < BoardSize; i++) {
                    if (BoardNumMap[i][col] == 0) {//�·�Ϊ�գ������ƶ�һ��
                        BoardMap[i - 1][col]->MoveBlock(i, col);//(i,col)ΪĿ�����꣬(i-1,col)��Ϊ��ǰ�ķ��龫��
                        //���ֱ�Ǹ���
                        BoardNumMap[i][col] = 1;
                        BoardNumMap[i - 1][col] = 0;
                        //�������̸���
                        BoardMap[i][col] = BoardMap[i - 1][col];
                        BoardMap[i - 1][col] = nullptr;

                        isMoved = 1;
                    }
                    else {//�·��з���
                        if (BoardMap[i - 1][col]->getNum() == BoardMap[i][col]->getNum()) {//�����������ͬ���ϲ�
                            BoardMap[i - 1][col]->MoveBlock(i, col);//(i,col)ΪĿ�����꣬(i-1,col)��Ϊ��ǰ�ķ��龫��
                            //�÷��龫�����ƺ��Ƴ������궯��Ч����ɾ�����������Ϸ����龫��ϲ���
                            BoardMap[i - 1][col]->removeFromParent();
                            BoardNumMap[i - 1][col] = 0;
                            //�Ϸ����龫��*2
                            BoardMap[i][col]->ChangeBlock();
                            //��������
                            NowScore += BoardMap[i][col]->getNum();

                            scoreLabel->setString(StringUtils::format("%d", NowScore));

                            isMoved = 1;
                            break;
                        }
                        else
                            break;
                    }
                }
            }
        }
    }
}

void GameScene::MoveLeft()//�������Ҽ��������Ƶķ���
{
    for (int row = 0; row < BoardSize; row++) {
        for (int col = 1; col < BoardSize; col++) {
            if (BoardNumMap[row][col] != 0) {
                for (int j = col - 1; j >=0; j--) {
                    if (BoardNumMap[row][j] == 0) {//��Ϊ��
                        BoardMap[row][j+1]->MoveBlock(row, j);
                        //���ֱ�Ǹ���
                        BoardNumMap[row][j] = 1;
                        BoardNumMap[row][j+1] = 0;
                        //�������̸���
                        BoardMap[row][j] = BoardMap[row][j + 1];
                        BoardMap[row][j + 1] = nullptr;

                        isMoved = 1;
                    }
                    else {//�·��з���
                        if (BoardMap[row][j + 1]->getNum() == BoardMap[row][j ]->getNum()) {//�����������ͬ���ϲ�
                            BoardMap[row][j + 1]->MoveBlock(row,j);
                            //�÷��龫�����ƺ��Ƴ������궯��Ч����ɾ�����������Ϸ����龫��ϲ���
                            BoardMap[row][j + 1]->removeFromParent();
                            BoardNumMap[row][j + 1] = 0;
                            //�󷽷��龫��*2
                            BoardMap[row][j ]->ChangeBlock();
                            //��������
                            NowScore += BoardMap[row][j]->getNum();

                            scoreLabel->setString(StringUtils::format("%d", NowScore));

                            isMoved = 1;
                            break;
                        }
                        else
                            break;
                    }
                }
            }
        }
    }
}

void GameScene::MoveRight()
{
    for (int row = 0; row < BoardSize; row++) {
        for (int col = 2; col >= 0; col--) {
            if (BoardNumMap[row][col] != 0) {
                for (int j = col + 1; j < BoardSize; j++) {
                    if (BoardNumMap[row][j] == 0) {
                        BoardMap[row][j - 1]->MoveBlock(row, j);
                        //���ֱ�Ǹ���
                        BoardNumMap[row][j] = 1;
                        BoardNumMap[row][j - 1] = 0;
                        //�������̸���
                        BoardMap[row][j] = BoardMap[row][j - 1];
                        BoardMap[row][j - 1] = nullptr;

                        isMoved = 1;
                    }
                    else {//�ҷ��з���
                        if (BoardMap[row][j - 1]->getNum() == BoardMap[row][j]->getNum()) {//�����������ͬ���ϲ�
                            BoardMap[row][j - 1]->MoveBlock(row, j);
                            //�÷��龫�����ƺ��Ƴ������궯��Ч����ɾ�����������Ϸ����龫��ϲ���
                            BoardMap[row][j - 1]->removeFromParent();
                            BoardNumMap[row][j - 1] = 0;
                            //�󷽷��龫��*2
                            BoardMap[row][j]->ChangeBlock();
                            //��������
                            NowScore += BoardMap[row][j]->getNum();

                            scoreLabel->setString(StringUtils::format("%d", NowScore));

                            isMoved = 1;
                            break;
                        }
                        else
                            break;
                    }
                }
            }
        }
    }
}

bool GameScene::isOver()
{
    //����Ƿ��п�λ
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            if (BoardNumMap[i][j]==0)
                return false;//���п�λ����û�н���
        }
    }

    //û�п�λ������Ƿ�����ͨ��
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            if (i != 0) {//����Ϸ�
                if (BoardMap[i][j]->getNum() == BoardMap[i - 1][j]->getNum())
                    return false;
            }
            if (i != BoardSize-1) {//����·�
                if (BoardMap[i][j]->getNum() == BoardMap[i + 1][j]->getNum())
                    return false;
            }
            if (j != 0) {//�����
                if (BoardMap[i][j]->getNum() == BoardMap[i ][j- 1]->getNum())
                    return false;
            }
            if (j != BoardSize - 1) {//����ҷ�
                if (BoardMap[i][j]->getNum() == BoardMap[i][j+1]->getNum())
                    return false;
            }
        }
    }

    //û�п�λ��û����ͨ��
    return true;
}

bool GameScene::isWin()
{
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            if (BoardNumMap[i][j] != 0 && BoardMap[i][j]->getNum() == 2048)//����з���λ���Ϸ��������
                return true;
        }
    }

    return false;
}

void GameScene::GameOver()
{
    UpdateRankings(NowScore);
    GameOverScore = NowScore;
    Scene* scene = GameOver::scene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.3f, scene));
}

void GameScene::GameWin()
{
    UpdateRankings(NowScore);
    GameOverScore = NowScore;
    Scene* scene = GameWin::scene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.3f, scene));

}

void GameScene::BacktoMain(Ref* pSender)
{
    Scene* scene = MainScene::scene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.3f, scene));
}

void GameScene::PauseGame(Ref* pSender)
{

    if (Director::getInstance()->isPaused()) {
        Director::getInstance()->resume(); //�����ǰ����ͣ״̬����ָ���Ϸ
        _eventDispatcher->resumeEventListenersForTarget(this);
        removeChildByTag(PAUSE);
    }
    else {
        Director::getInstance()->pause(); //��ͣ��Ϸ
        _eventDispatcher->pauseEventListenersForTarget(this);
        _eventDispatcher->pauseEventListenersForTarget(this); 
        
        //������ͣʱ����
        auto director = Director::getInstance();
        auto visibleSize = director->getVisibleSize();
        Sprite* PauseBG = Sprite::create("BoardBG.png");
        PauseBG->setAnchorPoint(Vec2(0.5, 0));
        PauseBG->setPosition(Vec2(visibleSize.width / 2, 10));
        PauseBG->setScale(1.9f);
        PauseBG->setOpacity(128);
        PauseBG->setTag(PAUSE);
        this->addChild(PauseBG);

        auto PauseLabel = Label::createWithTTF("Pause...", "fonts/tahomabd.ttf", 50);
        PauseLabel->setColor(Color3B(255, 255, 255));
        //�뱳�����
        PauseBG->addChild(PauseLabel);
        PauseLabel->setPosition(Vec2(PauseBG->getContentSize().width / 2, PauseBG->getContentSize().height / 2));
    }
}

bool cmp(int x, int y)
{
    return x > y;
}

void GameScene::UpdateRankings(int NewScore)
{
    int x;
    UserDefault* userDefault = UserDefault::getInstance();
    for (int i = 1; i <= 10; i++) {//��ȡǰʮ�����趨ֻ��¼ǰʮ�����а�
        std::string Rank = "Rank" + std::to_string(i);
        x = userDefault->getIntegerForKey(Rank.c_str(), -1);
        if (x == -1)
            break;//�洢����ʮ��
        AllScore.push_back(x);
    }

    AllScore.push_back(NewScore);
    sort(AllScore.begin(), AllScore.end(),cmp);

    for (int j = 0; j < 10 && j < AllScore.size(); j++) {//����ǰʮ�����ȥ
        std::string Rank = "Rank" + std::to_string(j+1);
        userDefault->setIntegerForKey(Rank.c_str(), AllScore[j]);
    }
}
