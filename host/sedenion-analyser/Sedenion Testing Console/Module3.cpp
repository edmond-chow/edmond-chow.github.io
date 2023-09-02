﻿#include "Base.h"
#include "Complex 3.0.h"
using namespace SedenionTestingConsole;
using namespace Cmplx3;
using namespace Cmplx3::MainType;
namespace Mod3
{
	template <typename F = Octonion(CMPLX3_FUNC_CALL*)(const Octonion&, const Octonion&)>
	void op(const std::wstring& LeftValue, const wchar_t* RightValue, F Subroutine)
	{
		if (LeftValue == RightValue)
		{
			Octonion Union = Octonion::GetInstance(Base::Input(L"Union = "));
			Octonion Value = Octonion::GetInstance(Base::Input(L"Value = "));
			Base::Output(to_wstring(std::invoke(Subroutine, Union, Value)));
		}
	};
	template <typename F>
	void power(const std::wstring& LeftValue, const wchar_t* RightValue, F Subroutine)
	{
		if (LeftValue == RightValue)
		{
			Octonion Base = Octonion::GetInstance(Base::Input(L"Base = "));
			std::int64_t Exponent = stoi64_t(Base::Input(L"Exponent = "));
			Base::Output(to_wstring(std::invoke(Subroutine, Base, Exponent)));
		}
	};
	template <typename... args>
	void power(const std::wstring& LeftValue, std::wstring&& RightValue, Octonion(CMPLX3_FUNC_CALL* Subroutine)(const Octonion&, const Octonion&, std::int64_t, args...))
	{
		if (LeftValue == RightValue)
		{
			Octonion Union = Octonion::GetInstance(Base::Input(L"Union = "));
			Octonion Value = Octonion::GetInstance(Base::Input(L"Value = "));
			std::array<std::int64_t, 1 + sizeof...(args)> Data{};
			power_get(Data);
			power_result(Subroutine, RightValue, Union, Value, Data);
		}
		else if (LeftValue == RightValue + L"()")
		{
			Octonion Union = Octonion::GetInstance(Base::Input(L"Union = "));
			Octonion Value = Octonion::GetInstance(Base::Input(L"Value = "));
			std::array<std::pair<std::int64_t, std::int64_t>, 1 + sizeof...(args)> Data{};
			power_get(Data);
			power_result(Subroutine, RightValue, Union, Value, Data);
		}
	};
	template <typename F>
	void basic(const std::wstring& LeftValue, const wchar_t* RightValue, F Subroutine)
	{
		if (LeftValue == RightValue)
		{
			Octonion Value = Octonion::GetInstance(Base::Input(L"Value = "));
			Base::Output(to_wstring(std::invoke(Subroutine, Value)));
		}
	};
	template <typename R>
	void basic(const std::wstring& LeftValue, std::wstring&& RightValue, R(CMPLX3_FUNC_CALL* Subroutine)(const Octonion&, std::int64_t))
	{
		if (LeftValue == RightValue)
		{
			Octonion Value = Octonion::GetInstance(Base::Input(L"Value = "));
			std::int64_t Theta = stoi64_t(Base::Input(L"Theta = "));
			Base::Output(to_wstring(std::invoke(Subroutine, Value, Theta)));
		}
		else if (LeftValue == RightValue + L"()")
		{
			Octonion Value = Octonion::GetInstance(Base::Input(L"Value = "));
			std::int64_t ThetaMin = stoi64_t(Base::Input(L"ThetaMin = "));
			std::int64_t ThetaMax = stoi64_t(Base::Input(L"ThetaMax = "));
			for (std::int64_t Theta = ThetaMin; Theta <= ThetaMax; ++Theta)
			{
				Base::Output(RightValue + L"(" + to_wstring(Theta) + L") = ", to_wstring(std::invoke(Subroutine, Value, Theta)));
			}
		}
	};
	template <typename F = Octonion(CMPLX3_FUNC_CALL*)(const Octonion&)>
	inline void tri(const std::wstring& LeftValue, const wchar_t* RightValue, F Subroutine)
	{
		if (LeftValue == RightValue)
		{
			Octonion Value = Octonion::GetInstance(Base::Input(L"Value = "));
			Base::Output(to_wstring(std::invoke(Subroutine, Value)));
		}
	};
	template <typename F = Octonion(CMPLX3_FUNC_CALL*)(const Octonion&, bool, std::int64_t)>
	inline void arctri(const std::wstring& LeftValue, std::wstring&& RightValue, F Subroutine)
	{
		if (LeftValue == RightValue)
		{
			Octonion Value = Octonion::GetInstance(Base::Input(L"Value = "));
			bool Sign = false;
			std::wstring Input = std::regex_replace(Base::Input(L"Sign : "), std::wregex(L" "), L"");
			if (Input == L"+") { Sign = true; }
			else if (Input != L"-") { throw std::invalid_argument("A string interpretation of the sign cannot be converted as a bool value."); }
			std::int64_t Period = stoi64_t(Base::Input(L"Period = "));
			Base::Output(to_wstring(std::invoke(Subroutine, Value, Sign, Period)));
		}
		else if (LeftValue == RightValue + L"()")
		{
			Octonion Value = Octonion::GetInstance(Base::Input(L"Value = "));
			std::int64_t PeriodMin = stoi64_t(Base::Input(L"PeriodMin = "));
			std::int64_t PeriodMax = stoi64_t(Base::Input(L"PeriodMax = "));
			for (std::int64_t Period = PeriodMin; Period <= PeriodMax; ++Period)
			{
				Base::Output(RightValue + L"(+, " + to_wstring(Period) + L") = ", to_wstring(std::invoke(Subroutine, Value, true, Period)));
			}
			for (std::int64_t Period = PeriodMin; Period <= PeriodMax; ++Period)
			{
				Base::Output(RightValue + L"(-, " + to_wstring(Period) + L") = ", to_wstring(std::invoke(Subroutine, Value, false, Period)));
			}
		}
	};
	class MyModule3 final
	{
	private:
		constexpr MyModule3() noexcept = delete;
		constexpr ~MyModule3() noexcept = delete;
	public:
		static void Load() noexcept;
	};
	void MyModule3::Load() noexcept
	{
		static thread_local struct CapturedLocal { std::wstring* Line; } Captured;
		Base::Startup(Base::GetTitle());
		Base::Selection(L"=   +   -   *   /   ^   power()   root()   log()");
		Base::Selection(L"abs   arg()   conjg   sgn   inverse   exp   ln()");
		Base::Selection(L"sin   cos   tan   csc   sec   cot   arcsin()   arccos()   arctan()   arccsc()   arcsec()   arccot()");
		Base::Selection(L"sinh   cosh   tanh   csch   sech   coth   arcsinh()   arccosh()   arctanh()   arccsch()   arcsech()   arccoth()");
		Base::Selection(Base::GetStartupLine());
		for (std::wstring Line; !Base::IsSwitchTo(Line); Line = Base::Input())
		{
			struct LatestCapturedLocal { std::wstring* Line; } LatestCaptured{ Captured.Line };
			Captured.Line = &Line;
			if (Line.empty()) { continue; }
			operate_t operate = +[]() -> void {
				const std::wstring& Line = *Captured.Line;
				op(Line, L"=", operator ==);
				op(Line, L"+", operator +);
				op(Line, L"-", operator -);
				op(Line, L"*", operator *);
				op(Line, L"/", operator /);
				/****/
				power(Line, L"^", operator ^);
				power(Line, L"power", Octonion::power);
				power(Line, L"root", Octonion::root);
				power(Line, L"log", Octonion::log);
				/****/
				basic(Line, L"abs", Octonion::abs);
				basic(Line, L"arg", Octonion::arg);
				basic(Line, L"conjg", Octonion::conjg);
				basic(Line, L"sgn", Octonion::sgn);
				basic(Line, L"inverse", Octonion::inverse);
				basic(Line, L"exp", Octonion::exp);
				basic(Line, L"ln", Octonion::ln);
				/****/
				tri(Line, L"sin", Octonion::sin);
				tri(Line, L"cos", Octonion::cos);
				tri(Line, L"tan", Octonion::tan);
				tri(Line, L"csc", Octonion::csc);
				tri(Line, L"sec", Octonion::sec);
				tri(Line, L"cot", Octonion::cot);
				tri(Line, L"sinh", Octonion::sinh);
				tri(Line, L"cosh", Octonion::cosh);
				tri(Line, L"tanh", Octonion::tanh);
				tri(Line, L"csch", Octonion::csch);
				tri(Line, L"sech", Octonion::sech);
				tri(Line, L"coth", Octonion::coth);
				arctri(Line, L"arcsin", Octonion::arcsin);
				arctri(Line, L"arccos", Octonion::arccos);
				arctri(Line, L"arctan", Octonion::arctan);
				arctri(Line, L"arccsc", Octonion::arccsc);
				arctri(Line, L"arcsec", Octonion::arcsec);
				arctri(Line, L"arccot", Octonion::arccot);
				arctri(Line, L"arcsinh", Octonion::arcsinh);
				arctri(Line, L"arccosh", Octonion::arccosh);
				arctri(Line, L"arctanh", Octonion::arctanh);
				arctri(Line, L"arccsch", Octonion::arccsch);
				arctri(Line, L"arcsech", Octonion::arcsech);
				arctri(Line, L"arccoth", Octonion::arccoth);
			};
			caught_t caught = +[](const std::exception& ex) -> void { Base::Exception(ex); };
			evaluate(operate, caught);
			Captured.Line = LatestCaptured.Line;
		}
	};
}
