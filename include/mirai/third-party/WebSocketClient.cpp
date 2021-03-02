#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <thread>
#include <regex>
#include "WebSocketClient.h"
using namespace std;


#if defined(_WIN32)

#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

#else

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>

#endif

typedef unsigned char byte_t;

#if defined(_WIN32)
typedef SOCKET socket_t;
#else
typedef int socket_t;
#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR   (-1)
#endif
#define closesocket(s) ::close(s)
#endif

namespace cyanray
{
	socket_t hostname_connect(const std::string& hostname, int port) {
		struct addrinfo* result;

		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		int ret = getaddrinfo(hostname.c_str(), to_string(port).c_str(), &hints, &result);
		if (ret != 0)
		{
			throw std::runtime_error("getaddrinfo failed.");
		}

		socket_t sockfd = INVALID_SOCKET;
		for (struct addrinfo* p = result; p != NULL; p = p->ai_next)
		{
			sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
			if (sockfd == INVALID_SOCKET) { continue; }
			if (connect(sockfd, p->ai_addr, (int)p->ai_addrlen) != SOCKET_ERROR)
			{
				break;
			}
			closesocket(sockfd);
			sockfd = INVALID_SOCKET;
		}
		freeaddrinfo(result);
		return sockfd;
	}

	enum class WebSocketOpcode
	{
		Continuation = 0x00,
		Text = 0x01,
		Binary = 0x02,
		Close = 0x08,
		Ping = 0x09,
		Pong = 0x0A
	};

	struct WebSocketClient::pimpl
	{
		socket_t wsSocket = INVALID_SOCKET;
		std::thread recvLoop;

		void Send(WebSocketOpcode opcode)
		{
			array<byte_t, 6> frame_data = { 0x00, 0x80, 0x00, 0x00, 0x00, 0x00 };
			frame_data[0] = 0x80 | (byte_t)opcode;
			int sendResult = send(wsSocket, (char*)frame_data.data(), (int)frame_data.size(), 0);
			if (sendResult == SOCKET_ERROR)
			{
				throw std::runtime_error("socket error (send).");
			}
		}

		template<class Iterator>
		void Send(WebSocketOpcode opcode, Iterator begin, Iterator end)
		{
			size_t length = end - begin;
			vector<byte_t> frame_data;
			frame_data.reserve(2048);	// TODO: replace with a const variable
			frame_data.push_back(0x80 | (byte_t)opcode);	// Text Frame
			// MASK must be 1
			if (length <= 125)
			{
				frame_data.push_back(0x80 | (byte_t)length);
			}
			else if (length <= (std::numeric_limits<uint16_t>::max)())
			{
				uint16_t t = (uint16_t)length;
				frame_data.push_back(0xFE);
				frame_data.push_back((t >> 8) & 0xFF);
				frame_data.push_back((t >> 0) & 0xFF);
			}
			else if (length <= (std::numeric_limits<uint64_t>::max)())
			{
				uint64_t t = (uint64_t)length;
				frame_data.push_back(0xFF);
				frame_data.push_back((t >> 56) & 0xFF);
				frame_data.push_back((t >> 48) & 0xFF);
				frame_data.push_back((t >> 40) & 0xFF);
				frame_data.push_back((t >> 32) & 0xFF);
				frame_data.push_back((t >> 24) & 0xFF);
				frame_data.push_back((t >> 16) & 0xFF);
				frame_data.push_back((t >> 8) & 0xFF);
				frame_data.push_back((t >> 0) & 0xFF);
			}
			else
			{
				throw std::runtime_error("Data is too large. Does not support fragmentation.");
			}

			std::array<byte_t, 4> mask_key{ 0xd2, 0x28, 0xb6, 0xde };
			frame_data.insert(frame_data.end(), mask_key.begin(), mask_key.end());

			int i = 0;
			for (auto it = begin; it != end; ++it)
			{
				frame_data.push_back(*it ^ mask_key[i++ % 4]);
			}

			int sendResult = send(wsSocket, (char*)frame_data.data(), (int)frame_data.size(), 0);
			if (sendResult == SOCKET_ERROR)
			{
				throw std::runtime_error("socket error (send).");
			}

		}
	};

	struct FrameInfo
	{
		bool Fin = false;
		bool Mask = false;
		WebSocketOpcode Opcode = WebSocketOpcode::Text;
		byte_t MaskKey[4] = { };
		uint64_t PayloadLength = 0;
	};

	int TryParseFrame(FrameInfo* frame_info, const byte_t* frame_data, size_t len)
	{
		if (len < 2) return -1;
		int offset = 2;
		FrameInfo& info = *frame_info;
		info.Fin = (frame_data[0] & 0x80) == 0x80;
		info.Mask = (frame_data[1] & 0x80) == 0x80;
		info.Opcode = static_cast<WebSocketOpcode>(frame_data[0] & 0x0F);
		info.PayloadLength = frame_data[1] & 0x7F;
		if (info.PayloadLength == 126)
		{
			if (len < 4) return -1;
			info.PayloadLength = (frame_data[2] << 8) | frame_data[3];
			offset = 4;
		}
		else if (info.PayloadLength == 127)
		{
			if (len < 10) return -1;
			memcpy(&info.PayloadLength, &frame_data[2], sizeof(uint64_t));
			offset = 10;
		}
		if (info.Mask)
		{
			memcpy(&info.MaskKey, &frame_data[offset], sizeof(info.MaskKey));
			offset += 4;
		}
		return offset;
	}

	WebSocketClient::WebSocketClient() : PrivateMembers(new pimpl()), status(Status::Closed)
	{
#if defined(_WIN32)
		WSAData data;
		int WsResult = WSAStartup(MAKEWORD(2, 2), &data);
		if (WsResult != 0)
		{
			throw std::runtime_error("Can't start winsock.");
		}
#endif
	}
	
	WebSocketClient::~WebSocketClient()
	{
		delete PrivateMembers;
#if defined(_WIN32)
		WSACleanup();
#endif
	}

	void WebSocketClient::Connect(const string& ws_uri)
	{
		regex pattern(R"(ws:\/\/([^:\/]+):?(\d+)?(\/[\S]*)?)");
		std::smatch matches;
		std::regex_search(ws_uri, matches, pattern);
		if (matches.size() == 0)
		{
			throw std::runtime_error("Unable to parse websocket uri.");
		}
		else
		{
			return Connect(matches[1].str(),
				matches[2].matched ? std::stoi(matches[2].str()) : 80,
				matches[3].matched ? matches[3].str() : "/");
		}
	}

	void WebSocketClient::Connect(const string& hostname, int port, const string& path)
	{
		PrivateMembers->wsSocket = hostname_connect(hostname, port);
		if (PrivateMembers->wsSocket == INVALID_SOCKET)
		{
			throw std::runtime_error("Unable to connect to " + hostname);
		}

		string handshakingWords;
		handshakingWords.append("GET ").append(path).append(" HTTP/1.1").append("\r\n");
		handshakingWords.append("Host: ").append(hostname).append(":").append(to_string(port)).append("\r\n");
		handshakingWords.append("Connection: Upgrade").append("\r\n");
		handshakingWords.append("Upgrade: websocket").append("\r\n");
		handshakingWords.append("Origin: http://example.com").append("\r\n");
		handshakingWords.append("Sec-WebSocket-Version: 13").append("\r\n");
		handshakingWords.append("Sec-WebSocket-Key: O7Tk4xI04v+X91cuvefLSQ==").append("\r\n");
		handshakingWords.append("\r\n");

		int sendResult = send(PrivateMembers->wsSocket, handshakingWords.c_str(), (int)handshakingWords.size(), 0);
		if (sendResult == SOCKET_ERROR)
		{
			throw std::runtime_error("An error occurred during the handshake.");
		}

		char response_buffer[4096] = { 0 };
		int bytesReceived = recv(PrivateMembers->wsSocket, response_buffer, 4096, 0);
		// TODO: parse Response

		status = Status::Open;
		PrivateMembers->recvLoop = std::thread([this]() {RecvLoop(); });
		PrivateMembers->recvLoop.detach();
	}

	void WebSocketClient::Shutdown()
	{
		if (status == Status::Closed) return;
		status = Status::Closed;
		closesocket(PrivateMembers->wsSocket);
	}

	void WebSocketClient::OnTextReceived(Callback<string> callback)
	{
		TextReceivedCallback = callback;
	}

	void WebSocketClient::OnBinaryReceived(Callback<vector<uint8_t>> callback)
	{
		BinaryReceivedCallback = callback;
	}

	void WebSocketClient::OnError(Callback<string> callback)
	{
		ErrorCallback = callback;
	}

	void WebSocketClient::OnLostConnection(Callback<int> callback)
	{
		LostConnectionCallback = callback;
	}

	void WebSocketClient::SendText(const string& text)
	{
		if (status == Status::Closed)
			throw std::runtime_error("WebSocket is closed.");
		PrivateMembers->Send(WebSocketOpcode::Text, text.begin(), text.end());
	}

	void WebSocketClient::SendBinary(const char* data, size_t length)
	{
		if (status == Status::Closed)
			throw std::runtime_error("WebSocket is closed.");
		PrivateMembers->Send(WebSocketOpcode::Binary, data, data + length);
	}

	void WebSocketClient::SendBinary(const uint8_t* data, size_t length)
	{
		if (status == Status::Closed)
			throw std::runtime_error("WebSocket is closed.");
		PrivateMembers->Send(WebSocketOpcode::Binary, data, data + length);
	}

	void WebSocketClient::Ping()
	{
		if (status == Status::Closed) return;
		PrivateMembers->Send(WebSocketOpcode::Ping);
	}

	void WebSocketClient::Pong()
	{
		if (status == Status::Closed) return;
		PrivateMembers->Send(WebSocketOpcode::Pong);
	}

	void WebSocketClient::Pong(const vector<uint8_t>& data)
	{
		if (status == Status::Closed) return;
		PrivateMembers->Send(WebSocketOpcode::Pong, data.begin(), data.end());
	}

	void WebSocketClient::Close()
	{
		if (status == Status::Closed) return;
		status = Status::Closing;
		PrivateMembers->Send(WebSocketOpcode::Close);
	}

	void WebSocketClient::RecvLoop()
	{
		socket_t sock = PrivateMembers->wsSocket;
		vector<byte_t> buffer;
		buffer.reserve(8192);

		fd_set fds_read;
		FD_ZERO(&fds_read);

		struct timeval tv;
		while (status == Status::Open)
		{
			tv.tv_sec = 0;
			tv.tv_usec = 200 * 1000;
			FD_SET(sock, &fds_read);
			int ret = select((int)(sock + 1), &fds_read, NULL, NULL, &tv);
			if (ret < 0)
			{
				if (ErrorCallback != nullptr)
				{
					ErrorCallback(*this, "select error.");
				}
			}
			if (ret != 0 && FD_ISSET(sock, &fds_read))
			{
				array<char, 2048>  buf = {};
				int bytesReceived = recv(sock, buf.data(), (int)buf.size(), 0);
				if (bytesReceived > 0)
				{
					buffer.insert(buffer.end(), buf.begin(), buf.begin() + bytesReceived);
				}
				else
				{
					// If close socket actively(shutdown), should not call the callback function.
					if (status == Status::Open)
					{
						this->Shutdown();
						if (LostConnectionCallback != nullptr)
						{
							LostConnectionCallback(*this, 1006);
						}
					}
					break;
				}
			}

			while (buffer.size())
			{
				FrameInfo info;
				int offset = TryParseFrame(&info, buffer.data(), buffer.size());
				if (offset < 0)
				{
					if (ErrorCallback != nullptr)
					{
						ErrorCallback(*this, "Failed to parse frame.");
					}
					break;
				}
				else
				{
					if (buffer.size() >= offset + info.PayloadLength)
					{
						auto payload_start = buffer.begin() + offset;
						auto payload_end = buffer.begin() + (offset + info.PayloadLength);
						if (info.Mask)
						{
							int i = 0;
							for (auto it = payload_start; it != payload_end; ++it)
							{
								*it = *it ^ info.MaskKey[i++ % 4];
							}
						}
						if (info.Opcode == WebSocketOpcode::Text)
						{
							string text = string(payload_start, payload_end);
							if (TextReceivedCallback != nullptr)
							{
								TextReceivedCallback(*this, text);
							}
						}
						else if (info.Opcode == WebSocketOpcode::Binary)
						{
							if (BinaryReceivedCallback != nullptr)
							{
								BinaryReceivedCallback(*this, vector<uint8_t>(payload_start, payload_end));
							}
						}
						else if (info.Opcode == WebSocketOpcode::Ping)
						{
							try
							{
								Pong(vector<uint8_t>(payload_start, payload_end));
							}
							catch (const std::exception& ex)
							{
								if (ErrorCallback != nullptr)
								{
									ErrorCallback(*this, string("An error occurs on sending pong frame. error: ")+ ex.what());
								}
							}
						}
						else if (info.Opcode == WebSocketOpcode::Close)
						{
							if (status == Status::Closing)
							{
								Shutdown();
							}
							else if (status == Status::Open)
							{
								// close frame from server, response a close frame and close socket.
								Close();
								Shutdown();
								if (LostConnectionCallback != nullptr)
								{
									LostConnectionCallback(*this, 1000);
								}
							}
							return;
						}
						else
						{
							if (ErrorCallback != nullptr)
							{
								ErrorCallback(*this, 
									string("The opcode #")
										.append(to_string((int)info.Opcode)
										.append(" is not supported.") ));

							}
						}
						buffer.erase(buffer.begin(), buffer.begin() + (offset + info.PayloadLength));
					}
					else
					{
						break;
					}
				}
			}
		}

	}


}


