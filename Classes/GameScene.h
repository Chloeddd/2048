#ifndef __GameScene_H__
#define __GameScene_H__

#include <vector>
#include "cocos2d.h"
#include "Block.h"
USING_NS_CC;

//��������
#define MOVE_RIGHT 0
#define MOVE_LEFT 1
#define MOVE_UP 2
#define MOVE_DOWN 3

//���ڲ��ҷ���label�ڵ�tag
#define SCORE 100
//��ͣʱҳ��Ľڵ�tag
#define PAUSE 90

const int BoardSize = 4;

extern int GameOverScore;

class GameScene :public cocos2d::Layer
{
private:
	Block* BoardMap[BoardSize][BoardSize];//��������
	int BoardNumMap[BoardSize][BoardSize];//��Ӧ��������������(��0/1��¼�Ƿ�Ϊ��λ)

	int moveDir;//�ƶ�������

	Label* scoreLabel;//��ǰ�÷ֵ�label

	std::vector<int> AllScore;

	bool isMoved;//��Ǹô��ƶ��Ƿ�ɹ�

	Vec2 touchStartPos;//��궯����ʼ��

	//������
	cocos2d::EventListenerTouchOneByOne* touchListener;
	cocos2d::EventListenerKeyboard* keyboardListener;

	//��궯������
	bool GameScene::onTouchBegan(Touch* touch, Event* event);//��¼��ʼλ��
	void GameScene::onTouchMoved(Touch* touch, Event* event);//�ж��ƶ�����
	//���̲�������
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

public:
	virtual bool init();
	static cocos2d::Scene* scene();

	void CreateBlock();//�ڿ�λ��������·���
	void BacktoMain(Ref* pSender);//�ص�������
	void PauseGame(Ref* pSender);//��ͣ��Ϸ

	//bool isFull();//��⵱ǰ�����Ƿ����ˣ����ھ����Ƿ�����·���

	bool isOver();//�����Ϸ�Ƿ�ʧ��
	bool isWin();//�����Ϸ�Ƿ�ʤ��
	void GameOver();//��Ϸʧ�ܽ���
	void GameWin();//��Ϸʤ������

	void MoveAllBlocks();//��moveDir�ķ����ƶ�ȫ������

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void UpdateRankings(int NewScore);

	int NowScore;//��ǰ�÷�
	//int BestScore;//��ѵ÷�

	CREATE_FUNC(GameScene);

};


#endif