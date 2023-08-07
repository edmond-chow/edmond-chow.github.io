#pragma once
namespace Mod3
{
	class MyModule3 final
	{
	private:
		constexpr MyModule3() noexcept = delete;
		constexpr ~MyModule3() noexcept = delete;
	public:
		static void Load() noexcept;
	};
}
