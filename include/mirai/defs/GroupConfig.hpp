#pragma once
#ifndef mirai_cpp_defs_group_config_hpp_H_
#define mirai_cpp_defs_group_config_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "ISerializable.hpp"

namespace Cyan
{
	/**
	 * @brief 群设置
	*/
	class GroupConfig : public ISerializable
	{
	public:
		/**
		 * @brief 群名称
		*/
		string Name;

		/**
		 * @brief 群公告
		*/
//		string Announcement;

		/**
		 * @brief 是否允许坦白说
		*/
		bool ConfessTalk = false;

		/**
		 * @brief 是否允许群成员邀请入群
		*/
		bool AllowMemberInvite = false;

		/**
		 * @brief 是否自动批准入群
		*/
		bool AutoApprove = false;

		/**
		 * @brief 是否允许匿名聊天
		*/
		bool AnonymousChat = false;

		virtual bool Set(const json& j) override
		{
			Name = j["name"].get<string>();
//			Announcement = j["announcement"].get<string>();
			ConfessTalk = j["confessTalk"].get<bool>();
			AllowMemberInvite = j["allowMemberInvite"].get<bool>();
			AutoApprove = j["autoApprove"].get<bool>();
			AnonymousChat = j["anonymousChat"].get<bool>();
			return true;
		}

		virtual json ToJson() const override
		{
			json j = json::object();
			j["name"] = Name;
//			j["announcement"] = Announcement;
			j["confessTalk"] = ConfessTalk;
			j["allowMemberInvite"] = AllowMemberInvite;
			j["autoApprove"] = AutoApprove;
			j["anonymousChat"] = AnonymousChat;
			return j;
		}
	};

}

#endif // !mirai_cpp_defs_group_config_hpp_H_