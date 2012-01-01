#include <TypeAPI.h>
void alias_types()
{
	Alias("integer{size:32, signed:true, format:dec}","int32d");
	Alias("integer{size:32, signed:false, format:dec}","uint32d");
	Alias("integer{size:32, signed:true, format:hex}","int32h");
	Alias("integer{size:32, signed:false, format:hex}","uint32h");

	Alias("integer{size:64, signed:true, format:dec}","int64d");
	Alias("integer{size:64, signed:false, format:dec}","uint64d");
	Alias("integer{size:64, signed:true, format:hex}","int64h");
	Alias("integer{size:64, signed:false, format:hex}","uint64h");
}
