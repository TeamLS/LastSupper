//
//  SceneEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/25.
//
//

#include "Event/SceneEvent.h"

#include "Event/EventScriptValidator.h"
#include "Event/EventScriptMember.h"

#include "Scenes/DungeonScene.h"

#include "MapObjects/MapObjectList.h"
#include "MapObjects/Character.h"

#pragma mark ChangeMapEvent

bool ChangeMapEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    Direction direction {Direction::SIZE};

    // directionの指定がされている時
    if(this->validator->hasMember(json, member::DIRECTION))
    {
        direction = MapUtils::toEnumDirection(json[member::DIRECTION].GetString());
    }
    // directionが指定されていない時
    else
    {
        direction = this->validator->getMapObjectList()->getMainCharacter()->getDirection();
    }
    
    this->location = PlayerDataManager::Location(stoi(json[member::MAP_ID].GetString()), json[member::X].GetInt(), json[member::Y].GetInt(), direction);
    
    return true;
}

void ChangeMapEvent::run()
{
    PlayerDataManager::getInstance()->setLocation(this->location);
    this->setDone();
    Director::getInstance()->replaceScene(DungeonScene::createScene());
}

#pragma mark -
#pragma mark CameraEvent

bool CameraEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    return true;
}

void CameraEvent::run()
{
    
}

#pragma mark -
#pragma mark WaitEvent

bool WaitEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    float duration {static_cast<float>(json[member::TIME].GetDouble())};
    
    // 0秒指定だったらfalseを返す（falseを返すと生成されない）
    if(duration == 0.f) return false;
    
    this->duration = duration;
    
    return true;
}

void WaitEvent::run()
{
    this->validator->getScene()->runAction(Sequence::createWithTwoActions(DelayTime::create(this->duration), CallFunc::create([this](){this->setDone();})));
}