#include <Evaluation.h>
#include <csetjmp>
#include <string>
#include <iostream>
#include <stdexcept>
#include "Module.h"
#include "Module2.h"
#include "Module3.h"
#include "SedenionMod.h"
#include "Base Extension.h"
using namespace SedenConExt;
namespace SedenionTestingConsole
{
	class Base final
	{
	private:
		constexpr Base() noexcept = delete;
		constexpr ~Base() noexcept = delete;
		///
		/// Base
		///
		static constexpr const wchar_t* TestingConsole[] { L"Exit", L"Complex Testing Console", L"Quaternion Testing Console", L"Octonion Testing Console", L"SedenionMode" };
		static constexpr const std::size_t DefaultIndex = 3;
		static std::size_t Index;
		static std::wstring AddSquares(const std::wstring& Str) { return L"[" + Str + L"]"; };
	public:
		static std::wstring GetTitle();
		static std::wstring GetStartupLine();
		static bool IsSwitchTo(const std::wstring& Str);
		///
		/// Main Thread
		///
		static void Main();
		///
		/// Console Line Materials
		///
		static std::wstring Exception(const std::exception& ex);
		static std::wstring Exception();
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
	std::size_t Base::Index = DefaultIndex;
	std::wstring Base::GetTitle()
	{
		return Index > DefaultIndex ? L"SedenionMode (Sedenion, Pathion, Chingon, Routon, Voudon, ...)" : TestingConsole[Index];
	};
	std::wstring Base::GetStartupLine()
	{
		std::wstring Output = L" >> ";
		for (std::size_t i = 1; i < std::extent_v<decltype(TestingConsole)>; ++i)
		{
			Output.append(AddSquares(TestingConsole[i])).append(L"   ");
		}
		return Output.substr(0, Output.length() - 3);
	};
	bool Base::IsSwitchTo(const std::wstring& Str)
	{
		for (std::size_t i = 0; i < std::extent_v<decltype(TestingConsole)>; ++i)
		{
			if (Str == AddSquares(TestingConsole[i]))
			{
				Index = i;
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
				Index = DefaultIndex;
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
				throw_now(std::logic_error("The branch should ensure not instantiated at compile time."));
			}
		}
	};
	///
	/// Console Line Materials
	///
	std::wstring Base::Exception(const std::exception& ex)
	{
		setForegroundColor(ConsoleColor::DarkCyan);
		dom::wcout << dom::endl << L"   [" << typeid(ex).name() << L"] ";
		setForegroundColor(ConsoleColor::Cyan);
		dom::wcout << ex.what() << dom::endl;
		setForegroundColor(ConsoleColor::White);
		dom::wcout << L"   Press any key to continue . . .   " << dom::endl;
		pressAnyKey();
		dom::wcout << dom::endl;
		return L"";
	};
	std::wstring Base::Exception() { return Exception(std::exception()); };
	std::wstring Base::Selection(const std::wstring& str)
	{
		setForegroundColor(ConsoleColor::DarkCyan);
		dom::wcout << L" %   ";
		setForegroundColor(ConsoleColor::Blue);
		dom::wcout << str << dom::endl;
		return str;
	};
	std::wstring Base::Selection() { return Selection(L""); };
	std::wstring Base::Input(const std::wstring& str)
	{
		setForegroundColor(ConsoleColor::Yellow);
		dom::wcout << L" >   ";
		setForegroundColor(ConsoleColor::DarkGreen);
		dom::wcout << str;
		setForegroundColor(ConsoleColor::Green);
		std::wstring output;
		dom::getline(dom::wcin, output);
		return output;
	};
	std::wstring Base::Input() { return Input(L""); };
	std::wstring Base::Output(const std::wstring& main, const std::wstring& str)
	{
		setForegroundColor(ConsoleColor::Magenta);
		dom::wcout << L" #   ";
		setForegroundColor(ConsoleColor::DarkRed);
		dom::wcout << main;
		setForegroundColor(ConsoleColor::Red);
		dom::wcout << str << dom::endl;
		return str;
	};
	std::wstring Base::Output(const std::wstring& str) { return Output(L"", str); };
	std::wstring Base::Output() { return Output(L""); };
	std::wstring Base::Comment(const std::wstring& head, const std::wstring& str)
	{
		setForegroundColor(ConsoleColor::White);
		dom::wcout << L" /   ";
		setForegroundColor(ConsoleColor::Cyan);
		dom::wcout << head;
		setForegroundColor(ConsoleColor::Gray);
		dom::wcout << str << dom::endl;
		return str;
	};
	std::wstring Base::Comment(const std::wstring& str) { return Comment(L"", str); };
	std::wstring Base::Comment() { return Comment(L""); };
	void Base::Startup(const std::wstring& title)
	{
		clear();
		setTitle(title);
		dom::wcout << dom::endl;
	};
}
