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
		MessageChain();
		MessageChain(const MessageChain& mc);
		MessageChain(MessageChain&& mc) noexcept;
		MessageChain& operator=(const MessageChain& mc);
		MessageChain& operator=(MessageChain&& mc) noexcept;
		MessageChain& operator+(const MessageChain& mc);
		MessageChain& operator+(const string& val);
		MessageChain& operator+=(const string& val);
		bool operator==(const MessageChain& mc) const;
		bool operator!=(const MessageChain& mc) const;
		std::shared_ptr<IMessage> operator[](int i);
		virtual ~MessageChain() = default;

		template<class T>
		std::vector<T> GetAll() const
		{
			vector<T> tmp;
			for (size_t i = 0; i < messages_.size(); i++)
			{
				if (auto m = std::dynamic_pointer_cast<T>(messages_[i]))
				{
					tmp.push_back(*m);
				}
			}
			return tmp;
		}

		template<class T>
		T GetFirst() const
		{
			for (size_t i = 0; i < messages_.size(); i++)
			{
				if (auto m = std::dynamic_pointer_cast<T>(messages_[i]))
				{
					return *m;
				}
			}
			throw std::runtime_error("没有找到指定类型的元素");
		}

		template<class T>
		MessageChain& Add(const T& m)
		{
			static_assert(std::is_base_of<IMessage, T>::value, "只能接受 IMessage 的派生类");
			std::shared_ptr<IMessage> m_ptr(new T(m));
			messages_.push_back(m_ptr);
			return *this;
		}

		size_t Count() const
		{
			return messages_.size();
		}

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