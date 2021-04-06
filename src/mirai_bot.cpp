#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <exception>
#include "mirai/mirai_bot.hpp"
#include "mirai/third-party/WebSocketClient.h"
#include "mirai/third-party/WebSocketClient.cpp"

using std::runtime_error;
using std::stringstream;

namespace
{
	// 因为 httplib 使用 string 来保存文件内容，这里返回值也跟着适配
	string ReadFile(const string& filename)
	{
		std::ifstream ifs(filename, std::ifstream::binary);
		if (!ifs.is_open()) throw std::runtime_error("打开文件失败，请确认路径是否正确并检查文件是否存在");
		std::filebuf* pbuf = ifs.rdbuf();
		std::size_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
		pbuf->pubseekpos(0, ifs.in);
		string result(size, '\0');
		pbuf->sgetn(&result[0], size);
		ifs.close();
		return result;
	}
}

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
	MiraiBot::MiraiBot(const string& host, int port, int threadNums) :
		host_(host),
		port_(port),
		qq_(0),
		cacheSize_(4096),
		ws_enabled_(true),
		http_client_(host, port),
		pool_(threadNums) {}
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

	string MiraiBot::GetMiraiApiHttpVersion()
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


	MessageId_t MiraiBot::SendMessage(QQ_t target, const MessageChain& messageChain, MessageId_t msgId)
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
			MessageId_t msg_id = re_json["messageId"].get<int>();
			return msg_id;
		}
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}


	MessageId_t MiraiBot::SendMessage(GID_t target, const MessageChain& messageChain, MessageId_t msgId)
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
			MessageId_t msg_id = re_json["messageId"].get<int>();
			return msg_id;
		}
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}


	MessageId_t MiraiBot::SendMessage(GID_t gid, QQ_t qq, const MessageChain& messageChain, MessageId_t msgId)
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
			MessageId_t msg_id = re_json["messageId"].get<int>();
			return msg_id;
		}
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}

	void MiraiBot::SendNudge(int64_t target, int64_t subject_id, const string& kind)
	{
		json data =
		{
			{ "sessionKey", sessionKey_ },
			{ "target", target },
			{ "subject", subject_id },
			{ "kind" , kind }
		};

		auto res = http_client_.Post("/sendNudge", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code != 0)
		{
			string msg = re_json["msg"].get<string>();
			throw runtime_error(msg);
		}
	}

	void MiraiBot::SendNudge(QQ_t target, QQ_t subject_id)
	{
		SendNudge(target.ToInt64(), subject_id.ToInt64(), "Friend");
	}

	void MiraiBot::SendNudge(QQ_t target, GID_t subject_id)
	{
		SendNudge(target.ToInt64(), subject_id.ToInt64(), "Group");
	}

	void MiraiBot::SendNudge(QQ_t target, const UID_t& subject_id)
	{
		if (typeid(subject_id) == typeid(GID_t))
		{
			SendNudge(target, (const GID_t&)(subject_id));
		}
		else
		{
			SendNudge(target, (const QQ_t&)(subject_id));
		}
	}

	MiraiImage MiraiBot::UploadImage(const string& filename, const string& type)
	{
		string base_filename = filename.substr(filename.find_last_of("/\\") + 1);
		string img_data = ReadFile(filename);
		httplib::MultipartFormDataItems items =
		{
		  { "sessionKey", sessionKey_, "", "" },
		  { "type", type, "", "" },
		  { "img", img_data, base_filename, "image/png" }
		};

		auto res = http_client_.Post("/uploadImage", items);

		if (!res)
			throw runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-http-api error]: " + res->body);
		json re_json = json::parse(res->body);
		MiraiImage img;
		img.Id = re_json["imageId"].get<string>();
		img.Url = re_json["url"].get<string>();
		img.Path = re_json["path"].get<string>();
		return img;
	}

	FriendImage MiraiBot::UploadFriendImage(const string& filename)
	{
		return UploadImage(filename, "friend");
	}

	GroupImage MiraiBot::UploadGroupImage(const string& filename)
	{
		return UploadImage(filename, "group");
	}

	TempImage MiraiBot::UploadTempImage(const string& filename)
	{
		return UploadImage(filename, "temp");
	}

	MiraiVoice MiraiBot::UploadVoice(const string& filename, const string& type)
	{
		string base_filename = filename.substr(filename.find_last_of("/\\") + 1);
		string voice_data = ReadFile(filename);
		httplib::MultipartFormDataItems items =
		{
		  { "sessionKey", sessionKey_, "", "" },
		  { "type", type, "", "" },
		  { "voice", voice_data, base_filename, "application/octet-stream"  }
		};

		auto res = http_client_.Post("/uploadVoice", items);

		if (!res)
			throw runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-http-api error]: " + res->body);
		json re_json = json::parse(res->body);
		MiraiVoice result;
		result.Id = re_json["voiceId"].get<string>();
		if (!re_json["url"].is_null())
			result.Url = re_json["url"].get<string>();
		result.Path = re_json["path"].get<string>();
		return result;
	}

	MiraiVoice MiraiBot::UploadGroupVoice(const string& filename)
	{
		return UploadVoice(filename, "group");
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
		if (re_json.find("code") != re_json.end())
		{
			throw std::runtime_error(re_json["msg"]);
		}
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


	bool MiraiBot::Recall(MessageId_t mid)
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

	FriendMessage MiraiBot::GetFriendMessageFromId(MessageId_t mid)
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


	GroupMessage MiraiBot::GetGroupMessageFromId(MessageId_t mid)
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

	MiraiBot& MiraiBot::RegisterCommand(
		const string& commandName,
		const vector<string> alias,
		const string& description,
		const string& helpMessage)
	{
		json data =
		{
			{ "authKey", authKey_ },
			{ "name", commandName },
			{ "alias", json(alias) },
			{ "description", description },
			{ "usage", helpMessage }
		};
		auto res = http_client_.Post("/command/register", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-http-api error]: " + res->body);

		return *this;
	}

	MiraiBot& MiraiBot::SendCommand(const string& commandName, const vector<string> args)
	{
		json data =
		{
			{ "authKey", authKey_ },
			{ "name", commandName },
			{ "args", json(args) }
		};
		auto res = http_client_.Post("/command/send", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-http-api error]: " + res->body);
		return *this;
	}

	vector<QQ_t> MiraiBot::GetManagers()
	{
		vector<QQ_t> result;
		stringstream api_url;
		api_url << "/managers?qq=" << GetBotQQ().ToInt64();
		auto res = http_client_.Get(api_url.str().data());
		if (!res)
			throw runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-http-api error]: " + res->body);
		json re_json = json::parse(res->body);
		if (re_json.is_array())
		{
			for (const auto& qq : re_json)
			{
				result.emplace_back(qq.get<int64_t>());
			}
		}
		return result;
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
			try
			{
				if (ws_enabled_)
					FetchEventsWs();
				else
					FetchEventsHttp(count_per_loop);
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
			SleepMilliseconds(time_interval);
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
				throw std::runtime_error("失去与mirai的连接，已重新连接。");
			}
			string msg = re_json["msg"].get<string>();
			throw runtime_error(msg);
		}

		int received_count = 0;
		for (const auto& ele : re_json["data"])
		{
			HandlingSingleEvent(ele);
			received_count++;
		}
		return received_count;
	}

	void MiraiBot::FetchEventsWs()
	{
		using namespace cyanray;

		std::queue<string> event_queue;
		mutex mutex_event_queue;
		condition_variable cv;

		stringstream all_events_url;
		all_events_url << "ws://" << host_ << ":" << port_ << "/all?sessionKey=" << sessionKey_;
		WebSocketClient events_client;

		events_client.Connect(all_events_url.str());
		events_client.OnTextReceived([&](WebSocketClient& client, string text)
			{
				lock_guard<mutex> lock(mutex_event_queue);
				event_queue.emplace(text);
				cv.notify_one();
			});
		events_client.OnLostConnection([&](WebSocketClient& client, int code)
			{
				cv.notify_one();
			});


		stringstream command_url;
		command_url << "ws://" << host_ << ":" << port_ << "/command?authKey=" << authKey_;
		WebSocketClient command_client;

		command_client.Connect(command_url.str());
		command_client.OnTextReceived([&](WebSocketClient& client, string text)
			{
				lock_guard<mutex> lock(mutex_event_queue);
				event_queue.emplace(text);
				cv.notify_one();
			});
		command_client.OnLostConnection([&](WebSocketClient& client, int code)
			{
				cv.notify_one();
			});


		// 循环处理事件队列(WebSocket库不可执行耗时操作，因此在此线程处理事件)
		while (true)
		{
			if (event_queue.empty() &&
				(events_client.GetStatus() != WebSocketClient::Status::Open ||
					command_client.GetStatus() != WebSocketClient::Status::Open))
			{
				events_client.Shutdown();
				command_client.Shutdown();
				break;
			}
			unique_lock<mutex> lock(mutex_event_queue);
			if (event_queue.empty())
			{
				cv.wait(lock);
			}
			if (event_queue.empty()) continue;
			string event_text = event_queue.front();
			event_queue.pop();
			lock.unlock();
			ProcessEvent(event_text);
			std::this_thread::yield();
		}

	}

	void MiraiBot::ProcessEvent(std::string& event_json_str)
	{

		if (!event_json_str.empty())
		{
			json j = json::parse(event_json_str);
			// code 不存在，说明没错误，处理事件/消息
			if (j.find("code") == j.end())
			{
				HandlingSingleEvent(j);
			}
			// code 存在，按照 code 进行错误处理
			else if (j["code"].get<int>() == 3 || j["code"].get<int>() == 4)
			{
				Release();
				Auth(authKey_, qq_);
				SessionConfigure(cacheSize_, ws_enabled_);
				throw std::runtime_error("失去与mirai的连接，已重新连接。");
			}
		}
	}

	void MiraiBot::HandlingSingleEvent(const nlohmann::json& ele)
	{
		MiraiEvent mirai_event;
		// 要么是事件要么是指令，不应该是别的
		if (ele.find("type") != ele.end())
		{
			string event_name = ele["type"].get<string>();
			mirai_event = MiraiEventStr(event_name);
		}
		else
			mirai_event = MiraiEvent::Command;
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
		catch (...)
		{
			return false;
		}

	}

} // namespace Cyan