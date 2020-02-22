#pragma once
#ifndef mirai_cpp__mirai_hpp_H_
#define mirai_cpp__mirai_hpp_H_
#include <CURLWrapper.h>
#include <string>
#include <vector>
#include <exception>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>  
#include "typedef.hpp"
#include "message_chain.hpp"
#include <iostream>
using std::string;
using std::runtime_error;
using std::vector;

namespace Cyan
{
	class MiraiBot
	{
	public:
		MiraiBot() = default;
		~MiraiBot() = default;
		bool Auth(const string& authKey, QQ_t qq)
		{
			using namespace rapidjson;
			static const char* const jsonStr = R"( { "authKey" : "" } )";
			static const string api_url = api_url_prefix_ + "/auth";
			JsonDoc jdoc;
			if (jdoc.Parse(jsonStr).HasParseError()) throw runtime_error("未知错误");
			jdoc["authKey"].SetString(authKey.data(), authKey.size());
			string pData = JsonDoc2String(jdoc);

			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				JsonDoc reJson;
				if (reJson.Parse(res.Content.data()).HasParseError())
					throw runtime_error("解析响应 JSON 时出错");
				Value::MemberIterator code_it = reJson.FindMember("code");
				if (code_it == reJson.MemberEnd() || !code_it->value.IsNumber())
					throw runtime_error("解析响应 JSON 时出错");
				if (code_it->value.GetInt() == 0)
				{
					Value::MemberIterator session_it = reJson.FindMember("session");
					if (session_it == reJson.MemberEnd() || !session_it->value.IsString())
						throw runtime_error("解析响应 JSON 时出错");
					this->sessionKey_ = string(session_it->value.GetString(), session_it->value.GetStringLength());
					this->qq_ = qq;
					return SessionVerify();
				}
				else
					throw runtime_error("Auth Key 不正确");
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;
		}
		bool SendFriendMessage(QQ_t target, MessageChain& messageChain)
		{
			using namespace rapidjson;
			static const char* const jsonStr = R"( { "sessionKey": "" , "target": 0 , "messageChain": null } )";
			static const string api_url = api_url_prefix_ + "/sendFriendMessage";
			JsonDoc jdoc;
			if (jdoc.Parse(jsonStr).HasParseError()) throw runtime_error("未知错误");
			jdoc["sessionKey"].SetString(sessionKey_.data(), sessionKey_.size());
			jdoc["target"].SetInt64(target);
			// 深度复制 messageChain
			Value mc(messageChain.ToJson(), jdoc.GetAllocator());
			jdoc["messageChain"] = mc;
			string pData = JsonDoc2String(jdoc);

			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				JsonDoc reJson;
				if (reJson.Parse(res.Content.data()).HasParseError())
					throw runtime_error("解析响应 JSON 时出错");
				Value::MemberIterator code_it = reJson.FindMember("code");
				Value::MemberIterator msg_it = reJson.FindMember("msg");
				if (code_it == reJson.MemberEnd() || !code_it->value.IsNumber())
					throw runtime_error("解析响应 JSON 时出错");
				if (msg_it == reJson.MemberEnd() || !msg_it->value.IsString())
					throw runtime_error("解析响应 JSON 时出错");
				if (code_it->value.GetInt() == 0)
					return true;
				else
					throw runtime_error(msg_it->value.GetString());
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;
		}
		bool SendGroupMessage(QQ_t target, MessageChain& messageChain)
		{
			using namespace rapidjson;
			static const char* const jsonStr = R"( { "sessionKey": "" , "target": 0 , "messageChain": null } )";
			static const string api_url = api_url_prefix_ + "/sendGroupMessage";
			JsonDoc jdoc;
			if (jdoc.Parse(jsonStr).HasParseError()) throw runtime_error("未知错误");
			jdoc["sessionKey"].SetString(sessionKey_.data(), sessionKey_.size());
			jdoc["target"].SetInt64(target);
			// 深度复制 messageChain
			Value mc(messageChain.ToJson(), jdoc.GetAllocator());
			jdoc["messageChain"] = mc;
			string pData = JsonDoc2String(jdoc);

			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				JsonDoc reJson;
				if (reJson.Parse(res.Content.data()).HasParseError())
					throw runtime_error("解析响应 JSON 时出错");
				Value::MemberIterator code_it = reJson.FindMember("code");
				Value::MemberIterator msg_it = reJson.FindMember("msg");
				if (code_it == reJson.MemberEnd() || !code_it->value.IsNumber())
					throw runtime_error("解析响应 JSON 时出错");
				if (msg_it == reJson.MemberEnd() || !msg_it->value.IsString())
					throw runtime_error("解析响应 JSON 时出错");
				if (code_it->value.GetInt() == 0)
					return true;
				else
					throw runtime_error(msg_it->value.GetString());
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;
		}
		bool SendQuoteMessage(MessageSourceID target, const MessageChain& messageChain);
		FriendImage UploadFriendImage(const string& fileName);
		GroupImage UploadGroupImage(const string& fileName);
		vector<Friend_t> GetFriendList()
		{
			using namespace rapidjson;
			static const string api_url = api_url_prefix_ + "/friendList?sessionKey=" + sessionKey_;
			vector<Friend_t> result;

			HTTP http;
			auto res = http.Get(api_url);
			if (res.Ready)
			{
				JsonDoc reJson;
				if (reJson.Parse(res.Content.data()).HasParseError())
					throw runtime_error("解析响应 JSON 时出错");
				if (!reJson.IsArray())
					throw runtime_error("解析响应 JSON 时出错");
				for (SizeType i = 0; i < reJson.Size(); i++)
				{
					Friend_t f;
					f.Set(reJson[i]);
					result.push_back(f);
				}
			}
			else
				throw runtime_error(res.ErrorMsg);
			return result;

		}
		vector<Group_t> GetGroupList() const;
		vector<GroupMember_t> GetGroupMembers(GID_t target) const;
		bool MuteAll(GID_t target);
		bool UnMuteAll(GID_t target);
		bool Mute(GID_t GID, QQ_t memberID, unsigned int time);
		bool UnMute(GID_t GID, QQ_t memberID, unsigned int time);
		bool Kick(GID_t GID, QQ_t memberID, const string& msg);
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
	private:
		bool SessionVerify() const
		{
			using namespace rapidjson;
			static const char* const jsonStr = R"( { "sessionKey" : "", "qq":1234567 } )";
			static const string api_url = api_url_prefix_ + "/verify";
			JsonDoc jdoc;
			if (jdoc.Parse(jsonStr).HasParseError()) throw runtime_error("未知错误");
			jdoc["sessionKey"].SetString(sessionKey_.data(), sessionKey_.size());
			jdoc["qq"].SetInt64(qq_);
			string pData = JsonDoc2String(jdoc);

			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				JsonDoc reJson;
				if (reJson.Parse(res.Content.data()).HasParseError())
					throw runtime_error("解析响应 JSON 时出错");
				Value::MemberIterator code_it = reJson.FindMember("code");
				Value::MemberIterator msg_it = reJson.FindMember("msg");
				if (code_it == reJson.MemberEnd() || !code_it->value.IsNumber())
					throw runtime_error("解析响应 JSON 时出错");
				if (msg_it == reJson.MemberEnd() || !msg_it->value.IsString())
					throw runtime_error("解析响应 JSON 时出错");
				if (code_it->value.GetInt() == 0)
					return true;
				else
					throw runtime_error(msg_it->value.GetString());
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;
		}
		bool SessionRelease() const
		{
			using namespace rapidjson;
			static const char* const jsonStr = R"( { "sessionKey" : "", "qq":1234567 } )";
			static const string api_url = api_url_prefix_ + "/release";
			JsonDoc jdoc;
			if (jdoc.Parse(jsonStr).HasParseError()) throw runtime_error("未知错误");
			jdoc["sessionKey"].SetString(sessionKey_.data(), sessionKey_.size());
			jdoc["qq"].SetInt64(qq_);
			string pData = JsonDoc2String(jdoc);

			HTTP http; http.SetContentType("application/json;charset=UTF-8");
			auto res = http.Post(api_url, pData);
			if (res.Ready)
			{
				JsonDoc reJson;
				if (reJson.Parse(res.Content.data()).HasParseError())
					throw runtime_error("解析响应 JSON 时出错");
				Value::MemberIterator code_it = reJson.FindMember("code");
				Value::MemberIterator msg_it = reJson.FindMember("msg");
				if (code_it == reJson.MemberEnd() || !code_it->value.IsNumber())
					throw runtime_error("解析响应 JSON 时出错");
				if (msg_it == reJson.MemberEnd() || !msg_it->value.IsString())
					throw runtime_error("解析响应 JSON 时出错");
				if (code_it->value.GetInt() == 0)
					return true;
				else
					throw runtime_error(msg_it->value.GetString());
			}
			else
				throw runtime_error(res.ErrorMsg);
			return false;

		}
		QQ_t qq_;
		string sessionKey_;
		string api_url_prefix_ = "http://localhost:8080";
	};
} // namespace Cyan



#endif // !_mirai_cpp_mirai_hpp__
