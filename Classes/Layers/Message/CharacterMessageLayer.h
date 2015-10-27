//
//  CharacterMessageLayer.h
//  LastSupper
//
//  Created by Kohei on 2015/07/12.
//
//

#ifndef __CHARACTER_MESSAGE_LAYER_H__
#define __CHARACTER_MESSAGE_LAYER_H__

#include "Layers/Message/MessageLayer.h"

#include "cocos-ext.h"

class CharacterMessageData;

class CharacterMessageLayer : public MessageLayer
{
// 定数
private:
    static const float TOP_MARGIN;
    static const float LEFT_MARGIN;
    static const float H_MARGIN_S;
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_TWO_PARAM(CharacterMessageLayer, const queue<CharacterMessageData*>&, function<void()>)
	
// インスタンス変数
private:
    queue<CharacterMessageData*> datas {};
    ui::Scale9Sprite* nameFrame { nullptr };

// インスタンスメソッド
private:
	CharacterMessageLayer();
	~CharacterMessageLayer();
    virtual bool init(const queue<CharacterMessageData*>& datas, function<void()> onCloseCallback);
	virtual Label* createMessage() override;
    virtual bool hasNextPage() override;
};

#endif // __CHARACTER_MESSAGE_LAYER_H__
