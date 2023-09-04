#pragma once
/* ============= */
/*               */
/*   Extension   */
/*               */
/* ============= */
#include <string>
#include <sstream>
namespace SedenConExt
{
	namespace dom
	{
		extern struct wcin_t {} wcin;
		extern struct wcout_t : std::wstringstream {} wcout;
		void getline(wcin_t&, std::wstring& line);
		wcout_t& endl(wcout_t& wcout);
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
	enum class ConsoleColor : std::uint8_t
	{
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
	ConsoleColor GetForegroundColor();
	ConsoleColor GetBackgroundColor();
	std::wstring GetTitle();
	void SetForegroundColor(ConsoleColor Color);
	void SetBackgroundColor(ConsoleColor Color);
	void SetTitle(const std::wstring& Text);
	void Clear();
	void PressAnyKey();
}
