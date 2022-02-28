#pragma once
#ifndef mirai_cpp_defs_messages_file_message_hpp_H_
#define mirai_cpp_defs_messages_file_message_hpp_H_

#include <string>
#include <string_view>
#include "mirai/defs/IMessage.hpp"
#include "mirai/defs/QQType.hpp"
using std::string;
using std::string_view;


namespace Cyan
{
	class FileMessage : public IMessage
	{
	public:
		FileMessage() : id_(), name_(), size_(0) {}
		FileMessage(const FileMessage& m) : 
			id_(m.id_), 
			name_(m.name_), 
			size_(m.size_) {}
		FileMessage(const MiraiFile& m) :
			id_(m.Id),
			name_(m.FileName),
			size_(m.FileSize) {}
		FileMessage(FileMessage&& m) noexcept
		{
			std::swap(this->id_, m.id_);
			std::swap(this->name_, m.name_);
			std::swap(this->size_, m.size_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const FileMessage*>(&m))
			{
				return m_ptr->id_ == this->id_;
			}
			return false;
		}

		virtual bool Set(const json& json) override
		{
			if (json["type"].is_null() || json["type"].get<string>() != this->GetType())
				throw std::runtime_error("给定的json不正确");

			id_ = json["id"].get<string>();
			name_ = json["name"].get<string>();
			size_ = json["size"].get<size_t>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "id", id_ },
				{ "name", name_ },
				{ "size", size_  }
			};
		}

		const string& Id() const { return id_; }
		void Id(std::string_view id) { id_ = id; }

		const string& Name() const { return name_; }
		void Name(std::string_view name) { name_ = name; }

		size_t FileSize() const { return size_; }
		void FileSize(size_t size) { size_ = size; }

	private:
		const string type_ = "File";
		string id_;
		string name_;
		size_t size_;
	};

}
#endif

