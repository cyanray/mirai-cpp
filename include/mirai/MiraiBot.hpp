#pragma once
#ifndef mirai_cpp__mirai_bot_hpp_H_
#define mirai_cpp__mirai_bot_hpp_H_
// std libraries
#include <string>
#include <vector>
#include <thread>
#include <sstream>
#include <unordered_map>
#include <memory>
#include <optional>
// third-party
#include "mirai/third-party/nlohmann/json.hpp"
// mirai header files
#include "mirai/exported.h"
#include "mirai/defs/defs.hpp"
#include "mirai/events/EventCallback.hpp"
#include "mirai/events/FriendMessage.hpp"
#include "mirai/events/GroupMessage.hpp"
#include "mirai/events/Message.hpp"
#include "mirai/events/LostConnection.hpp"
#include "mirai/events/EventParsingError.hpp"
#include "mirai/SessionOptions.hpp"

using std::string;
using std::vector;
using std::function;
using nlohmann::json;

// 前置声明
namespace httplib
{
    class Client;
}

namespace Cyan
{
    class EXPORTED MiraiBot
    {
    public:
        MiraiBot();
        ~MiraiBot();
        MiraiBot(const MiraiBot&) = delete;
        MiraiBot& operator=(const MiraiBot&) = delete;

        /**
         * \brief 获得 mirai-cpp 的版本号
         * \return 用字符串表示的版本号，如："1.6.5"
         */
        inline string GetMiraiCppVersion() const
        {
            return "2.4.0";
        }

        std::shared_ptr<httplib::Client> GetHttpClient();

        /**
         * \brief 获得 mirai-api-http 插件的版本
         * \return 用字符串表示的版本号，如："1.6.5"
         */
        string GetMiraiApiHttpVersion();

        /**
         * \brief 获得验证后的 SessionKey
         * \return SessionKey
         */
        string GetSessionKey() const;

        /**
         * @brief 获取 SessionOptions
         * @return SessionOptions
        */
        const SessionOptions& GetSessionOptions() const;

        /**
         * @brief 与 mirai-api-http 建立连接
         * @param opts
        */
        void Connect(const SessionOptions& opts);

        /**
         * @brief 与 mirai-api-http 重新建立连接
        */
        void Reconnect();

        /**
         * @brief 释放 Session，程序结束前请必须执行一次此函数，否则可能导致 mirai-api-http 内存泄漏.
        */
        void Disconnect();

        /**
         * \brief 获得验证后的 QQ 号码
         * \return QQ_t
         */
        QQ_t GetBotQQ() const;

        /**
         * \brief 发送私聊消息
         * \param target 发送对象(QQ_t)
         * \param messageChain 消息链
         * \param msgId 可选, 如果不为空则发送引用消息
         * \return 用于引用或撤回的消息 ID (MessageId)
         */
        MessageId_t SendMessage(const QQ_t& target, const MessageChain& messageChain, MessageId_t msgId = 0);

        /**
         * \brief 发送群聊消息
         * \param target 发送对象(GID_t)
         * \param messageChain 消息链
         * \param msgId 可选, 如果不为空则发送引用消息
         * \return 用于引用或撤回的消息 ID (MessageId)
         */
        MessageId_t SendMessage(const GID_t& target, const MessageChain& messageChain, MessageId_t msgId = 0);

        /**
         * \brief 发送临时消息
         * \param gid 群组(GID)
         * \param qq 群成员(QQ_t)
         * \param messageChain 消息链
         * \param msgId 可选, 如果不为空则发送引用消息
         * \return 用于引用或撤回的消息 ID (MessageId)
         */
        MessageId_t SendMessage(const GID_t& gid, const QQ_t& qq, const MessageChain& messageChain, MessageId_t msgId = 0);

        /**
         * @brief 发送戳一戳
         * @param target 目标QQ，可以是好友或者Bot的QQ
         * @param subject_id 戳一戳接收主体，好友QQ
        */
        void SendNudge(const QQ_t& target, const QQ_t& subject_id);

        /**
         * @brief 发送戳一戳
         * @param target 目标QQ，可以是好友或者Bot的QQ
         * @param subject_id 戳一戳接收主体，群号码
        */
        void SendNudge(const QQ_t& target, const GID_t& subject_id);

        /**
         * @brief 发送戳一戳
         * @param target 目标QQ，可以是好友或者Bot的QQ
         * @param subject_id 戳一戳接收主体
        */
        void SendNudge(const QQ_t& target, const UID_t& subject_id);

        /**
         * @brief 设置精华消息
         * @param target 群消息的 MessageId
        */
        void SetEssence(MessageId_t target);

        /**
         * \brief 上传可以发送给好友的图片
         * \param fileName 文件名
         * \return 好友图片
         */
        FriendImage UploadFriendImage(const string& fileName);

        /**
         * \brief 上传可以发送给群组的图片
         * \param fileName 文件名
         * \return 群组图片
         */
        GroupImage UploadGroupImage(const string& fileName);

        /**
         * \brief 上传可以发送给临时消息的图片
         * \param fileName 文件名
         * \return 临时消息图片
         */
        TempImage UploadTempImage(const string& fileName);

        /**
         * @brief 上传可以发给群组的语音
         * @param filename 文件名(amr文件)
         * @return MiraiVoice
        */
        MiraiVoice UploadGroupVoice(const string& filename);

        /**
         * @brief 上传并发送群文件
         * @param gid 目标群
         * @param filename 文件名
         * @return MiraiFile
        */
        MiraiFile UploadFileAndSend(const GID_t& gid, const string& filename);

        /**
         * \brief 获得好友列表
         * \return vector<Friend_t>
         */
        vector<Friend_t> GetFriendList();

        /**
         * \brief 获得群组列表
         * \return vector<Group_t>
         */
        vector<Group_t> GetGroupList();

        /**
         * \brief 获得群组的群成员列表
         * \param target 群组(GID_t)
         * \return vector<GroupMember>
         */
        vector<GroupMember> GetGroupMembers(const GID_t& target);

        /**
         * \brief 获得群成员的信息
         * \param gid 群组(GID_t)
         * \param memberId 群成员(QQ_t)
         * \return GroupMember
         */
        GroupMember GetGroupMemberInfo(const GID_t& gid, const QQ_t& memberId);

        /**
         * @brief 获取群文件列表
         * @param gid 群组(GID_t)
         * @param offset 分页偏移
         * @param size 分页大小
         * @param parentId 父目录ID
         * @param withDownloadInfo 获取下载信息(需要较长时间)
         * @return 群文件列表
        */
        vector<GroupFile> GetGroupFiles(const GID_t& gid, bool withDownloadInfo = false, int offset = 0, int size = 20, const string& parentId = "");

        /**
         * @brief 通过群文件Id获取GroupFile对象
         * @param gid 群组(GID_t)
         * @param groupFileId 群文件Id
         * @param withDownloadInfo 获取下载信息(需要较长时间)
         * @return GroupFile
        */
        GroupFile GetGroupFileById(const GID_t& gid, const string& groupFileId, bool withDownloadInfo = false);

        /**
         * @brief 创建群文件夹
         * @param target 目标群(GID_t)
         * @param dictionaryName 文件夹名称
         * @return 群文件夹(GroupFile)
        */
        GroupFile GroupFileMakeDirectory(const GID_t& target, const string& dictionaryName);

        /**
         * @brief 重命名群文件
         * @param groupFile 群文件(GroupFile)
         * @param newName 新群文件名
        */
        void GroupFileRename(const GroupFile& groupFile, const string& newName);

        /**
         * @brief 移动群文件
         * @param groupFile 群文件(GroupFile)
         * @param targetId 目标文件夹的ID
        */
        void GroupFileMove(const GroupFile& groupFile, const string& targetId);

        /**
         * @brief 删除群文件
         * @param groupFile 群文件(GroupFile)
        */
        void GroupFileDelete(const GroupFile& groupFile);

        /**
         * @brief 获取 Bot 账号的个人简介
         * @return Profile
        */
        Profile GetBotProfile();

        /**
         * @brief 获取好友的个人简介
         * @param qq 好友 QQ
         * @return Profile
        */
        Profile GetFriendProfile(const QQ_t& qq);

        /**
         * @brief 获取群成员的个人简介
         * @param gid 群号码
         * @param memberQQ 群成员QQ
         * @return Profile
        */
        Profile GetGroupMemberProfile(const GID_t& gid, const QQ_t& memberQQ);

        /**
         * @brief 获取QQ用户的个人简介
         * @param qq 用户QQ
         * @return Profile
        */
        Profile GetUserProfile(const QQ_t& qq);

        /**
         * \brief 设置群成员的群名片
         * \param gid 群组(GID_t)
         * \param memberId 群成员(QQ_t)
         * \param name 新的群名片
         * \return 始终为 true 出错会抛出异常
         */
        void SetGroupMemberName(const GID_t& gid, const QQ_t& memberId, const string& name);

        /**
         * \brief 设置群成员的群头衔
         * \param gid 群组(GID_t)
         * \param memberId 群成员(QQ_t)
         * \param title 新的群头衔
         * \return 始终为 true 出错会抛出异常
         */
        void SetGroupMemberSpecialTitle(const GID_t& gid, const QQ_t& memberId, const string& title);

        /**
         * \brief 全体禁言
         * \param target 群组(GID_t)
         * \return 始终为 true 出错会抛出异常
         */
        void MuteAll(const GID_t& target);

        /**
         * \brief 取消全体禁言
         * \param target 群组(GID_t)
         * \return 始终为 true 出错会抛出异常
         */
        void UnMuteAll(const GID_t& target);

        /**
         * \brief 禁言群成员
         * \param gid 群组(GID_t)
         * \param memberId 群成员(QQ_t)
         * \param time_seconds 时长(秒)
         * \return 始终为 true 出错会抛出异常
         */
        void Mute(const GID_t& gid, const QQ_t& memberId, unsigned int time_seconds);

        /**
         * \brief 取消禁言群成员
         * \param gid 群组(GID_t)
         * \param memberId 群成员(QQ_t)
         * \return 始终为 true 出错会抛出异常
         */
        void UnMute(const GID_t& gid, const QQ_t& memberId);

        /**
         * \brief 将群成员踢出群组
         * \param gid 群组(GID_t)
         * \param memberId 群成员(QQ_t)
         * \param reason_msg 可选, 填写踢人理由, 默认为空
         * \return 始终为 true 出错会抛出异常
         */
        void Kick(const GID_t& gid, const QQ_t& memberId, const string& reason_msg = "");

        /**
         * \brief 撤回一条消息
         * \param mid 消息ID(MessageId)
         * \return 始终为 true 出错会抛出异常
         */
        void Recall(MessageId_t mid);

        /**
         * \brief 让 Bot 退出一个群
         * \param group 要退出的群(GID_t)
         * \return 始终为 true 出错会抛出异常
         */
        void QuitGroup(const GID_t& group);

        /**
         * @brief 删除好友
         * @param  friendQQ 好友QQ
        */
        void DeleteFriend(const QQ_t& friendQQ);

        /**
         * \brief 获取群设置
         * \param group 群(GID_t)
         * \return 群设置
         */
        GroupConfig GetGroupConfig(const GID_t& group);

        /**
         * \brief 设置群设置
         * \param group 群(GID_t)
         * \param groupConfig 群设置
         * \return 始终为 true 出错会抛出异常
         */
        void SetGroupConfig(const GID_t& group, const GroupConfig& groupConfig);

        /**
         * @brief 获取群公告
         * @param group 群(GID_t)
         * @param offset 分页偏移
         * @param size 分页大小
         * @return std::vector<Announcement>
        */
        std::vector<GroupAnnouncement> GetGroupAnnouncement(const GID_t& group, int offset = 0, int size = 10);


        /**
         * @brief 发布群公告
         * @param group 群(GID_t)
         * @param content 内容
         * @param flags 发布设置
         * @param image 群公告图片
         * @return 群公告对象
        */
        GroupAnnouncement PublishGroupAnnouncement(const GID_t& group, 
                                                   const string& content, 
                                                   int flags, 
                                                   const std::optional<MiraiImage>& image = std::nullopt);

        /**
         * @brief 删除群公告
         * @param group 群(GID_t)
         * @param announcementId 群公告ID 
        */
        void DeleteGroupAnnouncement(const GID_t& group, const string& announcementId);

        /**
         * @brief 删除群公告
         * @param announcement 群公告对象
        */
        void DeleteGroupAnnouncement(const GroupAnnouncement& announcement);

        /**
         * \brief 根据消息ID(MessageId)获取对应的好友消息
         * \param mid 消息ID(MessageId)
         * \return 始终为 true 出错会抛出异常
         */
        FriendMessage GetFriendMessageFromId(MessageId_t mid);

        /**
         * \brief 根据消息ID(MessageId)获取对应的群组消息
         * \param mid 消息ID(MessageId)
         * \return 始终为 true 出错会抛出异常
         */
        GroupMessage GetGroupMessageFromId(MessageId_t mid);

        /**
         * @brief 注册指令
         * @param commandName 指令名称
         * @param alias 指令别名
         * @param description 指令描述
         * @param helpMessage 指令帮助，执行出错时显示
        */
        void RegisterCommand(
            const string& commandName,
            const vector<string>& alias,
            const string& description,
            const string& helpMessage = "");

        /**
         * @brief 发送指令
         * @param command 指令，第一个元素为指令名称，后面的元素为指令参数
        */
        void SendCommand(const vector<string>& command);

        /**
         * @brief 获取 Session 信息
         * @return 关于 Bot 的信息
        */
        Friend_t GetSessionInfo();

        /**
         * @brief 设置群成员管理员
         * @param group 群号码
         * @param member 成员号码
         * @param assign 赋值(true: 设置为管理员; false: 取消管理员;)
        */
        void SetGroupAdmin(const GID_t& group, const QQ_t& member, bool assign);

        /**
         * \brief 监听事件
         * \tparam T 事件类型
         * \param ep 事件处理函数
         * \return MiraiBot 引用
         */
        template <typename T>
        MiraiBot& On(const EventCallback<T>& ep)
        {
            return OnEventReceived<T>(ep);
        }

        /**
         * \brief 监听事件
         * \tparam T 事件类型
         * \param ep 事件处理函数
         * \return MiraiBot 引用
         */
        template <typename T>
        MiraiBot& OnEventReceived(const EventCallback<T>& ep);

        /**
         * \brief 睡眠当前线程
         * \param sec 时长(秒)
         */
        void static SleepSeconds(int sec)
        {
            std::this_thread::sleep_for(std::chrono::seconds(sec));
        }

        /**
         * \brief 睡眠当前线程
         * \param ms 时长(毫秒)
         */
        void static SleepMilliseconds(int ms)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }

    private:
        struct pimpl;
        pimpl* pmem = nullptr;
        EventCallback<LostConnection> lostConnectionCallback;
        EventCallback<EventParsingError> eventParsingErrorCallback;
        std::unordered_multimap<MiraiEvent, CallbackInvoker> processors;

        template <typename T>
        CallbackInvoker GetCallbackInvoker(const EventCallback<T>& ep)
        {
            return [=](WeakEvent we)
            {
                // 这个lambda函数有两个作用
                // 1.创建类型为T的WeakEvent
                // 2.将传入的WeakEvent转化为类型T
                //   然后给 EventProcessor 使用
                if (we == nullptr)
                {
                    std::shared_ptr<T> e = std::make_shared<T>();
                    return std::dynamic_pointer_cast<EventBase>(e);
                }
                else
                {
                    ep(*(std::dynamic_pointer_cast<T>(we)));
                    return we;
                }
            };
        }

        template <typename T>
        void StoreCallbackInvoker(CallbackInvoker);
    };

    template<typename T>
    inline void MiraiBot::StoreCallbackInvoker(CallbackInvoker func)
    {
        processors.insert({ T::GetMiraiEvent(), func });
    }

    template<>
    inline void MiraiBot::StoreCallbackInvoker<Message>(CallbackInvoker func)
    {
        processors.insert({ MiraiEvent::FriendMessage, func });
        processors.insert({ MiraiEvent::GroupMessage, func });
        processors.insert({ MiraiEvent::TempMessage, func });
    }

    template <typename T>
    inline MiraiBot& MiraiBot::OnEventReceived(const EventCallback<T>& ep)
    {
        auto func = GetCallbackInvoker<T>(ep);
        StoreCallbackInvoker<T>(func);
        return *this;
    }

    /**
     * @brief 对 LostConnection 的偏特化
    */
    template<>
    inline MiraiBot& MiraiBot::OnEventReceived<LostConnection>(const EventCallback<LostConnection>& cb)
    {
        lostConnectionCallback = cb;
        return *this;
    }

    /**
    * @brief 对 EventParsingError 的偏特化
    */
    template<>
    inline MiraiBot& MiraiBot::OnEventReceived<EventParsingError>(const EventCallback<EventParsingError>& cb)
    {
        eventParsingErrorCallback = cb;
        return *this;
    }

} // namespace Cyan

#endif // !mirai_cpp__mirai_bot_hpp_H_
