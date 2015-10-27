//
//  ModalLayerEvent.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/25.
//
//

#ifndef __LastSupper__MessageEvent__
#define __LastSupper__MessageEvent__

#include "Event/GameEvent.h"

class CharacterMessageData;
class StoryMessageData;
class SystemMessageData;

class ModalLayerEvent : public GameEvent
{
// インスタンスメソッド
protected:
    ModalLayerEvent() {FUNCLOG};
    ~ModalLayerEvent() {FUNCLOG};
    virtual bool init() override;
    void onOpenModalLayer();
public:
    void onCloseModalLayer();
};

// キャラクターメッセージイベント
class CharacterMessage : public ModalLayerEvent
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(CharacterMessage, rapidjson::Value&)

// インスタンス変数
private:
    queue<CharacterMessageData*> datas {};

// インスタンスメソッド
private:
    CharacterMessage() {FUNCLOG};
    ~CharacterMessage() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    void run() override;
};

// ストーリメッセージイベント
class StoryMessage : public ModalLayerEvent
{
public:
    CREATE_FUNC_WITH_PARAM(StoryMessage, rapidjson::Value&)
    
private:
    string title {};
    queue<StoryMessageData*> datas {};
    
private:
    StoryMessage():title("") {FUNCLOG};
    ~StoryMessage() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// システムメッセージイベント
class SystemMessage : public ModalLayerEvent
{
public:
    CREATE_FUNC_WITH_PARAM(SystemMessage, rapidjson::Value&)

private:
    queue<SystemMessageData*> datas {};
    
private:
    SystemMessage() {FUNCLOG};
    ~SystemMessage() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

#endif /* defined(__LastSupper__MessageEvent__) */
