#include <chartable.h>
#include <assert.h>
int main()
{
	int i;
	GlimpseCharTable_t* table = glimpse_chartable_new();
	char data[] = "thisapiabcdefghijklmnopqrstuvw";
	for(i = 0; data[i]; i ++)
		glimpse_chartable_insert(table, (unsigned char)data[i], data + i);
	for(i = 0; data[i]; i ++)
		assert(data[i] == *(char*)glimpse_chartable_find(table, (unsigned char)data[i]));
	glimpse_chartable_free(table);
	table = glimpse_chartable_new();
	for(i = 0; i < 255; i ++)
		glimpse_chartable_insert(table, i, (void*)i);
	for(i = 0; i < 255; i ++)
		assert((void*)i == glimpse_chartable_find(table, i));
	glimpse_chartable_free(table);
	return 0;
}
