/*
* Tests if the format string for double serialization is handled correctly
*/

#ifdef NDEBUG
#undef NDEBUG
#endif
#include "config.h"
#include <stdio.h>

#include "json_object.h"
#include "json_object_private.h"

/* Avoid compiler warnings about diving by constant zero */
double zero_dot_zero = 0.0;

int main(int argc, char **argv)
{
	struct json_object *obj = json_object_new_double(0.5);
	char udata[] = "test";

	printf("Test default serializer:\n");
	printf("obj.to_string(standard)=%s\n", json_object_to_json_string(obj));

	printf("Test default serializer with custom userdata:\n");
	obj->_userdata = udata;
	printf("obj.to_string(userdata)=%s\n", json_object_to_json_string(obj));

	printf("Test explicit serializer with custom userdata:\n");
	json_object_set_serializer(obj, json_object_double_to_json_string, udata, NULL);
	printf("obj.to_string(custom)=%s\n", json_object_to_json_string(obj));

	printf("Test reset serializer to default:\n");
	json_object_set_serializer(obj, NULL, NULL, NULL);
	printf("obj.to_string(reset)=%s\n", json_object_to_json_string(obj));
	json_object_put(obj);

	printf("Test JSON_C_TO_STRING_NOZERO:\n");
	obj = json_object_new_double(1.5);
	// Set an explicit serializer, to ensure printf is used
	char custom_format[] = "%.17f"; // Use %f b/c it pads on the right with zeros
	json_object_set_serializer(obj, json_object_double_to_json_string, custom_format, NULL);

	printf("obj.to_string(%s, 0 flags)=%s\n", custom_format, json_object_to_json_string_ext(obj, 0));
	printf("obj.to_string(%s, nozero)=%s\n", custom_format, json_object_to_json_string_ext(obj, JSON_C_TO_STRING_NOZERO));

	json_object_set_serializer(obj, NULL, NULL, NULL);
	printf("obj.to_string(default serializer, 0 flags)=%s\n", json_object_to_json_string_ext(obj, 0));
	printf("obj.to_string(default serializer, nozero)=%s\n", json_object_to_json_string_ext(obj, JSON_C_TO_STRING_NOZERO));

	json_object_put(obj);

	obj = json_object_new_double(1.1);
	json_object_set_serializer(obj, NULL, NULL, NULL);
	// Check that 1.1 serializes to "1.1" instead of "1.1000000000000001"
	printf("obj.to_string(1.1)=%s\n", json_object_to_json_string_ext(obj, 0));

	json_object_set_double(obj, 0);
	printf("obj.to_string(0.0)=%s\n", json_object_to_json_string_ext(obj, 0));

	json_object_set_double(obj, 100);
	printf("obj.to_string(100.0)=%s\n", json_object_to_json_string_ext(obj, 0));

	json_object_set_double(obj, 0.5123);
	printf("obj.to_string(0.5123)=%s\n", json_object_to_json_string_ext(obj, 0));

	json_object_set_double(obj, 1000000);
	printf("obj.to_string(1000000)=%s\n", json_object_to_json_string_ext(obj, 0));

	json_object_set_double(obj, -1);
	printf("obj.to_string(-1)=%s\n", json_object_to_json_string_ext(obj, 0));

	json_object_set_double(obj, -1000);
	printf("obj.to_string(-1000)=%s\n", json_object_to_json_string_ext(obj, 0));

	json_object_set_double(obj, -1000.1);
	printf("obj.to_string(-1000.1)=%s\n", json_object_to_json_string_ext(obj, 0));

	json_object_put(obj);

	obj = json_object_new_double(1.1);
	if (json_c_set_serialization_double_format("x%0.3fy", JSON_C_OPTION_GLOBAL) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj.to_string(with global format)=%s\n", json_object_to_json_string(obj));
#ifdef HAVE___THREAD
	if (json_c_set_serialization_double_format("T%0.2fX", JSON_C_OPTION_THREAD) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj.to_string(with thread format)=%s\n", json_object_to_json_string(obj));
	if (json_c_set_serialization_double_format("Ttttttttttttt%0.2fxxxxxxxxxxxxxxxxxxX",
	                                           JSON_C_OPTION_THREAD) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj.to_string(long thread format)=%s\n", json_object_to_json_string(obj));
	if (json_c_set_serialization_double_format(NULL, JSON_C_OPTION_THREAD) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj.to_string(back to global format)=%s\n", json_object_to_json_string(obj));
#else
	// Just fake it up, so the output matches.
	printf("obj.to_string(with thread format)=%s\n", "T0.52X");
	printf("obj.to_string(long thread format)=%s\n", "Ttttttttttttt0.52xxxxxxxxxxxxxxxxxxX");
	printf("obj.to_string(back to global format)=%s\n", "x0.524y");
#endif
	if (json_c_set_serialization_double_format(NULL, JSON_C_OPTION_GLOBAL) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj.to_string(back to default format)=%s\n", json_object_to_json_string(obj));

	json_object_put(obj);

	obj = json_object_new_double(12.0);
	printf("obj(12.0).to_string(default format)=%s\n", json_object_to_json_string(obj));
	if (json_c_set_serialization_double_format("%.0f", JSON_C_OPTION_GLOBAL) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj(12.0).to_string(%%.0f)=%s\n", json_object_to_json_string(obj));

	if (json_c_set_serialization_double_format("%.0g", JSON_C_OPTION_GLOBAL) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj(12.0).to_string(%%.0g)=%s\n", json_object_to_json_string(obj));

	if (json_c_set_serialization_double_format("%.2g", JSON_C_OPTION_GLOBAL) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj(12.0).to_string(%%.1g)=%s\n", json_object_to_json_string(obj));

	// Reset to default to free memory
	if (json_c_set_serialization_double_format(NULL, JSON_C_OPTION_GLOBAL) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");

	json_object_put(obj);

	obj = json_object_new_double(-12.0);
	printf("obj(-12.0).to_string(default format)=%s\n", json_object_to_json_string(obj));
	json_object_put(obj);

	/* Test NaN handling */
	obj = json_object_new_double(zero_dot_zero / zero_dot_zero);
	printf("obj(0.0/0.0)=%s\n", json_object_to_json_string(obj));
	json_object_put(obj);

	/* Test Infinity and -Infinity handling */
	obj = json_object_new_double(1.0 / zero_dot_zero);
	printf("obj(1.0/0.0)=%s\n", json_object_to_json_string(obj));
	json_object_put(obj);

	obj = json_object_new_double(-1.0 / zero_dot_zero);
	printf("obj(-1.0/0.0)=%s\n", json_object_to_json_string(obj));
	json_object_put(obj);

	return 0;
}
