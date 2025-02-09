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
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
#pragma once
#include <cstddef>
#include <iomanip>
#include <string>
#include <regex>
#include <sstream>
#include <stdexcept>
inline std::wstring Str(double Num)
{
	std::wstringstream Rst;
	Rst << std::defaultfloat << std::setprecision(17) << Num;
	return std::regex_replace(Rst.str(), std::wregex(L"e-0(?=[1-9])"), L"e-");
};
inline std::size_t stos_t(const std::wstring& Str)
{
	int& Err{ errno };
	Err = 0;
	if (Str[0] == L' ')
	{
		Err = EINVAL;
		return 0;
	}
	const wchar_t* Beg{ Str.data() };
	wchar_t* End{ nullptr };
	unsigned long long Rst{ std::wcstoull(Beg, &End, 10) };
	if (Beg == End || End != &*Str.end())
	{
		Err = EINVAL;
		return 0;
	}
	if (Err == ERANGE) { return 0; }
	std::size_t TRst{ static_cast<std::size_t>(Rst) };
	if (TRst != Rst)
	{
		Err = ERANGE;
		return 0;
	}
	return TRst;
};
inline double stod_t(const std::wstring& Str)
{
	int& Err{ errno };
	Err = 0;
	if (Str[0] == L' ')
	{
		Err = EINVAL;
		return 0;
	}
	const wchar_t* Beg{ Str.data() };
	wchar_t* End{ nullptr };
	double Rst{ std::wcstod(Beg, &End) };
	if (Beg == End || End != &*Str.end())
	{
		Err = EINVAL;
		return 0;
	}
	if (Err == ERANGE) { return 0; }
	return Rst;
};
inline constexpr const wchar_t SignBefore[] = LR"((-|\+|^))";
inline constexpr const wchar_t UnsignedReal[] = LR"((\d+)(\.\d+|)([Ee](-|\+|)(\d+)|))";
inline constexpr const wchar_t SignAfter[] = LR"((-|\+|$))";
inline std::wstring AddGroup(const std::wstring& Pat, bool Opt)
{
	return L"(" + Pat + (Opt ? L"|" : L"") + L")";
};
inline std::wstring FollowedBy(const std::wstring& Pat, const std::wstring& Text)
{
	return Pat + L"(?=" + Text + L")";
};
inline std::wstring GetPattern(const std::wstring& Trm)
{
	return FollowedBy(SignBefore + AddGroup(UnsignedReal, !Trm.empty()), Trm + SignAfter);
};
inline std::wstring Replace(const std::wstring& Ipt, const std::wstring& Sch, const std::wstring& Rpt)
{
	std::wstring Rst = Ipt;
	std::size_t Pos = Rst.find(Sch);
	while (Pos != std::wstring::npos)
	{
		Rst = Rst.replace(Pos, Sch.size(), Rpt);
		Pos = Rst.find(Sch, Pos + Rpt.size());
	}
	return Rst;
};
inline std::wstring ToString(const std::vector<double>& Num, const std::vector<std::wstring>& Trm)
{
	if (Num.size() != Trm.size()) { throw std::runtime_error{ "The branch should unreachable." }; }
	std::wstringstream Rst;
	bool Fst = true;
	for (std::size_t i = 0; i < Num.size(); ++i)
	{
		if (Num[i] == 0) { continue; }
		if (Num[i] > 0 && !Fst) { Rst << L"+"; }
		else if (Num[i] == -1) { Rst << L"-"; }
		if (Num[i] != 1 && Num[i] != -1) { Rst << Str(Num[i]); }
		else if (Trm[i].empty()) { Rst << L"1"; }
		if (!Trm[i].empty()) { Rst << Trm[i]; }
		Fst = false;
	}
	if (Fst) { Rst << L"0"; }
	return Rst.str();
};
template <typename N, typename... Ts>
std::wstring ToString(N& Rst, bool Vec, Ts... As)
{
	std::vector<std::wstring> Trm{ As... };
	std::vector<double> Num(Trm.size());
	for (std::size_t i = 0, o = Vec ? 1 : 0; i < Trm.size(); ++i) { Num[i] = Rst[i + o]; }
	return ToString(Num, Trm);
};
inline std::vector<double> ToNumbers(const std::wstring& Val, const std::vector<std::wstring>& Trm)
{
	int& Err{ errno };
	std::vector<double> Num(Trm.size());
	std::size_t Cnt = Val.length();
	if (Cnt == 0)
	{
		Err = EINVAL;
		return std::vector<double>(Trm.size());
	}
	for (std::size_t i = 0; i < Num.size(); ++i)
	{
		double Data = 0;
		std::wstring Rest = Val;
		std::wsmatch Mat;
		std::regex_constants::match_flag_type Flag = std::regex_constants::match_default;
		while (std::regex_search(Rest, Mat, std::wregex(GetPattern(Trm[i])), Flag))
		{
			std::wstring Cap = Mat.str();
			Cnt -= Cap.length() + Trm[i].length();
			if (Cap.empty() || Cap == L"+") { ++Data; }
			else if (Cap == L"-") { --Data; }
			else
			{
				Data += stod_t(Cap);
				if (Err != 0) { return std::vector<double>(Trm.size()); }
			}
			Rest = Mat.suffix().str();
			Flag |= std::regex_constants::match_not_bol;
		}
		Num[i] = Data;
	}
	if (Cnt != 0)
	{
		Err = EINVAL;
		return std::vector<double>(Trm.size());
	}
	return Num;
};
template <typename N, typename... Ts>
void ToNumbers(const std::wstring& Val, N& Rst, bool Vec, Ts... As)
{
	std::vector<std::wstring> Trm{ As... };
	std::vector<double> Num = ToNumbers(Val, Trm);
	for (std::size_t i = 0, o = Vec ? 1 : 0; i < Trm.size(); ++i) { Rst[i + o] = Num[i]; }
};
#pragma pack(push)
#pragma push_macro("Ths")
#if defined(X86) || defined(ARM)
#pragma pack(4)
#elif defined(X64) || defined(ARM64)
#pragma pack(8)
#endif
#if defined(_MSVC_LANG)
#define Ths __thiscall
#else
#define Ths
#endif
namespace Num
{
	class String
	{
	private:
		wchar_t* Pointer;
	public:
		Ths String();
		Ths String(const wchar_t* Pt);
		String(const String& Self) = delete;
		Ths String(String&& Self) noexcept;
		String& operator =(const String& O) & = delete;
		String& Ths operator =(String&& O) & noexcept;
		Ths ~String() noexcept;
		const wchar_t* Ths Ptr() const&;
	};
}
#pragma pop_macro("Ths")
#pragma pack(pop)
