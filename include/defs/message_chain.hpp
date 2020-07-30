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
#include "defs/messages/ImageMessage.hpp"
#include "defs/messages/FlashImageMessage.hpp"
#include "defs/messages/AtMessage.hpp"
#include "defs/messages/AtAllMessage.hpp"
#include "defs/messages/FaceMessage.hpp"

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

		template<class T>
		void Remove(const T& m)
		{
			static_assert(std::is_base_of<IMessage, T>::value, "只能接受 IMessage 的派生类");
			messages_.erase(std::remove_if(messages_.begin(), messages_.end(), 
				[&](std::shared_ptr<IMessage> item) 
				{
					return *item == m;
				}), messages_.end());
		}

		size_t Count() const
		{
			return messages_.size();
		}

		void Clear()
		{
			messages_.clear();
		}

		std::vector<std::shared_ptr<IMessage>>& ToVector()
		{
			return messages_;
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
			return this->Add(PlainMessage(plainText));
		}

		template<typename T>
		MessageChain& Plain(const T& val)
		{
			std::stringstream ss;
			ss << val;
			return this->Add(PlainMessage(ss.str()));
		}

		MessageChain& Image(const MiraiImage& Image)
		{
			return this->Add(ImageMessage(Image));
		}

		MessageChain& FlashImage(const MiraiImage& Image)
		{
			return this->Add(FlashImageMessage(Image));
		}

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

		string GetPlainText() const;

		string GetPlainTextFirst() const;

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

		virtual bool Set(const json& j) override;
		virtual json ToJson() const override
		{
			json j = json::array();
			for (const auto& m : messages_)
			{
				j.push_back(m->ToJson());
			}
			return j;
		}

	private:
		int64_t timestamp_;
		MessageId messageId_;
		vector<std::shared_ptr<IMessage>> messages_;
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