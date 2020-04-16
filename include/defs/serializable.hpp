#pragma once
#ifndef mirai_cpp_defs_serializable_hpp_H_
#define mirai_cpp_defs_serializable_hpp_H_
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
		virtual bool Set(const json& json) { return false; }
		virtual json ToJson() const { return json::object(); }
		virtual string ToString() const
		{
			return ToJson().dump();
		}
		virtual ~Serializable() {}
	};



} // namespace Cyan

#endif // !mirai_cpp_defs_serializable_hpp_H_