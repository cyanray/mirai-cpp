#pragma once
#include "typedef.hpp"
#include "message_chain.hpp"
namespace Cyan
{

	class GroupMessage : public Serializable
	{
	public:
		MessageChain MessageChain;
		GroupMember_t Sender;

		GroupMessage() = default;
		GroupMessage(const GroupMessage& gm)
		{
			MessageChain = gm.MessageChain;
			Sender = gm.Sender;
		}
		GroupMessage& operator=(const GroupMessage& t)
		{
			GroupMessage tmp(t);
			std::swap(this->MessageChain, tmp.MessageChain);
			std::swap(this->Sender, tmp.Sender);
			return *this;
		}
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
		virtual string ToString() const override
		{
			return ToJson().dump();
		}
	};
} // namespace Cyan