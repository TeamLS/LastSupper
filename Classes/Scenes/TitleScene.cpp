﻿//
//  TitleScene.cpp
//  LastSupper
//
//  Created by Kohei on 2015/06/05.
//
//

#include "Scenes/TitleScene.h"
#include "Scenes/DungeonScene.h"

#include "Datas/Scene/TitleSceneData.h"
#include "Datas/Scene/DungeonSceneData.h"

#include "Layers/LoadingLayer.h"
#include "Layers/Menu/SaveDataSelector.h"
#include "Layers/Menu/TitleMainMenuLayer.h"
#include "Layers/Menu/TrophyListLayer.h"

#include "Models/GlobalPlayerData.h"

// コンストラクタ
TitleScene::TitleScene(){FUNCLOG}

// デストラクタ
TitleScene::~TitleScene(){FUNCLOG}

// 初期化
bool TitleScene::init()
{
    if(!BaseScene::init(TitleSceneData::create())) return false;
    
    return true;
}

// シーン切り替え完了時
void TitleScene::onEnter()
{
    BaseScene::onEnter();
}

// リソースロード終了後の処理
void TitleScene::onPreloadFinished(LoadingLayer* loadingLayer)
{
    // ローディング終了
    loadingLayer->onLoadFinished();
    
    // メインメニューレイヤーを生成
    TitleMainMenuLayer* mainMenu {TitleMainMenuLayer::create()};
    this->addChild(mainMenu);
    
    // メインメニューのイベントをリッスン
    mainMenu->onStartSelected = CC_CALLBACK_0(TitleScene::onStartSelected, this);
    mainMenu->onContinueSelected = CC_CALLBACK_0(TitleScene::onContinueSelected, this);
    mainMenu->onExitSelected = CC_CALLBACK_0(TitleScene::onExitSelected, this);
    mainMenu->onTrophySelected = CC_CALLBACK_0(TitleScene::onTrophyListSelected, this);
    
    mainMenu->show();
    this->mainMenu = mainMenu;
    
    // セーブデータ選択レイヤーを生成
    SaveDataSelector* saveDataSelector { SaveDataSelector::create(false) };
    this->addChild(saveDataSelector);
    
    // セーブデータ選択レイヤーのイベントをリッスン
    saveDataSelector->onSaveDataSelectCancelled = CC_CALLBACK_0(TitleScene::onSaveDataSelectCancelled, this);
    saveDataSelector->setVisible(false);
    saveDataSelector->hide();
    this->saveDataSelector = saveDataSelector;
    
    // BGM
    SoundManager::getInstance()->playBGM("title_bgm.mp3", true, 0.7f);
}

// 最初からが選ばれた時
void TitleScene::onStartSelected()
{
	FUNCLOG
    SoundManager::getInstance()->stopBGMAll();
	SoundManager::getInstance()->playSE("gameStart.mp3");
	PlayerDataManager::getInstance()->setGameStart(0);
    Director::getInstance()->replaceScene(DungeonScene::create(DungeonSceneData::create(PlayerDataManager::getInstance()->getLocalData()->getLocation())));
}

// 続きからが選ばれた時
void TitleScene::onContinueSelected()
{
	FUNCLOG
    SoundManager::getInstance()->playSE(Resource::SE::TITLE_ENTER);
	this->mainMenu->hide();
	this->saveDataSelector->show();
}

// 終了が選ばれた時
void TitleScene::onExitSelected()
{
	FUNCLOG
    SoundManager::getInstance()->playSE(Resource::SE::BACK);
	Director::getInstance()->end();
}

// セーブデータ選択をキャンセルした時
void TitleScene::onSaveDataSelectCancelled()
{
	FUNCLOG
    SoundManager::getInstance()->playSE(Resource::SE::BACK);
	this->saveDataSelector->hide();
	this->mainMenu->show();
}

#pragma mark -
#pragma mark TrophyListLayer

// トロフィーリストを生成
void TitleScene::createTrophyListLayer()
{
    TrophyListLayer* trophyList {TrophyListLayer::create()};
    trophyList->onTrophyListCanceled = CC_CALLBACK_0(TitleScene::onTrophyListCanceled, this);
    trophyList->setVisible(false);
    this->addChild(trophyList);
    this->trophyList = trophyList;
}

// トロフィーリストを選択した時
void TitleScene::onTrophyListSelected()
{
    // クリアしていない場合は見れない
    if (!PlayerDataManager::getInstance()->getGlobalData()->isCleared())
    {
        SoundManager::getInstance()->playSE("failure.mp3");
        return;
    }
    
    // トロフイーリストレイヤーを作成
    SoundManager::getInstance()->playSE(Resource::SE::TITLE_ENTER);
    if(!this->trophyList)
    {
        this->createTrophyListLayer();
    }
    this->mainMenu->hide();
    this->trophyList->show();
}

// トロフィーリストをキャンセル時
void TitleScene::onTrophyListCanceled()
{
    SoundManager::getInstance()->playSE(Resource::SE::BACK);
    this->trophyList->hide();
    this->mainMenu->show();
}

