#pragma once
#ifndef mirai_cpp_events_BotReloginEvent_hpp_H_
#define mirai_cpp_events_BotReloginEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "EventBase.hpp"

namespace Cyan
{
	/**
	 * \brief bot 主动重新登录事件
	 */
	class BotReloginEvent : public EventBase
	{
	public:
		QQ_t QQ;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::BotReloginEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->QQ = (QQ_t)(j["qq"].get<int64_t>());
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotReloginEvent";
			j["qq"] = (int64_t)this->QQ;
			return j;
		}
	};

}

#endif