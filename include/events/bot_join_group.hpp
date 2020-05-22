#pragma once
#ifndef mirai_cpp_events_bot_join_group_hpp_H_
#define mirai_cpp_events_bot_join_group_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/group.hpp"
#include "event_interface.hpp"

namespace Cyan
{
	// bot 加入新群事件
	class BotJoinGroupEvent : public EventBase
	{
	public:
		Group_t Group;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::BotJoinGroupEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->Group.Set(j["group"]);
			return true;
		}
		
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotJoinGroupEvent";
			j["group"] = this->Group.ToJson();
			return j;
		}
	};

}

#endif // !mirai_cpp_events_bot_join_group_hpp_H_