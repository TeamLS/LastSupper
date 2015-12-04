//
//  EnemyTask.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/21.
//
//

#ifndef EnemyTask_h
#define EnemyTask_h

#include "Tasks/GameTask.h"

#include "Datas/MapObject/EnemyData.h"

class Enemy;

class EnemyTask : public GameTask
{
// クラスメソッド
public:
    CREATE_FUNC(EnemyTask);

// インスタンス変数
private:
    vector<SummonData> datas {};
    int currentMapId { etoi(MapID::UNDIFINED) };
public:
    function<void()> onAllEnemyRemoved { nullptr };
    
// インスタンスメソッド
private:
    EnemyTask();
    ~EnemyTask();
    bool init();
public:
    void start(const int mapId);
    void stop();
    void removeEnemy(const int enemyId);
    void update(float delta);
    vector<SummonData> createDatas(const Vector<Enemy*>& enemies, const Location& destLocation, const Location& currentLocation);
    bool existsEnemy() const;
};

#endif /* EnemyTask_h */
