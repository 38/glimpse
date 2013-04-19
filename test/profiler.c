#include <profiler.h>
int main()
{
	int i;
	int sum = 0;
	GLIMPSE_STATEMENT_PROFILER(
	for(i = 0; i < 100000000; i ++)
		sum = GLIMPSE_EXPRESSION_PROFILER(sum + i);
	);
	return 0;
}
