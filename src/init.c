#include <init.h>
#include <symbol.h>
#include <typesystem.h>
/* call the init function */
void glimpse_init()
{
	glimpse_typesystem_init();
	Glimpse_TypeAPI_init();
}
/* call the cleanup function */
void glimpse_cleanup()
{
	glimpse_typesystem_cleanup();
	glimpse_symbol_cleaup();
	glimpse_pluginloader_cleanup();
}
