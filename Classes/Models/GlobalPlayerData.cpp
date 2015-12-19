﻿//
//  GlobalPlayerData.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/04.
//
//

#include "Models/GlobalPlayerData.h"
#include "Managers/CsvDataManager.h"
#include "Managers/NotificationManager.h"
#include "Utils/JsonUtils.h"
#include "Utils/StringUtils.h"

const string GlobalPlayerData::GLOBAL_DATA_PATH {"save/global.json"};
const string GlobalPlayerData::GLOBAL_TEMPLATE_PATH {"save/global_template.json"};

const char* GlobalPlayerData::STATUS {"status"};
const char* GlobalPlayerData::GLOBAL_ID {"global_id"};
const char* GlobalPlayerData::CLEAR_COUNT {"clear_count"};
const char* GlobalPlayerData::BEST_SAVE_COUNT {"best_save_count"};
const char* GlobalPlayerData::BEST_CLEAR_TIME {"best_clear_time"};
const char* GlobalPlayerData::TROPHY {"trophy"};
const char* GlobalPlayerData::TOKENS {"tokens"};
const char* GlobalPlayerData::BGM_VOLUME {"bgm_volume"};
const char* GlobalPlayerData::SE_VOLUME {"se_volume"};

const int GlobalPlayerData::CHIKEN_SAVE_COUNT {80};
const int GlobalPlayerData::FAST_CLEAR_TIME {3600};

#pragma mark GlobalDataFile

// 初期化
bool GlobalPlayerData::init()
{
    if (this->loadGlobalData()) return true;
    if (this->initGlobalData()) return true;
    return false;
}

// グローバルデータのロード
bool GlobalPlayerData::loadGlobalData()
{
    string path = FileUtils::getInstance()->fullPathForFilename(GLOBAL_DATA_PATH);
    if (path == "") return false;
    
    this->globalData = LastSupper::JsonUtils::readJsonFile(path);
    if(this->globalData[STATUS].GetInt() == 0) return false;
    
    return true;
}

// グローバルデータの初期化
bool GlobalPlayerData::initGlobalData()
{
    string path = FileUtils::getInstance()->fullPathForFilename(GLOBAL_TEMPLATE_PATH);
    if (path == "") return false;
    
    this->globalData = LastSupper::JsonUtils::readJsonFile(path);
    LastSupper::JsonUtils::writeJsonFile(path, this->globalData);
    
    return true;
}

// グローバルデータのセーブ
void GlobalPlayerData::saveGlobalData(){LastSupper::JsonUtils::writeJsonFile(GLOBAL_DATA_PATH, this->globalData);}

#pragma mark -
#pragma mark ClearCount

// クリア回数をtokenチェックを行ってからセット
void GlobalPlayerData::setClearCount(const string& token)
{
    if (!this->isClearedToken(token))
    {
        int clear_count = this->globalData[CLEAR_COUNT].GetInt();
        if (clear_count < 999)
        {
            this->globalData[CLEAR_COUNT].SetInt(clear_count + 1);
        }
    }
}

// クリア回数を取得
int GlobalPlayerData::getClearCount(){return this->globalData[CLEAR_COUNT].GetInt();}

// クリアしているか
bool GlobalPlayerData::isCleared(){return this->getClearCount() > 0 ? true : false;}

#pragma mark -
#pragma mark MinimumSaveCount

// 最小セーブ記録をセット
void GlobalPlayerData::setBestSaveCount(const int save_count)
{
    // トロフィーチェック
    if (save_count >= CHIKEN_SAVE_COUNT) this->setTrophy(9); // チキン料理人
    if (save_count == 0) this->setTrophy(11); // マエストロ
    
    if (save_count > this->getBestSaveCount()) return;
    this->globalData[BEST_SAVE_COUNT].SetInt(save_count);
}

// セーブ回数の最小記録を取得
int GlobalPlayerData::getBestSaveCount(){return this->globalData[BEST_SAVE_COUNT].GetInt();}

#pragma mark -
#pragma mark BestClearTime

// 最短クリア時間をセット
void GlobalPlayerData::setBestClearTime(const int play_time)
{
    if (play_time > this->getBestClearTimeSecound()) return;
    this->globalData[BEST_CLEAR_TIME].SetInt(play_time);
    
    // トロフィーチェック
    if (play_time < FAST_CLEAR_TIME) this->setTrophy(10); // 3分クッキング
}

// 最短クリア時間を取得
int GlobalPlayerData::getBestClearTimeSecound(){return this->globalData[BEST_CLEAR_TIME].GetInt();}

#pragma mark -
#pragma mark TrophyData

// トロフィーゲット処理
void GlobalPlayerData::setTrophy(const int trophy_id)
{
    char tid_char[10];
    sprintf(tid_char, "%d", trophy_id);
    rapidjson::Value tid  (kStringType);
    tid.SetString(tid_char, strlen(tid_char), this->globalData.GetAllocator());
    rapidjson::Value::ConstMemberIterator itr = this->globalData[TROPHY].FindMember(tid_char);
    
    // 指定したトロフィーIDが存在するかチェック
    if(itr == this->globalData[TROPHY].MemberEnd()){
        // keyを作成してtrueを登録
        this->globalData[TROPHY].AddMember(tid, rapidjson::Value(true), this->globalData.GetAllocator());
        
        // トロフィーゲット通知
        NotificationManager::getInstance()->notifyTrophy(trophy_id);
        
        // グローバルデータをセーブ　　　
        this->saveGlobalData();
    }
}

// 指定のトロフィーを持っているか
bool GlobalPlayerData::hasTrophy(const int trophy_id)
{
    bool hasTrophy {false};
    char tid_char[10];
    sprintf(tid_char, "%d", trophy_id);
    
    if (this->globalData[TROPHY].HasMember(tid_char))
    {
        if (this->globalData[TROPHY][tid_char].GetBool())
        {
            hasTrophy =  true;
        }
    }
    return hasTrophy;
}

// トロフィーコンプリート処理
void GlobalPlayerData::setTrophyComplete()
{
    vector<int> trophies = CsvDataManager::getInstance()->getTrophyIdAll();
    int trophy_count {0};
    for(int trophy_id : trophies)
    {
        if (!this->hasTrophy(trophy_id)) trophy_count++;
    }
    if (trophy_count == trophies.size())
    {
        this->setTrophy(trophy_count);
    }
}

#pragma mark -
#pragma mark Token

// トークンをセット
void GlobalPlayerData::setClearToken(const string& token)
{
    rapidjson::Value jtoken  (kStringType);
    jtoken.SetString(token.c_str(), strlen(token.c_str()), this->globalData.GetAllocator());
    this->globalData[TOKENS].PushBack(jtoken, this->globalData.GetAllocator());
}

// クリア済みのデータかチェックする
bool GlobalPlayerData::isClearedToken(const string &token)
{
    int token_count = this->globalData[TOKENS].Size();
    for (int i = 0; i < token_count; i++)
    {
        if (this->globalData[TOKENS][i].GetString() == token)
        {
            return true;
        }
    }
    return false;
}

#pragma mark -
#pragma mark Volume

// BGMのマスターボリュームをセット
void GlobalPlayerData::setBgmVolume(const float& volume)
{
    this->globalData[BGM_VOLUME].SetDouble(volume);
}

// BGMのマスターボリュームをゲット
float GlobalPlayerData::getBgmVolume()
{
    return this->globalData[BGM_VOLUME].GetDouble();
}

// SEのマスターボリュームをセット
void GlobalPlayerData::setSeVolume(const float &volume)
{
    this->globalData[SE_VOLUME].SetDouble(volume);
}

// SEのマスターボリュームをゲット
float GlobalPlayerData::getSeVolume()
{
    return this->globalData[SE_VOLUME].GetDouble();
}