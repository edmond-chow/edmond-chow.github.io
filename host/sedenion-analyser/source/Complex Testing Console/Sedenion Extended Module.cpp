/*
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
#include "Sedenion.h"
#pragma push_macro("Gbl")
#if defined(_MSVC_LANG)
#define Gbl __stdcall
#else
#define Gbl
#endif
using namespace ComplexTestingConsole;
using namespace Num;
namespace SedenBasis
{
	template <typename T>
	void Mul(const std::wstring& L, const wchar_t* R, T(Gbl* F)(const Seden&, const Seden&))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Seden U = Val<Seden>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			Seden V = Val<Seden>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, U, V)));
		}
	};
	template <typename T>
	void Op(const std::wstring& L, const wchar_t* R, T(Gbl* F)(const Seden&, const Seden&))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Seden U = Val<Seden>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			Seden V = Val<Seden>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, U, V)));
		}
	};
	inline void Pow(const std::wstring& L, const wchar_t* R, Seden(Gbl* F)(const Seden&, std::int64_t))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Seden U = Val<Seden>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			std::int64_t V = Int(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, U, V)));
		}
	};
	template <typename... As>
	void Pow(const std::wstring& L, std::wstring&& R, Seden(Gbl* F)(const Seden&, const Seden&, std::int64_t, As...))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Seden U = Val<Seden>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			Seden V = Val<Seden>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			std::array<std::int64_t, 1 + sizeof...(As)> Dat{};
			PowGet(Dat);
			PowRst(F, U, V, Dat);
		}
		else if (L == R + L"()")
		{
			Seden U = Val<Seden>(Base::Input(L"U = "));
			if (Err != 0) { return; }
			Seden V = Val<Seden>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			std::array<std::pair<std::int64_t, std::int64_t>, 1 + sizeof...(As)> Dat{};
			PowGet(Dat);
			PowRst(F, R, U, V, Dat);
		}
	};
	template <typename T>
	void Bas(const std::wstring& L, const wchar_t* R, T(Gbl* F)(const Seden&))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Seden V = Val<Seden>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, V)));
		}
	};
	template <typename T>
	void BasP(const std::wstring& L, std::wstring&& R, T(Gbl* F)(const Seden&, std::int64_t))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Seden V = Val<Seden>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			std::int64_t P = Int(Base::Input(L"P = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, V, P)));
		}
		else if (L == R + L"()")
		{
			Seden V = Val<Seden>(Base::Input(L"V = "));
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
	inline void Tri(const std::wstring& L, const wchar_t* R, Seden(Gbl* F)(const Seden&))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Seden V = Val<Seden>(Base::Input(L"V = "));
			if (Err != 0) { return; }
			Base::Output(ToModStr(std::invoke(F, V)));
		}
	};
	inline void Atri(const std::wstring& L, std::wstring&& R, Seden(Gbl* F)(const Seden&, bool, std::int64_t))
	{
		int& Err{ errno };
		if (Err != 0) { return; }
		if (L == R)
		{
			Seden V = Val<Seden>(Base::Input(L"V = "));
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
			Seden V = Val<Seden>(Base::Input(L"V = "));
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
	class SedenConsole final
	{
	private:
		constexpr SedenConsole() noexcept = delete;
		constexpr ~SedenConsole() noexcept = delete;
	public:
		static void Load() noexcept;
	};
	void SedenConsole::Load() noexcept
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
			Pow(L, L"power", Seden::Power);
			Pow(L, L"root", Seden::Root);
			Pow(L, L"log", Seden::Log);
			/****/
			Mul(L, L"dot", Seden::Dot);
			Mul(L, L"outer", Seden::Outer);
			Mul(L, L"even", Seden::Even);
			Mul(L, L"cross", Seden::Cross);
			/****/
			Bas(L, L"abs", Seden::Abs);
			BasP(L, L"arg", Seden::Arg);
			Bas(L, L"conjg", Seden::Conjg);
			Bas(L, L"sgn", Seden::Sgn);
			Bas(L, L"inverse", Seden::Inverse);
			Bas(L, L"exp", Seden::Exp);
			BasP(L, L"ln", Seden::Ln);
			/****/
			Tri(L, L"sin", Seden::Sin);
			Tri(L, L"cos", Seden::Cos);
			Tri(L, L"tan", Seden::Tan);
			Tri(L, L"csc", Seden::Csc);
			Tri(L, L"sec", Seden::Sec);
			Tri(L, L"cot", Seden::Cot);
			Tri(L, L"sinh", Seden::Sinh);
			Tri(L, L"cosh", Seden::Cosh);
			Tri(L, L"tanh", Seden::Tanh);
			Tri(L, L"csch", Seden::Csch);
			Tri(L, L"sech", Seden::Sech);
			Tri(L, L"coth", Seden::Coth);
			Atri(L, L"arcsin", Seden::Arcsin);
			Atri(L, L"arccos", Seden::Arccos);
			Atri(L, L"arctan", Seden::Arctan);
			Atri(L, L"arccsc", Seden::Arccsc);
			Atri(L, L"arcsec", Seden::Arcsec);
			Atri(L, L"arccot", Seden::Arccot);
			Atri(L, L"arcsinh", Seden::Arcsinh);
			Atri(L, L"arccosh", Seden::Arccosh);
			Atri(L, L"arctanh", Seden::Arctanh);
			Atri(L, L"arccsch", Seden::Arccsch);
			Atri(L, L"arcsech", Seden::Arcsech);
			Atri(L, L"arccoth", Seden::Arccoth);
			if (Err == EINVAL) { Base::Exception(std::invalid_argument{ " The int& errno becomes EINVAL. " }); }
			if (Err == ERANGE) { Base::Exception(std::out_of_range{ " The int& errno becomes ERANGE. " }); }
		}
	};
}
#pragma pop_macro("Gbl")
