#include "mirai_bot.hpp"
#include <iostream>
#include <exception>
#include "easywsclient.hpp"
#define _SSIZE_T_DEFINED
#include "easywsclient.cpp"

using std::runtime_error;
using std::stringstream;

namespace Cyan
{
	MiraiBot::MiraiBot() :
		qq_(0),
		pool_(4),
		http_client_("localhost", 8080),
		host_("localhost"),
		port_(8080),
		cacheSize_(4096),
		ws_enabled_(true) {}
	MiraiBot::MiraiBot(const string& host, int port) :
		qq_(0),
		pool_(4),
		http_client_(host, port),
		host_(host),
		port_(port),
		cacheSize_(4096),
		ws_enabled_(true) {}
	MiraiBot::~MiraiBot()
	{
		Release();
	}
	string MiraiBot::GetSessionKey() const
	{
		return sessionKey_;
	}
	QQ_t MiraiBot::GetBotQQ() const
	{
		return qq_;
	}
	httplib::Client* MiraiBot::GetHttpClient()
	{
		return &(this->http_client_);
	}

	string MiraiBot::GetApiVersion()
	{
		string version;
		auto res = http_client_.Get("/about");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-http-api error]: " + res->body);
			json reJson = json::parse(res->body);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			int code = reJson["code"].get<int>();
			if (code == 0)
				version =  reJson["data"]["version"].get<string>();
			else
			{
				string msg = reJson["errorMessage"].get<string>();
				throw runtime_error(msg);
			}
		}
		else
			throw runtime_error("网络错误");
		return version;
	}

	bool MiraiBot::Auth(const string& authKey, QQ_t qq)
	{
		json data =
		{
			{ "authKey", authKey }
		};

		auto res = http_client_.Post("/auth", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
			int code = reJson["code"].get<int>();
			if (code == 0)
			{
				this->sessionKey_ = reJson["session"].get<string>();
				this->authKey_ = authKey;
				this->qq_ = qq;
				return SessionVerify();
			}
			else
				throw runtime_error("Auth Key 不正确");
		}
		else
			throw std::runtime_error("网络错误");
	}


	MessageId MiraiBot::SendMessage(QQ_t target, const MessageChain& messageChain, MessageId msgId)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{"target",int64_t(target)}
		};
		data["messageChain"] = messageChain.ToJson();
		if (msgId != 0) data["quote"] = msgId;

		auto res = http_client_.Post("/sendFriendMessage", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
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
			throw std::runtime_error("网络错误");
	}


	MessageId MiraiBot::SendMessage(GID_t target, const MessageChain& messageChain, MessageId msgId)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{"target",int64_t(target)}
		};
		data["messageChain"] = messageChain.ToJson();
		if (msgId != 0) data["quote"] = msgId;

		auto res = http_client_.Post("/sendGroupMessage", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
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
			throw std::runtime_error("网络错误");
	}


	MessageId MiraiBot::SendMessage(GID_t gid, QQ_t qq, const MessageChain& messageChain, MessageId msgId)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{"group",int64_t(gid)},
			{"qq",int64_t(qq)}
		};
		data["messageChain"] = messageChain.ToJson();
		if (msgId != 0) data["quote"] = msgId;

		auto res = http_client_.Post("/sendTempMessage", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
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
			throw std::runtime_error("网络错误");
	}


	FriendImage MiraiBot::UploadFriendImage(const string& fileName)
	{
		srand(time(0));
		string img_data = ReadFile(fileName);
		httplib::MultipartFormDataItems items =
		{
		  { "sessionKey", sessionKey_, "", "" },
		  { "type", "friend", "", "" },
		  { "img", img_data,std::to_string(rand()) + ".png", "image/png" }
		};

		auto res = http_client_.Post("/uploadImage", items);
		FriendImage fImg;
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-http-api error]: " + res->body);
			json reJson = json::parse(res->body);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			fImg.ID = reJson["imageId"].get<string>();
			fImg.Url = reJson["url"].get<string>();
			fImg.Path = reJson["path"].get<string>();
		}
		else
			throw runtime_error("网络错误");
		return fImg;
	}


	GroupImage MiraiBot::UploadGroupImage(const string& fileName)
	{
		srand(time(0));
		string img_data = ReadFile(fileName);
		httplib::MultipartFormDataItems items =
		{
		  { "sessionKey", sessionKey_, "", "" },
		  { "type", "group", "", "" },
		  { "img", img_data, std::to_string(rand()) + ".png", "image/png"  }
		};

		auto res = http_client_.Post("/uploadImage", items);
		GroupImage gImg;
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-http-api error]: " + res->body);
			json reJson = json::parse(res->body);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			gImg.ID = reJson["imageId"].get<string>();
			gImg.Url = reJson["url"].get<string>();
			gImg.Path = reJson["path"].get<string>();
		}
		else
			throw runtime_error("网络错误");
		return gImg;
	}


	TempImage MiraiBot::UploadTempImage(const string& fileName)
	{
		srand(time(0));
		string img_data = ReadFile(fileName);
		httplib::MultipartFormDataItems items =
		{
		  { "sessionKey", sessionKey_, "", "" },
		  { "type", "temp", "", "" },
		  { "img", img_data, std::to_string(rand()) + ".png", "image/png"  }
		};

		auto res = http_client_.Post("/uploadImage", items);
		TempImage tImg;
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-http-api error]: " + res->body);
			json reJson = json::parse(res->body);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			tImg.ID = reJson["imageId"].get<string>();
			tImg.Url = reJson["url"].get<string>();
			tImg.Path = reJson["path"].get<string>();
		}
		else
			throw runtime_error("网络错误");
		return tImg;
	}


	vector<Friend_t> MiraiBot::GetFriendList()
	{
		auto res = http_client_.Get(("/friendList?sessionKey=" + sessionKey_).data());
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			vector<Friend_t> result;
			json reJson = json::parse(res->body);
			if (!reJson.is_array()) throw runtime_error("解析返回 JSON 时出错");
			for (const auto& ele : reJson)
			{
				Friend_t f;
				f.Set(ele);
				result.emplace_back(f);
			}
			return result;
		}
		else
			throw std::runtime_error("网络错误");
	}


	vector<Group_t> MiraiBot::GetGroupList()
	{
		auto res = http_client_.Get(("/groupList?sessionKey=" + sessionKey_).data());
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			vector<Group_t> result;
			json reJson = json::parse(res->body);
			if (!reJson.is_array()) throw runtime_error("解析返回 JSON 时出错");
			for (const auto& ele : reJson)
			{
				Group_t f;
				f.Set(ele);
				result.emplace_back(f);
			}
			return result;
		}
		else
			throw std::runtime_error("网络错误");
	}


	vector<GroupMember_t> MiraiBot::GetGroupMembers(GID_t target)
	{
		stringstream api_url;
		api_url
			<< "/memberList?sessionKey="
			<< sessionKey_
			<< "&target="
			<< target;
		auto res = http_client_.Get(api_url.str().data());
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			vector<GroupMember_t> result;
			json reJson = json::parse(res->body);
			if (!reJson.is_array()) throw runtime_error("解析返回 JSON 时出错");
			for (const auto& ele : reJson)
			{
				GroupMember_t f;
				f.Set(ele);
				result.push_back(f);
			}
			return result;
		}
		else
			throw std::runtime_error("网络错误");
	}


	bool MiraiBot::MuteAll(GID_t target)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(target)}
		};

		auto res = http_client_.Post("/muteAll", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
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
			throw std::runtime_error("网络错误");
		return false;
	}


	bool MiraiBot::UnMuteAll(GID_t target)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(target)}
		};

		auto res = http_client_.Post("/unmuteAll", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
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
			throw std::runtime_error("网络错误");
		return false;
	}


	bool MiraiBot::Mute(GID_t GID, QQ_t memberID, unsigned int time_seconds)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(GID)},
			{ "memberId", int64_t(memberID)},
			{ "time", time_seconds}
		};

		auto res = http_client_.Post("/mute", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
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
			throw std::runtime_error("网络错误");
		return false;
	}


	bool MiraiBot::UnMute(GID_t GID, QQ_t memberID)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(GID)},
			{ "memberId", int64_t(memberID)}
		};

		auto res = http_client_.Post("/unmute", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
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
			throw std::runtime_error("网络错误");
		return false;
	}


	bool MiraiBot::Kick(GID_t GID, QQ_t memberID, const string& msg)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(GID)},
			{ "memberId", int64_t(memberID)},
			{ "msg" , msg}
		};

		auto res = http_client_.Post("/kick", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
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
			throw std::runtime_error("网络错误");
		return false;
	}


	bool MiraiBot::Recall(MessageId mid)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(mid)}
		};

		auto res = http_client_.Post("/recall", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
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
			throw std::runtime_error("网络错误");
		return false;
	}


	FriendMessage MiraiBot::GetFriendMessageFromId(MessageId mid)
	{
		stringstream api_url;
		api_url
			<< "/messageFromId?sessionKey="
			<< sessionKey_
			<< "&id="
			<< mid;
		FriendMessage result;
		auto res = http_client_.Get(api_url.str().data());
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-http-api error]: " + res->body);
			json reJson = json::parse(res->body);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			int code = reJson["code"].get<int>();
			if (code == 0)
				result.Set(reJson["data"]);
			else
			{
				string msg = reJson["msg"].get<string>();
				throw runtime_error(msg);
			}
			return result;
		}
		else
			throw runtime_error("网络错误");
	}


	GroupMessage MiraiBot::GetGroupMessageFromId(MessageId mid)
	{
		stringstream api_url;
		api_url
			<< "/messageFromId?sessionKey="
			<< sessionKey_
			<< "&id="
			<< mid;
		GroupMessage result;
		auto res = http_client_.Get(api_url.str().data());
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-http-api error]: " + res->body);
			json reJson = json::parse(res->body);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			int code = reJson["code"].get<int>();
			if (code == 0)
				result.Set(reJson["data"]);
			else
			{
				string msg = reJson["msg"].get<string>();
				throw runtime_error(msg);
			}
			return result;
		}
		else
			throw runtime_error("网络错误");
	}


	MiraiBot& MiraiBot::SetCacheSize(int cacheSize)
	{
		cacheSize_ = cacheSize;
		SessionConfigure(cacheSize_, ws_enabled_);
		return *this;
	}

	MiraiBot& MiraiBot::UseWebSocket()
	{
		this->ws_enabled_ = true;
		SessionConfigure(cacheSize_, ws_enabled_);
		return *this;
	}

	MiraiBot& MiraiBot::UseHTTP()
	{
		this->ws_enabled_ = false;
		SessionConfigure(cacheSize_, ws_enabled_);
		return *this;
	}

	void MiraiBot::EventLoop(function<void(const char*)> errLogger)
	{
		const unsigned count_per_loop = 20;
		const unsigned time_interval = 100;
		SessionConfigure(cacheSize_, ws_enabled_);
		while (true)
		{
			unsigned count = 0;
			try
			{
				if (ws_enabled_)
					FetchEvents_WS();
				else
					count = FetchEvents_HTTP(count_per_loop);
			}
			catch (const std::exception& ex)
			{
				if (errLogger == nullptr)
				{
					std::cerr << ex.what() << std::endl;
				}
				else
				{
					errLogger(ex.what());
				}
			}

			if (count < count_per_loop)
			{
				SleepMilliseconds(time_interval);
			}
		}

	}


	bool MiraiBot::SessionVerify()
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "qq", int64_t(qq_)}
		};

		auto res = http_client_.Post("/verify", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
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
			throw std::runtime_error("网络错误");
		return false;
	}


	bool MiraiBot::SessionRelease()
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "qq", int64_t(qq_)}
		};

		auto res = http_client_.Post("/release", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
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
			throw std::runtime_error("网络错误");
		return false;
	}

	bool MiraiBot::SessionConfigure(int cacheSize, bool enableWebsocket)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "cacheSize", cacheSize },
			{ "enableWebsocket", enableWebsocket }
		};

		auto res = http_client_.Post("/config", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
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
			throw std::runtime_error("网络错误");
		return false;

	}


	unsigned int MiraiBot::FetchEvents_HTTP(unsigned int count)
	{
		int received_count = 0;
		stringstream api_url;
		api_url
			<< "/fetchMessage?sessionKey="
			<< sessionKey_
			<< "&count="
			<< count;
		http_client_.set_timeout_sec(10);
		auto res = http_client_.Get(api_url.str().data());
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			json reJson = json::parse(res->body);
			if (!reJson.is_object()) throw runtime_error("解析返回 JSON 时出错");
			int code = reJson["code"].get<int>();
			if (code != 0)
			{
				// 特判，code=3为session失效，releas后重新auth
				if (code == 3)
				{
					Release();
					Auth(authKey_, qq_);
					throw std::runtime_error("失去与mirai的连接，尝试重新验证...");
				}
				string msg = reJson["msg"].get<string>();
				throw runtime_error(msg);
			}
			for (const auto& ele : reJson["data"])
			{
				ProcessEvents(ele);
				received_count++;
			}
		}
		else
			throw std::runtime_error("网络错误");
		return received_count;

	}

	void MiraiBot::FetchEvents_WS()
	{
		using namespace easywsclient;
		stringstream url;
		url << "ws://" << host_ << ":" << port_ << "/all?sessionKey=" << sessionKey_;
		std::shared_ptr<WebSocket> ws(WebSocket::from_url(url.str()));
		if (!ws)
		{
			throw std::runtime_error("无法建立 WebSocket 连接!");
		}
		string eventJsonStr;
		while (ws->getReadyState() != WebSocket::CLOSED && this->ws_enabled_)
		{

			ws->poll(20);
			ws->dispatch([&](const std::string& message)
				{
					eventJsonStr = message;
				});
			// 这部分不能在lambda表示中，否则异常无法被EventLoop捕捉
			if (!eventJsonStr.empty())
			{
				json j = json::parse(eventJsonStr);
				if (j.find("code") != j.end() && j["code"].get<int>() == 3)
				{
					Release();
					Auth(authKey_, qq_);
					SessionConfigure(cacheSize_, ws_enabled_);
					throw std::runtime_error("失去与mirai的连接，尝试重新验证...");
				}
				ProcessEvents(j);
				eventJsonStr.resize(0);
			}
		}
	}

	void MiraiBot::ProcessEvents(const nlohmann::json& ele)
	{
		string event_name = ele["type"].get<string>();
		MiraiEvent mirai_event = MiraiEventStr(event_name);
		// 寻找能处理事件的 Processor
		auto pit = processors_.find(mirai_event);
		if (pit != processors_.end())
		{
			auto exector = pit->second;
			WeakEvent pevent = CreateEvent(mirai_event, ele);
			pool_.enqueue([=]()
				{
					exector(pevent);
				});
		}
	}


	WeakEvent MiraiBot::CreateEvent(MiraiEvent mirai_event, const json& json_)
	{
		if (mirai_event == MiraiEvent::GroupMessage)
		{
			return MakeWeakEvent<GroupMessage>(json_);
		}
		if (mirai_event == MiraiEvent::FriendMessage)
		{
			return MakeWeakEvent<FriendMessage>(json_);
		}
		if (mirai_event == MiraiEvent::TempMessage)
		{
			return MakeWeakEvent<TempMessage>(json_);
		}
		if (mirai_event == MiraiEvent::NewFriendRequestEvent)
		{
			return MakeWeakEvent<NewFriendRequestEvent>(json_);
		}
		if (mirai_event == MiraiEvent::MemberJoinRequestEvent)
		{
			return MakeWeakEvent<MemberJoinRequestEvent>(json_);
		}
		if (mirai_event == MiraiEvent::MemberJoinEvent)
		{
			return MakeWeakEvent<MemberJoinEvent>(json_);
		}
		if (mirai_event == MiraiEvent::BotMuteEvent)
		{
			return MakeWeakEvent<BotMuteEvent>(json_);
		}
		if (mirai_event == MiraiEvent::BotUnmuteEvent)
		{
			return MakeWeakEvent<BotUnmuteEvent>(json_);
		}
		if (mirai_event == MiraiEvent::MemberMuteEvent)
		{
			return MakeWeakEvent<MemberMuteEvent>(json_);
		}
		if (mirai_event == MiraiEvent::MemberUnmuteEvent)
		{
			return MakeWeakEvent<MemberUnmuteEvent>(json_);
		}
		if (mirai_event == MiraiEvent::MemberLeaveEventKick)
		{
			return MakeWeakEvent<MemberLeaveEventKick>(json_);
		}
		if (mirai_event == MiraiEvent::MemberLeaveEventQuit)
		{
			return MakeWeakEvent<MemberLeaveEventQuit>(json_);
		}
		if (mirai_event == MiraiEvent::GroupRecallEvent)
		{
			return MakeWeakEvent<GroupRecallEvent>(json_);
		}
		if (mirai_event == MiraiEvent::FriendRecallEvent)
		{
			return MakeWeakEvent<FriendRecallEvent>(json_);
		}
		if (mirai_event == MiraiEvent::BotOnlineEvent)
		{
			return MakeWeakEvent<BotOnlineEvent>(json_);
		}
		if (mirai_event == MiraiEvent::BotOfflineEventActive)
		{
			return MakeWeakEvent<BotOfflineEventActive>(json_);
		}
		if (mirai_event == MiraiEvent::BotOfflineEventForce)
		{
			return MakeWeakEvent<BotOfflineEventForce>(json_);
		}
		if (mirai_event == MiraiEvent::BotOfflineEventDropped)
		{
			return MakeWeakEvent<BotOfflineEventDropped>(json_);
		}
	}

	bool MiraiBot::Release() noexcept
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

	// 因为 httplib 使用 string 来保存文件内容，这里返回值也跟着适配
	string MiraiBot::ReadFile(const string& filename)
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

} // namespace Cyan