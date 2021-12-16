#pragma once
#ifndef mirai_cpp_defs_messages_face_message_hpp_H_
#define mirai_cpp_defs_messages_face_message_hpp_H_
#include <utility>
#include "mirai/defs/IMessage.hpp"

namespace Cyan
{
	class FaceMessage : public IMessage
	{
	public:
		FaceMessage() : faceId_(0), name_() {}
		FaceMessage(const int& id) : faceId_(id), name_() {}
		FaceMessage(const string& name) : faceId_(0), name_(name) {}
		template<int N>
		FaceMessage(const char(&name)[N]) : faceId_(0), name_(name,N) {}
		FaceMessage(const FaceMessage& m) :faceId_(m.faceId_), name_(m.name_) {}
		FaceMessage(FaceMessage&& m) noexcept
		{
			std::swap(this->faceId_,m.faceId_);
			std::swap(this->name_, m.name_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const FaceMessage*>(&m))
			{
				return m_ptr->faceId_ == this->faceId_;
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
			faceId_ = json["faceId"].get<int>();
			name_ = json["name"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "faceId", faceId_ },
				{ "name",name_ }
			};
		}
		virtual ~FaceMessage() {}

		int FaceId() const { return faceId_; }
		void FaceId(int id) { this->faceId_ = id; }

		string Name() const { return name_; }
		void Name(const string& n) { this->name_ = n; }

	private:
		const string type_ = "Face";
		int faceId_;
		string name_;
	};

}
#endif

