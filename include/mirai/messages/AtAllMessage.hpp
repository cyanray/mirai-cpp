#pragma once
#ifndef mirai_cpp_defs_messages_atall_message_hpp_H_
#define mirai_cpp_defs_messages_atall_message_hpp_H_

#include "mirai/defs/IMessage.hpp"

namespace Cyan
{
	class AtAllMessage : public IMessage
	{
	public:
		AtAllMessage() {}
		AtAllMessage(const AtAllMessage&) {}
		AtAllMessage(AtAllMessage&&) noexcept {}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const AtAllMessage*>(&m))
			{
				return true;
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
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ }
			};
		}
		virtual ~AtAllMessage() {}

	private:
		const string type_ = "AtAll";
	};

}
#endif

