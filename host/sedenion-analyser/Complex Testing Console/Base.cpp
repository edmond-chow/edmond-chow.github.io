#include <csetjmp>
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
		static constexpr const wchar_t* TestingConsole[] { L"Exit", L"Complex Testing Console", L"Quaternion Testing Console", L"Octonion Testing Console", L"SedenionMode" };
		static constexpr const std::size_t DefaultIndex = 3;
		static std::size_t Index;
		static std::wstring AddSquares(const std::wstring& Str) { return L"[" + Str + L"]"; };
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
		wcout << endl << L"   [" << type << L"] ";
		setForegroundColor(ConsoleColor::Cyan);
		wcout << what << endl;
		setForegroundColor(ConsoleColor::White);
		wcout << L"   Press any key to continue . . .   " << endl;
		pressAnyKey();
		wcout << endl;
		return L"";
	};
	std::wstring Base::Selection(const std::wstring& str)
	{
		setForegroundColor(ConsoleColor::DarkCyan);
		wcout << L" %   ";
		setForegroundColor(ConsoleColor::Blue);
		wcout << str << endl;
		return str;
	};
	std::wstring Base::Selection() { return Selection(L""); };
	std::wstring Base::Input(const std::wstring& str)
	{
		setForegroundColor(ConsoleColor::Yellow);
		wcout << L" >   ";
		setForegroundColor(ConsoleColor::DarkGreen);
		wcout << str;
		setForegroundColor(ConsoleColor::Green);
		read();
		return resolveReaded();
	};
	std::wstring Base::Input() { return Input(L""); };
	std::wstring Base::Output(const std::wstring& main, const std::wstring& str)
	{
		setForegroundColor(ConsoleColor::Magenta);
		wcout << L" #   ";
		setForegroundColor(ConsoleColor::DarkRed);
		wcout << main;
		setForegroundColor(ConsoleColor::Red);
		wcout << str << endl;
		return str;
	};
	std::wstring Base::Output(const std::wstring& str) { return Output(L"", str); };
	std::wstring Base::Output() { return Output(L""); };
	std::wstring Base::Comment(const std::wstring& head, const std::wstring& str)
	{
		setForegroundColor(ConsoleColor::White);
		wcout << L" /   ";
		setForegroundColor(ConsoleColor::Cyan);
		wcout << head;
		setForegroundColor(ConsoleColor::Gray);
		wcout << str << endl;
		return str;
	};
	std::wstring Base::Comment(const std::wstring& str) { return Comment(L"", str); };
	std::wstring Base::Comment() { return Comment(L""); };
	void Base::Startup(const std::wstring& title)
	{
		clear();
		setTitle(title);
		wcout << endl;
	};
}
void throw_now(std::wstring&& type, std::wstring&& what)
{
	ComplexTestingConsole::Base::Exception(type, what);
	std::longjmp(stack_pointer, 1);
};
