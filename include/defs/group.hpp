#pragma once
#ifndef mirai_cpp_defs_group_hpp_H_
#define mirai_cpp_defs_group_hpp_H_

#include <nlohmann/json.hpp>
#include "qq_types.hpp"
#include "serializable.hpp"

namespace Cyan
{

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
			GID = (GID_t)(j["id"].get<int64_t>());
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