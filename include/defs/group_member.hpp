#pragma once
#ifndef mirai_cpp_defs_group_member_hpp_H_
#define mirai_cpp_defs_group_member_hpp_H_

#include <nlohmann/json.hpp>
#include "qq_types.hpp"
#include "serializable.hpp"
#include "group.hpp"

namespace Cyan
{

	// 群组成员格式
	class GroupMember_t : public Serializable
	{
	public:
		QQ_t QQ = 0;
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
			QQ = j["id"].get<int64_t>();
			MemberName = j["memberName"].get<string>();
			Permission = GroupPermissionStr(j["permission"].get<string>());
			Group.Set(j["group"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["id"] = int64_t(QQ);
			j["memberName"] = MemberName;
			j["permission"] = GroupPermissionStr(Permission);
			j["group"] = Group.ToJson();
			return j;
		}

	};

}

#endif // !mirai_cpp_defs_group_member_hpp_H_