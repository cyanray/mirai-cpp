#pragma once
#ifndef mirai_cpp_exceptions_mirai_api_http_exception_hpp_H_
#define mirai_cpp_exceptions_mirai_api_http_exception_hpp_H_

#include <exception>
#include <string>

namespace Cyan
{
	/**
	 * @brief mirai-api-http “Ï≥£
	*/
	class MiraiApiHttpException : public std::runtime_error
	{
	public:
		const int Code;
		const string Message;
		MiraiApiHttpException(int code, const std::string& error_msg) : 
			Code(code),
			Message(error_msg),
			std::runtime_error("mirai-api-http ¥ÌŒÛ: " + error_msg) {}
		virtual ~MiraiApiHttpException() = default;
	};

}

#endif // !mirai_cpp_exceptions_mirai_api_http_exception_hpp_H_