﻿/*
 *   Copyright 2022 Edmond Chow
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   disTributed under the License is disTributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
#include "Base.h"
#include "Octonion.h"
#pragma push_macro("Gbl")
#if defined(_MSVC_LANG)
#define Gbl __stdcall
#else
#define Gbl
#endif
using namespace ComplexTestingConsole;
using namespace Num;
namespace OctonBasis
{
	template <typename T>
	void Mul(const std::wstring& L, const wchar_t* R, T(Gbl* F)(const Octon&, const Octon&))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Octon U = Val<Octon>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			Octon V = Val<Octon>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			T Rst = std::invoke(F, U, V);
			std::wstring Str;
			if constexpr (std::is_same_v<T, Vec7D>) { Str = ToModStr(Octon{ 0, Rst }); }
			else { Str = ToModStr(Rst); }
			Base::Output(Str);
		}
	};
	template <typename T>
	void Op(const std::wstring& L, const wchar_t* R, T(Gbl* F)(const Octon&, const Octon&))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Octon U = Val<Octon>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			Octon V = Val<Octon>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, U, V)));
		}
	};
	inline void Pow(const std::wstring& L, const wchar_t* R, Octon(Gbl* F)(const Octon&, std::int64_t))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Octon U = Val<Octon>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			std::int64_t V = Int(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, U, V)));
		}
	};
	template <typename... As>
	void Pow(const std::wstring& L, std::wstring&& R, Octon(Gbl* F)(const Octon&, const Octon&, std::int64_t, As...))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Octon U = Val<Octon>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			Octon V = Val<Octon>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			std::array<std::int64_t, 1 + sizeof...(As)> Dat{};
			PowGet(Dat);
			PowRst(F, U, V, Dat);
		}
		else if (L == R + L"()")
		{
			Octon U = Val<Octon>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			Octon V = Val<Octon>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			std::array<std::pair<std::int64_t, std::int64_t>, 1 + sizeof...(As)> Dat{};
			PowGet(Dat);
			PowRst(F, R, U, V, Dat);
		}
	};
	template <typename T>
	void Bas(const std::wstring& L, const wchar_t* R, T(Gbl* F)(const Octon&))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Octon V = Val<Octon>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, V)));
		}
	};
	template <typename T>
	void BasP(const std::wstring& L, std::wstring&& R, T(Gbl* F)(const Octon&, std::int64_t))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Octon V = Val<Octon>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			std::int64_t P = Int(Base::Input(L"P = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, V, P)));
		}
		else if (L == R + L"()")
		{
			Octon V = Val<Octon>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			std::int64_t PMin = Int(Base::Input(L"P(min) = "));
			if (Err != 0) { return; }
			std::int64_t PMax = Int(Base::Input(L"P(max) = "));
			if (Err != 0) { return; }
			for (std::int64_t P = PMin; P <= PMax; ++P)
			{
				Base::Output(R + L"(" + ToModStr(P) + L") = ", ToModStr(std::invoke(F, V, P)));
			}
		}
	};
	inline void Tri(const std::wstring& L, const wchar_t* R, Octon(Gbl* F)(const Octon&))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Octon V = Val<Octon>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, V)));
		}
	};
	inline void Atri(const std::wstring& L, std::wstring&& R, Octon(Gbl* F)(const Octon&, bool, std::int64_t))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Octon V = Val<Octon>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			bool S = false;
			std::wstring Ipt = std::regex_replace(Base::Input(L"Sign : "), std::wregex(L" "), L"");
			if (Ipt == L"+") { S = true; }
			else if (Ipt != L"-") { throw std::invalid_argument{ "A String interpretation of the sign cannot be converted as a bool value." }; }
			std::int64_t P = Int(Base::Input(L"P = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, V, S, P)));
		}
		else if (L == R + L"()")
		{
			Octon V = Val<Octon>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			std::int64_t PMin = Int(Base::Input(L"P(min) = "));
			if (Err != 0) { return; }
			std::int64_t PMax = Int(Base::Input(L"P(max) = "));
			if (Err != 0) { return; }
			for (std::int64_t P = PMin; P <= PMax; ++P)
			{
				Base::Output(R + L"(+, " + ToModStr(P) + L") = ", ToModStr(std::invoke(F, V, true, P)));
			}
			for (std::int64_t P = PMin; P <= PMax; ++P)
			{
				Base::Output(R + L"(-, " + ToModStr(P) + L") = ", ToModStr(std::invoke(F, V, false, P)));
			}
		}
	};
	class OctonConsole final
	{
	private:
		constexpr OctonConsole() noexcept = delete;
		constexpr ~OctonConsole() noexcept = delete;
	public:
		static void Load() noexcept;
	};
	void OctonConsole::Load() noexcept
	{
		Base::Startup(Base::GetTitle());
		Base::Selection(L"=   +   -   *   /   ^   power()   root()   log()");
		Base::Selection(L"abs   arg()   conjg   sgn   inverse   exp   ln()   dot   outer   even   cross");
		Base::Selection(L"sin   cos   tan   csc   sec   cot   arcsin()   arccos()   arctan()   arccsc()   arcsec()   arccot()");
		Base::Selection(L"sinh   cosh   tanh   csch   sech   coth   arcsinh()   arccosh()   arctanh()   arccsch()   arcsech()   arccoth()");
		Base::Selection(Base::GetStartupLine());
		for (std::wstring L; !Base::IsSwitchTo(L); L = Base::Input())
		{
			if (L.empty()) { continue; }
			int& Err{ errno };
			Err = 0;
			Op(L, L"=", operator ==);
			Op(L, L"+", operator +);
			Op(L, L"-", operator -);
			Op(L, L"*", operator *);
			Op(L, L"/", operator /);
			/****/
			Pow(L, L"^", operator ^);
			Pow(L, L"power", Octon::Power);
			Pow(L, L"root", Octon::Root);
			Pow(L, L"log", Octon::Log);
			/****/
			Mul(L, L"dot", Octon::Dot);
			Mul(L, L"outer", Octon::Outer);
			Mul(L, L"even", Octon::Even);
			Mul(L, L"cross", Octon::Cross);
			/****/
			Bas(L, L"abs", Octon::Abs);
			BasP(L, L"arg", Octon::Arg);
			Bas(L, L"conjg", Octon::Conjg);
			Bas(L, L"sgn", Octon::Sgn);
			Bas(L, L"inverse", Octon::Inverse);
			Bas(L, L"exp", Octon::Exp);
			BasP(L, L"ln", Octon::Ln);
			/****/
			Tri(L, L"sin", Octon::Sin);
			Tri(L, L"cos", Octon::Cos);
			Tri(L, L"tan", Octon::Tan);
			Tri(L, L"csc", Octon::Csc);
			Tri(L, L"sec", Octon::Sec);
			Tri(L, L"cot", Octon::Cot);
			Tri(L, L"sinh", Octon::Sinh);
			Tri(L, L"cosh", Octon::Cosh);
			Tri(L, L"tanh", Octon::Tanh);
			Tri(L, L"csch", Octon::Csch);
			Tri(L, L"sech", Octon::Sech);
			Tri(L, L"coth", Octon::Coth);
			Atri(L, L"arcsin", Octon::Arcsin);
			Atri(L, L"arccos", Octon::Arccos);
			Atri(L, L"arctan", Octon::Arctan);
			Atri(L, L"arccsc", Octon::Arccsc);
			Atri(L, L"arcsec", Octon::Arcsec);
			Atri(L, L"arccot", Octon::Arccot);
			Atri(L, L"arcsinh", Octon::Arcsinh);
			Atri(L, L"arccosh", Octon::Arccosh);
			Atri(L, L"arctanh", Octon::Arctanh);
			Atri(L, L"arccsch", Octon::Arccsch);
			Atri(L, L"arcsech", Octon::Arcsech);
			Atri(L, L"arccoth", Octon::Arccoth);
			if (Err == EINVAL) { Base::Exception(std::invalid_argument{ " The int& errno becomes EINVAL. " }); }
			if (Err == ERANGE) { Base::Exception(std::out_of_range{ " The int& errno becomes ERANGE. " }); }
		}
	};
}
#pragma pop_macro("Gbl")
