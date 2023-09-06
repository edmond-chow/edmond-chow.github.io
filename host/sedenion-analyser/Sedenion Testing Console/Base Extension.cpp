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
			let line = await iostream.read();
			await iostream.writeLine(line);
			return getUTF8String(__asyncjs__ReadSync, line);
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
	void ReloadSync()
	{
		Native::ReloadSync();
	};
	namespace dom
	{
		struct client_t : public std::wstreambuf
		{
		private:
			enum class state
			{
				write = 0,
				read = 1,
			};
			state state;
			std::wstringstream output;
			std::size_t output_lines;
			std::wstring input;
			std::wstring::const_iterator input_current;
			std::wstring::const_iterator input_end;
			wchar_t input_popped;
		public:
			client_t() : state{ state::write }, output{}, output_lines{}, input{}, input_current{}, input_end{}, input_popped{} {};
		protected:
			virtual int sync() override
			{
				if (state == state::read) { return -1; }
				WriteSync(output.str());
				output.str(L"");
				output_lines = 0;
				return 0;
			};
			virtual int_type overflow(int_type c) override
			{
				if (output_lines >= 0xff) { sync(); }
				if (c == L'\n') { ++output_lines; }
				output << static_cast<wchar_t>(c);
				return c;
			};
			virtual int_type underflow() override
			{
				if (state == state::write)
				{
					sync();
					input = ReadSync();
					input_current = input.cbegin();
					input_end = input.cend();
					state = state::read;
				}
				setg(&input_popped, &input_popped, &input_popped + 1);
				if (input_current == input_end)
				{
					input_popped = L'\n';
					state = state::write;
				}
				else
				{
					input_popped = *input_current;
					++input_current;
				}
				return static_cast<int_type>(input_popped);
			};
		};
		static client_t client{};
		std::wistream wcin{ &client };
		std::wostream wcout{ &client };
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
		dom::client.pubsync();
		ClearSync();
	};
	void PressAnyKey()
	{
		dom::client.pubsync();
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
