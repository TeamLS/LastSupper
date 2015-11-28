//
//  PlayerControlTask.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/30.
//
//

#ifndef __LastSupper__ControlMainCharacterTask__
#define __LastSupper__ControlMainCharacterTask__

#include "Tasks/GameTask.h"

class EventListenerKeyboardLayer;
class Party;

class PlayerControlTask : public GameTask
{
// 定数
private:
    static const string START_WALKING_SCHEDULE_KEY;

// クラスメソッド
public:
    CREATE_FUNC(PlayerControlTask)

// インスタンス変数
private:
    int riddenEventID { static_cast<int>(EventID::UNDIFINED)};
    bool enableControl { true };
    
// インスタンスメソッド
private:
    PlayerControlTask();
    ~PlayerControlTask();
    bool init();
public:
    void turn(const Key& key, Party* party);                  // 向きを変える
    void search(Party* party);                                // 目の前を調べる
    void walking(const vector<Key>& keys, Party* party);      // 歩行中
    void onPartyMovedOneGrid(Party* party);                   // 一マス分移動し終えた時
    void setControlEnable(bool enable, Party* party);         // 操作可能状態かどうか設定
};

#endif /* defined(__LastSupper__ControlMainCharacterTask__) */
