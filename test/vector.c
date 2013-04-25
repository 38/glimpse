#include <glimpse/vector.h>
#include <assert.h>
int main()
{
	GlimpseVector_t* vec = glimpse_vector_new(sizeof(long long));
	long long i;
	for(i = 0; i < 1000; i ++)
		assert(0 == glimpse_vector_push(vec, &i));
	for(i = 0; i < 1000; i ++)
	{
		int* ret = glimpse_vector_get(vec, i);
		assert(NULL != ret);
		assert(ret[0] == i);
		assert(ret[1] == 0);
	}
	for(i = 0; i < 1000; i ++)
		assert(0 == glimpse_vector_push(vec, &i));
	for(i = 0; i < 1000; i ++)
	{
		int* ret = glimpse_vector_get(vec, i + 1000);
		assert(NULL != ret);
		assert(ret[0] == i);
		assert(ret[1] == 0);
	}
	glimpse_vector_init(vec);
	for(i = 0; i < 2000; i ++)
		assert(0 == glimpse_vector_push(vec, &i));
	for(i = 0; i < 2000; i ++)
	{
		int* ret = glimpse_vector_get(vec, i);
		assert(NULL != ret);
		assert(ret[0] == i);
		assert(ret[1] == 0);
	}
	return 0;
}
