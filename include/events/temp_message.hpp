#pragma once
#ifndef mirai_cpp_events_temp_message_hpp_H_
#define mirai_cpp_events_temp_message_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/message_chain.hpp"
#include "defs/group_member.hpp"
#include "exported.h"
namespace Cyan
{
	// 由群组发来的临时消息
	class EXPORTED TempMessage : public EventBase
	{
	public:
		Cyan::MessageChain MessageChain;
		GroupMember_t Sender;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::FriendRecallEvent;
		}

		virtual void SetMiraiBot(MiraiBot* bot) override
		{
			this->bot_ = bot;
		}

		MessageId GetMessageId() const
		{
			return (this->MessageChain).GetMessageId();
		}

		int64_t GetTimestamp() const
		{
			return (this->MessageChain).GetTimestamp();
		}

		MessageId Reply(const Cyan::MessageChain& mc) const;
		MessageId QuoteReply(const Cyan::MessageChain& mc) const;

		virtual bool Set(const json& j) override
		{
			this->MessageChain.Set(j["messageChain"]);
			Sender.Set(j["sender"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["messageChain"] = this->MessageChain.ToJson();
			j["sender"] = this->Sender.ToJson();
			return j;
		}
	private:
		MiraiBot* bot_;
	};



}

#endif // !mirai_cpp_events_temp_message_hpp_H_