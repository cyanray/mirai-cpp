#pragma once
#ifndef mirai_cpp_events_Message_hpp_H_
#define mirai_cpp_events_Message_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "FriendMessage.hpp"
#include "GroupMessage.hpp"
#include "TempMessage.hpp"
#include "EventBase.hpp"
#include "mirai/exported.h"

namespace Cyan
{

	enum class MessageType
	{
		FriendMessage,		// 好友消息
		GroupMessage,		// 群组消息
		TempMessage			// 临时消息
	};

	/**
	 * \brief 通用消息事件 (可转换为 FriendMessage、GroupMessage、TempMessage)
	 */
	class EXPORTED Message : public EventBase
	{
	public:
		QQ_t Sender;
		Cyan::MessageChain MessageChain;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::Message;
		}

		MessageType GetMessageType() const
		{
			return messageType_;
		}

		FriendMessage ToFriendMessage() const
		{
			if (messageType_ != MessageType::FriendMessage)
				throw std::runtime_error("错误的消息类型转换");
			return friendMessage_;
		}

		GroupMessage ToGroupMessage() const
		{
			if (messageType_ != MessageType::GroupMessage)
				throw std::runtime_error("错误的消息类型转换");
			return groupMessage_;
		}

		TempMessage ToTempMessage() const
		{
			if (messageType_ != MessageType::TempMessage)
				throw std::runtime_error("错误的消息类型转换");
			return tempMessage_;
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

		virtual bool Set(const json& j) override
		{
			if (j["type"] == "FriendMessage")
			{
				messageType_ = MessageType::FriendMessage;
				friendMessage_.Set(j);
				friendMessage_.SetMiraiBot(bot_);
			}
			if (j["type"] == "GroupMessage")
			{
				messageType_ = MessageType::GroupMessage;
				groupMessage_.Set(j);
				groupMessage_.SetMiraiBot(bot_);
			}
			if (j["type"] == "TempMessage")
			{
				messageType_ = MessageType::TempMessage;
				tempMessage_.Set(j);
				tempMessage_.SetMiraiBot(bot_);
			}
			this->Sender = QQ_t(j["sender"]["id"].get<int64_t>());
			this->MessageChain.Set(j["messageChain"]);
			json_ = j;
			return true;
		}
		virtual json ToJson() const override
		{
			return json_;
		}
	private:
		json json_;
		MessageType messageType_;
		FriendMessage friendMessage_;
		GroupMessage groupMessage_;
		TempMessage tempMessage_;
	};
}

#endif