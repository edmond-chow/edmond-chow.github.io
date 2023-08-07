#include <cmath>
#include <numbers>
#include "Module.h"
#include "Cayley Dickson Algebra.h"
namespace Cmplx
{
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
			friend bool operator ==(const Vector1D& Union, const Vector1D& Value) noexcept;
			friend bool operator !=(const Vector1D& Union, const Vector1D& Value) noexcept;
			friend Vector1D operator +(const Vector1D& Value) noexcept;
			friend Vector1D operator -(const Vector1D& Value) noexcept;
			friend Vector1D operator +(const Vector1D& Union, const Vector1D& Value) noexcept;
			friend Vector1D operator -(const Vector1D& Union, const Vector1D& Value) noexcept;
			friend Vector1D operator *(double Union, const Vector1D& Value) noexcept;
			friend Vector1D operator *(const Vector1D& Union, double Value) noexcept;
			friend Vector1D operator /(const Vector1D& Union, double Value) noexcept;
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
			/* Casting */
			inline auto to_number() const& noexcept
				-> decltype(forward_as_number(0, (*this)[index::e1]))
			{
				return forward_as_number(0, (*this)[index::e1]);
			};
			template <std::size_t N>
			inline auto from(const Number<N>& number) && noexcept -> decltype(*this)
			{
				double temp{};
				number.store(temp, (*this)[index::e1]);
				return *this;
			};
			static double build_in_dot(const Vector1D& Union, const Vector1D& Value);
			static Vector1D build_in_cross(const Vector1D& Union, const Vector1D& Value);
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
		/* struct Vector1D */
		const double Vector1D::pi = std::numbers::pi;
		const double Vector1D::e = std::numbers::e;
		const Vector1D Vector1D::e1 = Vector1D(1);
		///
		/// Initializer
		///
		Vector1D::Vector1D() noexcept : x1() {};
		Vector1D::Vector1D(double x1) noexcept : x1(x1) {};
		Vector1D::Vector1D(const Vector1D& Value) noexcept : x1(Value.x1) {};
		///
		/// Operators
		///
		Vector1D Vector1D::operator ()() const noexcept { return *this; };
		double& Vector1D::operator [](index i) &
		{
			switch (i)
			{
			case index::e1:
				return this->x1;
			default:
				throw_now(L"std::out_of_range", L"The index is out of range.");
			}
		};
		double& Vector1D::operator [](std::int64_t i) & noexcept
		{
			try { return this->operator[](static_cast<index>(PeriodShift(i, Period<Vector1D>))); }
			catch (...) { return this->operator[](i); }
		};
		const double& Vector1D::operator [](index i) const&
		{
			switch (i)
			{
			case index::e1:
				return this->x1;
			default:
				throw_now(L"std::out_of_range", L"The index is out of range.");
			}
		};
		const double& Vector1D::operator [](std::int64_t i) const& noexcept
		{
			try { return this->operator[](static_cast<index>(PeriodShift(i, Period<Vector1D>))); }
			catch (...) { return this->operator[](i); }
		};
		bool operator ==(const Vector1D& Union, const Vector1D& Value) noexcept { return Union.to_number() == Value.to_number(); };
		bool operator !=(const Vector1D& Union, const Vector1D& Value) noexcept { return !(Union == Value); };
		Vector1D operator +(const Vector1D& Value) noexcept { return Value; };
		Vector1D operator -(const Vector1D& Value) noexcept { return Vector1D{}.from(-Value.to_number()); };
		Vector1D operator +(const Vector1D& Union, const Vector1D& Value) noexcept { return Vector1D{}.from(Union.to_number() + Value.to_number()); };
		Vector1D operator -(const Vector1D& Union, const Vector1D& Value) noexcept { return Vector1D{}.from(Union.to_number() - Value.to_number()); };
		Vector1D operator *(double Union, const Vector1D& Value) noexcept { return Vector1D{}.from(Union * Value.to_number()); };
		Vector1D operator *(const Vector1D& Union, double Value) noexcept { return Vector1D{}.from(Union.to_number() * Value); };
		Vector1D operator /(const Vector1D& Union, double Value) noexcept { return Vector1D{}.from(Union.to_number() / Value); };
		Vector1D& Vector1D::operator =(const Vector1D& Value) & noexcept
		{
			this->x1 = Value.x1;
			return *this;
		};
		Vector1D& Vector1D::operator +=(const Vector1D& Value) & noexcept { return *this = *this + Value; };
		Vector1D& Vector1D::operator +=(const std::initializer_list<Vector1D>& Value) & noexcept
		{
			for (std::initializer_list<Vector1D>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this += *ite; }
			return *this;
		};
		Vector1D& Vector1D::operator -=(const Vector1D& Value) & noexcept { return *this = *this - Value; };
		Vector1D& Vector1D::operator -=(const std::initializer_list<Vector1D>& Value) & noexcept
		{
			for (std::initializer_list<Vector1D>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this -= *ite; }
			return *this;
		};
		Vector1D& Vector1D::operator *=(double Value) & noexcept { return *this = *this * Value; };
		Vector1D& Vector1D::operator *=(const std::initializer_list<double>& Value) & noexcept
		{
			for (std::initializer_list<double>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this *= *ite; }
			return *this;
		};
		Vector1D& Vector1D::operator /=(double Value) & noexcept { return *this = *this / Value; };
		Vector1D& Vector1D::operator /=(const std::initializer_list<double>& Value) & noexcept
		{
			for (std::initializer_list<double>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this /= *ite; }
			return *this;
		};
		///
		/// Basic functions for constructing vectors
		///
		double Vector1D::abs(const Vector1D& Value) { return std::sqrt(dot(Value, Value)); };
		Vector1D Vector1D::sgn(const Vector1D& Value) { return Value / abs(Value); };
		double Vector1D::dot(const Vector1D& Union, const Vector1D& Value) noexcept { return build_in_dot(Union, Value); };
		Vector1D Vector1D::cross(const Vector1D& Union, const Vector1D& Value) noexcept { return build_in_cross(Union, Value); };;
		///
		/// Conversion of Types
		///
		std::wstring Vector1D::CType_String(const Vector1D& Value)
		{
			return ToString(Value[index::e1], L"e1");
		};
		Vector1D Vector1D::CType_Vector1D(const std::wstring& Value)
		{
			Vector1D Object;
			ToNumbers(Value, Object[index::e1], L"e1");
			return Object;
		};
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
			friend bool operator ==(const Complex& Union, const Complex& Value) noexcept;
			friend bool operator !=(const Complex& Union, const Complex& Value) noexcept;
			friend Complex operator +(const Complex& Value) noexcept;
			friend Complex operator -(const Complex& Value) noexcept;
			friend Complex operator ~(const Complex& Value) noexcept;
			Complex& operator ++() & noexcept;
			Complex operator ++(int) & noexcept;
			Complex& operator --() & noexcept;
			Complex operator --(int) & noexcept;
			friend Complex operator +(const Complex& Union, const Complex& Value) noexcept;
			friend Complex operator -(const Complex& Union, const Complex& Value) noexcept;
			friend Complex operator *(const Complex& Union, const Complex& Value) noexcept;
			friend Complex operator /(const Complex& Union, const Complex& Value) noexcept;
			friend Complex operator ^(const Complex& Base, std::int64_t Exponent) noexcept;
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
			/* Casting */
			inline auto to_number() const& noexcept
				-> decltype(forward_as_number((*this)[index::s], (*this)[index::i]))
			{
				return forward_as_number((*this)[index::s], (*this)[index::i]);
			};
			template <std::size_t N>
			inline auto from(const Number<N>& number) && noexcept -> decltype(*this)
			{
				number.store((*this)[index::s], (*this)[index::i]);
				return *this;
			};
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
		/* struct Complex */
		const double Complex::pi = std::numbers::pi;
		const double Complex::e = std::numbers::e;
		const Complex Complex::i = Complex(0, 1);
		///
		/// Initializer
		///
		Complex::Complex() noexcept : real(), imaginary() {};
		Complex::Complex(double s, const BaseType::Vector1D& v) noexcept : real(s), imaginary(v) {};
		Complex::Complex(double s, double i) noexcept : real(s), imaginary(BaseType::Vector1D(i)) {};
		Complex::Complex(double Value) noexcept : real(Value), imaginary() {};
		Complex::Complex(const BaseType::Vector1D& Value) noexcept : real(), imaginary(Value) {};
		Complex::Complex(const Complex& Value) noexcept : real(Value.real), imaginary(Value.imaginary) {};
		double Complex::Scalar(const Complex& Value) noexcept { return Value.real; };
		BaseType::Vector1D Complex::Vector(const Complex& Value) noexcept { return Value.imaginary; };
		double Complex::Re(const Complex& z) noexcept { return Scalar(z); };
		double Complex::Im(const Complex& z) noexcept { return BaseType::Vector1D::dot(Vector(z), BaseType::Vector1D::e1); };
		///
		/// Operators
		///
		Complex Complex::operator ()() const noexcept { return *this; };
		double& Complex::operator [](index i) &
		{
			switch (i)
			{
			case index::s:
				return this->real;
			case index::i:
				return this->imaginary[BaseType::index::e1];
			default:
				throw_now(L"std::out_of_range", L"The index is out of range.");
			}
		};
		double& Complex::operator [](std::int64_t i) & noexcept
		{
			try { return this->operator[](static_cast<index>(i % Period<Complex>)); }
			catch (...) { return this->operator[](i); }
		};
		const double& Complex::operator [](index i) const&
		{
			switch (i)
			{
			case index::s:
				return this->real;
			case index::i:
				return this->imaginary[BaseType::index::e1];
			default:
				throw_now(L"std::out_of_range", L"The index is out of range.");
			}
		};
		const double& Complex::operator [](std::int64_t i) const& noexcept
		{
			try { return this->operator[](static_cast<index>(i % Period<Complex>)); }
			catch (...) { return this->operator[](i); }
		};
		bool operator ==(const Complex& Union, const Complex& Value) noexcept { return Union.to_number() == Value.to_number(); };
		bool operator !=(const Complex& Union, const Complex& Value) noexcept { return !(Union == Value); };
		Complex operator +(const Complex& Value) noexcept { return Value; };
		Complex operator -(const Complex& Value) noexcept { return Complex{}.from(-Value.to_number()); };
		Complex operator ~(const Complex& Value) noexcept { return Complex{}.from(~Value.to_number()); };
		Complex& Complex::operator ++() & noexcept
		{
			++this->real;
			return *this;
		};
		Complex Complex::operator ++(int) & noexcept
		{
			Complex temp = *this;
			++this->real;
			return temp;
		};
		Complex& Complex::operator --() & noexcept
		{
			--this->real;
			return *this;
		};
		Complex Complex::operator --(int) & noexcept
		{
			Complex temp = *this;
			--this->real;
			return temp;
		};
		Complex operator +(const Complex& Union, const Complex& Value) noexcept { return Complex{}.from(Union.to_number() + Value.to_number()); };
		Complex operator -(const Complex& Union, const Complex& Value) noexcept { return Complex{}.from(Union.to_number() - Value.to_number()); };
		Complex operator *(const Complex& Union, const Complex& Value) noexcept { return Complex{}.from(Union.to_number() * Value.to_number()); };
		Complex operator /(const Complex& Union, const Complex& Value) noexcept
		{
			try
			{
				if (Complex::Vector(Value) == 0) { return Complex{}.from(Union.to_number() / Complex::Scalar(Value)); }
				return Union * Complex::inverse(Value);
			}
			catch (...) { return 0; }
		};
		Complex operator ^(const Complex& Base, std::int64_t Exponent) noexcept
		{
			try { return Complex::Power(Base, static_cast<double>(Exponent)); }
			catch (...) { return 0; }
		};
		Complex& Complex::operator =(const Complex& Value) & noexcept
		{
			this->real = Value.real;
			this->imaginary = Value.imaginary;
			return *this;
		};
		Complex& Complex::operator +=(const Complex& Value) & noexcept { return *this = *this + Value; };
		Complex& Complex::operator +=(const std::initializer_list<Complex>& Value) & noexcept
		{
			for (std::initializer_list<Complex>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this += *ite; }
			return *this;
		};
		Complex& Complex::operator -=(const Complex& Value) & noexcept { return *this = *this - Value; };
		Complex& Complex::operator -=(const std::initializer_list<Complex>& Value) & noexcept
		{
			for (std::initializer_list<Complex>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this -= *ite; }
			return *this;
		};
		Complex& Complex::operator *=(const Complex& Value) & noexcept { return *this = *this * Value; };
		Complex& Complex::operator *=(const std::initializer_list<Complex>& Value) & noexcept
		{
			for (std::initializer_list<Complex>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this *= *ite; }
			return *this;
		};
		Complex& Complex::operator /=(const Complex& Value) & noexcept { return *this = *this / Value; };
		Complex& Complex::operator /=(const std::initializer_list<Complex>& Value) & noexcept
		{
			for (std::initializer_list<Complex>::const_iterator ite = Value.begin(); ite != Value.end(); ++ite) { *this /= *ite; }
			return *this;
		};
		Complex& Complex::operator ^=(std::int64_t Exponent) & noexcept { return *this = *this ^ Exponent; };
		Complex& Complex::operator ^=(const std::initializer_list<std::int64_t>& Exponent) & noexcept
		{
			for (std::initializer_list<std::int64_t>::const_iterator ite = Exponent.begin(); ite != Exponent.end(); ++ite) { *this ^= *ite; }
			return *this;
		};
		///
		/// Basic functions for constructing numbers
		///
		double Complex::abs(const Complex& Value) { return std::sqrt(dot(Value, Value)); };
		double Complex::arg(const Complex& Value) { return arg(Value, 0); };
		double Complex::arg(const Complex& Value, std::int64_t Theta) { return std::acos(Scalar(Value) / abs(Value)) + 2 * pi * static_cast<double>(Theta); };
		Complex Complex::conjg(const Complex& Value) { return ~Value; };
		Complex Complex::sgn(const Complex& Value) { return Value / abs(Value); };
		Complex Complex::inverse(const Complex& Value) { return conjg(Value) / dot(Value, Value); };
		Complex Complex::exp(const Complex& Value) { return std::exp(Scalar(Value)) * (std::cos(abs(Vector(Value))) + sgn(Vector(Value)) * std::sin(abs(Vector(Value)))); };
		Complex Complex::ln(const Complex& Value) { return ln(Value, 0); };
		Complex Complex::ln(const Complex& Value, std::int64_t Theta) { return std::log(abs(Value)) + sgn(Vector(Value)) * arg(Value, Theta); };
		///
		/// 1st rank tensor algorithms
		///
		double Complex::dot(const Complex& Union, const Complex& Value) noexcept { return Scalar(conjg(Union) * Value + conjg(Value) * Union) / 2; };
		Complex Complex::outer(const Complex& Union, const Complex& Value) noexcept { return (conjg(Union) * Value - conjg(Value) * Union) / 2; };
		Complex Complex::even(const Complex& Union, const Complex& Value) noexcept { return (Union * Value + Value * Union) / 2; };
		Complex Complex::cross(const Complex& Union, const Complex& Value) noexcept { return (Union * Value - Value * Union) / 2; };
		///
		/// Operation 3 algorithms
		///
		Complex Complex::Power(const Complex& Base, const Complex& Exponent) { return Power(Base, Exponent, 0); };
		Complex Complex::Power(const Complex& Base, const Complex& Exponent, std::int64_t Theta)
		{
			return exp(Exponent * ln(Base, Theta));
		};
		Complex Complex::Power(const Complex& Base, double Exponent) { return Power(Base, Exponent, 0); };
		Complex Complex::Power(const Complex& Base, double Exponent, std::int64_t Theta)
		{
			if (Base == 0) { return Exponent == 0 ? 1 : 0; }
			return std::pow(abs(Base), Exponent) *
				(std::cos(Exponent * arg(Base, Theta)) + sgn(Vector(Base)) * std::sin(Exponent * arg(Base, Theta)));
		};
		Complex Complex::Root(const Complex& Base, const Complex& Exponent) { return Root(Base, Exponent, 0); };
		Complex Complex::Root(const Complex& Base, const Complex& Exponent, std::int64_t Theta) { return Power(Base, inverse(Exponent), Theta); };
		Complex Complex::Root(const Complex& Base, double Exponent) { return Root(Base, Exponent, 0); };
		Complex Complex::Root(const Complex& Base, double Exponent, std::int64_t Theta) { return Power(Base, 1 / Exponent, Theta); };
		Complex Complex::Log(const Complex& Base, const Complex& Number) { return Log(Base, Number, 0, 0); };
		Complex Complex::Log(const Complex& Base, const Complex& Number, std::int64_t Theta, std::int64_t Phi)
		{
			return ln(Number, Theta) / ln(Base, Phi);
		};
		///
		/// Trigonometric functions
		///
		Complex Complex::sin(const Complex& Value)
		{
			return std::sin(Scalar(Value)) * std::cosh(abs(Vector(Value)))
				+ std::cos(Scalar(Value)) * sgn(Vector(Value)) * std::sinh(abs(Vector(Value)));
		};
		Complex Complex::arcsin(const Complex& Value) { return arcsin(Value, true, 0); };
		Complex Complex::arcsin(const Complex& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return -sgn(Vector(Value)) * arcsinh(Value * sgn(Vector(Value))); }
			else { return pi - arcsin(Value, true, Period); }
		};
		Complex Complex::sinh(const Complex& Value)
		{
			return std::sinh(Scalar(Value)) * std::cos(abs(Vector(Value)))
				+ std::cosh(Scalar(Value)) * sgn(abs(Vector(Value))) * std::sin(abs(Vector(Value)));
		};
		Complex Complex::arcsinh(const Complex& Value) { return arcsinh(Value, true, 0); };
		Complex Complex::arcsinh(const Complex& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return ln(Value + Root(Power(Value, 2) + 1, 2), Period); }
			else { return pi * sgn(Vector(Value)) - arcsinh(Value, true, Period); }
		};
		Complex Complex::cos(const Complex& Value)
		{
			return std::cos(Scalar(Value)) * std::cosh(abs(Vector(Value)))
				- std::sin(Scalar(Value)) * sgn(Vector(Value)) * std::sinh(abs(Vector(Value)));
		};
		Complex Complex::arccos(const Complex& Value) { return arccos(Value, true, 0); };
		Complex Complex::arccos(const Complex& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return -sgn(Vector(Value)) * arccosh(Value); }
			else { return 2 * pi - arccos(Value, true, Period); }
		};
		Complex Complex::cosh(const Complex& Value)
		{
			return std::cosh(Scalar(Value)) * std::cos(abs(Vector(Value)))
				+ std::sinh(Scalar(Value)) * sgn(abs(Vector(Value))) * std::sin(abs(Vector(Value)));
		};
		Complex Complex::arccosh(const Complex& Value) { return arccosh(Value, true, 0); };
		Complex Complex::arccosh(const Complex& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return ln(Value + Root(Power(Value, 2) - 1, 2), Period); }
			else { return 2 * pi * sgn(Vector(Value)) - arccosh(Value, true, Period); }
		};
		Complex Complex::tan(const Complex& Value)
		{
			return Root(Power(sec(Value), 2) - 1, 2);
		};
		Complex Complex::arctan(const Complex& Value) { return arctan(Value, true, 0); };
		Complex Complex::arctan(const Complex& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return -sgn(Vector(Value)) * arctanh(Value * sgn(Vector(Value))); }
			else { return pi + arctan(Value, true, Period); }
		};
		Complex Complex::tanh(const Complex& Value)
		{
			return 1 - 2 * inverse(exp(2 * Value) + 1);
		};
		Complex Complex::arctanh(const Complex& Value) { return arctanh(Value, true, 0); };
		Complex Complex::arctanh(const Complex& Value, bool Sign, std::int64_t Period)
		{
			if (Sign == true) { return (ln(1 + Value, Period) - ln(1 - Value)) / 2; }
			else { return pi * sgn(Vector(Value)) + arctanh(Value, true, Period); }
		};
		Complex Complex::csc(const Complex& Value) { return inverse(sin(Value)); };
		Complex Complex::arccsc(const Complex& Value) { return arccsc(Value, true, 0); };
		Complex Complex::arccsc(const Complex& Value, bool Sign, std::int64_t Period) { return arcsin(inverse(Value), Sign, Period); };
		Complex Complex::csch(const Complex& Value) { return inverse(sinh(Value)); };
		Complex Complex::arccsch(const Complex& Value) { return arccsch(Value, true, 0); };
		Complex Complex::arccsch(const Complex& Value, bool Sign, std::int64_t Period) { return arcsinh(inverse(Value), Sign, Period); };
		Complex Complex::sec(const Complex& Value) { return inverse(cos(Value)); };
		Complex Complex::arcsec(const Complex& Value) { return arcsec(Value, true, 0); };
		Complex Complex::arcsec(const Complex& Value, bool Sign, std::int64_t Period) { return arccos(inverse(Value), Sign, Period); };
		Complex Complex::sech(const Complex& Value) { return inverse(cosh(Value)); };
		Complex Complex::arcsech(const Complex& Value) { return arcsech(Value, true, 0); };
		Complex Complex::arcsech(const Complex& Value, bool Sign, std::int64_t Period) { return arccosh(inverse(Value), Sign, Period); };
		Complex Complex::cot(const Complex& Value) { return inverse(tan(Value)); };
		Complex Complex::arccot(const Complex& Value) { return arccot(Value, true, 0); };
		Complex Complex::arccot(const Complex& Value, bool Sign, std::int64_t Period) { return arctan(inverse(Value), Sign, Period); };
		Complex Complex::coth(const Complex& Value) { return inverse(tanh(Value)); };
		Complex Complex::arccoth(const Complex& Value) { return arccoth(Value, true, 0); };
		Complex Complex::arccoth(const Complex& Value, bool Sign, std::int64_t Period) { return arctanh(inverse(Value), Sign, Period); };
		///
		/// Conversion of Types
		///
		std::wstring Complex::CType_String(const Complex& Value)
		{
			return ToString(Value[index::s], L"", Value[index::i], L"i");
		};
		Complex Complex::CType_Complex(const std::wstring& Value)
		{
			Complex Object;
			ToNumbers(Value, Object[index::s], L"", Object[index::i], L"i");
			return Object;
		};
	}
	double BaseType::Vector1D::build_in_dot(const BaseType::Vector1D& Union, const BaseType::Vector1D& Value)
	{
		return MainType::Complex::dot(MainType::Complex{ Union }, MainType::Complex{ Value });
	};
	BaseType::Vector1D BaseType::Vector1D::build_in_cross(const BaseType::Vector1D& Union, const BaseType::Vector1D& Value)
	{
		return Vector1D{}.from(MainType::Complex::cross(MainType::Complex{ Union }, MainType::Complex{ Value }).to_number());
	};
}
