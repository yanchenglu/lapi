/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: yanchenglu                                                      |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "lapi_load.h"
#include "ext/standard/php_string.h"
#include "zend_smart_str.h"
#include "zend_smart_str_public.h"

zend_class_entry *lapi_load_ce;

void lapi_autoload(zval *obj)
{
    zval callback;
    zval result;
    zval params[1] = {0};
    zval function_name;

    ZVAL_STRING(&function_name, LAPI_LOAD_AUTOLOAD);
    array_init(&callback);

    add_next_index_string(&callback, "LAPI\\Load");
    add_next_index_string(&callback, "import");

    params[0] = callback;

    call_user_function(NULL, NULL, &function_name, &result, 1, params);

    zval_ptr_dtor(&function_name);
    zval_ptr_dtor(&callback);
}


PHP_METHOD(lapi_load, __construct)
{
    zval *self = getThis();

    lapi_autoload(self);
}

PHP_METHOD(lapi_load, import)
{
    zend_string *classname;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &classname) == FAILURE)
    {
        return;
    }

    zval *app_path = zend_read_static_property(lapi_load_ce, ZEND_STRL(LAPI_LOAD_APP_PATH), 1);

    smart_str pstr = {0};
    smart_str_append(&pstr, Z_STR_P(app_path));

    zval result;
    array_init(&result);

    zend_string *delimiter = zend_string_init(ZEND_STRL("\\"), 0);
    zend_string *path_delimiter = zend_string_init(ZEND_STRL("/"), 0);
    zend_string *postfix = zend_string_init(ZEND_STRL(".php"), 0);

    php_explode(delimiter, classname, &result, 0);

    zval *foreach_val;

    ZEND_HASH_FOREACH_VAL(Z_ARRVAL(result), foreach_val) {
        smart_str_append(&pstr, path_delimiter);
        smart_str_append(&pstr, Z_STR_P(foreach_val));
    } ZEND_HASH_FOREACH_END();

    smart_str_append(&pstr, postfix);
    smart_str_0(&pstr);

    zval filename;
    ZVAL_STR(&filename, pstr.s);

    zend_op_array *op_array = compile_filename(ZEND_REQUIRE_ONCE, &filename);

    zend_string_release(pstr.s);
    zend_string_release(delimiter);
    zend_string_release(path_delimiter);
    zend_string_release(postfix);

    zval_ptr_dtor(&filename);
    zval_ptr_dtor(&result);

    if (op_array)
    {
        zval ret_value;
        zend_execute(op_array, &ret_value);
        zval_ptr_dtor(&ret_value);

        destroy_op_array(op_array);
        efree(op_array);
    }
}

const zend_function_entry load_function_entrys[] = {
PHP_ME(lapi_load, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
PHP_ME(lapi_load, import, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
PHP_FE_END
};

ZEND_MINIT_FUNCTION(lapi_load)
{
    zend_class_entry lapi_load_entry;
    INIT_CLASS_ENTRY(lapi_load_entry, "LAPI\\Load", load_function_entrys);

    lapi_load_ce = zend_register_internal_class(&lapi_load_entry);

    zend_declare_property_null(lapi_load_ce, ZEND_STRL(LAPI_LOAD_APP_PATH), ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);
}
