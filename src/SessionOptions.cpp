#include "mirai/SessionOptions.hpp"
#include <fstream>
#include <regex>
using namespace std;
using namespace nlohmann;

namespace Cyan
{
	SessionOptions SessionOptions::FromCommandLine(int argc, char* argv[])
	{
		json config_json;
		regex pattern(R"(--([^=]+)=([\S]+))");
		for (int i = 0; i < argc; i++)
		{
			cmatch match;
			if (regex_match(argv[i], match, pattern))
			{
				if (match.size() >= 2)
				{
					string key = match[1].str();
					string value = match[2].str();
					config_json[key] = value;
				}
			}
		}
		return SessionOptions::FromJson(config_json);
	}

	SessionOptions SessionOptions::FromJsonFile(const string& filename)
	{
		ifstream json_file(filename);
		if (!json_file.is_open())
		{
			throw std::runtime_error("打开文件时出错!");
		}
		json config_json = json::parse(json_file);
		return SessionOptions::FromJson(config_json);
	}

	SessionOptions SessionOptions::FromJson(const json& json)
	{
		SessionOptions opt;
		if (json.contains("hostname"))
		{
			string hostname = json["hostname"].get<string>();
			opt.WebSocketHostname = hostname;
			opt.HttpHostname = hostname;
		}
		if (json.contains("port"))
		{
			auto &json_port = json["port"];
			short port;
			if (json_port.type() == nlohmann::detail::value_t::string)
			{
				port = stoi(json_port.get<string>());
			}
			else
			{
				port = json_port.get<short>();
			}
			opt.HttpPort = port;
			opt.WebSocketPort = port;
		}
		if (json.contains("botQQ"))
		{
			opt.BotQQ = (QQ_t)(json["botQQ"].get<int64_t>());
		}
		if (json.contains("bot-qq"))
		{
			auto qq_str = json["bot-qq"].get<string>();
			opt.BotQQ = (QQ_t)(strtoll(qq_str.data(), nullptr, 0));
		}
		if (json.contains("verifyKey"))
		{
			opt.VerifyKey = json["verifyKey"].get<string>();
		}
		if (json.contains("verify-key"))
		{
			opt.VerifyKey = json["verify-key"].get<string>();
		}
		return opt;
	}
}