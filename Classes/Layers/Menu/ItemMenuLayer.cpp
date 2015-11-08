//
//  ItemMenuLayer.cpp
//  LastSupper
//
//  Created by 猪野凌也 on 2015/11/01.
//
//

#include "ItemMenuLayer.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"
#include "Scenes/DungeonScene.h"

// コンストラクタ
ItemMenuLayer::ItemMenuLayer(){FUNCLOG}

// デストラクタ
ItemMenuLayer::~ItemMenuLayer(){FUNCLOG}

// ItemMenuLayer生成
bool ItemMenuLayer::init()
{
    FUNCLOG
    int obj_count = PlayerDataManager::getInstance()->getItemAll().size();
    int sizeX = obj_count > 1 ? 2 : 1;
    int sizeY = obj_count > 20 ? 10 : floor(obj_count/ 2) + 1;
    if (!MenuLayer::init(sizeX, sizeY)) return false;
    
    SpriteUtils::Square square;
    SpriteUtils::Margin margin;
    
    // 白い背景を生成
    Sprite* white = Sprite::create();
    white->setTextureRect(Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    white->setColor(Color3B::WHITE);
    white->setPosition(WINDOW_CENTER);
    //white->setOpacity(128);
    this->addChild(white);
    
    // タイトル
    square = SpriteUtils::Square(0,80,30,100);
    margin = SpriteUtils::Margin(3.0,1.5,1.5,3.0);
    Sprite* leftTop = SpriteUtils::getSquareSprite(square, margin);
    leftTop->setColor(Color3B(128,0,0));
    this->addChild(leftTop);
    
    Label* title = Label::createWithTTF("アイテム", "fonts/cinecaption2.28.ttf", 48);
    title->setPosition(leftTop->getContentSize().width / 2, leftTop->getContentSize().height / 2);
    title->setColor(Color3B::WHITE);
    leftTop->addChild(title);
    
    // アイテム詳細
    square = SpriteUtils::Square(0,0,30,80);
    margin = SpriteUtils::Margin(1.5,1.5,3.0,3.0);
    Sprite* leftBottom = SpriteUtils::getSquareSprite(square, margin);
    leftBottom->setColor(Color3B::BLACK);
    leftBottom->setName("leftBottom");
    this->addChild(leftBottom);
    
    // 装備
    square = SpriteUtils::Square(30,80,100,100);
    margin = SpriteUtils::Margin(3.0,3.0,1.5,1.5);
    Sprite* rightTop = SpriteUtils::getSquareSprite(square, margin);
    rightTop->setColor(Color3B::BLACK);
    this->addChild(rightTop);
    
    int right_id = PlayerDataManager::getInstance()->getItemEquipment(Direction::RIGHT);
    int left_id = PlayerDataManager::getInstance()->getItemEquipment(Direction::LEFT);
    string right_equip = (right_id != 0) ? CsvDataManager::getInstance()->getItemName(right_id) : "なし";
    string left_equip = (left_id != 0) ? CsvDataManager::getInstance()->getItemName(left_id) : "なし";
    Label* equipment = Label::createWithTTF("装備\n右手 : " + right_equip + "\n左手 : " + left_equip, "fonts/cinecaption2.28.ttf", 26);
    equipment->setPosition(equipment->getContentSize().width/2 + 10, rightTop->getContentSize().height/2);
    equipment->setColor(Color3B::WHITE);
    rightTop->addChild(equipment);
    
    // アイテムリスト
    square = SpriteUtils::Square(30,0,100,80);
    margin = SpriteUtils::Margin(1.5,3.0,3.0,1.5);
    Sprite* rightBottom = SpriteUtils::getSquareSprite(square, margin);
    rightBottom->setColor(Color3B::BLACK);
    this->addChild(rightBottom);
    
    map<int,int> items = PlayerDataManager::getInstance()->getItemAll();
    if (items.size() == 0){
        // アイテムが時は空用の番号をインサート
        items.insert({-1,0});
    }
    int i = 0;
    for(auto itr = items.begin(); itr != items.end(); itr++)
    {
        // パネル生成
        Sprite* panel = Sprite::create();
        Size list_size {rightBottom->getContentSize()};
        panel->setTextureRect(Rect(0, 0, list_size.width / 2, list_size.height / 10));
        panel->setColor(Color3B::BLACK);
        panel->setTag(i);
        Size panel_size {panel->getContentSize()};
        panel->setPosition((i%2) * (list_size.width / 2) + panel_size.width/2, list_size.height - ((floor(i/2) + 1)  *  (list_size.height/10)) + panel_size.height/2);
        rightBottom->addChild(panel);
        // メニューオブジェクトに登録
        this->menuObjects.push_back(panel);
        // 不透明度を半分にしておく
        panel->setCascadeOpacityEnabled(true);
        panel->setOpacity(100);
        
        // アイテム
        this->items.push_back(itr->first);
        Label* item = Label::createWithTTF(CsvDataManager::getInstance()->getItemName(itr->first), "fonts/cinecaption2.28.ttf", 24);
        item->setPosition(panel_size.width/2 , panel_size.height / 2);
        item->setColor(Color3B::WHITE);
        panel->addChild(item);
        // インクリメント
        i++;
    }
    // デフォルトセレクト
    this->onIndexChanged(0, false);
    
    return true;
}

// アイテム説明部分更新
void ItemMenuLayer::changeItemDiscription(const int idx)
{
    // 親のスプライトを取得
    Node* leftBottom = this->getChildByName("leftBottom");
    // 子のスプライトがすでに存在すれば消してから生成
    string labelName = "discriptionLabel";
    if (leftBottom->getChildByName(labelName)){
        leftBottom->removeChildByName(labelName);
    }
    Label* discription = Label::createWithTTF(CsvDataManager::getInstance()->getItemDiscription(this->items[idx]), "fonts/cinecaption2.28.ttf", 18);
    discription->setPosition(leftBottom->getContentSize().width / 2, leftBottom->getContentSize().height / 2);
    discription->setColor(Color3B::WHITE);
    discription->setName(labelName);
    leftBottom->addChild(discription);
}

// 表示
void ItemMenuLayer::show()
{
    this->listenerKeyboard->setEnabled(true);
    this->setVisible(true);
}

// 非表示
void ItemMenuLayer::hide()
{
    this->listenerKeyboard->setEnabled(false);
    this->setVisible(false);
}

// 戻る
void ItemMenuLayer::onMenuKeyPressed()
{
    FUNCLOG
    if(this->onItemMenuCanceled)
    {
        this->onItemMenuCanceled();
    }
}

// スペースキーを押した時
void ItemMenuLayer::onSpacePressed(int idx)
{
    SoundManager::getInstance()->playSound("se/failure.mp3");
}

// 選択対象が変わった時
void ItemMenuLayer::onIndexChanged(int newIdx, bool sound)
{
    if (sound)
    {
        SoundManager::getInstance()->playSound("se/cursorMove.mp3");
    }
    for(Node* obj : this->menuObjects)
    {
        if(obj->getTag() == newIdx)
        {
            obj->runAction(FadeTo::create(0.2f, 255));
        }
        else
        {
            obj->runAction(FadeTo::create(0.2f, 100));
        }
    }
    // アイテム詳細を更新
    this->changeItemDiscription(newIdx);
}