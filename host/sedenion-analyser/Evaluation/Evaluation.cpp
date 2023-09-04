#include <emscripten.h>
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
			Object.freeze(Module.onAbort);
		);
	};
};
const evaluate_t evaluate_t::evaluate{};
struct evaluate_local
{
	evaluate_local* lastest_state;
	void(*caught_delegate)(const std::exception& ex);
	std::jmp_buf stack_pointer;
};
static const std::runtime_error runtime_error("An unhandled exception has occurred.");
static thread_local const std::exception* rethrow_captured{ &runtime_error };
static thread_local evaluate_local* local_pointer{ nullptr };
void evaluate(void(*operate)(), void(*caught)(const std::exception& ex)) noexcept
{
	evaluate_local local{ local_pointer, caught };
	auto jump_yield = setjmp(local.stack_pointer);
	if (jump_yield == 0)
	{
		local_pointer = &local;
		operate();
	}
	else if (jump_yield == -1)
	{
		local_pointer = local.lastest_state;
		rethrow_captured = &runtime_error;
		caught(runtime_error);
	}
};
void throw_now(const std::exception& ex) noexcept
{
	evaluate_local* local_reference = local_pointer;
	if (local_reference == nullptr) { std::terminate(); }
	local_pointer = local_reference->lastest_state;
	rethrow_captured = &ex;
	local_reference->caught_delegate(ex);
	std::longjmp(local_reference->stack_pointer, 1);
};
void rethrow_current() noexcept
{
	evaluate_local* local_reference = local_pointer;
	if (local_reference == nullptr) { std::terminate(); }
	local_pointer = local_reference->lastest_state;
	if (rethrow_captured == nullptr) { rethrow_captured = &runtime_error; }
	local_reference->caught_delegate(*rethrow_captured);
	std::longjmp(local_reference->stack_pointer, 1);
};
void EMSCRIPTEN_KEEPALIVE abort_unwind() noexcept
{
	if (local_pointer != nullptr) { std::longjmp(local_pointer->stack_pointer, -1); }
};
