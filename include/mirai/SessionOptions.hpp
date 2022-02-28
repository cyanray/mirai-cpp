#pragma once
#ifndef mirai_cpp__SessionOptions_hpp_H_
#define mirai_cpp__SessionOptions_hpp_H_

#include <string>
#include "mirai/defs/QQType.hpp"
#include "mirai/Property.hpp"

using std::string;
using namespace std::literals::string_literals;

namespace Cyan
{
	class SessionOptions
	{
	public:
		/**
		 * @brief 是否进行认证流程.
		*/
		Property<bool> EnableVerify = true;

		/**
		 * @brief 是否为 SingleMode.
		*/
		Property<bool> SingleMode = false;

		/**
		 * @brief 与 mirai-api-http 通信的端口(Http 适配器)，默认值为 8080.
		*/
		Property<short> HttpPort = 8080;

		/**
		 * @brief 与 mirai-api-http 通信的端口(WebSocket 适配器)，默认值为 8080.
		*/
		Property<short> WebSocketPort = 8080;

		/**
		 * @brief WebSocket 通信保留的同步ID(默认为"-1").
		*/
		Property<string> ReservedSyncId = "-1";

		/**
		 * @brief mirai-api-http 历史消息缓存容量, 过小可能导致撤回消息失败, 默认值为 4096.
		*/
		Property<int> CacheSize = 4096;

		/**
		 * @brief mirai-cpp 线程池的容量, 建议设置为 CPU 核心数 * 3.
		*/
		Property<short> ThreadPoolSize = 6;

		/**
		 * @brief 机器人的QQ号(SingleMode=true时忽略此设置).
		*/
		Property<QQ_t> BotQQ;

		/**
		 * @brief Hostname (Http 适配器), 默认值为 localhost.
		*/
		Property<string> HttpHostname = "localhost"s;

		/**
		 * @brief Hostname (WebScoket 适配器), 默认值为 localhost.
		*/
		Property<string> WebSocketHostname = "localhost"s;

		/**
		 * @brief 认证流程需要的密钥(EnableVerify=false时忽略此参数).
		*/
		Property<string> VerifyKey;

		static SessionOptions FromCommandLine(int argc, char* argv[]);
		static SessionOptions FromJsonFile(const string& filename);
		static SessionOptions FromJson(const json& json);
	};
}

#endif //! mirai_cpp__SessionOptions_hpp_H_