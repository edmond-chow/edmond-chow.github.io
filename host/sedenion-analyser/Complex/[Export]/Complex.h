#pragma once
#include <csetjmp>
#include <string>
#include <stdexcept>
namespace Cmplx
{
	void set_throw(jmp_buf* const stack, std::wstring* const type, std::wstring* const what);
	namespace BaseType
	{
		enum struct index : std::int64_t
		{
			e1 = 1,
		};
		struct Vector1D
		{
		public:
			static const double pi;
			static const double e;
			static const Vector1D e1;
		private:
			///
			/// Initializer
			///
			double x1;
		public:
			explicit Vector1D() noexcept;
			explicit Vector1D(double x1) noexcept;
			Vector1D(const Vector1D& Value) noexcept;
			///
			/// Operators
			///
			Vector1D operator ()() const noexcept;
			double& operator [](index i) &;
			double& operator [](std::int64_t i) & noexcept;
			const double& operator [](index i) const&;
			const double& operator [](std::int64_t i) const& noexcept;
			Vector1D& operator =(const Vector1D& Value) & noexcept;
			Vector1D& operator +=(const Vector1D& Value) & noexcept;
			Vector1D& operator +=(const std::initializer_list<Vector1D>& Value) & noexcept;
			Vector1D& operator -=(const Vector1D& Value) & noexcept;
			Vector1D& operator -=(const std::initializer_list<Vector1D>& Value) & noexcept;
			Vector1D& operator *=(double Value) & noexcept;
			Vector1D& operator *=(const std::initializer_list<double>& Value) & noexcept;
			Vector1D& operator /=(double Value) & noexcept;
			Vector1D& operator /=(const std::initializer_list<double>& Value) & noexcept;
			///
			/// Basic functions for constructing vectors
			///
			static double abs(const Vector1D& Value);
			static Vector1D sgn(const Vector1D& Value);
			static double dot(const Vector1D& Union, const Vector1D& Value) noexcept;
			static Vector1D cross(const Vector1D& Union, const Vector1D& Value) noexcept;
			///
			/// Conversion of Types
			///
			static std::wstring CType_String(const Vector1D& Value);
			static Vector1D CType_Vector1D(const std::wstring& Value);
		};
		bool operator ==(const Vector1D& Union, const Vector1D& Value) noexcept;
		bool operator !=(const Vector1D& Union, const Vector1D& Value) noexcept;
		Vector1D operator +(const Vector1D& Value) noexcept;
		Vector1D operator -(const Vector1D& Value) noexcept;
		Vector1D operator +(const Vector1D& Union, const Vector1D& Value) noexcept;
		Vector1D operator -(const Vector1D& Union, const Vector1D& Value) noexcept;
		Vector1D operator *(double Union, const Vector1D& Value) noexcept;
		Vector1D operator *(const Vector1D& Union, double Value) noexcept;
		Vector1D operator /(const Vector1D& Union, double Value) noexcept;
	}
	namespace MainType
	{
		enum struct index : std::int64_t
		{
			s = 0,
			i = 1,
		};
		struct Complex
		{
		public:
			static const double pi;
			static const double e;
			static const Complex i;
		private:
			///
			/// Initializer
			///
			double real;
			BaseType::Vector1D imaginary;
		public:
			explicit Complex() noexcept;
			explicit Complex(double s, const BaseType::Vector1D& v) noexcept;
			explicit Complex(double s, double i) noexcept;
			Complex(double Value) noexcept;
			Complex(const BaseType::Vector1D& Value) noexcept;
			Complex(const Complex& Value) noexcept;
			static double Scalar(const Complex& Value) noexcept;
			static BaseType::Vector1D Vector(const Complex& Value) noexcept;
			static double Re(const Complex& z) noexcept;
			static double Im(const Complex& z) noexcept;
			///
			/// Operators
			///
			Complex operator ()() const noexcept;
			double& operator [](index i) &;
			double& operator [](std::int64_t i) & noexcept;
			const double& operator [](index i) const&;
			const double& operator [](std::int64_t i) const& noexcept;
			Complex& operator ++() & noexcept;
			Complex operator ++(int) & noexcept;
			Complex& operator --() & noexcept;
			Complex operator --(int) & noexcept;
			Complex& operator =(const Complex& Value) & noexcept;
			Complex& operator +=(const Complex& Value) & noexcept;
			Complex& operator +=(const std::initializer_list<Complex>& Value) & noexcept;
			Complex& operator -=(const Complex& Value) & noexcept;
			Complex& operator -=(const std::initializer_list<Complex>& Value) & noexcept;
			Complex& operator *=(const Complex& Value) & noexcept;
			Complex& operator *=(const std::initializer_list<Complex>& Value) & noexcept;
			Complex& operator /=(const Complex& Value) & noexcept;
			Complex& operator /=(const std::initializer_list<Complex>& Value) & noexcept;
			Complex& operator ^=(std::int64_t Exponent) & noexcept;
			Complex& operator ^=(const std::initializer_list<std::int64_t>& Exponent) & noexcept;
			///
			/// Basic functions for constructing numbers
			///
			static double abs(const Complex& Value);
			static double arg(const Complex& Value);
			static double arg(const Complex& Value, std::int64_t Theta);
			static Complex conjg(const Complex& Value);
			static Complex sgn(const Complex& Value);
			static Complex inverse(const Complex& Value);
			static Complex exp(const Complex& Value);
			static Complex ln(const Complex& Value);
			static Complex ln(const Complex& Value, std::int64_t Theta);
			///
			/// 1st rank tensor algorithms
			///
			static double dot(const Complex& Union, const Complex& Value) noexcept;
			static Complex outer(const Complex& Union, const Complex& Value) noexcept;
			static Complex even(const Complex& Union, const Complex& Value) noexcept;
			static Complex cross(const Complex& Union, const Complex& Value) noexcept;
			///
			/// Operation 3 algorithms
			///
			static Complex Power(const Complex& Base, const Complex& Exponent);
			static Complex Power(const Complex& Base, const Complex& Exponent, std::int64_t Theta);
			static Complex Power(const Complex& Base, double Exponent);
			static Complex Power(const Complex& Base, double Exponent, std::int64_t Theta);
			static Complex Root(const Complex& Base, const Complex& Exponent);
			static Complex Root(const Complex& Base, const Complex& Exponent, std::int64_t Theta);
			static Complex Root(const Complex& Base, double Exponent);
			static Complex Root(const Complex& Base, double Exponent, std::int64_t Theta);
			static Complex Log(const Complex& Base, const Complex& Number);
			static Complex Log(const Complex& Base, const Complex& Number, std::int64_t Theta, std::int64_t Phi);
			///
			/// Trigonometric functions
			///
			static Complex sin(const Complex& Value);
			static Complex arcsin(const Complex& Value);
			static Complex arcsin(const Complex& Value, bool Sign, std::int64_t Period);
			static Complex sinh(const Complex& Value);
			static Complex arcsinh(const Complex& Value);
			static Complex arcsinh(const Complex& Value, bool Sign, std::int64_t Period);
			static Complex cos(const Complex& Value);
			static Complex arccos(const Complex& Value);
			static Complex arccos(const Complex& Value, bool Sign, std::int64_t Period);
			static Complex cosh(const Complex& Value);
			static Complex arccosh(const Complex& Value);
			static Complex arccosh(const Complex& Value, bool Sign, std::int64_t Period);
			static Complex tan(const Complex& Value);
			static Complex arctan(const Complex& Value);
			static Complex arctan(const Complex& Value, bool Sign, std::int64_t Period);
			static Complex tanh(const Complex& Value);
			static Complex arctanh(const Complex& Value);
			static Complex arctanh(const Complex& Value, bool Sign, std::int64_t Period);
			static Complex csc(const Complex& Value);
			static Complex arccsc(const Complex& Value);
			static Complex arccsc(const Complex& Value, bool Sign, std::int64_t Period);
			static Complex csch(const Complex& Value);
			static Complex arccsch(const Complex& Value);
			static Complex arccsch(const Complex& Value, bool Sign, std::int64_t Period);
			static Complex sec(const Complex& Value);
			static Complex arcsec(const Complex& Value);
			static Complex arcsec(const Complex& Value, bool Sign, std::int64_t Period);
			static Complex sech(const Complex& Value);
			static Complex arcsech(const Complex& Value);
			static Complex arcsech(const Complex& Value, bool Sign, std::int64_t Period);
			static Complex cot(const Complex& Value);
			static Complex arccot(const Complex& Value);
			static Complex arccot(const Complex& Value, bool Sign, std::int64_t Period);
			static Complex coth(const Complex& Value);
			static Complex arccoth(const Complex& Value);
			static Complex arccoth(const Complex& Value, bool Sign, std::int64_t Period);
			///
			/// Conversion of Types
			///
			static std::wstring CType_String(const Complex& Value);
			static Complex CType_Complex(const std::wstring& Value);
		};
		bool operator ==(const Complex& Union, const Complex& Value) noexcept;
		bool operator !=(const Complex& Union, const Complex& Value) noexcept;
		Complex operator +(const Complex& Value) noexcept;
		Complex operator -(const Complex& Value) noexcept;
		Complex operator ~(const Complex& Value) noexcept;
		Complex operator +(const Complex& Union, const Complex& Value) noexcept;
		Complex operator -(const Complex& Union, const Complex& Value) noexcept;
		Complex operator *(const Complex& Union, const Complex& Value) noexcept;
		Complex operator /(const Complex& Union, const Complex& Value) noexcept;
		Complex operator ^(const Complex& Base, std::int64_t Exponent) noexcept;
		/* suffix operator */
		inline Complex operator"" _c(long double Value) noexcept { return Complex(static_cast<double>(Value)); };
		inline Complex operator"" _c(unsigned long long int Value) noexcept { return operator"" _c(static_cast<long double>(Value)); };
	}
}
