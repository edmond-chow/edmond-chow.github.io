#pragma once
namespace SedenionMod
{
	class MySedenionTestor final
	{
	private:
		constexpr MySedenionTestor() noexcept = delete;
		constexpr ~MySedenionTestor() noexcept = delete;
	public:
		static void Load() noexcept;
	};
}
