#pragma once
#ifndef mirai_cpp_defs_friend_hpp_H_
#define mirai_cpp_defs_friend_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "qq_types.hpp"
#include "serializable.hpp"

namespace Cyan
{

	// 好友数据格式
	class Friend_t : public ISerializable
	{
	public:
		QQ_t QQ;
		string NickName;
		string Remark;
		virtual bool Set(const json& j) override
		{
			QQ = QQ_t(j["id"].get<int64_t>());
			NickName = j["nickname"].get<string>();
			Remark = j["remark"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["id"] = int64_t(QQ);
			j["nickname"] = NickName;
			j["remark"] = Remark;
			return j;
		}

	};

}

#endif // !mirai_cpp_defs_friend_hpp_H_