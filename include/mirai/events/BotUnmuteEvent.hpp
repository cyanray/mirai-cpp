#pragma once
#ifndef mirai_cpp_events_BotUnmuteEvent_hpp_H_
#define mirai_cpp_events_BotUnmuteEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/GroupMember.hpp"
#include "EventBase.hpp"

namespace Cyan
{
	/**
	 * \brief bot 被解除禁言事件
	 */
	class BotUnmuteEvent : public EventBase
	{
	public:
		GroupMember Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::BotUnmuteEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->Operator.Set(j["operator"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotUnmuteEvent";
			j["operator"] = this->Operator.ToJson();
			return j;
		}

	};

}

#endif