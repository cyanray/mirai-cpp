#pragma once
#ifndef mirai_cpp_defs_group_member_hpp_H_
#define mirai_cpp_defs_group_member_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "QQType.hpp"
#include "ISerializable.hpp"
#include "Group.hpp"

namespace Cyan
{

	/**
	 * @brief QQ群成员
	*/
	class GroupMember : public ISerializable
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
		 * @brief 群头衔
		*/
		string SpecialTitle;


		/**
		 * @brief 入群时间戳
		*/
		int64_t JoinTimestamp = 0;

		/**
		 * @brief 上次发言时间戳
		*/
		int64_t LastSpeakTimestamp = 0;

		/**
		 * @brief 剩余禁言时间
		*/
		int MuteTimeRemaining = 0;

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
			SpecialTitle = j["specialTitle"].get<string>();
			JoinTimestamp = j["joinTimestamp"].get<int64_t>();
			LastSpeakTimestamp = j["lastSpeakTimestamp"].get<int64_t>();
			MuteTimeRemaining = j["muteTimeRemaining"].get<int>();
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
			j["specialTitle"] = SpecialTitle;
			j["joinTimestamp"] = JoinTimestamp;
			j["lastSpeakTimestamp"] = LastSpeakTimestamp;
			j["muteTimeRemaining"] = MuteTimeRemaining;
			j["group"] = Group.ToJson();
			return j;
		}
	};
}

#endif // !mirai_cpp_defs_group_member_hpp_H_