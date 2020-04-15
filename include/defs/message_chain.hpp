#pragma once
#ifndef mirai_cpp_defs_message_chain_hpp_H_
#define mirai_cpp_defs_message_chain_hpp_H_

#include <nlohmann/json.hpp>
#include <sstream>
#include "qq_types.hpp"
#include "serializable.hpp"

namespace Cyan
{
	class MessageChain : public Serializable
	{
	public:
		friend MessageChain& operator+(const string& str, MessageChain& mc);
		MessageChain() :messages_(json::array()) {}
		MessageChain(const MessageChain& mc)
		{
			messages_ = mc.messages_;
			messageId_ = mc.messageId_;
			timestamp_ = mc.timestamp_;
		}
		MessageChain(MessageChain&& mc) noexcept
		{
			std::swap(messages_, mc.messages_);
			std::swap(messageId_, mc.messageId_);
			std::swap(timestamp_, mc.timestamp_);
		}
		MessageChain& operator=(const MessageChain& mc)
		{
			MessageChain tmp(mc);
			std::swap(messages_, tmp.messages_);
			std::swap(messageId_, tmp.messageId_);
			std::swap(timestamp_, tmp.timestamp_);
			return *this;
		}
		MessageChain& operator=(MessageChain&& mc) noexcept
		{
			std::swap(messages_, mc.messages_);
			std::swap(messageId_, mc.messageId_);
			std::swap(timestamp_, mc.timestamp_);
			return *this;
		}
		MessageChain& operator+(const MessageChain& mc)
		{
			messages_.insert(messages_.end(), mc.messages_.begin(), mc.messages_.end());
			return *this;
		}
		MessageChain& operator+(const string& val)
		{
			return this->Plain(val);
		}
		MessageChain& operator+=(const string& val)
		{
			return this->Plain(val);
		}
		virtual ~MessageChain() = default;
		MessageChain& At(const QQ_t qq)
		{
			json j;
			j["type"] = "At";
			j["target"] = int64_t(qq);
			messages_.push_back(j);
			return *this;
		}
		MessageChain& AtAll()
		{
			messages_.push_back({ {"type","AtAll"} });
			return *this;
		}
		MessageChain& Face(int faceID)
		{
			json j;
			j["type"] = "Face";
			j["faceId"] = faceID;
			messages_.push_back(j);
			return *this;
		}
		MessageChain& Plain(const string& plainText)
		{
			json j;
			j["type"] = "Plain";
			j["text"] = plainText;
			messages_.push_back(j);
			return *this;
		}
		template<typename T>
		MessageChain& Plain(const T& val)
		{
			std::stringstream ss;
			ss << val;
			this->Plain(ss.str());
			return *this;
		}
		MessageChain& Image(const FriendImage& Image)
		{
			json j;
			j["type"] = "Image";
			j["imageId"] = Image.ID;
			messages_.push_back(j);
			return *this;
		}
		MessageChain& Image(const GroupImage& Image)
		{
			json j;
			j["type"] = "Image";
			j["imageId"] = Image.ID;
			messages_.push_back(j);
			return *this;
		}
		MessageChain& Image(const TempImage& Image)
		{
			json j;
			j["type"] = "Image";
			j["imageId"] = Image.ID;
			messages_.push_back(j);
			return *this;
		}

		string GetPlainText() const
		{
			using std::stringstream;
			stringstream ss;
			for (const auto& ele : messages_)
			{
				if (ele["type"].get<string>() == "Plain")
				{
					ss << ele["text"].get<string>();
				}
			}
			return ss.str();
		}
		string GetPlainTextFirst() const
		{
			for (const auto& ele : messages_)
			{
				if (ele["type"].get<string>() == "Plain")
				{
					return ele["text"].get<string>();
				}
			}
			return string();
		}

		MessageId GetMessageId() const
		{
			return messageId_;
		}

		int64_t GetTimestamp() const
		{
			return timestamp_;
		}

		virtual bool Set(const json& j) override
		{
			messages_ = j;
			if (j.size() >= 1)
			{
				try
				{
					if (j[0]["type"].get<string>() == "Source")
					{
						this->messageId_ = j[0]["id"].get<int64_t>();
						this->timestamp_ = j[0]["time"].get<int64_t>();
					}
				}
				catch (const std::exception&)
				{
					// do nothing...
				}
			}
			return true;
		}
		virtual json ToJson() const override
		{
			return messages_;
		}
		virtual string ToString() const override
		{
			return messages_.dump();
		}

	private:
		json messages_;
		int64_t timestamp_;
		MessageId messageId_;
	};

	inline MessageChain& operator+(const string& str, MessageChain& mc)
	{
		MessageChain tmp;
		tmp.Plain(str);
		mc.messages_.insert(mc.messages_.begin(), tmp.messages_.begin(), tmp.messages_.end());
		return mc;
	}
}


#endif // !mirai_cpp_defs_message_chain_hpp_H_