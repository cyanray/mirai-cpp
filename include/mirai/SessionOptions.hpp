#pragma once
#ifndef mirai_cpp__SessionOptions_hpp_H_
#define mirai_cpp__SessionOptions_hpp_H_

#include <string>
#include "mirai/defs/qq_types.hpp"
#include "mirai/Property.hpp"

using std::string;
using namespace std::literals::string_literals;

namespace Cyan
{
	class SessionOptions
	{
	public:
		Property<short> Port = 8080;
		Property<QQ_t> BotQQ;
		Property<string> Hostname = "localhost"s;
		Property<string> VerifyKey;
	};
}

#endif //! mirai_cpp__SessionOptions_hpp_H_