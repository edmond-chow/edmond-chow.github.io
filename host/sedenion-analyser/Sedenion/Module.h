#include <cmath>
#include <cstdint>
#include <iomanip>
#include <string>
#include <regex>
#include <sstream>
#include <tuple>
#include <array>
#include <functional>
#include <stdexcept>
#include "../Evaluation/[Export]/Evaluation.h"
inline std::int16_t wtoi16_t(const wchar_t* str)
{
	if (str[0] == L'\0') { throw_now(std::invalid_argument("The string cannot not be converted as an integer.")); }
	const wchar_t* number = str;
	if (str[0] == L'-' || str[0] == L'+')
	{
		if (str[1] == L'\0') { throw_now(std::invalid_argument("The string cannot not be converted as an integer.")); }
		++number;
	}
	std::size_t number_size = 0;
	const wchar_t* number_end = number;
	while (*number_end != L'\0')
	{
		if (static_cast<std::uint16_t>(*number_end) < 48 || static_cast<std::uint16_t>(*number_end) > 57) { throw_now(std::invalid_argument("The string cannot not be converted as an integer.")); }
		++number_end;
		++number_size;
	}
	const wchar_t wcharsPlus[] = L"32767";
	const wchar_t wcharsMinus[] = L"32768";
	wchar_t digitsCheck[5]{ L'\0' };
	if (number_size > 5)
	{
		throw_now(std::out_of_range("An integer is exceeded the limit."));
	}
	std::int16_t accumulate = 1;
	std::int16_t output = 0;
	for (std::size_t i = 0; i < number_size; ++i)
	{
		wchar_t wchar = number[number_size - i - 1];
		digitsCheck[number_size - i - 1] = wchar;
		std::uint16_t digit = static_cast<std::uint16_t>(wchar) - 48;
		if (str[0] == L'-') { output -= digit * accumulate; }
		else { output += digit * accumulate; }
		accumulate = accumulate * 10;
	}
	if (number_size == 5)
	{
		for (std::size_t i = 0; i < 5; ++i)
		{
			if (str[0] == L'-')
			{
				if (digitsCheck[i] < wcharsMinus[i]) { break; }
				else if (digitsCheck[i] > wcharsMinus[i]) { throw_now(std::out_of_range("An integer is exceeded the limit.")); }
			}
			else
			{
				if (digitsCheck[i] < wcharsPlus[i]) { break; }
				else if (digitsCheck[i] > wcharsPlus[i]) { throw_now(std::out_of_range("An integer is exceeded the limit.")); }
			}
		}
	}
	return output;
};
inline std::int16_t stoi16_t(const std::wstring& str)
{
	std::wstring result = std::regex_replace(str, std::wregex(L" "), L"");
	return wtoi16_t(result.c_str());
};
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
inline bool CheckForRange(const std::wstring& Number, const std::wstring& Check, bool Sign)
{
	for (std::size_t i = 0; i < Number.size() < Check.size() ? Number.size() : Check.size(); ++i)
	{
		if (Sign)
		{
			if (Number[i] < Check[i]) { break; }
			else if (Number[i] > Check[i]) { return false; }
		}
		else
		{
			if (Number[i] > Check[i]) { break; }
			else if (Number[i] < Check[i]) { return false; }
		}
	}
	return true;
};
inline double CaseToDouble(const std::wsmatch& Match)
{
	std::int16_t Exponent = 0;
	std::wstring Exponential = Match.str(4);
	if (!Exponential.empty())
	{
		std::wstring ExponentialSign = Match.str(5);
		std::wstring ExponentialDigits = Match.str(6);
		static thread_local const struct References {
			std::int16_t* Exponent;
			std::wstring* ExponentialSign;
			std::wstring* ExponentialDigits;
		} References{ &Exponent, &ExponentialSign, &ExponentialDigits };
		operate_t operate = +[]() -> void {
			*References.Exponent = stoi16_t(*References.ExponentialSign + *References.ExponentialDigits);
		};
		caught_t caught = +[](const std::exception& ex) -> void {
			if (typeid(ex) == typeid(std::out_of_range)) { throw_now(std::out_of_range("The number is out of range which cannot be a vaild representation in double.")); }
			rethrow_current();
		};
		evaluate(operate, caught);
	}
	std::wstring Integral = Match.str(2);
	std::wstring Decimal = Match.str(3);
	for (std::wstring::const_iterator ite = ++Integral.begin(); ite < Integral.end(); ++ite)
	{
		Exponent += 1;
	}
	if ((Exponent == 308 && CheckForRange(Integral + (Decimal.empty() ? L"" : Decimal.substr(1)), L"17976931348623157", true) == false) || Exponent > 308)
	{
		throw_now(std::out_of_range("The number is out of range which cannot be a vaild representation in double."));
	}
	else if ((Exponent == -324 && CheckForRange(Integral + (Decimal.empty() ? L"" : Decimal.substr(1)), L"49406564584124654", false) == false) || Exponent < -324)
	{
		return 0;
	}
	return std::stod(Match.str());
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
			Data += CaseToDouble(Match);
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
	if (!TestForValid(TheValue, Size, Terms)) { throw_now(std::invalid_argument("The string is invalid.")); }
	if (TheValue.length() == 0) { throw_now(std::invalid_argument("The string is empty.")); }
	SetForValue(TheValue, Size, Numbers, Terms);
};
