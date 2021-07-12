#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <locale>
#include <codecvt>
#include "mirai/mirai_bot.hpp"
#include "mirai/third-party/ThreadPool.h"
#include "mirai/third-party/httplib.h"
#include "mirai/third-party/WebSocketClient.h"
#include "mirai/third-party/WebSocketClient.cpp"
#include "mirai/exceptions/network_exception.hpp"
#include "mirai/exceptions/mirai_api_http_exception.hpp"

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

using namespace std;
using namespace cyanray;

namespace
{
	const string CONTENT_TYPE = "application/json;charset=UTF-8";

	// 因为 httplib 使用 string 来保存文件内容，这里返回值也跟着适配
	string ReadFile(const string& filename)
	{
#ifdef _MSC_VER
		// 将 utf-8 转换为 utf-16
		// 这样才能打开 windows 上带有 unicode 字符的文件
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::ifstream ifs(converter.from_bytes(filename), std::ifstream::binary);
#else
		std::ifstream ifs(filename, std::ifstream::binary);
#endif // _MSC_VER
		if (!ifs.is_open()) throw std::runtime_error("打开文件失败，请确认路径是否正确并检查文件是否存在");
		std::filebuf* pbuf = ifs.rdbuf();
		std::size_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
		pbuf->pubseekpos(0, ifs.in);
		string result(size, '\0');
		pbuf->sgetn(&result[0], size);
		ifs.close();
		return result;
	}

	json ParseOrThrowException(const shared_ptr<httplib::Response>& response)
	{
		using namespace Cyan;
		if (!response) throw NetworkException();
		json re_json = json::parse(response->body);
		if (re_json.contains("code"))
		{
			int code = re_json["code"].get<int>();
			if (code != 0)
			{
				string msg = re_json["msg"].get<string>();
				throw MiraiApiHttpException(code, msg);
			}
		}
		return re_json;
	}

}

namespace Cyan
{
	struct MiraiBot::pimpl
	{
		QQ_t botQQ;
		string sessionKey;
		std::shared_ptr<SessionOptions> sessionOptions;
		std::shared_ptr<httplib::Client> httpClient;
		std::unique_ptr<ThreadPool> threadPool;
		WebSocketClient eventClient;

		/**
		 * \brief 验证 VerifyKey
		 * \param verifyKey VerifyKey
		 * \return 如果成功, 返回 SessionKey.
		 */
		string Verify(const string& verifyKey);

		/**
		 * @brief 绑定 Bot QQ 到 Session
		 * @param sessionKey 通过 Verify 得到的 SessionKey.
		 * @param qq Bot QQ
		 * @return true
		*/
		void SessionBind(const string& sessionKey, const QQ_t& qq);

		/**
		 * @brief 释放 Session.
		 * @param sessionKey 通过 Verify 得到的 SessionKey.
		 * @param qq Bot QQ
		 * @return true
		*/
		void SessionRelease(const string& sessionKey, const QQ_t& qq);

		/**
		 * @brief 发送“戳一戳”
		 * @param target 聊天目标 (私聊为聊天对象QQ号，群聊为群号码)
		 * @param subject_id 受体目标 (私聊为自己或对象的QQ号码，群聊为群成员QQ号码)
		 * @param kind 类型 (Friend | Group)
		*/
		void SendNudge(int64_t target, int64_t subject_id, const string& kind);

		/**
		 * @brief 上传图像
		 * @param fileName 文件路径
		 * @param type 类型 (Friend | Group | Temp)
		 * @return
		*/
		MiraiImage UploadImage(const string& fileName, const string& type);

		/**
		 * @brief 上传声音
		 * @param filename 文件路径
		 * @param type 类型 (Friend | Group)
		 * @return
		*/
		MiraiVoice UploadVoice(const string& filename, const string& type);

		/**
		 * @brief 上传文件并发送
		 * @param target 发送目标(好友或群组)
		 * @param filename 文件路径
		 * @param type 类型 (Friend | Group)
		 * @return
		*/
		MiraiFile UploadFileAndSend(int64_t target, const string& filename, const string& type);

		/**
		 * @brief 更新群成员信息（群名片和群头衔）
		 * @param gid 群号码
		 * @param memberId 群成员QQ
		 * @param name 群名片
		 * @param specialTitle 群头衔
		 * @return
		*/
		void SetGroupMemberInfo(GID_t gid, QQ_t memberId, const string& name, const string& specialTitle)
		{
			json data =
			{
				{ "sessionKey", sessionKey },
				{ "target", int64_t(gid)},
				{ "memberId", int64_t(memberId)},
				{ "info",
					{
						{ "name", name },
						{ "specialTitle", specialTitle }
					}
				}
			};

			auto res = httpClient->Post("/memberInfo", data.dump(), CONTENT_TYPE.c_str());
			ParseOrThrowException(res);
		}
	};

	MiraiBot::MiraiBot() : pmem(nullptr)
	{
	}

	MiraiBot::~MiraiBot()
	{
		delete pmem;
	};

	void MiraiBot::Connect(const SessionOptions& opts)
	{
		delete pmem;
		pmem = new pimpl();
		pmem->sessionOptions = std::make_shared<SessionOptions>(opts);
		pmem->threadPool = std::make_unique<ThreadPool>(opts.ThreadPoolSize.Get());
		pmem->httpClient = std::make_shared<httplib::Client>(opts.HttpHostname.Get(), opts.HttpPort.Get());
		pmem->botQQ = opts.BotQQ.Get();
		string& sessionKey = pmem->sessionKey;
		if (opts.EnableVerify.Get())
		{
			sessionKey = pmem->Verify(opts.VerifyKey.Get());
		}
		if (!opts.SingleMode.Get())
		{
			pmem->SessionBind(sessionKey, opts.BotQQ.Get());
		}

		pmem->eventClient.Connect(
			opts.WebSocketHostname.Get(),
			opts.WebSocketPort.Get(),
			"/all?verifyKey="s.append(opts.VerifyKey.Get()).append("&sessionKey=").append(sessionKey));

		pmem->eventClient.OnTextReceived([&](WebSocketClient& client, string text)
			{
				try
				{
					json event_json = json::parse(text)["data"];
					if (!event_json.contains("type")) return;
					string event_name = event_json["type"].get<string>();
					MiraiEvent mirai_event = MiraiEventStr(event_name);
					auto range = processors.equal_range(mirai_event);
					for (auto& it = range.first; it != range.second; ++it)
					{
						auto& executor = it->second;
						// 给 executor 传入 nullptr 可以创建一个 WeakEvent
						WeakEvent pevent = executor(nullptr);
						pevent->SetMiraiBot(this);
						pevent->Set(event_json);

						pmem->threadPool->enqueue([=]()
							{
								executor(pevent);
							});
					}

				}
				catch (...)
				{
					if (eventParsingErrorCallback)
					{
						eventParsingErrorCallback(EventParsingError(std::current_exception()));
					}
				}
			});

		pmem->eventClient.OnLostConnection([&](WebSocketClient& client, int code)
			{
				if (lostConnectionCallback)
				{
					LostConnection result;
					result.Code = code;
					result.ErrorMessage = "与 mirai-api-http 失去连接.";
					lostConnectionCallback(result);
				}
			});
	}

	void MiraiBot::Reconnect()
	{
		pmem->eventClient.Shutdown();
		auto& opts = *pmem->sessionOptions;
		auto& sessionKey = pmem->sessionKey;
		if (opts.EnableVerify.Get())
		{
			sessionKey = pmem->Verify(opts.VerifyKey.Get());
		}
		if (!opts.SingleMode.Get())
		{
			pmem->SessionBind(sessionKey, opts.BotQQ.Get());
		}

		pmem->eventClient.Connect(
			opts.WebSocketHostname.Get(),
			opts.WebSocketPort.Get(),
			"/all?verifyKey="s.append(opts.VerifyKey.Get()).append("&sessionKey=").append(sessionKey));

	}

	void MiraiBot::Disconnect()
	{
		pmem->eventClient.Shutdown();
		pmem->SessionRelease(pmem->sessionKey, pmem->botQQ);
	}

	string MiraiBot::GetSessionKey() const
	{
		return pmem->sessionKey;
	}

	const SessionOptions& MiraiBot::GetSessionOptions() const
	{
		return *pmem->sessionOptions;
	}

	QQ_t MiraiBot::GetBotQQ() const
	{
		return pmem->botQQ;
	}

	std::shared_ptr<httplib::Client> MiraiBot::GetHttpClient()
	{
		return pmem->httpClient;
	}

	string MiraiBot::GetMiraiApiHttpVersion()
	{
		auto res = pmem->httpClient->Get("/about");
		if (!res)
			throw NetworkException();
		if (res->status != 200)
			throw MiraiApiHttpException(-1, res->body);
		json re_json = json::parse(res->body);
		int code = re_json["code"].get<int>();
		if (code == 0)
		{
			string version = re_json["data"]["version"].get<string>();
			return version;
		}
		string msg = re_json["msg"].get<string>();
		throw runtime_error(msg);
	}

	string MiraiBot::pimpl::Verify(const string& verifyKey)
	{
		json data =
		{
			{ "verifyKey", verifyKey }
		};
		auto res = httpClient->Post("/verify", data.dump(), CONTENT_TYPE.c_str());
		json re_json = ParseOrThrowException(res);
		return re_json["session"].get<string>();
	}

	void MiraiBot::pimpl::SessionBind(const string& sessionKey, const QQ_t& qq)
	{
		json data =
		{
			{ "sessionKey", sessionKey },
			{ "qq", int64_t(qq)}
		};

		auto res = httpClient->Post("/bind", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}

	void MiraiBot::pimpl::SessionRelease(const string& sessionKey, const QQ_t& qq)
	{
		json data =
		{
			{ "sessionKey", sessionKey },
			{ "qq", int64_t(qq)}
		};

		auto res = httpClient->Post("/release", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}

	MessageId_t MiraiBot::SendMessage(const QQ_t& target, const MessageChain& messageChain, MessageId_t msgId)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target",int64_t(target) }
		};
		data["messageChain"] = messageChain.ToJson();
		if (msgId != 0) data["quote"] = msgId;

		auto res = pmem->httpClient->Post("/sendFriendMessage", data.dump(), CONTENT_TYPE.c_str());
		json re_json = ParseOrThrowException(res);
		MessageId_t msg_id = re_json["messageId"].get<int>();
		return msg_id;
	}


	MessageId_t MiraiBot::SendMessage(const GID_t& target, const MessageChain& messageChain, MessageId_t msgId)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target",int64_t(target) }
		};
		data["messageChain"] = messageChain.ToJson();
		if (msgId != 0) data["quote"] = msgId;

		auto res = pmem->httpClient->Post("/sendGroupMessage", data.dump(), CONTENT_TYPE.c_str());
		json re_json = ParseOrThrowException(res);
		MessageId_t msg_id = re_json["messageId"].get<int>();
		return msg_id;
	}

	MessageId_t MiraiBot::SendMessage(const GID_t& gid, const QQ_t& qq, const MessageChain& messageChain, MessageId_t msgId)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "group",int64_t(gid) },
			{ "qq",int64_t(qq) }
		};
		data["messageChain"] = messageChain.ToJson();
		if (msgId != 0) data["quote"] = msgId;

		auto res = pmem->httpClient->Post("/sendTempMessage", data.dump(), CONTENT_TYPE.c_str());
		json re_json = ParseOrThrowException(res);
		MessageId_t msg_id = re_json["messageId"].get<int>();
		return msg_id;
	}

	void MiraiBot::pimpl::SendNudge(int64_t target, int64_t subject_id, const string& kind)
	{
		json data =
		{
			{ "sessionKey", sessionKey },
			{ "target", target },
			{ "subject", subject_id },
			{ "kind" , kind }
		};

		auto res = httpClient->Post("/sendNudge", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}

	void MiraiBot::SendNudge(const QQ_t& target, const QQ_t& subject_id)
	{
		pmem->SendNudge(target.ToInt64(), subject_id.ToInt64(), "Friend");
	}

	void MiraiBot::SendNudge(const QQ_t& target, const GID_t& subject_id)
	{
		pmem->SendNudge(target.ToInt64(), subject_id.ToInt64(), "Group");
	}

	void MiraiBot::SendNudge(const QQ_t& target, const UID_t& subject_id)
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

	void MiraiBot::SetEssence(MessageId_t target)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", target }
		};

		auto res = pmem->httpClient->Post("/setEssence", data.dump(), CONTENT_TYPE.c_str());
		json re_json = ParseOrThrowException(res);
	}

	MiraiImage MiraiBot::pimpl::UploadImage(const string& filename, const string& type)
	{
		string base_filename = filename.substr(filename.find_last_of("/\\") + 1);
		string img_data = ReadFile(filename);
		httplib::MultipartFormDataItems items =
		{
		  { "sessionKey", sessionKey, "", "" },
		  { "type", type, "", "" },
		  { "img", img_data, base_filename, "image/png" }
		};

		auto res = httpClient->Post("/uploadImage", items);
		json re_json = ParseOrThrowException(res);
		MiraiImage img;
		img.Id = re_json["imageId"].get<string>();
		img.Url = re_json.value("url", "");
		img.Path = re_json.value("path", "");
		return img;
	}

	FriendImage MiraiBot::UploadFriendImage(const string& filename)
	{
		return pmem->UploadImage(filename, "friend");
	}

	GroupImage MiraiBot::UploadGroupImage(const string& filename)
	{
		return pmem->UploadImage(filename, "group");
	}

	TempImage MiraiBot::UploadTempImage(const string& filename)
	{
		return pmem->UploadImage(filename, "temp");
	}

	MiraiVoice MiraiBot::pimpl::UploadVoice(const string& filename, const string& type)
	{
		string base_filename = filename.substr(filename.find_last_of("/\\") + 1);
		string voice_data = ReadFile(filename);
		httplib::MultipartFormDataItems items =
		{
		  { "sessionKey", sessionKey, "", "" },
		  { "type", type, "", "" },
		  { "voice", voice_data, base_filename, "application/octet-stream"  }
		};

		auto res = httpClient->Post("/uploadVoice", items);
		json re_json = ParseOrThrowException(res);
		MiraiVoice result;
		result.Id = re_json["voiceId"].get<string>();
		if (!re_json["url"].is_null())
			result.Url = re_json["url"].get<string>();
		result.Path = re_json["path"].get<string>();
		return result;
	}

	MiraiVoice MiraiBot::UploadGroupVoice(const string& filename)
	{
		return pmem->UploadVoice(filename, "group");
	}

	MiraiFile MiraiBot::pimpl::UploadFileAndSend(int64_t target, const string& filename, const string& type)
	{
		string base_filename = filename.substr(filename.find_last_of("/\\") + 1);
		string file_data = ReadFile(filename);
		httplib::MultipartFormDataItems items =
		{
		  { "sessionKey", sessionKey, "", "" },
		  { "type", type, "", "" },
		  { "target", to_string(target), "", "" },
		  { "path", "/" + base_filename, "", "" },
		  { "file", file_data, base_filename, "application/octet-stream"  }
		};

		auto res = httpClient->Post("/uploadFileAndSend", items);
		json re_json = ParseOrThrowException(res);
		MiraiFile result;
		result.FileSize = file_data.size();
		result.FileName = base_filename;
		result.Id = re_json["id"].get<string>();
		return result;
	}

	MiraiFile MiraiBot::UploadFileAndSend(const GID_t& gid, const string& filename)
	{
		return pmem->UploadFileAndSend(gid.ToInt64(), filename, "Group");
	}

	vector<Friend_t> MiraiBot::GetFriendList()
	{
		auto res = pmem->httpClient->Get(("/friendList?sessionKey=" + pmem->sessionKey).data());
		json re_json = ParseOrThrowException(res);
		vector<Friend_t> result;
		for (const auto& ele : re_json["data"])
		{
			Friend_t f;
			f.Set(ele);
			result.emplace_back(f);
		}
		return result;
	}


	vector<Group_t> MiraiBot::GetGroupList()
	{
		auto res = pmem->httpClient->Get(("/groupList?sessionKey=" + pmem->sessionKey).data());
		json re_json = ParseOrThrowException(res);
		vector<Group_t> result;
		for (const auto& ele : re_json["data"])
		{
			Group_t group;
			group.Set(ele);
			result.emplace_back(group);
		}
		return result;
	}


	vector<GroupMember> MiraiBot::GetGroupMembers(const GID_t& target)
	{
		stringstream api_url;
		api_url
			<< "/memberList?sessionKey="
			<< pmem->sessionKey
			<< "&target="
			<< target;
		auto res = pmem->httpClient->Get(api_url.str().data());
		json re_json = ParseOrThrowException(res);
		vector<GroupMember> result;
		for (const auto& ele : re_json["data"])
		{
			GroupMember f;
			f.Set(ele);
			result.push_back(f);
		}
		return result;
	}

	GroupMember MiraiBot::GetGroupMemberInfo(const GID_t& gid, const QQ_t& memberId)
	{
		stringstream api_url;
		api_url
			<< "/memberInfo?sessionKey="
			<< pmem->sessionKey
			<< "&target="
			<< int64_t(gid)
			<< "&memberId="
			<< int64_t(memberId);
		auto res = pmem->httpClient->Get(api_url.str().data());
		json re_json = ParseOrThrowException(res);
		GroupMember result;
		result.Set(re_json);
		return result;
	}

	Profile MiraiBot::GetBotProfile()
	{
		stringstream api_url;
		api_url
			<< "/botProfile?sessionKey="
			<< pmem->sessionKey;
		auto res = pmem->httpClient->Get(api_url.str().data());
		json re_json = ParseOrThrowException(res);
		Profile result;
		result.Set(re_json);
		return result;
	}

	Profile MiraiBot::GetFriendProfile(const QQ_t& qq)
	{
		stringstream api_url;
		api_url
			<< "/friendProfile?sessionKey="
			<< pmem->sessionKey
			<< "&target="
			<< int64_t(qq);
		auto res = pmem->httpClient->Get(api_url.str().data());
		json re_json = ParseOrThrowException(res);
		Profile result;
		result.Set(re_json);
		return result;
	}

	Profile MiraiBot::GetGroupMemberProfile(const GID_t& gid, const QQ_t& memberQQ)
	{
		stringstream api_url;
		api_url
			<< "/memberProfile?sessionKey="
			<< pmem->sessionKey
			<< "&target="
			<< int64_t(gid)
			<< "&memberId="
			<< int64_t(memberQQ);
		auto res = pmem->httpClient->Get(api_url.str().data());
		json re_json = ParseOrThrowException(res);
		Profile result;
		result.Set(re_json);
		return result;
	}

	void MiraiBot::SetGroupMemberName(const GID_t& gid, const QQ_t& memberId, const string& name)
	{
		auto member_info = this->GetGroupMemberInfo(gid, memberId);
		pmem->SetGroupMemberInfo(gid, memberId, name, member_info.SpecialTitle);
	}

	void MiraiBot::SetGroupMemberSpecialTitle(const GID_t& gid, const QQ_t& memberId, const string& title)
	{
		auto member_info = this->GetGroupMemberInfo(gid, memberId);
		pmem->SetGroupMemberInfo(gid, memberId, member_info.MemberName, title);
	}

	vector<GroupFile> MiraiBot::GetGroupFiles(const GID_t& gid, const string& parentId)
	{
		stringstream api_url;
		api_url
			<< "/file/list?sessionKey="
			<< pmem->sessionKey
			<< "&target="
			<< int64_t(gid)
			<< "&id="
			<< parentId;
		// 取文件列表响应比较慢
		pmem->httpClient->set_read_timeout(60, 0);
		auto res = pmem->httpClient->Get(api_url.str().data());
		json re_json = ParseOrThrowException(res);
		vector<GroupFile> result;
		for (const auto& item : re_json["data"])
		{
			GroupFile f;
			f.Set(item);
			result.push_back(f);
		}
		return result;
	}

	GroupFile MiraiBot::GetGroupFilById(const GID_t& gid, const string& groupFileId)
	{
		stringstream api_url;
		api_url
			<< "/file/info?sessionKey="
			<< pmem->sessionKey
			<< "&target="
			<< int64_t(gid)
			<< "&id="
			<< groupFileId;
		auto res = pmem->httpClient->Get(api_url.str().data());
		json re_json = ParseOrThrowException(res);
		GroupFile result;
		result.Set(re_json);
		return result;

	}

	GroupFileInfo MiraiBot::GetGroupFileInfo(GID_t gid, const GroupFile& groupFile)
	{
		stringstream api_url;
		api_url
			<< "/groupFileInfo?sessionKey="
			<< pmem->sessionKey
			<< "&target="
			<< int64_t(gid)
			<< "&id="
			<< groupFile.Id;
		auto res = pmem->httpClient->Get(api_url.str().data());
		json re_json = ParseOrThrowException(res);
		GroupFileInfo result;
		result.Set(re_json);
		return result;
	}

	GroupFile MiraiBot::GroupFileMakeDirectory(const GID_t& target, const string& dictionaryName)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "group", int64_t(target) },
			{ "dictionaryName", dictionaryName }
		};

		auto res = pmem->httpClient->Post("/file/mkdir", data.dump(), CONTENT_TYPE.c_str());
		auto re_json = ParseOrThrowException(res);
		GroupFile result;
		result.Set(re_json);
		return result;
	}

	void MiraiBot::GroupFileRename(const GroupFile& groupFile, const string& newName)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", int64_t(groupFile.Group.GID) },
			{ "id", groupFile.Id },
			{ "renameTo", newName }
		};

		auto res = pmem->httpClient->Post("/file/rename", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}

	void MiraiBot::GroupFileMove(const GroupFile& groupFile, const string& targetId)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", int64_t(groupFile.Group.GID) },
			{ "id", groupFile.Id },
			{ "moveTo", targetId }
		};

		auto res = pmem->httpClient->Post("/file/move", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}

	void MiraiBot::GroupFileDelete(const GroupFile& groupFile)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", int64_t(groupFile.Group.GID) },
			{ "id", groupFile.Id }
		};

		auto res = pmem->httpClient->Post("/file/delete", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}

	void MiraiBot::MuteAll(const GID_t& target)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", int64_t(target)}
		};

		auto res = pmem->httpClient->Post("/muteAll", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}


	void MiraiBot::UnMuteAll(const GID_t& target)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", int64_t(target)}
		};

		auto res = pmem->httpClient->Post("/unmuteAll", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}


	void MiraiBot::Mute(const GID_t& gid, const QQ_t& memberId, unsigned int time_seconds)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", int64_t(gid)},
			{ "memberId", int64_t(memberId)},
			{ "time", time_seconds}
		};

		auto res = pmem->httpClient->Post("/mute", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}


	void MiraiBot::UnMute(const GID_t& gid, const QQ_t& memberId)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", int64_t(gid)},
			{ "memberId", int64_t(memberId)}
		};

		auto res = pmem->httpClient->Post("/unmute", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}


	void MiraiBot::Kick(const GID_t& gid, const QQ_t& memberId, const string& reason_msg)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", int64_t(gid)},
			{ "memberId", int64_t(memberId)},
			{ "reason_msg" , reason_msg}
		};

		auto res = pmem->httpClient->Post("/kick", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}


	void MiraiBot::Recall(MessageId_t mid)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", int64_t(mid)}
		};

		auto res = pmem->httpClient->Post("/recall", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}

	void MiraiBot::QuitGroup(const GID_t& group)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", int64_t(group)}
		};

		auto res = pmem->httpClient->Post("/quit", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}

	void MiraiBot::DeleteFriend(const QQ_t& friendQQ)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", int64_t(friendQQ)}
		};

		auto res = pmem->httpClient->Post("/deleteFriend", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}

	GroupConfig MiraiBot::GetGroupConfig(const GID_t& group)
	{
		stringstream api_url;
		api_url
			<< "/groupConfig?sessionKey="
			<< pmem->sessionKey
			<< "&target="
			<< int64_t(group);
		auto res = pmem->httpClient->Get(api_url.str().data());
		json re_json = ParseOrThrowException(res);
		GroupConfig group_config;
		group_config.Set(re_json);
		return group_config;
	}

	void MiraiBot::SetGroupConfig(const GID_t& group, const GroupConfig& groupConfig)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "target", int64_t(group) }
		};
		data["config"] = groupConfig.ToJson();

		auto res = pmem->httpClient->Post("/groupConfig", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}

	FriendMessage MiraiBot::GetFriendMessageFromId(MessageId_t mid)
	{
		stringstream api_url;
		api_url
			<< "/messageFromId?sessionKey="
			<< pmem->sessionKey
			<< "&id="
			<< mid;
		auto res = pmem->httpClient->Get(api_url.str().data());
		json re_json = ParseOrThrowException(res);
		FriendMessage result;
		result.Set(re_json["data"]);
		return result;
	}


	GroupMessage MiraiBot::GetGroupMessageFromId(MessageId_t mid)
	{
		stringstream api_url;
		api_url
			<< "/messageFromId?sessionKey="
			<< pmem->sessionKey
			<< "&id="
			<< mid;

		auto res = pmem->httpClient->Get(api_url.str().data());
		json re_json = ParseOrThrowException(res);
		GroupMessage result;
		result.Set(re_json["data"]);
		return result;
	}

	void MiraiBot::RegisterCommand(
		const string& commandName,
		const vector<string>& alias,
		const string& description,
		const string& helpMessage)
	{
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "name", commandName },
			{ "alias", json(alias) },
			{ "description", description },
			{ "usage", helpMessage }
		};
		auto res = pmem->httpClient->Post("/cmd/register", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}

	void MiraiBot::SendCommand(const vector<string>& command)
	{
		MessageChain mc;
		for (const auto& val : command)
		{
			mc.Plain(val);
		}
		json data =
		{
			{ "sessionKey", pmem->sessionKey },
			{ "command", mc.ToJson() }
		};
		auto res = pmem->httpClient->Post("/cmd/execute", data.dump(), CONTENT_TYPE.c_str());
		ParseOrThrowException(res);
	}
} // namespace Cyan