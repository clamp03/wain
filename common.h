#include <assert.h>

#define DEV_ASSERT(cond, message) if (!(cond)) { cerr << message << endl; assert(cond); }
#define NOT_YET_IMPLEMENTED DEV_ASSERT(false, "Not yet implemented")
