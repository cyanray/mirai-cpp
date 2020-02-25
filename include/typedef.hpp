#pragma once
#ifndef mirai_cpp__typedef_hpp_H_
#define mirai_cpp__typedef_hpp_H_
#include <cstdint>
#include <string>
#include <exception>
#include <sstream>
#include <nlohmann/json.hpp>
#include "serializable.hpp"
using std::runtime_error;
using std::string;
using nlohmann::json;
namespace Cyan
{
	// QQ 号码类型
	typedef int64_t QQ_t;

	// 群号码类型
	typedef int64_t GID_t;

	// 消息源 ID
	typedef int64_t MessageSourceID;

	// 群成员权限
	enum class GroupPermission
	{
		Member,
		Administrator,
		Owner
	};

	inline string GroupPermissionStr(GroupPermission gp)
	{
		string result;
		switch (gp)
		{
		case Cyan::GroupPermission::Member:
			result = "MEMBER";
			break;
		case Cyan::GroupPermission::Administrator:
			result = "ADMINISTRATOR";
			break;
		case Cyan::GroupPermission::Owner:
			result = "OWNER";
			break;
		}
		return result;
	}

	inline GroupPermission GroupPermissionStr(const string& gp)
	{
		if (gp == "MEMBER") return GroupPermission::Member;
		if (gp == "ADMINISTRATOR") return GroupPermission::Administrator;
		if (gp == "OWNER") return GroupPermission::Owner;
		throw runtime_error("错误的 GroupPermissionStr");
	}

	// Mirai 事件类型
	enum class MiraiEvent
	{
		Default,
		FriendMessage,
		GroupMessage
	};

	inline MiraiEvent MiraiEventStr(const string& miraiEvent)
	{
		if (miraiEvent == "FriendMessage") return MiraiEvent::FriendMessage;
		if (miraiEvent == "GroupMessage") return MiraiEvent::GroupMessage;
		return MiraiEvent::Default;
	}

	inline string MiraiEventStr(MiraiEvent miraiEvent)
	{
		string result;
		switch (miraiEvent)
		{
		case Cyan::MiraiEvent::FriendMessage:
			result = "FriendMessage";
			break;
		case Cyan::MiraiEvent::GroupMessage:
			result = "GroupMessage";
			break;
		default:
			result = "Default";
			break;
		}
		return result;
	}


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
		QQ_t QQ;
		string NickName;
		string Remark;

		Friend_t() = default;
		Friend_t(const Friend_t& f)
		{
			QQ = f.QQ;
			NickName = f.NickName;
			Remark = f.Remark;
		}
		Friend_t& operator=(const Friend_t& t)
		{
			Friend_t tmp(t);
			std::swap(this->QQ, tmp.QQ);
			std::swap(this->NickName, tmp.NickName);
			std::swap(this->Remark, tmp.Remark);
			return *this;
		}
		virtual ~Friend_t() = default;
		virtual bool Set(const json& j) override
		{
			QQ = j["id"].get<QQ_t>();
			NickName = j["nickName"].get<string>();
			Remark = j["remark"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["id"] = QQ;
			j["nickName"] = NickName;
			j["remark"] = Remark;
			return j;
		}

	};

	// 群组数据格式
	class Group_t : public Serializable
	{
	public:
		GID_t GID;
		string Name;
		GroupPermission Permission;

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
		virtual bool Set(const json& j) override
		{
			GID = j["id"].get<GID_t>();
			Name = j["name"].get<string>();
			Permission = GroupPermissionStr(j["permission"].get<string>());
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["id"] = GID;
			j["nickName"] = Name;
			j["remark"] = GroupPermissionStr(Permission);
			return j;
		}
	};

	// 群组成员格式
	class GroupMember_t : public Serializable
	{
	public:
		QQ_t QQ;
		string MemberName;
		GroupPermission Permission;
		Group_t Group;

		GroupMember_t() = default;
		GroupMember_t(const GroupMember_t& gm)
		{
			QQ = gm.QQ;
			MemberName = gm.MemberName;
			Permission = gm.Permission;
			Group = gm.Group;
		}
		GroupMember_t& operator=(const GroupMember_t& t)
		{
			GroupMember_t tmp(t);
			std::swap(this->QQ, tmp.QQ);
			std::swap(this->MemberName, tmp.MemberName);
			std::swap(this->Permission, tmp.Permission);
			std::swap(this->Group, tmp.Group);
			return *this;
		}
		virtual ~GroupMember_t() = default;
		virtual bool Set(const json& j) override
		{
			QQ = j["id"].get<GID_t>();
			MemberName = j["memberName"].get<string>();
			Permission = GroupPermissionStr(j["permission"].get<string>());
			Group.Set(j["group"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["id"] = QQ;
			j["memberName"] = MemberName;
			j["permission"] = GroupPermissionStr(Permission);
			j["group"] = Group.ToJson();
			return j;
		}

	};

	// MessageChain 消息容器
	class MessageChain : public Serializable
	{
	public:
		MessageChain() :messages_(json::array()) {}
		MessageChain(const MessageChain& mc)
		{
			messages_ = mc.messages_;
		}
		MessageChain(MessageChain&& mc) noexcept
		{
			std::swap(messages_, mc.messages_);
		}
		MessageChain& operator=(const MessageChain& mc)
		{
			MessageChain tmp(mc);
			std::swap(messages_, tmp.messages_);
			return *this;
		}
		MessageChain& operator=(MessageChain&& mc) noexcept
		{
			std::swap(messages_, mc.messages_);
			return *this;
		}
		MessageChain& operator+(const MessageChain& mc)
		{
			messages_.insert(messages_.end(), mc.messages_.begin(), mc.messages_.end());
			return *this;
		}
		virtual ~MessageChain() = default;
		MessageChain& At(const QQ_t qq)
		{
			json j;
			j["type"] = "At";
			j["target"] = qq;
			j["display"] = "@@";
			messages_.push_back(j);
			return *this;
		}
		MessageChain& AtAll()
		{
			messages_.push_back({ {"type","AtAll"} });
			return *this;
		}
		MessageChain& Face(int faceID)
		{
			json j;
			j["type"] = "Face";
			j["faceId"] = faceID;
			messages_.push_back(j);
			return *this;
		}
		MessageChain& Plain(const string& plainText)
		{
			json j;
			j["type"] = "Plain";
			j["text"] = plainText;
			messages_.push_back(j);
			return *this;
		}
		MessageChain& Image(const FriendImage& Image)
		{
			json j;
			j["type"] = "Image";
			j["imageId"] = Image.ID;
			messages_.push_back(j);
			return *this;
		}
		MessageChain& Image(const GroupImage& Image)
		{
			json j;
			j["type"] = "Image";
			j["imageId"] = Image.ID;
			messages_.push_back(j);
			return *this;
		}

		string GetPlainText() const
		{
			using std::stringstream;
			stringstream ss;
			for (const auto& ele : messages_)
			{
				if (ele["type"].get<string>() == "Plain")
				{
					ss << ele["text"].get<string>();
				}
			}
			return ss.str();
		}
		string GetPlainTextFirst() const
		{
			for (const auto& ele : messages_)
			{
				if (ele["type"].get<string>() == "Plain")
				{
					return ele["text"].get<string>();
				}
			}
			return string();
		}

		virtual bool Set(const json& j) override
		{
			messages_ = j;
			return true;
		}
		virtual json ToJson() const override
		{
			return messages_;
		}
		virtual string ToString() const override
		{
			return messages_.dump();
		}

	private:
		json messages_;
	};

	// 好友发来的消息
	class FriendMessage : public Serializable
	{
	public:
		MessageChain MessageChain;
		Friend_t Sender;

		FriendMessage() = default;
		FriendMessage(const FriendMessage& gm)
		{
			MessageChain = gm.MessageChain;
			Sender = gm.Sender;
		}
		FriendMessage& operator=(const FriendMessage& t)
		{
			FriendMessage tmp(t);
			std::swap(this->MessageChain, tmp.MessageChain);
			std::swap(this->Sender, tmp.Sender);
			return *this;
		}
		virtual ~FriendMessage() = default;
		virtual bool Set(const json& j) override
		{
			this->MessageChain.Set(j["messageChain"]);
			this->Sender.Set(j["sender"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["messageChain"] = this->MessageChain.ToJson();
			j["sender"] = this->Sender.ToJson();
			return j;
		}
	};

	// 群组发来的消息
	class GroupMessage : public Serializable
	{
	public:
		MessageChain MessageChain;
		GroupMember_t Sender;

		GroupMessage() = default;
		GroupMessage(const GroupMessage& gm)
		{
			MessageChain = gm.MessageChain;
			Sender = gm.Sender;
		}
		GroupMessage& operator=(const GroupMessage& t)
		{
			GroupMessage tmp(t);
			std::swap(this->MessageChain, tmp.MessageChain);
			std::swap(this->Sender, tmp.Sender);
			return *this;
		}
		virtual ~GroupMessage() = default;
		virtual bool Set(const json& j) override
		{
			this->MessageChain.Set(j["messageChain"]);
			Sender.Set(j["sender"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["messageChain"] = this->MessageChain.ToJson();
			j["sender"] = this->Sender.ToJson();
			return j;
		}
	};

}

#endif // !mirai_cpp__message_chain_hpp_H_
