#pragma once
#ifndef mirai_cpp_events_bot_offline_dropped_hpp_H_
#define mirai_cpp_events_bot_offline_dropped_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"

namespace Cyan
{
	// bot 因网络原因掉线
	class BotOfflineEventDropped : public Serializable
	{
	public:
		QQ_t QQ;

		BotOfflineEventDropped() = default;
		BotOfflineEventDropped(const BotOfflineEventDropped& gm)
		{
			QQ = gm.QQ;
		}
		BotOfflineEventDropped& operator=(const BotOfflineEventDropped& t)
		{
			BotOfflineEventDropped tmp(t);
			std::swap(this->QQ, tmp.QQ);
			return *this;
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		virtual ~BotOfflineEventDropped() = default;
		virtual bool Set(const json& j) override
		{
			this->QQ = (QQ_t)(j["qq"].get<int64_t>());
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotOfflineEventDropped";
			j["qq"] = (int64_t)this->QQ;
			return j;
		}

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_bot_offline_dropped_hpp_H_