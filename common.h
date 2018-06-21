#include <assert.h>
#include <stdint.h>
#include <iostream>

#define DEV_ASSERT(cond, message) if (!(cond)) { std::cerr << message << std::endl; assert(cond); }
#define NOT_YET_IMPLEMENTED DEV_ASSERT(false, "Not yet implemented")
