#pragma once
/* ============= */
/*               */
/*   Extension   */
/*               */
/* ============= */
#include <string>
namespace CmplxConExt
{
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
	void write(const std::string& content);
	void write(const std::wstring& content);
	void write(const char* content);
	void write(const wchar_t* content);
	void writeLine(const std::string& content);
	void writeLine(const std::wstring& content);
	void writeLine(const char* content);
	void writeLine(const wchar_t* content);
	void read();
	std::wstring resolveReaded();
}
