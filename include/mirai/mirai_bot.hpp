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
// third-party
#include "mirai/third-party/ThreadPool.h"
#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/third-party/httplib.h"
// mirai header files
#include "mirai/exported.h"
#include "mirai/defs/qq_types.hpp"
#include "mirai/defs/message_chain.hpp"
#include "mirai/defs/friend.hpp"
#include "mirai/defs/group.hpp"
#include "mirai/defs/group_member.hpp"
#include "mirai/defs/group_member_info.hpp"
#include "mirai/defs/group_config.hpp"
#include "mirai/events/event_processer.hpp"
#include "mirai/events/friend_message.hpp"
#include "mirai/events/group_message.hpp"

using std::string;
using std::vector;
using std::function;
using nlohmann::json;

// fu*k windows.h
#ifdef max
#undef max
#endif
#ifdef SendMessage
#undef SendMessage
#endif
#ifdef CreateEvent
#undef CreateEvent
#endif

namespace Cyan
{
	class EXPORTED MiraiBot
	{
	public:
		MiraiBot();
		/**
		 * \brief
		 * \param host hostname
		 * \param port port
		 */
		MiraiBot(const string& host, int port);
		/**
		 * \brief 
		 * \param host hostname
		 * \param port port
		 * \param threadNums 线程池线程数
		 */
		MiraiBot(const string& host, int port, int threadNums);
		MiraiBot(const MiraiBot&) = delete;
		MiraiBot& operator=(const MiraiBot&) = delete;
		~MiraiBot();

		/**
		 * \brief 获得 mirai-cpp 的版本号
		 * \return 用字符串表示的版本号，如："1.6.5"
		 */
		string GetMiraiCppVersion() const
		{
			// mirai-api-http v1.6.5
			return "1.9.6";
		}

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
		 * \brief 获得验证后的 QQ 号码
		 * \return QQ_t
		 */
		QQ_t GetBotQQ() const;
		/**
		 * \brief 获得用于访问 mirai-api-http 的 HttpClient
		 * \return httplib::Client
		 */
		httplib::Client* GetHttpClient();
		/**
		 * \brief 验证 AuthKey 并自动验证 Session Key
		 * \param authKey AuthKey
		 * \param qq Bot QQ
		 * \return 始终为 true (失败会抛出异常)
		 */
		bool Auth(const string& authKey, QQ_t qq);
		/**
		 * \brief 发送私聊消息
		 * \param target 发送对象(QQ_t)
		 * \param messageChain 消息链
		 * \param msgId 可选, 如果不为空则发送引用消息
		 * \return 用于引用或撤回的消息 ID (MessageId)
		 */
		MessageId_t SendMessage(QQ_t target, const MessageChain& messageChain, MessageId_t msgId = 0);
		/**
		 * \brief 发送群聊消息
		 * \param target 发送对象(GID_t)
		 * \param messageChain 消息链
		 * \param msgId 可选, 如果不为空则发送引用消息
		 * \return 用于引用或撤回的消息 ID (MessageId)
		 */
		MessageId_t SendMessage(GID_t target, const MessageChain& messageChain, MessageId_t msgId = 0);
		/**
		 * \brief 发送临时消息
		 * \param gid 群组(GID)
		 * \param qq 群成员(QQ_t)
		 * \param messageChain 消息链
		 * \param msgId 可选, 如果不为空则发送引用消息
		 * \return 用于引用或撤回的消息 ID (MessageId)
		 */
		MessageId_t SendMessage(GID_t gid, QQ_t qq, const MessageChain& messageChain, MessageId_t msgId = 0);
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
		 * \return vector<GroupMember_t>
		 */
		vector<GroupMember_t> GetGroupMembers(GID_t target);
		/**
		 * \brief 获得群成员的群名片和群头衔信息
		 * \param gid 群组(GID_t)
		 * \param memberId 群成员(QQ_t)
		 * \return GroupMemberInfo
		 */
		GroupMemberInfo GetGroupMemberInfo(GID_t gid, QQ_t memberId);
		/**
		 * \brief 设置群成员的群名片和群头衔信息
		 * \param gid 群组(GID_t)
		 * \param memberId 群成员(QQ_t)
		 * \param memberInfo 群成员信息
		 * \return 始终为 true 出错会抛出异常
		 */
		bool SetGroupMemberInfo(GID_t gid, QQ_t memberId, const GroupMemberInfo& memberInfo);
		/**
		 * \brief 设置群成员的群名片
		 * \param gid 群组(GID_t)
		 * \param memberId 群成员(QQ_t)
		 * \param name 新的群名片
		 * \return 始终为 true 出错会抛出异常
		 */
		bool SetGroupMemberName(GID_t gid, QQ_t memberId, const string& name);
		/**
		 * \brief 设置群成员的群头衔
		 * \param gid 群组(GID_t)
		 * \param memberId 群成员(QQ_t)
		 * \param title 新的群头衔
		 * \return 始终为 true 出错会抛出异常
		 */
		bool SetGroupMemberSpecialTitle(GID_t gid, QQ_t memberId, const string& title);
		/**
		 * \brief 全体禁言
		 * \param target 群组(GID_t)
		 * \return 始终为 true 出错会抛出异常
		 */
		bool MuteAll(GID_t target);
		/**
		 * \brief 取消全体禁言
		 * \param target 群组(GID_t)
		 * \return 始终为 true 出错会抛出异常
		 */
		bool UnMuteAll(GID_t target);
		/**
		 * \brief 禁言群成员
		 * \param gid 群组(GID_t)
		 * \param memberId 群成员(QQ_t)
		 * \param time_seconds 时长(秒)
		 * \return 始终为 true 出错会抛出异常
		 */
		bool Mute(GID_t gid, QQ_t memberId, unsigned int time_seconds);
		/**
		 * \brief 取消禁言群成员
		 * \param gid 群组(GID_t)
		 * \param memberId 群成员(QQ_t)
		 * \return 始终为 true 出错会抛出异常
		 */
		bool UnMute(GID_t gid, QQ_t memberId);
		/**
		 * \brief 将群成员踢出群组
		 * \param gid 群组(GID_t)
		 * \param memberId 群成员(QQ_t)
		 * \param reason_msg 可选, 填写踢人理由, 默认为空
		 * \return 始终为 true 出错会抛出异常
		 */
		bool Kick(GID_t gid, QQ_t memberId, const string& reason_msg = "");
		/**
		 * \brief 撤回一条消息
		 * \param mid 消息ID(MessageId)
		 * \return 始终为 true 出错会抛出异常
		 */
		bool Recall(MessageId_t mid);
		/**
		 * \brief 让 Bot 退出一个群
		 * \param group 要退出的群(GID_t)
		 * \return 始终为 true 出错会抛出异常
		 */
		bool Quit(GID_t group);
		/**
		 * \brief 获取群设置
		 * \param group 群(GID_t)
		 * \return 群设置
		 */
		GroupConfig GetGroupConfig(GID_t group);
		/**
		 * \brief 设置群设置
		 * \param group 群(GID_t)
		 * \param groupConfig 群设置
		 * \return 始终为 true 出错会抛出异常
		 */
		bool SetGroupConfig(GID_t group, GroupConfig groupConfig);
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
		 * @return *this
		*/
		MiraiBot& RegisterCommand(
			const string& commandName,
			const vector<string> alias,
			const string& description,
			const string& helpMessage = "");

		/**
		 * @brief 发送指令
		 * @param commandName 指令名称
		 * @param args 参数
		 * @return *this
		*/
		MiraiBot& SendCommand(const string& commandName, const vector<string> args);

		/**
		 * @brief 获得 Managers
		 * @return Managers
		*/
		vector<QQ_t> GetManagers();

		/**
		 * \brief 监听事件
		 * \tparam T 事件类型
		 * \param ep 事件处理函数
		 * \return MiraiBot 引用
		 */
		template <typename T>
		MiraiBot& On(const EventProcessor<T>& ep)
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
		MiraiBot& OnEventReceived(const EventProcessor<T>& ep);

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

		/**
		 * \brief 设置缓存消息的条数(默认为 4096 条, 过小可能会导致撤回/引用失败)
		 * \param cacheSize 缓存消息的条数
		 * \return MiraiBot 引用
		 */
		MiraiBot& SetCacheSize(int cacheSize);
		/**
		 * \brief 使用WebSocket获取消息和事件
		 * \return MiraiBot 引用
		 */
		MiraiBot& UseWebSocket();
		/**
		 * \brief 使用HTTP轮询获取消息和事件
		 * \return MiraiBot 引用
		 */
		MiraiBot& UseHttp();
		/**
		 * \brief 阻塞当前线程,轮询/等待消息
		 * \param errLogger 可选, 错误信息处理函数
		 */
		void EventLoop(function<void(const char*)> errLogger = nullptr);
	private:
		// 私有成员函数
		bool SessionVerify();
		bool SessionRelease();
		bool SessionConfigure(int cacheSize, bool enableWebsocket);
		unsigned int FetchEventsHttp(unsigned int count = 10);
		void FetchEventsWs();
		void ProcessMessage(std::string& event_json_str);
		void ProcessEvents(const json& ele);
		bool Release() noexcept;
		static inline string ReadFile(const string& filename);
		// 私有成员变量
		string host_;
		int port_;
		string authKey_;
		QQ_t qq_;
		string sessionKey_;
		int cacheSize_;
		bool ws_enabled_;
		httplib::Client http_client_;
		ThreadPool pool_;
		std::unordered_multimap<MiraiEvent, function<WeakEvent(WeakEvent)>> processors_;
	};

	template <typename T>
	MiraiBot& MiraiBot::OnEventReceived(const EventProcessor<T>& ep)
	{
		auto func = [=](WeakEvent we)
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

		// 特别处理通用消息事件
		if (T::GetMiraiEvent() == MiraiEvent::Message)
		{
			processors_.insert({ MiraiEvent::FriendMessage, func });
			processors_.insert({ MiraiEvent::GroupMessage, func });
			processors_.insert({ MiraiEvent::TempMessage, func });
		}
		else
		{
			processors_.insert({ T::GetMiraiEvent(), func });
		}

		return *this;
	}

} // namespace Cyan

#endif // !mirai_cpp__mirai_bot_hpp_H_
