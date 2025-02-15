/*
 *   Copyright 2022 Edmond Chow
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
/* ============= */
/*               */
/*   Extension   */
/*               */
/* ============= */
#include <emscripten.h>
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
			static constexpr const std::ptrdiff_t put_sz = 5113;
			static constexpr const std::ptrdiff_t get_sz = 2041;
			static constexpr const std::ptrdiff_t bak_sz = 1017;
			struct builder : public std::wstreambuf
			{
			public:
				char_type put[put_sz + 1];
				char_type get[bak_sz + get_sz + 1];
				char_type* put_lst;
				const char_type* get_nxt;
				bool put_confg_beg;
				bool put_flush_now;
				explicit builder()
					: put{}, get{}, put_lst{ put }, get_nxt{ nullptr }, put_confg_beg{ false }, put_flush_now{ false }
				{
					this->setp(put, put + put_sz);
				};
				friend class stream;
			};
			builder bd;
		public:
			explicit stream()
				: bd{}, std::wstreambuf{}
			{
				this->setg(bd.get, bd.get + bak_sz, bd.get + bak_sz);
			};
		protected:
			virtual int sync() override
			{
				char_type* pptr = bd.pptr();
				if (pptr == bd.put + put_sz || bd.put_flush_now)
				{
					char_type put_ch = *bd.put_lst;
					*bd.put_lst = L'\0';
					console::write_code(bd.put);
					*bd.put_lst = put_ch;
					std::copy(bd.put_lst, pptr, bd.put);
					bd.pbump(bd.put - bd.put_lst);
					bd.put_lst = bd.put;
					bd.put_flush_now = false;
				}
				return 0;
			};
		public:
			int send()
			{
				bd.put_flush_now = true;
				return this->sync();
			};
		protected:
			virtual int_type overflow(int_type ch) override
			{
				this->sync();
				if (ch != traits_type::eof())
				{
					char_type* pptr = bd.pptr();
					char_type wc = traits_type::to_char_type(ch);
					*pptr++ = wc;
					if (wc == L'\\')
					{
						if (bd.put_confg_beg) { bd.put_lst = pptr; }
						bd.put_confg_beg = !bd.put_confg_beg;
					}
					else if (wc == L'\n')
					{
						bd.put_confg_beg = false;
						bd.put_lst = pptr;
					}
					else if (!bd.put_confg_beg) { bd.put_lst = pptr; }
					bd.pbump(1);
				}
				return ch;
			};
			virtual int_type underflow() override
			{
				char_type* gptr = this->gptr();
				if (gptr == this->egptr())
				{
					std::copy(gptr - bak_sz, gptr, bd.get);
					gptr = bd.get + bak_sz;
					if (bd.get_nxt == nullptr)
					{
						this->send();
						bd.get_nxt = console::read_line();
					}
					const char_type* nw_nxt = bd.get_nxt;
					while (nw_nxt < bd.get_nxt + get_sz && *nw_nxt != L'\0') { ++nw_nxt; }
					std::copy(bd.get_nxt, nw_nxt, gptr);
					std::ptrdiff_t nw_off = nw_nxt - bd.get_nxt;
					if (*nw_nxt == L'\0')
					{
						gptr[nw_off++] = L'\n';
						nw_nxt = nullptr;
					}
					this->setg(bd.get, gptr, gptr + nw_off);
					bd.get_nxt = nw_nxt;
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
	void SetForegroundColor(ConsoleColor Col)
	{
		*dom::console::foreground() = static_cast<std::uint8_t>(Col);
		dom::wcout << L"\\f" << dom::console::color_char_code(static_cast<std::uint8_t>(Col)) << L"\\";
	};
	void SetBackgroundColor(ConsoleColor Col)
	{
		*dom::console::background() = static_cast<std::uint8_t>(Col);
		dom::wcout << L"\\b" << dom::console::color_char_code(static_cast<std::uint8_t>(Col)) << L"\\";
	};
	void SetTitle(const std::wstring& Tle)
	{
		*dom::console::title() = Tle;
		dom::wcout << L"\\t" << Tle << L"\\";
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
