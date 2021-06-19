#pragma
#ifndef __CYAN_CPP_PROPERTY_hpp_H_
#define __CYAN_CPP_PROPERTY_hpp_H_

namespace Cyan
{
	template<typename T>
	class Property
	{
	public:
		Property() : value_() {}

		template <typename ...Args>
		Property(Args && ...args) : value_(std::forward<Args>(args)...)
		{
		}

		T& operator*()
		{
			return Get();
		}

		T* operator->()
		{
			return &Get();
		}

		template<typename U>
		Property<T>& operator=(U&& v)
		{
			Set(std::forward<U>(v));
			return *this;
		}

		const T& Get() const
		{
			return value_;
		}

		T& Get()
		{
			return value_;
		}

		template<typename U>
		void Set(U&& value)
		{
			value_ = value;
		}

	private:
		T value_;
	};
}

#endif // !__CYAN_CPP_PROPERTY_hpp_H_