//
//  MenuLayer.h
//  LastSupper
//
//  Created by Kohei on 2015/07/05.
//
//

#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "Common.h"

class EventListenerKeyboardLayer;

class MenuLayer : public Layer
{
// インスタンス変数
protected:
    vector<Node*> menuObjects {};
    EventListenerKeyboardLayer* listenerKeyboard { nullptr };
    
private:
    int indexX { 0 };
    int indexY { 0 };
    int sizeX { 0 };
    int sizeY { 0 };

// インスタンスメソッド
public:
	virtual void show() = 0;
	virtual void hide() = 0;
protected:
	MenuLayer();
	~MenuLayer();
	virtual bool init(const Point& index, const Size& size);
	virtual bool init(int sizeX, int sizeY);
	virtual void onIndexChanged(int newIdx, bool sound = true) = 0;
    void onCursorKeyPressed(const Key& key);
    void onSpaceKeyPressed();
	virtual void onSpacePressed(int idx) = 0;
	virtual void onMenuKeyPressed() = 0;
	virtual int getSelectedIndex() const;
    virtual void setSelectedIndex(const int index);
	int getMenuSizeX() const;
	int getMenuSizeY() const;
};

#endif // __MENU_LAYER_H__
