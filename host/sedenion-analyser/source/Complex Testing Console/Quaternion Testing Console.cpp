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
#include "Quaternion.h"
#pragma push_macro("Gbl")
#if defined(_MSVC_LANG)
#define Gbl __stdcall
#else
#define Gbl
#endif
using namespace ComplexTestingConsole;
using namespace Num;
namespace QuterBasis
{
	static thread_local int& Err{ errno };
	template <typename T>
	void Mul(const std::wstring& L, const wchar_t* R, T(Gbl* F)(const Quter&, const Quter&))
	{
		if (Err != 0) { return; }
		if (L == R)
		{
			Quter U = Val<Quter>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			Quter V = Val<Quter>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			T Rst = std::invoke(F, U, V);
			std::wstring Str;
			if constexpr (std::is_same_v<T, Vec3D>) { Str = ToModStr(Quter{ 0, Rst }); }
			else { Str = ToModStr(Rst); }
			Base::Output(Str);
		}
	};
	template <typename T>
	void Op(const std::wstring& L, const wchar_t* R, T(Gbl* F)(const Quter&, const Quter&))
	{
		if (Err != 0) { return; }
		if (L == R)
		{
			Quter U = Val<Quter>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			Quter V = Val<Quter>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, U, V)));
		}
	};
	inline void Pow(const std::wstring& L, const wchar_t* R, Quter(Gbl* F)(const Quter&, std::int64_t))
	{
		if (Err != 0) { return; }
		if (L == R)
		{
			Quter U = Val<Quter>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			std::int64_t V = Int(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, U, V)));
		}
	};
	template <typename... As>
	void Pow(const std::wstring& L, std::wstring&& R, Quter(Gbl* F)(const Quter&, const Quter&, std::int64_t, As...))
	{
		if (Err != 0) { return; }
		if (L == R)
		{
			Quter U = Val<Quter>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			Quter V = Val<Quter>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			std::array<std::int64_t, 1 + sizeof...(As)> Dat{};
			PowGet(Dat);
			PowRst(F, U, V, Dat);
		}
		else if (L == R + L"()")
		{
			Quter U = Val<Quter>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			Quter V = Val<Quter>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			std::array<std::pair<std::int64_t, std::int64_t>, 1 + sizeof...(As)> Dat{};
			PowGet(Dat);
			PowRst(F, R, U, V, Dat);
		}
	};
	template <typename T>
	void Bas(const std::wstring& L, const wchar_t* R, T(Gbl* F)(const Quter&))
	{
		if (Err != 0) { return; }
		if (L == R)
		{
			Quter V = Val<Quter>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, V)));
		}
	};
	template <typename T>
	void BasP(const std::wstring& L, std::wstring&& R, T(Gbl* F)(const Quter&, std::int64_t))
	{
		if (Err != 0) { return; }
		if (L == R)
		{
			Quter V = Val<Quter>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			std::int64_t P = Int(Base::Input(L"P = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, V, P)));
		}
		else if (L == R + L"()")
		{
			Quter V = Val<Quter>(Base::Input(L"V = "));
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
	inline void Tri(const std::wstring& L, const wchar_t* R, Quter(Gbl* F)(const Quter&))
	{
		if (Err != 0) { return; }
		if (L == R)
		{
			Quter V = Val<Quter>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, V)));
		}
	};
	inline void Atri(const std::wstring& L, std::wstring&& R, Quter(Gbl* F)(const Quter&, bool, std::int64_t))
	{
		if (Err != 0) { return; }
		if (L == R)
		{
			Quter V = Val<Quter>(Base::Input(L"V = "));
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
			Quter V = Val<Quter>(Base::Input(L"V = "));
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
	class QuterConsole final
	{
	private:
		constexpr QuterConsole() noexcept = delete;
		constexpr ~QuterConsole() noexcept = delete;
	public:
		static void Load() noexcept;
	};
	void QuterConsole::Load() noexcept
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
			Op(L, L"=", operator ==);
			Op(L, L"+", operator +);
			Op(L, L"-", operator -);
			Op(L, L"*", operator *);
			Op(L, L"/", operator /);
			/****/
			Pow(L, L"^", operator ^);
			Pow(L, L"power", Quter::Power);
			Pow(L, L"root", Quter::Root);
			Pow(L, L"log", Quter::Log);
			/****/
			Mul(L, L"dot", Quter::Dot);
			Mul(L, L"outer", Quter::Outer);
			Mul(L, L"even", Quter::Even);
			Mul(L, L"cross", Quter::Cross);
			/****/
			Bas(L, L"abs", Quter::Abs);
			BasP(L, L"arg", Quter::Arg);
			Bas(L, L"conjg", Quter::Conjg);
			Bas(L, L"sgn", Quter::Sgn);
			Bas(L, L"inverse", Quter::Inverse);
			Bas(L, L"exp", Quter::Exp);
			BasP(L, L"ln", Quter::Ln);
			/****/
			Tri(L, L"sin", Quter::Sin);
			Tri(L, L"cos", Quter::Cos);
			Tri(L, L"tan", Quter::Tan);
			Tri(L, L"csc", Quter::Csc);
			Tri(L, L"sec", Quter::Sec);
			Tri(L, L"cot", Quter::Cot);
			Tri(L, L"sinh", Quter::Sinh);
			Tri(L, L"cosh", Quter::Cosh);
			Tri(L, L"tanh", Quter::Tanh);
			Tri(L, L"csch", Quter::Csch);
			Tri(L, L"sech", Quter::Sech);
			Tri(L, L"coth", Quter::Coth);
			Atri(L, L"arcsin", Quter::Arcsin);
			Atri(L, L"arccos", Quter::Arccos);
			Atri(L, L"arctan", Quter::Arctan);
			Atri(L, L"arccsc", Quter::Arccsc);
			Atri(L, L"arcsec", Quter::Arcsec);
			Atri(L, L"arccot", Quter::Arccot);
			Atri(L, L"arcsinh", Quter::Arcsinh);
			Atri(L, L"arccosh", Quter::Arccosh);
			Atri(L, L"arctanh", Quter::Arctanh);
			Atri(L, L"arccsch", Quter::Arccsch);
			Atri(L, L"arcsech", Quter::Arcsech);
			Atri(L, L"arccoth", Quter::Arccoth);
			if (Err == EINVAL) { Base::Exception(std::invalid_argument{ " The int& errno becomes EINVAL. " }); }
			if (Err == ERANGE) { Base::Exception(std::out_of_range{ " The int& errno becomes ERANGE. " }); }
			Err = 0;
		}
	};
}
#pragma pop_macro("Gbl")
