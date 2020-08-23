#pragma once
#ifndef mirai_cpp_defs_group_member_hpp_H_
#define mirai_cpp_defs_group_member_hpp_H_

#include "third-party/nlohmann/json.hpp"
#include "qq_types.hpp"
#include "serializable.hpp"
#include "group.hpp"

namespace Cyan
{

	// 群组成员格式
	class GroupMember_t : public ISerializable
	{
	public:
		QQ_t QQ;
		string MemberName;
		GroupPermission Permission;
		Group_t Group;
		virtual bool Set(const json& j) override
		{
			QQ = (QQ_t)(j["id"].get<int64_t>());
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