#pragma once
#ifndef mirai_cpp_defs_group_file_hpp_H_
#define mirai_cpp_defs_group_file_hpp_H_
#include <memory>
#include <optional>
#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "ISerializable.hpp"
#include "FileDownloadInfo.hpp"
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
		 * @brief 文件大小
		*/
		int Size = 0;

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

		/**
		 * @brief 文件下载信息
		*/
		std::optional<FileDownloadInfo> DownloadInfo = std::nullopt;

		virtual bool Set(const json& j) override
		{
			Size = j["size"].get<int>();
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
			if (!j["downloadInfo"].is_null())
			{
				FileDownloadInfo t;
				t.Set(j["downloadInfo"]);
				DownloadInfo = t;
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
				{ "size", Size },
				{ "contact", Group.ToJson() },
				{ "isFile", !IsDirectory },
				{ "isDirectory", IsDirectory },
				{ "parent", ParentDirectory != nullptr ? ParentDirectory->ToJson() : RootDirectoryJson() },
				{ "downloadInfo", DownloadInfo ? DownloadInfo->ToJson() : json(nullptr) }
			};
		}
	private:
		json RootDirectoryJson() const
		{
			return
			{
				{ "id", json(nullptr) },
				{ "parent", json(nullptr) },
				{ "downloadInfo", json(nullptr) },
				{ "name", "" },
				{ "path", "/" },
				{ "Size", 0 },
				{ "isFile", false },
				{ "isDirectory", true },
				{ "contact", Group.ToJson() }
			};
		}
	};

}

#endif // !mirai_cpp_defs_group_file_hpp_H_