#ifndef __Block_H__
#define __Block_H__

#include "cocos2d.h"

#define BlockSize 120 //方块预设大小

//用于查找方块的背景节点和数字节点
#define BlockBG 80
#define BlockNUM 40

//棋盘对于原点的偏移坐标
#define BOARD_X 37
#define BOARD_Y 25

USING_NS_CC;

class Block : public cocos2d::Sprite
{
private:
	//方块所在行列
	int BlockRow;
	int BlockCol;

	int BlockNum;//方块数字
public:
	virtual bool init();

	void MoveBlock(int row, int col);
	int getNum();
	void ChangeBlock();


	CREATE_FUNC(Block);
};

#endif