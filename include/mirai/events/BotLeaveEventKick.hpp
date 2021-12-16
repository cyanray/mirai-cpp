#pragma once
#ifndef mirai_cpp_events_BotLeaveEventKick_hpp_H_
#define mirai_cpp_events_BotLeaveEventKick_hpp_H_

#include <optional>
#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/Group.hpp"
#include "mirai/defs/GroupMember.hpp"
#include "EventBase.hpp"
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
		std::optional<GroupMember> Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::BotLeaveEventKick;
		}

		virtual bool Set(const json& j) override
		{
			this->Group.Set(j["group"]);
			if (!j["operator"].is_null())
			{
				GroupMember tmp;
				tmp.Set(j["operator"]);
				this->Operator = tmp;
			}
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotLeaveEventKick";
			j["group"] = this->Group.ToJson();
			j["operator"] = (Operator ? this->Operator->ToJson() : json(nullptr));
			return j;
		}

	};

}

#endif