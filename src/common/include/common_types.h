/*
 * Type definitions used in both the master and slave applications.
 */
#ifndef _COMMON_TYPES_H
#define _COMMON_TYPES_H

/*
 * Conditionally define a function as static inline.
 *
 * This is used to make a function static inline when building for the target,
 * but extern when building for unit tests.  This allows the function to be
 * inlined in the release build for maximum efficiency while still allowing it
 * to be mocked out during testing.
 */
#ifdef TEST
// Function is extern during testing to allow mocking.
#define INLINE_ON_TARGET extern
#else
// Function is static inline in production for speed optimization.
#define INLINE_ON_TARGET __attribute__((always_inline)) static inline
#endif

#endif
