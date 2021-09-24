#pragma once
#ifndef mirai_cpp_events_BotLeaveEventKick_hpp_H_
#define mirai_cpp_events_BotLeaveEventKick_hpp_H_

#include <optional>
#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/group.hpp"
#include "mirai/defs/group_member.hpp"
#include "event_interface.hpp"
using std::optional;

namespace Cyan
{

	/**
	 * \brief bot 被踢出群
	 */
	class BotLeaveEventKick : public EventBase
	{
	public:
		Group_t Group;
		std::optional<GroupMember> Inviter;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::BotLeaveEventKick;
		}

		virtual bool Set(const json& j) override
		{
			this->Group.Set(j["group"]);
			if (!j["invitor"].is_null())
			{
				GroupMember tmp;
				tmp.Set(j["invitor"]);
				this->Inviter = tmp;
			}
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotLeaveEventKick";
			j["group"] = this->Group.ToJson();
			// Not a typo, MAH made a typo.
			j["invitor"] = (Inviter ? this->Inviter->ToJson() : json(nullptr));
			return j;
		}

	};

}

#endif