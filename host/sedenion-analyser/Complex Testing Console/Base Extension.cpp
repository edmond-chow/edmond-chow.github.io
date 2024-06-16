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
				return Console.GetColorCode(Module['iostream'].ForegroundColor);
			});
			EM_JS(std::uint8_t, background, (), {
				return Console.GetColorCode(Module['iostream'].BackgroundColor);
			});
			EM_JS(const wchar_t*, title, (), {
				return Module['getUTF32String'](title, Console.Title);
			});
			EM_JS(std::char_traits<wchar_t>::int_type, color_char_code, (std::uint8_t code), {
				return Console.GetColorCharCode(code);
			});
			EM_ASYNC_JS(const wchar_t*, read_line, (), {
				let line = await Module['iostream'].readLine();
				await Module['iostream'].writeLine(line, false);
				return Module['getUTF32String'](__asyncjs__read_line, line);
			});
			EM_ASYNC_JS(void, write_code, (const wchar_t* content), {
				await Module['iostream'].write(UTF32ToString(content));
			});
			EM_ASYNC_JS(void, press_any_key, (), {
				await Module['iostream'].pressAnyKey();
			});
			EM_JS(void, clear, (), {
				Module['iostream'].clear();
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
			static const wchar_t* read_line()
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
				static constexpr const int out_sz = 1048576;
				static constexpr const int in_sz = 1024;
				static constexpr const int back_sz = 32;
				char_type out[out_sz + 1];
				char_type* out_last;
				char_type in[back_sz + in_sz];
				const char_type* in_next;
				bool in_slash;
				bool flush_now;
				explicit builder() : out{}, out_last{ out }, in{}, in_next{ nullptr }, in_slash{ false }, flush_now{ false }
				{
					this->setp(out, out + out_sz);
				};
				friend class stream;
			};
			builder builder;
		public:
			explicit stream() : builder{}
			{
				this->setg(builder.in, builder.in + builder::back_sz, builder.in + builder::back_sz);
			};
		protected:
			virtual int sync() override
			{
				char_type* pptr = builder.pptr();
				if (pptr == builder.out + builder::out_sz || builder.flush_now)
				{
					char_type out_char = *builder.out_last;
					*builder.out_last = L'\0';
					console::write_code(builder.out);
					*builder.out_last = out_char;
					std::copy(builder.out_last, pptr, builder.out);
					builder.pbump(builder.out - builder.out_last);
					builder.out_last = builder.out;
					builder.flush_now = false;
				}
				return 0;
			};
		public:
			int send()
			{
				builder.flush_now = true;
				return this->sync();
			};
		protected:
			virtual int_type overflow(int_type ch) override
			{
				this->sync();
				if (ch != traits_type::eof())
				{
					char_type* pptr = builder.pptr();
					*pptr++ = traits_type::to_char_type(ch);
					if (traits_type::to_char_type(ch) == L'\\')
					{
						if (builder.in_slash) { builder.out_last = pptr; }
						builder.in_slash = !builder.in_slash;
					}
					else if (traits_type::to_char_type(ch) == L'\n')
					{
						builder.in_slash = false;
						builder.out_last = pptr;
					}
					else if (builder.in_slash == false) { builder.out_last = pptr; }
					builder.pbump(1);
				}
				return ch;
			};
			virtual int_type underflow() override
			{
				char_type* gptr = this->gptr();
				if (gptr == this->egptr())
				{
					char_type* nw_eback = builder.in;
					std::copy(gptr - builder::back_sz, gptr, nw_eback);
					if (builder.in_next == nullptr)
					{
						this->send();
						builder.in_next = console::read_line();
					}
					const char_type* nw_next = builder.in_next;
					while (nw_next < builder.in_next + builder::in_sz && *nw_next != L'\0') { ++nw_next; }
					char_type* nw_gptr = nw_eback + builder::back_sz;
					std::copy(builder.in_next, nw_next, nw_gptr);
					int nw_offset = nw_next - builder.in_next;
					if (*nw_next == L'\0')
					{
						nw_gptr[nw_offset++] = L'\n';
						nw_next = nullptr;
					}
					char_type* nw_egptr = nw_gptr + nw_offset;
					this->setg(nw_eback, nw_gptr, nw_egptr);
					builder.in_next = nw_next;
					gptr = nw_gptr;
				}
				return traits_type::to_int_type(*gptr);
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
