#pragma once
#ifndef mirai_cpp__serializable_hpp_H_
#define mirai_cpp__serializable_hpp_H_
#include <rapidjson/document.h>
#include <string>
using std::string;

typedef rapidjson::Document JsonDoc;
typedef rapidjson::Value JsonVal;
namespace Cyan
{
	class Serializable
	{
	public:
		Serializable() {}
		virtual bool Set(JsonVal& json) = 0;
		virtual const JsonDoc& ToJson() = 0;
		virtual string ToString() = 0;
		virtual ~Serializable() {}
	};



} // namespace Cyan

#endif // !mirai_cpp__serializable_hpp_H_