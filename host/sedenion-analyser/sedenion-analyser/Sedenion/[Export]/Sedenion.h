#pragma once
#include <csetjmp>
#include <string>
#include <stdexcept>
namespace Seden
{
	void set_throw(jmp_buf* const stack, std::wstring* const type, std::wstring* const what);
	class Sedenion
	{
	public:
		static const double pi;
		static const double e;
	private:
		///
		/// Initializer
		///
		double* data;
		std::size_t size;
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
		double& operator [](std::int64_t i)&;
		const double& operator [](std::int64_t i) const&;
		Sedenion& operator ++()&;
		Sedenion operator ++(int)&;
		Sedenion& operator --()&;
		Sedenion operator --(int)&;
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
		Sedenion& operator ^=(std::int64_t Exponent)&;
		Sedenion& operator ^=(const std::initializer_list<std::int64_t>& Exponent)&;
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
}
