#pragma once
#ifndef mirai_cpp_defs_group_hpp_H_
#define mirai_cpp_defs_group_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "qq_types.hpp"
#include "serializable.hpp"

namespace Cyan
{

	// 群组数据格式
	class Group_t : public ISerializable
	{
	public:
		GID_t GID;
		string Name;
		GroupPermission Permission;
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
			j["nickname"] = Name;
			j["remark"] = GroupPermissionStr(Permission);
			return j;
		}
	};

}

#endif // !mirai_cpp_defs_group_hpp_H_