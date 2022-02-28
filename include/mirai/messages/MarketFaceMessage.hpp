#pragma once
#ifndef mirai_cpp_defs_messages_marketface_message_hpp_H_
#define mirai_cpp_defs_messages_marketface_message_hpp_H_
#include <utility>
#include "mirai/defs/IMessage.hpp"

namespace Cyan
{
	class MarketFaceMessage : public IMessage
	{
	public:
		MarketFaceMessage() : id_(0), name_() {}
		MarketFaceMessage(const int& id) : id_(id), name_() {}
		MarketFaceMessage(const string& name) : id_(0), name_(name) {}
		template<int N>
		MarketFaceMessage(const char(&name)[N]) : id_(0), name_(name,N) {}
		MarketFaceMessage(const MarketFaceMessage& m) :id_(m.id_), name_(m.name_) {}
		MarketFaceMessage(MarketFaceMessage&& m) noexcept
		{
			std::swap(this->id_,m.id_);
			std::swap(this->name_, m.name_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const MarketFaceMessage*>(&m))
			{
				return m_ptr->id_ == this->id_;
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
			id_ = json["id"].get<int>();
			name_ = json["name"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "id", id_ },
				{ "name",name_ }
			};
		}
		virtual ~MarketFaceMessage() {}

		int FaceId() const { return id_; }
		void FaceId(int id) { this->id_ = id; }

		string Name() const { return name_; }
		void Name(const string& n) { this->name_ = n; }

	private:
		const string type_ = "MarketFace";
		int id_;
		string name_;
	};

}
#endif

