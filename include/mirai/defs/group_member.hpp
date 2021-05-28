#pragma once
#ifndef mirai_cpp_defs_group_member_hpp_H_
#define mirai_cpp_defs_group_member_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "qq_types.hpp"
#include "serializable.hpp"
#include "group.hpp"

namespace Cyan
{

	/**
	 * @brief QQ群成员
	*/
	class GroupMember_t : public ISerializable
	{
	public:
		/**
		 * @brief QQ号码
		*/
		QQ_t QQ;

		/**
		 * @brief 群名片
		*/
		string MemberName;

		/**
		 * @brief QQ群成员在该群的权限
		*/
		GroupPermission Permission = GroupPermission::Member;

		/**
		 * @brief QQ群
		*/
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