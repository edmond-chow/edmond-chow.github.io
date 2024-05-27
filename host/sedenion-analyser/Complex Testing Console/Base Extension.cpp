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
	namespace dom
	{
		namespace native
		{
			EM_JS(std::uint8_t, foreground, (), {
				return iostream.getForegroundColor().toConsoleColor();
			});
			EM_JS(std::uint8_t, background, (), {
				return iostream.getBackgroundColor().toConsoleColor();
			});
			EM_JS(const wchar_t*, title, (), {
				return getUTF32String(title, getTitle());
			});
			EM_JS(std::char_traits<wchar_t>::int_type, color_char_code, (std::uint8_t code), {
				return Console.GetColorCharCode(code);
			});
			EM_ASYNC_JS(const wchar_t*, read_line, (), {
				let line = await iostream.readLine();
				await iostream.writeLine(line, false);
				return getUTF32String(__asyncjs__read, line);
			});
			EM_ASYNC_JS(void, write_code, (const wchar_t* content), {
				await iostream.write(UTF32ToString(content));
			});
			EM_ASYNC_JS(void, press_any_key, (), {
				await iostream.pressAnyKey();
			});
			EM_JS(void, clear, (), {
				iostream.clear();
			});
		}
		struct js_console
		{
		private:
			explicit js_console(const js_console&) = delete;
			explicit js_console(js_console&&) = delete;
			explicit js_console() = delete;
		public:
			static wchar_t color_char_code(std::uint8_t code)
			{
				return std::char_traits<wchar_t>::to_char_type(native::color_char_code(code));
			};
			static std::uint8_t* foreground()
			{
				static std::uint8_t data = native::foreground();
				return &data;
			};
			static std::uint8_t* background()
			{
				static std::uint8_t data = native::background();
				return &data;
			};
			static std::wstring* title()
			{
				static std::wstring data = native::title();
				return &data;
			};
			static std::wstring read_line()
			{
				return native::read_line();
			};
			static void write_code(const std::wstring& content)
			{
				return native::write_code(content.c_str());
			};
			static void press_any_key()
			{
				return native::press_any_key();
			};
			static void clear()
			{
				native::clear();
			};
		};
		struct client_stream : public std::wstreambuf
		{
		private:
			static constexpr const std::size_t out_lm = 32768;
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
					js_console::write_code(out.str());
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
					in = js_console::read_line();
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
	ConsoleColor GetForegroundColor()
	{
		return static_cast<ConsoleColor>(*dom::js_console::foreground());
	};
	ConsoleColor GetBackgroundColor()
	{
		return static_cast<ConsoleColor>(*dom::js_console::background());
	};
	std::wstring GetTitle()
	{
		return *dom::js_console::title();
	};
	void SetForegroundColor(ConsoleColor Color)
	{
		*dom::js_console::foreground() = static_cast<std::uint8_t>(Color);
		dom::wcout << L"\\f" << dom::js_console::color_char_code(static_cast<std::uint8_t>(Color)) << L"\\";
	};
	void SetBackgroundColor(ConsoleColor Color)
	{
		*dom::js_console::background() = static_cast<std::uint8_t>(Color);
		dom::wcout << L"\\b" << dom::js_console::color_char_code(static_cast<std::uint8_t>(Color)) << L"\\";
	};
	void SetTitle(const std::wstring& Text)
	{
		*dom::js_console::title() = Text;
		dom::wcout << L"\\t" << Text << L"\\";
	};
	void PressAnyKey()
	{
		dom::client.send();
		dom::js_console::press_any_key();
	};
	void Clear()
	{
		dom::client.send();
		dom::js_console::clear();
	};
}
int main()
{
	ComplexTestingConsole::Base::Main();
	return EXIT_SUCCESS;
};
