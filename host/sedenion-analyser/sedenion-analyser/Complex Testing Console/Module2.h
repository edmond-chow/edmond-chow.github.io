#pragma once
namespace Mod2
{
	class MyModule2 final
	{
	private:
		constexpr MyModule2() noexcept = delete;
		constexpr ~MyModule2() noexcept = delete;
	public:
		static void Load() noexcept;
	};
}
