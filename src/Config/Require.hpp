#pragma once

#include "Config.hpp"

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace pokesim::debug {
class require : public std::exception {
  std::string errorMessage;

 public:
  static inline std::size_t count = 0U;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  require(const char* file, int line, const char* function, const char* condition, const std::string& message) {
    errorMessage += file;
    errorMessage += ":" + std::to_string(line);

    errorMessage += "  ";
    errorMessage += function;
    errorMessage += "  ( ";
    errorMessage += condition;
    errorMessage += " )";

    if (!message.empty()) {
      errorMessage += "  \" ";
      errorMessage += message;
      errorMessage += " \"";
    }
  }

  const char* what() const noexcept override { return errorMessage.c_str(); }
};
}  // namespace pokesim::debug

// NOLINTBEGIN cppcoreguidelines-macro-usage
#if defined __clang__ || defined __GNUC__
#define POKESIM_REQUIRE(condition, message) \
  pokesim::debug::require::count++;         \
  if (!(condition)) throw pokesim::debug::require(__FILE__, __LINE__, __PRETTY_FUNCTION__, #condition, message)
#elif defined _MSC_VER
#define POKESIM_REQUIRE(condition, message) \
  pokesim::debug::require::count++;         \
  if (!(condition)) throw pokesim::debug::require(__FILE__, __LINE__, __FUNCSIG__, #condition, message)
#endif

#else
#define POKESIM_REQUIRE(condition, message)
#endif

// An assert with no message. For use in debug checks only.
#define POKESIM_REQUIRE_NM(condition) POKESIM_REQUIRE(condition, "")
#define POKESIM_REQUIRE_FAIL(message) POKESIM_REQUIRE(false, message)
// NOLINTEND cppcoreguidelines-macro-usage