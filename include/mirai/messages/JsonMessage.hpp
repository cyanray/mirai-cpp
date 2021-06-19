#pragma once
#ifndef mirai_cpp_defs_messages_json_message_hpp_H_
#define mirai_cpp_defs_messages_json_message_hpp_H_

#include "mirai/defs/message_interface.hpp"

namespace Cyan
{
	class JsonMessage : public IMessage
	{
	public:
		JsonMessage() : json_() {}
		JsonMessage(const string& json) : json_(json) {}
		JsonMessage(const JsonMessage& m) : json_(m.json_) {}
		JsonMessage(JsonMessage&& m) noexcept
		{
			std::swap(this->json_, m.json_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const JsonMessage*>(&m))
			{
				return m_ptr->json_ == this->json_;
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
			json_ = json["json"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "json", json_ }
			};
		}
		virtual ~JsonMessage() {}

		const string& Json() const { return json_; }
		void Json(const string& json) { this->json_ = json; }

	private:
		const string type_ = "Plain";
		string json_;
	};

}
#endif

