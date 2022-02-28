#pragma once
#ifndef mirai_cpp_events_GroupSyncMessage_hpp_H_
#define mirai_cpp_events_GroupSyncMessage_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "mirai/defs/MessageChain.hpp"
#include "mirai/defs/Group.hpp"
#include "EventBase.hpp"

namespace Cyan
{
	/**
	 * \brief Bot在其他客户端发送给群组的消息
	 */
	class GroupSyncMessage : public EventBase
	{
	public:
		Cyan::MessageChain MessageChain;
		Group_t Subject;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::GroupSyncMessage;
		}

		MessageId_t MessageId() const
		{
			return (this->MessageChain).MessageId();
		}

		int64_t Timestamp() const
		{
			return (this->MessageChain).Timestamp();
		}

		virtual bool Set(const json& j) override
		{
			this->MessageChain.Set(j["messageChain"]);
			Subject.Set(j["subject"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["messageChain"] = this->MessageChain.ToJson();
			j["subject"] = this->Subject.ToJson();
			return j;
		}
	};
}

#endif