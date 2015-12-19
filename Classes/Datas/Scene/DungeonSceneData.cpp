﻿//
//  DungeonSceneData.cpp
//  LastSupper
//
//  Created by Kohei on 2015/06/23.
//
//

#include "Datas/Scene/DungeonSceneData.h"

#include "Event/EventScript.h"

// コンストラクタ
DungeonSceneData::DungeonSceneData(){FUNCLOG}

// デストラクタ
DungeonSceneData::~DungeonSceneData()
{
    FUNCLOG

    CC_SAFE_RELEASE_NULL(this->eventScript);
}

// 初期化
bool DungeonSceneData::init(const Location& location)
{
    // ロケーション格納
    this->location = location;
    
    // イベントスクリプト生成
    EventScript* eventScript {EventScript::create(CsvDataManager::getInstance()->getMapFileName(location.map_id))};
    CC_SAFE_RETAIN(eventScript);
    this->eventScript = eventScript;

	// プリロード用リストを取得
    this->soundFilePaths = eventScript->getPreLoadList("sound");
    this->textureFilePaths = eventScript->getPreLoadList("texture");
	
	return true;
}

// イベントスクリプトを取得
EventScript* DungeonSceneData::getEventScript() const
{
    return this->eventScript;
}

// changeMap時に渡されるEventIDを設定
void DungeonSceneData::setInitialEventId(const int eventId)
{
    this->initEventId = eventId;
}

// changeMap時に渡されるEventIDを取得
int DungeonSceneData::getInitialEventId() const
{
    return this->initEventId;
}

// 場所を取得
Location DungeonSceneData::getLocation() const
{
    return this->location;
}