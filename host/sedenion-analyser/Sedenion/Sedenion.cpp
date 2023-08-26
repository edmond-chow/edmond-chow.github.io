#include <cmath>
#include <numbers>
#include <limits>
#include "Module.h"
#include "Cayley Dickson Algebra.h"
inline std::size_t wtos_t(const wchar_t* str)
{
static const std::wstring limit = std::to_wstring(std::numeric_limits<std::size_t>::max());
	if (str[0] == L'\0' || str[0] == L'-') { throw std::invalid_argument("The string cannot not be converted as an integer."); }
	const wchar_t* number = str;
	if (str[0] == L'+')
	{
		if (str[1] == L'\0') { throw std::invalid_argument("The string cannot not be converted as an integer."); }
		++number;
	}
	std::size_t number_size = std::wcslen(str);
	const wchar_t* number_end = number;
	while (*number_end != L'\0')
	{
		if (static_cast<std::uint16_t>(*number_end) < 48 || static_cast<std::uint16_t>(*number_end) > 57) { throw std::invalid_argument("The string cannot not be converted as an integer."); }
		++number_end;
	}
	const wchar_t* wchars = limit.c_str();
	std::size_t wchars_size = limit.size();
	wchar_t* digitsCheck = new wchar_t[wchars_size] { L'\0' };
	if (number_size > wchars_size)
	{
		throw std::out_of_range("An integer is exceeded the limit.");
	}
	std::int64_t accumulate = 1;
	std::int64_t output = 0;
	for (std::size_t i = 0; i < number_size; ++i)
	{
		wchar_t wchar = number[number_size - i - 1];
		digitsCheck[number_size - i - 1] = wchar;
		std::uint16_t digit = static_cast<std::uint16_t>(wchar) - 48;
		output += digit * accumulate;
		accumulate = accumulate * 10;
	}
	if (number_size == wchars_size)
	{
		for (std::size_t i = 0; i < wchars_size; ++i)
		{
			if (digitsCheck[i] < wchars[i]) { break; }
			else if (digitsCheck[i] > wchars[i]) { throw std::out_of_range("An integer is exceeded the limit."); }
		}
	}
	delete[] digitsCheck;
	return output;
};
inline std::size_t stos_t(const std::wstring& str)
{
	std::wstring result = std::regex_replace(str, std::wregex(L" "), L"");
	return wtos_t(result.c_str());
};
namespace Seden
{
	class Sedenion
	{
	private:
		static std::size_t GetDimension(std::size_t Value)
		{
			std::size_t n = 0;
			while (Value > 0)
			{
				Value >>= static_cast<size_t>(1);
				++n;
			}
			return static_cast<size_t>(1) << n;
		};
	public:
		static const double pi;
		static const double e;
	private:
		///
		/// Initializer
		///
		double* data;
		std::size_t size;
		explicit Sedenion(const double* data, std::size_t size) : data{ new double[size] {} }, size{ size }
		{
			std::copy(data, data + size, this->data);
		};
	public:
		explicit Sedenion();
		explicit Sedenion(const std::initializer_list<double>& numbers);
		Sedenion(double Value);
		Sedenion(const Sedenion& Value);
		Sedenion(Sedenion&& Value) noexcept;
		~Sedenion() noexcept;
		static double Scalar(const Sedenion& Value);
		static Sedenion Vector(const Sedenion& Value);
		///
		/// Operators
		///
		Sedenion operator ()() const;
		double& operator [](std::int64_t i) &;
		const double& operator [](std::int64_t i) const&;
		friend bool operator ==(const Sedenion& Union, const Sedenion& Value);
		friend bool operator !=(const Sedenion& Union, const Sedenion& Value);
		friend Sedenion operator +(const Sedenion& Value);
		friend Sedenion operator -(const Sedenion& Value);
		friend Sedenion operator ~(const Sedenion& Value);
		Sedenion& operator ++() &;
		Sedenion operator ++(int) &;
		Sedenion& operator --() &;
		Sedenion operator --(int) &;
		friend Sedenion operator +(const Sedenion& Union, const Sedenion& Value);
		friend Sedenion operator -(const Sedenion& Union, const Sedenion& Value);
		friend Sedenion operator *(const Sedenion& Union, const Sedenion& Value);
		friend Sedenion operator /(const Sedenion& Union, double Value);
		friend Sedenion operator ^(const Sedenion& Base, std::int64_t Exponent);
		Sedenion& operator =(const Sedenion& Value) &;
		Sedenion& operator =(Sedenion&& Value) & noexcept;
		Sedenion& operator +=(const Sedenion& Value) &;
		Sedenion& operator +=(const std::initializer_list<Sedenion>& Value) &;
		Sedenion& operator -=(const Sedenion& Value) &;
		Sedenion& operator -=(const std::initializer_list<Sedenion>& Value) &;
		Sedenion& operator *=(const Sedenion& Value) &;
		Sedenion& operator *=(const std::initializer_list<Sedenion>& Value) &;
		Sedenion& operator /=(double Value) &;
		Sedenion& operator /=(const std::initializer_list<double>& Value) &;
		Sedenion& operator ^=(std::int64_t Exponent) &;
		Sedenion& operator ^=(const std::initializer_list<std::int64_t>& Exponent) &;
		///
		/// Basic functions for constructing numbers
		///
		static double abs(const Sedenion& Value);
		static double arg(const Sedenion& Value);
		static double arg(const Sedenion& Value, std::int64_t Theta);
		static Sedenion conjg(const Sedenion& Value);
		static Sedenion sgn(const Sedenion& Value);
		static Sedenion inverse(const Sedenion& Value);
		static Sedenion exp(const Sedenion& Value);
		static Sedenion ln(const Sedenion& Value);
		static Sedenion ln(const Sedenion& Value, std::int64_t Theta);
		///
		/// 1st rank tensor algorithms
		///
		static double dot(const Sedenion& Union, const Sedenion& Value);
		static Sedenion outer(const Sedenion& Union, const Sedenion& Value);
		static Sedenion even(const Sedenion& Union, const Sedenion& Value);
		static Sedenion cross(const Sedenion& Union, const Sedenion& Value);
		///
		/// Operation 3 algorithms
		///
		static Sedenion Power(const Sedenion& Base, const Sedenion& Exponent);
		static Sedenion Power(const Sedenion& Base, const Sedenion& Exponent, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau);
		static Sedenion Power(const Sedenion& Base, double Exponent);
		static Sedenion Power(const Sedenion& Base, double Exponent, std::int64_t Theta);
		static Sedenion Root(const Sedenion& Base, const Sedenion& Exponent);
		static Sedenion Root(const Sedenion& Base, const Sedenion& Exponent, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau);
		static Sedenion Root(const Sedenion& Base, double Exponent);
		static Sedenion Root(const Sedenion& Base, double Exponent, std::int64_t Theta);
		static Sedenion Log(const Sedenion& Base, const Sedenion& Number);
		static Sedenion Log(const Sedenion& Base, const Sedenion& Number, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau, std::int64_t Omega);
		///
		/// Trigonometric functions
		///
		static Sedenion sin(const Sedenion& Value);
		static Sedenion arcsin(const Sedenion& Value);
		static Sedenion arcsin(const Sedenion& Value, bool Sign, std::int64_t Period);
		static Sedenion sinh(const Sedenion& Value);
		static Sedenion arcsinh(const Sedenion& Value);
		static Sedenion arcsinh(const Sedenion& Value, bool Sign, std::int64_t Period);
		static Sedenion cos(const Sedenion& Value);
		static Sedenion arccos(const Sedenion& Value);
		static Sedenion arccos(const Sedenion& Value, bool Sign, std::int64_t Period);
		static Sedenion cosh(const Sedenion& Value);
		static Sedenion arccosh(const Sedenion& Value);
		static Sedenion arccosh(const Sedenion& Value, bool Sign, std::int64_t Period);
		static Sedenion tan(const Sedenion& Value);
		static Sedenion arctan(const Sedenion& Value);
		static Sedenion arctan(const Sedenion& Value, bool Sign, std::int64_t Period);
		static Sedenion tanh(const Sedenion& Value);
		static Sedenion arctanh(const Sedenion& Value);
		static Sedenion arctanh(const Sedenion& Value, bool Sign, std::int64_t Period);
		static Sedenion csc(const Sedenion& Value);
		static Sedenion arccsc(const Sedenion& Value);
		static Sedenion arccsc(const Sedenion& Value, bool Sign, std::int64_t Period);
		static Sedenion csch(const Sedenion& Value);
		static Sedenion arccsch(const Sedenion& Value);
		static Sedenion arccsch(const Sedenion& Value, bool Sign, std::int64_t Period);
		static Sedenion sec(const Sedenion& Value);
		static Sedenion arcsec(const Sedenion& Value);
		static Sedenion arcsec(const Sedenion& Value, bool Sign, std::int64_t Period);
		static Sedenion sech(const Sedenion& Value);
		static Sedenion arcsech(const Sedenion& Value);
		static Sedenion arcsech(const Sedenion& Value, bool Sign, std::int64_t Period);
		static Sedenion cot(const Sedenion& Value);
		static Sedenion arccot(const Sedenion& Value);
		static Sedenion arccot(const Sedenion& Value, bool Sign, std::int64_t Period);
		static Sedenion coth(const Sedenion& Value);
		static Sedenion arccoth(const Sedenion& Value);
		static Sedenion arccoth(const Sedenion& Value, bool Sign, std::int64_t Period);
		///
		/// Conversion of Types
		///
		static std::wstring CType_String(const Sedenion& Value);
		static Sedenion CType_Sedenion(const std::wstring& Value);
		/* Casting */
	private:
		Factor to_factor() const& noexcept { return Factor{ data, size }; };
		static Sedenion from(const Factor& number)
		{
			std::size_t Size = number.get_size();
			double* Temp = new double[Size] {};
			std::copy(number.get_data(), number.get_data() + Size, Temp);
			Sedenion Output{ Temp, Size };
			delete[] Temp;
			return Output;
		};
	};
	bool operator ==(const Sedenion& Union, const Sedenion& Value);
	bool operator !=(const Sedenion& Union, const Sedenion& Value);
	Sedenion operator +(const Sedenion& Value);
	Sedenion operator -(const Sedenion& Value);
	Sedenion operator ~(const Sedenion& Value);
	Sedenion operator +(const Sedenion& Union, const Sedenion& Value);
	Sedenion operator -(const Sedenion& Union, const Sedenion& Value);
	Sedenion operator *(const Sedenion& Union, const Sedenion& Value);
	Sedenion operator /(const Sedenion& Union, double Value);
	Sedenion operator ^(const Sedenion& Base, std::int64_t Exponent);
	/* suffix operator */
	inline Sedenion operator"" _s(long double Value) { return Sedenion(static_cast<double>(Value)); };
	inline Sedenion operator"" _s(unsigned long long int Value) { return operator"" _s(static_cast<long double>(Value)); };
	/* class Sedenion */
	const double Sedenion::pi = std::numbers::pi;
	const double Sedenion::e = std::numbers::e;
	///
	/// Initializer
	///
	Sedenion::Sedenion()
		: data(new double[16] {}), size(16) {};
	Sedenion::Sedenion(const std::initializer_list<double>& numbers)
		: data(new double[numbers.size()] {}), size(numbers.size())
	{
		std::copy(numbers.begin(), numbers.end(), this->data);
	};
	Sedenion::Sedenion(double Value)
		: data(new double[1] { Value }), size(1) {};
	Sedenion::Sedenion(const Sedenion& Value)
		: data(new double[Value.size] {}), size(Value.size)
	{
		std::copy(Value.data, Value.data + Value.size, this->data);
	};
	Sedenion::Sedenion(Sedenion&& Value) noexcept
		: data(Value.data), size(Value.size)
	{
		Value.data = nullptr;
		Value.size = 0;
	};
	Sedenion::~Sedenion() noexcept { delete[] this->data; };
	double Sedenion::Scalar(const Sedenion& Value) { return Value[0]; };
	Sedenion Sedenion::Vector(const Sedenion& Value)
	{
		Sedenion ret = Value;
		ret[0] = 0;
		return ret;
	};
	///
	/// Operators
	///
	Sedenion Sedenion::operator ()() const { return *this; };
	double& Sedenion::operator [](std::int64_t i) &
	{
		return this->data[i % this->size];
	};
	const double& Sedenion::operator [](std::int64_t i) const&
	{
		return this->data[i % this->size];
	};
	bool operator ==(const Sedenion& Union, const Sedenion& Value) { return std::equal(Union.data, Union.data + Union.size, Value.data); };
	bool operator !=(const Sedenion& Union, const Sedenion& Value) { return !(Union == Value); };
	Sedenion operator +(const Sedenion& Value) { return Value; };
	Sedenion operator -(const Sedenion& Value) { return Sedenion::from(-Value.to_factor()); };
	Sedenion operator ~(const Sedenion& Value) { return Sedenion::from(~Value.to_factor()); };
	Sedenion& Sedenion::operator ++() &
	{
		++this->operator[](0);
		return *this;
	};
	Sedenion Sedenion::operator ++(int) &
	{
		Sedenion temp = *this;
		++this->operator[](0);
		return temp;
	};
	Sedenion& Sedenion::operator --() &
	{
		--this->operator[](0);
		return *this;
	};
	Sedenion Sedenion::operator --(int) &
	{
		Sedenion temp = *this;
		--this->operator[](0);
		return temp;
	};
	Sedenion operator +(const Sedenion& Union, const Sedenion& Value)
	{
		return Sedenion::from(Union.to_factor() + Value.to_factor());
	};
	Sedenion operator -(const Sedenion& Union, const Sedenion& Value)
	{
		return Sedenion::from(Union.to_factor() - Value.to_factor());
	};
	Sedenion operator *(const Sedenion& Union, const Sedenion& Value)
	{
		return Sedenion::from(Union.to_factor() * Value.to_factor());
	};
	Sedenion operator /(const Sedenion& Union, double Value)
	{
		return Sedenion::from(Union.to_factor() / Value);
	};
	Sedenion operator ^(const Sedenion& Base, std::int64_t Exponent)
	{
		try { return Sedenion::Power(Base, static_cast<double>(Exponent)); }
		catch (...) { return 0; }
	};
	Sedenion& Sedenion::operator =(const Sedenion& Value) &
	{
		if (this == &Value) { return *this; }
		delete[] this->data;
		this->data = new double[Value.size] {};
		std::copy(Value.data, Value.data + Value.size, this->data);
		this->size = Value.size;
		return *this;
	};
	Sedenion& Sedenion::operator =(Sedenion&& Value) & noexcept
	{
		this->data = Value.data;
		this->size = Value.size;
		Value.data = nullptr;
		Value.size = 0;
		return *this;
	};
	Sedenion& Sedenion::operator +=(const Sedenion& Value) & { return *this = *this + Value; };
	Sedenion& Sedenion::operator +=(const std::initializer_list<Sedenion>& Value) &
	{
		for (std::initializer_list<Sedenion>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this += *ite; }
		return *this;
	};
	Sedenion& Sedenion::operator -=(const Sedenion& Value) & { return *this = *this - Value; };
	Sedenion& Sedenion::operator -=(const std::initializer_list<Sedenion>& Value) &
	{
		for (std::initializer_list<Sedenion>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this -= *ite; }
		return *this;
	};
	Sedenion& Sedenion::operator *=(const Sedenion& Value) & { return *this = *this * Value; };
	Sedenion& Sedenion::operator *=(const std::initializer_list<Sedenion>& Value) &
	{
		for (std::initializer_list<Sedenion>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this *= *ite; }
		return *this;
	};
	Sedenion& Sedenion::operator /=(double Value) & { return *this = *this / Value; };
	Sedenion& Sedenion::operator /=(const std::initializer_list<double>& Value) &
	{
		for (std::initializer_list<double>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this /= *ite; }
		return *this;
	};
	Sedenion& Sedenion::operator ^=(std::int64_t Exponent) & { return *this = *this ^ Exponent; };
	Sedenion& Sedenion::operator ^=(const std::initializer_list<std::int64_t>& Exponent) &
	{
		for (std::initializer_list<std::int64_t>::const_iterator ite = Exponent.begin(); ite != Exponent.end(); ++ite) { *this ^= *ite; }
		return *this;
	};
	///
	/// Basic functions for constructing numbers
	///
	double Sedenion::abs(const Sedenion& Value) { return std::sqrt(dot(Value, Value)); };
	double Sedenion::arg(const Sedenion& Value) { return arg(Value, 0); };
	double Sedenion::arg(const Sedenion& Value, std::int64_t Theta) { return std::acos(Scalar(Value) / abs(Value)) + 2 * pi * static_cast<double>(Theta); };
	Sedenion Sedenion::conjg(const Sedenion& Value) { return ~Value; };
	Sedenion Sedenion::sgn(const Sedenion& Value) { return Value / abs(Value); };
	Sedenion Sedenion::inverse(const Sedenion& Value) { return conjg(Value) / dot(Value, Value); };
	Sedenion Sedenion::exp(const Sedenion& Value) { return std::exp(Scalar(Value)) * (std::cos(abs(Vector(Value))) + sgn(Vector(Value)) * std::sin(abs(Vector(Value)))); };
	Sedenion Sedenion::ln(const Sedenion& Value) { return ln(Value, 0); };
	Sedenion Sedenion::ln(const Sedenion& Value, std::int64_t Theta) { return std::log(abs(Value)) + sgn(Vector(Value)) * arg(Value, Theta); };
	///
	/// 1st rank tensor algorithms
	///
	double Sedenion::dot(const Sedenion& Union, const Sedenion& Value) { return Scalar(conjg(Union) * Value + conjg(Value) * Union) / 2; };
	Sedenion Sedenion::outer(const Sedenion& Union, const Sedenion& Value) { return (conjg(Union) * Value - conjg(Value) * Union) / 2; };
	Sedenion Sedenion::even(const Sedenion& Union, const Sedenion& Value) { return (Union * Value + Value * Union) / 2; };
	Sedenion Sedenion::cross(const Sedenion& Union, const Sedenion& Value) { return (Union * Value - Value * Union) / 2; };
	///
	/// Operation 3 algorithms
	///
	Sedenion Sedenion::Power(const Sedenion& Base, const Sedenion& Exponent) { return Power(Base, Exponent, 0, 0, 0); };
	Sedenion Sedenion::Power(const Sedenion& Base, const Sedenion& Exponent, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau)
	{
		return exp(exp(ln(ln(Base, Theta), Phi) + ln(Exponent, Tau)));
	};
	Sedenion Sedenion::Power(const Sedenion& Base, double Exponent) { return Power(Base, Exponent, 0); }
	Sedenion Sedenion::Power(const Sedenion& Base, double Exponent, std::int64_t Theta)
	{
		if (Base == 0) { return Exponent == 0 ? 1 : 0; }
		return std::pow(abs(Base), Exponent) *
			(std::cos(Exponent * arg(Base, Theta)) + sgn(Vector(Base)) * std::sin(Exponent * arg(Base, Theta)));
	};
	Sedenion Sedenion::Root(const Sedenion& Base, const Sedenion& Exponent) { return Root(Base, Exponent, 0, 0, 0); };
	Sedenion Sedenion::Root(const Sedenion& Base, const Sedenion& Exponent, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau) { return Power(Base, inverse(Exponent), Theta, Phi, Tau); };
	Sedenion Sedenion::Root(const Sedenion& Base, double Exponent) { return Power(Base, 1 / Exponent); };
	Sedenion Sedenion::Root(const Sedenion& Base, double Exponent, std::int64_t Theta) { return Power(Base, 1 / Exponent, Theta); };
	Sedenion Sedenion::Log(const Sedenion& Base, const Sedenion& Number) { return Log(Base, Number, 0, 0, 0, 0); };
	Sedenion Sedenion::Log(const Sedenion& Base, const Sedenion& Number, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau, std::int64_t Omega)
	{
		return exp(ln(ln(Number, Theta), Phi) - ln(ln(Base, Tau), Omega));
	};
	///
	/// Trigonometric functions
	///
	Sedenion Sedenion::sin(const Sedenion& Value)
	{
		return std::sin(Scalar(Value)) * std::cosh(abs(Vector(Value)))
			+ std::cos(Scalar(Value)) * sgn(Vector(Value)) * std::sinh(abs(Vector(Value)));
	};
	Sedenion Sedenion::arcsin(const Sedenion& Value) { return arcsin(Value, true, 0); };
	Sedenion Sedenion::arcsin(const Sedenion& Value, bool Sign, std::int64_t Period)
	{
		if (Sign == true) { return -sgn(Vector(Value)) * arcsinh(Value * sgn(Vector(Value))); }
		else { return pi - arcsin(Value, true, Period); }
	};
	Sedenion Sedenion::sinh(const Sedenion& Value)
	{
		return std::sinh(Scalar(Value)) * std::cos(abs(Vector(Value)))
			+ std::cosh(Scalar(Value)) * sgn(abs(Vector(Value))) * std::sin(abs(Vector(Value)));
	};
	Sedenion Sedenion::arcsinh(const Sedenion& Value) { return arcsinh(Value, true, 0); };
	Sedenion Sedenion::arcsinh(const Sedenion& Value, bool Sign, std::int64_t Period)
	{
		if (Sign == true) { return ln(Value + Root(Power(Value, 2) + 1, 2), Period); }
		else { return pi * sgn(Vector(Value)) - arcsinh(Value, true, Period); }
	};
	Sedenion Sedenion::cos(const Sedenion& Value)
	{
		return std::cos(Scalar(Value)) * std::cosh(abs(Vector(Value)))
			- std::sin(Scalar(Value)) * sgn(Vector(Value)) * std::sinh(abs(Vector(Value)));
	};
	Sedenion Sedenion::arccos(const Sedenion& Value) { return arccos(Value, true, 0); };
	Sedenion Sedenion::arccos(const Sedenion& Value, bool Sign, std::int64_t Period)
	{
		if (Sign == true) { return -sgn(Vector(Value)) * arccosh(Value); }
		else { return 2 * pi - arccos(Value, true, Period); }
	};
	Sedenion Sedenion::cosh(const Sedenion& Value)
	{
		return std::cosh(Scalar(Value)) * std::cos(abs(Vector(Value)))
			+ std::sinh(Scalar(Value)) * sgn(abs(Vector(Value))) * std::sin(abs(Vector(Value)));
	};
	Sedenion Sedenion::arccosh(const Sedenion& Value) { return arccosh(Value, true, 0); };
	Sedenion Sedenion::arccosh(const Sedenion& Value, bool Sign, std::int64_t Period)
	{
		if (Sign == true) { return ln(Value + Root(Power(Value, 2) - 1, 2), Period); }
		else { return 2 * pi * sgn(Vector(Value)) - arccosh(Value, true, Period); }
	};
	Sedenion Sedenion::tan(const Sedenion& Value)
	{
		return Root(Power(sec(Value), 2) - 1, 2);
	};
	Sedenion Sedenion::arctan(const Sedenion& Value) { return arctan(Value, true, 0); };
	Sedenion Sedenion::arctan(const Sedenion& Value, bool Sign, std::int64_t Period)
	{
		if (Sign == true) { return -sgn(Vector(Value)) * arctanh(Value * sgn(Vector(Value))); }
		else { return pi + arctan(Value, true, Period); }
	};
	Sedenion Sedenion::tanh(const Sedenion& Value)
	{
		return 1 - 2 * inverse(exp(2 * Value) + 1);
	};
	Sedenion Sedenion::arctanh(const Sedenion& Value) { return arctanh(Value, true, 0); };
	Sedenion Sedenion::arctanh(const Sedenion& Value, bool Sign, std::int64_t Period)
	{
		if (Sign == true) { return (ln(1 + Value, Period) - ln(1 - Value)) / 2; }
		else { return pi * sgn(Vector(Value)) + arctanh(Value, true, Period); }
	};
	Sedenion Sedenion::csc(const Sedenion& Value) { return inverse(sin(Value)); };
	Sedenion Sedenion::arccsc(const Sedenion& Value) { return arccsc(Value, true, 0); };
	Sedenion Sedenion::arccsc(const Sedenion& Value, bool Sign, std::int64_t Period) { return arcsin(inverse(Value), Sign, Period); };
	Sedenion Sedenion::csch(const Sedenion& Value) { return inverse(sinh(Value)); };
	Sedenion Sedenion::arccsch(const Sedenion& Value) { return arccsch(Value, true, 0); };
	Sedenion Sedenion::arccsch(const Sedenion& Value, bool Sign, std::int64_t Period) { return arcsinh(inverse(Value), Sign, Period); };
	Sedenion Sedenion::sec(const Sedenion& Value) { return inverse(cos(Value)); };
	Sedenion Sedenion::arcsec(const Sedenion& Value) { return arcsec(Value, true, 0); };
	Sedenion Sedenion::arcsec(const Sedenion& Value, bool Sign, std::int64_t Period) { return arccos(inverse(Value), Sign, Period); };
	Sedenion Sedenion::sech(const Sedenion& Value) { return inverse(cosh(Value)); };
	Sedenion Sedenion::arcsech(const Sedenion& Value) { return arcsech(Value, true, 0); };
	Sedenion Sedenion::arcsech(const Sedenion& Value, bool Sign, std::int64_t Period) { return arccosh(inverse(Value), Sign, Period); };
	Sedenion Sedenion::cot(const Sedenion& Value) { return inverse(tan(Value)); };
	Sedenion Sedenion::arccot(const Sedenion& Value) { return arccot(Value, true, 0); };
	Sedenion Sedenion::arccot(const Sedenion& Value, bool Sign, std::int64_t Period) { return arctan(inverse(Value), Sign, Period); };
	Sedenion Sedenion::coth(const Sedenion& Value) { return inverse(tanh(Value)); };
	Sedenion Sedenion::arccoth(const Sedenion& Value) { return arccoth(Value, true, 0); };
	Sedenion Sedenion::arccoth(const Sedenion& Value, bool Sign, std::int64_t Period) { return arctanh(inverse(Value), Sign, Period); };
	///
	/// Conversion of Types
	///
	std::wstring Sedenion::CType_String(const Sedenion& Value)
	{
		const double* Numbers = Value.data;
		std::wstring* Strings = new std::wstring[Value.size] {};
		for (std::size_t i = 0; i < Value.size; ++i) { Strings[i] = L"e" + std::to_wstring(i); }
		std::wstring Output = ToString(Value.size, Numbers, Strings);
		delete[] Strings;
		return Output;
	};
	Sedenion Sedenion::CType_Sedenion(const std::wstring& Value)
	{
		std::size_t Dimension = 0;
		std::wregex Regex(L"e\\d+(?=-|\\+|$)");
		std::wstring TheString = std::regex_replace(Value, std::wregex(L" "), L"");
		if (TheString == L"0") { return Sedenion{}; };
		std::wsmatch Match;
		while (std::regex_search(TheString, Match, Regex))
		{
			Dimension = std::max(Dimension, stos_t(Match.str().substr(1)));
			TheString = Match.suffix().str();
		}
		std::size_t Size = GetDimension(Dimension);
		double* Numbers = new double[Size] {};
		std::wstring* Strings = new std::wstring[Size] {};
		for (std::size_t i = 0; i < Size; ++i) { Strings[i] = L"e" + std::to_wstring(i); }
		ToNumbers(Value, Size, Numbers, Strings);
		delete[] Strings;
		return Sedenion{ Numbers, Size };
	};
}
