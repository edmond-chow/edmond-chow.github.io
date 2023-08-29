/* ============= */
/*               */
/*   Extension   */
/*               */
/* ============= */
#include <emscripten.h>
#include <emscripten/bind.h>
#include <cstdlib>
#include <string>
#include <sstream>
#include "Base.h"
struct evaluate_local
{
	std::jmp_buf stack_pointer;
	void(*caught)(const std::exception& ex);
};
static thread_local evaluate_local* local_ptr{ nullptr };
void evaluate(void(*operate)(), void(*caught)(const std::exception& ex))
{
	evaluate_local* pushed_local_ptr = local_ptr;
	evaluate_local local{{ 0 }, caught };
	if (setjmp(local.stack_pointer) == 0)
	{
		local_ptr = &local;
		operate();
	}
	local_ptr = pushed_local_ptr;
};
void throw_now(const std::exception& ex)
{
	if (local_ptr == nullptr) { std::terminate(); }
	local_ptr->caught(ex);
	std::longjmp(local_ptr->stack_pointer, 1);
};
namespace SedenConExt
{
	EM_ASYNC_JS(void, readWrapper, (), {
		iostream.registerReaded(await iostream.read());
	});
	EM_ASYNC_JS(void, pressAnyKeyWrapper, (), {
		await iostream.pressAnyKey();
	});
	EM_ASYNC_JS(void, suspendWrapper, (), {
		await defer(50);
	});
	inline std::string toMbsString(const std::wstring& string)
	{
		std::size_t length = string.size() * 5 + 1;
		char* temporary = new char[length] { '\0' };
		std::wcstombs(temporary, string.c_str(), length);
		std::string converted = temporary;
		delete[] temporary;
		return converted;
	};
	inline std::wstring toWcsString(const std::string& string)
	{
		std::size_t length = string.size() * 5 + 1;
		wchar_t* temporary = new wchar_t[length] { '\0' };
		std::mbstowcs(temporary, string.c_str(), length);
		std::wstring converted = temporary;
		delete[] temporary;
		return converted;
	};
	namespace dom
	{
		struct wcin_t {} wcin;
		struct wcout_t : std::wstringstream {} wcout;
		static std::size_t wcout_count = 0;
		inline void print(wcout_t& wcout)
		{
			wcout_count = 0;
			std::string call = "iostream.writeWithColorCodes('" + toMbsString(wcout.str()) + "')";
			emscripten_run_script(call.c_str()); 
			wcout.str(L"");
			suspendWrapper();
		};
		void getline(wcin_t&, std::wstring& line)
		{
			print(wcout);
			readWrapper();
			line = toWcsString(emscripten_run_script_string("iostream.resolveReaded()"));
		};
		wcout_t& endl(wcout_t& wcout)
		{
			wcout << L"\\n";
			if (++wcout_count > 100) { print(wcout); }
			return wcout;
		};
		template <typename T>
		wcout_t& operator <<(wcout_t& wcout, T o)
		{
			dynamic_cast<std::wstringstream&>(wcout) << o;
			return wcout;
		};
		inline wcout_t& operator <<(wcout_t& wcout, wcout_t&(*endl)(wcout_t&))
		{
			return endl(wcout);
		};
	}
	enum class ConsoleColor : std::uint8_t
	{
		Black = 0,
		DarkBlue = 1,
		DarkGreen = 2,
		DarkCyan = 3,
		DarkRed = 4,
		DarkMagenta = 5,
		DarkYellow = 6,
		Gray = 7,
		DarkGray = 8,
		Blue = 9,
		Green = 10,
		Cyan = 11,
		Red = 12,
		Magenta = 13,
		Yellow = 14,
		White = 15,
		Default = 16
	};
	static ConsoleColor ForegroundColor = ConsoleColor::DarkGray;
	static ConsoleColor BackgroundColor = ConsoleColor::Black;
	static std::wstring Title = L"";
	static const char* ConsoleColorList[] {
		"black",
		"dark-blue",
		"dark-green",
		"dark-cyan",
		"dark-red",
		"dark-magenta",
		"dark-yellow",
		"gray",
		"dark-gray",
		"blue",
		"green",
		"cyan",
		"red",
		"magenta",
		"yellow",
		"white",
		"default"
	};
	inline ConsoleColor toConsoleColor(const char* output)
	{
		for (std::size_t i = 0; i < std::extent_v<decltype(ConsoleColorList)>; ++i)
		{
			if (std::strcmp(output, ConsoleColorList[i]) == 0)
			{
				return static_cast<ConsoleColor>(i);
			}
		}
		return ConsoleColor::Default;
	};
	inline const char* toStringLiteral(ConsoleColor input)
	{
		return ConsoleColorList[static_cast<std::size_t>(input) % std::extent_v<decltype(ConsoleColorList)>];
	};
	ConsoleColor getForegroundColor()
	{
		return ForegroundColor;
	};
	ConsoleColor getBackgroundColor()
	{
		return BackgroundColor;
	};
	std::wstring getTitle()
	{
		return Title;
	};
	void setForegroundColor(ConsoleColor color)
	{
		ForegroundColor = color;
		dom::wcout << L"\\\\foreground:" << toStringLiteral(color) << L"\\\\";
	};
	void setBackgroundColor(ConsoleColor color)
	{
		BackgroundColor = color;
		dom::wcout << L"\\\\background:" << toStringLiteral(color) << L"\\\\";
	};
	void setTitle(const std::wstring& title)
	{
		Title = title;
		dom::wcout << L"\\\\title:" << title << L"\\\\";
	};
	void clear()
	{
		dom::print(dom::wcout);
		emscripten_run_script("iostream.clear()");
	};
	void pressAnyKey()
	{
		dom::print(dom::wcout);
		pressAnyKeyWrapper();
	};
}
int main()
{
	using namespace SedenionTestingConsole;
	using namespace SedenConExt;
	ForegroundColor = toConsoleColor(emscripten_run_script_string("iostream.getForegroundColor()"));
	BackgroundColor = toConsoleColor(emscripten_run_script_string("iostream.getBackgroundColor()"));
	Title = toWcsString(emscripten_run_script_string("getTitle()"));
	while (true)
	{
		Base::Main();
		dom::wcout << dom::endl;
		dom::wcout << L"   The program ended with a return code EXIT_SUCCESS successfully." << dom::endl;
		dom::wcout << dom::endl;
		dom::wcout << L"   >> Press any key to continue with restart the program . . .   " << dom::endl;
		pressAnyKey();
		Base::IsSwitchTo(L"[Octonion Testing Console]");
	}
	return EXIT_SUCCESS;
};
