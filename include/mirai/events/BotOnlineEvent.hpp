#pragma once
#ifndef mirai_cpp_events_BotOnlineEvent_hpp_H_
#define mirai_cpp_events_BotOnlineEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "EventBase.hpp"

namespace Cyan
{
	/**
	 * \brief bot 登录成功事件
	 */
	class BotOnlineEvent : public EventBase
	{
	public:
		QQ_t QQ;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::BotOnlineEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->QQ = (QQ_t)(j["qq"].get<int64_t>());
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotOnlineEvent";
			j["qq"] = (int64_t)this->QQ;
			return j;
		}

	};

}

#endif