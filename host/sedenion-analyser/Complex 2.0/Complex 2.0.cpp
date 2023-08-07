#include <cmath>
#include <numbers>
#include "Module.h"
#include "Cayley Dickson Algebra.h"
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
			Vector3D operator ()() const noexcept;
			double& operator [](index i) &;
			double& operator [](std::int64_t i) & noexcept;
			const double& operator [](index i) const&;
			const double& operator [](std::int64_t i) const& noexcept;
			friend bool operator ==(const Vector3D& Union, const Vector3D& Value) noexcept;
			friend bool operator !=(const Vector3D& Union, const Vector3D& Value) noexcept;
			friend Vector3D operator +(const Vector3D& Value) noexcept;
			friend Vector3D operator -(const Vector3D& Value) noexcept;
			friend Vector3D operator +(const Vector3D& Union, const Vector3D& Value) noexcept;
			friend Vector3D operator -(const Vector3D& Union, const Vector3D& Value) noexcept;
			friend Vector3D operator *(double Union, const Vector3D& Value) noexcept;
			friend Vector3D operator *(const Vector3D& Union, double Value) noexcept;
			friend Vector3D operator /(const Vector3D& Union, double Value) noexcept;
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
			/* Casting */
			inline auto to_number() const& noexcept
				-> decltype(forward_as_number(0, (*this)[index::e1], (*this)[index::e2], (*this)[index::e3]))
			{
				return forward_as_number(0, (*this)[index::e1], (*this)[index::e2], (*this)[index::e3]);
			};
			template <std::size_t N>
			inline auto from(const Number<N>& number) && noexcept -> decltype(*this)
			{
				double temp{};
				number.store(temp, (*this)[index::e1], (*this)[index::e2], (*this)[index::e3]);
				return *this;
			};
			static double build_in_dot(const Vector3D& Union, const Vector3D& Value);
			static Vector3D build_in_cross(const Vector3D& Union, const Vector3D& Value);
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
		/* struct Vector3D */
		const double Vector3D::pi = std::numbers::pi;
		const double Vector3D::e = std::numbers::e;
		const Vector3D Vector3D::e1 = Vector3D(1, 0, 0);
		const Vector3D Vector3D::e2 = Vector3D(0, 1, 0);
		const Vector3D Vector3D::e3 = Vector3D(0, 0, 1);
		///
		/// Initializer
		///
		Vector3D::Vector3D() noexcept : x1(), x2(), x3() {};
		Vector3D::Vector3D(double x1, double x2, double x3) noexcept : x1(x1), x2(x2), x3(x3) {};
		Vector3D::Vector3D(const Vector3D& Value) noexcept : x1(Value.x1), x2(Value.x2), x3(Value.x3) {};
		///
		/// Operators
		///
		Vector3D Vector3D::operator ()() const noexcept { return *this; };
		double& Vector3D::operator [](index i) &
		{
			switch (i)
			{
			case index::e1:
				return this->x1;
			case index::e2:
				return this->x2;
			case index::e3:
				return this->x3;
			default:
				throw_now(L"std::out_of_range", L"The index is out of range.");
			}
		};
		double& Vector3D::operator [](std::int64_t i) & noexcept
		{
			try { return this->operator[](static_cast<index>(PeriodShift(i, Period<Vector3D>))); }
			catch (...) { return this->operator[](i); }
		};
		const double& Vector3D::operator [](index i) const&
		{
			switch (i)
			{
			case index::e1:
				return this->x1;
			case index::e2:
				return this->x2;
			case index::e3:
				return this->x3;
			default:
				throw_now(L"std::out_of_range", L"The index is out of range.");
			}
		};
		const double& Vector3D::operator [](std::int64_t i) const& noexcept
		{
			try { return this->operator[](static_cast<index>(PeriodShift(i, Period<Vector3D>))); }
			catch (...) { return this->operator[](i); }
		};
		bool operator ==(const Vector3D& Union, const Vector3D& Value) noexcept { return Union.to_number() == Value.to_number(); };
		bool operator !=(const Vector3D& Union, const Vector3D& Value) noexcept { return !(Union == Value); };
		Vector3D operator +(const Vector3D& Value) noexcept { return Value; };
		Vector3D operator -(const Vector3D& Value) noexcept { return Vector3D{}.from(-Value.to_number()); };
		Vector3D operator +(const Vector3D& Union, const Vector3D& Value) noexcept { return Vector3D{}.from(Union.to_number() + Value.to_number()); };
		Vector3D operator -(const Vector3D& Union, const Vector3D& Value) noexcept { return Vector3D{}.from(Union.to_number() - Value.to_number()); };
		Vector3D operator *(double Union, const Vector3D& Value) noexcept { return Vector3D{}.from(Union * Value.to_number()); };
		Vector3D operator *(const Vector3D& Union, double Value) noexcept { return Vector3D{}.from(Union.to_number() * Value); };
		Vector3D operator /(const Vector3D& Union, double Value) noexcept { return Vector3D{}.from(Union.to_number() / Value); };
		Vector3D& Vector3D::operator =(const Vector3D& Value) & noexcept
		{
			this->x1 = Value.x1;
			this->x2 = Value.x2;
			this->x3 = Value.x3;
			return *this;
		};
		Vector3D& Vector3D::operator +=(const Vector3D& Value) & noexcept { return *this = *this + Value; };
		Vector3D& Vector3D::operator +=(const std::initializer_list<Vector3D>& Value) & noexcept
		{
			for (std::initializer_list<Vector3D>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this += *ite; }
			return *this;
		};
		Vector3D& Vector3D::operator -=(const Vector3D& Value) & noexcept { return *this = *this - Value; };
		Vector3D& Vector3D::operator -=(const std::initializer_list<Vector3D>& Value) & noexcept
		{
			for (std::initializer_list<Vector3D>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this -= *ite; }
			return *this;
		};
		Vector3D& Vector3D::operator *=(double Value) & noexcept { return *this = *this * Value; };
		Vector3D& Vector3D::operator *=(const std::initializer_list<double>& Value) & noexcept
		{
			for (std::initializer_list<double>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this *= *ite; }
			return *this;
		};
		Vector3D& Vector3D::operator /=(double Value) & noexcept { return *this = *this / Value; };
		Vector3D& Vector3D::operator /=(const std::initializer_list<double>& Value) & noexcept
		{
			for (std::initializer_list<double>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this /= *ite; }
			return *this;
		};
		///
		/// Basic functions for constructing vectors
		///
		double Vector3D::abs(const Vector3D& Value) { return std::sqrt(dot(Value, Value)); };
		Vector3D Vector3D::sgn(const Vector3D& Value) { return Value / abs(Value); };
		double Vector3D::dot(const Vector3D& Union, const Vector3D& Value) noexcept { return build_in_dot(Union, Value); };
		Vector3D Vector3D::cross(const Vector3D& Union, const Vector3D& Value) noexcept { return build_in_cross(Union, Value); };;
		///
		/// Conversion of Types
		///
		std::wstring Vector3D::CType_String(const Vector3D& Value)
		{
			return ToString(Value[index::e1], L"e1", Value[index::e2], L"e2", Value[index::e3], L"e3");
		};
		Vector3D Vector3D::CType_Vector3D(const std::wstring& Value)
		{
			Vector3D Object;
			ToNumbers(Value, Object[index::e1], L"e1", Object[index::e2], L"e2", Object[index::e3], L"e3");
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
			Quaternion operator ()() const noexcept;
			double& operator [](index i) &;
			double& operator [](std::int64_t i) & noexcept;
			const double& operator [](index i) const&;
			const double& operator [](std::int64_t i) const& noexcept;
			friend bool operator ==(const Quaternion& Union, const Quaternion& Value) noexcept;
			friend bool operator !=(const Quaternion& Union, const Quaternion& Value) noexcept;
			friend Quaternion operator +(const Quaternion& Value) noexcept;
			friend Quaternion operator -(const Quaternion& Value) noexcept;
			friend Quaternion operator ~(const Quaternion& Value) noexcept;
			Quaternion& operator ++() & noexcept;
			Quaternion operator ++(int) & noexcept;
			Quaternion& operator --() & noexcept;
			Quaternion operator --(int) & noexcept;
			friend Quaternion operator +(const Quaternion& Union, const Quaternion& Value) noexcept;
			friend Quaternion operator -(const Quaternion& Union, const Quaternion& Value) noexcept;
			friend Quaternion operator *(const Quaternion& Union, const Quaternion& Value) noexcept;
			friend Quaternion operator /(const Quaternion& Union, const Quaternion& Value) noexcept;
			friend Quaternion operator ^(const Quaternion& Base, std::int64_t Exponent) noexcept;
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
			/* Casting */
			inline auto to_number() const& noexcept
				-> decltype(forward_as_number((*this)[index::s], (*this)[index::i], (*this)[index::j], (*this)[index::k]))
			{
				return forward_as_number((*this)[index::s], (*this)[index::i], (*this)[index::j], (*this)[index::k]);
			};
			template <std::size_t N>
			inline auto from(const Number<N>& number) && noexcept -> decltype(*this)
			{
				number.store((*this)[index::s], (*this)[index::i], (*this)[index::j], (*this)[index::k]);
				return *this;
			};
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
		/* struct Quaternion */
		const double Quaternion::pi = std::numbers::pi;
		const double Quaternion::e = std::numbers::e;
		const Quaternion Quaternion::i = Quaternion(0, 1, 0, 0);
		const Quaternion Quaternion::j = Quaternion(0, 0, 1, 0);
		const Quaternion Quaternion::k = Quaternion(0, 0, 0, 1);
		///
		/// Initializer
		///
		Quaternion::Quaternion() noexcept : real(), imaginary() {};
		Quaternion::Quaternion(double s, const BaseType::Vector3D& v) noexcept : real(s), imaginary(v) {};
		Quaternion::Quaternion(double s, double i, double j, double k) noexcept : real(s), imaginary(BaseType::Vector3D(i, j, k)) {};
		Quaternion::Quaternion(double Value) noexcept : real(Value), imaginary() {};
		Quaternion::Quaternion(const BaseType::Vector3D& Value) noexcept : real(), imaginary(Value) {};
		Quaternion::Quaternion(const Quaternion& Value) noexcept : real(Value.real), imaginary(Value.imaginary) {};
		double Quaternion::Scalar(const Quaternion& Value) noexcept { return Value.real; };
		BaseType::Vector3D Quaternion::Vector(const Quaternion& Value) noexcept { return Value.imaginary; };
		///
		/// Operators
		///
		Quaternion Quaternion::operator ()() const noexcept { return *this; };
		double& Quaternion::operator [](index i) &
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
			default:
				throw_now(L"std::out_of_range", L"The index is out of range.");
			}
		};
		double& Quaternion::operator [](std::int64_t i) & noexcept
		{
			try { return this->operator[](static_cast<index>(i % Period<Quaternion>)); }
			catch (...) { return this->operator[](i); }
		};
		const double& Quaternion::operator [](index i) const&
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
			default:
				throw_now(L"std::out_of_range", L"The index is out of range.");
			}
		};
		const double& Quaternion::operator [](std::int64_t i) const& noexcept
		{
			try { return this->operator[](static_cast<index>(i % Period<Quaternion>)); }
			catch (...) { return this->operator[](i); }
		};
		bool operator ==(const Quaternion& Union, const Quaternion& Value) noexcept { return Union.to_number() == Value.to_number(); };
		bool operator !=(const Quaternion& Union, const Quaternion& Value) noexcept { return !(Union == Value); };
		Quaternion operator +(const Quaternion& Value) noexcept { return Value; };
		Quaternion operator -(const Quaternion& Value) noexcept { return Quaternion{}.from(-Value.to_number()); };
		Quaternion operator ~(const Quaternion& Value) noexcept { return Quaternion{}.from(~Value.to_number()); };
		Quaternion& Quaternion::operator ++() & noexcept
		{
			++this->real;
			return *this;
		};
		Quaternion Quaternion::operator ++(int) & noexcept
		{
			Quaternion temp = *this;
			++this->real;
			return temp;
		};
		Quaternion& Quaternion::operator --() & noexcept
		{
			--this->real;
			return *this;
		};
		Quaternion Quaternion::operator --(int) & noexcept
		{
			Quaternion temp = *this;
			--this->real;
			return temp;
		};
		Quaternion operator +(const Quaternion& Union, const Quaternion& Value) noexcept { return Quaternion{}.from(Union.to_number() + Value.to_number()); };
		Quaternion operator -(const Quaternion& Union, const Quaternion& Value) noexcept { return Quaternion{}.from(Union.to_number() - Value.to_number()); };
		Quaternion operator *(const Quaternion& Union, const Quaternion& Value) noexcept { return Quaternion{}.from(Union.to_number() * Value.to_number()); };
		Quaternion operator /(const Quaternion& Union, const Quaternion& Value) noexcept
		{
			try
			{
				if (Quaternion::Vector(Value) == 0) { return Quaternion{}.from(Union.to_number() / Quaternion::Scalar(Value)); }
				return Union * Quaternion::inverse(Value);
			}
			catch (...) { return 0; }
		};
		Quaternion operator ^(const Quaternion& Base, std::int64_t Exponent) noexcept
		{
			try { return Quaternion::Power(Base, static_cast<double>(Exponent)); }
			catch (...) { return 0; }
		};
		Quaternion& Quaternion::operator =(const Quaternion& Value) & noexcept
		{
			this->real = Value.real;
			this->imaginary = Value.imaginary;
			return *this;
		};
		Quaternion& Quaternion::operator +=(const Quaternion& Value) & noexcept { return *this = *this + Value; };
		Quaternion& Quaternion::operator +=(const std::initializer_list<Quaternion>& Value) & noexcept
		{
			for (std::initializer_list<Quaternion>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this += *ite; }
			return *this;
		};
		Quaternion& Quaternion::operator -=(const Quaternion& Value) & noexcept { return *this = *this - Value; };
		Quaternion& Quaternion::operator -=(const std::initializer_list<Quaternion>& Value) & noexcept
		{
			for (std::initializer_list<Quaternion>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this -= *ite; }
			return *this;
		};
		Quaternion& Quaternion::operator *=(const Quaternion& Value) & noexcept { return *this = *this * Value; };
		Quaternion& Quaternion::operator *=(const std::initializer_list<Quaternion>& Value) & noexcept
		{
			for (std::initializer_list<Quaternion>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this *= *ite; }
			return *this;
		};
		Quaternion& Quaternion::operator /=(const Quaternion& Value) & noexcept { return *this = *this / Value; };
		Quaternion& Quaternion::operator /=(const std::initializer_list<Quaternion>& Value) & noexcept
		{
			for (std::initializer_list<Quaternion>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this /= *ite; }
			return *this;
		};
		Quaternion& Quaternion::operator ^=(std::int64_t Exponent) & noexcept { return *this = *this ^ Exponent; };
		Quaternion& Quaternion::operator ^=(const std::initializer_list<std::int64_t>& Exponent) & noexcept
		{
			for (std::initializer_list<std::int64_t>::const_iterator ite = Exponent.begin(); ite != Exponent.end(); ++ite) { *this ^= *ite; }
			return *this;
		};
		///
		/// Basic functions for constructing numbers
		///
		double Quaternion::abs(const Quaternion& Value) { return std::sqrt(dot(Value, Value)); };
		double Quaternion::arg(const Quaternion& Value) { return arg(Value, 0); };
		double Quaternion::arg(const Quaternion& Value, std::int64_t Theta) { return std::acos(Scalar(Value) / abs(Value)) + 2 * pi * static_cast<double>(Theta); };
		Quaternion Quaternion::conjg(const Quaternion& Value) { return ~Value; };
		Quaternion Quaternion::sgn(const Quaternion& Value) { return Value / abs(Value); };
		Quaternion Quaternion::inverse(const Quaternion& Value) { return conjg(Value) / dot(Value, Value); };
		Quaternion Quaternion::exp(const Quaternion& Value) { return std::exp(Scalar(Value)) * (std::cos(abs(Vector(Value))) + sgn(Vector(Value)) * std::sin(abs(Vector(Value)))); };
		Quaternion Quaternion::ln(const Quaternion& Value) { return ln(Value, 0); };
		Quaternion Quaternion::ln(const Quaternion& Value, std::int64_t Theta) { return std::log(abs(Value)) + sgn(Vector(Value)) * arg(Value, Theta); };
		///
		/// 1st rank tensor algorithms
		///
		double Quaternion::dot(const Quaternion& Union, const Quaternion& Value) noexcept { return Scalar(conjg(Union) * Value + conjg(Value) * Union) / 2; };
		Quaternion Quaternion::outer(const Quaternion& Union, const Quaternion& Value) noexcept { return (conjg(Union) * Value - conjg(Value) * Union) / 2; };
		Quaternion Quaternion::even(const Quaternion& Union, const Quaternion& Value) noexcept { return (Union * Value + Value * Union) / 2; };
		Quaternion Quaternion::cross(const Quaternion& Union, const Quaternion& Value) noexcept { return (Union * Value - Value * Union) / 2; };
		///
		/// Operation 3 algorithms
		///
		Quaternion Quaternion::Power(const Quaternion& Base, const Quaternion& Exponent) { return Power(Base, Exponent, 0, 0, 0); };
		Quaternion Quaternion::Power(const Quaternion& Base, const Quaternion& Exponent, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau)
		{
			return exp(exp(ln(ln(Base, Theta), Phi) + ln(Exponent, Tau)));
		};
		Quaternion Quaternion::Power(const Quaternion& Base, double Exponent) { return Power(Base, Exponent, 0); }
		Quaternion Quaternion::Power(const Quaternion& Base, double Exponent, std::int64_t Theta)
		{
			if (Base == 0) { return Exponent == 0 ? 1 : 0; }
			return std::pow(abs(Base), Exponent) *
				(std::cos(Exponent * arg(Base, Theta)) + sgn(Vector(Base)) * std::sin(Exponent * arg(Base, Theta)));
		};
		Quaternion Quaternion::Root(const Quaternion& Base, const Quaternion& Exponent) { return Root(Base, Exponent, 0, 0, 0); };
		Quaternion Quaternion::Root(const Quaternion& Base, const Quaternion& Exponent, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau) { return Power(Base, inverse(Exponent), Theta, Phi, Tau); };
		Quaternion Quaternion::Root(const Quaternion& Base, double Exponent) { return Root(Base, Exponent, 0); };
		Quaternion Quaternion::Root(const Quaternion& Base, double Exponent, std::int64_t Theta) { return Power(Base, 1 / Exponent, Theta); };
		Quaternion Quaternion::Log(const Quaternion& Base, const Quaternion& Number) { return Log(Base, Number, 0, 0, 0, 0); };
		Quaternion Quaternion::Log(const Quaternion& Base, const Quaternion& Number, std::int64_t Theta, std::int64_t Phi, std::int64_t Tau, std::int64_t Omega)
		{
			return exp(ln(ln(Number, Theta), Phi) - ln(ln(Base, Tau), Omega));
		};
		///
		/// Trigonometric functions
		///
		Quaternion Quaternion::sin(const Quaternion& Value)
		{
			return std::sin(Scalar(Value)) * std::cosh(abs(Vector(Value)))
				+ std::cos(Scalar(Value)) * sgn(Vector(Value)) * std::sinh(abs(Vector(Value)));
		};
		Quaternion Quaternion::arcsin(const Quaternion& Value) { return arcsin(Value, true, 0); };
		Quaternion Quaternion::arcsin(const Quaternion& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return -sgn(Vector(Value)) * arcsinh(Value * sgn(Vector(Value))); }
			else { return pi - arcsin(Value, true, Period); }
		};
		Quaternion Quaternion::sinh(const Quaternion& Value)
		{
			return std::sinh(Scalar(Value)) * std::cos(abs(Vector(Value)))
				+ std::cosh(Scalar(Value)) * sgn(abs(Vector(Value))) * std::sin(abs(Vector(Value)));
		};
		Quaternion Quaternion::arcsinh(const Quaternion& Value) { return arcsinh(Value, true, 0); };
		Quaternion Quaternion::arcsinh(const Quaternion& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return ln(Value + Root(Power(Value, 2) + 1, 2), Period); }
			else { return pi * sgn(Vector(Value)) - arcsinh(Value, true, Period); }
		};
		Quaternion Quaternion::cos(const Quaternion& Value)
		{
			return std::cos(Scalar(Value)) * std::cosh(abs(Vector(Value)))
				- std::sin(Scalar(Value)) * sgn(Vector(Value)) * std::sinh(abs(Vector(Value)));
		};
		Quaternion Quaternion::arccos(const Quaternion& Value) { return arccos(Value, true, 0); };
		Quaternion Quaternion::arccos(const Quaternion& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return -sgn(Vector(Value)) * arccosh(Value); }
			else { return 2 * pi - arccos(Value, true, Period); }
		};
		Quaternion Quaternion::cosh(const Quaternion& Value)
		{
			return std::cosh(Scalar(Value)) * std::cos(abs(Vector(Value)))
				+ std::sinh(Scalar(Value)) * sgn(abs(Vector(Value))) * std::sin(abs(Vector(Value)));
		};
		Quaternion Quaternion::arccosh(const Quaternion& Value) { return arccosh(Value, true, 0); };
		Quaternion Quaternion::arccosh(const Quaternion& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return ln(Value + Root(Power(Value, 2) - 1, 2), Period); }
			else { return 2 * pi * sgn(Vector(Value)) - arccosh(Value, true, Period); }
		};
		Quaternion Quaternion::tan(const Quaternion& Value)
		{
			return Root(Power(sec(Value), 2) - 1, 2);
		};
		Quaternion Quaternion::arctan(const Quaternion& Value) { return arctan(Value, true, 0); };
		Quaternion Quaternion::arctan(const Quaternion& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return -sgn(Vector(Value)) * arctanh(Value * sgn(Vector(Value))); }
			else { return pi + arctan(Value, true, Period); }
		};
		Quaternion Quaternion::tanh(const Quaternion& Value)
		{
			return 1 - 2 * inverse(exp(2 * Value) + 1);
		};
		Quaternion Quaternion::arctanh(const Quaternion& Value) { return arctanh(Value, true, 0); };
		Quaternion Quaternion::arctanh(const Quaternion& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return (ln(1 + Value, Period) - ln(1 - Value)) / 2; }
			else { return pi * sgn(Vector(Value)) + arctanh(Value, true, Period); }
		};
		Quaternion Quaternion::csc(const Quaternion& Value) { return inverse(sin(Value)); };
		Quaternion Quaternion::arccsc(const Quaternion& Value) { return arccsc(Value, true, 0); };
		Quaternion Quaternion::arccsc(const Quaternion& Value, bool Sign, std::int64_t Period) { return arcsin(inverse(Value), Sign, Period); };
		Quaternion Quaternion::csch(const Quaternion& Value) { return inverse(sinh(Value)); };
		Quaternion Quaternion::arccsch(const Quaternion& Value) { return arccsch(Value, true, 0); };
		Quaternion Quaternion::arccsch(const Quaternion& Value, bool Sign, std::int64_t Period) { return arcsinh(inverse(Value), Sign, Period); };
		Quaternion Quaternion::sec(const Quaternion& Value) { return inverse(cos(Value)); };
		Quaternion Quaternion::arcsec(const Quaternion& Value) { return arcsec(Value, true, 0); };
		Quaternion Quaternion::arcsec(const Quaternion& Value, bool Sign, std::int64_t Period) { return arccos(inverse(Value), Sign, Period); };
		Quaternion Quaternion::sech(const Quaternion& Value) { return inverse(cosh(Value)); };
		Quaternion Quaternion::arcsech(const Quaternion& Value) { return arcsech(Value, true, 0); };
		Quaternion Quaternion::arcsech(const Quaternion& Value, bool Sign, std::int64_t Period) { return arccosh(inverse(Value), Sign, Period); };
		Quaternion Quaternion::cot(const Quaternion& Value) { return inverse(tan(Value)); };
		Quaternion Quaternion::arccot(const Quaternion& Value) { return arccot(Value, true, 0); };
		Quaternion Quaternion::arccot(const Quaternion& Value, bool Sign, std::int64_t Period) { return arctan(inverse(Value), Sign, Period); };
		Quaternion Quaternion::coth(const Quaternion& Value) { return inverse(tanh(Value)); };
		Quaternion Quaternion::arccoth(const Quaternion& Value) { return arccoth(Value, true, 0); };
		Quaternion Quaternion::arccoth(const Quaternion& Value, bool Sign, std::int64_t Period) { return arctanh(inverse(Value), Sign, Period); };
		///
		/// Conversion of Types
		///
		std::wstring Quaternion::CType_String(const Quaternion& Value)
		{
			return ToString(Value[index::s], L"", Value[index::i], L"i", Value[index::j], L"j", Value[index::k], L"k");
		};
		Quaternion Quaternion::CType_Quaternion(const std::wstring& Value)
		{
			Quaternion Object;
			ToNumbers(Value, Object[index::s], L"", Object[index::i], L"i", Object[index::j], L"j", Object[index::k], L"k");
			return Object;
		};
	}
	double BaseType::Vector3D::build_in_dot(const BaseType::Vector3D& Union, const BaseType::Vector3D& Value)
	{
		return MainType::Quaternion::dot(MainType::Quaternion{ Union }, MainType::Quaternion{ Value });
	};
	BaseType::Vector3D BaseType::Vector3D::build_in_cross(const BaseType::Vector3D& Union, const BaseType::Vector3D& Value)
	{
		return Vector3D{}.from(MainType::Quaternion::cross(MainType::Quaternion{ Union }, MainType::Quaternion{ Value }).to_number());
	};
}
