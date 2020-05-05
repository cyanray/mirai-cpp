#pragma once
#ifndef mirai_cpp_defs_group_member_info_hpp_H_
#define mirai_cpp_defs_group_member_info_hpp_H_

#include <string>
#include <nlohmann/json.hpp>
#include "serializable.hpp"


namespace Cyan
{

	// 群组成员信息
	class GroupMemberInfo : public Serializable
	{
	public:
		string Name;
		string SpecialTitle;

		GroupMemberInfo() = default;
		GroupMemberInfo(const GroupMemberInfo& gm)
		{
			Name = gm.Name;
			SpecialTitle = gm.SpecialTitle;
		}
		GroupMemberInfo& operator=(const GroupMemberInfo& t)
		{
			GroupMemberInfo tmp(t);
			std::swap(this->Name, tmp.Name);
			std::swap(this->SpecialTitle, tmp.SpecialTitle);
			return *this;
		}
		virtual ~GroupMemberInfo() = default;
		virtual bool Set(const json& j) override
		{
			Name = j["name"].get<string>();
			SpecialTitle = j["specialTitle"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["name"] = Name;
			j["specialTitle"] = SpecialTitle;
			return j;
		}

	};

}

#endif // !mirai_cpp_defs_group_member_info_hpp_H_