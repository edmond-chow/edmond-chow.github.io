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
#include "Base.h"
namespace CmplxConExt
{
	namespace Native
	{
		EM_ASYNC_JS(const wchar_t*, ReadSync, (), {
			let line = await iostream.readLine();
			await iostream.writeLine(line, false);
			return getUTF32String(__asyncjs__ReadSync, line);
		});
		EM_ASYNC_JS(void, WriteSync, (const wchar_t* Content), {
			await iostream.write(UTF32ToString(Content));
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
		EM_JS(const wchar_t*, GetSyncTitle, (), {
			return getUTF32String(GetSyncTitle, getTitle());
		});
		EM_JS(std::char_traits<wchar_t>::int_type, GetColorCharCode, (std::uint8_t code), {
			return Console.GetColorCharCode(code);
		});
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
	std::wstring ReadSync()
	{
		return Native::ReadSync();
	};
	void WriteSync(const std::wstring& Content)
	{
		Native::WriteSync(Content.c_str());
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
		return Native::GetSyncTitle();
	};
	std::char_traits<wchar_t>::char_type GetColorCharCode(std::uint8_t code)
	{
		return std::char_traits<wchar_t>::to_char_type(Native::GetColorCharCode(code));
	};
	namespace dom
	{
		struct client_stream : public std::wstreambuf
		{
		private:
			static constexpr const std::size_t out_lm = 1024;
			enum class state
			{
				write = 1,
				freeze = 2,
				read = 3,
			};
			state state;
			std::wstringstream out;
			std::size_t out_l;
			std::wstring in;
			std::wstring::const_iterator in_cur;
			wchar_t in_pop;
		public:
			client_stream() : state{ state::freeze }, out{}, out_l{}, in{}, in_cur{}, in_pop{} {};
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
				if (state == state::freeze) { state = state::write; }
				return sync();
			};
		protected:
			virtual int_type overflow(int_type c) override
			{
				if (state == state::read) { return std::char_traits<wchar_t>::eof(); }
				if (out_l >= out_lm) { send(); }
				if (c == std::char_traits<wchar_t>::to_int_type(L'\n')) { ++out_l; }
				out << std::char_traits<wchar_t>::to_char_type(c);
				return c;
			};
			virtual int_type underflow() override
			{
				send();
				if (state == state::freeze)
				{
					in = ReadSync();
					in_cur = in.cbegin();
					state = state::read;
				}
				setg(&in_pop, &in_pop, &in_pop + 1);
				if (in_cur == in.cend())
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
		static client_stream client{};
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
		dom::wcout << L"\\f" << GetColorCharCode(static_cast<std::uint8_t>(Color)) << L"\\";
	};
	void SetBackgroundColor(ConsoleColor Color)
	{
		BackgroundColor = Color;
		dom::wcout << L"\\b" << GetColorCharCode(static_cast<std::uint8_t>(Color)) << L"\\";
	};
	void SetTitle(const std::wstring& Text)
	{
		Title = Text;
		dom::wcout << L"\\t" << Text << L"\\";
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
