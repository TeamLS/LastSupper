//
//  PlayerDataManager.h
//  LastSupper
//
//  Created by 猪野凌也 on 2015/06/28.
//
//

#ifndef __LastSupper__PlayerDataManager__
#define __LastSupper__PlayerDataManager__

#include "define.h"

class CharacterData;

class PlayerDataManager
{
//構造体
public:
    struct SaveIndex
    {
        int data_id {0};
        string chapter {};
        string map_name {};
        string play_time {};
        string save_count {};
        SaveIndex(int i, const string& chap, const string& mn, const string& pt, const string& sc):data_id(i), chapter(chap), map_name(mn), play_time(pt), save_count(sc){};
        SaveIndex(){};
    };
    

//インスタンス変数
private:
    // グローバルセーブデータ
    rapidjson::Document global;
    // ローカルセーブデータ
    rapidjson::Document local;
    // 対象ローカルセーブデータid
    int local_id {0};
    // FileUtils
    cocos2d::FileUtils* fu;
    // 時間計測開始時間(単位:ms)
    double start_time_ms {0};
    // ローカルセーブデータの存在の有無
    bool local_exist[MAX_SAVE_COUNT];

//通常関数
public:
    //メインとなるローカルデータのセット
    void setMainLocalData(const int id);
    //ローカルセーブデータリストの取得
    vector<SaveIndex> getSaveList();
    //セーブ
    void save(const int id);
    // セーブデータが存在するかチェック
    bool checkSaveDataExists(const int id);
    // セーブデータIDの取得
    int getSaveDataId();
    // プレイ時間の取得
    int getPlayTimeSeconds();
    // 時間取得
    double getSec();
    /* flag管理系 */
    // SET
    void setLocation(const Location& location, const int num = 0);
    void setFriendship(const int chara_id, const int level);
    void setEventFlag(const int map_id, const int event_id, const bool flag);
    void setPeculiarFlag(const int flag_id, const bool flag);
    void setItem(const int item_id);
    void setItemEquipment(const Direction direction, const int item_id);
    void setChapterId(const int chapter_id);
    void setCharacterProfile(const int chara_id, const int level);
    void setPartyMember(const CharacterData& chara);
    
    // REMOVE
    bool removeItem(const int item_id);
    bool removePartyMember(const int chara_id);
    
    // GET
    Location getLocation(const int num = 0);
    int getFriendship(const int chara_id);
    bool getEventFlag(const int map_id, const int event_id);
    bool getPeculiarFlag(const int flag_id);
    int getItem(const int item_id);
    map<int, int> getItemAll();
    int getItemEquipment(Direction direction);
    int getChapterId();
    int getCharacterProfileLevel(const int chara_id);
    CharacterData getPartyMember(const int num = 0);
    vector<CharacterData> getPartyMemberAll();
    
    // CHECK
    bool checkItem(const int item_id);
    bool checkItemEquipment(const int item_id);
    bool checkFriendship(const int chara_id, const int val);
    bool checkChapterId(const int chapter_id);
    
private:
    // グローバルデータのセット
    bool setGlobalData();
    // セーブデータを全初期化
    void initializeFiles();
    // JSONファイル読み込み
    rapidjson::Document readJsonFile(const string& path);
    // JSONファイル書き出し
    void writeJsonFile(const string& path, const rapidjson::Document& doc);
    // rapidjsoのstringを生成
//    rapidjson::Value& getRapidjsonString(const char* str);
//    rapidjson::Value& getRapidjsonString(const int num);
//singleton用関数
public:
    static PlayerDataManager* getInstance();
    static void  destroy();
private:
    PlayerDataManager();    //コンストラクタ
    ~PlayerDataManager();   //デストラクタ
    PlayerDataManager(const PlayerDataManager& other){};              //コピーコンストラクタ
    PlayerDataManager& operator = (const PlayerDataManager& other); //代入演算子
    
};
#endif /* defined(__LastSupper__PlayerDataManager__) */
