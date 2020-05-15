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
		host_("localhost"),
		port_(8080),
		qq_(0),
		cacheSize_(4096),
		ws_enabled_(true),
		http_client_("localhost", 8080),
		pool_(4) {}
	MiraiBot::MiraiBot(const string& host, int port) :
		host_(host),
		port_(port),
		qq_(0),
		cacheSize_(4096),
		ws_enabled_(true),
		http_client_(host, port),
		pool_(4) {}
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

		auto res = http_client_.Get("/about");
		if (!res)
			throw runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-http-api error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
		{
			string version = re_json["data"]["version"].get<string>();
			return version;
		}
		string msg = re_json["errorMessage"].get<string>();
		throw runtime_error(msg);
	}

	bool MiraiBot::Auth(const string& authKey, QQ_t qq)
	{
		json data =
		{
			{ "authKey", authKey }
		};

		auto res = http_client_.Post("/auth", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
		{
			this->sessionKey_ = re_json["session"].get<string>();
			this->authKey_ = authKey;
			this->qq_ = qq;
			return SessionVerify();
		}
		throw runtime_error("Auth Key 不正确");
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
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
		{
			MessageId msg_id = re_json["messageId"].get<int>();
			return msg_id;
		}
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
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
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
		{
			MessageId msg_id = re_json["messageId"].get<int>();
			return msg_id;
		}
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
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
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
		{
			MessageId msg_id = re_json["messageId"].get<int>();
			return msg_id;
		}
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
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
		
		if (!res)
			throw runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-http-api error]: " + res->body);
		json re_json = json::parse(res->body);
		FriendImage img;
		img.ID = re_json["imageId"].get<string>();
		img.Url = re_json["url"].get<string>();
		img.Path = re_json["path"].get<string>();
		return img;
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
		
		if (!res)
			throw runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-http-api error]: " + res->body);
		json re_json = json::parse(res->body);
		GroupImage img;
		img.ID = re_json["imageId"].get<string>();
		img.Url = re_json["url"].get<string>();
		img.Path = re_json["path"].get<string>();
		return img;
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
		
		if (!res)
			throw runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-http-api error]: " + res->body);
		json re_json = json::parse(res->body);
		TempImage img;
		img.ID = re_json["imageId"].get<string>();
		img.Url = re_json["url"].get<string>();
		img.Path = re_json["path"].get<string>();
		return img;
	}


	vector<Friend_t> MiraiBot::GetFriendList()
	{
		auto res = http_client_.Get(("/friendList?sessionKey=" + sessionKey_).data());
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		vector<Friend_t> result;
		json re_json = json::parse(res->body);
		for (const auto& ele : re_json)
		{
			Friend_t f;
			f.Set(ele);
			result.emplace_back(f);
		}
		return result;
	}


	vector<Group_t> MiraiBot::GetGroupList()
	{
		auto res = http_client_.Get(("/groupList?sessionKey=" + sessionKey_).data());
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		vector<Group_t> result;
		json re_json = json::parse(res->body);
		for (const auto& ele : re_json)
		{
			Group_t group;
			group.Set(ele);
			result.emplace_back(group);
		}
		return result;
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
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		vector<GroupMember_t> result;
		json re_json = json::parse(res->body);
		for (const auto& ele : re_json)
		{
			GroupMember_t f;
			f.Set(ele);
			result.push_back(f);
		}
		return result;
	}

	GroupMemberInfo MiraiBot::GetGroupMemberInfo(GID_t gid, QQ_t memberId)
	{
		
		stringstream api_url;
		api_url
			<< "/memberInfo?sessionKey="
			<< sessionKey_
			<< "&target="
			<< int64_t(gid)
			<< "&memberId="
			<< int64_t(memberId);
		auto res = http_client_.Get(api_url.str().data());
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		GroupMemberInfo result;
		result.Set(re_json);
		return result;
	}

	bool MiraiBot::SetGroupMemberInfo(GID_t gid, QQ_t memberId, const GroupMemberInfo& memberInfo)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(gid)},
			{ "memberId", int64_t(memberId)}
		};
		data["info"] = memberInfo.ToJson();

		auto res = http_client_.Post("/memberInfo", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
			return true;
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}

	bool MiraiBot::SetGroupMemberName(GID_t gid, QQ_t memberId, const string& name)
	{
		auto member_info = this->GetGroupMemberInfo(gid, memberId);
		member_info.Name = name;
		return this->SetGroupMemberInfo(gid, memberId, member_info);
	}

	bool MiraiBot::SetGroupMemberSpecialTitle(GID_t gid, QQ_t memberId, const string& title)
	{
		auto member_info = this->GetGroupMemberInfo(gid, memberId);
		member_info.SpecialTitle = title;
		return this->SetGroupMemberInfo(gid, memberId, member_info);
	}

	bool MiraiBot::MuteAll(GID_t target)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(target)}
		};

		auto res = http_client_.Post("/muteAll", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
			return true;
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}


	bool MiraiBot::UnMuteAll(GID_t target)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(target)}
		};

		auto res = http_client_.Post("/unmuteAll", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
			return true;
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}


	bool MiraiBot::Mute(GID_t gid, QQ_t memberId, unsigned int time_seconds)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(gid)},
			{ "memberId", int64_t(memberId)},
			{ "time", time_seconds}
		};

		auto res = http_client_.Post("/mute", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
			return true;
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}


	bool MiraiBot::UnMute(GID_t gid, QQ_t memberId)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(gid)},
			{ "memberId", int64_t(memberId)}
		};

		auto res = http_client_.Post("/unmute", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
			return true;
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}


	bool MiraiBot::Kick(GID_t gid, QQ_t memberId, const string& reason_msg)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(gid)},
			{ "memberId", int64_t(memberId)},
			{ "reason_msg" , reason_msg}
		};

		auto res = http_client_.Post("/kick", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
			return true;
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}


	bool MiraiBot::Recall(MessageId mid)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(mid)}
		};

		auto res = http_client_.Post("/recall", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
			return true;
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}

	bool MiraiBot::Quit(GID_t group)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(group)}
		};

		auto res = http_client_.Post("/quit", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
			return true;
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}

	GroupConfig MiraiBot::GetGroupConfig(GID_t group)
	{
		stringstream api_url;
		api_url
			<< "/groupConfig?sessionKey="
			<< sessionKey_
			<< "&target="
			<< int64_t(group);
		auto res = http_client_.Get(api_url.str().data());
		if (!res)
			throw runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-http-api error]: " + res->body);
		json re_json = json::parse(res->body);
		GroupConfig group_config;
		group_config.Set(re_json);
		return group_config;

	}

	bool MiraiBot::SetGroupConfig(GID_t group, GroupConfig groupConfig)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", int64_t(group) }
		};
		data["config"] = groupConfig.ToJson();

		auto res = http_client_.Post("/groupConfig", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
			return true;
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}

	FriendMessage MiraiBot::GetFriendMessageFromId(MessageId mid)
	{
		stringstream api_url;
		api_url
			<< "/messageFromId?sessionKey="
			<< sessionKey_
			<< "&id="
			<< mid;
		auto res = http_client_.Get(api_url.str().data());
		if (!res)
			throw runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-http-api error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
		{
			FriendMessage result;
			result.Set(re_json["data"]);
			return result;
		}
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}


	GroupMessage MiraiBot::GetGroupMessageFromId(MessageId mid)
	{
		stringstream api_url;
		api_url
			<< "/messageFromId?sessionKey="
			<< sessionKey_
			<< "&id="
			<< mid;

		auto res = http_client_.Get(api_url.str().data());
		if (!res)
			throw runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-http-api error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
		{
			GroupMessage result;
			result.Set(re_json["data"]);
			return result;
		}
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
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

	MiraiBot& MiraiBot::UseHttp()
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
					FetchEventsWs();
				else
					count = FetchEventsHttp(count_per_loop);
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
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
			return true;
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}


	bool MiraiBot::SessionRelease()
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "qq", int64_t(qq_)}
		};

		auto res = http_client_.Post("/release", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
			return true;
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
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
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
			return true;
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}


	unsigned int MiraiBot::FetchEventsHttp(unsigned int count)
	{
		stringstream api_url;
		api_url
			<< "/fetchMessage?sessionKey="
			<< sessionKey_
			<< "&count="
			<< count;
		http_client_.set_timeout_sec(10);
		auto res = http_client_.Get(api_url.str().data());
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();

		if (code != 0)
		{
			// 特判，code=3为session失效，releas后重新auth
			if (code == 3)
			{
				Release();
				Auth(authKey_, qq_);
				throw std::runtime_error("失去与mirai的连接，尝试重新验证...");
			}
			string msg = re_json["msg"].get<string>();
			throw runtime_error(msg);
		}

		int received_count = 0;
		for (const auto& ele : re_json["data"])
		{
			ProcessEvents(ele);
			received_count++;
		}
		return received_count;
	}

	void MiraiBot::FetchEventsWs()
	{
		using namespace easywsclient;
		stringstream url;
		url << "ws://" << host_ << ":" << port_ << "/all?sessionKey=" << sessionKey_;
		std::shared_ptr<WebSocket> ws(WebSocket::from_url(url.str()));
		if (!ws)
			throw std::runtime_error("无法建立 WebSocket 连接!");
		string event_json_str;
		while (ws->getReadyState() != WebSocket::CLOSED && this->ws_enabled_)
		{

			ws->poll(20);
			ws->dispatch([&](const std::string& message)
				{
					event_json_str = message;
				});
			// 这部分不能在lambda表达式中，否则异常无法被EventLoop捕捉
			if (!event_json_str.empty())
			{
				json j = json::parse(event_json_str);
				// code 不存在，说明没错误，处理事件/消息
				if (j.find("code") == j.end())
				{
					ProcessEvents(j);
					event_json_str.resize(0);
					continue;
				}
				// code 存在，按照 code 进行错误处理
				if (j["code"].get<int>() == 3 || j["code"].get<int>() == 4)
				{
					Release();
					Auth(authKey_, qq_);
					SessionConfigure(cacheSize_, ws_enabled_);
					throw std::runtime_error("失去与mirai的连接，尝试重新验证...");
				}
			}
		}
	}

	void MiraiBot::ProcessEvents(const nlohmann::json& ele)
	{
		string event_name = ele["type"].get<string>();
		MiraiEvent mirai_event = MiraiEventStr(event_name);
		// 寻找能处理事件的 Processor
		auto range = processors_.equal_range(mirai_event);
		for (auto it = range.first; it != range.second; ++it)
		{
			auto executor = it->second;
			// 给 executor 传入 nullptr 可以创建一个 WeakEvent
			WeakEvent pevent = executor(nullptr);
			pevent->SetMiraiBot(this);
			pevent->Set(ele);

			pool_.enqueue([=]()
				{
					executor(pevent);
				});
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