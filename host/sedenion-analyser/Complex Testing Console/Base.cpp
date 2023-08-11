﻿#include <csetjmp>
#include <string>
#include "Module.h"
#include "Module2.h"
#include "Module3.h"
#include "SedenionMod.h"
#include "Base Extension.h"
thread_local jmp_buf stack_pointer;
using namespace CmplxConExt;
namespace ComplexTestingConsole
{
	class Base final
	{
	private:
		constexpr Base() noexcept = delete;
		constexpr ~Base() noexcept = delete;
		///
		/// Base
		///
		static constexpr const wchar_t* TestingConsole[] { L"Exit", L"Complex Testing Console", L"Quaternion Testing Console", L"Octonion Testing Console" };
		static constexpr const wchar_t* SedenionModeConsole[] { L"SedenionMode", L"Sedenion", L"Pathion", L"Chingon", L"Routon", L"Voudon" };
		static std::size_t Index;
	public:
		static std::wstring GetTitle();
		static std::wstring GetStartupLine();
		static std::wstring GetSedenTitle();
		static bool IsSwitchTo(const std::wstring& Str);
		///
		/// Main Thread
		///
		static void Main();
		///
		/// Console Line Materials
		///
		static std::wstring Exception(const std::wstring& type, const std::wstring& what);
		static std::wstring Selection(const std::wstring& str);
		static std::wstring Selection();
		static std::wstring Input(const std::wstring& str);
		static std::wstring Input();
		static std::wstring Output(const std::wstring& main, const std::wstring& str);
		static std::wstring Output(const std::wstring& str);
		static std::wstring Output();
		static std::wstring Comment(const std::wstring& head, const std::wstring& str);
		static std::wstring Comment(const std::wstring& str);
		static std::wstring Comment();
		static void Startup(const std::wstring& title);
	};
	///
	/// Base
	///
	std::size_t Base::Index = std::extent_v<decltype(Base::TestingConsole)> - 1;
	std::wstring Base::GetTitle() { return Index != std::extent_v<decltype(Base::TestingConsole)> ? TestingConsole[Index] : SedenionModeConsole[0]; };
	std::wstring Base::GetStartupLine()
	{
		std::wstring Output = L" >> ";
		for (const wchar_t* Name : TestingConsole)
		{
			if (std::wstring(Name) == TestingConsole[0]) { continue; }
			Output.append(L"[").append(Name).append(L"]   ");
		}
		return Output.append(L"[").append(SedenionModeConsole[0]).append(L"]");
	};
	std::wstring Base::GetSedenTitle()
	{
		std::wstring Output;
		for (std::size_t i = 1; i < std::extent_v<decltype(Base::SedenionModeConsole)>; ++i)
		{
			Output += SedenionModeConsole[i];
			Output += L", ";
		}
		return Output += L"...";
	};
	bool Base::IsSwitchTo(const std::wstring& Str)
	{
		for (std::size_t s = 0; s < std::extent_v<decltype(TestingConsole)> + 1; ++s)
		{
			if (Str == std::wstring{}.append(L"[").append(s != std::extent_v<decltype(Base::TestingConsole)> ? TestingConsole[s] : SedenionModeConsole[0]).append(L"]"))
			{
				Index = s;
				return true;
			}
		}
		return false;
	};
	///
	/// Main Thread
	///
	void Base::Main()
	{
		while (true)
		{
			switch (Index)
			{
			case 0:
				clear();
				return;
			case 1:
				Mod::MyModule::Load();
				break;
			case 2:
				Mod2::MyModule2::Load();
				break;
			case 3:
				Mod3::MyModule3::Load();
				break;
			case 4:
				SedenionMod::MySedenionTestor::Load();
				break;
			default:
				throw;
			}
		}
	};
	///
	/// Console Line Materials
	///
	std::wstring Base::Exception(const std::wstring& type, const std::wstring& what)
	{
		setForegroundColor(ConsoleColor::DarkCyan);
		writeLine(L"");
		write(L"   [");
		write(type);
		write(L"] ");
		setForegroundColor(ConsoleColor::Cyan);
		writeLine(what);
		setForegroundColor(ConsoleColor::White);
		writeLine(L"   Press any key to continue . . .   ");
		pressAnyKey();
		writeLine(L"");
		return L"";
	};
	std::wstring Base::Selection(const std::wstring& str)
	{
		setForegroundColor(ConsoleColor::DarkCyan);
		write(L" %   ");
		setForegroundColor(ConsoleColor::Blue);
		writeLine(str);
		return str;
	};
	std::wstring Base::Selection() { return Selection(L""); };
	std::wstring Base::Input(const std::wstring& str)
	{
		setForegroundColor(ConsoleColor::Yellow);
		write(L" >   ");
		setForegroundColor(ConsoleColor::DarkGreen);
		write(str);
		setForegroundColor(ConsoleColor::Green);
		read();
		return resolveReaded();
	};
	std::wstring Base::Input() { return Input(L""); };
	std::wstring Base::Output(const std::wstring& main, const std::wstring& str)
	{
		setForegroundColor(ConsoleColor::Magenta);
		write(L" #   ");
		setForegroundColor(ConsoleColor::DarkRed);
		write(main);
		setForegroundColor(ConsoleColor::Red);
		writeLine(str);
		return str;
	};
	std::wstring Base::Output(const std::wstring& str) { return Output(L"", str); };
	std::wstring Base::Output() { return Output(L""); };
	std::wstring Base::Comment(const std::wstring& head, const std::wstring& str)
	{
		setForegroundColor(ConsoleColor::White);
		write(L" /   ");
		setForegroundColor(ConsoleColor::Cyan);
		write(head);
		setForegroundColor(ConsoleColor::Gray);
		writeLine(str);
		return str;
	};
	std::wstring Base::Comment(const std::wstring& str) { return Comment(L"", str); };
	std::wstring Base::Comment() { return Comment(L""); };
	void Base::Startup(const std::wstring& title)
	{
		clear();
		setTitle(title);
		writeLine(L"");
	};
}
void throw_now(std::wstring&& type, std::wstring&& what)
{
	ComplexTestingConsole::Base::Exception(type, what);
	std::longjmp(stack_pointer, 1);
};
