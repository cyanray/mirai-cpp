#pragma once
#ifndef mirai_cpp_events_GroupMessage_hpp_H_
#define mirai_cpp_events_GroupMessage_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "mirai/defs/MessageChain.hpp"
#include "mirai/defs/GroupMember.hpp"
#include "EventBase.hpp"
#include "mirai/exported.h"

namespace Cyan
{
	/**
	 * \brief 群组发来的消息
	 */
	class EXPORTED GroupMessage : public EventBase
	{
	public:
		Cyan::MessageChain MessageChain;
		GroupMember Sender;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::GroupMessage;
		}

		MessageId_t MessageId() const
		{
			return (this->MessageChain).MessageId();
		}

		int64_t Timestamp() const
		{
			return (this->MessageChain).Timestamp();
		}

		MessageId_t Reply(const Cyan::MessageChain& mc) const;
		MessageId_t QuoteReply(const Cyan::MessageChain& mc) const;
		void Recall() const;

		/**
		 * @brief 检查是否有\@机器人.
		 * @return 如果有那么返回 true, 反之返回 false.
		*/
		bool AtMe() const;

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
	};
}

#endif