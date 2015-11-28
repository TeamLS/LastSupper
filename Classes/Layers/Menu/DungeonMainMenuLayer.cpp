//
//  DungeonMainMenuLayer.cpp
//  LastSupper
//
//  Created by Kohei on 2015/09/06.
//
//

#include "Layers/Menu/DungeonMainMenuLayer.h"

#include "Layers/EventListener/EventListenerKeyboardLayer.h"
#include "Scenes/TitleScene.h"
#include "Datas/MapObject/CharacterData.h"
#include "Layers/Menu/MiniSelector.h"
#include "UI/SlideNode.h"

// 定数
const float DungeonMainMenuLayer::SLIDE_TIME {0.3f};

// コンストラクタ
DungeonMainMenuLayer::DungeonMainMenuLayer(){FUNCLOG}

// デストラクタ
DungeonMainMenuLayer::~DungeonMainMenuLayer(){FUNCLOG}

// 初期化
bool DungeonMainMenuLayer::init()
{
	FUNCLOG
	if(!MenuLayer::init(static_cast<int>(Type::SIZE), 1)) return false;
	
    //半透明の黒幕を生成
	Sprite* cover { Sprite::create() };
	cover->setTextureRect(Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	cover->setColor(Color3B::BLACK);
	cover->setPosition(WINDOW_CENTER);
	cover->setOpacity(0);
	this->addChild(cover);
    this->cover = cover;
    
	// ヘッダーメニューを生成
    Sprite* hBg { Sprite::createWithSpriteFrameName("main_menu_panel.png") };
    Size hBgSize {hBg->getContentSize()};
    hBg->setScaleY(1.1f);
    
    // ヘッダーメニュースライドノードにセット
    Point outPosition {Point(hBgSize.width / 2, WINDOW_HEIGHT + hBgSize.height / 2)};
    Point inPosition {Point(hBgSize.width/2, WINDOW_HEIGHT - hBgSize.height/2)};
    SlideNode* header {SlideNode::create(inPosition, outPosition)};
    header->addChild(hBg);
	this->addChild(header);
    this->slideNodes.push_back(header);
    
    // チャプター表示
    int chapter_id = PlayerDataManager::getInstance()->getChapterId();
    Label* chapter_name = Label::createWithTTF(CsvDataManager::getInstance()->getChapterName(chapter_id), "fonts/cinecaption2.28.ttf", 30);
    float chapter_name_y = WINDOW_HEIGHT - hBg->getContentSize().height - chapter_name->getContentSize().height / 2 - 15;
    inPosition = Point(chapter_name->getContentSize().width/2 + 15, chapter_name_y);
    outPosition = Point(-1 * chapter_name->getContentSize().width / 2, chapter_name_y);
    SlideNode* chap1 {SlideNode::create(inPosition, outPosition)};
    chap1->addChild(chapter_name);
    cover->addChild(chap1);
    this->slideNodes.push_back(chap1);
    
    Label* chapter_title = Label::createWithTTF(CsvDataManager::getInstance()->getChapterTitle(chapter_id), "fonts/cinecaption2.28.ttf", 30);
    float chapter_title_y = WINDOW_HEIGHT - hBg->getContentSize().height - chapter_name->getContentSize().height - chapter_title->getContentSize().height / 2 - 30;
    inPosition = Point(chapter_title->getContentSize().width / 2 + 45, chapter_title_y);
    outPosition = Point(-1 * chapter_title->getContentSize().width / 2, chapter_title_y);
    SlideNode* chap2 {SlideNode::create(inPosition, outPosition)};
    chap2->addChild(chapter_title);
    cover->addChild(chap2);
    this->slideNodes.push_back(chap2);
    
    // メニューの選択肢を生成
    map<Type, string> menuStrings
    {
        {Type::ITEM, "アイテム"},
        { Type::SAVE, "セーブ" },
        { Type::CHARA, "キャラ" },
        { Type::TITLE, "タイトル" },
        { Type::CLOSE, "閉じる" },
    };
    
    for(int i {0}; i < static_cast<int>(Type::SIZE); i++)
    {
        Label* menu = Label::createWithTTF(menuStrings.at(static_cast<Type>(i)), "fonts/cinecaption2.28.ttf", 26);
        menu->setPosition((WINDOW_WIDTH / static_cast<int>(Type::SIZE)) * (i + 0.5), 40);
        menu->setOpacity(100);
        menu->setTag(i);
        hBg->addChild(menu);
        this->menuObjects.push_back(menu);
    }
    
    // マップ名表示
    Label* mapName = Label::createWithTTF(CsvDataManager::getInstance()->getMapName(PlayerDataManager::getInstance()->getLocation().map_id), "fonts/cinecaption2.28.ttf", 26);
    mapName->setPosition(mapName->getContentSize().width / 2 + 15, hBg->getContentSize().height - mapName->getContentSize().height / 2 - 15);
    hBg->addChild(mapName);
    
    // プレイ時間表示
    Label* play_time = Label::createWithTTF(PlayerDataManager::getInstance()->getPlayTimeDisplay(), "fonts/cinecaption2.28.ttf", 26);
    play_time->setPosition(hBg->getContentSize().width - play_time->getContentSize().width/2 - 15, hBg->getContentSize().height - play_time->getContentSize().height / 2 - 15);
    hBg->addChild(play_time);
    this->play_time = play_time;
    
	// 下のメニューを生成
    Sprite* fBg { Sprite::createWithSpriteFrameName("main_menu_panel.png") };
    Size fBgSize {fBg->getContentSize()};
    fBg->setScale(1.f, 1.11f);
    inPosition = Point(fBgSize.width/2, fBgSize.height/2);
    outPosition = Point(fBgSize.width / 2, -1 * fBgSize.height / 2);
    SlideNode* footer {SlideNode::create(inPosition, outPosition)};
    this->addChild(footer);
    footer->addChild(fBg);
	this->slideNodes.push_back(footer);
    
    // 装備品表示
    int right_id = PlayerDataManager::getInstance()->getItemEquipment(Direction::RIGHT);
    int left_id = PlayerDataManager::getInstance()->getItemEquipment(Direction::LEFT);
    string right = (right_id != 0) ? CsvDataManager::getInstance()->getItemName(right_id) : "なし";
    string left = (left_id != 0) ? CsvDataManager::getInstance()->getItemName(left_id) : "なし";
    Label* equipment = Label::createWithTTF("装備\n右手 : " + right + "\n左手 : " + left, "fonts/cinecaption2.28.ttf", 26);
    equipment->setPosition(equipment->getContentSize().width / 2 + 15, fBg->getContentSize().height - equipment->getContentSize().height / 2 - 15);
    fBg->addChild(equipment);
    
    // キャラ表示
    vector<CharacterData> charas = PlayerDataManager::getInstance()->getPartyMemberAll();
    int party_count = charas.size();
    Size  cPanelSize = Size(fBg->getContentSize().width/5, fBg->getContentSize().height);
    float stand_scale = 0.25;
    for (int i = 0; i < party_count; i++)
    {
        float colum_position = cPanelSize.width * (5 - party_count + i) + cPanelSize.width / 2;
        // キャラ毎にパネルを作成
        Sprite* chara_panel {Sprite::create()};
        chara_panel->setTextureRect(Rect(0,0, cPanelSize.width, cPanelSize.height));
        chara_panel->setPosition(colum_position, cPanelSize.height / 2);
        chara_panel->setOpacity(0);
        fBg->addChild(chara_panel);
        
        // 通り名
        Label* street= Label::createWithTTF("-" + CsvDataManager::getInstance()->getCharaStreetName(charas[i].chara_id) + "-", "fonts/cinecaption2.28.ttf", 24);
        street->setPosition(cPanelSize.width / 2, cPanelSize.height - street->getContentSize().height / 2 - 10);
        chara_panel->addChild(street);
        
        // キャラ名
        Label* name = Label::createWithTTF(CsvDataManager::getInstance()->getCharaName(charas[i].chara_id), "fonts/cinecaption2.28.ttf", 24);
        name->setPosition(cPanelSize.width / 2, cPanelSize.height - street->getContentSize().height - name->getContentSize().height /2 - 20);
        chara_panel->addChild(name);
        
        // キャラのドット絵
        string file_name = charas[i].getDotFileName();
        if (SpriteFrameCache::getInstance()->getSpriteFrameByName(file_name))
        {
            Sprite* dotimg {Sprite::createWithSpriteFrameName(file_name)};
            dotimg->setPosition(cPanelSize.width / 2, dotimg->getContentSize().height / 2 + 15);
            dotimg->setZOrder(1);
            chara_panel->addChild(dotimg);
        }
        
        // キャラの立ち絵
        file_name = charas[i].getStandFileName();
        if(SpriteFrameCache::getInstance()->getSpriteFrameByName(file_name))
        {
            Sprite* img { Sprite::createWithSpriteFrameName(file_name)};
            img->setScale(stand_scale);
            float img_y = img->getContentSize().height * stand_scale / 2 + fBg->getContentSize().height + 10;
            inPosition = Point(colum_position, img_y);
            outPosition = Point(WINDOW_WIDTH + img->getContentSize().width * stand_scale / 2, img_y);
            SlideNode* stand {SlideNode::create(inPosition, outPosition)};
            stand->addChild(img);
            cover->addChild(stand);
            this->slideNodes.push_back(stand);
        }
        
    }
    
	return true;
}

// メニュー表示
void DungeonMainMenuLayer::show()
{
	FUNCLOG
    // プレイ時間を1秒ごとにアップデート登録
    this->schedule(schedule_selector(DungeonMainMenuLayer::updateTime), 1);
    this->slideIn();
    this->cover->runAction(EaseCubicActionInOut::create(FadeTo::create(0.3f, 64)));
    this->onIndexChanged(this->menuIndex);
    this->setVisible(true);
    this->listenerKeyboard->setEnabled(true);
}

// メニューを非表示
void DungeonMainMenuLayer::hide()
{
	FUNCLOG
    this->slideOut();
    this->cover->runAction(EaseCubicActionInOut::create(FadeTo::create(0.3f, 0)));
    this->listenerKeyboard->setEnabled(false);
}

// カーソル移動時
void DungeonMainMenuLayer::onIndexChanged(int newIdx, bool sound)
{
    this->menuIndex = newIdx;
    // カーソル処理
    for(Node* obj : this->menuObjects)
    {
        if(obj->getTag() == newIdx)
        {
            obj->runAction(FadeTo::create(0.1f, 255));
            obj->runAction(ScaleTo::create(0.2f, 1.2f));
        }
        else
        {
            obj->runAction(FadeTo::create(0.1f, 128));
            obj->runAction(ScaleTo::create(0.2f, 1.0f));
        }
        obj->runAction(TintTo::create(0.5f, 255, 255, 255));
    }
    if(sound)SoundManager::getInstance()->playSE("cursorMove.mp3");
    return;
}

// 決定キー入力時
void DungeonMainMenuLayer::onSpacePressed(int idx)
{
    SoundManager::getInstance()->playSE("cursorMove.mp3");
    switch (static_cast<Type>(idx)) {
        case Type::ITEM:
            if (this->onItemMenuSelected)
            {
                this->onItemMenuSelected();
            }
            this->onItemMenuSelected();
            break;
        case Type::SAVE:
            if (this->onSaveMenuSelected)
            {
                this->onSaveMenuSelected();
            }
            break;
        case Type::CHARA:
            if (this->onCharacterMenuSelected)
            {
                this->onCharacterMenuSelected();
            }
            break;
        case Type::TITLE:
            this->confirmTitleback();
            break;
        case Type::CLOSE:
        default:
            this->onMenuKeyPressed();
            break;
    }
}

// タイトルへ戻る確認画面
void DungeonMainMenuLayer::confirmTitleback()
{
    // アイテムメニューのキーボードを無効化
    this->listenerKeyboard->setEnabled(false);
    // メニューラベル
    vector<string> labels = {"本当に戻る", "キャンセル"};
    Point index = Point(1,labels.size()); // 要素数
    SpriteUtils::Square position = SpriteUtils::Square(35,40,65,60); // 位置
    MiniSelector::Selector selector = MiniSelector::Selector(index, position, labels);
    MiniSelector* mini = {MiniSelector::create(selector)};
    this->addChild(mini);
    mini->show();
    mini->onMiniSelectorCanceled = CC_CALLBACK_0(DungeonMainMenuLayer::onConfirmCanceled, this);
    mini->onMiniIndexSelected = CC_CALLBACK_1(DungeonMainMenuLayer::onConfirmSelected, this);
    this->confirm = mini;
}

// 確認画面
void DungeonMainMenuLayer::onConfirmCanceled()
{
    SoundManager::getInstance()->playSE("back.mp3");
    this->runAction(Sequence::createWithTwoActions(
        CallFunc::create([this](){this->confirm->hide();}),
        CallFunc::create([this](){this->listenerKeyboard->setEnabled(true);})
    ));
}

// タイトル画面へ戻る確認画面セレクター処理
void DungeonMainMenuLayer::onConfirmSelected(int idx)
{
    if (idx == 0)
    {
        // タイトルへ戻る
        SoundManager::getInstance()->playSE("back.mp3");
        Director::getInstance()->replaceScene(TitleScene::create());
    }
    else
    {
        // メニューへ戻る
        this->onConfirmCanceled();
    }
}

//　メニューキー入力時
void DungeonMainMenuLayer::onMenuKeyPressed()
{
    this->hide();
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(SLIDE_TIME),CallFunc::create([this]
    {
        if(this->onMenuHidden)
        {
            this->onMenuHidden();
        }
    })));
}

// 選択されているメニュー番号を取得
int DungeonMainMenuLayer::getMenuIndex(){
    return this->getMenuIndex();
}

// ノードをスライドイン
void DungeonMainMenuLayer::slideIn()
{
    int node_size = this->slideNodes.size();
    for (int i = 0; i < node_size; i++)
    {
        this->slideNodes[i]->slideIn();
    }
}

// ノードをスライドアウト
void DungeonMainMenuLayer::slideOut()
{
    int node_size = this->slideNodes.size();
    for (int i = 0; i < node_size; i++)
    {
        this->slideNodes[i]->slideOut();
    }
}

// プレイ時間をアップデート
void DungeonMainMenuLayer::updateTime(float delta)
{
    this->play_time->setString(PlayerDataManager::getInstance()->getPlayTimeDisplay());
}