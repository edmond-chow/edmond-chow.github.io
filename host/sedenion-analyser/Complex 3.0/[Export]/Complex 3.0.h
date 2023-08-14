#pragma once
#include <string>
#include <stdexcept>
namespace Cmplx3
{
	namespace BaseType
	{
		enum struct index : std::int64_t
		{
			e1 = 1,
			e2 = 2,
			e3 = 3,
			e4 = 4,
			e5 = 5,
			e6 = 6,
			e7 = 7,
		};
		struct Vector7D
		{
		public:
			static const double pi;
			static const double e;
			static const Vector7D e1;
			static const Vector7D e2;
			static const Vector7D e3;
			static const Vector7D e4;
			static const Vector7D e5;
			static const Vector7D e6;
			static const Vector7D e7;
		private:
			///
			/// Initializer
			///
			double x1;
			double x2;
			double x3;
			double x4;
			double x5;
			double x6;
			double x7;
		public:
			explicit Vector7D() noexcept;
			explicit Vector7D(double x1, double x2, double x3, double x4, double x5, double x6, double x7) noexcept;
			Vector7D(const Vector7D& Value) noexcept;
			///
			/// Operators
			///
			Vector7D operator ()() const noexcept;
			double& operator [](index i) &;
			double& operator [](std::int64_t i) &;
			const double& operator [](index i) const&;
			const double& operator [](std::int64_t i) const&;
			Vector7D& operator =(const Vector7D& Value) & noexcept;
			Vector7D& operator +=(const Vector7D& Value) & noexcept;
			Vector7D& operator +=(const std::initializer_list<Vector7D>& Value) & noexcept;
			Vector7D& operator -=(const Vector7D& Value) & noexcept;
			Vector7D& operator -=(const std::initializer_list<Vector7D>& Value) & noexcept;
			Vector7D& operator *=(double Value) & noexcept;
			Vector7D& operator *=(const std::initializer_list<double>& Value) & noexcept;
			Vector7D& operator /=(double Value) & noexcept;
			Vector7D& operator /=(const std::initializer_list<double>& Value) & noexcept;
			///
			/// Basic functions for constructing vectors
			///
			static double abs(const Vector7D& Value);
			static Vector7D sgn(const Vector7D& Value);
			static double dot(const Vector7D& Union, const Vector7D& Value) noexcept;
			static Vector7D cross(const Vector7D& Union, const Vector7D& Value) noexcept;
			///
			/// Conversion of Types
			///
			static std::wstring CType_String(const Vector7D& Value);
			static Vector7D CType_Vector7D(const std::wstring& Value);
		};
		bool operator ==(const Vector7D& Union, const Vector7D& Value) noexcept;
		bool operator !=(const Vector7D& Union, const Vector7D& Value) noexcept;
		Vector7D operator +(const Vector7D& Value) noexcept;
		Vector7D operator -(const Vector7D& Value) noexcept;
		Vector7D operator +(const Vector7D& Union, const Vector7D& Value) noexcept;
		Vector7D operator -(const Vector7D& Union, const Vector7D& Value) noexcept;
		Vector7D operator *(double Union, const Vector7D& Value) noexcept;
		Vector7D operator *(const Vector7D& Union, double Value) noexcept;
		Vector7D operator /(const Vector7D& Union, double Value) noexcept;
	}
	namespace MainType
	{
		enum struct index : std::int64_t
		{
			s = 0,
			i = 1,
			j = 2,
			k = 3,
			l = 4,
			il = 5,
			jl = 6,
			kl = 7,
		};
		struct Octonion
		{
		public:
			static const double pi;
			static const double e;
			static const Octonion i;
			static const Octonion j;
			static const Octonion k;
			static const Octonion l;
			static const Octonion il;
			static const Octonion jl;
			static const Octonion kl;
		private:
			///
			/// Initializer
			///
			double real;
			BaseType::Vector7D imaginary;
		public:
			explicit Octonion() noexcept;
			explicit Octonion(double s, const BaseType::Vector7D& v) noexcept;
			explicit Octonion(double s, double i, double j, double k, double l, double il, double jl, double kl) noexcept;
			Octonion(double Value) noexcept;
			Octonion(const BaseType::Vector7D& Value) noexcept;
			Octonion(const Octonion& Value) noexcept;
			static double Scalar(const Octonion& Value) noexcept;
			static BaseType::Vector7D Vector(const Octonion& Value) noexcept;
			///
			/// Operators
			///
			Octonion operator ()() const noexcept;
			double& operator [](index i) &;
			double& operator [](std::int64_t i) &;
			const double& operator [](index i) const&;
			const double& operator [](std::int64_t i) const&;
			Octonion& operator ++() & noexcept;
			Octonion operator ++(int) & noexcept;
			Octonion& operator --() & noexcept;
			Octonion operator --(int) & noexcept;
			Octonion& operator =(const Octonion& Value) & noexcept;
			Octonion& operator +=(const Octonion& Value) & noexcept;
			Octonion& operator +=(const std::initializer_list<Octonion>& Value) & noexcept;
			Octonion& operator -=(const Octonion& Value) & noexcept;
			Octonion& operator -=(const std::initializer_list<Octonion>& Value) & noexcept;
			Octonion& operator *=(const Octonion& Value) & noexcept;
			Octonion& operator *=(const std::initializer_list<Octonion>& Value) & noexcept;
			Octonion& operator /= (const Octonion& Value) & noexcept;
			Octonion& operator /=(const std::initializer_list<Octonion>& Value) & noexcept;
			Octonion& operator ^=(std::int64_t Exponent) & noexcept;
			Octonion& operator ^=(const std::initializer_list<std::int64_t>& Exponent) & noexcept;
			///
			/// Basic functions for constructing numbers
			///
			static double abs(const Octonion& Value);
			static double arg(const Octonion& Value);
			static double arg(const Octonion& Value, std::int64_t Theta);
			static Octonion conjg(const Octonion& Value);
			static Octonion sgn(const Octonion& Value);
			static Octonion inverse(const Octonion& Value);
			static Octonion exp(const Octonion& Value);
			static Octonion ln(const Octonion& Value);
			static Octonion ln(const Octonion& Value, std::int64_t Theta);
			///
			/// 1st rank tensor algorithms
			///
			static double dot(const Octonion& Union, const Octonion& Value) noexcept;
			static Octonion outer(const Octonion& Union, const Octonion& Value) noexcept;
			static Octonion even(const Octonion& Union, const Octonion& Value) noexcept;
			static Octonion cross(const Octonion& Union, const Octonion& Value) noexcept;
			///
			/// Operation 3 algorithms
			///
			static Octonion Power(const Octonion& Base, const Octonion& Exponent);
			static Octonion Power(const Octonion& Base, const Octonion& Exponent, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau);
			static Octonion Power(const Octonion& Base, double Exponent);
			static Octonion Power(const Octonion& Base, double Exponent, std::int64_t Theta);
			static Octonion Root(const Octonion& Base, const Octonion& Exponent);
			static Octonion Root(const Octonion& Base, const Octonion& Exponent, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau);
			static Octonion Root(const Octonion& Base, double Exponent);
			static Octonion Root(const Octonion& Base, double Exponent, std::int64_t Theta);
			static Octonion Log(const Octonion& Base, const Octonion& Number);
			static Octonion Log(const Octonion& Base, const Octonion& Number, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau, std::int64_t Omega);
			///
			/// Trigonometric functions
			///
			static Octonion sin(const Octonion& Value);
			static Octonion arcsin(const Octonion& Value);
			static Octonion arcsin(const Octonion& Value, bool Sign, std::int64_t Period);
			static Octonion sinh(const Octonion& Value);
			static Octonion arcsinh(const Octonion& Value);
			static Octonion arcsinh(const Octonion& Value, bool Sign, std::int64_t Period);
			static Octonion cos(const Octonion& Value);
			static Octonion arccos(const Octonion& Value);
			static Octonion arccos(const Octonion& Value, bool Sign, std::int64_t Period);
			static Octonion cosh(const Octonion& Value);
			static Octonion arccosh(const Octonion& Value);
			static Octonion arccosh(const Octonion& Value, bool Sign, std::int64_t Period);
			static Octonion tan(const Octonion& Value);
			static Octonion arctan(const Octonion& Value);
			static Octonion arctan(const Octonion& Value, bool Sign, std::int64_t Period);
			static Octonion tanh(const Octonion& Value);
			static Octonion arctanh(const Octonion& Value);
			static Octonion arctanh(const Octonion& Value, bool Sign, std::int64_t Period);
			static Octonion csc(const Octonion& Value);
			static Octonion arccsc(const Octonion& Value);
			static Octonion arccsc(const Octonion& Value, bool Sign, std::int64_t Period);
			static Octonion csch(const Octonion& Value);
			static Octonion arccsch(const Octonion& Value);
			static Octonion arccsch(const Octonion& Value, bool Sign, std::int64_t Period);
			static Octonion sec(const Octonion& Value);
			static Octonion arcsec(const Octonion& Value);
			static Octonion arcsec(const Octonion& Value, bool Sign, std::int64_t Period);
			static Octonion sech(const Octonion& Value);
			static Octonion arcsech(const Octonion& Value);
			static Octonion arcsech(const Octonion& Value, bool Sign, std::int64_t Period);
			static Octonion cot(const Octonion& Value);
			static Octonion arccot(const Octonion& Value);
			static Octonion arccot(const Octonion& Value, bool Sign, std::int64_t Period);
			static Octonion coth(const Octonion& Value);
			static Octonion arccoth(const Octonion& Value);
			static Octonion arccoth(const Octonion& Value, bool Sign, std::int64_t Period);
			///
			/// Conversion of Types
			///
			static std::wstring CType_String(const Octonion& Value);
			static Octonion CType_Octonion(const std::wstring& Value);
		};
		bool operator ==(const Octonion& Union, const Octonion& Value) noexcept;
		bool operator !=(const Octonion& Union, const Octonion& Value) noexcept;
		Octonion operator +(const Octonion& Value) noexcept;
		Octonion operator -(const Octonion& Value) noexcept;
		Octonion operator ~(const Octonion& Value) noexcept;
		Octonion operator +(const Octonion& Union, const Octonion& Value) noexcept;
		Octonion operator -(const Octonion& Union, const Octonion& Value) noexcept;
		Octonion operator *(const Octonion& Union, const Octonion& Value) noexcept;
		Octonion operator /(const Octonion& Union, const Octonion& Value) noexcept;
		Octonion operator ^(const Octonion& Base, std::int64_t Exponent) noexcept;
		/* suffix operator */
		inline Octonion operator"" _o(long double Value) noexcept { return Octonion(static_cast<double>(Value)); };
		inline Octonion operator"" _o(unsigned long long int Value) noexcept { return operator"" _o(static_cast<long double>(Value)); };
	}
}
