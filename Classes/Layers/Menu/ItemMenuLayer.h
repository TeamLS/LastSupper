﻿//
//  ItemMenuLayer.hpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/01.
//
//

#ifndef ItemMenuLayer_h
#define ItemMenuLayer_h

#include "Layers/Menu/MenuLayer.h"

class MiniSelector;

class ItemMenuLayer : public MenuLayer
{
    // クラスメソッド
public:
    CREATE_FUNC(ItemMenuLayer)
    virtual bool init();
    
    // インスタンスメソッド
private:
    virtual void onIndexChanged(int newIdx, bool sound);
    virtual void onSpacePressed(int idx);
    virtual void onMenuKeyPressed();
    virtual void onPageChanged(const int page) override;
    void changeItemDiscription(const int idx);
    void createMiniSelector();
    void onMiniSelectorCanceled();
    void onMiniIndexSelected(const int idx);
    void moveUpDown(Node* target);
protected:
    ItemMenuLayer();
    ~ItemMenuLayer();
public:
    virtual void show() override;
    virtual void hide() override;
    
    // インスタンス変数
private:
    vector<int> items {};
    MiniSelector* miniSelector {nullptr};
    int selected_item {0};
    bool isEquip {true};
public:
    function<void()> onItemMenuCanceled { nullptr };
};
#endif /* ItemMenuLayer_h */
