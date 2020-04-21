#include "mirai_bot.hpp"

namespace Cyan
{
	bool MiraiBot::Auth(const string& authKey, QQ_t qq)
	{
		static const string api_url = api_url_prefix_ + "/auth";

		json j;
		j["authKey"] = authKey;
		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);
		if (res.StatusCode != 200)
			throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
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
	}
	

	MessageId MiraiBot::SendMessage(QQ_t target, const MessageChain& messageChain, MessageId msgId)
	{
		static const string api_url = api_url_prefix_ + "/sendFriendMessage";

		json j;
		j["sessionKey"] = sessionKey_;
		j["target"] = int64_t(target);
		j["messageChain"] = messageChain.ToJson();
		if (msgId != -1) j["quote"] = msgId;

		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);
		if (res.StatusCode != 200)
			throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
		if (res.Ready)
		{
			json reJson;
			reJson = reJson.parse(res.Content);
			int code = reJson["code"].get<int>();
			if (code == 0)
			{
				MessageId msgId = reJson["messageId"].get<int>();
				return msgId;
			}
			else
			{
				string msg = reJson["msg"].get<string>();
				throw runtime_error(msg);
			}
		}
		else
			throw runtime_error(res.ErrorMsg);
	}


	MessageId MiraiBot::SendMessage(GID_t target, const MessageChain& messageChain, MessageId msgId)
	{
		static const string api_url = api_url_prefix_ + "/sendGroupMessage";
		json j;
		j["sessionKey"] = sessionKey_;
		j["target"] = int64_t(target);
		j["messageChain"] = messageChain.ToJson();
		if (msgId != -1) j["quote"] = msgId;
		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
		if (res.Ready)
		{
			json reJson;
			reJson = reJson.parse(res.Content);
			int code = reJson["code"].get<int>();
			if (code == 0)
			{
				MessageId msgId = reJson["messageId"].get<int>();
				return msgId;
			}
			else
			{
				string msg = reJson["msg"].get<string>();
				throw runtime_error(msg);
			}
		}
		else
			throw runtime_error(res.ErrorMsg);
	}


	MessageId MiraiBot::SendMessage(GID_t gid, QQ_t qq, const MessageChain& messageChain, MessageId msgId)
	{
		static const string api_url = api_url_prefix_ + "/sendTempMessage";
		json j;
		j["sessionKey"] = sessionKey_;
		j["qq"] = int64_t(qq);
		j["group"] = int64_t(gid);
		j["messageChain"] = messageChain.ToJson();
		if (msgId != -1) j["quote"] = msgId;
		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
		if (res.Ready)
		{
			json reJson;
			reJson = reJson.parse(res.Content);
			int code = reJson["code"].get<int>();
			if (code == 0)
			{
				MessageId msgId = reJson["messageId"].get<int>();
				return msgId;
			}
			else
			{
				string msg = reJson["msg"].get<string>();
				throw runtime_error(msg);
			}
		}
		else
			throw runtime_error(res.ErrorMsg);
	}


	FriendImage MiraiBot::UploadFriendImage(const string& fileName)
	{
		static const string api_url = api_url_prefix_ + "/uploadImage";

		HTTP http; http.SetContentType("multipart/form-data");
		http.AddPostData("sessionKey", sessionKey_);
		http.AddPostData("type", "friend");
		http.AddFile("img", fileName);
		auto res = http.Post(api_url);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
		FriendImage fImg;

		if (res.Ready)
		{
			json reJson;
			reJson = reJson.parse(res.Content);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			fImg.ID = reJson["imageId"].get<string>();
			fImg.Url = reJson["url"].get<string>();
			fImg.Path = reJson["path"].get<string>();
		}
		else
			throw runtime_error(res.ErrorMsg);
		return fImg;
	}


	GroupImage MiraiBot::UploadGroupImage(const string& fileName)
	{
		static const string api_url = api_url_prefix_ + "/uploadImage";

		HTTP http; http.SetContentType("multipart/form-data");
		http.AddPostData("sessionKey", sessionKey_);
		http.AddPostData("type", "group");
		http.AddFile("img", fileName);
		auto res = http.Post(api_url);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
		GroupImage gImg;

		if (res.Ready)
		{
			json reJson;
			reJson = reJson.parse(res.Content);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			gImg.ID = reJson["imageId"].get<string>();
			gImg.Url = reJson["url"].get<string>();
			gImg.Path = reJson["path"].get<string>();
		}
		else
			throw runtime_error(res.ErrorMsg);
		return gImg;
	}


	TempImage MiraiBot::UploadTempImage(const string& fileName)
	{
		static const string api_url = api_url_prefix_ + "/uploadImage";

		HTTP http; http.SetContentType("multipart/form-data");
		http.AddPostData("sessionKey", sessionKey_);
		http.AddPostData("type", "temp");
		http.AddFile("img", fileName);
		auto res = http.Post(api_url);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
		TempImage tImg;

		if (res.Ready)
		{
			json reJson;
			reJson = reJson.parse(res.Content);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			tImg.ID = reJson["imageId"].get<string>();
			tImg.Url = reJson["url"].get<string>();
			tImg.Path = reJson["path"].get<string>();
		}
		else
			throw runtime_error(res.ErrorMsg);
		return tImg;
	}


	vector<Friend_t> MiraiBot::GetFriendList()
	{
		static const string api_url = api_url_prefix_ + "/friendList?sessionKey=" + sessionKey_;
		vector<Friend_t> result;

		HTTP http;
		auto res = http.Get(api_url);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
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


	vector<Group_t> MiraiBot::GetGroupList()
	{
		static const string api_url = api_url_prefix_ + "/groupList?sessionKey=" + sessionKey_;
		vector<Group_t> result;

		HTTP http;
		auto res = http.Get(api_url);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
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


	vector<GroupMember_t> MiraiBot::GetGroupMembers(GID_t target)
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
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
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


	bool MiraiBot::MuteAll(GID_t target)
	{
		static const string api_url = api_url_prefix_ + "/muteAll";
		json j;
		j["sessionKey"] = sessionKey_;
		j["target"] = int64_t(target);

		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
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


	bool MiraiBot::UnMuteAll(GID_t target)
	{
		static const string api_url = api_url_prefix_ + "/unmuteAll";
		json j;
		j["sessionKey"] = sessionKey_;
		j["target"] = int64_t(target);

		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
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


	bool MiraiBot::Mute(GID_t GID, QQ_t memberID, unsigned int time_seconds)
	{
		static const string api_url = api_url_prefix_ + "/mute";
		json j;
		j["sessionKey"] = sessionKey_;
		j["target"] = int64_t(GID);
		j["memberId"] = int64_t(memberID);
		j["time"] = time_seconds;

		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
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


	bool MiraiBot::UnMute(GID_t GID, QQ_t memberID)
	{
		static const string api_url = api_url_prefix_ + "/unmute";
		json j;
		j["sessionKey"] = sessionKey_;
		j["target"] = int64_t(GID);
		j["memberId"] = int64_t(memberID);

		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
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


	bool MiraiBot::Kick(GID_t GID, QQ_t memberID, const string& msg)
	{
		static const string api_url = api_url_prefix_ + "/kick";
		json j;
		j["sessionKey"] = sessionKey_;
		j["target"] = int64_t(GID);
		j["memberId"] = int64_t(memberID);
		j["msg"] = msg;

		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
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


	bool MiraiBot::Recall(MessageId mid)
	{
		static const string api_url = api_url_prefix_ + "/recall";
		json j;
		j["sessionKey"] = sessionKey_;
		j["target"] = int64_t(mid);

		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
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


	FriendMessage MiraiBot::GetFriendMessageFromId(MessageId mid)
	{
		stringstream api_url;
		api_url
			<< api_url_prefix_
			<< "/messageFromId?sessionKey="
			<< sessionKey_
			<< "&id="
			<< mid;
		FriendMessage result;
		HTTP http;
		auto res = http.Get(api_url.str());
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
		if (res.Ready)
		{
			json reJson;
			reJson = reJson.parse(res.Content);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			result.Set(reJson["data"]);
			return result;
		}
		else
			throw runtime_error(res.ErrorMsg);
	}


	GroupMessage MiraiBot::GetGroupMessageFromId(MessageId mid)
	{
		stringstream api_url;
		api_url
			<< api_url_prefix_
			<< "/messageFromId?sessionKey="
			<< sessionKey_
			<< "&id="
			<< mid;
		GroupMessage result;
		HTTP http;
		auto res = http.Get(api_url.str());
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
		if (res.Ready)
		{
			json reJson;
			reJson = reJson.parse(res.Content);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			result.Set(reJson["data"]);
			return result;
		}
		else
			throw runtime_error(res.ErrorMsg);
	}


	void MiraiBot::EventLoop()
	{
		const unsigned count_per_loop = 20;
		const unsigned time_interval = 100;
		while (true)
		{
			unsigned count = 0;
			try
			{
				count = FetchMessagesAndEvents(count_per_loop);
			}
			catch (const std::exception& ex)
			{
				std::cerr << ex.what() << std::endl;
			}

			if (count < count_per_loop)
			{
				SleepMilliseconds(time_interval);
			}
		}

	}


	bool MiraiBot::SessionVerify() const
	{
		static const string api_url = api_url_prefix_ + "/verify";
		json j;
		j["sessionKey"] = sessionKey_;
		j["qq"] = int64_t(qq_);

		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
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


	bool MiraiBot::SessionRelease() const
	{
		static const string api_url = api_url_prefix_ + "/release";
		json j;
		j["sessionKey"] = sessionKey_;
		j["qq"] = int64_t(qq_);

		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
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


	unsigned int MiraiBot::FetchMessagesAndEvents(unsigned int count)
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
		if (res.StatusCode != 200) throw std::runtime_error("mirai-http-api 出现了异常，请检查日志");
		if (res.Ready)
		{
			json reJson;
			reJson = reJson.parse(res.Content);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			for (const auto& ele : reJson["data"])
			{
				string event_name = ele["type"].get<string>();
				MiraiEvent mirai_event = MiraiEventStr(event_name);
				// 寻找能处理事件的 Processor
				auto pit = processors_.find(mirai_event);
				if (pit != processors_.end())
				{
					auto exector = pit->second;
					WeakEvent pevent = CreateEvent(mirai_event, ele);
					boost::asio::post(pool_, [=]()
						{
							exector(pevent);
						});

				}
				received_count++;
			}
		}
		else
			throw runtime_error(res.ErrorMsg);
		return received_count;
	}


	WeakEvent MiraiBot::CreateEvent(MiraiEvent mirai_event, const json& json_)
	{
		if (mirai_event == MiraiEvent::GroupMessage)
		{
			std::shared_ptr<GroupMessage> gm = std::make_shared<GroupMessage>();
			gm->SetMiraiBot(this);
			gm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(gm);
		}
		if (mirai_event == MiraiEvent::FriendMessage)
		{
			std::shared_ptr<FriendMessage> fm = std::make_shared<FriendMessage>();
			fm->SetMiraiBot(this);
			fm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(fm);
		}
		if (mirai_event == MiraiEvent::TempMessage)
		{
			std::shared_ptr<TempMessage> tm = std::make_shared<TempMessage>();
			tm->SetMiraiBot(this);
			tm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(tm);
		}
		if (mirai_event == MiraiEvent::NewFriendRequestEvent)
		{
			std::shared_ptr<NewFriendRequestEvent> tm = std::make_shared<NewFriendRequestEvent>();
			tm->SetMiraiBot(this);
			tm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(tm);
		}
		if (mirai_event == MiraiEvent::MemberJoinRequestEvent)
		{
			std::shared_ptr<MemberJoinRequestEvent> tm = std::make_shared<MemberJoinRequestEvent>();
			tm->SetMiraiBot(this);
			tm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(tm);
		}
		if (mirai_event == MiraiEvent::MemberJoinEvent)
		{
			std::shared_ptr<MemberJoinEvent> tm = std::make_shared<MemberJoinEvent>();
			tm->SetMiraiBot(this);
			tm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(tm);
		}
		if (mirai_event == MiraiEvent::BotMuteEvent)
		{
			std::shared_ptr<BotMuteEvent> tm = std::make_shared<BotMuteEvent>();
			tm->SetMiraiBot(this);
			tm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(tm);
		}
		if (mirai_event == MiraiEvent::BotUnmuteEvent)
		{
			std::shared_ptr<BotUnmuteEvent> tm = std::make_shared<BotUnmuteEvent>();
			tm->SetMiraiBot(this);
			tm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(tm);
		}
		if (mirai_event == MiraiEvent::MemberMuteEvent)
		{
			std::shared_ptr<MemberMuteEvent> tm = std::make_shared<MemberMuteEvent>();
			tm->SetMiraiBot(this);
			tm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(tm);
		}
		if (mirai_event == MiraiEvent::MemberUnmuteEvent)
		{
			std::shared_ptr<MemberUnmuteEvent> tm = std::make_shared<MemberUnmuteEvent>();
			tm->SetMiraiBot(this);
			tm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(tm);
		}
		if (mirai_event == MiraiEvent::MemberLeaveEventKick)
		{
			std::shared_ptr<MemberLeaveEventKick> tm = std::make_shared<MemberLeaveEventKick>();
			tm->SetMiraiBot(this);
			tm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(tm);
		}
		if (mirai_event == MiraiEvent::MemberLeaveEventQuit)
		{
			std::shared_ptr<MemberLeaveEventQuit> tm = std::make_shared<MemberLeaveEventQuit>();
			tm->SetMiraiBot(this);
			tm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(tm);
		}
		if (mirai_event == MiraiEvent::GroupRecallEvent)
		{
			std::shared_ptr<GroupRecallEvent> tm = std::make_shared<GroupRecallEvent>();
			tm->SetMiraiBot(this);
			tm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(tm);
		}
		if (mirai_event == MiraiEvent::FriendRecallEvent)
		{
			std::shared_ptr<FriendRecallEvent> tm = std::make_shared<FriendRecallEvent>();
			tm->SetMiraiBot(this);
			tm->Set(json_);
			return std::dynamic_pointer_cast<Serializable>(tm);
		}
	}

} // namespace Cyan