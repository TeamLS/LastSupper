﻿//
//  Character.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/06/15.
//
//

#include "MapObjects/Character.h"

#include "Datas/MapObject/CharacterData.h"

#include "MapObjects/MovePatterns/MovePattern.h"
#include "MapObjects/MovePatterns/MovePatternFactory.h"

// キャラのプロパティリストのディレクトリ
const string Character::basePath = "img/character/";

// コンストラクタ
Character::Character(){FUNCLOG}

// デストラクタ
Character::~Character()
{
    FUNCLOG
    
    CC_SAFE_RELEASE(this->movePattern);
}

// 初期化
bool Character::init(const CharacterData& data)
{
	if(!Node::init()) return false;
    
	// 生成時の情報をセット
    this->charaId = data.chara_id;
	this->location = data.location;
    this->setObjectId(data.obj_id);
    this->texturePrefix = CsvDataManager::getInstance()->getCharaFileName(charaId);
    
    if(this->movePattern)
    {
        // 動きのアルゴリズムを生成
        MovePatternFactory* factory { MovePatternFactory::create() };
        CC_SAFE_RETAIN(factory);
        this->movePattern = factory->createMovePattern(data.move_pattern, this);
        CC_SAFE_RETAIN(this->movePattern);
        CC_SAFE_RELEASE(factory);
    }
    
	// Spriteを生成
    this->setSprite(Sprite::createWithSpriteFrameName(this->texturePrefix + "_" + to_string(static_cast<int>(data.location.direction)) +"_0.png"));
    
    // サイズ、衝突判定範囲をセット
    this->setContentSize(this->getSprite()->getContentSize());
    this->setCollisionRect(Rect(0, 0, this->getContentSize().width, this->getContentSize().height / 2));
	
    for(int i {0}; i < static_cast<int>(Direction::SIZE); i++)
	{
        // 右足だけ動くタイプと左足だけ動くタイプでアニメーションを分ける
        for(int k = 0; k < 2; k++)
        {
            Animation* pAnimation = Animation::create();
            
            // それぞれの向きのアニメーション用画像を追加していく
            pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(this->texturePrefix + "_" + to_string(i) + "_" + to_string(k + 1) + ".png"));
            
            // キャッシュに保存
            AnimationCache::getInstance()->addAnimation(pAnimation, this->texturePrefix + to_string(i) + to_string(k));
        }
	}
	return true;
}

// キャラクタIDを取得
int Character::getCharacterId() const {return this->charaId;}

// 自身のキャラクターデータを返す
CharacterData Character::getCharacterData() const { return CharacterData(this->charaId, this->getObjectId(), this->location); }

// キャラクターの向きを変える
void Character::setDirection(const Direction direction)
{
    MapObject::setDirection(direction);
    
	// 画像差し替え
	this->getSprite()->setSpriteFrame(this->texturePrefix + "_" + to_string(static_cast<int>(direction)) + "_0.png");
}

// 足踏み
void Character::stamp(const Direction direction, float ratio)
{
    this->getSprite()->stopAllActions();
    
    Animation* anime = AnimationCache::getInstance()->getAnimation(this->texturePrefix + to_string(static_cast<int>(direction)) + to_string(this->stampingState < 2 ? 0 : 1));
    this->stampingState++;
    if(this->stampingState > 3) this->stampingState = 0;
    anime->setDelayPerUnit(DURATION_MOVE_ONE_GRID / ratio);
    
    this->getSprite()->runAction(Animate::create(anime));
    this->getSprite()->runAction(Sequence::createWithTwoActions(DelayTime::create(DURATION_MOVE_ONE_GRID / ratio), CallFunc::create([this]{this->setDirection(this->getDirection());})));
}

// 方向を指定して歩行させる
bool Character::walkBy(const Direction& direction, function<void()> onWalked, const float ratio, const bool back)
{
    vector<Direction> directions {direction};
    
    return this->walkBy(directions, onWalked, ratio, back);
}

// 方向を指定して歩行させる
bool Character::walkBy(const vector<Direction>& directions, function<void()> onWalked, const float ratio, const bool back)
{
    if(!MapObject::moveBy(directions, onWalked, ratio)) return false;
    
    this->setDirection(back ? MapUtils::oppositeDirection(directions.back()) : directions.back());
    
    this->stamp(directions.back(), ratio);
    
    return true;
}

// 方向とマス数してで歩行させる
void Character::walkBy(const Direction& direction, const int gridNum, function<void(bool)> callback, const float ratio, const bool back)
{
    vector<Direction> directions {direction};
    
    this->walkBy(directions, gridNum, callback, ratio, back);
}

// 方向とマス数指定で歩行させる
void Character::walkBy(const vector<Direction>& directions, const int gridNum, function<void(bool)> callback, const float ratio, const bool back)
{
    if(directions.empty() || this->isMoving()) return;
    
    deque<vector<Direction>> directionsQueue {};
    
    // 方向をキューに登録
    for(int i { 0 }; i < gridNum; i++)
    {
        directionsQueue.push_back(directions);
    }
    
    // キューに登録した歩行を実行
    this->walkByQueue(directionsQueue, callback, ratio, back);
}

// キューで歩行させる
void Character::walkByQueue(deque<Direction> directionQueue, function<void(bool)> callback, const float ratio, const bool back)
{
    if(directionQueue.empty())
    {
        if(callback) callback(true);
        
        return;
    }
    
    deque<vector<Direction>> directionsQueue {};
    
    for(Direction direction : directionQueue)
    {
        directionsQueue.push_back(vector<Direction>({direction}));
    }
    
    this->walkByQueue(directionsQueue, callback, ratio, back);
}

// キューで歩行させる
void Character::walkByQueue(deque<vector<Direction>> directionsQueue, function<void(bool)> callback, const float ratio, const bool back)
{
    // 初回のみ中身が存在するため、空でない時は格納する
    if(!directionsQueue.empty()) this->directionsQueue = directionsQueue;
    
    // キューが空になったら成功としてコールバックを呼び出し
    if(this->directionsQueue.empty())
    {
        if(callback) callback(true);
        
        return;
    }
    
    // キューの先頭を実行
    vector<Direction> directions { this->directionsQueue.front() };
    this->directionsQueue.pop_front();
    
    // 移動開始。失敗時はコールバックを失敗として呼び出し
    if(this->walkBy(directions, [callback, ratio, back, this]{this->walkByQueue(deque<vector<Direction>>({}), callback, ratio, back);}, ratio, back)) return;
    
    if(callback) callback(false);
}

// 周りを見渡す
void Character::lookAround(function<void()> callback)
{
    this->setDirection(this->convertToWorldDir(Direction::RIGHT));
    this->runAction(Sequence::create(DelayTime::create(1.f), CallFunc::create([this]{this->setDirection(this->convertToWorldDir(Direction::BACK));}), DelayTime::create(1.f), CallFunc::create([callback]{callback();}), nullptr));
}

// マップに配置された時
void Character::onEnterMap()
{
    if(this->movePattern) this->movePattern->start();
}

// 主人公一行が動いた時
void Character::onPartyMoved()
{
    if(this->movePattern) this->movePattern->onPartyMoved();
}

// 調べられた時
void Character::onSearched(MapObject* mainChara)
{
    // 主人公の反対の方向を向かせる（向かいあわせる）
    this->setDirection(MapUtils::oppositeDirection(mainChara->getDirection()));
}
