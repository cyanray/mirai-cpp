#pragma once
#ifndef mirai_cpp_defs_qq_types_hpp_H_
#define mirai_cpp_defs_qq_types_hpp_H_

#include <exception>
#include <iostream>
#include <functional>
#include "ISerializable.hpp"
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

		bool operator<(const UID_t& a) const
		{
			return data_ < a.data_;
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

	/**
	 * @brief 上传图片后得到的对象，可用于发图片消息
	*/
	typedef struct MiraiImage
	{
		string Id;
		string Url;
		string Path;
		string Base64;
	} FriendImage, GroupImage, TempImage;

	/**
	 * @brief 上传语音后得到的对象，可用于发语音消息
	*/
	struct MiraiVoice
	{
		string Id;
		string Url;
		string Path;
		string Base64;
		/**
		 * @brief 语音长度
		*/
		size_t Length = 0;
	};

	/**
	 * @brief 上传文件后得到的对象，可用于发文件消息
	*/
	struct MiraiFile
	{
		string Id;
		string FileName;
		/**
		 * @brief 文件大小
		*/
		size_t FileSize = 0;
		int InternalId = 102;
	};

	// 预先声明 MiraiBot 类
	class EXPORTED MiraiBot;

}

namespace std
{
	template<> struct hash<Cyan::UID_t>
	{
		size_t operator() (const Cyan::UID_t& t) const {
			return size_t(t.ToInt64());
		}
	};
	template<> struct hash<Cyan::GID_t>
	{
		size_t operator() (const Cyan::GID_t& t) const {
			return size_t(t.ToInt64());
		}
	};
	template<> struct hash<Cyan::QQ_t>
	{
		size_t operator() (const Cyan::QQ_t& t) const {
			return size_t(t.ToInt64());
		}
	};
}

#endif // !mirai_cpp_defs_qq_types_hpp_H_