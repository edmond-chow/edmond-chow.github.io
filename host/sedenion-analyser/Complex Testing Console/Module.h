#pragma once
namespace Mod
{
	class MyModule final
	{
	private:
		constexpr MyModule() noexcept = delete;
		constexpr ~MyModule() noexcept = delete;
	public:
		static void Load() noexcept;
	};
}
