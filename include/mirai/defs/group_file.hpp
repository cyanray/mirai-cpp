#pragma once
#ifndef mirai_cpp_defs_group_file_hpp_H_
#define mirai_cpp_defs_group_file_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "serializable.hpp"

namespace Cyan
{
	// 群文件
	class GroupFile : public ISerializable
	{
	public:
		/**
		 * @brief 文件名称
		*/
		string Name;
		/**
		 * @brief 文件ID
		*/
		string Id;
		/**
		 * @brief 完整路径
		*/
		string Path;
		/**
		 * @brief 是否为目录
		*/
		bool IsDirectory = false;

		virtual bool Set(const json& j) override
		{
			Name = j["name"].get<string>();
			Id = j["id"].get<string>();
			Path = j["path"].get<string>();
			IsDirectory = !j["isFile"].get<bool>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "name", Name },
				{ "id", Id },
				{ "path", Path },
				{ "isFile", IsDirectory }
			};
		}
	};

}

#endif // !mirai_cpp_defs_group_file_hpp_H_