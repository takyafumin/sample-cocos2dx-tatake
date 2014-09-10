/*
 * GameScene.cpp
 *
 *  Created on: 2014/09/10
 *      Author: takyafumin
 */

#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	return scene;
}

GameScene::~GameScene() {
	// TODO Auto-generated destructor stub
}

GameScene::GameScene() {
	// TODO 自動生成されたコンストラクター・スタブ

}


bool GameScene::init()
{
	if (! Layer::init())
		return false;

	// 背景表示
	auto background = Sprite::create(PngBackground);
	background->setAnchorPoint(Point(0, 0));
	this->addChild(background);

	// スタートボタン表示
	auto startButton = MenuItemFont::create("Start", [this](Ref* sender){
		static_cast<MenuItemFont*>(sender)->removeFromParent();
		this->start();
	});

	auto menu = Menu::create(startButton, nullptr);
	this->addChild(menu);


	return true;
}

/**
 * スタート
 */
void GameScene::start()
{
	static const uint numSlimes = 3;
	for (uint i = 0; i < numSlimes; i++)
	{

		// スライム表示
		auto slime = Sprite::create(PngSlime_1_1);
		slime->setPosition(160, 200 + i * 50);
		this->addChild(slime);

		// アニメーション設定
		auto animation = Animation::create();
		animation->addSpriteFrameWithFile(PngSlime_1_1);
		animation->addSpriteFrameWithFile(PngSlime_1_2);
		animation->setDelayPerUnit(0.5f);
		slime->runAction(
				RepeatForever::create(
						Animate::create(animation)
				)
		);

		// 保持配列に追加
		mSlimes.pushBack(slime);
	}
}


/**
 * 終了
 */
void GameScene::finish()
{

}
