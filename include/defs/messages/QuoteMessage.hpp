#pragma once
#ifndef mirai_cpp_defs_messages_quote_message_hpp_H_
#define mirai_cpp_defs_messages_quote_message_hpp_H_

#include <iostream>
#include "defs/message_interface.hpp"
#include "defs/qq_types.hpp"

namespace Cyan
{
	class QuoteMessage : public IMessage
	{
	public:
		QuoteMessage() :messageId_(0) {}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const QuoteMessage*>(&m))
			{
				return m_ptr->messageId_ == this->messageId_;
			}
			return false;
		}
		virtual bool operator!=(const IMessage& m) const override
		{
			return !(*this == m);
		}
		virtual bool Set(const json& json) override
		{
			if (json["type"].is_null() || json["type"].get<string>() != this->GetType())
				throw std::runtime_error("给定的json不正确");
			messageId_ = json["id"].get<int64_t>();
			std::cout << json.dump() << std::endl;
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "id", messageId_ }
			};
		}
		virtual ~QuoteMessage() {}

		MessageId_t MessageId() const { return messageId_; }
		void MessageId(MessageId_t mid) { this->messageId_ = mid; }

	private:
		string type_ = "Quote";
		MessageId_t messageId_;
	};

}
#endif

