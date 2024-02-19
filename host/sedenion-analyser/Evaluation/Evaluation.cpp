#include <csetjmp>
#include <stdexcept>
#include <functional>
using operate_t = std::function<void()>;
using caught_t = std::function<void(const std::exception& ex)>;
enum ev_state : int
{
	before = -1,
	operate = 0,
	after = 1,
};
static const std::runtime_error unhandled{ "An unhandled exception has occurred." };
struct eval
{
private:
	static thread_local eval* last;
	static thread_local const std::exception* captured;
	eval* previous;
	const caught_t* handler;
	std::jmp_buf sp;
public:
	eval(const caught_t& handler)
		: previous{ last }, handler{ &handler }, sp{}
	{
		last = this;
	};
	std::jmp_buf& success()
	{
		last = this->previous;
		return this->sp;
	};
	std::jmp_buf& failure(const std::exception* ex_ptr)
	{
		last = this->previous;
		if (ex_ptr != nullptr) { captured = ex_ptr; }
		else if (captured == nullptr) { captured = &unhandled; }
		this->handler->operator()(*captured);
		captured = nullptr;
		return this->sp;
	};
	std::jmp_buf& get_sp()
	{
		return this->sp;
	};
	static eval* get_last()
	{
		return last;
	};
};
thread_local eval* eval::last{ nullptr };
thread_local const std::exception* eval::captured{ nullptr };
void evaluate(const operate_t& operate, const caught_t& caught) noexcept
{
	eval local{ caught };
	int jmp = setjmp(local.get_sp());
	if (jmp == ev_state::operate)
	{
		operate.operator()();
		eval::get_last()->success();
	}
	else if (jmp == ev_state::before)
	{
		eval::get_last()->failure(&unhandled);
	}
};
void throw_now(const std::exception& ex) noexcept
{
	if (eval::get_last() == nullptr) { std::terminate(); }
	std::longjmp(eval::get_last()->failure(&ex), ev_state::after);
};
void rethrow_current() noexcept
{
	if (eval::get_last() == nullptr) { std::terminate(); }
	std::longjmp(eval::get_last()->failure(nullptr), ev_state::after);
};
void abort_unwind() noexcept
{
	if (eval::get_last() != nullptr) { std::longjmp(eval::get_last()->get_sp(), ev_state::before); }
};
struct fn_list
{
private:
	static const std::size_t count = 32;
	static std::size_t slot;
	static fn_list* last;
	fn_list* previous;
	void (*func[count])(void*);
	void* arg[count];
	fn_list() = default;
	static fn_list* get_default()
	{
		static fn_list list{};
		return &list;
	};
public:
	static int push(void (*func)(void*), void* arg)
	{
		if (empty()) { last = get_default(); }
		if (slot == count)
		{
			fn_list* node = new (std::nothrow) fn_list{};
			if (node == nullptr) { return -1; }
			node->previous = last;
			last = node;
			slot = 0;
		}
		last->func[slot] = func;
		last->arg[slot] = arg;
		++slot;
		return 0;
	};
	static int pop()
	{
		if (empty()) { return -1; }
		--slot;
		void (*fn_ptr)(void*) = last->func[slot];
		if (fn_ptr != nullptr) { (*fn_ptr)(last->arg[slot]); }
		if (!empty() && slot == 0)
		{
			slot = count;
			fn_list* node = last;
			last = node->previous;
			delete node;
		}
		return 0;
	};
	static bool empty()
	{
		return last == nullptr || (last == get_default() && slot == 0);
	};
};
std::size_t fn_list::slot{ 0 };
fn_list* fn_list::last{ nullptr };
extern "C" int __cxa_atexit(void (*func)(void*), void* arg, void* dso_handle)
{
	return fn_list::push(func, arg);
};
extern "C" void __funcs_on_exit()
{
	while (!fn_list::empty())
	{
		fn_list::pop();
	}
};
