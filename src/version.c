#include <glimpse/future.h>
#define Y "=yes"
#define N "=no"
const char* glimpse_name = "glimpse";
const char* glimpse_version = VERSION;
const char* glimpse_configure = 
"multi-char-seperator"
#ifdef STRING_SEPERATOR_SUPPORT
	Y
#else
	N
#endif
",parser-stack"
#ifdef HANDLER_STACK
	Y
#else
	N
#endif
",multi-threading"
#ifdef THREAD_SAFE
	Y
#else
	N
#endif
",lazy-instance"
#ifdef LAZY_INSTANCE
	Y
#else
	N
#endif
",char-table"
#ifdef CHAR_HASH_TABLE
	Y
#else
	N
#endif
;
