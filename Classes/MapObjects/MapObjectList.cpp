//
//  MapObjectList.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/02.
//
//

#include "MapObjects/MapObjectList.h"

#include "MapObjects/Party.h"
#include "MapObjects/TerrainObject/PlainArea.h"

// create関数
MapObjectList* MapObjectList::create(const Vector<MapObject*>& availableObjects, const Vector<MapObject*> disableObjects, const Vector<TerrainObject*> terrainObjects)
{
    MapObjectList* p {new(nothrow) MapObjectList()};
    if(p && p->init(availableObjects, disableObjects, terrainObjects))
    {
        p->autorelease();
        return p;
    }
    else
    {
        delete p;
        p = nullptr;
        return nullptr;
    }
}

// コンストラクタ
MapObjectList::MapObjectList() {FUNCLOG};

// デストラクタ
MapObjectList::~MapObjectList()
{
    FUNCLOG

    this->availableObjects.clear();
    this->disableObjects.clear();
    this->enemies.clear();
    this->terrainObjects.clear();
    CC_SAFE_RELEASE_NULL(this->plainArea);
};

// 初期化
bool MapObjectList::init(const Vector<MapObject*>& availableObjects, const Vector<MapObject*> disableObjects, const Vector<TerrainObject*> terrainObjects)
{
    this->availableObjects = availableObjects;
    this->disableObjects = disableObjects;
    this->terrainObjects = terrainObjects;
    
    // ノーマルの地形を生成
    PlainArea* plainArea { PlainArea::create() };
    CC_SAFE_RETAIN(plainArea);
    this->plainArea = plainArea;
    
    // 敵と主人公一行の衝突判定開始
    this->scheduleUpdate();
    
    return true;
}

// 指定IDのマップオブジェクトを取得
MapObject* MapObjectList::getMapObject(int objId) const
{
    for(MapObject* obj : this->availableObjects)
    {
        if(objId == obj->getObjectId())
        {
            return obj;
        }
    }
    
    return nullptr;
}

// 無効リストから指定IDのマップオブジェクトを取得
MapObject* MapObjectList::getMapObjectFromDisableList(int objId) const
{
    for(MapObject* obj : this->disableObjects)
    {
        if(objId == obj->getObjectId())
        {
            return obj;
        }
    }
    
    return nullptr;
}

// マップオブジェクトのベクタを取得
const Vector<MapObject*>& MapObjectList::getMapObjects() const
{
    return this->availableObjects;
}

// 指定範囲内にあるマップオブジェクトのベクタを取得
Vector<MapObject*> MapObjectList::getMapObjects(const Rect& rect) const
{
    Vector<MapObject*> mapObjects {};
    for(MapObject* obj : this->availableObjects)
    {
        if(rect.intersectsRect(obj->getCollisionRect())) mapObjects.pushBack(obj);
    }
    
    return mapObjects;
}

// 指定座標を含むマップオブジェクトのベクタを取得
Vector<MapObject*> MapObjectList::getMapObjects(const Point& position) const
{
    Vector<MapObject*> mapObjects {};
    for(MapObject* obj : this->availableObjects)
    {
        if(obj->getCollisionRect().containsPoint(position)) mapObjects.pushBack(obj);
    }
    
    return mapObjects;
}

// Rect(マス座標, マスサイズ)とトリガーからマップオブジェクトを取得
Vector<MapObject*> MapObjectList::getMapObjectsByGridRect(const Rect& gridRect, const Trigger trigger) const
{
    Vector<MapObject*> mapObjects {};
    bool needToCheckTrigger {trigger != Trigger::SIZE};
    
    for(MapObject* obj : this->availableObjects)
    {
        bool flag { false };
        
        Rect rect {obj->getGridRect()};
        
        if(MapUtils::intersectsGridRect(gridRect, rect))
        {
            if(!needToCheckTrigger) flag = true;
            
            if(obj->getTrigger() == trigger) flag = true;
        }
        
        if(flag) mapObjects.pushBack(obj);
    }
    
    return mapObjects;
}

// 指定トリガーのマップオブジェクトのEventIDベクタを取得
vector<int> MapObjectList::getEventIds(const Trigger trigger) const
{
    vector<int> eventIds {};
    
    for(MapObject* obj : this->availableObjects)
    {
        if(obj->getTrigger() == trigger) eventIds.push_back(obj->getEventId());
    }
    
    return eventIds;
}

// マスRectとTriggerからイベントIDを取得
vector<int> MapObjectList::getEventIdsByGridRect(const Rect& gridRect, const Trigger trigger) const
{
    vector<int> ids {};
    
    for(MapObject* obj : this->getMapObjectsByGridRect(gridRect, trigger))
    {
        if(obj->getEventId() == etoi(EventID::UNDIFINED)) continue;
        ids.push_back(obj->getEventId());
    }
    
    return ids;
}

// 指定範囲内にあたり判定を持つマップオブジェクトが存在するか
const bool MapObjectList::containsCollisionObject(const Rect& rect) const
{
    for(MapObject* obj : this->getMapObjects(rect))
    {
        if(obj->isHit()) return true;
    }
    
    return false;
}

// 当たり判定を持つオブジェクトのマスRectを全て取得
vector<Rect> MapObjectList::getGridCollisionRects() const
{
    vector<Rect> gridRects {};
    
    for(MapObject* obj : this->availableObjects)
    {
        if(!obj->isHit()) continue;
        
        gridRects.push_back(obj->getGridRect());
    }
    
    return gridRects;
}

// マップオブジェクトを追加
void MapObjectList::add(MapObject* mapObject)
{
    // もうすでにリスト内にあればリターン
    if(this->availableObjects.find(mapObject) != this->availableObjects.end()) return;
    
    this->availableObjects.pushBack(mapObject);
    
    std::mutex mtx;
    
    // 無効リストに存在する場合は削除
    for(MapObject* obj : this->disableObjects)
    {
        if(mapObject->getObjectId() == obj->getObjectId())
        {
            mtx.lock();
            this->disableObjects.eraseObject(mapObject);
            mtx.unlock();
            
            break;
        }
    }
}

// マップオブジェクトを削除
void MapObjectList::removeById(const int objectId)
{
    mutex mtx;
    
    for(MapObject* obj : this->availableObjects)
    {
        if(obj->getObjectId() == objectId)
        {
            mtx.lock();
            obj->removeFromParent();
            this->availableObjects.eraseObject(obj);
            mtx.unlock();
        }
    }
}

#pragma mark -
#pragma mark Enemy

// 敵を追加
void MapObjectList::addEnemy(Enemy* enemy)
{
    this->enemies.pushBack(enemy);
}

// 敵を削除
void MapObjectList::removeEnemyById(const int enemyId)
{
    std::mutex mtx;
    
    for(Enemy* enemy : this->enemies)
    {
        if(enemy->getEnemyId() == enemyId)
        {
            mtx.lock();
            enemy->removeFromParent();
            this->enemies.eraseObject(enemy);
            mtx.unlock();
        }
    }
}

// 敵を全て取得
Vector<Enemy*> MapObjectList::getEnemiesAll()
{
    return this->enemies;
}

// 敵が存在するか
bool MapObjectList::existsEnemy() const
{
    return !this->enemies.empty();
}

#pragma mark -
#pragma mark Party

// 主人公一行を格納
void MapObjectList::setParty(Party* party)
{
    // 一行が動いた時のコールバックを設定
    party->onPartyMoved = CC_CALLBACK_1(MapObjectList::onPartyMoved, this);
    
    this->party = party;
}

// 主人公一行を取得
Party* MapObjectList::getParty()
{
    return this->party;
}

// 主人公一行が移動した時
void MapObjectList::onPartyMoved(const Rect& gridRect)
{
    // 敵に移動後の主人公のマス座標を通知する
    for(Enemy* enemy : this->enemies)
    {
        enemy->onPartyMoved(gridRect);
    }
}

#pragma mark -
#pragma mark TerrainObject

// 地形オブジェクトをマスRectから取得
TerrainObject* MapObjectList::getTerrainByGridRect(const Rect& gridRect)
{
    for(TerrainObject* obj : this->terrainObjects)
    {
        if(!MapUtils::includesGridRect(obj->getGridRect(), gridRect)) continue;
        
        return obj;
    }
    
    // 何も見つからなかった場合はノーマル地形を返す
    return this->plainArea;
}

#pragma mark -
#pragma mark update

// 敵と主人公一行の衝突監視用updateメソッド
void MapObjectList::update(float delta)
{
    // partyがnullptrまたは、敵が一人もいない時は処理を中止
    if(!this->party || this->enemies.empty()) return;
    
    for(MapObject* obj : this->enemies)
    {
        // 主人公と敵が一体でもぶつかっていれば、コールバック呼び出し
        if(obj->getCollisionRect().intersectsRect(party->getMainCharacter()->getCollisionRect()))
        {
            // スケジュール終了
            this->unscheduleUpdate();
            if(!this->onContactWithEnemy) return;
            this->onContactWithEnemy();
        }
    }
}
