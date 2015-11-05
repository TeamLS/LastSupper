//
//  DungeonMenuScene.h
//  LastSupper
//
//  Created by 猪野凌也 on 2015/10/04.
//
//

#ifndef __LastSupper__DungeonMenuScene__
#define __LastSupper__DungeonMenuScene__

#include "Scenes/baseScene.h"

class DungeonMainMenuLayer;
class SaveDataSelector;
class ItemMenuLayer;

class DungeonMenuScene : public baseScene
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_TWO_PARAM(DungeonMenuScene, Texture2D*, function<void()>);

// インスタンス変数
public:
    function<void()> onPopMenuScene;
private:
    DungeonMainMenuLayer* mainMenu {nullptr};
    SaveDataSelector* saveDataSelector {nullptr};
    ItemMenuLayer* itemMenu {nullptr};
    int menuIndex {0};
    
// インスタンスメソッド
public:
    void onMenuHidden();
private:
    DungeonMenuScene();
    ~DungeonMenuScene();
    bool init(Texture2D* screen, function<void()> onPopMenuScene);
    virtual void onPreloadFinished() override;
    virtual void onCursorKeyPressed(const Key& key) override;
    virtual void onSpaceKeyPressed() override;
    virtual void onMenuKeyPressed() override;
    virtual void intervalInputCheck(const vector<Key>& keys);
    void createMainMenu();
    void createSaveMenu();
    void createItemMenu();
    // メインメニュー
    void onSaveMenuSelected();
    void onItemMenuSelected();
    // セーブデータセレクター
    void onSaveDataSelected(int dataId);
    void onSaveDataSelectCancelled();
    // アイテムメニュー
    void onItemMenuCanceled();
};

#endif /* defined(__LastSupper__DungeonMenuScene__) */
