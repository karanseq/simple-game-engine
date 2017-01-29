#ifndef HELPER_MACROS_H_
#define HELPER_MACROS_H_

#define SAFE_DELETE(ptr)						do { if (ptr) { delete (ptr); (ptr) = nullptr; } } while (0)
#define SAFE_DELETE_ARRAY(ptr)					do { if (ptr) { delete[] (ptr); (ptr) = nullptr; } } while (0)
#define SAFE_FREE(ptr)							do { if (ptr) { free((ptr)); (ptr) = nullptr; } } while (0)

#endif // HELPER_MACROS_H_