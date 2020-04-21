#pragma once
#ifndef mirai_cpp_events_friend_message_hpp_H_
#define mirai_cpp_events_friend_message_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/message_chain.hpp"
#include "defs/friend.hpp"
#include "exported.h"

namespace Cyan
{
	// 好友发来的消息
	class EXPORTED FriendMessage : public Serializable
	{
	public:
		Cyan::MessageChain MessageChain;
		Friend_t Sender;

		FriendMessage() = default;
		FriendMessage(const FriendMessage& gm)
		{
			MessageChain = gm.MessageChain;
			Sender = gm.Sender;
			bot_ = gm.bot_;
		}
		FriendMessage& operator=(const FriendMessage& t)
		{
			FriendMessage tmp(t);
			std::swap(this->MessageChain, tmp.MessageChain);
			std::swap(this->Sender, tmp.Sender);
			std::swap(this->bot_, tmp.bot_);
			return *this;
		}
		MessageId GetMessageId() const
		{
			return this->MessageChain.GetMessageId();
		}

		int64_t GetTimestamp() const
		{
			return this->MessageChain.GetTimestamp();
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		MessageId Reply(const Cyan::MessageChain& mc) const;
		MessageId QuoteReply(const Cyan::MessageChain& mc) const;

		virtual ~FriendMessage() = default;
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

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_friend_message_hpp_H_