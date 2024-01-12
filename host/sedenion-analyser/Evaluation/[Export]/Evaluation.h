#pragma once
#ifndef __EVALUATION__
#define __EVALUATION__
#include <stdexcept>
#include <functional>
using operate_t = std::function<void()>;
using caught_t = std::function<void(const std::exception& ex)>;
void evaluate(const operate_t& operate, const caught_t& caught) noexcept;
void throw_now(const std::exception& ex) noexcept;
void rethrow_current() noexcept;
#define throw_now(ex) throw_now(ex); throw;
#endif
