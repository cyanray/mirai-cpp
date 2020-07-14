#pragma once
#ifndef mirai_cpp_defs_messages_plain_message_hpp_H_
#define mirai_cpp_defs_messages_plain_message_hpp_H_

#include "defs/message_interface.hpp"

namespace Cyan
{
	class PlainMessage : public IMessage
	{
	public:
		PlainMessage() :text() {}
		virtual bool Set(const json& json) override
		{
			if (json["type"].get<string>() != this->GetType())
				throw std::runtime_error("给定的json不正确");
			text = json["text"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", GetType() },
				{ "text", text }
			};
		}
		virtual const string& GetType() const override
		{
			return "Plain";
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const PlainMessage*>(&m))
			{
				return m_ptr->text == this->text;
			}
		}
		virtual bool operator!=(const IMessage& m) const override
		{
			return !(*this == m);
		}
		virtual ~PlainMessage() {}

		const string& GetText() const { return text; }
		void SetText(const string& text) { this->text = text; }

	private:
		string text;
	};

}
#endif

