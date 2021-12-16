#pragma once
#ifndef mirai_cpp_defs_messages_plain_message_hpp_H_
#define mirai_cpp_defs_messages_plain_message_hpp_H_

#include <string>
#include <string_view>
#include "mirai/defs/IMessage.hpp"
using std::string;
using std::string_view;

namespace Cyan
{
	class PlainMessage : public IMessage
	{
	public:
		PlainMessage() : text_() {}
		PlainMessage(std::string_view text) : text_(text) {}
		PlainMessage(const PlainMessage& m) :text_(m.text_) {}
		PlainMessage(PlainMessage&& m) noexcept
		{
			std::swap(this->text_, m.text_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const PlainMessage*>(&m))
			{
				return m_ptr->text_ == this->text_;
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
			text_ = json["text"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "text", text_ }
			};
		}
		virtual ~PlainMessage() {}

		const string& Text() const { return text_; }
		void Text(std::string_view text) { this->text_ = text; }

	private:
		const string type_ = "Plain";
		string text_;
	};

}
#endif

