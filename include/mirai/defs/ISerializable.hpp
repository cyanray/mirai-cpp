#pragma once
#ifndef mirai_cpp_defs_serializable_hpp_H_
#define mirai_cpp_defs_serializable_hpp_H_
#include "mirai/third-party/nlohmann/json.hpp"
#include <string>
using std::string;
using nlohmann::json;

namespace Cyan
{
	class ISerializable
	{
	public:
		ISerializable() = default;
		virtual bool Set(const json& json) = 0;
		virtual json ToJson() const = 0;
		virtual string ToString() const
		{
			return ToJson().dump();
		}
		virtual explicit operator string() const
		{
			return ToJson().dump();
		}
		virtual ~ISerializable() = default;
	};



} // namespace Cyan

#endif // !mirai_cpp_defs_serializable_hpp_H_