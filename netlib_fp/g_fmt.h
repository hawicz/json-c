
#ifndef _json_c_g_fmt_h_
#define _json_c_g_fmt_h_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * json-c internal copy of g_fmt from https://netlib.org/fp/
 * which converts a double value to
 * "the shortest decimal string that correctly rounds to x"
 */
char *json_c_g_fmt(char *buf, double x);

#ifdef __cplusplus
}
#endif

#endif
