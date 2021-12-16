#pragma once
#ifndef mirai_cpp_defs_group_hpp_H_
#define mirai_cpp_defs_group_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "QQType.hpp"
#include "ISerializable.hpp"

namespace Cyan
{

	/**
	 * @brief QQ群
	*/
	class Group_t : public ISerializable
	{
	public:
		/**
		 * @brief 群号码
		*/
		GID_t GID;

		/**
		 * @brief 群昵称
		*/
		string Name;

		/**
		 * @brief 机器人在该群的权限
		*/
		GroupPermission Permission = GroupPermission::Member;

		virtual bool Set(const json& j) override
		{
			GID = GID_t(j["id"].get<int64_t>());
			Name = j["name"].get<string>();
			Permission = GroupPermissionStr(j["permission"].get<string>());
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["id"] = int64_t(GID);
			j["name"] = Name;
			j["permission"] = GroupPermissionStr(Permission);
			return j;
		}
	};

}

#endif // !mirai_cpp_defs_group_hpp_H_