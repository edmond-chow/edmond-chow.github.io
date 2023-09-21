#include <Evaluation.h>
#include <algorithm>
#include <stdexcept>
constexpr bool is_factor(std::size_t n) noexcept
{
	if (n == 1) { return true; }
	else if (n == 0 || (n >> 1 << 1 != n)) { return false; }
	return is_factor(n >> 1);
};
class Factor
{
private:
	double* data;
	std::size_t size;
public:
	constexpr const double* get_data() const { return data; };
	constexpr std::size_t get_size() const { return size; };
	constexpr double& operator [](std::int64_t i) & noexcept { return data[i % size]; };
	constexpr const double& operator [](std::int64_t i) const& noexcept { return data[i % size]; };
	constexpr Factor(std::size_t size)
		: data{ nullptr }, size{ size }
	{
		data = new double[size] {};
	};
	constexpr Factor(const double* data, std::size_t size)
		: data{ nullptr }, size{ size }
	{
		this->data = new double[size] {};
		std::copy(data, data + size, this->data);
	};
	constexpr Factor(const std::initializer_list<double>& init)
		: data{ nullptr }, size{ init.size() }
	{
		data = new double[size] {};
		std::copy(init.begin(), init.end(), data);
	};
	constexpr Factor(const Factor& Value)
		: data(new double[Value.size] {}), size(Value.size)
	{
		std::copy(Value.data, Value.data + Value.size, data);
	};
	constexpr Factor(Factor&& Value) noexcept
		: data(Value.data), size(Value.size)
	{
		Value.data = nullptr;
		Value.size = 0;
	};
	constexpr Factor& operator =(const Factor& Value) &
	{
		if (this == &Value) { return *this; }
		delete[] data;
		data = new double[Value.size] {};
		size = Value.size;
		std::copy(Value.data, Value.data + Value.size, data);
		return *this;
	};
	constexpr Factor& operator =(Factor&& Value) & noexcept
	{
		data = Value.data;
		size = Value.size;
		Value.data = nullptr;
		Value.size = 0;
		return *this;
	};
	constexpr ~Factor() noexcept { delete[] data; };
	constexpr Factor& extend(std::size_t size) &
	{
		if (size > this->size)
		{
			double* new_data = new double[size] {};
			std::copy(data, data + this->size, new_data);
			delete[] data;
			data = new_data;
		};
		return *this;
	};
	static Factor merge(const Factor& Left, const Factor& Right)
	{
		Factor Output(Left.size + Right.size);
		std::copy(Right.data, Right.data + Right.size, std::copy(Left.data, Left.data + Left.size, Output.data));
		return Output;
	};
	constexpr Factor left(std::size_t count) const
	{
		if (0 >= count || count > size) { throw_now(std::out_of_range("The count is out of range.")); }
		Factor Output(count);
		std::copy(data, data + count, Output.data);
		return Output;
	};
	constexpr Factor right(std::size_t count) const
	{
		if (0 > count || count >= size) { throw_now(std::out_of_range("The count is out of range.")); }
		Factor Output(size - count);
		std::copy(data + count, data + size, Output.data);
		return Output;
	};
	constexpr Factor left() const
	{
		return left(size >> 1);
	};
	constexpr Factor right() const
	{
		return right(size >> 1);
	};
	friend constexpr Factor operator -(const Factor& Value);
	friend constexpr Factor operator ~(const Factor& Value);
	friend constexpr Factor operator +(const Factor& Union, const Factor& Value);
	friend constexpr Factor operator -(const Factor& Union, const Factor& Value);
	friend constexpr Factor operator *(const Factor& Union, const Factor& Value);
	friend constexpr Factor operator *(double Union, const Factor& Value);
	friend constexpr Factor operator *(const Factor& Union, double Value);
	friend constexpr Factor operator /(const Factor& Union, double Value);
};
constexpr Factor operator -(const Factor& Value)
{
	Factor Output{ Value.data, Value.size };
	const double* ite_oe = Output.data + Output.size;
	for (double* ite_o = Output.data; ite_o != ite_oe; ++ite_o) { *ite_o = -*ite_o; }
	return Output;
};
constexpr Factor operator ~(const Factor& Value)
{
	Factor Output{ Value.data, Value.size };
	const double* ite_oe = Output.data + Output.size;
	for (double* ite_o = Output.data + 1; ite_o != ite_oe; ++ite_o) { *ite_o = -*ite_o; }
	return Output;
};
constexpr Factor operator +(const Factor& Union, const Factor& Value)
{
	if (Union.size > Value.size) { return Value + Union; }
	Factor Output{ Value.data, Value.size };
	const double* ite_u = Union.data;
	const double* ite_ue = Union.data + Union.size;
	for (double* ite_o = Output.data; ite_u != ite_ue; ++ite_o, ++ite_u) { *ite_o += *ite_u; }
	return Output;
};
constexpr Factor operator -(const Factor& Union, const Factor& Value)
{
	return Union + (-Value);
};
constexpr Factor operator *(const Factor& Union, const Factor& Value)
{
	if (!is_factor(Union.size) || !is_factor(Value.size))
	{
		throw_now(std::invalid_argument("The size must be a number which is 2 to the power of a natural number."));
	}
	else if (Union.size != Value.size)
	{
		std::size_t NewSize = std::max(Union.size, Value.size);
		Factor NewUnion = Union;
		NewUnion.extend(NewSize);
		Factor NewValue = Union;
		NewValue.extend(NewSize);
		return NewUnion * NewValue;
	}
	else if (Union.size == 1) { return Factor{ Union[0] * Value[0] }; }
	return Factor::merge(
		Union.left() * Value.left() - ~Value.right() * Union.right(),
		Value.right() * Union.left() + Union.right() * ~Value.left()
	);
};
constexpr Factor operator *(double Union, const Factor& Value)
{
	Factor Output{ Value.data, Value.size };
	for (double* ite = Value.data; ite != Value.data + Value.size; ++ite) { *ite *= Union; }
	return Output;
};
constexpr Factor operator *(const Factor& Union, double Value)
{
	return Value * Union;
};
constexpr Factor operator /(const Factor& Union, double Value)
{
	return Union * (1 / Value);
};
