//
//  GameEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/23.
//
//

#include "Event/GameEvent.h"

#include "Event/EventFactory.h"
#include "Event/EventScriptValidator.h"
#include "Event/EventScriptMember.h"

#pragma mark GameEvent

//コンストラクタ
GameEvent::GameEvent() {};

// デストラクタ
GameEvent::~GameEvent()
{
    CC_SAFE_RELEASE_NULL(this->factory);
    CC_SAFE_RELEASE_NULL(this->validator);
};

// 初期化
bool GameEvent::init()
{
    EventFactory* factory {EventScriptManager::getInstance()->getFactory()};
    EventScriptValidator* validator {EventScriptManager::getInstance()->getValidator()};
    
    if(!factory || !validator) return false;
    
    CC_SAFE_RETAIN(factory);
    this->factory = factory;
    
    CC_SAFE_RETAIN(validator);
    this->validator = validator;
    
    return true;
}

// イベントが終了しているか
bool GameEvent::isDone() const
{
    return _isDone;
}

// イベントを終了状態にする
void GameEvent::setDone()
{
    this->_isDone = true;
}

// EventIDもしくはaction配列からイベントを生成
GameEvent* GameEvent::createSpawnFromIdOrAction(rapidjson::Value& json)
{
    // eventIDの指定があれば、指定のIDに対応するjsonから生成
    if(this->validator->hasMember(json, member::EVENT_ID))
    {
        return this->factory->createGameEvent(EventScriptManager::getInstance()->getScript(json[member::EVENT_ID].GetInt()));
    }
    // eventIDの指定がなければ、action配列から生成
    else
    {
        return this->factory->createGameEvent(json[member::ACTION]);
    }
}

#pragma mark -
#pragma mark EventSequence

// Sequence
bool EventSequence::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    this->events = this->factory->createEventQueue(json);
    
    return true;
}

void EventSequence::run()
{
    if(this->events.size() == 0) return;
    
    // 最初のイベントを開始
    this->events.front()->run();
}

void EventSequence::update(float delta)
{
    this->events.front()->update(delta);
    
    if(this->events.front()->isDone())
    {
        CC_SAFE_RELEASE(this->events.front());
        this->events.pop();
        
        // 次のイベントがあればを開始
        if(!this->events.empty())
        {
           this->events.front()->run();
        }
        // 次のイベントがなければ終了
        else
        {
            this->setDone();
        }
    }
}

#pragma mark -
#pragma mark EventSpawn

// Spawn
bool EventSpawn::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    this->events = this->factory->createEventVector(json);
    
    return true;
}

void EventSpawn::run()
{
    if(this->events.size() == 0) return;
    
    for(GameEvent* event : this->events)
    {
        event->run();
    }
}

void EventSpawn::update(float delta)
{
    // 持っているイベントを更新し、終了したイベントを削除していく
    for (GameEvent* event : this->events)
    {
        event->update(delta);
        
        if (event->isDone())
        {
            this->events.eraseObject(event);
        }
    }
    
    // ベクタが空になったら終了
    if(this->events.empty())
    {
        this->setDone();
    }
}

#pragma mark -
#pragma mark EventIf

// If
bool EventIf::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // conditionをチェックしてtrueであればイベントを生成
    if(this->validator->detectCondition(json))
    {
        this->event = this->createSpawnFromIdOrAction(json);
        CC_SAFE_RETAIN(this->event);
    }
    // falseの場合はその場で終了
    else
    {
        this->setDone();
    }
    
    return true;
}

void EventIf::run()
{
    if(!this->event)
    {
        this->setDone();
        return;
    }
    
    this->event->run();
}

void EventIf::update(float delta)
{
    if(!this->event) return;
    
    this->event->update(delta);
    
    if(this->event->isDone())
    {
        this->setDone();
        CC_SAFE_RELEASE_NULL(this->event);
    }
}