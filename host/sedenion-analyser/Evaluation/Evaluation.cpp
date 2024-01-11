#include <csetjmp>
#include <stdexcept>
using operate_t = void(*)();
using caught_t = void(*)(const std::exception& ex);
enum ev_state : int
{
	before = -1,
	operate = 0,
	after = 1,
};
struct eval
{
	eval* prev;
	void(*handler)(const std::exception& ex);
	std::jmp_buf sp;
};
static const std::runtime_error err("An unhandled exception has occurred.");
static thread_local const std::exception* captured{ nullptr };
static thread_local eval* last{ nullptr };
void evaluate(operate_t operate, caught_t caught) noexcept
{
	eval local{ last, caught };
	int jmp = setjmp(local.sp);
	if (jmp == ev_state::operate)
	{
		last = &local;
		operate();
		last = local.prev;
	}
	else if (jmp == ev_state::before)
	{
		last = local.prev;
		captured = &err;
		caught(err);
		captured = nullptr;
	}
};
void throw_now(const std::exception& ex) noexcept
{
	eval* local = last;
	if (local == nullptr) { std::terminate(); }
	last = local->prev;
	captured = &ex;
	local->handler(ex);
	captured = nullptr;
	std::longjmp(local->sp, ev_state::after);
};
void rethrow_current() noexcept
{
	eval* local = last;
	if (local == nullptr) { std::terminate(); }
	last = local->prev;
	if (captured == nullptr) { captured = &err; }
	local->handler(*captured);
	captured = nullptr;
	std::longjmp(local->sp, ev_state::after);
};
void abort_unwind() noexcept
{
	if (last != nullptr) { std::longjmp(last->sp, ev_state::before); }
};
