#pragma once
#ifndef mirai_cpp__mirai_hpp_H_
#define mirai_cpp__mirai_hpp_H_
#include <CURLWrapper.h>
#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include <future>
#include <functional>
#include <nlohmann/json.hpp>
#include "typedef.hpp"
#include "FriendMessage.h"
#include "GroupMessage.h"
#include "message_chain.hpp"
using std::string;
using std::runtime_error;
using std::vector;
using std::stringstream;
using std::function;
using nlohmann::json;

namespace Cyan
{
	typedef std::function<void(FriendMessage)> FriendMessageProcesser;
	typedef std::function<void(GroupMessage)> GroupMessageProcesser;

	class MiraiBot
	{
	public:
		MiraiBot() = default;
		~MiraiBot() = default;
		bool Auth(const string& authKey, QQ_t qq)
		{
			static const string api_url = api_url_prefix_ + "/auth";

			json j;
			j["authKey"] = authKey;
			string pData = j.dump();
			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);

			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				int code = reJson["code"].get<int>();
				if (code == 0)
				{
					this->sessionKey_ = reJson["session"].get<string>();
					this->qq_ = qq;
					// 启动消息循环
					//std::async(std::launch::async, [&]() { EventLoop(); });
					return SessionVerify();
				}
				else
					throw runtime_error("Auth Key 不正确");
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;
		}
		bool SendFriendMessage(QQ_t target,const MessageChain& messageChain)
		{
			static const string api_url = api_url_prefix_ + "/sendFriendMessage";

			json j;
			j["sessionKey"] = sessionKey_;
			j["target"] = target;
			j["messageChain"] = messageChain.ToJson();

			string pData = j.dump();
			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				int code = reJson["code"].get<int>();
				if (code == 0)
					return true;
				else
				{
					string msg = reJson["msg"].get<string>();
					throw runtime_error(msg);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;
		}
		bool SendGroupMessage(GID_t target,const MessageChain& messageChain)
		{
			static const string api_url = api_url_prefix_ + "/sendGroupMessage";
			json j;
			j["sessionKey"] = sessionKey_;
			j["target"] = target;
			j["messageChain"] = messageChain.ToJson();

			string pData = j.dump();
			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				int code = reJson["code"].get<int>();
				if (code == 0)
					return true;
				else
				{
					string msg = reJson["msg"].get<string>();
					throw runtime_error(msg);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;

		}
		bool SendQuoteMessage(MessageSourceID target,const MessageChain& messageChain);
		FriendImage UploadFriendImage(const string& fileName)
		{
			static const string api_url = api_url_prefix_ + "/uploadImage";

			HTTP http; http.SetContentType("multipart/form-data");
			http.AddPostData("sessionKey", sessionKey_);
			http.AddPostData("type", "friend");
			http.AddFile("img", fileName);
			auto res = http.Post(api_url);
			FriendImage fImg;

			if (res.Ready)
			{
				if (!res.Content.empty())
				{
					fImg.ID = res.Content;
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return fImg;
		}
		GroupImage UploadGroupImage(const string& fileName)
		{
			static const string api_url = api_url_prefix_ + "/uploadImage";

			HTTP http; http.SetContentType("multipart/form-data");
			http.AddPostData("sessionKey", sessionKey_);
			http.AddPostData("type", "group");
			http.AddFile("img", fileName);
			auto res = http.Post(api_url);
			GroupImage gImg;

			if (res.Ready)
			{
				if (!res.Content.empty())
				{
					gImg.ID = res.Content;
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return gImg;
		}
		vector<Friend_t> GetFriendList()
		{
			static const string api_url = api_url_prefix_ + "/friendList?sessionKey=" + sessionKey_;
			vector<Friend_t> result;

			HTTP http;
			auto res = http.Get(api_url);
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				if (!reJson.is_array()) throw runtime_error("解析返回 JSON 时出错");
				for (const auto& ele : reJson)
				{
					Friend_t f;
					f.Set(ele);
					result.push_back(f);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return result;

		}
		vector<Group_t> GetGroupList()
		{
			static const string api_url = api_url_prefix_ + "/groupList?sessionKey=" + sessionKey_;
			vector<Group_t> result;

			HTTP http;
			auto res = http.Get(api_url);
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				if (!reJson.is_array()) throw runtime_error("解析返回 JSON 时出错");
				for (const auto& ele : reJson)
				{
					Group_t f;
					f.Set(ele);
					result.push_back(f);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return result;
		}
		vector<GroupMember_t> GetGroupMembers(GID_t target)
		{
			stringstream api_url;
			api_url
				<< api_url_prefix_
				<< "/memberList?sessionKey="
				<< sessionKey_
				<< "&target="
				<< target;

			vector<GroupMember_t> result;

			HTTP http;
			auto res = http.Get(api_url.str());
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				if (!reJson.is_array()) throw runtime_error("解析返回 JSON 时出错");
				for (const auto& ele : reJson)
				{
					GroupMember_t f;
					f.Set(ele);
					result.push_back(f);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return result;

		}
		bool MuteAll(GID_t target)
		{
			static const string api_url = api_url_prefix_ + "/muteAll";
			json j;
			j["sessionKey"] = sessionKey_;
			j["target"] = target;

			string pData = j.dump();
			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				int code = reJson["code"].get<int>();
				if (code == 0)
					return true;
				else
				{
					string msg = reJson["msg"].get<string>();
					throw runtime_error(msg);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;
		}
		bool UnMuteAll(GID_t target)
		{
			static const string api_url = api_url_prefix_ + "/unmuteAll";
			json j;
			j["sessionKey"] = sessionKey_;
			j["target"] = target;

			string pData = j.dump();
			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				int code = reJson["code"].get<int>();
				if (code == 0)
					return true;
				else
				{
					string msg = reJson["msg"].get<string>();
					throw runtime_error(msg);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;

		}
		bool Mute(GID_t GID, QQ_t memberID, unsigned int time)
		{
			static const string api_url = api_url_prefix_ + "/mute";
			json j;
			j["sessionKey"] = sessionKey_;
			j["target"] = GID;
			j["memberId"] = memberID;
			j["time"] = time;

			string pData = j.dump();
			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				int code = reJson["code"].get<int>();
				if (code == 0)
					return true;
				else
				{
					string msg = reJson["msg"].get<string>();
					throw runtime_error(msg);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;

		}
		bool UnMute(GID_t GID, QQ_t memberID)
		{
			static const string api_url = api_url_prefix_ + "/unmute";
			json j;
			j["sessionKey"] = sessionKey_;
			j["target"] = GID;
			j["memberId"] = memberID;

			string pData = j.dump();
			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				int code = reJson["code"].get<int>();
				if (code == 0)
					return true;
				else
				{
					string msg = reJson["msg"].get<string>();
					throw runtime_error(msg);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;

		}
		bool Kick(GID_t GID, QQ_t memberID, const string& msg = "")
		{
			static const string api_url = api_url_prefix_ + "/kick";
			json j;
			j["sessionKey"] = sessionKey_;
			j["target"] = GID;
			j["memberId"] = memberID;
			j["msg"] = msg;

			string pData = j.dump();
			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				int code = reJson["code"].get<int>();
				if (code == 0)
					return true;
				else
				{
					string msg = reJson["msg"].get<string>();
					throw runtime_error(msg);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;
		}
		bool Release()
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

		void OnFriendMessageReceived(FriendMessageProcesser friendMessageProcesser)
		{
			friendMessageProcesser_ = friendMessageProcesser;
		}
		void OnGroupMessageReceived(GroupMessageProcesser groupMessageProcesser)
		{
			groupMessageProcesser_ = groupMessageProcesser;
		}

		void EventLoop()
		{
			unsigned count_per_loop = 20;
			unsigned time_interval = 100;
			while (true)
			{
				unsigned count = FetchMessagesAndEvents();
				if (count < count_per_loop)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(time_interval));
				}
			}
		}

	private:
		bool SessionVerify() const
		{
			static const string api_url = api_url_prefix_ + "/verify";
			json j;
			j["sessionKey"] = sessionKey_;
			j["qq"] = qq_;

			string pData = j.dump();
			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				int code = reJson["code"].get<int>();
				if (code == 0)
					return true;
				else
				{
					string msg = reJson["msg"].get<string>();
					throw runtime_error(msg);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;
		}
		bool SessionRelease() const
		{
			static const string api_url = api_url_prefix_ + "/release";
			json j;
			j["sessionKey"] = sessionKey_;
			j["qq"] = qq_;

			string pData = j.dump();
			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				int code = reJson["code"].get<int>();
				if (code == 0)
					return true;
				else
				{
					string msg = reJson["msg"].get<string>();
					throw runtime_error(msg);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;
		}

		unsigned int FetchMessagesAndEvents(unsigned int count = 10)
		{
			stringstream api_url;
			api_url
				<< api_url_prefix_
				<< "/fetchMessage?sessionKey="
				<< sessionKey_
				<< "&count="
				<< count;

			int received_count = 0;

			HTTP http;
			auto res = http.Get(api_url.str());
			if (res.Ready)
			{
				json reJson;
				reJson = reJson.parse(res.Content);
				if (!reJson.is_array()) throw runtime_error("解析返回 JSON 时出错");
				for (const auto& ele : reJson)
				{
					MiraiEvent type = MiraiEventStr(ele["type"].get<string>());
					if (groupMessageProcesser_ && type == MiraiEvent::GroupMessage)
					{
						GroupMessage gm;
						gm.Set(ele);
						std::async(std::launch::async, [&]() { groupMessageProcesser_(gm); });
						continue;
					}
					if (friendMessageProcesser_ && type == MiraiEvent::FriendMessage)
					{
						FriendMessage fm;
						fm.Set(ele);
						std::async(std::launch::async, [&]() { friendMessageProcesser_(fm); });
						continue;
					}
	
					received_count++;
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return received_count;
		}
		
		QQ_t qq_;
		string sessionKey_;
		string api_url_prefix_ = "http://127.0.0.1:8080";
		GroupMessageProcesser groupMessageProcesser_;
		FriendMessageProcesser friendMessageProcesser_;

	};
} // namespace Cyan



#endif // !_mirai_cpp_mirai_hpp__
