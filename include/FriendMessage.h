#pragma once
#ifndef mirai_cpp__FriendMessage_h_H_
#define mirai_cpp__FriendMessage_h_H_

#include "typedef.hpp"
#include "message_chain.hpp"
namespace Cyan
{

	class FriendMessage : public Serializable
	{
	public:
		MessageChain MessageChain;
		Friend_t Sender;

		FriendMessage() = default;
		FriendMessage(const FriendMessage& gm)
		{
			MessageChain = gm.MessageChain;
			Sender = gm.Sender;
		}
		FriendMessage& operator=(const FriendMessage& t)
		{
			FriendMessage tmp(t);
			std::swap(this->MessageChain, tmp.MessageChain);
			std::swap(this->Sender, tmp.Sender);
			return *this;
		}
		virtual ~FriendMessage() = default;
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
		virtual string ToString() const override
		{
			return ToJson().dump();
		}
	};
} // namespace Cyan

#endif // !mirai_cpp__GroupMessage_h_H_