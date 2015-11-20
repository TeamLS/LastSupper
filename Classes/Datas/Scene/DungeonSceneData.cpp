//
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
	FUNCLOG
    
    this->initialLocation = location;
    
    // イベントスクリプト生成
    EventScript* eventScript {EventScript::create(CsvDataManager::getInstance()->getMapFileName(location.map_id))};
    CC_SAFE_RETAIN(eventScript);
    this->eventScript = eventScript;

	// プリロード用リストを取得
    this->soundFilePaths = eventScript->getPreLoadList("sound");
    this->textureFilePaths = eventScript->getPreLoadList("texture");
	
	this->textureFilePaths.push_back("frame");
	this->textureFilePaths.push_back("ui");
    this->textureFilePaths.push_back("obj");
	
	return true;
}

// 初期位置データを取得
Location DungeonSceneData::getInitialLocation() const
{
    return this->initialLocation;
}

// イベントスクリプトを取得
EventScript* DungeonSceneData::getEventScript() const
{
    return this->eventScript;
}

#pragma mark -
#pragma mark Cover

// カバー情報をセット
void DungeonSceneData::setCoverInfo(const CoverInfo& coverInfo)
{
    this->coverInfo = coverInfo;
}

// カバーを残すか確認
bool DungeonSceneData::remainsCover() const
{
    return this->coverInfo.first;
}

// カバー色を取得
Color3B DungeonSceneData::getCoverColor() const
{
    return this->coverInfo.second;
}