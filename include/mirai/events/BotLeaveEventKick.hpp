#pragma once
#ifndef mirai_cpp_events_BotLeaveEventKick_hpp_H_
#define mirai_cpp_events_BotLeaveEventKick_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/group.hpp"
#include "event_interface.hpp"

namespace Cyan
{

	/**
	 * \brief bot 被踢出群
	 */
	class BotLeaveEventKick : public EventBase
	{
	public:
		Group_t Group;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::BotLeaveEventKick;
		}

		virtual bool Set(const json& j) override
		{
			this->Group.Set(j["group"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotLeaveEventKick";
			j["group"] = this->Group.ToJson();
			return j;
		}

	};

}

#endif