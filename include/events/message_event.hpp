#pragma once
#ifndef mirai_cpp_events_message_hpp_H_
#define mirai_cpp_events_message_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "friend_message.hpp"
#include "group_message.hpp"
#include "temp_message.hpp"
#include "event_interface.hpp"
#include "exported.h"

namespace Cyan
{

	enum class MessageType
	{
		FriendMessage,		// 好友消息
		GroupMessage,		// 群组消息
		TempMessage			// 临时消息
	};

	// 通用消息事件 (可转换为 FriendMessage GroupMessage TempMessage)
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

		MessageId GetMessageId() const
		{
			return (this->MessageChain).GetMessageId();
		}

		int64_t GetTimestamp() const
		{
			return (this->MessageChain).GetTimestamp();
		}

		virtual void SetMiraiBot(MiraiBot* bot) override
		{
			this->bot_ = bot;
		}

		MessageId Reply(const Cyan::MessageChain& mc) const;
		MessageId QuoteReply(const Cyan::MessageChain& mc) const;

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
		MiraiBot* bot_;
		json json_;
		MessageType messageType_;
		FriendMessage friendMessage_;
		GroupMessage groupMessage_;
		TempMessage tempMessage_;
	};
}

#endif // !mirai_cpp_events_message_hpp_H_