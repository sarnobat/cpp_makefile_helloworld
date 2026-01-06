extern "C" {
#include "php.h"
}

PHP_FUNCTION(hello_world);

ZEND_BEGIN_ARG_INFO_EX(arginfo_hello_world, 0, 0, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry helloworld_functions[] = {
    PHP_FE(hello_world, arginfo_hello_world)
    PHP_FE_END
};

zend_module_entry helloworld_module_entry = {
    STANDARD_MODULE_HEADER,
    "helloworld",
    helloworld_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "0.1.0",
    STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(helloworld)

PHP_FUNCTION(hello_world)
{
    RETURN_STRING("Hello from the C++ PHP extension");
}
