﻿//
//  EventListenerKeyboardLayer.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/23.
//
//

#include "Layers/EventListener/EventListenerKeyboardLayer.h"

// 定数
// キー変換用連想配列
const map<EventKeyboard::KeyCode, Key> EventListenerKeyboardLayer::keyMap =
{
    {EventKeyboard::KeyCode::KEY_UP_ARROW, Key::UP},
    {EventKeyboard::KeyCode::KEY_W, Key::UP},
    {EventKeyboard::KeyCode::KEY_DOWN_ARROW, Key::DOWN},
    {EventKeyboard::KeyCode::KEY_S, Key::DOWN},
    {EventKeyboard::KeyCode::KEY_LEFT_ARROW, Key::LEFT},
    {EventKeyboard::KeyCode::KEY_A, Key::LEFT},
    {EventKeyboard::KeyCode::KEY_RIGHT_ARROW, Key::RIGHT},
    {EventKeyboard::KeyCode::KEY_D, Key::RIGHT},
    {EventKeyboard::KeyCode::KEY_X, Key::MENU},
    {EventKeyboard::KeyCode::KEY_UNDERSCORE, Key::MENU},
    {EventKeyboard::KeyCode::KEY_SHIFT, Key::DASH},
    {EventKeyboard::KeyCode::KEY_SPACE, Key::SPACE},
};

// コンストラクタ
EventListenerKeyboardLayer::EventListenerKeyboardLayer(){ FUNCLOG }

// デストラクタ
EventListenerKeyboardLayer::~EventListenerKeyboardLayer(){ FUNCLOG }

// 初期化
bool EventListenerKeyboardLayer::init()
{
    if(!Layer::init()) return false;
    
    EventListenerKeyboard* listenerKeyboard { EventListenerKeyboard::create() };
    listenerKeyboard->onKeyPressed = CC_CALLBACK_1(EventListenerKeyboardLayer::onKeyPressed, this);
    listenerKeyboard->onKeyReleased = CC_CALLBACK_1(EventListenerKeyboardLayer::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKeyboard, this);
    this->listenerKeyboard = listenerKeyboard;
    
    return true;
}

// リスナを有効/無効化
void EventListenerKeyboardLayer::setEnabled(bool enabled)
{
    this->releaseKeyAll();
    this->listenerKeyboard->setEnabled(enabled);
}

// キーを押した瞬間から初回キー入力確認までの時間を設定
void EventListenerKeyboardLayer::setInputCheckDelay(float delay)
{
    this->delay = delay;
}

// キー入力の確認間隔を設定
void EventListenerKeyboardLayer::setInputCheckInterval(float interval)
{
    if(this->interval == interval) return;
    
    this->interval = interval;
    
    // すでにスケジュールされていたら、一旦スケジュール停止して新たなインターバルで再開する
    if(this->isScheduled(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck)))
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck));
        this->schedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck), interval);
    }
}

// キーを押した時
void EventListenerKeyboardLayer::onKeyPressed(const EventKeyboard::KeyCode& keyCode)
{
    Key key {this->convertKeyCode(keyCode)};
    if(key == Key::SIZE) return;
    this->keyStatus[key] = true;
    switch (key) {
        case Key::UP:
        case Key::DOWN:
        case Key::LEFT:
        case Key::RIGHT:
            if(this->onCursorKeyPressed && !this->paused) this->onCursorKeyPressed(key);
            this->pressingKeys.push_back(key);
            // 方向キーを押した時は、入力チェック用にスケジューリング
            this->scheduleIntervalCheck();
            if(this->pressingKeys.size() == 1) this->scheduleDelayedCheck();
            break;
            
        case Key::SPACE:
            if(this->onSpaceKeyPressed && !this->paused) this->onSpaceKeyPressed();
            break;
            
        case Key::MENU:
            if(this->onMenuKeyPressed && !this->paused) this->onMenuKeyPressed();
            break;
            
        case Key::DASH:
            if(this->onDashKeyPressed && !this->paused) this->onDashKeyPressed();
            break;
            
        default:
            break;
    }
}

// キーを離した時
void EventListenerKeyboardLayer::onKeyReleased(const EventKeyboard::KeyCode& keyCode)
{
    Key key {this->convertKeyCode(keyCode)};
    this->releaseKey(key);
}

//　キーを離すとき
void EventListenerKeyboardLayer::releaseKey(const Key& key)
{
    if(key == Key::SIZE) return;
    this->keyStatus[key] = false;
    if(find(this->pressingKeys.begin(), this->pressingKeys.end(), key) != this->pressingKeys.end()) this->pressingKeys.erase(remove(this->pressingKeys.begin(), this->pressingKeys.end(), key));
    if(this->pressingKeys.empty())
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck));
        this->unschedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::delayedCheck));
    }
}

// 全てのキーを強制リリースする
void EventListenerKeyboardLayer::releaseKeyAll()
{
    for(int key = 0; key < static_cast<int>(Key::SIZE); key++)
    {
        this->releaseKey(static_cast<Key>(key));
    }
}

// キーを押し続けている時
void EventListenerKeyboardLayer::intervalCheck(float duration)
{
    if(this->paused) return;
    if(this->intervalInputCheck) this->intervalInputCheck(this->pressingKeys);
}

// キーを一定時間押した時
void EventListenerKeyboardLayer::delayedCheck(float duration)
{
    if(this->paused) return;
    if(this->delayedInputCheck) this->delayedInputCheck(this->pressingKeys);
}

// キーコードを変換。ゲームで使わないキーが与えられた場合はSIZEを返す
Key EventListenerKeyboardLayer::convertKeyCode(const EventKeyboard::KeyCode& keyCode)
{return (keyMap.count(keyCode) == 0)?Key::SIZE:keyMap.at(keyCode);}

// 指定のキーが押し状態か判別
bool EventListenerKeyboardLayer::isPressed(const Key& key)
{
    if(this->keyStatus.count(key) == 0) return false;
    return this->keyStatus.at(key);
}

void EventListenerKeyboardLayer::setPaused(bool paused)
{
    this->paused = paused;
    
    if(paused)
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck));
        this->unschedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::delayedCheck));
    }
    else
    {
        if(!this->pressingKeys.empty())
        {
            this->scheduleIntervalCheck();
            this->scheduleDelayedCheck();
        }
    }
}

// 入力されている方向キーを取得
vector<Key> EventListenerKeyboardLayer::getPressedCursorKeys() const
{
    return this->pressingKeys;
}

void EventListenerKeyboardLayer::scheduleIntervalCheck()
{
    if(this->isScheduled(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck))) this->unschedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck));
    if(this->intervalInputCheck) this->schedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck), this->interval, CC_REPEAT_FOREVER, this->delay);
}

void EventListenerKeyboardLayer::scheduleDelayedCheck()
{
    if(this->isScheduled(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::delayedCheck))) this->unschedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::delayedCheck));
    if(this->delayedInputCheck) this->scheduleOnce(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::delayedCheck), this->delay);
}