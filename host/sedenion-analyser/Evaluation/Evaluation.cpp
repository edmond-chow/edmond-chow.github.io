/*
 *   BSD 2-Clause License
 *   
 *   Copyright (c) 2024 Edmond Chow
 *   
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are met:
 *   
 *   1. Redistributions of source code must retain the above copyright notice, this
 *      list of conditions and the following disclaimer.
 *   
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
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
