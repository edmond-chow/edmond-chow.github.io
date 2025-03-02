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
				return Console.GetColorCharFromCode(code);
			});
			EM_ASYNC_JS(void, write_controlized, (const wchar_t* content), {
				await Module['iostream'].write(UTF32ToString(content), true);
			});
			EM_ASYNC_JS(const wchar_t*, read_echoing, (), {
				let line = await Module['iostream'].readLine(true);
				return Module['getUTF32String'](__asyncjs__read_echoing, line);
			});
			EM_ASYNC_JS(void, press_any_key, (), {
				await Module['iostream'].readKey(false);
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
			static void write_controlized(const wchar_t* content)
			{
				return native::write_controlized(content);
			};
			static const wchar_t* read_echoing()
			{
				return native::read_echoing();
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
			static constexpr const std::ptrdiff_t put_sz = 5000;
			static constexpr const std::ptrdiff_t get_sz = 2000;
			static constexpr const std::ptrdiff_t bak_sz = 1000;
			char_type put[put_sz + 1];
			char_type get[bak_sz + get_sz + 1];
			char_type* put_buf_lst;
			const char_type* get_stm_nxt;
			bool put_flush_now;
		public:
			explicit stream()
				: put{}, get{}, put_buf_lst{}, get_stm_nxt{}, put_flush_now{}, std::wstreambuf{}
			{
				this->setp(put, put + put_sz);
				this->setg(get, get + bak_sz, get + bak_sz);
				put_buf_lst = put;
			};
		public:
			void send()
			{
				char_type* put_ptr = this->pptr();
				char_type* put_try = put_buf_lst;
				bool put_confg_beg = false;
				while (put_try < put_ptr)
				{
					if (*put_try == L'\\')
					{
						put_confg_beg = !put_confg_beg;
						if (put_confg_beg) { put_buf_lst = put_try; }
					}
					else if (*put_try == L'\n')
					{
						put_confg_beg = false;
						put_buf_lst = put_try;
					}
					else if (!put_confg_beg) { put_buf_lst = put_try; }
					++put_try;
				}
				put_flush_now = true;
				this->sync();
			};
		protected:
			virtual int sync() override
			{
				char_type* put_ptr = this->pptr();
				if (put_ptr == put + put_sz || put_flush_now)
				{
					char_type put_ch = *put_buf_lst;
					*put_buf_lst = L'\0';
					console::write_controlized(put);
					*put_buf_lst = put_ch;
					std::copy(put_buf_lst, put_ptr, put);
					this->pbump(put - put_buf_lst);
					put_buf_lst = put;
					put_flush_now = false;
				}
				return 0;
			};
		protected:
			virtual int_type overflow(int_type ch) override
			{
				this->send();
				if (ch != traits_type::eof())
				{
					char_type wc = traits_type::to_char_type(ch);
					this->sputc(wc);
				}
				return ch;
			};
			virtual int_type underflow() override
			{
				char_type* get_ptr = this->gptr();
				char_type* eget_ptr = this->egptr();
				std::ptrdiff_t get_buf_off = eget_ptr - get_ptr;
				std::copy(get_ptr - bak_sz, eget_ptr, get);
				get_ptr = get + bak_sz;
				eget_ptr = get_ptr + get_buf_off;
				if (get_stm_nxt == nullptr)
				{
					this->send();
					get_stm_nxt = console::read_echoing();
				}
				const char_type* get_try = get_stm_nxt;
				while (get_try < get_stm_nxt + get_sz && *get_try != L'\0') { ++get_try; }
				std::copy(get_stm_nxt, get_try, eget_ptr);
				std::ptrdiff_t get_stm_off = get_try - get_stm_nxt;
				get_stm_nxt = get_try;
				if (*get_try == L'\0')
				{
					eget_ptr[get_stm_off++] = L'\n';
					get_stm_nxt = nullptr;
				}
				this->setg(get, get_ptr, eget_ptr + get_stm_off);
				return traits_type::to_int_type(*get_ptr);
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
