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
	EM_ASYNC_JS(const char*, readWrapper, (), {
		return getUTF8String(__asyncjs__readWrapper, await iostream.registerReaded(await iostream.read()));
	});
	EM_ASYNC_JS(void, clearWrapper, (), {
		await iostream.clear();
	});
	EM_ASYNC_JS(void, pressAnyKeyWrapper, (), {
		await iostream.pressAnyKey();
	});
	EM_ASYNC_JS(void, writeWithColorCodesWrapper, (const char* str), {
		await iostream.writeWithColorCodes(UTF8ToString(str));
	});
	EM_JS(std::uint8_t, getInitForegroundColorWrapper, (), {
		return iostream.getForegroundColor().toConsoleColor();
	});
	EM_JS(std::uint8_t, getInitBackgroundColorWrapper, (), {
		return iostream.getBackgroundColor().toConsoleColor();
	});
	EM_JS(const char*, getInitTitleWrapper, (), {
		return getUTF8String(getInitTitleWrapper, getTitle());
	});
	EM_JS(const char*, ToStringLiteralWrapper, (std::uint8_t Color), {
		return getUTF8String(ToStringLiteralWrapper, Color.fromConsoleColor());
	});
	inline std::string ToMbsString(const std::wstring& string)
	{
		std::size_t length = string.size() * 5 + 1;
		char* temporary = new char[length] { '\0' };
		std::wcstombs(temporary, string.c_str(), length);
		std::string converted = temporary;
		delete[] temporary;
		return converted;
	};
	inline std::wstring ToWcsString(const std::string& string)
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
			std::string call = ToMbsString(wcout.str());
			writeWithColorCodesWrapper(call.c_str());
			wcout.str(L"");
		};
		void getline(wcin_t&, std::wstring& line)
		{
			print(wcout);
			line = ToWcsString(readWrapper());
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
		inline wcout_t& operator <<(wcout_t& wcout, wcout_t&(*endl)(wcout_t&))
		{
			return endl(wcout);
		};
	}
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
	static ConsoleColor ForegroundColor{ ConsoleColor::Gray };
	static ConsoleColor BackgroundColor{ ConsoleColor::Default };
	static std::wstring Title{ L"" };
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
		dom::wcout << L"\\foreground:" << ToStringLiteralWrapper(static_cast<std::uint8_t>(Color)) << L"\\";
	};
	void SetBackgroundColor(ConsoleColor Color)
	{
		BackgroundColor = Color;
		dom::wcout << L"\\background:" << ToStringLiteralWrapper(static_cast<std::uint8_t>(Color)) << L"\\";
	};
	void SetTitle(const std::wstring& Text)
	{
		Title = Text;
		dom::wcout << L"\\title:" << Text << L"\\";
	};
	void Clear()
	{
		dom::print(dom::wcout);
		clearWrapper();
	};
	void PressAnyKey()
	{
		dom::print(dom::wcout);
		pressAnyKeyWrapper();
	};
	struct initiator_t
	{
	private:
		static const initiator_t initiator;
		initiator_t()
		{
			ForegroundColor = static_cast<ConsoleColor>(getInitForegroundColorWrapper());
			BackgroundColor = static_cast<ConsoleColor>(getInitBackgroundColorWrapper());
			Title = ToWcsString(getInitTitleWrapper());
		};
	};
	const initiator_t initiator_t::initiator{};
}
EM_ASYNC_JS(void, operateExitWrapper, (), {
	await iostream.operateExit(0);
});
int main()
{
	while (true)
	{
		SedenionTestingConsole::Base::Main();
		operateExitWrapper();
	}
	return EXIT_SUCCESS;
};
