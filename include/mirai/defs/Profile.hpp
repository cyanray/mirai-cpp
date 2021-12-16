#pragma once
#ifndef mirai_cpp_defs_profile_hpp_H_
#define mirai_cpp_defs_profile_hpp_H_

#include <string>
#include "mirai/third-party/nlohmann/json.hpp"
#include "ISerializable.hpp"


namespace Cyan
{
	/**
	 * @brief 个人简介
	*/
	class Profile : public ISerializable
	{
	public:
		/**
		 * @brief 年龄
		*/
		int Age = 0;

		/**
		 * @brief QQ账号等级
		*/
		int Level = 0;

		/**
		 * @brief 个性签名
		*/
		string Sign;

		/**
		 * @brief 性别 (MALE | FEMALE)
		*/
		string Sex;

		/**
		 * @brief 昵称
		*/
		string NickName;

		/**
		 * @brief 邮箱
		*/
		string Email;
		
		virtual bool Set(const json& j) override
		{
			NickName = j["nickname"].get<string>();
			Email = j["email"].get<string>();
			Sex = j["sex"].get<string>();
			Sign = j["sign"].get<string>();
			Age = j["age"].get<int>();
			Level = j["level"].get<int>();
			return true;
		}
		virtual json ToJson() const override
		{
			return 
			{
				{ "nickname", NickName },
				{ "email", Email },
				{ "sex", Sex },
				{ "sign", Sign },
				{ "age", Age },
				{ "level", Level }
			};
		}

	};

}

#endif // !mirai_cpp_defs_group_member_info_hpp_H_