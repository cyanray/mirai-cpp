#pragma once
#ifndef mirai_cpp__typedef_hpp_H_
#define mirai_cpp__typedef_hpp_H_
#include <cstdint>
#include <string>
#include <exception>
#include "serializable.hpp"
#include <rapidjson/document.h>
using std::runtime_error;
using std::string;
namespace Cyan
{

	string JsonDoc2String(const JsonDoc& jsonDoc)
	{
		using namespace rapidjson;
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		jsonDoc.Accept(writer);
		return buffer.GetString();
	}

	// QQ 号码类型
	typedef int64_t QQ_t;
	// 群号码类型
	typedef int64_t GID_t;
	// 消息源 ID
	typedef int64_t MessageSourceID;
	// Document 含义不够清晰，重命名为 JsonDoc 和 JsonVal
	typedef rapidjson::Document JsonDoc;
	typedef rapidjson::Value JsonVal;


	enum class GroupPermission
	{
		Member,
		Administrator,
		Owner
	};

	// 发给好友的图片类型
	struct FriendImage
	{
		string ID;
	};

	// 发给群组的图片类型
	struct GroupImage
	{
		string ID;
	};

	// 好友数据格式
	class Friend_t : public Serializable
	{
	public:
		Friend_t() = default;
		Friend_t(const Friend_t& f)
		{
			QQ = f.QQ;
			NickName = f.NickName;
			Remark = f.Remark;
		}
		virtual ~Friend_t() = default;
		QQ_t QQ;
		string NickName;
		string Remark;
		virtual bool Set(JsonVal& json) override
		{
			auto qq_it = json.FindMember("id");
			auto nickName_it = json.FindMember("nickName");
			auto remark_it = json.FindMember("remark");
			if (qq_it == json.MemberEnd() || !qq_it->value.IsNumber())
				throw runtime_error("解析 JSON 时出错");
			if (nickName_it == json.MemberEnd() || !nickName_it->value.IsString())
				throw runtime_error("解析 JSON 时出错");
			if (remark_it == json.MemberEnd() || !remark_it->value.IsString())
				throw runtime_error("解析 JSON 时出错");
			QQ = qq_it->value.GetInt64();
			NickName = string(nickName_it->value.GetString(), nickName_it->value.GetStringLength());
			Remark = string(remark_it->value.GetString(), remark_it->value.GetStringLength());
			return true;
		}
		virtual JsonDoc& ToJson() override
		{
			using namespace rapidjson;
			Value v(kObjectType);
			v.AddMember("id", QQ, jdata_.GetAllocator());
			Value nn; nn.SetString(NickName.data(), NickName.size(), jdata_.GetAllocator());
			v.AddMember("nickName", nn, jdata_.GetAllocator());
			Value r; nn.SetString(Remark.data(), Remark.size(), jdata_.GetAllocator());
			v.AddMember("remark", r, jdata_.GetAllocator());
			v.Swap(jdata_);
			return jdata_;
		}
		virtual string ToString() override
		{
			return JsonDoc2String(ToJson());
		}
	private:
		JsonDoc jdata_;
	};

	// 群组数据格式
	class Group_t : public Serializable
	{
	public:
		Group_t() = default;
		Group_t(const Group_t& g)
		{
			GID = g.GID;
			Name = g.Name;
			Permission = g.Permission;
		}
		Group_t& operator=(const Group_t& t)
		{
			Group_t tmp(t);
			std::swap(this->GID, tmp.GID);
			std::swap(this->Name, tmp.Name);
			std::swap(this->Permission, tmp.Permission);
			return *this;
		}
		virtual ~Group_t() = default;
		GID_t GID;
		string Name;
		GroupPermission Permission;
		virtual bool Set(JsonVal& json) override
		{
			auto gid_it = json.FindMember("id");
			auto name_it = json.FindMember("name");
			auto permission_it = json.FindMember("permission");
			if (gid_it == json.MemberEnd() || !gid_it->value.IsNumber())
				throw runtime_error("解析 JSON 时出错");
			if (name_it == json.MemberEnd() || !name_it->value.IsString())
				throw runtime_error("解析 JSON 时出错");
			if (permission_it == json.MemberEnd() || !permission_it->value.IsString())
				throw runtime_error("解析 JSON 时出错");
			GID = gid_it->value.GetInt64();
			Name = string(name_it->value.GetString(), name_it->value.GetStringLength());
			string pStr = string(permission_it->value.GetString(), permission_it->value.GetStringLength());
			if (pStr == "MEMBER") Permission = GroupPermission::Member;
			else if (pStr == "ADMINISTRATOR") Permission = GroupPermission::Administrator;
			else if (pStr == "OWNER") Permission = GroupPermission::Owner;
			else throw runtime_error("未知的 Permission");
			return true;
		}
		virtual JsonDoc& ToJson() override
		{
			using namespace rapidjson;

			string pStr;
			switch (Permission)
			{
			case GroupPermission::Member:
				pStr = "MEMBER";
				break;
			case GroupPermission::Administrator:
				pStr = "ADMINISTRATOR";
				break;
			case GroupPermission::Owner:
				pStr = "OWNER";
				break;
			}

			Value v(kObjectType);
			v.AddMember("id", GID, jdata_.GetAllocator());
			Value n; n.SetString(Name.data(), Name.size(), jdata_.GetAllocator());
			v.AddMember("name", n, jdata_.GetAllocator());
			Value p; p.SetString(pStr.data(), pStr.size(), jdata_.GetAllocator());
			v.AddMember("permission", p, jdata_.GetAllocator());
			v.Swap(jdata_);
			return jdata_;
		}
		virtual string ToString() override
		{
			return JsonDoc2String(ToJson());
		}
	private:
		JsonDoc jdata_;
	};

	class GroupMember_t : public Serializable
	{
	public:
		GroupMember_t() = default;
		GroupMember_t(const GroupMember_t& gm)
		{
			QQ = gm.QQ;
			MemberName = gm.MemberName;
			Permission = gm.Permission;
			Group = gm.Group;
		}
		virtual ~GroupMember_t() = default;
		QQ_t QQ;
		string MemberName;
		GroupPermission Permission;
		Group_t Group;
		virtual bool Set(JsonVal& json) override
		{
			auto qq_it = json.FindMember("id");
			auto memeber_name_it = json.FindMember("memberName");
			auto permission_it = json.FindMember("permission");
			auto group_it = json.FindMember("group");
			if (qq_it == json.MemberEnd() || !qq_it->value.IsNumber())
				throw runtime_error("解析 JSON 时出错");
			if (memeber_name_it == json.MemberEnd() || !memeber_name_it->value.IsString())
				throw runtime_error("解析 JSON 时出错");
			if (permission_it == json.MemberEnd() || !permission_it->value.IsString())
				throw runtime_error("解析 JSON 时出错");
			if (group_it == json.MemberEnd() || !group_it->value.IsObject())
				throw runtime_error("解析 JSON 时出错");
			QQ = qq_it->value.GetInt64();
			MemberName = string(memeber_name_it->value.GetString(), memeber_name_it->value.GetStringLength());
			string pStr = string(permission_it->value.GetString(), permission_it->value.GetStringLength());
			if (pStr == "MEMBER") Permission = GroupPermission::Member;
			else if (pStr == "ADMINISTRATOR") Permission = GroupPermission::Administrator;
			else if (pStr == "OWNER") Permission = GroupPermission::Owner;
			else throw runtime_error("未知的 Permission");
			Group.Set(group_it->value);
			return true;
		}
		virtual JsonDoc& ToJson() override
		{
			using namespace rapidjson;

			string pStr;
			switch (Permission)
			{
			case GroupPermission::Member:
				pStr = "MEMBER";
				break;
			case GroupPermission::Administrator:
				pStr = "ADMINISTRATOR";
				break;
			case GroupPermission::Owner:
				pStr = "OWNER";
				break;
			}

			Value v(kObjectType);
			v.AddMember("id", QQ, jdata_.GetAllocator());
			Value n; n.SetString(MemberName.data(), MemberName.size(), jdata_.GetAllocator());
			v.AddMember("memberName", n, jdata_.GetAllocator());
			Value p; p.SetString(pStr.data(), pStr.size(), jdata_.GetAllocator());
			v.AddMember("permission", p, jdata_.GetAllocator());
			v.AddMember("group", Group.ToJson(), jdata_.GetAllocator());
			v.Swap(jdata_);
			return jdata_;
		}
		virtual string ToString() override
		{
			return JsonDoc2String(ToJson());
		}
	private:
		JsonDoc jdata_;
	};

}

#endif // !mirai_cpp__message_chain_hpp_H_
