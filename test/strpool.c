#include <strpool.h>
#include <assert.h>
int main()
{
	glimpse_strpool_init();
	char a[] = "test";
	char* b = glimpse_strpool_new(a);
	a[0]='T';
	assert(0 == strcmp(b,"test"));
	assert(0 == strcmp(a,"Test"));
	char* c = glimpse_strpool_new(a);
	assert(0 == strcmp(c,"Test"));
	glimpse_strpool_cleanup();
	return 0;
}
