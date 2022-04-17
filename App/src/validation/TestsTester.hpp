#pragma once

namespace {

inline int factorial(int number)
{
    // return number <= 1 ? number : factorial(number - 1) * number;  // fail
    return number <= 1 ? 1 : factorial(number - 1) * number; // pass
}

} // namespace
