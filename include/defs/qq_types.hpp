#pragma once
#ifndef mirai_cpp_defs_qq_types_hpp_H_
#define mirai_cpp_defs_qq_types_hpp_H_

#include <exception>
#include "serializable.hpp"

namespace Cyan
{
	// QQ 号码类型
	struct QQ_t
	{
		int64_t QQ;
		QQ_t() :QQ(-1) {}
		explicit  QQ_t(int64_t qq) :QQ(qq) {}
		operator int64_t() const { return QQ; }
	};

	QQ_t operator "" qq(unsigned long long int v)
	{
		return QQ_t(int64_t(v));
	}

	QQ_t operator "" _qq(unsigned long long int v)
	{
		return QQ_t(int64_t(v));
	}

	// 群号码类型
	struct GID_t
	{
		int64_t GID;
		GID_t() :GID(-1) {}
		explicit GID_t(int64_t gid) :GID(gid) {}
		operator int64_t() const { return GID; }
	};

	GID_t operator "" gid(unsigned long long int v)
	{
		return GID_t(int64_t(v));
	}

	GID_t operator "" _gid(unsigned long long int v)
	{
		return GID_t(int64_t(v));
	}

	// 消息 ID
	typedef int64_t MessageId;

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
		throw std::runtime_error("错误的 GroupPermissionStr");
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
		string Url;
		string Path;
	};

	// 发给群组的图片类型
	struct GroupImage
	{
		string ID;
		string Url;
		string Path;
	};


}

#endif // !mirai_cpp_defs_qq_types_hpp_H_