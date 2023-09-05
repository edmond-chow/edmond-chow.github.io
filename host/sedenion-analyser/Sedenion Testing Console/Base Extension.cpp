/* ============= */
/*               */
/*   Extension   */
/*               */
/* ============= */
#include <emscripten.h>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <sstream>
#include <stdexcept>
#include "Base.h"
namespace SedenConExt
{
	namespace Native
	{
		EM_ASYNC_JS(const char*, ReadSync, (), {
			return getUTF8String(__asyncjs__ReadSync, await iostream.writeLine(await iostream.read()));
		});
		EM_ASYNC_JS(void, WriteSync, (const char* Content), {
			await iostream.writeWithColorCodes(UTF8ToString(Content));
		});
		EM_ASYNC_JS(void, ClearSync, (), {
			await iostream.clear();
		});
		EM_ASYNC_JS(void, PressAnyKeySync, (), {
			await iostream.pressAnyKey();
		});
		EM_JS(std::uint8_t, GetSyncForeground, (), {
			return iostream.getForegroundColor().toConsoleColor();
		});
		EM_JS(std::uint8_t, GetSyncBackground, (), {
			return iostream.getBackgroundColor().toConsoleColor();
		});
		EM_JS(const char*, GetSyncTitle, (), {
			return getUTF8String(GetSyncTitle, getTitle());
		});
		EM_JS(const char*, GetString, (std::uint8_t Color), {
			return getUTF8String(GetString, Color.fromConsoleColor());
		});
		EM_ASYNC_JS(void, ReloadSync, (), {
			await iostream.reload(0);
		});
	}
	inline std::string ToMbsString(const std::wstring& String)
	{
		std::size_t length = String.size() * 2 + 1;
		char* temporary = new char[length] { '\0' };
		std::wcstombs(temporary, String.c_str(), length);
		std::string converted = temporary;
		delete[] temporary;
		return converted;
	};
	inline std::wstring ToWcsString(const std::string& String)
	{
		std::size_t length = String.size() + 1;
		wchar_t* temporary = new wchar_t[length] { L'\0' };
		std::mbstowcs(temporary, String.c_str(), length);
		std::wstring converted = temporary;
		delete[] temporary;
		return converted;
	};
	enum class ConsoleColor : std::uint8_t
	{
		Default = 0xff,
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
	};
	std::wstring ReadSync()
	{
		return ToWcsString(Native::ReadSync());
	};
	void WriteSync(const std::wstring& Content)
	{
		std::string temporary = ToMbsString(Content);
		Native::WriteSync(temporary.c_str());
	};
	void ClearSync()
	{
		Native::ClearSync();
	};
	void PressAnyKeySync()
	{
		Native::PressAnyKeySync();
	};
	ConsoleColor GetSyncForeground()
	{
		return static_cast<ConsoleColor>(Native::GetSyncForeground());
	};
	ConsoleColor GetSyncBackground()
	{
		return static_cast<ConsoleColor>(Native::GetSyncBackground());
	};
	std::wstring GetSyncTitle()
	{
		return ToWcsString(Native::GetSyncTitle());
	};
	std::wstring GetString(ConsoleColor Color)
	{
		return ToWcsString(Native::GetString(static_cast<std::uint8_t>(Color)));
	};
	void ReloadSync() {
		Native::ReloadSync();
	};
	namespace dom
	{
		struct wcin_t {} wcin;
		struct wcout_t : std::wstringstream {} wcout;
		static std::size_t wcout_count = 0;
		inline void print(wcout_t& wcout)
		{
			wcout_count = 0;
			WriteSync(wcout.str());
			wcout.str(L"");
		};
		void getline(wcin_t&, std::wstring& line)
		{
			print(wcout);
			line = ReadSync();
		};
		wcout_t& endl(wcout_t& wcout)
		{
			wcout << L"\n";
			if (++wcout_count >= 0xff) { print(wcout); }
			return wcout;
		};
		template <typename T>
		wcout_t& operator <<(wcout_t& wcout, T o)
		{
			dynamic_cast<std::wstringstream&>(wcout) << o;
			return wcout;
		};
		inline wcout_t& operator <<(wcout_t& wcout, wcout_t& (*endl)(wcout_t&))
		{
			return endl(wcout);
		};
	}
	static ConsoleColor ForegroundColor{ GetSyncForeground() };
	static ConsoleColor BackgroundColor{ GetSyncBackground() };
	static std::wstring Title{ GetSyncTitle() };
	ConsoleColor GetForegroundColor()
	{
		return ForegroundColor;
	};
	ConsoleColor GetBackgroundColor()
	{
		return BackgroundColor;
	};
	std::wstring GetTitle()
	{
		return Title;
	};
	void SetForegroundColor(ConsoleColor Color)
	{
		ForegroundColor = Color;
		dom::wcout << L"\\foreground:" << GetString(Color) << L"\\";
	};
	void SetBackgroundColor(ConsoleColor Color)
	{
		BackgroundColor = Color;
		dom::wcout << L"\\background:" << GetString(Color) << L"\\";
	};
	void SetTitle(const std::wstring& Text)
	{
		Title = Text;
		dom::wcout << L"\\title:" << Text << L"\\";
	};
	void Clear()
	{
		dom::print(dom::wcout);
		ClearSync();
	};
	void PressAnyKey()
	{
		dom::print(dom::wcout);
		PressAnyKeySync();
	};
}
int main()
{
	while (true)
	{
		SedenionTestingConsole::Base::Main();
		SedenConExt::ReloadSync();
	}
	return EXIT_SUCCESS;
};
