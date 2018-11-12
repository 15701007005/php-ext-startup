#include "test.class.h"

zend_class_entry *test_ce;

PHP_METHOD(Test, helloworld)
{
	if  (zend_parse_parameters_none() ==  FAILURE ) {
        return;
    }

	php_printf("hello world!");
	RETVAL_NULL();
}

PHP_METHOD(Test, echoHelloWorld)
{
	if  (zend_parse_parameters_none() ==  FAILURE ) {
        return;
    }

	php_printf("hello world!!");

	RETVAL_NULL();
}

PHP_METHOD(Test, __call)
{
    zval *params;
    char *method;
	size_t method_len;
	zend_string *strg;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "sz", &method, &method_len, &params) == FAILURE)
    {
        return;
    }

	zend_string *delim = zend_string_init("-", sizeof("-"), 0);

	php_implode(delim, params, return_value);

	smart_str str = {0};
	smart_str_apppend();

	zend_string_release(delim);

	zval *tmp = return_value;

	int len = Z_STRLEN_P(tmp);
	char *xy = estrndup(Z_STRVAL_P(tmp), len);

	// if (tmp != return_value) {
	// 	zval_ptr_dtor(&tmp);
	// }

	PHPWRITE(xy, sizeof(xy));

	strg = strpprintf(0, "method:%s,args:-=-=-=- @@ %s @@\n", method, xy);

	// php_var_dump(return_value, 1);
	// PHPWRITE(Z_STRVAL_P(return_value), Z_STRLEN_P(return_value));


	RETURN_STR(strg);
	// RETURN_STRING(msg);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_call, 0, 0, 2)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

static zend_function_entry methods[] = {
	PHP_ME(Test, helloworld, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test, echoHelloWorld, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test, __call, arginfo_call, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

void php_start_register_test()
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, PHP_STARTUP_NS(Test), methods);

	test_ce = zend_register_internal_class(&ce);

	zend_declare_property_string(test_ce, "publicProperty", strlen("publicProperty"), "hello world +property", ZEND_ACC_PUBLIC);

	zend_declare_property_string(test_ce, "publicPropertyStatic", strlen("publicPropertyStatic"), "hello world +property +static", ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);

	zend_declare_class_constant_string(test_ce, "PUBLIC_CONST", strlen("PUBLIC_CONST"), "hello world +const");
}
