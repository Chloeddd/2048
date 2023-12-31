#include "Block.h"
#include "SettingScene.h"
#include "audio/include/AudioEngine.h"


bool Block::init()
{
	if (!Sprite::init())
		return false;

	//�·�����75%����Ϊ2��25%����Ϊ4
	int p = rand() % 4;
	this->BlockNum = p < 3 ? 2 : 4;

	auto BlockColor = LayerColor::create(Color4B(0, 0, 0, 255), BlockSize, BlockSize);
	BlockColor->setTag(BlockBG);
	this->addChild(BlockColor);

	//���ò�ͬ������ɫ
	if (BlockNum == 2)
		BlockColor->setColor(Color3B(238, 228, 218));
	else
		BlockColor->setColor(Color3B(237, 224, 200));

	Label* number = Label::createWithTTF(StringUtils::format("%d", BlockNum), "fonts/tahomabd.ttf", 60);
	number->setColor(Color3B(119, 110, 101));
	number->setTag(BlockNUM);

	//������Ϊ������ӽڵ㣬��Ϊ����
	number->setPosition(Vec2(BlockSize / 2, BlockSize / 2));
	BlockColor->addChild(number);

	return true;
}

void Block::MoveBlock(int newRow,int newCol)
{
	//������ʱ��bug�����޸�
	//auto moveToAction = MoveTo::create(0.1f, Vec2(BOARD_X + newCol * (BlockSize + 15.0f), BOARD_Y + (3 - newRow) * (BlockSize + 15.0f)));
	//this->runAction(moveToAction);

	this->setPosition(Vec2(BOARD_X + newCol * (BlockSize + 15.0f), BOARD_Y + (3 - newRow) * (BlockSize + 15.0f)));

	if (IsMusicOn) {
		auto MoveSound = AudioEngine::play2d("move.mp3");
		AudioEngine::setVolume(MoveSound, 1.0f);
	}

	this->BlockCol = newCol;
	this->BlockRow = newRow;
}

void Block::ChangeBlock()
{
	this->BlockNum = this->BlockNum * 2;
	//����Ԥ��ڵ�Tag�ҵ���Ӧ�ķ���
	auto blockColor= this->getChildByTag(BlockBG);

	//blockColor->removeChildByTag(BlockNUM);//ɾ��ԭ�������ֽڵ�

	//�޸���ʾ��label����
	auto blockNumber= dynamic_cast<Label*>(blockColor->getChildByTag(BlockNUM));
	blockNumber->setString(StringUtils::format("%d", this->BlockNum));

	TTFConfig newTtfConfig = blockNumber->getTTFConfig();

	switch (this->BlockNum) {
		case 4:
			blockColor->setColor(Color3B(237, 224, 200));
			blockNumber->setColor(Color3B(119, 110, 101));
			break;
		case 8:
			blockColor->setColor(Color3B(242, 177, 121));
			blockNumber->setColor(Color3B(249, 246, 242));
			break;
		case 16:
			blockColor->setColor(Color3B(245, 149, 99));
			blockNumber->setColor(Color3B(249, 246, 242));
			break;
		case 32:
			blockColor->setColor(Color3B(246, 124, 95));
			blockNumber->setColor(Color3B(249, 246, 242));
			break;
		case 64:
			blockColor->setColor(Color3B(246, 94, 59));
			blockNumber->setColor(Color3B(249, 246, 242));
			break;
		case 128:
			blockColor->setColor(Color3B(237, 207, 114));
			blockNumber->setColor(Color3B(249, 246, 242));
			newTtfConfig.fontSize = 50;  //�޸��ֺ�����Ӧ��λ��
			blockNumber->setTTFConfig(newTtfConfig);
			break;
		case 256:
			blockColor->setColor(Color3B(237, 204, 97));
			blockNumber->setColor(Color3B(249, 246, 242));
			newTtfConfig.fontSize = 50;  //�޸��ֺ�����Ӧ��λ��
			blockNumber->setTTFConfig(newTtfConfig);
			break;
		case 512:
			blockColor->setColor(Color3B(237, 200, 80));
			blockNumber->setColor(Color3B(249, 246, 242));
			newTtfConfig.fontSize = 50;  //�޸��ֺ�����Ӧ��λ��
			blockNumber->setTTFConfig(newTtfConfig);
			break;
		case 1024:
			blockColor->setColor(Color3B(227, 185, 23));
			blockNumber->setColor(Color3B(249, 246, 242));
			newTtfConfig.fontSize = 40;  //�޸��ֺ�����Ӧ��λ��
			blockNumber->setTTFConfig(newTtfConfig);
			break;
		case 2048:
			blockColor->setColor(Color3B(234, 195, 4));
			blockNumber->setColor(Color3B(249, 246, 242));
			newTtfConfig.fontSize = 40;  //�޸��ֺ�����Ӧ��λ��
			blockNumber->setTTFConfig(newTtfConfig);
			break;
	}

	auto scaleToNormal = ScaleTo::create(0.1f, 1.0f);
	this->setScale(1.1f);
	this->runAction(scaleToNormal);
}

int Block::getNum()
{
	return this->BlockNum;
}