#pragma once
#ifndef mirai_cpp__serializable_hpp_H_
#define mirai_cpp__serializable_hpp_H_
#include <nlohmann/json.hpp>
#include <string>
using std::string;
using nlohmann::json;

namespace Cyan
{
	class Serializable
	{
	public:
		Serializable() {}
		virtual bool Set(const json& json) = 0;
		virtual json ToJson() const = 0;
		virtual string ToString() const = 0;
		virtual ~Serializable() {}
	};



} // namespace Cyan

#endif // !mirai_cpp__serializable_hpp_H_