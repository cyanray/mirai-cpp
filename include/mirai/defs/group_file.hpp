#pragma once
#ifndef mirai_cpp_defs_group_file_hpp_H_
#define mirai_cpp_defs_group_file_hpp_H_
#include <memory>
#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/qq_types.hpp"
#include "serializable.hpp"
using std::shared_ptr;

namespace Cyan
{
	// 群文件
	class GroupFile : public ISerializable
	{
	public:
		/**
		 * @brief 是否为目录
		*/
		bool IsDirectory = false;

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
		 * @brief 文件所属群
		*/
		Group_t Group;

		/**
		 * @brief 文件所属父目录(该值为 nullptr 时，父目录为根目录)
		*/
		std::shared_ptr<GroupFile> ParentDirectory = nullptr;

		virtual bool Set(const json& j) override
		{
			Name = j["name"].get<string>();
			Id = j["id"].get<string>();
			Path = j["path"].get<string>();
			IsDirectory = !j["isFile"].get<bool>();
			Group.Set(j["contact"]);
			if (!j["parent"]["id"].is_null())
			{
				ParentDirectory = std::make_shared<GroupFile>();
				ParentDirectory->Set(j["parent"]);
			}
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "name", Name },
				{ "id", Id },
				{ "path", Path },
				{ "contact", Group.ToJson() },
				{ "isFile", !IsDirectory },
				{ "isDictionary", IsDirectory },
				{ "parent", ParentDirectory != nullptr ? ParentDirectory->ToJson() : RootDirectoryJson() }
			};
		}
	private:
		json RootDirectoryJson() const
		{
			return
			{
				{ "id", json(nullptr) },
				{ "parent", json(nullptr) },
				{ "name", "" },
				{ "path", "/" },
				{ "isFile", false },
				{ "isDictionary", true },
				{ "contact", Group.ToJson() }
			};
		}
	};

}

#endif // !mirai_cpp_defs_group_file_hpp_H_