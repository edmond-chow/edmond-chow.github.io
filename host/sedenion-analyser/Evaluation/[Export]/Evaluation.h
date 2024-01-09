#pragma once
#ifndef __EVALUATION__
#define __EVALUATION__
#include <stdexcept>
typedef void(*operate_t)();
typedef void(*caught_t)(const std::exception& ex);
void evaluate(void(*operate)(), void(*caught)(const std::exception& ex)) noexcept;
void throw_now(const std::exception& ex) noexcept;
void rethrow_current() noexcept;
#define throw_now(ex) throw_now(ex); throw;
#endif
