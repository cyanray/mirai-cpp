#pragma once
#ifndef mirai_cpp_exceptions_network_exception_hpp_H_
#define mirai_cpp_exceptions_network_exception_hpp_H_

#include <stdexcept>

namespace Cyan
{
	/**
	 * @brief 网络错误异常
	*/
	class NetworkException : public std::runtime_error
	{
	public:
		NetworkException(): std::runtime_error("网络错误.") {}
		virtual ~NetworkException() = default;
	};

}

#endif // !mirai_cpp_exceptions_network_exception_hpp_H_