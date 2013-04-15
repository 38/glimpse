#include <parser.h>
#include <log.h>
/* TODO: how to determin when the scanner stop */
const char* glimpse_parser_parse(GlimpseParseTree_t* tree, const char* text, char stop_char, GlimpseDataInstance_t* storage)
{
	if(NULL == tree || NULL == text || NULL == storage) return NULL;
	GlimpseParserStatus_t status = NULL;
	const char* p;
	for(p = text; p && *p != stop_char && *p;)
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
