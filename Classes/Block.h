#ifndef __Block_H__
#define __Block_H__

#include "cocos2d.h"

#define BlockSize 120 //����Ԥ���С

//���ڲ��ҷ���ı����ڵ�����ֽڵ�
#define BlockBG 80
#define BlockNUM 40

//���̶���ԭ���ƫ������
#define BOARD_X 37
#define BOARD_Y 25

USING_NS_CC;

class Block : public cocos2d::Sprite
{
private:
	//������������
	int BlockRow;
	int BlockCol;

	int BlockNum;//��������
public:
	virtual bool init();

	void MoveBlock(int row, int col);
	int getNum();
	void ChangeBlock();


	CREATE_FUNC(Block);
};

#endif