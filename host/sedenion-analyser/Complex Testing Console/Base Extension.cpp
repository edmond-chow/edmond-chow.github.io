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
namespace CmplxConExt
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
	struct wcout_t : std::wstringstream
	{
	public:
		static constexpr const std::size_t max_count = 100;
	private:
		std::size_t count;
	public:
		wcout_t() : count(0) {};
		std::size_t add()
		{
			return count < max_count ? ++count : count;
		};
		std::size_t reset()
		{
			return count = 0;
		};
	};
	wcout_t wcout;
	wcout_t& endl(wcout_t& wcout)
	{
		wcout << L"\\n";
		return wcout;
	};
	inline void output_now(wcout_t& wcout)
	{
		wcout.reset();
		std::string call = "iostream.writeWithColorCodes('" + toMbsString(wcout.str()) + "')";
		emscripten_run_script(call.c_str()); 
		wcout.str(L"");
	};
	void output(wcout_t& wcout)
	{
		if (wcout.add() == wcout_t::max_count)
		{
			suspendWrapper();
			output_now(wcout);
		}
	};
	template <typename T>
	wcout_t& operator <<(wcout_t& wcout, T o)
	{
		dynamic_cast<std::wstringstream&>(wcout) << o;
		output(wcout);
		return wcout;
	};
	inline wcout_t& operator <<(wcout_t& wcout, wcout_t&(*endl)(wcout_t& wcout))
	{
		return endl(wcout);
	};
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
		wcout << L"\\\\foreground:" << toStringLiteral(color) << L"\\\\";
	};
	void setBackgroundColor(ConsoleColor color)
	{
		BackgroundColor = color;
		wcout << L"\\\\background:" << toStringLiteral(color) << L"\\\\";
	};
	void setTitle(const std::wstring& title)
	{
		Title = title;
		wcout << L"\\\\title:" << title << L"\\\\";
	};
	void clear()
	{
		output_now(wcout);
		emscripten_run_script("iostream.clear()");
	};
	void pressAnyKey()
	{
		output_now(wcout);
		pressAnyKeyWrapper();
	};
	void read()
	{
		output_now(wcout);
		readWrapper();
	};
	std::wstring resolveReaded()
	{
		return toWcsString(emscripten_run_script_string("iostream.resolveReaded()"));
	};
}
extern "C" int main()
{
	using namespace ComplexTestingConsole;
	using namespace CmplxConExt;
	ForegroundColor = toConsoleColor(emscripten_run_script_string("iostream.getForegroundColor()"));
	BackgroundColor = toConsoleColor(emscripten_run_script_string("iostream.getBackgroundColor()"));
	Title = toWcsString(emscripten_run_script_string("getTitle()"));
	while (true)
	{
		Base::Main();
		wcout << endl << L"The program ended with a return code EXIT_SUCCESS successfully." << endl << endl << L"   >> Press any key to continue with restart the program . . .   " << endl;
		pressAnyKey();
		Base::IsSwitchTo(L"[Octonion Testing Console]");	
	}
	return EXIT_SUCCESS;
};
