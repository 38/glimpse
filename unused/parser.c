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
/*const char* glimpse_parser_parse(GlimpseParseTree_t* tree, const char* text, GlimpseDataInstance_t* storage)
{
}*/
