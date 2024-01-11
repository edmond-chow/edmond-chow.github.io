#pragma once
#ifndef __EVALUATION__
#define __EVALUATION__
#include <stdexcept>
using operate_t = void(*)();
using caught_t = void(*)(const std::exception& ex);
void evaluate(operate_t operate, caught_t caught) noexcept;
void throw_now(const std::exception& ex) noexcept;
void rethrow_current() noexcept;
#define throw_now(ex) throw_now(ex); throw;
#endif
