#pragma once
#ifndef mirai_cpp_defs_messages_at_message_hpp_H_
#define mirai_cpp_defs_messages_at_message_hpp_H_
#include <utility>
#include "mirai/defs/IMessage.hpp"
#include "mirai/defs/QQType.hpp"

namespace Cyan
{
	class AtMessage : public IMessage
	{
	public:
		AtMessage() : target_(0), display_() {}
		AtMessage(const QQ_t& q) : target_(q), display_() {}
		AtMessage(const AtMessage& m) : target_(m.target_), display_(m.display_) {}
		AtMessage(AtMessage&& m) noexcept
		{
			std::swap(this->target_, m.target_);
			std::swap(this->display_, m.display_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const AtMessage*>(&m))
			{
				return m_ptr->target_ == this->target_;
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
			target_ = QQ_t(json["target"].get<int64_t>());
			if (!json["display"].is_null())
				display_ = json["display"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "target", (int64_t)(target_) },
				{ "display",display_ }
			};
		}
		virtual ~AtMessage() {}

		QQ_t Target() const { return target_; }
		void Target(const QQ_t& t) { this->target_ = t; }

		string Display() const { return display_; }

	private:
		const string type_ = "At";
		QQ_t target_;
		string display_;
	};

}
#endif

