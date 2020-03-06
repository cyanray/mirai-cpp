#pragma once
#include <string>
#include <map>
#include <regex>
#include <curl/curl.h>
#include <ostream>
#include <sstream>
using std::string;
using std::map;
using std::regex;
using std::stringstream;
namespace Cyan
{
	class CookieContainer : public map<string, string>
	{
	public:
		class CookieValue
		{
			friend class CookieContainer;
			friend std::ostream& operator <<(std::ostream&, const CookieValue&);
		public:
			void operator=(const string& value)
			{
				auto it = cc->find(CookieKey);
				if (it == cc->end())
				{
					cc->insert(std::make_pair(CookieKey, value));
				}
				else
				{
					it->second = value;
				}

			}
			operator string() const
			{
				auto it = cc->find(CookieKey);
				if (it != cc->end())
				{
					return it->second;
				}
				throw "未找到Key对应的Cookie";
			}
		private:
			CookieContainer* cc;
			string CookieKey;
			CookieValue(CookieContainer* value, const string& key) :cc(value), CookieKey(key) {}
			~CookieValue() = default;
		}; // class CookieValue

		CookieContainer() = default;
		CookieContainer(const string& cookies)
		{
			parse(cookies);
		}
		void parse(string cookies)
		{
			std::smatch match;
			std::regex pattern(R"(([^=]+)=([^;]+);?\s*)");
			while (std::regex_search(cookies, match, pattern))
			{
				string key = match[1].str();
				string val = match[2].str();
				trim(key);
				trim(val);
				map<string, string>::insert(std::make_pair(key, val));
				cookies = match.suffix().str();
			}
		}
		CookieValue operator[](const string& CookieKey)
		{
			return CookieValue(this, CookieKey);
		}
		string toString() const
		{
			stringstream ss;
			for (auto& Cookie : *this)
			{
				ss << Cookie.first << "=" << Cookie.second << "; ";
			}
			return ss.str();
		}
		bool exist(const string& CookieKey)
		{
			return (map<string, string>::find(CookieKey) != map<string, string>::end());
		}
		bool remove(const string& CookieKey)
		{
			auto it = map<string, string>::find(CookieKey);
			if (it == map<string, string>::end())
			{
				return false;
			}
			else
			{
				map<string, string>::erase(it);
				return true;
			}
		}
		virtual ~CookieContainer() {}
	private:
		void trim(string& s)
		{
			if (s.empty())
			{
				return;
			}
			s.erase(0, s.find_first_not_of(" "));
			s.erase(s.find_last_not_of(" ") + 1);
		}
	}; // class CookieContainer
	std::ostream& operator <<(std::ostream& os, const CookieContainer::CookieValue& v)
	{
		os << static_cast<string>(v);
		return os;
	}
	class HTTP
	{
	public:
		struct Response
		{
			bool Ready = false;
			CURLcode CURLCode;
			string ErrorMsg;
			string Content;
		};
		const string DefaultContentType = "application/x-www-form-urlencoded";
		const string DefaultAccept = "*/*";
		const string DefaultUserAgent =
			"Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
			"AppleWebKit/537.36 (KHTML, like Gecko) "
			"Chrome/64.0.3282.140 Safari/537.36 Edge/17.17134";
		HTTP() :
			curl(nullptr),
			slist(nullptr),
			followRedirect(true),
			timeout(120),
			maxRedirs(5),
			cookieContainer(),
			contentType(DefaultContentType),
			accept(DefaultAccept)
		{
			errbuf[0] = 0;
		}
		void static InitGlobal()
		{
			curl_global_init(CURL_GLOBAL_ALL);
		}

		void static ReleaseGlobal()
		{
			curl_global_cleanup();
		}

		HTTP& FollowRedirect(bool redirect)
		{
			followRedirect = redirect;
			return *this;
		}
		HTTP& SetTimeout(size_t seconds)
		{
			timeout = seconds;
			return *this;
		}
		HTTP& SetMaxRedirs(size_t times)
		{
			maxRedirs = times;
			return *this;
		}
		HTTP& SetCookieContainer(const CookieContainer& cc)
		{
			cookieContainer = cc;
			return *this;
		}
		HTTP& SetContentType(const string& ContentType)
		{
			contentType = ContentType;
			return *this;
		}
		HTTP& SetAccept(const string& Accept)
		{
			accept = Accept;
			return *this;
		}
		HTTP& SetUserAgent(const string& UserAgent)
		{
			userAgent = UserAgent;
			return *this;
		}
		HTTP& AddHeader(const string& name, const string& value)
		{
			string t = name + ": " + value;
			slist = curl_slist_append(slist, t.data());
			return *this;
		}
		CookieContainer& GetCookieContainer()
		{
			return cookieContainer;
		}

		HTTP& AddPostData(const string& name, const string& data)
		{
			if (form == nullptr)
				form = curl_mime_init(curl);
			field = curl_mime_addpart(form);
			curl_mime_name(field, name.data());
			curl_mime_data(field, data.data(), CURL_ZERO_TERMINATED);
			return *this;
		}

		HTTP& AddFile(const string& name, const string& fileName)
		{
			if (form == nullptr)
				form = curl_mime_init(curl);
			field = curl_mime_addpart(form);
			curl_mime_name(field, name.data());
			curl_mime_filedata(field, fileName.data());
			return *this;
		}

		string URLEncode(const string& str)
		{
			auto encoded = curl_easy_escape(curl, str.data(), str.size());	// TODO:错误处理
			return string(encoded);
		}

		const Response Get(const string& URL)
		{
			Response resp;
			if (curl == NULL)
			{
				curl = curl_easy_init();
			}

			// 初始化CURL失败，获得错误描述、清理CURL、返回
			if (!curl)
			{
				resp.Ready = false;
				resp.ErrorMsg = GetErrorStr();
				resp.CURLCode = curlCode;
				resp.Content = "";
				CURLCleanup();
				return resp;
			}

			string tStr = execute(URL);

			if (curlCode == CURLcode::CURLE_OK)
			{
				resp.Ready = true;
				resp.CURLCode = curlCode;
				resp.ErrorMsg = "";
				resp.Content = tStr;
			}
			else
			{
				resp.Ready = false;
				resp.ErrorMsg = GetErrorStr();
				resp.CURLCode = curlCode;
				resp.Content = "";
			}

			CURLCleanup();
			return resp;
		}

		const Response Post(const string& URL, const string& Data)
		{
			Response resp;
			if (curl == NULL)
			{
				curl = curl_easy_init();
			}

			// 初始化CURL失败，获得错误描述、清理CURL、返回
			if (!curl)
			{
				resp.Ready = false;
				resp.ErrorMsg = GetErrorStr();
				resp.CURLCode = curlCode;
				resp.Content = "";
				CURLCleanup();
				return resp;
			}

			// Post只是在Get的基础上增加几项设置
			if (!contentType.empty())
				slist = curl_slist_append(slist, ("Content-Type: " + contentType).data());
			curl_easy_setopt(curl, CURLOPT_POST, 1L);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Data.data());

			string tStr = execute(URL);

			if (curlCode == CURLcode::CURLE_OK)
			{
				resp.Ready = true;
				resp.CURLCode = curlCode;
				resp.ErrorMsg = "";
				resp.Content = tStr;
			}
			else
			{
				resp.Ready = false;
				resp.ErrorMsg = GetErrorStr();
				resp.CURLCode = curlCode;
				resp.Content = "";
			}

			CURLCleanup();
			return resp;
		}

		const Response Post(const string& URL)
		{
			Response resp;
			if (curl == NULL)
			{
				curl = curl_easy_init();
			}

			// 初始化CURL失败，获得错误描述、清理CURL、返回
			if (!curl)
			{
				resp.Ready = false;
				resp.ErrorMsg = GetErrorStr();
				resp.CURLCode = curlCode;
				resp.Content = "";
				CURLCleanup();
				return resp;
			}

			// Post只是在Get的基础上增加几项设置
			if (!contentType.empty())
				slist = curl_slist_append(slist, ("Content-Type: " + contentType).data());
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
			curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

			string tStr = execute(URL);

			if (curlCode == CURLcode::CURLE_OK)
			{
				resp.Ready = true;
				resp.CURLCode = curlCode;
				resp.ErrorMsg = "";
				resp.Content = tStr;
			}
			else
			{
				resp.Ready = false;
				resp.ErrorMsg = GetErrorStr();
				resp.CURLCode = curlCode;
				resp.Content = "";
			}

			CURLCleanup();
			return resp;
		}

		~HTTP()
		{
		}

	private:
		CURL* curl;
		CURLcode curlCode;
		char errbuf[CURL_ERROR_SIZE];
		struct curl_slist* slist;
		curl_mime* form = nullptr;
		curl_mimepart* field = nullptr;

		bool followRedirect;
		size_t timeout;
		size_t maxRedirs;
		CookieContainer cookieContainer;
		string contentType;
		string accept;
		string userAgent;

		string GetErrorStr()
		{
			string errorMsg;
			size_t len = strlen(errbuf);
			if (len)
				errorMsg = errbuf;
			else
				errorMsg = curl_easy_strerror(curlCode);
			return errorMsg;
		}

		void CURLCleanup()
		{
			curl_easy_cleanup(curl);
			curl = nullptr;
			curl_slist_free_all(slist);
			slist = nullptr;
			curl_mime_free(form);
			form = nullptr;
		}

		static size_t reWriter(char* buffer, size_t size, size_t nmemb, string* content)
		{
			unsigned long sizes = size * nmemb;
			content->append(buffer, sizes);
			return sizes;
		}

		static size_t heWriter(char* buffer, size_t size, size_t nmemb, string* content)
		{
			unsigned long sizes = size * nmemb;
			content->append(buffer, sizes);
			return sizes;
		}

		void trim(string& s)
		{
			if (s.empty())
			{
				return;
			}
			s.erase(0, s.find_first_not_of(" "));
			s.erase(s.find_last_not_of(" ") + 1);
		}

		void AutoCookies(string& reHeader)
		{
			std::smatch match;
			std::regex pattern(R"(Set-Cookie:([^=]+)=([^;]+);?\s*)");
			while (std::regex_search(reHeader, match, pattern))
			{
				string key = match[1].str();
				string val = match[2].str();
				trim(key);
				trim(val);
				cookieContainer.insert(std::make_pair(key, val));
				reHeader = match.suffix().str();
			}
		}

		string execute(const string& URL)
		{
			string reStr;
			string reHeader;
			// 自定义HTTP头
			slist = curl_slist_append(slist, ("User-Agent: " + userAgent).data());
			slist = curl_slist_append(slist, ("Accept: " + accept).data());
			// curl基础设置
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
			curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, followRedirect);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)timeout);
			curl_easy_setopt(curl, CURLOPT_MAXREDIRS, (long)maxRedirs);
			curl_easy_setopt(curl, CURLOPT_COOKIE, cookieContainer.toString().data());
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
			/* enable TCP keep-alive for this transfer */
			curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
			/* keep-alive idle time to 120 seconds */
			curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 120L);
			/* interval time between keep-alive probes: 60 seconds */
			curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 60L);
			// curl 数据处理函数
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, reWriter);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&reStr);
			// 设置处理HTTP头部的功能函数
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, heWriter);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)&reHeader);
			curl_easy_setopt(curl, CURLOPT_URL, URL.data());
			curlCode = curl_easy_perform(curl);
			// 从HTTP头部读取新的Cookie并加入到CookieContainer中
			AutoCookies(reHeader);

			return reStr;
		}


	};
}// namespace Cyan