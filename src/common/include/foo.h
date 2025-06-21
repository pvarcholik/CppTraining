#pragma once

#include <cstdint>
#include <string>

namespace CppTraining
{
	class Foo
	{
	public:
		explicit Foo(std::int32_t data = 0);
		Foo(const Foo& rhs);
		Foo(Foo&& rhs) noexcept;
		Foo& operator=(const Foo& rhs);
		Foo& operator=(Foo&& rhs) noexcept;
		virtual ~Foo();

		bool operator==(const Foo& rhs) const;
		bool operator!=(const Foo& rhs) const;

		std::int32_t getData() const;
		void setData(std::int32_t data);

	private:
		std::int32_t* data_;
	};

  std::string to_string(const Foo& value);
}
