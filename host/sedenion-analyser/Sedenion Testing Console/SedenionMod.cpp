#include "Base.h"
#include "../Sedenion/export/Sedenion.h"
using namespace SedenionTestingConsole;
using namespace Seden;
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
namespace SedenionMod
{
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
	inline double stod_t(const std::wstring& str)
	{
		static constexpr const wchar_t RealRegExp[] = L"(-|\\+|)(\\d+)(\\.\\d+|)([Ee](-|\\+|)(\\d+)|)";
		std::wstring Value = std::regex_replace(str, std::wregex(L" "), L"");
		std::wsmatch Match;
		std::wregex Regex(RealRegExp);
		if (!std::regex_match(Value, Match, Regex)) { throw_now(std::invalid_argument("The string is invalid.")); }
		return CaseToDouble(Match);
	};
	template <typename F = Sedenion(SEDEN_FUNC_CALL*)(const Sedenion&, const Sedenion&)>
	void op(const std::wstring& str, const wchar_t* ptr, F f)
	{
		if (str == ptr)
		{
			Sedenion Union = Sedenion::GetInstance(Base::Input(L"Union = "));
			Sedenion Value = Sedenion::GetInstance(Base::Input(L"Value = "));
			Base::Output(to_wstring(std::invoke(f, Union, Value)));
		}
	};
	inline void op(const std::wstring& str, const wchar_t* ptr, Sedenion(SEDEN_FUNC_CALL* f)(const Sedenion&, double))
	{
		if (str == ptr)
		{
			Sedenion Union = Sedenion::GetInstance(Base::Input(L"Union = "));
			double Value = stod_t(Base::Input(L"Value = "));
			Base::Output(to_wstring(std::invoke(f, Union, Value)));
		}
	};
	template <typename F>
	void power(const std::wstring& str, const wchar_t* ptr, F f)
	{
		if (str == ptr)
		{
			Sedenion Base = Sedenion::GetInstance(Base::Input(L"Base = "));
			std::int64_t Exponent = stoi64_t(Base::Input(L"Exponent = "));
			Base::Output(to_wstring(std::invoke(f, Base, Exponent)));
		}
	};
	template <typename... args>
	void power(const std::wstring& str, std::wstring&& ptr, Sedenion(SEDEN_FUNC_CALL* f)(const Sedenion&, const Sedenion&, std::int64_t, args...))
	{
		if (str == ptr)
		{
			Sedenion Union = Sedenion::GetInstance(Base::Input(L"Union = "));
			Sedenion Value = Sedenion::GetInstance(Base::Input(L"Value = "));
			std::array<std::int64_t, 1 + sizeof...(args)> Data{};
			power_get(Data);
			power_result(f, ptr, Union, Value, Data);
		}
		else if (str == ptr + L"()")
		{
			Sedenion Union = Sedenion::GetInstance(Base::Input(L"Union = "));
			Sedenion Value = Sedenion::GetInstance(Base::Input(L"Value = "));
			std::array<std::pair<std::int64_t, std::int64_t>, 1 + sizeof...(args)> Data{};
			power_get(Data);
			power_result(f, ptr, Union, Value, Data);
		}
	};
	template <typename F>
	void basic(const std::wstring& str, const wchar_t* ptr, F f)
	{
		if (str == ptr)
		{
			Sedenion Value = Sedenion::GetInstance(Base::Input(L"Value = "));
			Base::Output(to_wstring(std::invoke(f, Value)));
		}
	};
	template <typename R>
	void basic(const std::wstring& str, std::wstring&& ptr, R(SEDEN_FUNC_CALL* f)(const Sedenion&, std::int64_t))
	{
		if (str == ptr)
		{
			Sedenion Value = Sedenion::GetInstance(Base::Input(L"Value = "));
			std::int64_t Theta = stoi64_t(Base::Input(L"Theta = "));
			Base::Output(to_wstring(std::invoke(f, Value, Theta)));
		}
		else if (str == ptr + L"()")
		{
			Sedenion Value = Sedenion::GetInstance(Base::Input(L"Value = "));
			std::int64_t ThetaMin = stoi64_t(Base::Input(L"ThetaMin = "));
			std::int64_t ThetaMax = stoi64_t(Base::Input(L"ThetaMax = "));
			for (std::int64_t Theta = ThetaMin; Theta <= ThetaMax; ++Theta)
			{
				Base::Output(ptr + L"(" + to_wstring(Theta) + L") = ", to_wstring(std::invoke(f, Value, Theta)));
			}
		}
	};
	template <typename F = Sedenion(SEDEN_FUNC_CALL*)(const Sedenion&)>
	inline void tri(const std::wstring& str, const wchar_t* ptr, F f)
	{
		if (str == ptr)
		{
			Sedenion Value = Sedenion::GetInstance(Base::Input(L"Value = "));
			Base::Output(to_wstring(std::invoke(f, Value)));
		}
	};
	template <typename F = Sedenion(SEDEN_FUNC_CALL*)(const Sedenion&, bool, std::int64_t)>
	inline void arctri(const std::wstring& str, std::wstring&& ptr, F f)
	{
		if (str == ptr)
		{
			Sedenion Value = Sedenion::GetInstance(Base::Input(L"Value = "));
			bool Sign = false;
			std::wstring Input = std::regex_replace(Base::Input(L"Sign : "), std::wregex(L" "), L"");
			if (Input == L"+") { Sign = true; }
			else if (Input != L"-") { throw std::invalid_argument("A string interpretation of the sign cannot be converted as a bool value."); }
			std::int64_t Period = stoi64_t(Base::Input(L"Period = "));
			Base::Output(to_wstring(std::invoke(f, Value, Sign, Period)));
		}
		else if (str == ptr + L"()")
		{
			Sedenion Value = Sedenion::GetInstance(Base::Input(L"Value = "));
			std::int64_t PeriodMin = stoi64_t(Base::Input(L"PeriodMin = "));
			std::int64_t PeriodMax = stoi64_t(Base::Input(L"PeriodMax = "));
			for (std::int64_t Period = PeriodMin; Period <= PeriodMax; ++Period)
			{
				Base::Output(ptr + L"(+, " + to_wstring(Period) + L") = ", to_wstring(std::invoke(f, Value, true, Period)));
			}
			for (std::int64_t Period = PeriodMin; Period <= PeriodMax; ++Period)
			{
				Base::Output(ptr + L"(-, " + to_wstring(Period) + L") = ", to_wstring(std::invoke(f, Value, false, Period)));
			}
		}
	};
	class MySedenionTestor final
	{
	private:
		constexpr MySedenionTestor() noexcept = delete;
		constexpr ~MySedenionTestor() noexcept = delete;
	public:
		static void Load() noexcept;
	};
	void MySedenionTestor::Load() noexcept
	{
		static thread_local struct CapturedLocal { std::wstring* Str; } Captured;
		Base::Startup(Base::GetTitle());
		Base::Selection(L"=   +   -   *   /   ^   Power()   Root()   Log()");
		Base::Selection(L"abs   arg()   conjg   sgn   inverse   exp   ln()");
		Base::Selection(L"sin   cos   tan   csc   sec   cot   arcsin()   arccos()   arctan()   arccsc()   arcsec()   arccot()");
		Base::Selection(L"sinh   cosh   tanh   csch   sech   coth   arcsinh()   arccosh()   arctanh()   arccsch()   arcsech()   arccoth()");
		Base::Selection(Base::GetStartupLine());
		for (std::wstring Str; !Base::IsSwitchTo(Str); Str = Base::Input())
		{
			struct LatestCapturedLocal { std::wstring* Str; } LatestCaptured{ Captured.Str };
			Captured.Str = &Str;
			if (Str.empty()) { continue; }
			operate_t operate = +[]() -> void {
				const std::wstring& Str = *Captured.Str;
				op(Str, L"=", operator ==);
				op(Str, L"+", operator +);
				op(Str, L"-", operator -);
				op(Str, L"*", operator *);
				op(Str, L"/", operator /);
				/****/
				power(Str, L"^", operator ^);
				power(Str, L"Power", Sedenion::Power);
				power(Str, L"Root", Sedenion::Root);
				power(Str, L"Log", Sedenion::Log);
				/****/
				basic(Str, L"abs", Sedenion::abs);
				basic(Str, L"arg", Sedenion::arg);
				basic(Str, L"conjg", Sedenion::conjg);
				basic(Str, L"sgn", Sedenion::sgn);
				basic(Str, L"inverse", Sedenion::inverse);
				basic(Str, L"exp", Sedenion::exp);
				basic(Str, L"ln", Sedenion::ln);
				/****/
				tri(Str, L"sin", Sedenion::sin);
				tri(Str, L"cos", Sedenion::cos);
				tri(Str, L"tan", Sedenion::tan);
				tri(Str, L"csc", Sedenion::csc);
				tri(Str, L"sec", Sedenion::sec);
				tri(Str, L"cot", Sedenion::cot);
				tri(Str, L"sinh", Sedenion::sinh);
				tri(Str, L"cosh", Sedenion::cosh);
				tri(Str, L"tanh", Sedenion::tanh);
				tri(Str, L"csch", Sedenion::csch);
				tri(Str, L"sech", Sedenion::sech);
				tri(Str, L"coth", Sedenion::coth);
				arctri(Str, L"arcsin", Sedenion::arcsin);
				arctri(Str, L"arccos", Sedenion::arccos);
				arctri(Str, L"arctan", Sedenion::arctan);
				arctri(Str, L"arccsc", Sedenion::arccsc);
				arctri(Str, L"arcsec", Sedenion::arcsec);
				arctri(Str, L"arccot", Sedenion::arccot);
				arctri(Str, L"arcsinh", Sedenion::arcsinh);
				arctri(Str, L"arccosh", Sedenion::arccosh);
				arctri(Str, L"arctanh", Sedenion::arctanh);
				arctri(Str, L"arccsch", Sedenion::arccsch);
				arctri(Str, L"arcsech", Sedenion::arcsech);
				arctri(Str, L"arccoth", Sedenion::arccoth);
			};
			caught_t caught = +[](const std::exception& ex) -> void { Base::Exception(ex); };
			evaluate(operate, caught);
			Captured.Str = LatestCaptured.Str;
		}
	};
}
