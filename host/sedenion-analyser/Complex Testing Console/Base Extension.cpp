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
#include <streambuf>
#include <iostream>
#include "Base.h"
namespace CmplxConExt
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
	}
	inline std::string ToMbsString(const std::wstring& String)
	{
		std::size_t Length = String.size() * sizeof(wchar_t) + 1;
		char* Temporary = new char[Length] { '\0' };
		std::wcstombs(Temporary, String.c_str(), Length);
		std::string Converted = Temporary;
		delete[] Temporary;
		return Converted;
	};
	inline std::wstring ToWcsString(const std::string& String)
	{
		std::size_t Length = String.size() + 1;
		wchar_t* Temporary = new wchar_t[Length] { L'\0' };
		std::mbstowcs(Temporary, String.c_str(), Length);
		std::wstring Converted = Temporary;
		delete[] Temporary;
		return Converted;
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
	namespace dom
	{
		struct client_t : public std::wstreambuf
		{
		private:
			enum class state
			{
				write = 1,
				freeze = 2,
				read = 3,
			};
			state state;
			std::wstringstream out;
			std::size_t out_l;
			std::wstring in_buf;
			std::wstring::const_iterator in_cur;
			wchar_t in_pop;
		public:
			client_t() : state{ state::freeze }, out{}, out_l{}, in_buf{}, in_cur{}, in_pop{} {};
		protected:
			virtual int sync() override
			{
				if (state == state::read) { return -1; }
				if (state == state::write)
				{
					WriteSync(out.str());
					out.str(L"");
					out_l = 0;
					state = state::freeze;
				}
				return 0;
			};
		public:
			int send()
			{
				state = state::write;
				return sync();
			};
		protected:
			virtual int_type overflow(int_type c) override
			{
				if (out_l >= 1024) { send(); }
				if (c == L'\n') { ++out_l; }
				out << std::char_traits<wchar_t>::to_char_type(c);
				return c;
			};
			virtual int_type underflow() override
			{
				if (state == state::write || state == state::freeze)
				{
					send();
					in_buf = ReadSync();
					in_cur = in_buf.cbegin();
					state = state::read;
				}
				setg(&in_pop, &in_pop, &in_pop + 1);
				if (in_cur == in_buf.cend())
				{
					in_pop = L'\n';
					state = state::freeze;
				}
				else
				{
					in_pop = *in_cur;
					++in_cur;
				}
				return std::char_traits<wchar_t>::to_int_type(in_pop);
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
		dom::client.send();
		ClearSync();
	};
	void PressAnyKey()
	{
		dom::client.send();
		PressAnyKeySync();
	};
}
int main()
{
	ComplexTestingConsole::Base::Main();
	return EXIT_SUCCESS;
};
