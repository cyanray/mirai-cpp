#pragma once
#ifndef mirai_cpp_defs_messages_app_message_hpp_H_
#define mirai_cpp_defs_messages_app_message_hpp_H_

#include <string>
#include <string_view>
#include "mirai/defs/IMessage.hpp"
using std::string;
using std::string_view;

namespace Cyan
{
	class AppMessage : public IMessage
	{
	public:
		AppMessage() : content_() {}
		AppMessage(std::string_view content) : content_(content) {}
		AppMessage(const AppMessage& m) : content_(m.content_) {}
		AppMessage(AppMessage&& m) noexcept
		{
			std::swap(this->content_, m.content_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const AppMessage*>(&m))
			{
				return m_ptr->content_ == this->content_;
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
			content_ = json["content"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "content", content_ }
			};
		}
		virtual ~AppMessage() {}

		const string& Content() const { return content_; }
		void Content(std::string_view content) { this->content_ = content; }

	private:
		const string type_ = "App";
		string content_;
	};

}
#endif

