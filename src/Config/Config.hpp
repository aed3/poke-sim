#pragma once

#ifdef POKESIM_ENABLE_TESTING
#define POKESIM_TESTING
#endif

#if !defined(NDEBUG) || defined(POKESIM_TESTING)
#define POKESIM_DEBUG_CHECK_UTILITIES
#endif