#pragma once
#ifndef mirai_cpp_defs_file_download_info_hpp_H_
#define mirai_cpp_defs_file_download_info_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "ISerializable.hpp"

namespace Cyan
{
	/**
	 * @brief 文件下载信息
	*/
	class FileDownloadInfo : public ISerializable
	{
	public:
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
			DownloadUrl = j["url"].get<string>();
			Sha1 = j["sha1"].get<string>();
			Md5 = j["md5"].get<string>();
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
				{ "url", DownloadUrl },
				{ "sha1", Sha1 },
				{ "md5", Md5 },
				{ "downloadTimes", DownloadCount },
				{ "uploadTime", UploadTime },
				{ "lastModifyTime", LastModifyTime },
				{ "uploaderId", Uploader.ToInt64() }
			};
		}
	};

}

#endif // !mirai_cpp_defs_file_download_info_hpp_H_