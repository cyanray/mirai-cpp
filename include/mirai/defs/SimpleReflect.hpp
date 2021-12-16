#pragma once
#include <string>
#include <map>
#include <functional>
#include <memory>
using std::string;

namespace Cyan
{
	template<class BaseClass>
	class Reflection
	{
	public:

		template<typename T>
		void Register(const string& type_name)
		{
			auto func = [](void)
			{
				std::shared_ptr<T> e = std::make_shared<T>();
				return std::dynamic_pointer_cast<BaseClass>(e);
			};
			obj_map.insert({ type_name, func });
		}

		std::shared_ptr<BaseClass> DynamicCreate(const string& typeName)
		{
			auto& t = obj_map[typeName];
			return (t == nullptr) ? nullptr : t();
		}

		size_t size() const
		{
			return obj_map.size();
		}
	private:
		using ReflectedObjectPointer = std::shared_ptr<BaseClass>;
		std::map<string, std::function<ReflectedObjectPointer(void)>> obj_map;
	};
}


