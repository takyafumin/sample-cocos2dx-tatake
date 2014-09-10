/*
 * GameScene.h
 *
 *  Created on: 2014/09/10
 *      Author: takyafumin
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "cocos2d.h"
#include "Config.h"
#include <string.h>

class GameScene: public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

public:
	GameScene();
	virtual ~GameScene();

	virtual bool init();
	CREATE_FUNC(GameScene);

	virtual void update(float dt);

private:

	// 背景画像
	const std::string PngBackground = PNG_BACKGROUND;

	// スライム画像
	const std::string PngSlime_1_1 = PNG_SLIME_1_1;
	const std::string PngSlime_1_2 = PNG_SLIME_1_2;

	// スライム保持配列
	cocos2d::Vector<cocos2d::Sprite*> mSlimes;

	// スコア
	cocos2d::Label* mpScoreLabel;
	uint mScoreValue;

	// ゲーム時間
	cocos2d::Label* mpTimeLabel;
	float mTimeValue;


	void start();
	void finish();


	// スライムの移動
	void popSlime(cocos2d::Sprite* target);

	// スコア表示更新
	void updateScoreLabel();

	// 残り時間表示更新
	void updateTimeLabel();

	// 取得ポイント表示処理
	void showScore(const cocos2d::Point &point, uint score);
};

#endif /* GAMESCENE_H_ */
