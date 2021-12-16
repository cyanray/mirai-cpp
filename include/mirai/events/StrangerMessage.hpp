#pragma once
#ifndef mirai_cpp_events_StrangerMessage_hpp_H_
#define mirai_cpp_events_StrangerMessage_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "mirai/defs/MessageChain.hpp"
#include "mirai/defs/Friend.hpp"
#include "EventBase.hpp"

namespace Cyan
{
	/**
	 * \brief 陌生人发来的消息
	 */
	class StrangerMessage : public EventBase
	{
	public:
		Cyan::MessageChain MessageChain;
		Friend_t Sender;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::StrangerMessage;
		}

		MessageId_t MessageId() const
		{
			return this->MessageChain.MessageId();
		}

		int64_t Timestamp() const
		{
			return this->MessageChain.Timestamp();
		}

		virtual bool Set(const json& j) override
		{
			this->MessageChain.Set(j["messageChain"]);
			this->Sender.Set(j["sender"]);
			return true;
		}

		virtual json ToJson() const override
		{
			json j = json::object();
			j["messageChain"] = this->MessageChain.ToJson();
			j["sender"] = this->Sender.ToJson();
			return j;
		}
	};

}

#endif