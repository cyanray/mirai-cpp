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
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include "CURLWrapper.h"
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
		MiraiBot() :pool_(4), qq_(0) {}
		MiraiBot(const string& host, int port) : pool_(4), qq_(0)
		{
			stringstream ss;
			ss << "http://" << host << ":" << port;
			api_url_prefix_ = ss.str();
		}
		MiraiBot(const string& url_prefix) :api_url_prefix_(url_prefix), pool_(4), qq_(0) {}
		~MiraiBot()
		{
			Release();
		}
		string GetSessionKey() const
		{
			return sessionKey_;
		}
		string GetApiUrlPrefix() const
		{
			return api_url_prefix_;
		}

		bool Auth(const string& authKey, QQ_t qq);
		MessageId SendMessage(QQ_t target, const MessageChain& messageChain, MessageId msgId = -1);
		MessageId SendMessage(GID_t target, const MessageChain& messageChain, MessageId msgId = -1);
		MessageId SendMessage(GID_t gid, QQ_t qq, const MessageChain& messageChain, MessageId msgId = -1);
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

		void EventLoop();

	private:
		bool SessionVerify() const;
		bool SessionRelease() const;
		unsigned int FetchMessagesAndEvents(unsigned int count = 10);

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

		QQ_t qq_;
		string sessionKey_;
		string api_url_prefix_ = "http://127.0.0.1:8080";
		unordered_map<MiraiEvent, function<void(WeakEvent)> > processors_;
		boost::asio::thread_pool pool_;
	};

} // namespace Cyan

#endif // !mirai_cpp__mirai_bot_hpp_H_
