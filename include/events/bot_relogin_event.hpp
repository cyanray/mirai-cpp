#pragma once
#ifndef mirai_cpp_events_bot_relogin_event_hpp_H_
#define mirai_cpp_events_bot_relogin_event_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"

namespace Cyan
{
	// bot 主动重新登录事件
	class BotReloginEvent : public Serializable
	{
	public:
		QQ_t QQ;

		BotReloginEvent() = default;
		BotReloginEvent(const BotReloginEvent& gm)
		{
			QQ = gm.QQ;
		}
		BotReloginEvent& operator=(const BotReloginEvent& t)
		{
			BotReloginEvent tmp(t);
			std::swap(this->QQ, tmp.QQ);
			return *this;
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		virtual ~BotReloginEvent() = default;
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

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_bot_relogin_event_hpp_H_