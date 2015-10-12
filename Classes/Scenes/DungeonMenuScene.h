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

class DungeonMenuScene : public baseScene
{
// クラスメソッド
public:
    static Scene* createScene(Texture2D* screen);
    CREATE_FUNC_WITH_PARAM(DungeonMenuScene, Texture2D*);
// インスタンス変数
private:
    DungeonMainMenuLayer* mainMenu {nullptr};
    SaveDataSelector* saveDataSelector {nullptr};
    int menuIndex {0};
    function<void()> onPopMenuScene;
    
// インスタンスメソッド
public:
    void onMenuHidden();
private:
    DungeonMenuScene();
    ~DungeonMenuScene();
    bool init(Texture2D* screen);
    virtual void onPreloadFinished() override;
    virtual void onCursorKeyPressed(const Key& key) override;
    virtual void onSpaceKeyPressed() override;
    virtual void onMenuKeyPressed() override;
    virtual void intervalInputCheck(const vector<Key>& keys);
    void onSaveMenuSelected();
    void onSaveDataSelected(int dataId);
    void onSaveDataSelectCancelled();
};

#endif /* defined(__LastSupper__DungeonMenuScene__) */