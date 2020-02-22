#pragma once
#ifndef mirai_cpp__message_chain_hpp_H_
#define mirai_cpp__message_chain_hpp_H_
#include <sstream>
#include <string>
#include <exception>
#include <iostream>
#include "serializable.hpp"
#include "typedef.hpp"
#include <rapidjson/document.h>
using std::string;
using std::stringstream;
using std::runtime_error;
namespace Cyan
{
	class MessageChain : public Serializable
	{
	public:
		MessageChain()
		{
			messages_.SetArray();
		}
		virtual ~MessageChain() = default;
		MessageChain& At(const QQ_t qq)
		{
			using namespace rapidjson;
			static const char* const jsonStr = R"( { "type":"At" , "target":1234567 , "display":"@123" } )";
			JsonDoc jdoc;
			if (jdoc.Parse(jsonStr).HasParseError()) throw runtime_error("未知错误");
			jdoc["target"].SetInt64(qq);
			Value v(jdoc, messages_.GetAllocator());
			messages_.PushBack(v, messages_.GetAllocator());
			return *this;
		}
		MessageChain& AtAll()
		{
			using namespace rapidjson;
			static const char* const jsonStr = R"( { "type":"AtAll" } )";
			JsonDoc jdoc;
			if (jdoc.Parse(jsonStr).HasParseError()) throw runtime_error("未知错误");
			Value v(jdoc, messages_.GetAllocator());
			messages_.PushBack(v, messages_.GetAllocator());
			return *this;
		}
		MessageChain& Face(int faceID)
		{
			using namespace rapidjson;
			static const char* const jsonStr = R"( { "type":"Face" , "faceId":123 } )";
			JsonDoc jdoc;
			if (jdoc.Parse(jsonStr).HasParseError()) throw runtime_error("未知错误");
			jdoc["faceId"].SetInt(faceID);
			Value v(jdoc, messages_.GetAllocator());
			messages_.PushBack(v, messages_.GetAllocator());
			return *this;
		}
		MessageChain& Plain(const string& plainText)
		{
			using namespace rapidjson;
			static const char* const jsonStr = R"( { "type":"Plain" , "text":"" } )";
			JsonDoc jdoc;
			if (jdoc.Parse(jsonStr).HasParseError()) throw runtime_error("未知错误");
			jdoc["text"].SetString(plainText.data(), plainText.size(),messages_.GetAllocator());
			Value v(jdoc, messages_.GetAllocator());
			messages_.PushBack(v, messages_.GetAllocator());
			return *this;
		}
		MessageChain& Image(const FriendImage& ImageID)
		{
			using namespace rapidjson;
			static const char* const jsonStr = R"( { "type":"Image" , "imageID":"" } )";
			JsonDoc jdoc;
			if (jdoc.Parse(jsonStr).HasParseError()) throw runtime_error("未知错误");
			jdoc["imageID"].SetString(ImageID.ID.data(), ImageID.ID.size());
			Value v(jdoc, messages_.GetAllocator());
			messages_.PushBack(v, messages_.GetAllocator());
			return *this;
		}
		MessageChain& Image(const GroupImage& ImageID)
		{
			using namespace rapidjson;
			static const char* const jsonStr = R"( { "type":"Image" , "imageID":"" } )";
			JsonDoc jdoc;
			if (jdoc.Parse(jsonStr).HasParseError()) throw runtime_error("未知错误");
			jdoc["imageID"].SetString(ImageID.ID.data(), ImageID.ID.size());
			Value v(jdoc, messages_.GetAllocator());
			messages_.PushBack(v, messages_.GetAllocator());
			return *this;
		}
		
		// 通过 JsonDoc 设置对象的值，
		// 不检查传入 JsonDoc 的值是否符合要求
		virtual bool Set(JsonVal& json) override
		{
			if (!json.IsArray()) return false;
			JsonDoc newJsonDoc;
			messages_.Swap(newJsonDoc);
			messages_.SetArray();
			for (rapidjson::SizeType i = 0; i < json.Size(); i++)
			{
				messages_.PushBack(json[i], messages_.GetAllocator());
			}
		}
		virtual const JsonDoc& ToJson() override
		{
			return messages_;
		}
		virtual string ToString() override
		{
			return JsonDoc2String(messages_);
		}
	private:
		JsonDoc messages_;
	};


} // namespace Cyan

#endif // !mirai_cpp__message_chain_hpp_H_
