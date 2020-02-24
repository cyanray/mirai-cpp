#pragma once
#ifndef mirai_cpp__message_chain_hpp_H_
#define mirai_cpp__message_chain_hpp_H_
#include <sstream>
#include <string>
#include <exception>
#include <iostream>
#include "serializable.hpp"
#include "typedef.hpp"
#include <nlohmann/json.hpp>
using std::string;
using std::stringstream;
using std::runtime_error;
namespace Cyan
{
	class MessageChain : public Serializable
	{
	public:
		MessageChain() :messages_(json::array()) {}
		MessageChain(const MessageChain& mc)
		{
			messages_ = mc.messages_;
		}
		MessageChain(MessageChain&& mc)
		{
			std::swap(messages_, mc.messages_);
		}
		MessageChain& operator=(const MessageChain& mc)
		{
			MessageChain tmp(mc);
			std::swap(messages_, tmp.messages_);
			return *this;
		}
		MessageChain& operator=(MessageChain&& mc)
		{
			std::swap(messages_, mc.messages_);
			return *this;
		}
		MessageChain& operator+(const MessageChain& mc)
		{
			messages_.insert(messages_.end(), mc.messages_.begin(), mc.messages_.end());
			return *this;
		}
		virtual ~MessageChain() = default;
		MessageChain& At(const QQ_t qq)
		{
			json j;
			j["type"] = "At";
			j["target"] = qq;
			j["display"] = "@@";
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

		virtual bool Set(const json& j) override
		{
			messages_ = j;
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
	};


} // namespace Cyan

#endif // !mirai_cpp__message_chain_hpp_H_
