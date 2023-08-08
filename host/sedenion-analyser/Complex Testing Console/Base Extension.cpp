/* ============= */
/*               */
/*   Extension   */
/*               */
/* ============= */
#include <emscripten.h>
#include <emscripten/bind.h>
#include <cstdlib>
#include <string>
#include "Base.h"
namespace CmplxConExt
{
	EM_ASYNC_JS(void, readWrapper, (), {
		iostream.registerReaded(await iostream.read());
	});
	EM_ASYNC_JS(void, pressAnyKeyWrapper, (), {
		await iostream.pressAnyKey();
	});
	EM_ASYNC_JS(void, suspendWrapper, (), {
		await suspend();
	});
	inline std::string toMbsString(const std::wstring& string)
	{
		std::size_t length = string.size() * 5 + 1;
		char* temporary = new char[length] { '\0' };
		std::wcstombs(temporary, string.c_str(), length);
		std::string converted = temporary;
		delete[] temporary;
		return converted;
	};
	inline std::wstring toWcsString(const std::string& string)
	{
		std::size_t length = string.size() * 5 + 1;
		wchar_t* temporary = new wchar_t[length] { '\0' };
		std::mbstowcs(temporary, string.c_str(), length);
		std::wstring converted = temporary;
		delete[] temporary;
		return converted;
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
	static const char* ConsoleColorList[] {
		"black",
		"dark-blue",
		"dark-green",
		"dark-cyan",
		"dark-red",
		"dark-magenta",
		"dark-yellow",
		"gray",
		"dark-gray",
		"blue",
		"green",
		"cyan",
		"red",
		"magenta",
		"yellow",
		"white",
	};
	inline ConsoleColor toConsoleColor(const char* output) {
		for (std::size_t i = 0; i < std::extent_v<decltype(ConsoleColorList)>; ++i) {
			if (std::strcmp(output, ConsoleColorList[i]) == 0) {
				return static_cast<ConsoleColor>(i);
			}
		}
		return static_cast<ConsoleColor>(0);
	};
	inline const char* toStringLiteral(ConsoleColor input) {
		return ConsoleColorList[static_cast<std::size_t>(input) % std::extent_v<decltype(ConsoleColorList)>];
	};
	ConsoleColor getForegroundColor()
	{
		return toConsoleColor(emscripten_run_script_string("iostream.getForegroundColor()"));
	};
	ConsoleColor getBackgroundColor()
	{
		return toConsoleColor(emscripten_run_script_string("iostream.getBackgroundColor()"));
	};
	std::wstring getTitle()
	{
		return toWcsString(emscripten_run_script_string("getTitle()"));
	};
	void setForegroundColor(ConsoleColor color)
	{
		std::string call = "iostream.setForegroundColor(\"" + std::string(toStringLiteral(color)) + "\")";
		emscripten_run_script(call.c_str());
	};
	void setBackgroundColor(ConsoleColor color)
	{
		std::string call = "iostream.setBackgroundColor(\"" + std::string(toStringLiteral(color)) + "\")";
		emscripten_run_script(call.c_str());
	};
	void setTitle(const std::wstring& title)
	{
		std::string call = "setTitle(\"" + toMbsString(title) + "\")";
		emscripten_run_script(call.c_str());
	};
	void clear()
	{
		emscripten_run_script("iostream.clear()");
	};
	void pressAnyKey()
	{
		pressAnyKeyWrapper();
	};
	void write(const std::string& content) {
		suspendWrapper();
		std::string call = "iostream.write(\"" + content + "\")";
		emscripten_run_script(call.c_str());
	};
	void write(const std::wstring& content) {
		suspendWrapper();
		std::string call = "iostream.write(\"" + toMbsString(content) + "\")";
		emscripten_run_script(call.c_str());
	};
	void write(const char* content) {
		write(std::string(content));
	};
	void write(const wchar_t* content) {
		write(std::wstring(content));
	};
	void writeLine(const std::string& content) {
		suspendWrapper();
		std::string call = "iostream.writeLine(\"" + content + "\")";
		emscripten_run_script(call.c_str());
	};
	void writeLine(const std::wstring& content) {
		suspendWrapper();
		std::string call = "iostream.writeLine(\"" + toMbsString(content) + "\")";
		emscripten_run_script(call.c_str());
	};
	void writeLine(const char* content) {
		writeLine(std::string(content));
	};
	void writeLine(const wchar_t* content) {
		writeLine(std::wstring(content));
	};
	void read() {
		readWrapper();
	};
	std::wstring resolveReaded() {
		return toWcsString(emscripten_run_script_string("iostream.resolveReaded()"));
	};
}
extern "C" int main()
{
	while (true)
	{
		emscripten_run_script("console.log('loop')");
		ComplexTestingConsole::Base::Main();
		CmplxConExt::writeLine(L"The program ended with a return code EXIT_SUCCESS successfully.");
		CmplxConExt::writeLine(L"");
		CmplxConExt::writeLine(L"   Press any key to continue with restart the program . . .   ");
		CmplxConExt::pressAnyKey();
		ComplexTestingConsole::Base::IsSwitchTo(L"[Octonion Testing Console]");	
	}
	return EXIT_SUCCESS;
};
