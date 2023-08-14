#pragma once
#include <string>
#include <stdexcept>
namespace Cmplx2
{
	namespace BaseType
	{
		enum struct index : std::int64_t
		{
			e1 = 1,
			e2 = 2,
			e3 = 3,
		};
		struct Vector3D
		{
		public:
			static const double pi;
			static const double e;
			static const Vector3D e1;
			static const Vector3D e2;
			static const Vector3D e3;
		private:
			///
			/// Initializer
			///
			double x1;
			double x2;
			double x3;
		public:
			explicit Vector3D() noexcept;
			explicit Vector3D(double x1, double x2, double x3) noexcept;
			Vector3D(const Vector3D& Value) noexcept;
			///
			/// Operators
			///
			const Vector3D operator ()() const noexcept;
			double& operator [](index i) &;
			double& operator [](std::int64_t i) & noexcept;
			const double& operator [](index i) const&;
			const double& operator [](std::int64_t i) const& noexcept;
			Vector3D& operator =(const Vector3D& Value) & noexcept;
			Vector3D& operator +=(const Vector3D& Value) & noexcept;
			Vector3D& operator +=(const std::initializer_list<Vector3D>& Value) & noexcept;
			Vector3D& operator -=(const Vector3D& Value) & noexcept;
			Vector3D& operator -=(const std::initializer_list<Vector3D>& Value) & noexcept;
			Vector3D& operator *=(double Value) & noexcept;
			Vector3D& operator *=(const std::initializer_list<double>& Value) & noexcept;
			Vector3D& operator /=(double Value) & noexcept;
			Vector3D& operator /=(const std::initializer_list<double>& Value) & noexcept;
			///
			/// Basic functions for constructing vectors
			///
			static double abs(const Vector3D& Value);
			static Vector3D sgn(const Vector3D& Value);
			static double dot(const Vector3D& Union, const Vector3D& Value) noexcept;
			static Vector3D cross(const Vector3D& Union, const Vector3D& Value) noexcept;
			///
			/// Conversion of Types
			///
			static std::wstring CType_String(const Vector3D& Value);
			static Vector3D CType_Vector3D(const std::wstring& Value);
		};
		bool operator ==(const Vector3D& Union, const Vector3D& Value) noexcept;
		bool operator !=(const Vector3D& Union, const Vector3D& Value) noexcept;
		Vector3D operator +(const Vector3D& Value) noexcept;
		Vector3D operator -(const Vector3D& Value) noexcept;
		Vector3D operator +(const Vector3D& Union, const Vector3D& Value) noexcept;
		Vector3D operator -(const Vector3D& Union, const Vector3D& Value) noexcept;
		Vector3D operator *(double Union, const Vector3D& Value) noexcept;
		Vector3D operator *(const Vector3D& Union, double Value) noexcept;
		Vector3D operator /(const Vector3D& Union, double Value) noexcept;
	}
	namespace MainType
	{
		enum struct index : std::int64_t
		{
			s = 0,
			i = 1,
			j = 2,
			k = 3,
		};
		struct Quaternion
		{
		public:
			static const double pi;
			static const double e;
			static const Quaternion i;
			static const Quaternion j;
			static const Quaternion k;
		private:
			///
			/// Initializer
			///
			double real;
			BaseType::Vector3D imaginary;
		public:
			explicit Quaternion() noexcept;
			explicit Quaternion(double s, const BaseType::Vector3D& v) noexcept;
			explicit Quaternion(double s, double i, double j, double k) noexcept;
			Quaternion(double Value) noexcept;
			Quaternion(const BaseType::Vector3D& Value) noexcept;
			Quaternion(const Quaternion& Value) noexcept;
			static double Scalar(const Quaternion& Value) noexcept;
			static BaseType::Vector3D Vector(const Quaternion& Value) noexcept;
            ///
			/// Operators
			///
			const Quaternion operator ()() const noexcept;
			double& operator [](index i) &;
			double& operator [](std::int64_t i) & noexcept;
			const double& operator [](index i) const&;
			const double& operator [](std::int64_t i) const& noexcept;
			Quaternion& operator ++() & noexcept;
			const Quaternion operator ++(int) & noexcept;
			Quaternion& operator --() & noexcept;
			const Quaternion operator --(int) & noexcept;
			Quaternion& operator =(const Quaternion& Value) & noexcept;
			Quaternion& operator +=(const Quaternion& Value) & noexcept;
			Quaternion& operator +=(const std::initializer_list<Quaternion>& Value) & noexcept;
			Quaternion& operator -=(const Quaternion& Value) & noexcept;
			Quaternion& operator -=(const std::initializer_list<Quaternion>& Value) & noexcept;
			Quaternion& operator *=(const Quaternion& Value) & noexcept;
			Quaternion& operator *=(const std::initializer_list<Quaternion>& Value) & noexcept;
			Quaternion& operator /=(const Quaternion& Value) & noexcept;
			Quaternion& operator /=(const std::initializer_list<Quaternion>& Value) & noexcept;
			Quaternion& operator ^=(std::int64_t Exponent) & noexcept;
			Quaternion& operator ^=(const std::initializer_list<std::int64_t>& Exponent) & noexcept;
			///
			/// Basic functions for constructing numbers
			///
			static double abs(const Quaternion& Value);
			static double arg(const Quaternion& Value);
			static double arg(const Quaternion& Value, std::int64_t Theta);
			static Quaternion conjg(const Quaternion& Value);
			static Quaternion sgn(const Quaternion& Value);
			static Quaternion inverse(const Quaternion& Value);
			static Quaternion exp(const Quaternion& Value);
			static Quaternion ln(const Quaternion& Value);
			static Quaternion ln(const Quaternion& Value, std::int64_t Theta);
			///
			/// 1st rank tensor algorithms
			///
			static double dot(const Quaternion& Union, const Quaternion& Value) noexcept;
			static Quaternion outer(const Quaternion& Union, const Quaternion& Value) noexcept;
			static Quaternion even(const Quaternion& Union, const Quaternion& Value) noexcept;
			static Quaternion cross(const Quaternion& Union, const Quaternion& Value) noexcept;
			///
			/// Operation 3 algorithms
			///
			static Quaternion Power(const Quaternion& Base, const Quaternion& Exponent);
			static Quaternion Power(const Quaternion& Base, const Quaternion& Exponent, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau);
			static Quaternion Power(const Quaternion& Base, double Exponent);
			static Quaternion Power(const Quaternion& Base, double Exponent, std::int64_t Theta);
			static Quaternion Root(const Quaternion& Base, const Quaternion& Exponent);
			static Quaternion Root(const Quaternion& Base, const Quaternion& Exponent, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau);
			static Quaternion Root(const Quaternion& Base, double Exponent);
			static Quaternion Root(const Quaternion& Base, double Exponent, std::int64_t Theta);
			static Quaternion Log(const Quaternion& Base, const Quaternion& Number);
			static Quaternion Log(const Quaternion& Base, const Quaternion& Number, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau, std::int64_t Omega);
			///
			/// Trigonometric functions
			///
			static Quaternion sin(const Quaternion& Value);
			static Quaternion arcsin(const Quaternion& Value);
			static Quaternion arcsin(const Quaternion& Value, bool Sign, std::int64_t Period);
			static Quaternion sinh(const Quaternion& Value);
			static Quaternion arcsinh(const Quaternion& Value);
			static Quaternion arcsinh(const Quaternion& Value, bool Sign, std::int64_t Period);
			static Quaternion cos(const Quaternion& Value);
			static Quaternion arccos(const Quaternion& Value);
			static Quaternion arccos(const Quaternion& Value, bool Sign, std::int64_t Period);
			static Quaternion cosh(const Quaternion& Value);
			static Quaternion arccosh(const Quaternion& Value);
			static Quaternion arccosh(const Quaternion& Value, bool Sign, std::int64_t Period);
			static Quaternion tan(const Quaternion& Value);
			static Quaternion arctan(const Quaternion& Value);
			static Quaternion arctan(const Quaternion& Value, bool Sign, std::int64_t Period);
			static Quaternion tanh(const Quaternion& Value);
			static Quaternion arctanh(const Quaternion& Value);
			static Quaternion arctanh(const Quaternion& Value, bool Sign, std::int64_t Period);
			static Quaternion csc(const Quaternion& Value);
			static Quaternion arccsc(const Quaternion& Value);
			static Quaternion arccsc(const Quaternion& Value, bool Sign, std::int64_t Period);
			static Quaternion csch(const Quaternion& Value);
			static Quaternion arccsch(const Quaternion& Value);
			static Quaternion arccsch(const Quaternion& Value, bool Sign, std::int64_t Period);
			static Quaternion sec(const Quaternion& Value);
			static Quaternion arcsec(const Quaternion& Value);
			static Quaternion arcsec(const Quaternion& Value, bool Sign, std::int64_t Period);
			static Quaternion sech(const Quaternion& Value);
			static Quaternion arcsech(const Quaternion& Value);
			static Quaternion arcsech(const Quaternion& Value, bool Sign, std::int64_t Period);
			static Quaternion cot(const Quaternion& Value);
			static Quaternion arccot(const Quaternion& Value);
			static Quaternion arccot(const Quaternion& Value, bool Sign, std::int64_t Period);
			static Quaternion coth(const Quaternion& Value);
			static Quaternion arccoth(const Quaternion& Value);
			static Quaternion arccoth(const Quaternion& Value, bool Sign, std::int64_t Period);
			///
			/// Conversion of Types
			///
			static std::wstring CType_String(const Quaternion& Value);
			static Quaternion CType_Quaternion(const std::wstring& Value);
		};
		bool operator ==(const Quaternion& Union, const Quaternion& Value) noexcept;
		bool operator !=(const Quaternion& Union, const Quaternion& Value) noexcept;
		Quaternion operator +(const Quaternion& Value) noexcept;
		Quaternion operator -(const Quaternion& Value) noexcept;
		Quaternion operator ~(const Quaternion& Value) noexcept;
		Quaternion operator +(const Quaternion& Union, const Quaternion& Value) noexcept;
		Quaternion operator -(const Quaternion& Union, const Quaternion& Value) noexcept;
		Quaternion operator *(const Quaternion& Union, const Quaternion& Value) noexcept;
		Quaternion operator /(const Quaternion& Union, const Quaternion& Value) noexcept;
		Quaternion operator ^(const Quaternion& Base, std::int64_t Exponent) noexcept;
		/* suffix operator */
		inline Quaternion operator"" _q(long double Value) noexcept { return Quaternion(static_cast<double>(Value)); };
		inline Quaternion operator"" _q(unsigned long long int Value) noexcept { return operator"" _q(static_cast<long double>(Value)); };
	}
}
