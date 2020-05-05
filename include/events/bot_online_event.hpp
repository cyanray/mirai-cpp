#pragma once
#ifndef mirai_cpp_events_bot_online_event_hpp_H_
#define mirai_cpp_events_bot_online_event_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"

namespace Cyan
{
	// bot 登录成功事件
	class BotOnlineEvent : public Serializable
	{
	public:
		QQ_t QQ;

		BotOnlineEvent() = default;
		BotOnlineEvent(const BotOnlineEvent& gm)
		{
			QQ = gm.QQ;
		}
		BotOnlineEvent& operator=(const BotOnlineEvent& t)
		{
			BotOnlineEvent tmp(t);
			std::swap(this->QQ, tmp.QQ);
			return *this;
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		virtual ~BotOnlineEvent() = default;
		virtual bool Set(const json& j) override
		{
			this->QQ = (QQ_t)(j["qq"].get<int64_t>());
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotOnlineEvent";
			j["qq"] = (int64_t)this->DurationSeconds;
			return j;
		}

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_bot_online_event_hpp_H_