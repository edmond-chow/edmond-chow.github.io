#include <cmath>
#include <numbers>
#include "Module.h"
#include "Cayley Dickson Algebra.h"
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
			friend bool operator ==(const Vector7D& Union, const Vector7D& Value) noexcept;
			friend bool operator !=(const Vector7D& Union, const Vector7D& Value) noexcept;
			friend Vector7D operator +(const Vector7D& Value) noexcept;
			friend Vector7D operator -(const Vector7D& Value) noexcept;
			friend Vector7D operator +(const Vector7D& Union, const Vector7D& Value) noexcept;
			friend Vector7D operator -(const Vector7D& Union, const Vector7D& Value) noexcept;
			friend Vector7D operator *(double Union, const Vector7D& Value) noexcept;
			friend Vector7D operator *(const Vector7D& Union, double Value) noexcept;
			friend Vector7D operator /(const Vector7D& Union, double Value) noexcept;
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
			/* Casting */
			inline auto to_number() const& noexcept
				-> decltype(forward_as_number(0, (*this)[index::e1], (*this)[index::e2], (*this)[index::e3], (*this)[index::e4], (*this)[index::e5], (*this)[index::e6], (*this)[index::e7]))
			{
				return forward_as_number(0, (*this)[index::e1], (*this)[index::e2], (*this)[index::e3], (*this)[index::e4], (*this)[index::e5], (*this)[index::e6], (*this)[index::e7]);
			};
			template <std::size_t N>
			inline auto from(const Number<N>& number) && noexcept -> decltype(*this)
			{
				double temp{};
				number.store(temp, (*this)[index::e1], (*this)[index::e2], (*this)[index::e3], (*this)[index::e4], (*this)[index::e5], (*this)[index::e6], (*this)[index::e7]);
				return *this;
			};
			static double build_in_dot(const Vector7D& Union, const Vector7D& Value);
			static Vector7D build_in_cross(const Vector7D& Union, const Vector7D& Value);
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
		/* struct Vector7D */
		const double Vector7D::pi = std::numbers::pi;
		const double Vector7D::e = std::numbers::e;
		const Vector7D Vector7D::e1 = Vector7D(1, 0, 0, 0, 0, 0, 0);
		const Vector7D Vector7D::e2 = Vector7D(0, 1, 0, 0, 0, 0, 0);
		const Vector7D Vector7D::e3 = Vector7D(0, 0, 1, 0, 0, 0, 0);
		const Vector7D Vector7D::e4 = Vector7D(0, 0, 0, 1, 0, 0, 0);
		const Vector7D Vector7D::e5 = Vector7D(0, 0, 0, 0, 1, 0, 0);
		const Vector7D Vector7D::e6 = Vector7D(0, 0, 0, 0, 0, 1, 0);
		const Vector7D Vector7D::e7 = Vector7D(0, 0, 0, 0, 0, 0, 1);
		///
		/// Initializer
		///
		Vector7D::Vector7D() noexcept : x1(), x2(), x3(), x4(), x5(), x6(), x7() {};
		Vector7D::Vector7D(double x1, double x2, double x3, double x4, double x5, double x6, double x7) noexcept : x1(x1), x2(x2), x3(x3), x4(x4), x5(x5), x6(x6), x7(x7) {};
		Vector7D::Vector7D(const Vector7D& Value) noexcept : x1(Value.x1), x2(Value.x2), x3(Value.x3), x4(Value.x4), x5(Value.x5), x6(Value.x6), x7(Value.x7) {};
		///
		/// Operators
		///
		Vector7D Vector7D::operator ()() const noexcept { return *this; };
		double& Vector7D::operator [](index i) &
		{
			switch (i)
			{
			case index::e1:
				return this->x1;
			case index::e2:
				return this->x2;
			case index::e3:
				return this->x3;
			case index::e4:
				return this->x4;
			case index::e5:
				return this->x5;
			case index::e6:
				return this->x6;
			case index::e7:
				return this->x7;
			default:
				throw_now(L"std::out_of_range", L"The index is out of range.");
			}
		};
		double& Vector7D::operator [](std::int64_t i) &
		{
			try { return this->operator[](static_cast<index>(PeriodShift(i, Period<Vector7D>))); }
			catch (...) { return this->operator[](i); }
		};
		const double& Vector7D::operator [](index i) const&
		{
			switch (i)
			{
			case index::e1:
				return this->x1;
			case index::e2:
				return this->x2;
			case index::e3:
				return this->x3;
			case index::e4:
				return this->x4;
			case index::e5:
				return this->x5;
			case index::e6:
				return this->x6;
			case index::e7:
				return this->x7;
			default:
				throw_now(L"std::out_of_range", L"The index is out of range.");
			}
		};
		const double& Vector7D::operator [](std::int64_t i) const&
		{
			try { return this->operator[](static_cast<index>(PeriodShift(i, Period<Vector7D>))); }
			catch (...) { return this->operator[](i); }
		};
		bool operator ==(const Vector7D& Union, const Vector7D& Value) noexcept { return Union.to_number() == Value.to_number(); };
		bool operator !=(const Vector7D& Union, const Vector7D& Value) noexcept { return !(Union == Value); };
		Vector7D operator +(const Vector7D& Value) noexcept { return Value; };
		Vector7D operator -(const Vector7D& Value) noexcept { return Vector7D{}.from(-Value.to_number()); };
		Vector7D operator +(const Vector7D& Union, const Vector7D& Value) noexcept { return Vector7D{}.from(Union.to_number() + Value.to_number()); };
		Vector7D operator -(const Vector7D& Union, const Vector7D& Value) noexcept { return Vector7D{}.from(Union.to_number() - Value.to_number()); };
		Vector7D operator *(double Union, const Vector7D& Value) noexcept { return Vector7D{}.from(Union * Value.to_number()); };
		Vector7D operator *(const Vector7D& Union, double Value) noexcept { return Vector7D{}.from(Union.to_number() * Value); };
		Vector7D operator /(const Vector7D& Union, double Value) noexcept { return Vector7D{}.from(Union.to_number() / Value); };
		Vector7D& Vector7D::operator =(const Vector7D& Value) & noexcept
		{
			this->x1 = Value.x1;
			this->x2 = Value.x2;
			this->x3 = Value.x3;
			this->x4 = Value.x4;
			this->x5 = Value.x5;
			this->x6 = Value.x6;
			this->x7 = Value.x7;
			return *this;
		};
		Vector7D& Vector7D::operator +=(const Vector7D& Value) & noexcept { return *this = *this + Value; };
		Vector7D& Vector7D::operator +=(const std::initializer_list<Vector7D>& Value) & noexcept
		{
			for (std::initializer_list<Vector7D>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this += *ite; }
			return *this;
		};
		Vector7D& Vector7D::operator -=(const Vector7D& Value) & noexcept { return *this = *this - Value; };
		Vector7D& Vector7D::operator -=(const std::initializer_list<Vector7D>& Value) & noexcept
		{
			for (std::initializer_list<Vector7D>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this -= *ite; }
			return *this;
		};
		Vector7D& Vector7D::operator *=(double Value) & noexcept { return *this = *this * Value; };
		Vector7D& Vector7D::operator *=(const std::initializer_list<double>& Value) & noexcept
		{
			for (std::initializer_list<double>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this *= *ite; }
			return *this;
		};
		Vector7D& Vector7D::operator /=(double Value) & noexcept { return *this = *this / Value; };
		Vector7D& Vector7D::operator /=(const std::initializer_list<double>& Value) & noexcept
		{
			for (std::initializer_list<double>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this /= *ite; }
			return *this;
		};
		///
		/// Basic functions for constructing vectors
		///
		double Vector7D::abs(const Vector7D& Value) { return std::sqrt(dot(Value, Value)); };
		Vector7D Vector7D::sgn(const Vector7D& Value) { return Value / abs(Value); };
		double Vector7D::dot(const Vector7D& Union, const Vector7D& Value) noexcept { return build_in_dot(Union, Value); };
		Vector7D Vector7D::cross(const Vector7D& Union, const Vector7D& Value) noexcept { return build_in_cross(Union, Value); };
		///
		/// Conversion of Types
		///
		std::wstring Vector7D::CType_String(const Vector7D& Value)
		{
			return ToString(Value[index::e1], L"e1", Value[index::e2], L"e2", Value[index::e3], L"e3", Value[index::e4], L"e4", Value[index::e5], L"e5", Value[index::e6], L"e6", Value[index::e7], L"e7");
		};
		Vector7D Vector7D::CType_Vector7D(const std::wstring& Value)
		{
			Vector7D Object;
			ToNumbers(Value, Object[index::e1], L"e1", Object[index::e2], L"e2", Object[index::e3], L"e3", Object[index::e4], L"e4", Object[index::e5], L"e5", Object[index::e6], L"e6", Object[index::e7], L"e7");
			return Object;
		};
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
			friend bool operator ==(const Octonion& Union, const Octonion& Value) noexcept;
			friend bool operator !=(const Octonion& Union, const Octonion& Value) noexcept;
			friend Octonion operator +(const Octonion& Value) noexcept;
			friend Octonion operator -(const Octonion& Value) noexcept;
			friend Octonion operator ~(const Octonion& Value) noexcept;
			Octonion& operator ++() & noexcept;
			Octonion operator ++(int) & noexcept;
			Octonion& operator --() & noexcept;
			Octonion operator --(int) & noexcept;
			friend Octonion operator +(const Octonion& Union, const Octonion& Value) noexcept;
			friend Octonion operator -(const Octonion& Union, const Octonion& Value) noexcept;
			friend Octonion operator *(const Octonion& Union, const Octonion& Value) noexcept;
			friend Octonion operator /(const Octonion& Union, const Octonion& Value) noexcept;
			friend Octonion operator ^(const Octonion& Base, std::int64_t Exponent) noexcept;
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
			/* Casting */
			inline auto to_number() const& noexcept
				-> decltype(forward_as_number((*this)[index::s], (*this)[index::i], (*this)[index::j], (*this)[index::k], (*this)[index::l], (*this)[index::il], (*this)[index::jl], (*this)[index::kl]))
			{
				return forward_as_number((*this)[index::s], (*this)[index::i], (*this)[index::j], (*this)[index::k], (*this)[index::l], (*this)[index::il], (*this)[index::jl], (*this)[index::kl]);
			};
			template <std::size_t N>
			inline auto from(const Number<N>& number) && noexcept -> decltype(*this)
			{
				number.store((*this)[index::s], (*this)[index::i], (*this)[index::j], (*this)[index::k], (*this)[index::l], (*this)[index::il], (*this)[index::jl], (*this)[index::kl]);
				return *this;
			};
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
		/* struct Octonion */
		const double Octonion::pi = std::numbers::pi;
		const double Octonion::e = std::numbers::e;
		const Octonion Octonion::i = Octonion(0, 1, 0, 0, 0, 0, 0, 0);
		const Octonion Octonion::j = Octonion(0, 0, 1, 0, 0, 0, 0, 0);
		const Octonion Octonion::k = Octonion(0, 0, 0, 1, 0, 0, 0, 0);
		const Octonion Octonion::l = Octonion(0, 0, 0, 0, 1, 0, 0, 0);
		const Octonion Octonion::il = Octonion(0, 0, 0, 0, 0, 1, 0, 0);
		const Octonion Octonion::jl = Octonion(0, 0, 0, 0, 0, 0, 1, 0);
		const Octonion Octonion::kl = Octonion(0, 0, 0, 0, 0, 0, 0, 1);
		///
		/// Initializer
		///
		Octonion::Octonion() noexcept : real(), imaginary() {};
		Octonion::Octonion(double s, const BaseType::Vector7D& v) noexcept : real(s), imaginary(v) {};
		Octonion::Octonion(double s, double i, double j, double k, double l, double il, double jl, double kl) noexcept : real(s), imaginary(BaseType::Vector7D(i, j, k, l, il, jl, kl)) {};
		Octonion::Octonion(double Value) noexcept : real(Value), imaginary() {};
		Octonion::Octonion(const BaseType::Vector7D& Value) noexcept : real(), imaginary(Value) {};
		Octonion::Octonion(const Octonion& Value) noexcept : real(Value.real), imaginary(Value.imaginary) {};
		double Octonion::Scalar(const Octonion& Value) noexcept { return Value.real; };
		BaseType::Vector7D Octonion::Vector(const Octonion& Value) noexcept { return Value.imaginary; };
		///
		/// Operators
		///
		Octonion Octonion::operator ()() const noexcept { return *this; };
		double& Octonion::operator [](index i) &
		{
			switch (i)
			{
			case index::s:
				return this->real;
			case index::i:
				return this->imaginary[BaseType::index::e1];
			case index::j:
				return this->imaginary[BaseType::index::e2];
			case index::k:
				return this->imaginary[BaseType::index::e3];
			case index::l:
				return this->imaginary[BaseType::index::e4];
			case index::il:
				return this->imaginary[BaseType::index::e5];
			case index::jl:
				return this->imaginary[BaseType::index::e6];
			case index::kl:
				return this->imaginary[BaseType::index::e7];
			default:
				throw_now(L"std::out_of_range", L"The index is out of range.");
			}
		};
		double& Octonion::operator [](std::int64_t i) &
		{
			try { return this->operator[](static_cast<index>(i % Period<Octonion>)); }
			catch (...) { return this->operator[](i); }
		};
		const double& Octonion::operator [](index i) const&
		{
			switch (i)
			{
			case index::s:
				return this->real;
			case index::i:
				return this->imaginary[BaseType::index::e1];
			case index::j:
				return this->imaginary[BaseType::index::e2];
			case index::k:
				return this->imaginary[BaseType::index::e3];
			case index::l:
				return this->imaginary[BaseType::index::e4];
			case index::il:
				return this->imaginary[BaseType::index::e5];
			case index::jl:
				return this->imaginary[BaseType::index::e6];
			case index::kl:
				return this->imaginary[BaseType::index::e7];
			default:
				throw_now(L"std::out_of_range", L"The index is out of range.");
			}
		};
		const double& Octonion::operator [](std::int64_t i) const&
		{
			try { return this->operator[](static_cast<index>(i % Period<Octonion>)); }
			catch (...) { return this->operator[](i); }
		};
		bool operator ==(const Octonion& Union, const Octonion& Value) noexcept { return Union.to_number() == Value.to_number(); };
		bool operator !=(const Octonion& Union, const Octonion& Value) noexcept { return !(Union == Value); };
		Octonion operator +(const Octonion& Value) noexcept { return Value; };
		Octonion operator -(const Octonion& Value) noexcept { return Octonion{}.from(-Value.to_number()); };
		Octonion operator ~(const Octonion& Value) noexcept { return Octonion{}.from(~Value.to_number()); };
		Octonion& Octonion::operator ++() & noexcept
		{
			++this->real;
			return *this;
		};
		Octonion Octonion::operator ++(int) & noexcept
		{
			Octonion temp = *this;
			++this->real;
			return temp;
		};
		Octonion& Octonion::operator --() & noexcept
		{
			--this->real;
			return *this;
		};
		Octonion Octonion::operator --(int) & noexcept
		{
			Octonion temp = *this;
			--this->real;
			return temp;
		};
		Octonion operator +(const Octonion& Union, const Octonion& Value) noexcept { return Octonion{}.from(Union.to_number() + Value.to_number()); };
		Octonion operator -(const Octonion& Union, const Octonion& Value) noexcept { return Octonion{}.from(Union.to_number() - Value.to_number()); };
		Octonion operator *(const Octonion& Union, const Octonion& Value) noexcept { return Octonion{}.from(Union.to_number() * Value.to_number()); };
		Octonion operator /(const Octonion& Union, const Octonion& Value) noexcept
		{
			try
			{
				if (Octonion::Vector(Value) == 0) { return Octonion{}.from(Union.to_number() / Octonion::Scalar(Value)); }
				return Union * Octonion::inverse(Value);
			}
			catch (...) { return 0; }
		};
		Octonion operator ^(const Octonion& Base, std::int64_t Exponent) noexcept
		{
			try { return Octonion::Power(Base, static_cast<double>(Exponent)); }
			catch (...) { return 0; }
		};
		Octonion& Octonion::operator =(const Octonion& Value) & noexcept
		{
			this->real = Value.real;
			this->imaginary = Value.imaginary;
			return *this;
		};
		Octonion& Octonion::operator +=(const Octonion& Value) & noexcept { return *this = *this + Value; };
		Octonion& Octonion::operator +=(const std::initializer_list<Octonion>& Value) & noexcept
		{
			for (std::initializer_list<Octonion>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this += *ite; }
			return *this;
		};
		Octonion& Octonion::operator -=(const Octonion& Value) & noexcept { return *this = *this - Value; };
		Octonion& Octonion::operator -=(const std::initializer_list<Octonion>& Value) & noexcept
		{
			for (std::initializer_list<Octonion>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this -= *ite; }
			return *this;
		};
		Octonion& Octonion::operator *=(const Octonion& Value) & noexcept { return *this = *this * Value; };
		Octonion& Octonion::operator *=(const std::initializer_list<Octonion>& Value) & noexcept
		{
			for (std::initializer_list<Octonion>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this *= *ite; }
			return *this;
		};
		Octonion& Octonion::operator /=(const Octonion& Value) & noexcept { return *this = *this / Value; };
		Octonion& Octonion::operator /=(const std::initializer_list<Octonion>& Value) & noexcept
		{
			for (std::initializer_list<Octonion>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this /= *ite; }
			return *this;
		};
		Octonion& Octonion::operator ^=(std::int64_t Exponent) & noexcept { return *this = *this ^ Exponent; };
		Octonion& Octonion::operator ^=(const std::initializer_list<std::int64_t>& Exponent) & noexcept
		{
			for (std::initializer_list<std::int64_t>::const_iterator ite = Exponent.begin(); ite != Exponent.end(); ++ite) { *this ^= *ite; }
			return *this;
		};
		///
		/// Basic functions for constructing numbers
		///
		double Octonion::abs(const Octonion& Value) { return std::sqrt(dot(Value, Value)); };
		double Octonion::arg(const Octonion& Value) { return arg(Value, 0); };
		double Octonion::arg(const Octonion& Value, std::int64_t Theta) { return std::acos(Scalar(Value) / abs(Value)) + 2 * pi * static_cast<double>(Theta); };
		Octonion Octonion::conjg(const Octonion& Value) { return ~Value; };
		Octonion Octonion::sgn(const Octonion& Value) { return Value / abs(Value); };
		Octonion Octonion::inverse(const Octonion& Value) { return conjg(Value) / dot(Value, Value); };
		Octonion Octonion::exp(const Octonion& Value) { return std::exp(Scalar(Value)) * (std::cos(abs(Vector(Value))) + sgn(Vector(Value)) * std::sin(abs(Vector(Value)))); };
		Octonion Octonion::ln(const Octonion& Value) { return ln(Value, 0); };
		Octonion Octonion::ln(const Octonion& Value, std::int64_t Theta) { return std::log(abs(Value)) + sgn(Vector(Value)) * arg(Value, Theta); };
		///
		/// 1st rank tensor algorithms
		///
		double Octonion::dot(const Octonion& Union, const Octonion& Value) noexcept { return Scalar(conjg(Union) * Value + conjg(Value) * Union) / 2; };
		Octonion Octonion::outer(const Octonion& Union, const Octonion& Value) noexcept { return (conjg(Union) * Value - conjg(Value) * Union) / 2; };
		Octonion Octonion::even(const Octonion& Union, const Octonion& Value) noexcept { return (Union * Value + Value * Union) / 2; };
		Octonion Octonion::cross(const Octonion& Union, const Octonion& Value) noexcept { return (Union * Value - Value * Union) / 2; };
		///
		/// Operation 3 algorithms
		///
		Octonion Octonion::Power(const Octonion& Base, const Octonion& Exponent) { return Power(Base, Exponent, 0, 0, 0); };
		Octonion Octonion::Power(const Octonion& Base, const Octonion& Exponent, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau)
		{
			return exp(exp(ln(ln(Base, Theta), Phi) + ln(Exponent, Tau)));
		};
		Octonion Octonion::Power(const Octonion& Base, double Exponent) { return Power(Base, Exponent, 0); }
		Octonion Octonion::Power(const Octonion& Base, double Exponent, std::int64_t Theta)
		{
			if (Base == 0) { return Exponent == 0 ? 1 : 0; }
			return std::pow(abs(Base), Exponent) *
				(std::cos(Exponent * arg(Base, Theta)) + sgn(Vector(Base)) * std::sin(Exponent * arg(Base, Theta)));
		};
		Octonion Octonion::Root(const Octonion& Base, const Octonion& Exponent) { return Root(Base, Exponent, 0, 0, 0); };
		Octonion Octonion::Root(const Octonion& Base, const Octonion& Exponent, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau) { return Power(Base, inverse(Exponent), Theta, Phi, Tau); };
		Octonion Octonion::Root(const Octonion& Base, double Exponent) { return Power(Base, 1 / Exponent); };
		Octonion Octonion::Root(const Octonion& Base, double Exponent, std::int64_t Theta) { return Power(Base, 1 / Exponent, Theta); };
		Octonion Octonion::Log(const Octonion& Base, const Octonion& Number) { return Log(Base, Number, 0, 0, 0, 0); };
		Octonion Octonion::Log(const Octonion& Base, const Octonion& Number, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau, std::int64_t Omega)
		{
			return exp(ln(ln(Number, Theta), Phi) - ln(ln(Base, Tau), Omega));
		};
		///
		/// Trigonometric functions
		///
		Octonion Octonion::sin(const Octonion& Value)
		{
			return std::sin(Scalar(Value)) * std::cosh(abs(Vector(Value)))
				+ std::cos(Scalar(Value)) * sgn(Vector(Value)) * std::sinh(abs(Vector(Value)));
		};
		Octonion Octonion::arcsin(const Octonion& Value) { return arcsin(Value, true, 0); };
		Octonion Octonion::arcsin(const Octonion& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return -sgn(Vector(Value)) * arcsinh(Value * sgn(Vector(Value))); }
			else { return pi - arcsin(Value, true, Period); }
		};
		Octonion Octonion::sinh(const Octonion& Value)
		{
			return std::sinh(Scalar(Value)) * std::cos(abs(Vector(Value)))
				+ std::cosh(Scalar(Value)) * sgn(abs(Vector(Value))) * std::sin(abs(Vector(Value)));
		};
		Octonion Octonion::arcsinh(const Octonion& Value) { return arcsinh(Value, true, 0); };
		Octonion Octonion::arcsinh(const Octonion& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return ln(Value + Root(Power(Value, 2) + 1, 2), Period); }
			else { return pi * sgn(Vector(Value)) - arcsinh(Value, true, Period); }
		};
		Octonion Octonion::cos(const Octonion& Value)
		{
			return std::cos(Scalar(Value)) * std::cosh(abs(Vector(Value)))
				- std::sin(Scalar(Value)) * sgn(Vector(Value)) * std::sinh(abs(Vector(Value)));
		};
		Octonion Octonion::arccos(const Octonion& Value) { return arccos(Value, true, 0); };
		Octonion Octonion::arccos(const Octonion& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return -sgn(Vector(Value)) * arccosh(Value); }
			else { return 2 * pi - arccos(Value, true, Period); }
		};
		Octonion Octonion::cosh(const Octonion& Value)
		{
			return std::cosh(Scalar(Value)) * std::cos(abs(Vector(Value)))
				+ std::sinh(Scalar(Value)) * sgn(abs(Vector(Value))) * std::sin(abs(Vector(Value)));
		};
		Octonion Octonion::arccosh(const Octonion& Value) { return arccosh(Value, true, 0); };
		Octonion Octonion::arccosh(const Octonion& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return ln(Value + Root(Power(Value, 2) - 1, 2), Period); }
			else { return 2 * pi * sgn(Vector(Value)) - arccosh(Value, true, Period); }
		};
		Octonion Octonion::tan(const Octonion& Value)
		{
			return Root(Power(sec(Value), 2) - 1, 2);
		};
		Octonion Octonion::arctan(const Octonion& Value) { return arctan(Value, true, 0); };
		Octonion Octonion::arctan(const Octonion& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return -sgn(Vector(Value)) * arctanh(Value * sgn(Vector(Value))); }
			else { return pi + arctan(Value, true, Period); }
		};
		Octonion Octonion::tanh(const Octonion& Value)
		{
			return 1 - 2 * inverse(exp(2 * Value) + 1);
		};
		Octonion Octonion::arctanh(const Octonion& Value) { return arctanh(Value, true, 0); };
		Octonion Octonion::arctanh(const Octonion& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return (ln(1 + Value, Period) - ln(1 - Value)) / 2; }
			else { return pi * sgn(Vector(Value)) + arctanh(Value, true, Period); }
		};
		Octonion Octonion::csc(const Octonion& Value) { return inverse(sin(Value)); };
		Octonion Octonion::arccsc(const Octonion& Value) { return arccsc(Value, true, 0); };
		Octonion Octonion::arccsc(const Octonion& Value, bool Sign, std::int64_t Period) { return arcsin(inverse(Value), Sign, Period); };
		Octonion Octonion::csch(const Octonion& Value) { return inverse(sinh(Value)); };
		Octonion Octonion::arccsch(const Octonion& Value) { return arccsch(Value, true, 0); };
		Octonion Octonion::arccsch(const Octonion& Value, bool Sign, std::int64_t Period) { return arcsinh(inverse(Value), Sign, Period); };
		Octonion Octonion::sec(const Octonion& Value) { return inverse(cos(Value)); };
		Octonion Octonion::arcsec(const Octonion& Value) { return arcsec(Value, true, 0); };
		Octonion Octonion::arcsec(const Octonion& Value, bool Sign, std::int64_t Period) { return arccos(inverse(Value), Sign, Period); };
		Octonion Octonion::sech(const Octonion& Value) { return inverse(cosh(Value)); };
		Octonion Octonion::arcsech(const Octonion& Value) { return arcsech(Value, true, 0); };
		Octonion Octonion::arcsech(const Octonion& Value, bool Sign, std::int64_t Period) { return arccosh(inverse(Value), Sign, Period); };
		Octonion Octonion::cot(const Octonion& Value) { return inverse(tan(Value)); };
		Octonion Octonion::arccot(const Octonion& Value) { return arccot(Value, true, 0); };
		Octonion Octonion::arccot(const Octonion& Value, bool Sign, std::int64_t Period) { return arctan(inverse(Value), Sign, Period); };
		Octonion Octonion::coth(const Octonion& Value) { return inverse(tanh(Value)); };
		Octonion Octonion::arccoth(const Octonion& Value) { return arccoth(Value, true, 0); };
		Octonion Octonion::arccoth(const Octonion& Value, bool Sign, std::int64_t Period) { return arctanh(inverse(Value), Sign, Period); };
		///
		/// Conversion of Types
		///
		std::wstring Octonion::CType_String(const Octonion& Value)
		{
			return ToString(Value[index::s], L"", Value[index::i], L"i", Value[index::j], L"j", Value[index::k], L"k", Value[index::l], L"l", Value[index::il], L"il", Value[index::jl], L"jl", Value[index::kl], L"kl");
		};
		Octonion Octonion::CType_Octonion(const std::wstring& Value)
		{
			Octonion Object;
			ToNumbers(Value, Object[index::s], L"", Object[index::i], L"i", Object[index::j], L"j", Object[index::k], L"k", Object[index::l], L"l", Object[index::il], L"il", Object[index::jl], L"jl", Object[index::kl], L"kl");
			return Object;
		};
	}
	double BaseType::Vector7D::build_in_dot(const BaseType::Vector7D& Union, const BaseType::Vector7D& Value)
	{
		return MainType::Octonion::dot(MainType::Octonion{ Union }, MainType::Octonion{ Value });
	};
	BaseType::Vector7D BaseType::Vector7D::build_in_cross(const BaseType::Vector7D& Union, const BaseType::Vector7D& Value)
	{
		return Vector7D{}.from(MainType::Octonion::cross(MainType::Octonion{ Union }, MainType::Octonion{ Value }).to_number());
	};
}
