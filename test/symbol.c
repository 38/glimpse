#include <glimpse/symbol.h>
#include <stdio.h>
#include <assert.h>
int main()
{
	int temp[100];
	int i;
	for(i = 0; i < 100; i ++) temp[i] = i;
	for(i = 0; i < 100; i ++)
	{
		char buf[100];
		sprintf(buf,"symbol_%d",i);
		glimpse_symbol_exportsymbol(buf,temp + i);
	}
	for(i = 0 ; i < 100; i ++)
	{
		char buf[100];
		sprintf(buf, "symbol_%d", i);
		int* sym = (int*)glimpse_symbol_importsymbol(buf);
		assert(*sym == i);
		temp[i] = 101;
		assert(*sym == 101);
		assert(sym == temp + i);
	}
	glimpse_symbol_cleaup();
	return 0;
}
