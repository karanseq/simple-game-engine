#ifndef ASSERT_H_
#define ASSERT_H_

#include <assert.h>

/*
    A simple assert utility.
    Features:
    - Supports a simple assert function in debug mode at all verbosity levels
    ToDo:
    - Add an assert that outputs a message
*/

#if defined(BUILD_DEBUG)
// in debug mode, enable assert
#define ASSERT(cond)                    assert((cond))
#else
// in release mode, disable assert and verbose
#define ASSERT(cond)                    void(0)
#endif

#endif // ASSERT_H_