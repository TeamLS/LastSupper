//
//  DungeonMenuScene.cpp
//  LastSupper
//
//  Created by 猪野凌也 on 2015/10/04.
//
//

#include "Scenes/DungeonMenuScene.h"
#include "Layers/Menu/DungeonMainMenuLayer.h"
#include "Layers/Menu/SaveDataSelector.h"

// コンストラクタ
DungeonMenuScene::DungeonMenuScene(){FUNCLOG}

// デストラクタ
DungeonMenuScene::~DungeonMenuScene(){FUNCLOG}

// シーン生成
Scene* DungeonMenuScene::createScene(Texture2D* screen)
{
    Scene* scene = Scene::create();
    DungeonMenuScene* layer = DungeonMenuScene::create(screen);
    scene->addChild(layer);
    return scene;
}

// 初期化
bool DungeonMenuScene::init(Texture2D* screen)
{
    FUNCLOG
    
    //スクショを生成
    Sprite* screenSprite = Sprite::createWithTexture(screen);
    //screenSprite->setTextureRect(Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    screenSprite->setPosition(WINDOW_CENTER);
    screenSprite->setFlippedY(true);
    this->addChild(screenSprite);
    
    // メインメニューレイヤーを生成
    DungeonMainMenuLayer* menu = DungeonMainMenuLayer::create();
    menu->onSaveMenuSelected = CC_CALLBACK_0(DungeonMenuScene::onSaveMenuSelected, this);
    menu->onMenuHidden = CC_CALLBACK_0(DungeonMenuScene::onMenuHidden, this);
    this->addChild(menu);
    this->mainMenu = menu;
    this->mainMenu->show();
    
    // セーブデータ選択レイヤーを生成
    SaveDataSelector* saveDataSelector { SaveDataSelector::create(true) };
    this->addChild(saveDataSelector);
    
    // セーブデータ選択レイヤーのイベントをリッスン
    saveDataSelector->onSaveDataSelected = CC_CALLBACK_1(DungeonMenuScene::onSaveDataSelected, this);
    saveDataSelector->onSaveDataSelectCancelled = CC_CALLBACK_0(DungeonMenuScene::onSaveDataSelectCancelled, this);
    
    saveDataSelector->hide();
    this->saveDataSelector = saveDataSelector;
    return true;
}


void DungeonMenuScene::onPreloadFinished()
{
}

// 方向キーを押した時
void DungeonMenuScene::onCursorKeyPressed(const Key& key)
{
}

// スペースキーを押した時
void DungeonMenuScene::onSpaceKeyPressed()
{
}



// キーを押し続けている時
void DungeonMenuScene::intervalInputCheck(const vector<Key>& keys)
{
}

// メニューキー押したとき
void DungeonMenuScene::onMenuKeyPressed()
{
}

//メニューが削除されたとき
void DungeonMenuScene::onMenuHidden()
{
    FUNCLOG
    if(this->onPopMenuScene)
    {
        this->onPopMenuScene();
    }
    Director::getInstance()->popScene();
}

void DungeonMenuScene::onSaveMenuSelected()
{
    FUNCLOG
    this->mainMenu->hide();
    this->saveDataSelector->show();
}

// セーブデータが選ばれた時
void DungeonMenuScene::onSaveDataSelected(int dataId)
{
    FUNCLOG
    PlayerDataManager::getInstance()->setMainLocalData(dataId);
    SoundManager::getInstance()->playSound("se/failure.mp3");
}

// セーブデータ選択をキャンセルした時
void DungeonMenuScene::onSaveDataSelectCancelled()
{
    FUNCLOG
    SoundManager::getInstance()->playSound("se/back.mp3");
    runAction(Sequence::create(CallFunc::create([=](){this->saveDataSelector->hide();}), CallFunc::create([=](){this->mainMenu->show();}), nullptr));
}