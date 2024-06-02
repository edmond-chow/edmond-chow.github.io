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
			EM_JS(wchar_t*, title, (), {
				return getUTF32String(title, getTitle());
			});
			EM_JS(std::char_traits<wchar_t>::int_type, color_char_code, (std::uint8_t code), {
				return Console.GetColorCharCode(code);
			});
			EM_ASYNC_JS(wchar_t*, read_line, (), {
				let line = await iostream.readLine();
				await iostream.writeLine(line, false);
				return getUTF32String(__asyncjs__read_line, line);
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
		struct console
		{
		private:
			explicit console(const console&) = delete;
			explicit console(console&&) = delete;
			explicit console() = delete;
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
			static wchar_t* read_line()
			{
				return native::read_line();
			};
			static void write_code(const wchar_t* content)
			{
				return native::write_code(content);
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
		struct stream : public std::wstreambuf
		{
		private:
			struct builder : public std::wstreambuf
			{
			public:
				static constexpr const std::size_t out_sz = 1048576;
				char_type out[out_sz];
				char_type* out_last;
				bool end_slash;
				bool can_sync;
				explicit builder(std::size_t out_reserved) : out{}, out_last{ nullptr }, end_slash{ false }, can_sync{ false }
				{
					if (out_reserved > out_sz) { out_reserved = out_sz; }
					setp(out, out + out_sz - out_reserved);
				};
				char_type* out_cur() const
				{
					return pptr();
				};
				void out_jmp(int offset)
				{
					pbump(offset);
				};
			};
			builder builder;
		public:
			explicit stream() : builder{ 1 } {};
		protected:
			virtual int sync() override
			{
				char_type* out_cur = builder.out_cur();
				if (builder.can_sync || out_cur == builder.out + builder::out_sz)
				{
					if (builder.out_last == nullptr) { builder.out_last = out_cur; }
					else { ++builder.out_last; }
					char_type out_last_poped = *builder.out_last;
					*builder.out_last = L'\0';
					console::write_code(builder.out);
					*builder.out_last = out_last_poped;
					std::copy(builder.out_last, out_cur, builder.out);
					builder.out_jmp(builder.out - out_cur);
					builder.out_last = nullptr;
					builder.end_slash = false;
					builder.can_sync = false;
				}
				return 0;
			};
		public:
			int send()
			{
				builder.can_sync = true;
				return sync();
			};
		protected:
			virtual int_type overflow(int_type ch) override
			{
				sync();
				if (ch != traits_type::eof())
				{
					char_type* out_cur = builder.out_cur();
					*out_cur = traits_type::to_char_type(ch);
					if (traits_type::to_char_type(ch) == L'\n')
					{
						builder.out_last = out_cur;
						builder.end_slash = false;
					}
					else if (traits_type::to_char_type(ch) == L'\\')
					{
						if (builder.end_slash) { builder.out_last = out_cur; }
						builder.end_slash = !builder.end_slash;
					}
					builder.out_jmp(1);
				}
				return ch;
			};
			virtual int_type underflow() override
			{
				send();
				if (gptr() == egptr())
				{
					char_type* in = console::read_line();
					std::size_t in_sz = wcslen(in);
					setg(in, in, in + in_sz + 1);
					in[in_sz] = L'\n';
				}
				return traits_type::to_int_type(*gptr());
			};
		};
		static stream io{};
		std::wistream wcin{ &io };
		std::wostream wcout{ &io };
	}
	ConsoleColor GetForegroundColor()
	{
		return static_cast<ConsoleColor>(*dom::console::foreground());
	};
	ConsoleColor GetBackgroundColor()
	{
		return static_cast<ConsoleColor>(*dom::console::background());
	};
	std::wstring GetTitle()
	{
		return *dom::console::title();
	};
	void SetForegroundColor(ConsoleColor Color)
	{
		*dom::console::foreground() = static_cast<std::uint8_t>(Color);
		dom::wcout << L"\\f" << dom::console::color_char_code(static_cast<std::uint8_t>(Color)) << L"\\";
	};
	void SetBackgroundColor(ConsoleColor Color)
	{
		*dom::console::background() = static_cast<std::uint8_t>(Color);
		dom::wcout << L"\\b" << dom::console::color_char_code(static_cast<std::uint8_t>(Color)) << L"\\";
	};
	void SetTitle(const std::wstring& Text)
	{
		*dom::console::title() = Text;
		dom::wcout << L"\\t" << Text << L"\\";
	};
	void PressAnyKey()
	{
		dom::io.send();
		dom::console::press_any_key();
	};
	void Clear()
	{
		dom::io.send();
		dom::console::clear();
	};
}
int main()
{
	ComplexTestingConsole::Base::Main();
	return EXIT_SUCCESS;
};
