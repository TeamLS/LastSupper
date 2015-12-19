﻿//
//  AmbientLightLayer.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/10.
//
//

#ifndef _AmbientLightLayer__
#define _AmbientLightLayer__

#include "Common.h"

#include "Effects/Light.h"

class Light;

class AmbientLightLayer : public Layer
{
// 定数
public:
    static const Color3B DAY;
    static const Color3B EVENING;
    static const Color3B ROOM;
	static const Color3B NIGHT;
    static const Color3B BASEMENT;
    
// クラスメソッド
public:
	CREATE_FUNC_WITH_PARAM(AmbientLightLayer, const Color3B&)
    
// インスタンス変数
private:
    Sprite* ambientLight { nullptr };
    map<Light*, Light*> lightMap {};
    RenderTexture* renderTexture { nullptr };
    Sprite* renderTexSprite { nullptr };

// インスタンスメソッド
public:
    void setAmbient(const Color3B& color);
    void addLightSource(Light* lightSource);
    void removeLightSource(Light* lightSource);
    virtual void visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags);
private:
    AmbientLightLayer();   // コンストラクタ
    ~AmbientLightLayer();  // デストラクタ
    bool init(const Color3B& color);    // 初期化
};

#endif /* defined(_AmbientLightLayer__) */
