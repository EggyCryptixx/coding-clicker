#include "HelloWorldScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "extensions/cocos-ext.h"//ui
#include "ui/CocosGUI.h"//widgets
#include <vector>
#include "../cocos2d/cocos/editor-support/spine/Json.h"
USING_NS_CC;
USING_NS_CC_EXT;//ui features
using namespace cocos2d;
using namespace std;



Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    //storing visible screensize
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    size = 1;
    if(visibleSize.width == 2048 || visibleSize.height == 2048)
    {
        size = visibleSize.height / 1136;
        CCLOG("%f is the size of content scale", size);
    }
    //logs visible and original size. Testing purpose
    CCLOG("visibleSize is (%.2f, %.2f)", visibleSize.width, visibleSize.height);
    CCLOG("origin is (%.2f, %.2f)", origin.x, origin.y);
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    //////////////////
    // create and initialize a label/title
    auto label = Label::createWithTTF("Sign In", "fonts/Marker Felt.ttf", 45*size);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
    
    // add background image
    auto sprite = Sprite::create("greenbackground.png");
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    CCLOG("********%f, %f", sprite->getContentSize().width, sprite->getContentSize().height);

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    ////////////////////
    //editBox implementation*****************
    
    //username input
    ui::EditBox *userNameInput = ui::EditBox::create( Size(visibleSize.width/3, visibleSize.height/30), ui::Scale9Sprite::create("white.png"));
    //set textbox position with openGL coordinates
    userNameInput->setPosition(Vec2((visibleSize.width/2), visibleSize.height/1.15 + origin.y));
    //checking sprite position-testing purpose
    CCLOG("X and Y is: (%.2f, %.2f)",((visibleSize.width * 1.35)), (visibleSize.height));
    
    userNameInput->setFontSize(20*size);
    userNameInput->setFontColor(Color3B::BLACK);
    userNameInput->setPlaceHolder("Username");
    userNameInput->setPlaceholderFontSize(20);
    userNameInput->setPlaceholderFontColor(Color3B::GRAY);
    //limits the amount of characters the user may input
    userNameInput->setMaxLength(10);
    //type of return when finished typing
    userNameInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    userNameInput->setDelegate(this);
    this->addChild(userNameInput);
    
    //email address input
    ui::EditBox *emailAddressInput = ui::EditBox::create( Size(visibleSize.width/3, visibleSize.height/30), ui::Scale9Sprite::create("white.png"));
    //set textbox position with openGL coordinates
    emailAddressInput->setPosition(Vec2((visibleSize.width/2), visibleSize.height/1.23 + origin.y));
    emailAddressInput->setFontSize(20*size);
    emailAddressInput->setFontColor(Color3B::BLACK);
    emailAddressInput->setPlaceHolder("Email address");
    emailAddressInput->setPlaceholderFontSize(20);
    emailAddressInput->setPlaceholderFontColor(Color3B::GRAY);
    emailAddressInput->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
    //type of return when finished typing
    emailAddressInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    emailAddressInput->setDelegate(this);
    this->addChild(emailAddressInput);
    
    //email password input
    ui::EditBox *emailPasswordInput = ui::EditBox::create( Size(visibleSize.width/3, visibleSize.height/30), ui::Scale9Sprite::create("white.png"));
    //set textbox position with openGL coordinates
    emailPasswordInput->setPosition(Vec2((visibleSize.width/2), visibleSize.height/1.33 + origin.y));
    emailPasswordInput->setFontSize(20*size);
    emailPasswordInput->setFontColor(Color3B::BLACK);
    emailPasswordInput->setPlaceHolder("Email Password");
    emailPasswordInput->setPlaceholderFontSize(20);
    emailPasswordInput->setPlaceholderFontColor(Color3B::GRAY);
    emailPasswordInput->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    emailPasswordInput->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    //type of return when finished typing
    emailPasswordInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    emailPasswordInput->setDelegate(this);
    this->addChild(emailPasswordInput);
    
    //**********************************
    
    //creating button, 1st parameter is the normal state, and second is the pressed state
    ui::Button *loginButton = ui::Button::create("button_normal.png", "button_pressed.png");
    //270 x 215
    loginButton->setPosition(Vec2((visibleSize.width/2), visibleSize.height/1.50 + origin.y));
    loginButton->setTitleText("Login");
    loginButton->setTitleFontSize(25*size);
    this->addChild(loginButton);
    loginButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchEvent, this));
    
    /*
    //Establishes connection with server
    cocos2d::network::HttpRequest *request = new cocos2d::network::HttpRequest();
    request->setUrl("http://www.ericschvartzman.com/cocos2dx/clicker_skeleton/jsonarray.php");
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpRequestCompleted, this));
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
    
    //Create conditional statement - when user logs into account, establish connection with server
     */
    
    return true;
}


void HelloWorld::toGameScene(Ref *sender)
{
    CCLOG("to gameplay scene!");
    auto scene = GameScene::createGameScene();
    Director::getInstance()->replaceScene(scene);
    
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
//logging the occurences of touch events
void HelloWorld::touchEvent(Ref *sender, ui::Widget::TouchEventType type)
{
    switch(type)
    {
        //when the user first touch the button
        case ui::Widget::TouchEventType::BEGAN:
        {
            log("touch began");
            break;
        }
        case ui::Widget::TouchEventType::MOVED:
        {
            log("touch moved");
            break;
        }
        //if user gets interrupted, eg a phone call
        case ui::Widget::TouchEventType::ENDED:
        {
            log("touch ended");
            toGameScene(sender);
            break;
        }
        case ui::Widget::TouchEventType::CANCELED:
        {
            log("touch cancelled");
            break;
        }
        default:
        {
            break;
        }
    }
    log("hello");
}

void HelloWorld::editBoxEditingDidBegin(ui::EditBox* editBox)
{
    log("Begun");
}

void HelloWorld::editBoxEditingDidEnd(ui::EditBox* editBox)
{
    log("End");
}

void HelloWorld::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
    log("Text change, text : %s", text.c_str());
}

void HelloWorld::editBoxReturn(ui::EditBox* editBox)
{
    log("Returned");
}

/*
 void HelloWorld::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    // The data will be placed in the buffer.
    std::vector<char> * buffer = response->getResponseData();
    char *concatenated = (char *)malloc(buffer->size() + 1);
    std::string s2(buffer->begin(), buffer->end());
    strcpy(concatenated, s2.c_str());
    
    // JSON Parser
    Json *json = Json_create(concatenated);
    int test1 = Json_getInt(json, "a", -1);
    const char *test2 = Json_getString(json, "b", "default");
    float test3 = Json_getFloat(json, "c", -1.0f);
    
    // View the console
    CCLOG("HTTP Response : key a : %i", test1);
    CCLOG("HTTP Response : key b : %s", test2);
    CCLOG("HTTP Response : key c : %f", test3);
    
    // Delete the JSON object
    Json_dispose(json);
    
    
    if (response->getResponseCode() == 200)
    {
        printf("Succeeded");
        return;
    }
    else
    {
        printf("Failed");
    }
}

*/
