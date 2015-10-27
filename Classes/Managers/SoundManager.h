//
//  SoundManager.h
//  LastSupper
//
//  Created by Kohei on 2015/06/22.
//
//

#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "define.h"

class SoundManager
{
// 定数
public:
    static const string bgmPath;
    static const string sePath;

// クラスメソッド
public:
	static SoundManager* getInstance();
	static void destory();
	
// シングルトンであるためにprivateに
private:
	SoundManager();												// コンストラクタ
	SoundManager(const SoundManager& other);					// デストラクタ
	SoundManager& operator = (const SoundManager& other);		// 代入演算子
	~SoundManager();											// デストラクタ

public:
	void preloadSound(const string& filePath);
	void playSound(const string& filePath, bool loop = false, float volume = 1.0f);
	void unloadAllSounds();
	
	// インスタンス変数
private:
	map<string, int> soundMap;
};

#endif // __SOUND_MANAGER_H__
