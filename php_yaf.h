/*
  +----------------------------------------------------------------------+
  | Yet Another Framework                                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Xinchen Hui  <laruence@php.net>                              |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_YAF_H
#define PHP_YAF_H

extern zend_module_entry yaf_module_entry;
#define phpext_yaf_ptr &yaf_module_entry

#ifdef PHP_WIN32
#define PHP_YAF_API __declspec(dllexport)
#ifndef _MSC_VER
#define _MSC_VER 1600
#endif
#else
#define PHP_YAF_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef ZTS
#define YAF_G(v) TSRMG(yaf_globals_id, zend_yaf_globals *, v)
#else
#define YAF_G(v) (yaf_globals.v)
#endif

#ifndef ZEND_ACC_CTOR
# define ZEND_ACC_CTOR	0x0
# define ZEND_ACC_DTOR	0x0
#endif

#define PHP_YAF_VERSION 					"3.1.3-dev"

#define YAF_STARTUP_FUNCTION(module)   	ZEND_MINIT_FUNCTION(yaf_##module)
#define YAF_RINIT_FUNCTION(module)		ZEND_RINIT_FUNCTION(yaf_##module)
#define YAF_STARTUP(module)	 		  	ZEND_MODULE_STARTUP_N(yaf_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define YAF_SHUTDOWN_FUNCTION(module)  	ZEND_MSHUTDOWN_FUNCTION(yaf_##module)
#define YAF_SHUTDOWN(module)	 	    ZEND_MODULE_SHUTDOWN_N(yaf_##module)(INIT_FUNC_ARGS_PASSTHRU)

#define yaf_application_t	zval
#define yaf_view_t 			zval
#define yaf_controller_t	zval
#define yaf_request_t		zval
#define yaf_router_t		zval
#define yaf_route_t			zval
#define yaf_dispatcher_t	zval
#define yaf_action_t		zval
#define yaf_loader_t		zval
#define yaf_response_t		zval
#define yaf_config_t		zval
#define yaf_registry_t		zval
#define yaf_plugin_t		zval
#define yaf_session_t		zval
#define yaf_exception_t		zval

#define YAF_ME(c, m, a, f) {m, PHP_MN(c), a, (unsigned) (sizeof(a)/sizeof(struct _zend_arg_info)-1), f},

#define YAF_KNOWN_STRINGS(_) \
	_(YAF_STR_CONFIG,           "config") \
	_(YAF_STR_DISPATCHER,       "dispatcher") \
	_(YAF_STR__RUNNING,         "_running") \
	_(YAF_STR__APP,             "_app") \
	_(YAF_STR__ENVIRON,         "_environ") \
	_(YAF_STR__MODULES,         "_modules") \
	_(YAF_STR__ERR_NO,          "_err_no") \
	_(YAF_STR__ERR_MSG,         "_err_msg") \
	_(YAF_STR__HEADER,          "_header") \
	_(YAF_STR__BODY,            "_body") \
	_(YAF_STR__SENDHEADER,      "_sendheader") \
	_(YAF_STR_CONTENT,          "content") \
	_(YAF_STR_DEFAULT_BODY,     "DEFAULT_BODY") \
	_(YAF_STR__RESPONSE_CODE,   "_response_code") \
	_(YAF_STR_APPLICATION,      "application") \
	_(YAF_STR_YAF,              "yaf") \
	_(YAF_STR_DIRECTORY,        "directory") \
	_(YAF_STR_EXT,              "ext") \
	_(YAF_STR_BOOTSTRAP,        "bootstrap") \
	_(YAF_STR_LIBRARY,          "library") \
	_(YAF_STR_NAMESPACE,        "namespace") \
	_(YAF_STR_VIEW,             "view") \
	_(YAF_STR_BASEuRI,          "baseUri") \
	_(YAF_STR_INDEX,            "index") \
	_(YAF_STR_iNDEX,            "Index") \
	_(YAF_STR_PHP,              "php") \
	_(YAF_STR__DEFAULT,         "_default") \
	_(YAF_STR__CURRENT,         "_current") \
	_(YAF_STR__ROUTES,          "_routes") \
	_(YAF_STR__REQUEST,         "_request") \
	_(YAF_STR__VIEW,            "_view") \
	_(YAF_STR__ROUTER,          "_router") \
	_(YAF_STR__INSTANCE,        "_instance") \
	_(YAF_STR__AUTO_RENDER,     "_auto_render") \
	_(YAF_STR__RETURN_RESPONSE, "_return_response") \
	_(YAF_STR__INSTANTLY_FLUSH, "_instantly_flush") \
	_(YAF_STR__INVOKE_ARGS,     "_invoke_args") \
    _(YAF_STR__PLUGINS,         "_plugins") \
	_(YAF_STR__DMODULE,         "_default_module") \
	_(YAF_STR__DCONTROLLER,     "_default_controller") \
	_(YAF_STR__DACTION,         "_default_action") \
	_(YAF_STR__STARTED,         "_started") \
	_(YAF_STR__SESSION,         "_session") \
	_(YAF_STR__LOCAL_NS,        "_local_ns") \
	_(YAF_STR__LIBRARY,         "_library") \
	_(YAF_STR__GLOBAL_LIBRARY,  "_global_library") \
    _(YAF_STR__ENTRIES,         "_entries") \


typedef enum _yaf_known_string_id {
#define _YAF_STR_ID(id, str) id,
YAF_KNOWN_STRINGS(_YAF_STR_ID)
#undef _YAF_STR_ID
	YAF_STR_LAST_KNOWN
} yaf_known_string_id;

extern char *yaf_known_strings[];
#define YAF_STR(id)	((zend_string*)yaf_known_strings[id])

#if PHP_VERSION_ID < 70300
zend_always_inline static zval* php_yaf_read_property(zend_class_entry *scope, zval *object, zend_string *key, zend_bool slient, zval *rv) {
	zval *property;
#if PHP_VERSION_ID < 70100
	zend_class_entry *old_scope = EG(scope);
	EG(scope) = (scope);
#else
	zend_class_entry *old_scope = EG(fake_scope);
	EG(fake_scope) = (scope);
#endif
	if (object) {
		zval name;
		ZVAL_INTERNED_STR(&name, key);
		property = Z_OBJ_HT_P(object)->read_property(object, &name, BP_VAR_IS, NULL, rv);
	} else {
		property = zend_std_get_static_property(scope, key, slient);
	}
#if PHP_VERSION_ID < 70100
	EG(scope) = old_scope;
#else
	EG(fake_scope) = old_scope;
#endif
	return property;
}

zend_always_inline static void php_yaf_update_property(zend_class_entry *scope, zval *object, zend_string *key, zval *v) {
#if PHP_VERSION_ID < 70100
	zend_class_entry *old_scope = EG(scope);
	EG(scope) = (scope);
#else
	zend_class_entry *old_scope = EG(fake_scope);
	EG(fake_scope) = (scope);
#endif
	if (object) {
		zval name;
		ZVAL_INTERNED_STR(&name, key);
		Z_OBJ_HT_P(object)->write_property(object, &name, v, NULL);
	} else {
		zend_update_static_property(scope, ZSTR_VAL(key), ZSTR_LEN(key), v);
	}
#if PHP_VERSION_ID < 70100
	EG(scope) = old_scope;
#else
	EG(fake_scope) = old_scope;
#endif
}

#define zend_read_property_ex php_yaf_read_property
#define zend_read_static_property_ex(scope, key, slient) php_yaf_read_property(scope, NULL, key, slient, NULL)

#define zend_update_property_ex php_yaf_update_property
#define zend_update_static_property_ex(scope, key, rv) php_yaf_update_property(scope, NULL, key, rv)
#endif

extern PHPAPI void php_var_dump(zval **struc, int level);
extern PHPAPI void php_debug_zval_dump(zval **struc, int level);

ZEND_BEGIN_MODULE_GLOBALS(yaf)
    zend_string   *ext;
    zend_string   *base_uri;
    zend_string   *directory;
    zend_string   *local_library;
    zend_string   *view_directory;
    zend_string   *view_ext;
    zend_string   *default_module;
    zend_string   *default_controller;
    zend_string   *default_action;
    zend_string   *bootstrap;
    zend_array    *local_namespaces;
    zend_array    *configs;
    zend_array    *default_route;
    zend_array    *modules;

    char          *global_library;
    char          *environ_name;
    char          *name_separator;
    size_t         name_separator_len;

    zend_bool      lowcase_path;
    zend_bool      use_spl_autoload;
    zend_bool      throw_exception;
    zend_bool      action_prefer;
    zend_bool      name_suffix;
    zend_bool      autoload_started;
    zend_bool      running;
    zend_bool      in_exception;

    zend_bool      catch_exception;
    zend_bool      suppressing_warning;
    zend_bool      use_namespace;
    uint32_t       forward_limit;

    /*for ini parsing */
    zval           active_ini_file_section;
    zval          *ini_wanted_section;
    unsigned       parsing_flag;
ZEND_END_MODULE_GLOBALS(yaf)

PHP_MINIT_FUNCTION(yaf);
PHP_MSHUTDOWN_FUNCTION(yaf);
PHP_RINIT_FUNCTION(yaf);
PHP_RSHUTDOWN_FUNCTION(yaf);
PHP_MINFO_FUNCTION(yaf);

extern ZEND_DECLARE_MODULE_GLOBALS(yaf);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
