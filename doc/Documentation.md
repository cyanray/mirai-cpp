- [项目结构](#项目结构)
- [源文件编码](#源文件编码)
- [QQ_t和GID_t](#QQ_t和GID_t)
- [如何与mirai-api-http建立连接](#如何与mirai-api-http建立连接)
  * [代码构建SessionOptions](#代码构建SessionOptions)
  * [从命令行读取SessionOptions](#从命令行读取SessionOptions)
  * [从Json文件读取SessionOptions](#从Json文件读取SessionOptions)
  * [命令行参数和Json字段参考表](#命令行参数和Json字段参考表)
- [如何接收、处理事件](#如何接收、处理事件)
  * [mirai-cpp已经支持的事件](#mirai-cpp已经支持的事件)
  * [监听消息和事件(以 GroupMessage 为例)](#监听消息和事件(以-GroupMessage-为例))
  * [通用消息事件](#通用消息事件)
  * [同一个事件，多个处理函数](#同一个事件，多个处理函数)
  * [获取事件的方式](#获取事件的方式)
  * [注意事项⚠⚠](#注意事项⚠⚠)
- [如何发送消息、引用回复、撤回消息](#如何发送消息、引用回复、撤回消息)
  * [获得 ImageId 以发送图片](#获得-ImageId-以发送图片)
- [关于MessageChain](#关于MessageChain)
  * [构建一条消息链](#构建一条消息链)
  * [比较、遍历和拼接 MessageChain](#比较、遍历和拼接-MessageChain)
  * [获得纯文本内容](#获得纯文本内容)
  * [获取其他消息类型的内容](#获取其他消息类型的内容)
  * [其他函数](#其他函数)
- [指令系统](#指令系统)
- [关于异常](#关于异常)
- [关于MiraiBot类](#关于MiraiBot类)
- [处理失去连接的情况](#处理失去连接的情况)
- [其他](#其他) 
  * [ToJson和ToString](#ToJson和ToSting)
  * [枚举](#枚举)

## 项目结构

因为 mirai-cpp 头文件比较多，全部引入可能会造成编译缓慢。因此 mirai-cpp 的头文件可以按需引用。

如果你不在意编译的效率，或者你不想参与到 mirai-cpp 的开发，那么你不需要了解这一部分的内容。你只需要引用一个头文件就可以使用 mirai-cpp 的所有内容：

```c++
#include <mirai.h>
```

这个头文件会载入所有的 mirai-cpp 头文件。

如果你更喜欢按需引用头文件，或者要参与到 mirai-cpp 的开发中，那么我先介绍 mirai-cpp 的项目结构。

**MiraiBot 类**的声明位于 **mirai/mirai_bot.hpp** 文件中。

mirai-cpp 各种**类型**(比如 QQ_t、GID_t、Group_t、GroupFile 等类型)的声明和定义在 **mirai/defs/** 目录下。

mirai-cpp 各种**消息**(这里的消息指的是 文本消息、图片消息、闪照等消息的类型)的声明和定义在 **mirai/messages/** 目录下。

mirai-cpp 各种**事件**的声明和定义在 **mirai/events/** 目录下。

mirai-cpp 各种**异常**的声明和定义在 **mirai/exceptions/** 目录下。

引用 **MiraiBot.hpp** 会引用所有的**类型**和**常用的消息和事件**。

**常用的消息**包括：PlainMessage、ImageMessage、FlashImageMessage、AtMessage、AtAllMessage、FaceMessage； 

**常用的事件**包括：FriendMessage、GroupMessage、Message 和 LostConnection。

除这些之外的 **消息** 和 **事件** 都要引入对应的头文件才能使用。

比如要接收*新成员入群事件*(MemberJoinEvent)，那么需要引用 `#include <mirai/events/MemberJoinEvent.hpp>`。然后使用 On 函数接收该事件。

比如要发送 Json 消息，那么需要引用 `#include <mirai/message/JsonMessage.hpp>`。然后使用 MessageChain 类的 Add 函数加入该消息。

你可以通过引用 `#include <mirai/messages/messages.hpp>` 来一次性引用所有的**消息**。

类似的，有 `#include <mirai/events/events.hpp>` 和 `#include <mirai/exceptions/exceptions.hpp>`。


## 源文件编码

mirai-cpp 的所有源文件使用 UTF-8 编码保存。

**你的机器人项目**的**源文件**也需要使用 UTF-8 编码进行保存。

mirai-cpp 没有运行时的编码转换，如果传入给 mirai-cpp 的***中文文本**不为 UTF-8 编码，可能会导致乱码甚至异常。

如果你使用 MSVC (cl) 编译器，请加上 **/utf-8** 编译参数，以支持 UTF-8 编码的源文件。


## QQ_t和GID_t

QQ_t 与 GID_t 为储存 QQ 号码和群号码的数据类型。
在最初的版本， QQ_t 和 GID_t 的定义是:

```C++
typedef int64_t QQ_t;
typedef int64_t GID_t;
```

但是这样的定义会引起许多问题。

首先 QQ_t 和 GID_t 其实是同一个类型，typedef 只是取了个别名。所以对于发送消息接口，必须分别提供 SendFriendMessage 和 SendGroupMessage 两个接口来区别发送给 QQ 好友的消息和发送给群组的消息。

此外，将 QQ_t 传入需要 GID_t 的接口将不会在编译期出现任何错误。比如想要给某个群组发送消息，但是不小心传入了类型为 QQ_t 的变量。这会导致消息发送失败，而编译器不会提醒你哪里错了。

QQ_t 和 GID_t 现在的定义为:

```C++
struct QQ_t
{
    int64_t QQ;
};
struct GID_t
{
    int64_t GID;
};
```

在此定义下 QQ_t 和 GID_t 是两个不同的类型。为了更严格的类型安全 mirai-cpp 阻止了 QQ_t 和 GID_t 隐式转换为 int64_t 以及 int64_t 隐式转换为 QQ_t 和 GID_t。

将 int64_t 转换为 QQ_t 或者 GID_t 的方法有:

```C++
// 方法1: 使用构造函数
QQ_t qq = QQ_t(123456);
GID_t gid = GID_t(123456);
// 方法2: 使用字面量后缀
QQ_t qq = 123456_qq;
GID_t gid = 123456_gid;
```

将 QQ_t 或者 GID_t 转化为 int64_t 的方法有:

```C++
QQ_t qq = 123456_qq;
// 方法1: 使用 ToInt64() 方法
int64_t qq = qq.ToInt64();
int64_t gid = gid.ToInt64();
// 方法2: 强制类型转换
int64_t qq = int64_t(qq);
int64_t gid = int64_t(gid);
```

## 如何与mirai-api-http建立连接

使用 `MiraiBot` 类的 `Connect` 方法建立连接。与之对应的，使用 `Disconnect` 方法断开连接。

当 MiraiBot 对象销毁之前，或者应用程序退出前必须使用 `Disconnect` 断开连接。否则可能造成 mirai-api-http 的内存泄露。另外请注意捕获 `Disconnect` 的异常。

`Connect` 方法需要一个 `SessionOptions` 的对象作为参数。这个参数可以自己构建也可以从命令行、Json 文件里读取。

### 代码构建SessionOptions
```c++
SessionOptions opts;

// 使用 Set 函数赋予值
opts.BotQQ.Set(10001_qq);
// 也可以使用直接赋值的方式
opts.VerifyKey = "VerifyKey";
opts.HttpHostname = "localhost";

// 使用 Get 函数获取值
cout << opts.Port.Get() << endl;
// 也可以使用 * 符号
cout << *opts.HttpHostname << endl;
```

### 从命令行读取SessionOptions

```c++
// int main(int argc, char* argv[]) {  ...
SessionOptions opts = SessionOptions::FromCommandLine(argc, argv);
```

使用示例：
```bash
./bot.exe --hostname="localhost" --port=8080 --verify-key="HELLO" --enable-verify=true
```

### 从Json文件读取SessionOptions

```c++
SessionOptions opts = SessionOptions::FromJsonFile("./config.json");
```

json文件示例：
```json
{
    "hostname": "127.0.0.1",
    "port": 8081,
    "botQQ": 100001,
    "verifyKey": "Hello",
    "enableVerify": true,
    "singleMode": false,
    "reservedSyncId": "-1",
    "threadPoolSize": 4
}
```

### 命令行参数和Json字段参考表


|          命令行参数      |         Json 字段     |    Json 类型 |                    描述                    |
|:------------------------:|:---------------------:|:------------:|:------------------------------------------:|
|          --hostname      |          hostname     |    string    | 同时设置 httpHostname 和 websocketHostname |
|        --http-hostname   |        httpHostname   |    string    |      与 mirai-api-http 的配置文件一致      |
|     --websocket-hostname |     websocketHostname |    string    |      与 mirai-api-http 的配置文件一致      |
|            --port        |            port       |    number    |    同时设置 httpPort 和   websocketPort    |
|          --http-port     |          httpPort     |    string    |      与 mirai-api-http 的配置文件一致      |
|       --websocket-port   |       websocketPort   |    string    |      与 mirai-api-http 的配置文件一致      |
|           --bot-qq       |           botQQ       |    number    |    机器人的QQ号码(不管什么模式都要设置)    |
|         --verify-key     |         verifyKey     |    string    |      与 mirai-api-http 的配置文件一致      |
|        --enable-verify   |        enableVerify   |    boolean   |      与 mirai-api-http 的配置文件一致      |
|         --single-mode    |         singleMode    |    boolean   |      与 mirai-api-http 的配置文件一致      |
|     reserved-sync-id     |       reservedSyncId  |    string    |      与 mirai-api-http 的配置文件一致      |
|       thread-pool-size   |       threadPoolSize  |    number    |           mirai-cpp 的线程池大小           |


> 注意：mirai-api-http 支持分别设置 http 和 ws 的 hostname 和 port，考虑到一般情况下 http 和 ws 的 hostname (port) 的值都是一样的。因此 mirai-cpp 允许你通过 hostname (port) 同时设置 HttpHostname 和 WebSocketHostname (HttpPort 和 WebSocketPort)。

## 如何接收、处理事件

MiraiBot 类提供了 On 方法和 OnEventReceived 方法，这两个方法是等价的。mirai-cpp 做了许多工作，让你可以轻松地接收某个事件。

你只需要知道一个事件的名称就可以接收并读取事件的属性。这些事件的名称可以在  [mirai-api-http 的文档](https://github.com/project-mirai/mirai-api-http/blob/master/docs/api/EventType.md) 或者 [enum class MiraiEvent](https://github.com/cyanray/mirai-cpp/blob/master/include/mirai/events/mirai_event.hpp) 里找到。

### mirai-cpp已经支持的事件

|     名称                        |     描述             |
|---------------------------------|----------------------|
| BotOnlineEvent,							| Bot 登录成功                  |
| BotOfflineEventActive,					| Bot 主动离线                  |
| BotOfflineEventForce,					    | Bot 被挤下线                  |
| BotOfflineEventDropped,					| Bot 被挤下线                  |
| BotReloginEvent,						    | Bot 主动重新登录              |
| FriendMessage,							| 好友消息                      |
| GroupMessage,							    | 群组消息                      |
| TempMessage,							    | 临时消息                      |
| GroupRecallEvent,						    | 群消息撤回                    |
| FriendRecallEvent,						| 好友消息撤回                  |
| BotMuteEvent,							    | Bot被禁言                     |
| BotUnmuteEvent,							| Bot被取消禁言                 |
| BotJoinGroupEvent,						| Bot加入了一个新群              |
| GroupNameChangeEvent,					    | 某个群名称改变                 |
| GroupMuteAllEvent,						| 群全员禁言                     |
| MemberJoinEvent,						    | 新人入群事件                   |
| MemberLeaveEventKick,					    | 成员被踢出群(该成员不是Bot)     |
| MemberLeaveEventQuit,					    | 成员主动离开群组                |
| MemberMuteEvent,						    | 群成员被禁言(该成员不是Bot)     |
| MemberUnmuteEvent,						| 群成员被取消禁言(该成员不是Bot)  |
| NewFriendRequestEvent,					| 添加好友申请                    |
| MemberJoinRequestEvent,					| 用户入群申请                    |
| BotLeaveEventActive,					    | Bot 主动离开群                  |
| BotLeaveEventKick,						| Bot 被剔出群                    |
| Message,								    | 通用消息事件                     |
| BotInvitedJoinGroupRequestEvent,		    | Bot被邀请入群申请                |
| MemberCardChangeEvent,					| 群成员群名片被修改事件            |
| CommandExecutedEvent,					    | 指令事件                         |
| NudgeEvent,								| 戳一戳(头像)事件                  |
| StrangerMessage,						    | 陌生人消息                       |
| OtherClientMessage,						| 其他客户端消息                    |
| FriendInputStatusChangedEvent,			| 好友输入状态改变事件              |
| FriendNickChangedEvent,					| 好友昵称改变事件                  |
| GroupEntranceAnnouncementChangeEvent,	    | 某群入群公告改变                  |
| GroupAllowAnonymousChatEvent,			    | 群匿名聊天权限改变                |
| GroupAllowConfessTalkEvent,				| 群坦白说权限改变                  |
| GroupAllowMemberInviteEvent,			    | 群员邀请好友加群权限改变           |
| MemberSpecialTitleChangeEvent,			| 群头衔改动事件                    |
| BotGroupPermissionChangeEvent,			| bot 群权限改变事件                |
| MemberPermissionChangeEvent,			    | 群成员权限改变事件                 |
| MemberHonorChangeEvent					| 群成员称号改变事件                |



### 监听消息和事件(以 GroupMessage 为例)

以 GroupMessage 消息事件为例:

```C++
MiraiBot bot;
// bot.Connect(...); 这里省略 Connect 的代码

// 使用 On 函数注册一个 GroupMessage 事件的处理函数
bot.On<GroupMessage>(GroupMsgProcessor);
```

其中，GroupMsgProcessor 函数的定义为:

```C++
// 事件处理函数只包含一个参数，参数类型为事件的类型
void GroupMsgProcessor(GroupMessage m)
{
    cout << "收到一条来自 " 
         << int64_t(m.Sender.Group.GID) 
         << " 的消息." << endl;
    cout << m.MessageChain.ToString() << endl;
}
```

如果是比较简单的处理函数，推荐使用 lambda 表达式:

```C++
// 处理 *新好友申请事件*
bot.On<NewFriendRequestEvent>([&](NewFriendRequestEvent e)
{
    try
    {
        // 同意加为新好友
        e.Accept();
        // 等待 5 秒后
        MiraiBot::SleepSeconds(5);
        // 发送欢迎消息
        bot.SendMessage(e.FromId, MessageChain().Plain("你好!"));
    }
    catch (const exception& ex)
    {
        cout << ex.what() << endl;
    }
});
```

### 通用消息事件

通用消息事件 (Message) 是 mirai-cpp 创造的特殊事件(mirai-api-http 中不存在)。这个事件会在收到好友消息、群组消息和临时消息时被触发。

在要同时对这三种消息做出相同响应的时候(比如复读机)，使用这个事件可以省略很多代码。

```C++
// 通用型消息
// 收到 FriendMessage、GroupMessage、TempMessage 时都会调用它
// 判断类型之后，也可调用对应的转换函数进行转换 (类型不正确将转换失败抛出常)
bot.On<Message>(
    [&](Message m)
    {
        // 只能通过 Message 获得发送者的QQ号码
        // 如果想要“原路回复”(好友消息私聊回复，群组消息群组回复)，
        // 请使用 Message 事件的 Reply 方法。
        m.Reply("Message事件可处理三种消息:" + m.MessageChain);
        // 判断是否群组消息
        if(m.GetMessageType() == MessageType::GroupMessage)
        {
            GroupMessage gm = m.ToGroupMessage();
            // TODO: 针对群组消息的特别处理
        }
    });
```

### 同一个事件，多个处理函数

可以对同一类型的事件重复使用On方法，每个事件处理函数都会被**异步**调用，所以**不能保证调用的顺序**。

```C++
// 监听各类事件
bot.On<GroupMessage>(
     [&](GroupMessage m)
     {
          m.QuoteReply(m.MessageChain);
     });
// 可以多次监听同一事件，每个处理函数都会被执行，但是不保证执行顺序
bot.On<GroupMessage>(
     [&](GroupMessage m)
     {
          m.Reply("2222 " + m.MessageChain);
     });
```

### 获取事件的方式

mirai-cpp 依赖 WebSocket 拉取事件和消息，所以 mirai-api-http 必须开启 ws 适配器。

### 注意事项⚠⚠

mirai-cpp 监听到一个事件后，会将该事件加入到线程池中进行处理。因此你需要特别注意多线程环境下数据冲突的问题。（如访问全局变量需要加锁）

> 提示：使用 "cout <<" 这种方法来输出内容是多线程不安全的，可能会导致输出混乱。建议避免在**事件处理函数**里使用 cout。请使用 printf 或者多线程安全的日志库。

mirai-cpp 的线程池大小可在 `SessionOptions` 中设置。

```c++
SessionOptions opts = SessionOptions::FromCommandLine(argc, argv);
opts.ThreadPoolSize = 8;
```

线程池大小建议设置为 CPU 核心数的 2 ~ 3倍，有助于充分利用 CPU。



## 如何发送消息、引用回复、撤回消息

mirai 支持好友消息、群组消息和临时消息。在 mirai-cpp 中要发送这些消息统一使用 SendMessage 方法。

```C++
MessageChain msg = MessageChain().Plain("你好!");
// 好友消息
bot.SendMessage(123456_qq, msg);
// 群组消息
bot.SendMessage(123456_gid, msg);
// 临时消息(mirai限制了不能主动发送临时消息，但是可以在接收到 TempMessage 事件后发送)
bot.SendMessage(123456_gid,123456_qq,msg);
```

SendMessage 会返回 MessageId。MessageId可以用于引用回复和撤回消息。

```C++
MessageId_t msg_id = bot.SendMessage(123456_gid, msg);
// 在群组发送引用回复(好友、临时消息同理)
bot.SendMessage(123456_gid, msg, msg_id);
// 撤回消息
bot.Recall(msg_id);
```

大多数时候，机器人都是被动发送消息，即收到消息事件后进行回复。为了避免代码冗长，消息事件中提供了一些回复消息的函数:

```C++
// 定义群组消息处理函数
void GroupMsgProcessor(GroupMessage m)
{
    MessageChain msg = MessageChain().Plain("你好!");
    // 直接回复
    m.Reply(msg);
    // 等价于: bot.SendMessage(m.Sender.Group.GID,msg);

    // 引用回复
    m.QuoteReply(msg);
    // 等价于: bot.SendMessage(m.Sender.Group.GID,msg,m.GetMessageId());

    // 直接撤回
    m.Recall();
    // 等价于: bot.Recall(m.GetMessageId());

    // 判断有没有 At 我 (仅限 GroupMessage)
    m.AtMe()
}

bot.On<GroupMessage>(GroupMsgProcessor);
```

此外 `MiraiBot` 类中的 `GetFriendMessageFromId` 和 `GetGroupMessageFromId` 函数可以通过 MessageId 获得消息。(消息缓存于 mirai-http-api 插件中，可能会获取失败，获取失败会抛出异常)

> 注意: 无法通过 MessageId 得到 Message 的类型，所以如果某个 MessageId 指向一条 FriendMessage 却使用 GetGroupMessageFromId 获取其内容将会导致错误而抛出异常。

### 获得 ImageId 以发送图片

使用 UploadFriendImage 、UploadGroupImage 或 UploadTempImage 方法上传图片，得到 MiraiImage 结构体。（旧版本的 mirai 不支持好友图片发给群组、群组图片发给好友。使用mirai 2.0以上版本支持三种类型的图片互相转发。）

```C++
GroupImage img = UploadGroupImage("D:\\test.png");
// 普通图片
MessageChain msg = MessageChain().Image(img);
// 闪照
MessageChain msg = MessageChain().FlashImage(img);
```

## 关于MessageChain

MessageChain 是 IMessage 的容器。IMessage 的派生类有 PlainMessage、ImageMessage、FlashImageMessage、AtMessage、AtAllMessage等，分别对应文本消息、图片消息、闪照消息、At消息和At全体成员消息。

MessageChain 为了和其他方法保持风格一致，没有使用 stl 风格，你可以使用 `ToVector()` 方法来进行转换，得到类型为 `std::vector<std::shared_ptr<IMessage>>` 的容器。（注意：ToVector() 返回的是引用，对该引用的修改会影响MessageChain的内容）

### 构建一条消息链

MessageChain 支持 Method chaining 特性，可以直观地构建一条消息链。

```C++
MessageChain msg = MessageChain()
                    .Plain("文本消息1")
                    .Image(img)
                    .Plain("文本消息2")
                    .At(qq)
                    .AtAll();
```

你也可以使用 Add 方法增加成员：

```C++
// 方法1 (等价于.Plain("Hello"))
MessageChain().Add<PlainMessage>("Hello");
// 方法2 加入已存在的 IMessage 派生类
auto plain_msg = PlainMessage("Hello");
MessageChain().Add(plain_msg);
// 方法3 
MessageChain().Add(PlainMessage("Hello"));
```

### 比较、遍历和拼接 MessageChain

比较两条 MessageChain 是否相等：

```c++
MessageChain a = MessageChain().Plain("你好!").Face(14);
MessageChain b = MessageChain().Plain("早安!").Face(14);
if(a==b) // true
```

遍历 MessageChain 的内容

```c++
MessageChain a = MessageChain().Plain("你好!").Face(14);
for(std::shared_ptr<IMessage> m : mc)
{
    // ...
}

// 或者使用迭代器：
MessageChain::iterator it = a.begin();
for(;it != a.end(); ++it)
{
    // ...
}
```

两条 MessageChain 可以直接相加拼接：

```c++
MessageChain a = MessageChain().Plain("你好!");
MessageChain b = MessageChain().Face(14);
a += b;
// 或者
MessageChain c = a + b;

// MessageChain 特别处理了 Plain 消息，你可以做到将字符串和 MessageChain 直接相加：
MessageChain c = "早安!" + b;
```


### 获得纯文本内容

MessageChian 提供的 GetPlainText 可以获得所有 Plain 消息组成的 string。(如果消息不包含 Plain 成分，返回空文本)

比如:

```C++
MessageChain msg = MessageChain()
                    .Plain("文本消息1")
                    .Image(img)
                    .Plain("文本消息2");
string msg_str = msg.GetPlainText();
// msg_str == "文本消息1文本消息2";
```

如果只需要第一个 Plain 消息的内容，可以使用 GetPlainTextFirst 方法。(如果消息不包含 Plain 成分，返回空文本)

### 获取其他消息类型的内容

MessageChain 提供了 `GetAll<T>()` 和 `GetFirst<T>()` 方法，可以获取各种类型的消息。使用方法如下：

```C++
// 获取所有图片 （如果没有该类型的消息，vector为空）
vector<ImageMessage> img1 =  messageChain.GetAll<ImageMessage>();
// 获取第一张图片 （如果没有该类型的消息会抛出异常）
ImageMessage img2 =  messageChain.GetFirst<ImageMessage>();
```

### 其他函数

MessageId 方法可以获得这条消息的 MessageId。

Timestamp 方法可以获得这条消息的时间戳(类型为 int64_t )。

## 指令系统

mirai-api-http 提供了一个指令系统，mirai-cpp 对该系统进行了适配。
关于该系统的介绍请查阅[mirai-api-http的文档](https://github.com/project-mirai/mirai-api-http#%E6%8F%92%E4%BB%B6%E7%9B%B8%E5%85%B3console%E7%9B%B8%E5%85%B3)。具体使用方法请参考`examples/Command.cpp`中的内容。

mirai-cpp 将指令抽象为一个事件，因此你可以像处理事件一样处理指令。

```C++
// 使用 On 方法接收指令事件
bot.On<Command>(
    [&](Command e)
    {
        cout << "收到指令: " << e.CommandName << ", "
            << "发送者: " << e.Sender.ToInt64() << ", "
            << "发送群: " << e.GroupId.ToInt64() << endl;
        cout << "参数:" << endl;
        for (const auto& arg : e.Args)
            cout << arg << " ";
        cout << endl;

        bot.SendMessage(e.GroupId, MessageChain().Plain("执行指令: ").Plain(e.CommandName));
        
    });
```

## 关于异常

MiraiBot 中的方法几乎都会抛出异常，建议捕捉起来。**在事件处理函数中出现的异常不一定会导致程序崩溃，出现问题会难以调试。**

mirai-cpp 中的异常大概可分为 3 类：NetworkException、MiraiApiHttpException 和其他异常。

导致 NetworkException 的可能原因是网络请求超时，这个时候可以尝试重新发起请求。
导致 MiraiApiHttpException 的原因是 mirai-api-http 无法处理请求，可能原因有权限不足、操作对象不存在等。可以根据状态码(该异常的 Code 字段)来判断具体原因，状态码描述可见 [mirai-api-http 的文档](https://github.com/project-mirai/mirai-api-http/blob/master/docs/api/API.md#%E7%8A%B6%E6%80%81%E7%A0%81)。

## 关于MiraiBot类

MiraiBot 类被设计为**不可复制类**(immovable), 即 MiraiBot 对象不可复制不可赋值。如果你需要在许多源文件中共享一个 MiraiBot，可能会不怎么顺利。有时候传递 MiraiBot 的指针是一个解决方法。这里推荐一个特性，你可以通过**事件**来获取 MiraiBot 对象的引用。

```C++
bot.On<GroupMessage>(
[&](GroupMessage m)
{
    m.Reply(m.MessageChain);
    // 所有事件都实现了 GetMiraiBot 方法
    // 注意，返回的是 MiraiBot 引用
    MiraiBot& bot = m.GetMiraiBot();
    bot.SendMessage(/*...*/);
});
```

## 处理失去连接的情况
偶尔可能因为网络错误或者重启 mirai-console 导致连接中断。mirai-cpp 将连接中断抽象为 `LostConnection` 事件，你可以使用 On 函数接收失去连接的事件。

你可以在 LostConnection 的事件处理函数中重新建立连接，比如下面的代码实现了自动重连。

```c++
bot.On<LostConnection>([&](LostConnection e)
    {
        cout << e.ErrorMessage << " (" << e.Code << ")" << endl;
        while (true)
        {
            try
            {
                bot.Connect(opts);
                break;
            }
            catch (const std::exception& ex)
            {
                cout << ex.what() << endl;
            }
            MiraiBot::SleepSeconds(1);
        }
        cout << "与 mirai-api-http 重新建立连接!" << endl;
    });
```

## 其他

### ToJson和ToSting

mirai-cpp 几乎所有类型(类)都继承自 `ISerializable` 类。因此它们都实现了 `ToJson()` 和 `ToString()` 函数。

**ToJson()** 返回的是 nlohmann::json 对象，这是一个第三方 json 库的类型。

**ToString()** 一般返回 std::string 类型的 json 字符串，常用于调试。

### 枚举

mirai-cpp 中所有的枚举(enum)，都提供了相应的函数用于 enum 和 string 的相互转换。

比如表示群组权限的枚举：

```c++
enum class GroupPermission
{
	Member,
	Administrator,
	Owner
};
```

提供了函数 `GroupPermissionStr` 来进行枚举值和字符串的互相转换。

```c++
string GroupPermissionStr(GroupPermission gp);
GroupPermission GroupPermissionStr(const string& gp);
```

同样的，对于 `MusicShareKind` 枚举，也提供了 `MusicShareKindStr` 函数。
