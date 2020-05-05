#pragma once
#ifndef mirai_cpp_events_bot_offline_active_hpp_H_
#define mirai_cpp_events_bot_offline_active_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"

namespace Cyan
{
	// bot 主动离线事件
	class BotOfflineEventActive : public Serializable
	{
	public:
		QQ_t QQ;

		BotOfflineEventActive() = default;
		BotOfflineEventActive(const BotOfflineEventActive& gm)
		{
			QQ = gm.QQ;
		}
		BotOfflineEventActive& operator=(const BotOfflineEventActive& t)
		{
			BotOfflineEventActive tmp(t);
			std::swap(this->QQ, tmp.QQ);
			return *this;
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		virtual ~BotOfflineEventActive() = default;
		virtual bool Set(const json& j) override
		{
			this->QQ = (QQ_t)(j["qq"].get<int64_t>());
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotOfflineEventActive";
			j["qq"] = (int64_t)this->QQ;
			return j;
		}

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_bot_offline_active_hpp_H_