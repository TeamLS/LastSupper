//
//  MenuLayer.cpp
//  LastSupper
//
//  Created by Kohei on 2015/07/05.
//
//

#include "Layers/Menu/MenuLayer.h"

#include "Layers/EventListener/EventListenerKeyboardLayer.h"

// コンストラクタ
MenuLayer::MenuLayer(){FUNCLOG}

// デストラクタ
MenuLayer::~MenuLayer(){FUNCLOG}

// 初期化 (ページ数指定有)
bool MenuLayer::init(const Size& size, const int page_size)
{
    this->page_size = page_size;
    return this->init(size.width, size.height);
}

// 初期化
bool MenuLayer::init(const Point& index, const Size& size)
{
	this->indexX = index.x;
	this->indexY = index.y;
	
	return this->init(size.width, size.height);
}

// 初期化
bool MenuLayer::init(int sizeX, int sizeY)
{
	FUNCLOG
	if(!Layer::init()) return false;
	
	// メニューの項目数を設定
	this->sizeX = sizeX;
	this->sizeY = sizeY;
    
    EventListenerKeyboardLayer* listenerKeyboard { EventListenerKeyboardLayer::create() };
    listenerKeyboard->onCursorKeyPressed = CC_CALLBACK_1(MenuLayer::onCursorKeyPressed, this);
    listenerKeyboard->onSpaceKeyPressed = CC_CALLBACK_0(MenuLayer::onSpaceKeyPressed, this);
    listenerKeyboard->onMenuKeyPressed = CC_CALLBACK_0(MenuLayer::onMenuKeyPressed, this);
    listenerKeyboard->intervalInputCheck = CC_CALLBACK_1(MenuLayer::intervalInputCheck, this);
    listenerKeyboard->setInputCheckDelay(0.3f);
    listenerKeyboard->setInputCheckInterval(0.1f);
    listenerKeyboard->setEnabled(false);
    this->addChild(listenerKeyboard);
    this->listenerKeyboard = listenerKeyboard;

	return true;
}

// 方向キーを押した時
void MenuLayer::onCursorKeyPressed(const Key& key)
{
    // カーソルが無効な場合は何もしない
    if(!this->cursorEnabled) return;
    
    // インデックスの変更
    switch(key)
    {
        case Key::UP:
            if (this->page_size > 1 && indexY == 0)
            {
                this->page = this->page == 0 ? this->page_size - 1 : this->page - 1;
                this->onPageChanged(this->page);
            }
            if(sizeY >= 2) this->indexY = (indexY == 0) ? indexY = sizeY - 1 : (indexY - 1) % sizeY;
            break;
            
        case Key::DOWN:
            if (this->page_size > 1 && indexY + 1 == sizeY)
            {
                this->page = this->page + 1 % this->page_size;
                this->onPageChanged(this->page);
            }
            if(sizeY >= 2) this->indexY = (indexY + 1) % sizeY;
            // ページチェック
            if (this->calcSelectedIndex() > this->menuObjects.size() - 1)
            {
                if (this->calcSelectedIndex() - sizeX < this->page * sizeX * sizeY)
                {
                    // 飛び先のページに要素が存在しないとき
                    this->indexX = (this->menuObjects.size() - 1) % sizeX;
                }
                else
                {
                    // 飛び先のページの同じindexXのところへ飛ぶ
                    this->page = 0;
                    this->indexY = 0;
                }
                this->onPageChanged(this->page);
                
            }
            break;
            
        case Key::LEFT:
            if(sizeX >= 2) this->indexX = (indexX == 0)? indexX = sizeX - 1 : (indexX - 1) % sizeX;
            break;
            
        case Key::RIGHT:
            if(sizeX >= 2) this->indexX = (indexX + 1) % sizeX;
            break;
            
        default:
            break;
    }
    
    // ヴィジュアルの更新
    this->onIndexChanged(this->getSelectedIndex());
    cout << "page>>" << this->page << endl <<"indexX>>" << indexX << endl << "indexY>>" << indexY << endl;
}

// スペースを押した時
void MenuLayer::onSpaceKeyPressed()
{
    this->onSpacePressed(this->getSelectedIndex());
}

// 現在選ばれているメニューのINDEXを取得(現時点では横優先配置の場合のみ)
int MenuLayer::getSelectedIndex()
{
    int selected = this->calcSelectedIndex();
    int maxIndex = this->menuObjects.size() - 1;
    if (selected > maxIndex){
        if (sizeY >= 2) this->indexY = ((int)floor((maxIndex + 1) / sizeX)) % sizeY;
        selected = this->calcSelectedIndex();
        if (selected > maxIndex)
        {
            if (selected - sizeX < this->page * sizeX * sizeY)
            {
                selected = maxIndex;
                if (sizeX >= 2) this->indexX = (maxIndex + 1) % sizeX;
                if (sizeY >= 2) this->indexY = ((int)floor((maxIndex + 1) / sizeX)) % sizeY;
            }
            else
            {
                indexY--;
                selected -= sizeX;
            }
        }
    }
    return selected;
}

int MenuLayer::calcSelectedIndex()
{
    return this->sizeX * this->indexY + this->indexX + this->page * this->sizeX * this->sizeY;
}

// メニューサイズの横方向を取得
int MenuLayer::getMenuSizeX() const
{return this->sizeX;}

// メニューサイズの縦方向を取得
int MenuLayer::getMenuSizeY() const
{return this->sizeY;}

// メニューのインデックスをデフォルトセット
void MenuLayer::setSelectedIndex(const int index)
{
    this->indexX = index % this->sizeX;
    this->indexY = (index / this->sizeX) % this->sizeY;
}

// カーソルの有効無効を設定
void MenuLayer::setCursorEnable(const bool enable)
{
    this->cursorEnabled = enable;
}

void MenuLayer::intervalInputCheck(const vector<Key> keys)
{
    this->onCursorKeyPressed(keys.back());
}

// ページサイズを取得
int MenuLayer::getPageSize(){return this->page_size;}