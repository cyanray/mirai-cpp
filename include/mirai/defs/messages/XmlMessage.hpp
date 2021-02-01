#pragma once
#ifndef mirai_cpp_defs_messages_xml_message_hpp_H_
#define mirai_cpp_defs_messages_xml_message_hpp_H_

#include "mirai/defs/message_interface.hpp"

namespace Cyan
{
	class XmlMessage : public IMessage
	{
	public:
		XmlMessage() : xml_() {}
		XmlMessage(const string& xml) : xml_(xml) {}
		template<int N>
		XmlMessage(const char(&xml)[N]) : xml_(xml, N) {}
		XmlMessage(const XmlMessage& m) : xml_(m.xml_) {}
		XmlMessage(XmlMessage&& m) noexcept
		{
			std::swap(this->xml_, m.xml_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const XmlMessage*>(&m))
			{
				return m_ptr->xml_ == this->xml_;
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
			xml_ = json["xml"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "xml", xml_ }
			};
		}
		virtual ~XmlMessage() {}

		const string& Xml() const { return xml_; }
		void Xml(const string& xml) { this->xml_ = xml; }

	private:
		const string type_ = "Xml";
		string xml_;
	};

}
#endif

