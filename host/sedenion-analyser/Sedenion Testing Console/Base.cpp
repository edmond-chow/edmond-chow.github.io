#include <string>
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
		static std::wstring AddSquares(const std::wstring& Option) { return L"[" + Option + L"]"; };
	public:
		static std::wstring GetTitle();
		static std::wstring GetStartupLine();
		static bool IsSwitchTo(const std::wstring& Option);
		///
		/// Main Thread
		///
		static void Main();
		///
		/// Console Line Materials
		///
		static std::wstring Exception(const std::exception& Exception);
		static std::wstring Exception();
		static std::wstring Selection(const std::wstring& Content);
		static std::wstring Selection();
		static std::wstring Input(const std::wstring& Content);
		static std::wstring Input();
		static std::wstring Output(const std::wstring& Preceding, const std::wstring& Content);
		static std::wstring Output(const std::wstring& Content);
		static std::wstring Output();
		static std::wstring Comment(const std::wstring& Preceding, const std::wstring& Content);
		static std::wstring Comment(const std::wstring& Content);
		static std::wstring Comment();
		static void Startup(const std::wstring& Title);
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
	bool Base::IsSwitchTo(const std::wstring& Option)
	{
		for (std::size_t i = 0; i < std::extent_v<decltype(TestingConsole)>; ++i)
		{
			if (Option == AddSquares(TestingConsole[i]))
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
				Clear();
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
				throw std::logic_error("The branch should ensure not instantiated at compile time.");
			}
		}
	};
	///
	/// Console Line Materials
	///
	std::wstring Base::Exception(const std::exception& Exception)
	{
		SetForegroundColor(ConsoleColor::DarkCyan);
		dom::wcout << std::endl << L"   [" << typeid(Exception).name() << L"] ";
		SetForegroundColor(ConsoleColor::Cyan);
		dom::wcout << Exception.what() << std::endl;
		SetForegroundColor(ConsoleColor::White);
		dom::wcout << L"   Press any key to continue . . .   " << std::endl;
		PressAnyKey();
		dom::wcout << std::endl;
		return L"";
	};
	std::wstring Base::Exception() { return Exception(std::exception()); };
	std::wstring Base::Selection(const std::wstring& Content)
	{
		SetForegroundColor(ConsoleColor::DarkCyan);
		dom::wcout << L" %   ";
		SetForegroundColor(ConsoleColor::Blue);
		dom::wcout << Content << std::endl;
		return Content;
	};
	std::wstring Base::Selection() { return Selection(L""); };
	std::wstring Base::Input(const std::wstring& Content)
	{
		SetForegroundColor(ConsoleColor::Yellow);
		dom::wcout << L" >   ";
		SetForegroundColor(ConsoleColor::DarkGreen);
		dom::wcout << Content;
		SetForegroundColor(ConsoleColor::Green);
		std::wstring output;
		std::getline(dom::wcin, output);
		return output;
	};
	std::wstring Base::Input() { return Input(L""); };
	std::wstring Base::Output(const std::wstring& Preceding, const std::wstring& Content)
	{
		SetForegroundColor(ConsoleColor::Magenta);
		dom::wcout << L" #   ";
		SetForegroundColor(ConsoleColor::DarkRed);
		dom::wcout << Preceding;
		SetForegroundColor(ConsoleColor::Red);
		dom::wcout << Content << std::endl;
		return Content;
	};
	std::wstring Base::Output(const std::wstring& Content) { return Output(L"", Content); };
	std::wstring Base::Output() { return Output(L""); };
	std::wstring Base::Comment(const std::wstring& Preceding, const std::wstring& Content)
	{
		SetForegroundColor(ConsoleColor::White);
		dom::wcout << L" /   ";
		SetForegroundColor(ConsoleColor::Cyan);
		dom::wcout << Preceding;
		SetForegroundColor(ConsoleColor::Gray);
		dom::wcout << Content << std::endl;
		return Content;
	};
	std::wstring Base::Comment(const std::wstring& Content) { return Comment(L"", Content); };
	std::wstring Base::Comment() { return Comment(L""); };
	void Base::Startup(const std::wstring& Title)
	{
		Clear();
		SetTitle(Title);
		dom::wcout << std::endl;
	};
}
