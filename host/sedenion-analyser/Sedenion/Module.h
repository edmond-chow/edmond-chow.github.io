#include <cmath>
#include <iomanip>
#include <string>
#include <regex>
#include <sstream>
#include <tuple>
#include <array>
#include <functional>
extern void throw_now(std::wstring&& type, std::wstring&& what);
template <typename CharT, typename Traits, typename Allocator, typename RegexTraits, typename FuncT>
std::basic_string<CharT, Traits, Allocator> regex_search_and_replace(const std::basic_string<CharT, Traits, Allocator>& String, const std::basic_regex<CharT, RegexTraits>& Regex, FuncT Function)
{
	using StringT = std::basic_string<CharT, Traits, Allocator>;
	StringT Input = String;
	StringT Output{};
	bool NotBol = false;
	while (Input.empty() == false)
	{
		bool matched = false;
		std::size_t Cursor = 0;
		while (Cursor <= Input.size())
		{
			std::regex_constants::match_flag_type Type = std::regex_constants::match_default;
			if (NotBol == true) { Type = Type | std::regex_constants::match_not_bol; }
			if (Cursor > 0) { Type = Type | std::regex_constants::match_not_eol; }
			std::match_results<typename StringT::const_iterator> Match;
			StringT SubInput = Input.substr(0, Input.size() - Cursor);
			if (std::regex_search(SubInput, Match, Regex, Type))
			{
				Output.append(Match.prefix().str()).append(Function(std::cref(Match)));
				if (Match.str().empty() == false)
				{
					Input = Match.suffix().str();
					NotBol = true;
					matched = true;
				}
				break;
			}
			++Cursor;
		}
		if (matched == false)
		{
			Output.append(Input.substr(0, 1));
			Input = Input.substr(1);
			NotBol = true;
		}
	}
	return Output;
};
inline std::wstring double_to_wstring(double Number)
{
	std::wstringstream TheString;
	TheString << std::defaultfloat << std::setprecision(17) << Number;
	return std::regex_replace(TheString.str(), std::wregex(L"e-0(?=[1-9])"), L"e-");
};
inline void CheckForRange(const std::wstring& Number, const std::wstring& Check, bool Sign)
{
	for (std::size_t i = 0; i < Number.size() < Check.size() ? Number.size() : Check.size(); ++i)
	{
		if (Sign)
		{
			if (Number[i] < Check[i]) { break; }
			else if (Number[i] > Check[i]) { throw_now(L"std::out_of_range", L"The number is out of range which cannot be a vaild representation in double."); }
		}
		else
		{
			if (Number[i] > Check[i]) { break; }
			else if (Number[i] < Check[i]) { throw_now(L"std::out_of_range", L"The number is out of range which cannot be a vaild representation in double."); }
		}
	}
};
inline void TestForRange(const std::wsmatch& Match)
{
	long long Exponent = 0;
	if (!Match.str(4).empty()) { Exponent = std::stoll(Match.str(5) + Match.str(6)); }
	Exponent += static_cast<long long>(Match.str(2).length()) - 1;
	if (Exponent == 308)
	{
		CheckForRange(Match.str(2) + (Match.str(3).empty() ? L"" : Match.str(3).substr(1)), L"17976931348623157", true);
	}
	else if (Exponent == -324)
	{
		CheckForRange(Match.str(2) + (Match.str(3).empty() ? L"" : Match.str(3).substr(1)), L"49406564584124654", false);
	}
	else if (Exponent > 308 || Exponent < -324)
	{
		throw_now(L"std::out_of_range", L"The number is out of range which cannot be a vaild representation in double.");
	}
};
inline std::wstring ToString(std::size_t Size, const double* Numbers, const std::wstring* Terms)
{
	std::wstringstream TheString;
	for (std::size_t i = 0; i < Size; ++i)
	{
		std::wstring Replace = double_to_wstring(Numbers[i]);
		if (Numbers[i] > 0)
		{
			if (Terms[i].length() > 0) { Replace = std::regex_replace(Replace, std::wregex(L"^1$"), L""); }
			TheString << L'+' << Replace << Terms[i];
		}
		else if (Numbers[i] < 0)
		{
			if (Terms[i].length() > 0) { Replace = std::regex_replace(Replace, std::wregex(L"^-1$"), L"-"); }
			TheString << Replace << Terms[i];
		}
	}
	std::wstring RetString = TheString.str();
	RetString = std::regex_replace(RetString, std::wregex(L"^$"), L"0");
	RetString = std::regex_replace(RetString, std::wregex(L"^\\+"), L"");
	return RetString;
};
inline std::wstring GetInitTermRegexString(std::size_t Size, const std::wstring* Terms)
{
	std::wstringstream TheString;
	TheString << L"(^|\\+|-)(";
	for (std::size_t i = 0; i < Size; ++i)
	{
		if (Terms[i].length() > 0) { TheString << L"(?=" << Terms[i] << L")|"; }
	}
	return std::regex_replace(TheString.str(), std::wregex(L"\\)\\|$"), L"))");
};
inline std::wstring GetRegexString(const std::wstring& Term, bool With)
{
	static constexpr const wchar_t RealRegExp[] = L"(-|\\+|)(\\d+)(\\.\\d+|)([Ee](-|\\+|)(\\d+)|)";
	static constexpr const wchar_t NotOthers[] = L"(-|\\+|$)";
	return std::wstring().append(RealRegExp).append(With ? Term : L"").append(L"(?=").append(With ? L"" : Term).append(NotOthers).append(L")");
};
inline bool TestForValid(const std::wstring& Value, std::size_t Size, const std::wstring* Terms)
{
	std::wstring Test(Value);
	for (std::size_t i = 0; i < Size; ++i)
	{
		std::wregex Regex(GetRegexString(Terms[i], true));
		Test = std::regex_replace(Test, Regex, L"");
	}
	return Test.length() == 0;
};
inline void SetForValue(const std::wstring& TheValue, std::size_t Size, double* Numbers, const std::wstring* Terms)
{
	for (std::size_t i = 0; i < Size; ++i)
	{
		double Data = 0;
		std::wregex Regex(GetRegexString(Terms[i], false));
		std::wstring TheString = TheValue;
		std::wsmatch Match;
		while (std::regex_search(TheString, Match, Regex))
		{
			TestForRange(Match);
			Data += std::stod(Match.str());
			TheString = Match.suffix().str();
		}
		Numbers[i] = Data;
	}
};
inline void ToNumbers(const std::wstring& Value, std::size_t Size, double* Numbers, const std::wstring* Terms)
{
	std::wstring TheValue = std::regex_replace(Value, std::wregex(L" "), L"");
	TheValue = regex_search_and_replace(TheValue, std::wregex(GetInitTermRegexString(Size, Terms)), [](const std::wsmatch& Match) -> std::wstring {
		return Match.str() + L"1";
	});
	if (!TestForValid(TheValue, Size, Terms)) { throw_now(L"std::invalid_argument", L"The string is invalid."); }
	if (TheValue.length() == 0) { throw_now(L"std::invalid_argument", L"The string is empty."); }
	SetForValue(TheValue, Size, Numbers, Terms);
};
