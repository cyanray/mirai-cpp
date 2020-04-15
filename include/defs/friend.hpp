#pragma once
#ifndef mirai_cpp_defs_friend_hpp_H_
#define mirai_cpp_defs_friend_hpp_H_

#include <nlohmann/json.hpp>
#include "qq_types.hpp"
#include "serializable.hpp"

namespace Cyan
{

	// 好友数据格式
	class Friend_t : public Serializable
	{
	public:
		QQ_t QQ = 0;
		string NickName;
		string Remark;

		Friend_t() = default;
		Friend_t(const Friend_t& f)
		{
			QQ = f.QQ;
			NickName = f.NickName;
			Remark = f.Remark;
		}
		Friend_t& operator=(const Friend_t& t)
		{
			Friend_t tmp(t);
			std::swap(this->QQ, tmp.QQ);
			std::swap(this->NickName, tmp.NickName);
			std::swap(this->Remark, tmp.Remark);
			return *this;
		}
		virtual ~Friend_t() = default;
		virtual bool Set(const json& j) override
		{
			QQ = j["id"].get<int64_t>();
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