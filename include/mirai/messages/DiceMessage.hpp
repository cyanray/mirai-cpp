#pragma once
#ifndef mirai_cpp_defs_messages_DiceMessage_hpp_H_
#define mirai_cpp_defs_messages_DiceMessage_hpp_H_
#include <utility>
#include "mirai/defs/IMessage.hpp"

namespace Cyan
{
	class DiceMessage : public IMessage
	{
	public:
		DiceMessage() : value_(1) {}
		DiceMessage(const int& value) : value_(value) {}
		DiceMessage(const DiceMessage& m) :value_(m.value_) {}
		DiceMessage(DiceMessage&& m) noexcept
		{
			std::swap(this->value_,m.value_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const DiceMessage*>(&m))
			{
				return m_ptr->value_ == this->value_;
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
			value_ = json["value"].get<int>();

			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "value", value_ }
			};
		}
		virtual ~DiceMessage() {}

		int Value() const { return value_; }
		void Value(int id) { this->value_ = id; }

	private:
		const string type_ = "Dice";
		int value_;
	};

}
#endif

