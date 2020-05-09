#pragma once
#ifndef mirai_cpp_events_bot_leave_kick_hpp_H_
#define mirai_cpp_events_bot_leave_kick_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/group.hpp"
#include "event_interface.hpp"

namespace Cyan
{
	// bot 被踢出群
	class BotLeaveEventKick : public EventBase
	{
	public:
		Group_t Group;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::BotLeaveEventKick;
		}

		virtual void SetMiraiBot(MiraiBot* bot) override
		{
			this->bot_ = bot;
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

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_bot_leave_kick_hpp_H_