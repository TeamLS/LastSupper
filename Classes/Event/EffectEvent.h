﻿//
//  EffectEvent.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/02.
//
//

#ifndef LightEvent_h
#define LightEvent_h

#include "Event/GameEvent.h"

// 環境光の色を設定
class SetAmbientLightEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(SetAmbientLightEvent, rapidjson::Value&)
private:
    Color3B color {Color3B::BLACK};
private:
    SetAmbientLightEvent() { FUNCLOG };
    ~SetAmbientLightEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// アニメーション
class AnimationEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(AnimationEvent, rapidjson::Value&)
private:
    float delayPerUnit {0.2f};
    Vector<SpriteFrame*> spriteFrames {};
private:
    AnimationEvent() {FUNCLOG};
    ~AnimationEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

#endif /* EffectEvent_h */
