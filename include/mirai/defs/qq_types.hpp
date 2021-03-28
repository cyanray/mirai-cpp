#pragma once
#ifndef mirai_cpp_defs_qq_types_hpp_H_
#define mirai_cpp_defs_qq_types_hpp_H_

#include <exception>
#include <iostream>
#include "serializable.hpp"
#include "mirai/exported.h"

namespace Cyan
{
	// QQ_t 和 GID_t 的基类型
	class UID_t
	{
	public:
		int64_t ToInt64() const
		{
			return data_;
		}
		explicit operator int64_t() const
		{
			return data_;
		}
		bool operator==(const UID_t& a) const
		{
			return this->data_ == a.data_;
		}
		bool operator!=(const UID_t& a) const
		{
			return !operator==(a);
		}
		virtual ~UID_t() = default;
	protected:
		explicit UID_t(int64_t id) : data_(id) {}
		virtual void __avoiding_object_slicing() const = 0;
		int64_t data_;
	};
	
	// 储存 QQ 号码的类型
	class QQ_t : public UID_t
	{
	public:
		QQ_t() : UID_t(-1) {}
		explicit QQ_t(int64_t id) : UID_t(id) {}
	private:
		virtual void __avoiding_object_slicing() const override {}
	};

	// 储存群号码的类型
	class GID_t : public UID_t
	{
	public:
		GID_t() : UID_t(-1) {}
		explicit GID_t(int64_t id) : UID_t(id) {}
	private:
		virtual void __avoiding_object_slicing() const override {}
	};

	inline GID_t operator ""_gid(unsigned long long int id)
	{
		return (GID_t)(id);
	}

	inline QQ_t operator ""_qq(unsigned long long int id)
	{
		return (QQ_t)(id);
	}

	inline std::ostream& operator<<(std::ostream& o, const UID_t& uid)
	{
		o << (int64_t)(uid);
		return o;
	}

	// 消息 ID
	typedef int64_t MessageId_t;

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

	struct MiraiImage
	{
		string ID;
		string Url;
		string Path;
	};

	typedef MiraiImage FriendImage;

	typedef MiraiImage GroupImage;

	typedef MiraiImage TempImage;

	struct MiraiVoice
	{
		string Id;
		string Url;
		string Path;
	};

	// 预先声明 MiraiBot 类
	class EXPORTED MiraiBot;

}

#endif // !mirai_cpp_defs_qq_types_hpp_H_