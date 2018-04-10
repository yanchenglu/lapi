#ifndef LAPI_LOAD_H_INCLUDED
#define LAPI_LOAD_H_INCLUDED

#define LAPI_LOAD_APP_PATH "_app_path"
#define LAPI_LOAD_AUTOLOAD "spl_autoload_register"

extern zend_class_entry *lapi_load_ce;

void lapi_autoload(zval *obj);
ZEND_MINIT_FUNCTION(lapi_load);


#endif // LAPI_LOAD_H_INCLUDED
