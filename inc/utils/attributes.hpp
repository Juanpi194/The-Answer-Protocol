#pragma once
#include <cassert>

#define TAP_COLD			__attribute__((cold))
#define TAP_NONNULL			__attribute__((nonnull))
#define TAP_DEPRECATED		__attribute__((deprecated))
#define TAP_UNUSED_RESULT	__attribute__((warn_unused_result))
#define TAP_RETURNS_NONNULL	__attribute__((returns_nonnull))
#define TAP_ALWAYS_INLINE	__attribute__((__always_inline__))
