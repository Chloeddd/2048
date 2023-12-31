#ifndef __GameScene_H__
#define __GameScene_H__

#include <vector>
#include "cocos2d.h"
#include "Block.h"
USING_NS_CC;

//滑动方向
#define MOVE_RIGHT 0
#define MOVE_LEFT 1
#define MOVE_UP 2
#define MOVE_DOWN 3

//便于查找分数label节点tag
#define SCORE 100
//暂停时页面的节点tag
#define PAUSE 90

const int BoardSize = 4;

extern int GameOverScore;

class GameScene :public cocos2d::Layer
{
private:
	Block* BoardMap[BoardSize][BoardSize];//方块棋盘
	int BoardNumMap[BoardSize][BoardSize];//对应的数字数组棋盘(用0/1记录是否为空位)

	int moveDir;//移动方向标记

	Label* scoreLabel;//当前得分的label

	std::vector<int> AllScore;

	bool isMoved;//标记该次移动是否成功

	Vec2 touchStartPos;//鼠标动作起始点

	//监听器
	cocos2d::EventListenerTouchOneByOne* touchListener;
	cocos2d::EventListenerKeyboard* keyboardListener;

	//鼠标动作监听
	bool GameScene::onTouchBegan(Touch* touch, Event* event);//记录起始位置
	void GameScene::onTouchMoved(Touch* touch, Event* event);//判断移动方向
	//键盘操作监听
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

public:
	virtual bool init();
	static cocos2d::Scene* scene();

	void CreateBlock();//在空位随机生成新方块
	void BacktoMain(Ref* pSender);//回到主界面
	void PauseGame(Ref* pSender);//暂停游戏

	//bool isFull();//检测当前棋盘是否满了，用于决定是否产生新方块

	bool isOver();//检测游戏是否失败
	bool isWin();//检测游戏是否胜利
	void GameOver();//游戏失败界面
	void GameWin();//游戏胜利界面

	void MoveAllBlocks();//按moveDir的方向移动全部方块

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void UpdateRankings(int NewScore);

	int NowScore;//当前得分
	//int BestScore;//最佳得分

	CREATE_FUNC(GameScene);

};


#endif