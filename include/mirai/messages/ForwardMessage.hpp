#pragma once
#ifndef mirai_cpp_defs_messages_ForwardMessage_hpp_H_
#define mirai_cpp_defs_messages_ForwardMessage_hpp_H_
#include <vector>
#include <optional>
#include "mirai/defs/IMessage.hpp"
#include "mirai/defs/QQType.hpp"
#include "mirai/defs/MessageChain.hpp"
using std::optional;

namespace Cyan
{
	class ForwardMessage : public IMessage
	{
	public:

		class Node : ISerializable
		{
		public:
			int64_t Timestamp;
			QQ_t SenderId;
			string SenderName;
			std::optional<MessageId_t> MessageId;
			MessageChain_t MessageChain;

			bool operator==(const Node& node) const
			{
				if (Timestamp != node.Timestamp)
				{
					return false;
				}
				if (SenderId != node.SenderId)
				{
					return false;
				}
				if (SenderName != node.SenderName)
				{
					return false;
				}
				if (MessageId != node.MessageId)
				{
					return false;
				}
				if (MessageChain != node.MessageChain)
				{
					return false;
				}
				return true;
			}

			virtual bool Set(const json& json) override
			{
				auto& messageIdJson = json["messageId"];
				if (!messageIdJson.is_null())
				{
					MessageId = messageIdJson.get<int64_t>();
				}
				SenderId = QQ_t(json["senderId"].get<int64_t>());
				Timestamp = json["time"].get<int64_t>();
				SenderName = json["senderName"].get<string>();
				MessageChain.Set(json["messageChain"]);
				return true;
			}

			virtual json ToJson() const override
			{
				json result =
				{
					{ "senderId", SenderId.ToInt64() },
					{ "time", Timestamp },
					{ "senderName", SenderName },
					{ "messageChain", MessageChain.ToJson() }
				};
				if (MessageId.has_value())
				{
					result["messageId"] = MessageId.value();
				}
				else
				{
					result["messageId"] = nullptr;
				}
				return result;
			}
		};

		virtual const string& GetType() const override
		{
			return type_;
		}

		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const ForwardMessage*>(&m))
			{
				return m_ptr->nodes_ == this->nodes_;
			}
			return false;
		}

		virtual bool operator!=(const IMessage& m) const override
		{
			return !(*this == m);
		}

		virtual bool Set(const json& json) override
		{
			if (json["type"].is_null() || json["type"].get<string>() != this->GetType())
			{
				throw std::runtime_error("给定的json不正确");
			}
			auto& nodeListJson = json["nodeList"];
			if (!nodeListJson.is_array())
			{
				throw std::runtime_error("给定的json不正确");
			}
			for (int i = 0; i < nodeListJson.size(); ++i)
			{
				Node node;
				node.Set(nodeListJson[i]);
				nodes_.emplace_back(node);
			}
			return true;
		}

		virtual json ToJson() const override
		{
			json nodeListJson = json::array();
			for (const auto& n : nodes_)
			{
				nodeListJson.push_back(n.ToJson());
			}
			return
			{
				{ "type", type_ },
				{ "nodeList", nodeListJson }
			};
		}

		virtual ~ForwardMessage() {}

		const std::vector<Node>& NodeList() const { return nodes_; };
		std::vector<Node>& NodeList() { return nodes_; };

		void NodeList(const std::vector<Node>& nodeList) { nodes_ = nodeList; };

	private:
		const string type_ = "Forward";
		std::vector<Node> nodes_;
	};

}
#endif

