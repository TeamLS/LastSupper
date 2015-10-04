//
//  MapObjectList.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/02.
//
//

#include "MapObjects/MapObjectList.h"

#include "MapObjects/MapObject.h"

// コンストラクタ
MapObjectList::MapObjectList() {FUNCLOG};

// デストラクタ
MapObjectList::~MapObjectList() {FUNCLOG};

// 初期化
bool MapObjectList::init()
{
    return true;
}

// 初期化
bool MapObjectList::init(const vector<MapObject*>& mapObjects)
{
    this->mapObjects = mapObjects;
    
    return true;
}

// 指定座標のマップオブジェクトを取得
MapObject* MapObjectList::getMapObject(const Point& position) const
{
    for(MapObject* obj : this->mapObjects)
    {
        Rect rect {obj->getCollisionRect()};
        if(rect.containsPoint(position)) return obj;
    }
    
    // 該当がなければnullptrを返す
    return nullptr;
}

// マップオブジェクトのベクタを取得
const vector<MapObject*>& MapObjectList::getMapObjects() const
{
    return this->mapObjects;
}

// マップオブジェクトを追加
void MapObjectList::add(MapObject* mapObject)
{
    this->mapObjects.push_back(mapObject);
}

// 主人公を設定
void MapObjectList::setMainCharacter(Character* mainChara)
{
    this->mainCharacter = mainChara;
}

// 主人公を取得
Character* MapObjectList::getMainCharacter() const
{
    return this->mainCharacter;
}