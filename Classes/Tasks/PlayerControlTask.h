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
// クラスメソッド
public:
    CREATE_FUNC(PlayerControlTask)

// インスタンス変数
private:
    int riddenEventID { static_cast<int>(EventID::UNDIFINED)};
    
// インスタンスメソッド
private:
    PlayerControlTask();
    ~PlayerControlTask();
    bool init();
public:
    void turn(const Key& key, Party* party);                                // 向きを変える
    void search(Party* party);                                // 目の前を調べる
    void walking(const vector<Key>& keys, Party* party);      // 歩行中
    void onPartyMovedOneGrid();                               // 一マス分移動し終えた時
};

#endif /* defined(__LastSupper__ControlMainCharacterTask__) */