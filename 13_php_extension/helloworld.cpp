extern "C" {
#include "php.h"
}

// Prototypes
PHP_FUNCTION(hello_world);

ZEND_BEGIN_ARG_INFO_EX(arginfo_hello_world, 0, 0, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry helloworld_functions[] = {
    PHP_FE(hello_world, arginfo_hello_world)
    PHP_FE_END
};

zend_module_entry helloworld_module_entry = {
    STANDARD_MODULE_HEADER,      // Module API header/version metadata.
    "helloworld",                // Extension name for phpinfo() and loading.
    helloworld_functions,        // Function table exposed to PHP.
    NULL,                        // MINIT: module initialization callback.
    NULL,                        // MSHUTDOWN: module shutdown callback.
    NULL,                        // RINIT: per-request initialization callback.
    NULL,                        // RSHUTDOWN: per-request shutdown callback.
    NULL,                        // MINFO: phpinfo() table callback.
    "0.1.0",                     // Extension version string.
    STANDARD_MODULE_PROPERTIES   // Default module properties macro.
};

ZEND_GET_MODULE(helloworld)

// called as hello_world() in index.php
PHP_FUNCTION(hello_world)
{
    RETURN_STRING("SRIDHAR: Hello from the C++ PHP extension");
}
