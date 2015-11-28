//
//  TitleScene.h
//  LastSupper
//
//  Created by Kohei on 2015/06/05.
//
//

#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "Scenes/BaseScene.h"

class TitleMainMenuLayer;
class SaveDataSelector;

class TitleScene : public BaseScene
{
	// 列挙型
	enum struct MenuType
	{
		BEGINING,
		FINISH,
		SIZE,
	};
	// クラスメソッド
public:
	CREATE_FUNC(TitleScene);
	
	// インスタンスメソッド
private:
	virtual bool init();
    virtual void onEnter();
	void onPreloadFinished(LoadingLayer* loadingLayer);
	void onStartSelected();
	void onContinueSelected();
	void onExitSelected();
	void onSaveDataSelected(int dataId);
	void onSaveDataSelectCancelled();
public:
	TitleScene();
	~TitleScene();
	
	// インスタンス変数
	TitleMainMenuLayer* mainMenu { nullptr };
	SaveDataSelector* saveDataSelector { nullptr };
};

#endif // __TITLE_SCENE_H__