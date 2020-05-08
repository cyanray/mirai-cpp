#pragma once
#ifndef mirai_cpp_events_bot_offline_force_hpp_H_
#define mirai_cpp_events_bot_offline_force_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"

namespace Cyan
{
	// bot 被挤下线事件
	class BotOfflineEventForce : public EventBase
	{
	public:
		QQ_t QQ;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::BotOfflineEventForce;
		}

		virtual void SetMiraiBot(MiraiBot* bot) override
		{
			this->bot_ = bot;
		}

		virtual bool Set(const json& j) override
		{
			this->QQ = (QQ_t)(j["qq"].get<int64_t>());
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotOfflineEventForce";
			j["qq"] = (int64_t)this->QQ;
			return j;
		}

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_bot_offline_force_hpp_H_