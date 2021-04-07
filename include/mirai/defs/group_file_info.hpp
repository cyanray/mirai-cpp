#pragma once
#ifndef mirai_cpp_defs_group_file_info_hpp_H_
#define mirai_cpp_defs_group_file_info_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/qq_types.hpp"
#include "serializable.hpp"

namespace Cyan
{
	// 群文件详细信息
	class GroupFileInfo : public ISerializable
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
		 * @brief 下载地址
		*/
		string DownloadUrl;
		/**
		 * @brief Sha1
		*/
		string Sha1;
		/**
		 * @brief Md5
		*/
		string Md5;
		/**
		 * @brief 文件大小
		*/
		size_t Size = 0;
		/**
		 * @brief 下载次数
		*/
		size_t DownloadCount = 0;
		/**
		 * @brief 上传时间戳
		*/
		int64_t UploadTime = 0;
		/**
		 * @brief 最后修改时间戳
		*/
		int64_t LastModifyTime = 0;
		/**
		 * @brief 上传者
		*/
		QQ_t Uploader;

		virtual bool Set(const json& j) override
		{
			Name = j["name"].get<string>();
			Id = j["id"].get<string>();
			Path = j["path"].get<string>();
			DownloadUrl = j["downloadUrl"].get<string>();
			Sha1 = j["sha1"].get<string>();
			Md5 = j["md5"].get<string>();
			Size = j["length"].get<size_t>();
			DownloadCount = j["downloadTimes"].get<size_t>();
			UploadTime = j["uploadTime"].get<int64_t>();
			LastModifyTime = j["lastModifyTime"].get<int64_t>();
			Uploader = (QQ_t)(j["uploaderId"].get<int64_t>());
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "name", Name },
				{ "id", Id },
				{ "path", Path },
				{ "downloadUrl", DownloadUrl },
				{ "sha1", Sha1 },
				{ "md5", Md5 },
				{ "length", Size },
				{ "downloadTimes", DownloadCount },
				{ "uploadTime", UploadTime },
				{ "lastModifyTime", LastModifyTime },
				{ "uploaderId", Uploader.ToInt64() }
			};
		}
	};

}

#endif // !mirai_cpp_defs_group_file_info_hpp_H_