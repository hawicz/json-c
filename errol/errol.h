#ifndef ERROL_H
#define ERROL_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * common headers
 */

#include <stdint.h>

/*
 * json_c_errol declarations
 */

#define ERR_LEN   512
#define ERR_DEPTH 4

int json_c_errol0_dtoa(double val, char *buf);
int json_c_errol1_dtoa(double val, char *buf, bool *opt);
int json_c_errol2_dtoa(double val, char *buf, bool *opt);
int json_c_errol3_dtoa(double val, char *buf);
int json_c_errol3u_dtoa(double val, char *buf);
int json_c_errol4_dtoa(double val, char *buf);
int json_c_errol4u_dtoa(double val, char *buf);

int json_c_errol_int(double val, char *buf);
int json_c_errol_fixed(double val, char *buf);

struct json_c_errol_err_t {
	double val;
	char str[18];
	int exp;
};

struct json_c_errol_slab_t {
	char str[18];
	int exp;
};

typedef union {
	double d;
	uint64_t i;
} json_c_errol_bits_t;

#ifdef __cplusplus
}
#endif

#endif
