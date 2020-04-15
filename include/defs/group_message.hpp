#pragma once
#ifndef mirai_cpp_defs_group_message_hpp_H_
#define mirai_cpp_defs_group_message_hpp_H_

#include <nlohmann/json.hpp>
#include "qq_types.hpp"
#include "serializable.hpp"
#include "message_chain.hpp"
#include "group_member.hpp"

namespace Cyan
{
	// 群组发来的消息
	class GroupMessage : public Serializable
	{
	public:
		Cyan::MessageChain MessageChain;
		GroupMember_t Sender;

		GroupMessage() = default;
		GroupMessage(const GroupMessage& gm)
		{
			MessageChain = gm.MessageChain;
			Sender = gm.Sender;
			bot_ = gm.bot_;
		}
		GroupMessage& operator=(const GroupMessage& t)
		{
			GroupMessage tmp(t);
			std::swap(this->MessageChain, tmp.MessageChain);
			std::swap(this->Sender, tmp.Sender);
			std::swap(this->bot_, tmp.bot_);
			return *this;
		}
		MessageId GetMessageId() const
		{
			return (this->MessageChain).GetMessageId();
		}

		int64_t GetTimestamp() const
		{
			return (this->MessageChain).GetTimestamp();
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		MessageId Reply(const Cyan::MessageChain& mc) const;
		MessageId QuoteReply(const Cyan::MessageChain& mc) const;

		virtual ~GroupMessage() = default;
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

#endif // !mirai_cpp_defs_group_message_hpp_H_