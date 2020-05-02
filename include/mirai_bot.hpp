#pragma once
#ifndef mirai_cpp__mirai_bot_hpp_H_
#define mirai_cpp__mirai_bot_hpp_H_
#include <string>
#include <vector>
#include <thread>
#include <sstream>
#include <unordered_map>
#include <memory>
// third-party
#include "ThreadPool.h"
#include "nlohmann/json.hpp"
#include "httplib.h"
// mirai header files
#include "defs/defs.hpp"
#include "events/events.hpp"
#include "exported.h"

using std::string;
using std::vector;
using std::function;
using nlohmann::json;
using std::unordered_map;

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
		MiraiBot(const string& host, int port);
		~MiraiBot();
		string GetSessionKey() const;
		QQ_t GetBotQQ() const;
		httplib::Client* GetHttpClient();
		bool Auth(const string& authKey, QQ_t qq);
		MessageId SendMessage(QQ_t target, const MessageChain& messageChain, MessageId msgId = 0);
		MessageId SendMessage(GID_t target, const MessageChain& messageChain, MessageId msgId = 0);
		MessageId SendMessage(GID_t gid, QQ_t qq, const MessageChain& messageChain, MessageId msgId = 0);
		FriendImage UploadFriendImage(const string& fileName);
		GroupImage UploadGroupImage(const string& fileName);
		TempImage UploadTempImage(const string& fileName);
		vector<Friend_t> GetFriendList();
		vector<Group_t> GetGroupList();
		vector<GroupMember_t> GetGroupMembers(GID_t target);
		bool MuteAll(GID_t target);
		bool UnMuteAll(GID_t target);
		bool Mute(GID_t GID, QQ_t memberID, unsigned int time_seconds);
		bool UnMute(GID_t GID, QQ_t memberID);
		bool Kick(GID_t GID, QQ_t memberID, const string& msg = "");
		bool Recall(MessageId mid);
		FriendMessage GetFriendMessageFromId(MessageId mid);
		GroupMessage GetGroupMessageFromId(MessageId mid);
		template<typename T>
		void On(const EventProcessor<T>& ep)
		{
			OnEventReceived<T>(ep);
		}
		template<typename T>
		void OnEventReceived(const EventProcessor<T>& ep);
		void inline static SleepSeconds(int sec)
		{
			std::this_thread::sleep_for(std::chrono::seconds(sec));
		}
		void inline static SleepMilliseconds(int ms)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}
		MiraiBot& SetCacheSize(int cacheSize);
		MiraiBot& UseWebSocket();
		MiraiBot& UseHTTP();
		void EventLoop(function<void(const char*)> errLogger = nullptr);
	private:
		bool SessionVerify();
		bool SessionRelease();
		bool SessionConfigure(int cacheSize, bool enableWebsocket);
		unsigned int FetchEvents_HTTP(unsigned int count = 10);
		void FetchEvents_WS();
		void ProcessEvents(const nlohmann::json& ele);
		template<typename T>
		inline WeakEvent MakeWeakEvent(const nlohmann::json& json_);
		WeakEvent CreateEvent(MiraiEvent mirai_event, const nlohmann::json& json_);
		bool Release() noexcept;
		inline string ReadFile(const string& filename);

		string authKey_;
		QQ_t qq_;
		string sessionKey_;
		string host_;
		int port_;
		int cacheSize_;
		bool ws_enabled_;
		httplib::Client http_client_;
		unordered_map<MiraiEvent, function<void(WeakEvent)> > processors_;
		ThreadPool pool_;
	};

	template<typename T>
	inline void MiraiBot::OnEventReceived(const EventProcessor<T>& ep)
	{
		processors_.insert({ GetEventType<T>(),
			[=](WeakEvent we)
			{
				ep(*(std::dynamic_pointer_cast<T>(we)));
			}
			});
	}

	template<typename T>
	inline WeakEvent MiraiBot::MakeWeakEvent(const nlohmann::json& json_)
	{
		std::shared_ptr<T> e = std::make_shared<T>();
		e->SetMiraiBot(this);
		e->Set(json_);
		return std::dynamic_pointer_cast<Serializable>(e);
	}

} // namespace Cyan

#endif // !mirai_cpp__mirai_bot_hpp_H_
