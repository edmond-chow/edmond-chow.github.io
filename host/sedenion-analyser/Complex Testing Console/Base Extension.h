#pragma once
/* ============= */
/*               */
/*   Extension   */
/*               */
/* ============= */
#include <string>
#include <sstream>
namespace CmplxConExt
{
	struct wcout_t : std::wstringstream
	{
	private:
		std::size_t count;
	};
	extern wcout_t wcout;
	wcout_t& endl(wcout_t& wcout);
	void output(wcout_t& wcout);
	template <typename T>
	wcout_t& operator <<(wcout_t& wcout, T o)
	{
		dynamic_cast<std::wstringstream&>(wcout) << o;
		output(wcout);
		return wcout;
	};
	inline wcout_t& operator <<(wcout_t& wcout, wcout_t&(*endl)(wcout_t& wcout))
	{
		return endl(wcout);
	};
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
	ConsoleColor getForegroundColor();
	ConsoleColor getBackgroundColor();
	std::wstring getTitle();
	void setForegroundColor(ConsoleColor color);
	void setBackgroundColor(ConsoleColor color);
	void setTitle(const std::wstring& title);
	void clear();
	void pressAnyKey();
	void read();
	std::wstring resolveReaded();
}
