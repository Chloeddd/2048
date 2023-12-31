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

    //游戏背景
    auto GameBG = Sprite::create("GameBG.png");
    GameBG->setAnchorPoint(Vec2(0, 0));
    GameBG->setPosition(Vec2(0, 0));
    //自适应大小
    float x = visibleSize.width / GameBG->getContentSize().width;
    float y = visibleSize.height / GameBG->getContentSize().height;
    GameBG->setScale(x, y);
    this->addChild(GameBG);

    //分数标志背景
    Sprite* ScoreBG = Sprite::create("score.png");
    this->addChild(ScoreBG);
    ScoreBG->setScale(2.0f);

    //分数数字
    scoreLabel = Label::createWithTTF(StringUtils::format("%d", NowScore), "fonts/tahomabd.ttf", 15);
    scoreLabel->setColor(Color3B(255, 255, 255));
    scoreLabel->setTag(SCORE);

    //分数数字与背景结合
    ScoreBG->addChild(scoreLabel);
    scoreLabel->setPosition(Vec2(ScoreBG->getContentSize().width / 2, ScoreBG->getContentSize().height / 4+3));
    ScoreBG->setPosition(Vec2(510, 743));

    //返回主界面和暂停按钮
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

    //棋盘背景
    Sprite* BoardBG = Sprite::create("BoardBG.png");
    BoardBG->setAnchorPoint(Vec2(0.5, 0));
    BoardBG->setPosition(Vec2(visibleSize.width / 2, 10));
    BoardBG->setScale(1.9f);
    this->addChild(BoardBG);

    //初始化棋盘
	//绘制方块空位，顺便初始化数字棋盘
	for (int i = 0; i < BoardSize; i++) {
		for (int j = 0; j < BoardSize; j++) {
			auto EmptyBlock = LayerColor::create(Color4B(205, 193, 180, 255), BlockSize, BlockSize);
			EmptyBlock->setPosition(Vec2(BOARD_X + i * (BlockSize + 15.0f), BOARD_Y + j * (BlockSize + 15.0f)));
			this->addChild(EmptyBlock);
            BoardNumMap[i][j] = 0;
		}
	}

    //初始随机生成两个方块
    CreateBlock();
    CreateBlock();

    //鼠标滑动监听器
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    //键盘上下左右监听器
    keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    return true;
}

//记录开始的位置
bool GameScene::onTouchBegan(Touch* touch, Event* event) {
    touchStartPos = touch->getLocation();
    return true;
}

//计算滑动方向
void GameScene::onTouchMoved(Touch* touch, Event* event) {
    //鼠标滑动的偏移量
    Vec2 delta = touch->getDelta();

    //判断滑动方向
    if (fabs(delta.x) > fabs(delta.y)) {//水平方向滑动更大，判定为水平滑动
		if (delta.x > 10)
			moveDir = MOVE_RIGHT;
		else if (delta.x < -10)
			moveDir = MOVE_LEFT;
	}
	else {//垂直方向滑动
		if (delta.y > 10)
			moveDir = MOVE_UP;
		else if (delta.y < -10)
			moveDir = MOVE_DOWN;
	}

    isMoved = 0;//初始化为0，还未移动
    MoveAllBlocks();

    if (isMoved) {//如果滑动成功，生成新方块
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

    isMoved = 0;//初始化为0，还未移动
    MoveAllBlocks();

    if (isMoved) {//如果滑动成功，检测是否胜利，若未胜利则生成新方块
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

        //方块生成动画
        block->setPosition(Vec2(BOARD_X + col * (BlockSize + 15.0f), BOARD_Y + (3 - row) * (BlockSize + 15.0f)));
        block->setAnchorPoint(Vec2(0.5f, 0.5f));
        this->addChild(block);
        auto scaleToNormal = ScaleTo::create(0.1f, 1.0f);
        block->setScale(0.2f);
        block->runAction(scaleToNormal);
    }

    //生成方块后检测是否游戏结束
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

void GameScene::MoveUp()//从上向下检测可以上移的方块
{
    for (int col = 0; col < BoardSize; col++) {
        for (int row = 1; row < BoardSize; row++) {
            if (BoardNumMap[row][col] != 0) {
                for (int i = row - 1; i >= 0; i--) {
                    if (BoardNumMap[i][col] == 0) {//上方为空，向上移动一次
                        BoardMap[i+1][col]->MoveBlock(i,col);//(i,col)为目的坐标，(i+1,col)上为当前的方块精灵
                        //数字标记更换
                        BoardNumMap[i][col] = 1;
                        BoardNumMap[i + 1][col] = 0;
                        //精灵棋盘更换
                        BoardMap[i][col] = BoardMap[i + 1][col];
                        BoardMap[i + 1][col] = nullptr;

                        isMoved = 1;
                    }
                    else {//上方有方块
                        if (BoardMap[i + 1][col]->getNum() == BoardMap[i][col]->getNum()) {//方块的数字相同，合并
                            BoardMap[i + 1][col]->MoveBlock(i, col);//(i,col)为目的坐标，(i+1,col)上为当前的方块精灵
                            //该方块精灵上移后移除（做完动画效果后删除，看作与上方方块精灵合并）
                            BoardMap[i + 1][col]->removeFromParent();
                            BoardNumMap[i + 1][col] = 0;
                            //上方方块精灵*2
                            BoardMap[i][col]->ChangeBlock();
                            //分数增加
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

void GameScene::MoveDown()//从下向上检测可以下移的方块
{
    for (int col = 0; col < BoardSize; col++) {
        for (int row = 2; row >= 0; row--) {
            if (BoardNumMap[row][col] != 0) {
                for (int i = row + 1; i < BoardSize; i++) {
                    if (BoardNumMap[i][col] == 0) {//下方为空，向下移动一次
                        BoardMap[i - 1][col]->MoveBlock(i, col);//(i,col)为目的坐标，(i-1,col)上为当前的方块精灵
                        //数字标记更换
                        BoardNumMap[i][col] = 1;
                        BoardNumMap[i - 1][col] = 0;
                        //精灵棋盘更换
                        BoardMap[i][col] = BoardMap[i - 1][col];
                        BoardMap[i - 1][col] = nullptr;

                        isMoved = 1;
                    }
                    else {//下方有方块
                        if (BoardMap[i - 1][col]->getNum() == BoardMap[i][col]->getNum()) {//方块的数字相同，合并
                            BoardMap[i - 1][col]->MoveBlock(i, col);//(i,col)为目的坐标，(i-1,col)上为当前的方块精灵
                            //该方块精灵上移后移除（做完动画效果后删除，看作与上方方块精灵合并）
                            BoardMap[i - 1][col]->removeFromParent();
                            BoardNumMap[i - 1][col] = 0;
                            //上方方块精灵*2
                            BoardMap[i][col]->ChangeBlock();
                            //分数增加
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

void GameScene::MoveLeft()//从左向右检测可以左移的方块
{
    for (int row = 0; row < BoardSize; row++) {
        for (int col = 1; col < BoardSize; col++) {
            if (BoardNumMap[row][col] != 0) {
                for (int j = col - 1; j >=0; j--) {
                    if (BoardNumMap[row][j] == 0) {//左方为空
                        BoardMap[row][j+1]->MoveBlock(row, j);
                        //数字标记更换
                        BoardNumMap[row][j] = 1;
                        BoardNumMap[row][j+1] = 0;
                        //精灵棋盘更换
                        BoardMap[row][j] = BoardMap[row][j + 1];
                        BoardMap[row][j + 1] = nullptr;

                        isMoved = 1;
                    }
                    else {//下方有方块
                        if (BoardMap[row][j + 1]->getNum() == BoardMap[row][j ]->getNum()) {//方块的数字相同，合并
                            BoardMap[row][j + 1]->MoveBlock(row,j);
                            //该方块精灵上移后移除（做完动画效果后删除，看作与上方方块精灵合并）
                            BoardMap[row][j + 1]->removeFromParent();
                            BoardNumMap[row][j + 1] = 0;
                            //左方方块精灵*2
                            BoardMap[row][j ]->ChangeBlock();
                            //分数增加
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
                        //数字标记更换
                        BoardNumMap[row][j] = 1;
                        BoardNumMap[row][j - 1] = 0;
                        //精灵棋盘更换
                        BoardMap[row][j] = BoardMap[row][j - 1];
                        BoardMap[row][j - 1] = nullptr;

                        isMoved = 1;
                    }
                    else {//右方有方块
                        if (BoardMap[row][j - 1]->getNum() == BoardMap[row][j]->getNum()) {//方块的数字相同，合并
                            BoardMap[row][j - 1]->MoveBlock(row, j);
                            //该方块精灵上移后移除（做完动画效果后删除，看作与上方方块精灵合并）
                            BoardMap[row][j - 1]->removeFromParent();
                            BoardNumMap[row][j - 1] = 0;
                            //左方方块精灵*2
                            BoardMap[row][j]->ChangeBlock();
                            //分数增加
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
    //检测是否有空位
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            if (BoardNumMap[i][j]==0)
                return false;//还有空位，即没有结束
        }
    }

    //没有空位，检测是否有连通块
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            if (i != 0) {//检测上方
                if (BoardMap[i][j]->getNum() == BoardMap[i - 1][j]->getNum())
                    return false;
            }
            if (i != BoardSize-1) {//检测下方
                if (BoardMap[i][j]->getNum() == BoardMap[i + 1][j]->getNum())
                    return false;
            }
            if (j != 0) {//检测左方
                if (BoardMap[i][j]->getNum() == BoardMap[i ][j- 1]->getNum())
                    return false;
            }
            if (j != BoardSize - 1) {//检测右方
                if (BoardMap[i][j]->getNum() == BoardMap[i][j+1]->getNum())
                    return false;
            }
        }
    }

    //没有空位且没有连通块
    return true;
}

bool GameScene::isWin()
{
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            if (BoardNumMap[i][j] != 0 && BoardMap[i][j]->getNum() == 2048)//检查有方块位置上方块的数字
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
        Director::getInstance()->resume(); //如果当前是暂停状态，则恢复游戏
        _eventDispatcher->resumeEventListenersForTarget(this);
        removeChildByTag(PAUSE);
    }
    else {
        Director::getInstance()->pause(); //暂停游戏
        _eventDispatcher->pauseEventListenersForTarget(this);
        _eventDispatcher->pauseEventListenersForTarget(this); 
        
        //绘制暂停时画面
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
        //与背景结合
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
    for (int i = 1; i <= 10; i++) {//读取前十名（设定只记录前十名排行榜）
        std::string Rank = "Rank" + std::to_string(i);
        x = userDefault->getIntegerForKey(Rank.c_str(), -1);
        if (x == -1)
            break;//存储不足十名
        AllScore.push_back(x);
    }

    AllScore.push_back(NewScore);
    sort(AllScore.begin(), AllScore.end(),cmp);

    for (int j = 0; j < 10 && j < AllScore.size(); j++) {//更新前十名存回去
        std::string Rank = "Rank" + std::to_string(j+1);
        userDefault->setIntegerForKey(Rank.c_str(), AllScore[j]);
    }
}
