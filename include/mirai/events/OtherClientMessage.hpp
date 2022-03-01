#pragma once
#ifndef mirai_cpp_events_OtherClientMessage_hpp_H_
#define mirai_cpp_events_OtherClientMessage_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "mirai/defs/MessageChain.hpp"
#include "EventBase.hpp"

namespace Cyan
{
	/**
	 * \brief 其他客户端消息
	 */
	class OtherClientMessage : public EventBase
	{
	public:
		Cyan::MessageChain MessageChain;
		class Sender : public ISerializable
		{
		public:
			int Id = -1;
			string Platform;

			virtual bool Set(const json& j) override
			{
				Id = j["id"].get<int>();
				Platform = j["platform"].get<string>();
				return true;
			}

			virtual json ToJson() const override
			{
				return
				{
					{ "id", Id },
					{ "platform", Platform }
				};
			}
		} Sender;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::OtherClientMessage;
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