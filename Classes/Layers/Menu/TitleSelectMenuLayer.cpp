//
//  TitleSelectMenuLayer.cpp
//  6chefs
//
//  Created by Kohei Asami on 2017/09/17.
//
//

#include "Layers/Menu/TitleSelectMenuLayer.h"

// コンストラクタ
TitleSelectMenuLayer::TitleSelectMenuLayer() { FUNCLOG }

// デストラクタ
TitleSelectMenuLayer::~TitleSelectMenuLayer() { FUNCLOG }

// 初期化
bool TitleSelectMenuLayer::init(function<void(int)> onSelectTitle)
{
    if (!MenuLayer::init(2, 1)) return false;
    _onSelectTitle = onSelectTitle;
    
    // 背景
    Sprite* bg { Sprite::createWithSpriteFrameName("title_select_bg.png") };
    Size bgSize { bg->getContentSize() };
    bg->setPosition(bgSize.width / 2, bgSize.height / 2);
    this->addChild(bg);
    
    // 1タイトル名
    Sprite* title1 { Sprite::createWithSpriteFrameName("title_select_1.png") };
    title1->setPosition(60, 470);
    this->addChild(title1);
    
    // 2タイトル名
    Sprite* title2 { Sprite::createWithSpriteFrameName("title_select_2.png") };
    title2->setPosition(730, 130);
    this->addChild(title2);
    
    // 雪
    Sprite* yuki { Sprite::createWithSpriteFrameName("title_select_yuki.png") };
    Size yukiSize { yuki->getContentSize() };
    yuki->setPosition(yukiSize.width / 2 + 380, yukiSize.height / 2);
    this->addChild(yuki);
    
    // 孫一
    Sprite* magoichi { Sprite::createWithSpriteFrameName("title_select_magoichi.png") };
    Size magoSize { magoichi->getContentSize() };
    magoichi->setPosition(magoSize.width / 2 + 80, magoSize.height / 2);
    this->addChild(magoichi);
    
    return true;
}

// 表示
void TitleSelectMenuLayer::show()
{
    
}

// 非表示
void TitleSelectMenuLayer::hide()
{
    
}

// インデックスが変わった時
// カーソルを移動させたりする
void TitleSelectMenuLayer::onIndexChanged(int newIdx, bool sound)
{
    
}

// 決定キー押した時
void TitleSelectMenuLayer::onEnterKeyPressed(int idx)
{
    
}
