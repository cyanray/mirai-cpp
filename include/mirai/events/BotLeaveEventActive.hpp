#pragma once
#ifndef mirai_cpp_events_BotLeaveEventActive_hpp_H_
#define mirai_cpp_events_BotLeaveEventActive_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/Group.hpp"
#include "EventBase.hpp"

namespace Cyan
{

	/**
	 * \brief bot 主动退群
	 */
	class BotLeaveEventActive : public EventBase
	{
	public:
		Group_t Group;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::BotLeaveEventActive;
		}

		virtual bool Set(const json& j) override
		{
			this->Group.Set(j["group"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotLeaveEventActive";
			j["group"] = this->Group.ToJson();
			return j;
		}
	};

}

#endif