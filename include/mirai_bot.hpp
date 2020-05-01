#pragma once
#ifndef mirai_cpp__mirai_bot_hpp_H_
#define mirai_cpp__mirai_bot_hpp_H_
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <thread>
#include <sstream>
#include <unordered_map>
#include <memory>

#include "ThreadPool.h"
#include "nlohmann/json.hpp"

// fuck windows.h
#ifdef max
#undef max
#endif
#include "httplib.h"


#include "defs/defs.hpp"
#include "events/events.hpp"
#include "exported.h"


using std::string;
using std::runtime_error;
using std::vector;
using std::stringstream;
using std::function;
using nlohmann::json;
using std::unordered_map;


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
		MiraiBot() :
			qq_(0),
			pool_(4),
			http_client_("localhost", 8080),
			host_("localhost"),
			port_(8080),
			cacheSize_(4096), 
			ws_enabled_(false) {}
		MiraiBot(const string& host, int port) :
			qq_(0),
			pool_(4),
			http_client_(host, port),
			host_(host),
			port_(port),
			cacheSize_(4096),
			ws_enabled_(false) {}
		~MiraiBot()
		{
			Release();
		}
		string GetSessionKey() const
		{
			return sessionKey_;
		}
		QQ_t GetBotQQ() const
		{
			return qq_;
		}
		httplib::Client* GetHttpClient()
		{
			return &(this->http_client_);
		}

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
		void OnEventReceived(const EventProcessor<T>& ep)
		{
			processors_.insert({ GetEventType<T>(),
				[=](WeakEvent we)
				{
					ep(*(std::dynamic_pointer_cast<T>(we)));
				}
				});
		}

		void inline static SleepSeconds(int sec)
		{
			std::this_thread::sleep_for(std::chrono::seconds(sec));
		}

		void inline static SleepMilliseconds(int ms)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}

		MiraiBot& UseWebSocket()
		{
			this->ws_enabled_ = true;
			SessionConfigure(cacheSize_, ws_enabled_);
			return *this;
		}

		MiraiBot& UseHTTP()
		{
			this->ws_enabled_ = false;
			SessionConfigure(cacheSize_, ws_enabled_);
			return *this;
		}

		void EventLoop(function<void(const char*)> errLogger = nullptr);

	private:
		bool SessionVerify();
		bool SessionRelease();
		bool SessionConfigure(int cacheSize, bool enableWebsocket);
		unsigned int FetchEvents_HTTP(unsigned int count = 10);
		void FetchEvents_WS();
		void ProcessEvents(const nlohmann::json& ele);
		template<typename T>
		inline WeakEvent MakeWeakEvent(const json& json_)
		{
			std::shared_ptr<T> e = std::make_shared<T>();
			e->SetMiraiBot(this);
			e->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(e);
		}
		WeakEvent CreateEvent(MiraiEvent mirai_event, const json& json_);
		bool Release() noexcept
		{
			try
			{
				return SessionRelease();
			}
			catch (const std::exception&)
			{
				return false;
			}

		}

		// 因为 httplib 使用 string 来保存文件内容，这里适配一下
		inline string ReadFile(const string& filename)
		{
			std::ifstream ifs(filename, std::ifstream::binary);
			std::filebuf* pbuf = ifs.rdbuf();
			std::size_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
			pbuf->pubseekpos(0, ifs.in);
			string result(size, '\0');
			pbuf->sgetn(&result[0], size);
			ifs.close();
			return result;
		}

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

} // namespace Cyan

#endif // !mirai_cpp__mirai_bot_hpp_H_
