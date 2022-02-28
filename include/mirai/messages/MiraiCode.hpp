#pragma once
#ifndef mirai_cpp_defs_messages_mirai_code_hpp_H_
#define mirai_cpp_defs_messages_mirai_code_hpp_H_

#include <string>
#include <string_view>
#include "mirai/defs/IMessage.hpp"
using std::string;
using std::string_view;

namespace Cyan
{
	class MiraiCode : public IMessage
	{
	public:
		MiraiCode() : code_() {}
		MiraiCode(std::string_view xml) : code_(xml) {}
		MiraiCode(const MiraiCode& m) : code_(m.code_) {}
		MiraiCode(MiraiCode&& m) noexcept
		{
			std::swap(this->code_, m.code_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const MiraiCode*>(&m))
			{
				return m_ptr->code_ == this->code_;
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
			code_ = json["code"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "code", code_ }
			};
		}
		virtual ~MiraiCode() {}

		const string& Code() const { return code_; }
		void Code(std::string_view code) { this->code_ = code; }

	private:
		const string type_ = "MiraiCode";
		string code_;
	};

}
#endif

