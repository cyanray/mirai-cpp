#pragma once
#ifndef mirai_cpp_events_BotJoinGroupEvent_hpp_H_
#define mirai_cpp_events_BotJoinGroupEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "event_interface.hpp"
#include "mirai/defs/group.hpp"

namespace Cyan
{

	/**
	 * \brief bot 加入新群事件
	 */
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

#endif