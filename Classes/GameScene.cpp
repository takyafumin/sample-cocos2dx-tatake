/*
 * GameScene.cpp
 *
 *  Created on: 2014/09/10
 *      Author: takyafumin
 */

#include <string.h>
#include "GameScene.h"

#define LABEL_FONT "Arial-BoldMT"

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

GameScene::GameScene()
:Layer()
, mpScoreLabel(nullptr)
, mScoreValue(0)
, mpTimeLabel(nullptr)
, mTimeValue(0)
{
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

	// ゲーム時間設定
	mTimeValue = 15.0f;

	// スコアラベル表示
	mpScoreLabel = Label::createWithSystemFont("", LABEL_FONT, 24, Size(320, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
	mpScoreLabel->setAnchorPoint(Point(0.0f, 0.5f));
	mpScoreLabel->setPosition(10, 330);
	this->addChild(mpScoreLabel);

	// 時間ラベル表示
	mpTimeLabel = Label::createWithSystemFont("", LABEL_FONT, 24, Size(320, 50), TextHAlignment::RIGHT, TextVAlignment::CENTER);
	mpTimeLabel->setAnchorPoint(Point(1.0f, 0.5f));
	mpTimeLabel->setPosition(320 - 10, 330);
	this->addChild(mpTimeLabel);


	// タッチイベント設定
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
		for (auto slime : mSlimes)
		{
			const float distance = slime->getPosition().getDistance(touch->getLocation());
			if (distance <= 40.0f)
			{
				mScoreValue += 100;
				this->updateScoreLabel();
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

	// 自動更新設定
	this->scheduleUpdate();
}


/**
 * 終了処理
 */
void GameScene::finish()
{
	// 画面からスライムを消す
	for (auto slime : mSlimes)
	{
		slime->stopAllActions();
		slime->removeFromParent();
	}

	// 配列クリア
	mSlimes.clear();

	// フレーム更新停止
	this->unscheduleUpdate();

	// リセットボタン表示
	auto resetButton = MenuItemFont::create("Reset", [this](Ref* sender){
		static_cast<MenuItemFont*>(sender)->removeFromParent();

		// ゲームシーン再読み込み
		auto nextScene = GameScene::createScene();
		Director::getInstance()->replaceScene(nextScene);
	});

	auto menu = Menu::create(resetButton, nullptr);
	this->addChild(menu);
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


/**
 * スコア表示処理
 */
void GameScene::updateScoreLabel()
{
	mpScoreLabel->setString(StringUtils::format("%upts", mScoreValue));
}

/**
 * 残り時間表示
 */
void GameScene::updateTimeLabel()
{
	mpTimeLabel->setString(StringUtils::format("TIME：%.1f", mTimeValue));
}

/**
 * フレーム更新処理
 */
void GameScene::update(float dt)
{
	// 残り時間を減算
	mTimeValue -= dt;
	mTimeValue = MAX(mTimeValue, 0);
	this->updateTimeLabel();

	// 終了判定
	if (mTimeValue <= 0)
	{
		this->finish();
	}
}
