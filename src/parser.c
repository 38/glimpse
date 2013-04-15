#include <parser.h>
#include <log.h>
/* TODO: how to determin when the scanner stop , use a handler stack , so parser can track back the stack, obtain infomation of ancestors
 * algorithm for termination:
 * there is a set of string that cause the parser stop parsing text, called stop-set.
 * if the parent is sublog, stop-set = {parent's field seprator} + parent's stop-set
 * if the parent is vector, stop-set = {parent's vector seprator} + parent's stop-set
 * for the parser in the bottom of handler stack, stop-set = {"\0"}
 * 
 * A externed version of KMP algorithm is needed for this problem. 
 * The automa should be cached in memory, and hashed in a hash table.
 * The hash function should caculated as the parse function called, 
 * so the time comsuption of hash function is reasonable 
 */
const char* glimpse_parser_parse(GlimpseParseTree_t* tree, const char* text, GlimpseDataInstance_t* storage)
{
	if(NULL == tree || NULL == text || NULL == storage) return NULL;
	GlimpseParserStatus_t status = NULL;
	const char* p;
	for(p = text; p && *p;)
	{
		if(NULL == status) status = glimpse_tree_scan_start(tree);
		if(NULL == status)
		{
			GLIMPSE_LOG_FATAL("can not initialize scanner, abort");
			return NULL;
		}
		status = glimpse_tree_scan(status, *p);
		if(status->term) /* got a key */
		{
			int id = status->s.terminus.idx;
			GlimpseTypeHandler_t* handler = status->s.terminus.handler;
			const char* next_p = handler->parse(p, storage->data[id], handler->parse_data); /* parse the value */
			if(NULL == next_p)
			{
				GLIMPSE_LOG_FATAL("value parser returns a error status, abort");
				return NULL;
			}
			int i;
			for(i = 0; tree->sep_f[i] && next_p[i] && tree->sep_f[i] == next_p[i]; i ++);
			if(0 == tree->sep_f[i]) /* a seprator */
				p = next_p + i;
			else /* something unexcepted */
				return next_p; /* scan terminated */
			status = NULL;  /* ready to scan next field */
		}
	}
	return p;
}
