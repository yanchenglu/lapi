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
#include "lapi_application.h"
#include "lapi_load.h"
#include "zend_smart_str.h"
#include "zend_smart_str_public.h"
#include "ext/json/php_json.h"
#include "main/SAPI.h"

zend_class_entry *lapi_application_ce;

PHP_METHOD(lapi_application, __construct)
{
    char *app_path;
    long app_path_len;

    zval *app = zend_read_static_property(lapi_application_ce, ZEND_STRL(LAPI_APPLICATION_APP), 1);

    if (app && !ZVAL_IS_NULL(app))
    {
        php_printf("error");
        return;
    }

    zend_update_static_property(lapi_application_ce, ZEND_STRL(LAPI_APPLICATION_APP), getThis());

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &app_path, &app_path_len) == FAILURE)
    {
        return;
    }

    int ret = zend_update_static_property_stringl(lapi_application_ce, ZEND_STRL(LAPI_APPLICATION_APP_PATH), app_path, app_path_len);
    zend_update_static_property_stringl(lapi_load_ce, ZEND_STRL(LAPI_LOAD_APP_PATH), app_path, app_path_len);

    if (ret == FAILURE)
    {
        return;
    }

    zval route;
    array_init(&route);

    add_assoc_string(&route, "controller", "Test");
    add_assoc_string(&route, "action", "testAction");

    zend_update_static_property(lapi_application_ce, ZEND_STRL(LAPI_APPLICATION_APP_ROUTE), &route);

    zval_ptr_dtor(&route);

    zval load;
    object_init_ex(&load, lapi_load_ce);

    lapi_autoload(&load);

    zval_ptr_dtor(&load);
}

PHP_METHOD(lapi_application, run)
{
    zval *route = zend_read_static_property(lapi_application_ce, ZEND_STRL(LAPI_APPLICATION_APP_ROUTE), 1);

    zval *controller = zend_hash_str_find(Z_ARRVAL_P(route), ZEND_STRL("controller"));
    zval *action = zend_hash_str_find(Z_ARRVAL_P(route), ZEND_STRL("action"));

    smart_str pstr = {0};

    zend_string *delimiter = zend_string_init(ZEND_STRL("\\"), 0);
    zend_string *controller_str = zend_string_init(ZEND_STRL("controller"), 0);

    smart_str_append(&pstr, delimiter);
    smart_str_append(&pstr, controller_str);
    smart_str_append(&pstr, delimiter);
    smart_str_append(&pstr, Z_STR_P(controller));

    zval controller_obj;
    object_init_ex(&controller_obj, zend_fetch_class(pstr.s, ZEND_FETCH_CLASS_AUTO));

    zval result;
    call_user_function(NULL, &controller_obj, action, &result, 0, NULL);

    smart_str ppstr = {0};
    php_json_encode(&ppstr, &result, 0);

    php_printf(ZSTR_VAL(ppstr.s));

    zval_ptr_dtor(&controller_obj);
    zval_ptr_dtor(&result);
    zend_string_release(delimiter);
    zend_string_release(controller_str);
    zend_string_release(pstr.s);


    zend_string_release(ppstr.s);
}

const zend_function_entry lapi_application_function_entrys[] = {
PHP_ME(lapi_application, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
PHP_ME(lapi_application, run, NULL, ZEND_ACC_PUBLIC)
PHP_FE_END
};

ZEND_MINIT_FUNCTION(lapi_application)
{
    zend_class_entry lapi_application_entry;
    INIT_CLASS_ENTRY(lapi_application_entry, "Lapi\\Application", lapi_application_function_entrys);
    lapi_application_ce = zend_register_internal_class(&lapi_application_entry);

    zend_declare_property_null(lapi_application_ce, ZEND_STRL(LAPI_APPLICATION_APP), ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);
    zend_declare_property_null(lapi_application_ce, ZEND_STRL(LAPI_APPLICATION_APP_PATH), ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);
    zend_declare_property_null(lapi_application_ce, ZEND_STRL(LAPI_APPLICATION_APP_ROUTE), ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);
}
