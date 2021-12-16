#pragma once
#ifndef mirai_cpp_defs_friend_hpp_H_
#define mirai_cpp_defs_friend_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "QQType.hpp"
#include "ISerializable.hpp"

namespace Cyan
{

	/**
	 * @brief QQ好友
	*/
	class Friend_t : public ISerializable
	{
	public:
		/**
		 * @brief QQ号
		*/
		QQ_t QQ;

		/**
		 * @brief 昵称
		*/
		string NickName;

		/**
		 * @brief 备注
		*/
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