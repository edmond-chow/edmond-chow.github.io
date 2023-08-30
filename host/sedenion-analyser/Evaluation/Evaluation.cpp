#include <emscripten.h>
#include <emscripten/bind.h>
#include <csetjmp>
#include <stdexcept>
struct evaluate_t
{
private:
	static const evaluate_t evaluate;
	evaluate_t()
	{
		EM_ASM(
			Module.onAbort = () => { Module.__Z12abort_unwindv(); };
		);
	};
};
const evaluate_t evaluate_t::evaluate{};
struct evaluate_local
{
	evaluate_local* lastest_state;
	void(*caught_delegate)(const std::exception& ex);
	std::jmp_buf stack_pointer;
	const std::exception* rethrow_captured;
};
static thread_local evaluate_local* local_pointer{ nullptr };
static thread_local std::size_t throw_in_catch_count = 0;
void evaluate(void(*operate)(), void(*caught)(const std::exception& ex)) noexcept
{
	evaluate_local local{ local_pointer, caught };
	auto jump_yield = setjmp(local.stack_pointer);
	if (jump_yield == 0)
	{
		local_pointer = &local;
		operate();
	}
	local_pointer = local.lastest_state;
	if (jump_yield == -1) { caught(std::runtime_error("An unhandled exception has occurred.")); }
};
void throw_now(const std::exception& ex) noexcept
{
	++throw_in_catch_count;
	if (local_pointer == nullptr) { std::terminate(); }
	local_pointer->rethrow_captured = &ex;
	local_pointer->caught_delegate(ex);
	while (--throw_in_catch_count > 0)
	{
		local_pointer = local_pointer->lastest_state;
		if (local_pointer == nullptr) { std::terminate(); }
	}
	std::longjmp(local_pointer->stack_pointer, 1);
};
void rethrow_current() noexcept
{
	++throw_in_catch_count;
	if (local_pointer == nullptr) { std::terminate(); }
	local_pointer->caught_delegate(*local_pointer->rethrow_captured);
	while (--throw_in_catch_count > 0)
	{
		local_pointer = local_pointer->lastest_state;
		if (local_pointer == nullptr) { std::terminate(); }
	}
	std::longjmp(local_pointer->stack_pointer, 1);
};
#ifndef EMSCRIPTEN_KEEPALIVE
#define EMSCRIPTEN_KEEPALIVE
#endif
void EMSCRIPTEN_KEEPALIVE abort_unwind() noexcept
{
	if (local_pointer != nullptr) { std::longjmp(local_pointer->stack_pointer, -1); }
};
