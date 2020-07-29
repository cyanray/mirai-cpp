#pragma once
#ifndef mirai_cpp_defs_message_chain_hpp_H_
#define mirai_cpp_defs_message_chain_hpp_H_

#include <nlohmann/json.hpp>
#include <sstream>
#include <vector>
#include <memory>
#include "qq_types.hpp"
#include "serializable.hpp"
#include "defs/message_interface.hpp"
#include "defs/messages/PlainMessage.hpp"
#include "defs/simple_reflect.hpp"
using std::vector;

namespace Cyan
{

	enum class Poke
	{
		Poke,
		ShowLove,
		Like,
		Heartbroken,
		SixSixSix,
		FangDaZhao
	};

	class MessageChain : public ISerializable
	{
	public:
		//friend MessageChain& operator+(const string& str, MessageChain& mc);
		//template<int N>
		//friend MessageChain& operator+(const char(&str)[N], MessageChain& mc);
		MessageChain() :messages_(),messageId_(0),timestamp_(0) 
		{
			if (factory_.size() <= 0)
			{
				factory_.Register<PlainMessage>("Plain");
			}
		}
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
		//MessageChain& operator+(const MessageChain& mc)
		//{
		//	messages_.insert(messages_.end(), mc.messages_.begin(), mc.messages_.end());
		//	return *this;
		//}
		//MessageChain& operator+(const string& val)
		//{
		//	return this->Plain(val);
		//}
		//MessageChain& operator+=(const string& val)
		//{
		//	return this->Plain(val);
		//}
		//bool operator==(const MessageChain& mc) const
		//{
		//	auto it1 = mc.messages_.begin();
		//	auto it2 = messages_.begin();

		//	for (; it1 != mc.messages_.end() && it2 != messages_.end(); ++it1, ++it2)
		//	{
		//		if (it1.value()["type"] != it2.value()["type"]) return false;
		//		if (it1.value()["type"] == "At")
		//		{
		//			if (it1.value()["target"] != it2.value()["target"]) return false;
		//		}
		//		if (it1.value()["type"] == "Face")
		//		{
		//			if (it1.value()["faceId"] != it2.value()["faceId"]) return false;
		//		}
		//		if (it1.value()["type"] == "Plain")
		//		{
		//			if (it1.value()["text"] != it2.value()["text"]) return false;
		//		}
		//		if (it1.value()["type"] == "Image")
		//		{
		//			if (it1.value()["imageId"] != it2.value()["imageId"]) return false;
		//		}
		//		if (it1.value()["type"] == "FlashImage")
		//		{
		//			if (it1.value()["imageId"] != it2.value()["imageId"]) return false;
		//		}
		//		if (it1.value()["type"] == "Poke")
		//		{
		//			if (it1.value()["name"] != it2.value()["name"]) return false;
		//		}
		//	}
		//	return true;
		//}
		//bool operator!=(const MessageChain& mc) const
		//{
		//	return !this->operator==(mc);
		//}
		virtual ~MessageChain() = default;
		//MessageChain& At(const QQ_t qq)
		//{
		//	json j;
		//	j["type"] = "At";
		//	j["target"] = int64_t(qq);
		//	messages_.push_back(j);
		//	return *this;
		//}
		//MessageChain& AtAll()
		//{
		//	messages_.push_back({ {"type","AtAll"} });
		//	return *this;
		//}
		//MessageChain& Face(int faceID)
		//{
		//	json j;
		//	j["type"] = "Face";
		//	j["faceId"] = faceID;
		//	messages_.push_back(j);
		//	return *this;
		//}
		//MessageChain& Face(const string& name)
		//{
		//	json j;
		//	j["type"] = "Face";
		//	j["name"] = name;
		//	messages_.push_back(j);
		//	return *this;
		//}
		MessageChain& Plain(const string& plainText)
		{
			auto plain_ptr = std::make_shared<PlainMessage>();
			plain_ptr->SetText(plainText);
			this->messages_.emplace_back(plain_ptr);
			return *this;
		}
		//template<typename T>
		//MessageChain& Plain(const T& val)
		//{
		//	std::stringstream ss;
		//	ss << val;
		//	this->Plain(ss.str());
		//	return *this;
		//}
		//MessageChain& Image(const string& url)
		//{
		//	json j;
		//	j["type"] = "Image";
		//	j["url"] = url;
		//	messages_.push_back(j);
		//	return *this;
		//}
		//MessageChain& Image(const MiraiImage& Image)
		//{
		//	json j;
		//	j["type"] = "Image";
		//	j["imageId"] = Image.ID;
		//	messages_.push_back(j);
		//	return *this;
		//}
		//MessageChain& FlashImage(const MiraiImage& Image)
		//{
		//	json j;
		//	j["type"] = "FlashImage";
		//	j["imageId"] = Image.ID;
		//	messages_.push_back(j);
		//	return *this;
		//}
		//MessageChain& FlashImage(const string& url)
		//{
		//	json j;
		//	j["type"] = "FlashImage";
		//	j["url"] = url;
		//	messages_.push_back(j);
		//	return *this;
		//}
		//MessageChain& Xml(const string& xml_str)
		//{
		//	json j;
		//	j["type"] = "Xml";
		//	j["xml"] = xml_str;
		//	messages_.push_back(j);
		//	return *this;
		//}
		//MessageChain& Json(const string& json_str)
		//{
		//	json j;
		//	j["type"] = "Json";
		//	j["json"] = json_str;
		//	messages_.push_back(j);
		//	return *this;
		//}
		//MessageChain& App(const string& app_str)
		//{
		//	json j;
		//	j["type"] = "App";
		//	j["content"] = app_str;
		//	messages_.push_back(j);
		//	return *this;
		//}
		//MessageChain& Poke(Poke poke)
		//{
		//	const static string poke_name[] =
		//	{ "Poke","ShowLove","Like","Heartbroken","SixSixSix","FangDaZhao" };
		//	json j;
		//	j["type"] = "Poke";
		//	j["name"] = poke_name[static_cast<int>(poke)];
		//	messages_.push_back(j);
		//	return *this;
		//}

		//string GetPlainText() const
		//{
		//	using std::stringstream;
		//	stringstream ss;
		//	for (const auto& ele : messages_)
		//	{
		//		if (ele["type"].get<string>() == "Plain")
		//		{
		//			ss << ele["text"].get<string>();
		//		}
		//	}
		//	return ss.str();
		//}
		//string GetPlainTextFirst() const
		//{
		//	for (const auto& ele : messages_)
		//	{
		//		if (ele["type"].get<string>() == "Plain")
		//		{
		//			return ele["text"].get<string>();
		//		}
		//	}
		//	return string();
		//}

		//vector<string> GetPlain() const
		//{
		//	vector<string> res;
		//	for (const auto& ele : messages_)
		//	{
		//		if (ele["type"].get<string>() == "Plain")
		//		{
		//			res.emplace_back(ele["text"].get<string>());
		//		}
		//	}
		//	return res;
		//}

		//vector<MiraiImage> GetImage() const
		//{
		//	vector<MiraiImage> res;
		//	for (const auto& ele : messages_)
		//	{
		//		string type_name = ele["type"].get<string>();
		//		if (type_name == "FlashImage" || type_name == "Image")
		//		{
		//			MiraiImage img;
		//			img.IsFlashImage = (type_name == "FlashImage");
		//			img.Path = "";
		//			img.Url = ele["url"].get<string>();
		//			img.ID = ele["imageId"].get<string>();
		//			res.emplace_back(img);
		//		}
		//	}
		//	return res;
		//}

		vector<QQ_t> GetAt() const
		{
			vector<QQ_t> res;
			//for (const auto& ele : messages_)
			//{
			//	string type_name = ele["type"].get<string>();
			//	if (type_name == "At")
			//	{
			//		QQ_t qq = (QQ_t)(ele["target"].get<int64_t>());
			//		res.emplace_back(qq);
			//	}
			//}
			return res;
		}

		//vector<int> GetFace() const
		//{
		//	vector<int> res;
		//	for (const auto& ele : messages_)
		//	{
		//		string type_name = ele["type"].get<string>();
		//		if (type_name == "Face")
		//		{
		//			int faceId = ele["faceId"].get<int>();
		//			res.emplace_back(faceId);
		//		}
		//	}
		//	return res;
		//}

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
			if (!j.empty())
			{
				try
				{
					if (j[0]["type"].get<string>() == "Source")
					{
						this->messageId_ = j[0]["id"].get<int64_t>();
						this->timestamp_ = j[0]["time"].get<int64_t>();
					}
				}
				catch (...)
				{
					this->messageId_ = 0;
					this->timestamp_ = 0;
				}

				for (size_t i = 1; i < j.size(); i++)
				{
					auto msg_ptr =  factory_.DynamicCreate(j[i]["type"]);
					if (msg_ptr)
					{
						msg_ptr->Set(j[i]);
						messages_.push_back(msg_ptr);
					}
				}

			}

			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::array();
			for (const auto& m : messages_)
			{
				j.push_back(m->ToJson());
			}
			return j;
		}
		virtual string ToString() const override
		{
			return ToJson().dump();
		}

	private:
		int64_t timestamp_;
		MessageId messageId_;
		vector<std::shared_ptr<IMessage>> messages_;
		Cyan::Reflection<IMessage> factory_;
	};

	//template<int N>
	//inline MessageChain& operator+(const char (&str)[N], MessageChain& mc)
	//{
	//	MessageChain tmp;
	//	tmp.Plain(str);
	//	mc.messages_.insert(mc.messages_.begin(), tmp.messages_.begin(), tmp.messages_.end());
	//	return mc;
	//}

	//inline MessageChain& operator+(const string& str, MessageChain& mc)
	//{
	//	MessageChain tmp;
	//	tmp.Plain(str);
	//	mc.messages_.insert(mc.messages_.begin(), tmp.messages_.begin(), tmp.messages_.end());
	//	return mc;
	//}
}


#endif // !mirai_cpp_defs_message_chain_hpp_H_