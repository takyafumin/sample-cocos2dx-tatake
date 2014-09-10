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

	// タッチイベント設定
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
		for (auto slime : mSlimes)
		{
			const float distance = slime->getPosition().getDistance(touch->getLocation());
			if (distance <= 40.0f)
			{
				this->popSlime(slime);
				return false;
			}
		}
		return false;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


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


/**
 * スライム移動処理
 */
void GameScene::popSlime(Sprite* target)
{
	target->setVisible(true);

	static const float margin = 30.0f;
	const float x = margin + CCRANDOM_0_1() * (320.0f - margin * 2);
	const float y = margin + CCRANDOM_0_1() * 200.0f;

	target->setPosition(x, y);


	// 一定時間後に移動
	static const int rePopTag = 100;
	target->stopActionByTag(rePopTag);

	auto rePop = Sequence::create(
		DelayTime::create(1.0f),
		CallFunc::create([this, target](){
			this->popSlime(target);
		}),
		nullptr
	);

	rePop->setTag(rePopTag);
	target->runAction(rePop);
}
