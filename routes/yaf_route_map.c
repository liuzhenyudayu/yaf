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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "zend_smart_str.h" /* for smart_str */

#include "php_yaf.h"
#include "yaf_namespace.h"
#include "yaf_exception.h"
#include "yaf_request.h"

#include "yaf_router.h"
#include "routes/yaf_route_interface.h"
#include "routes/yaf_route_map.h"

zend_class_entry *yaf_route_map_ce;

/** {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(yaf_route_map_construct_arginfo, 0, 0, 0)
    ZEND_ARG_INFO(0, controller_prefer)
	ZEND_ARG_INFO(0, delimiter)
ZEND_END_ARG_INFO()
/* }}} */

yaf_route_t * yaf_route_map_instance(yaf_route_t *this_ptr, zend_bool controller_prefer, zend_string *delim) /* {{{ */{
	zval rv;
	if (Z_ISUNDEF_P(this_ptr)) {
		object_init_ex(this_ptr, yaf_route_map_ce);
	} 

	if (controller_prefer) {
		ZVAL_TRUE(&rv);
		yaf_write_property(yaf_route_map_ce, this_ptr, YAF_ROUTE_MAP_VAR_NAME_CTL_PREFER, &rv);
	}

	if (delim && ZSTR_LEN(delim)) {
		ZVAL_STR(&rv, delim);
		yaf_write_property(yaf_route_map_ce, this_ptr, YAF_ROUTE_MAP_VAR_NAME_DELIMETER, &rv);
	}

	return this_ptr;
}
/* }}} */

/** {{{ int yaf_route_map_route(yaf_route_t *route, yaf_request_t *request)
*/
int yaf_route_map_route(yaf_route_t *route, yaf_request_t *request) {
	zval *delimer, *uri, *base_uri;
	const char *req_uri, *query_str, *pos;
	size_t req_uri_len, query_str_len;
	smart_str route_result = {0};

	uri = yaf_read_property(yaf_request_ce, request, YAF_REQUEST_PROPERTY_NAME_URI);
	base_uri = yaf_read_property(yaf_request_ce, request, YAF_REQUEST_PROPERTY_NAME_BASE);

	if (Z_STRLEN_P(base_uri)) {
		req_uri = yaf_request_strip_base_uri(Z_STR_P(uri), Z_STR_P(base_uri), &req_uri_len);
	} else {
		req_uri = Z_STRVAL_P(uri);
		req_uri_len = Z_STRLEN_P(uri);
	}

	delimer	= yaf_read_property(yaf_route_map_ce, route, YAF_ROUTE_MAP_VAR_NAME_DELIMETER);

	if (UNEXPECTED(Z_TYPE_P(delimer) == IS_STRING && Z_STRLEN_P(delimer))) {
		if ((query_str = strstr(req_uri, Z_STRVAL_P(delimer))) && *(query_str - 1) == '/') {
			const char *rest = query_str + Z_STRLEN_P(delimer);

			while (*rest == YAF_ROUTER_URL_DELIMIETER) {
				rest++;
			}
			if (*rest != '\0') {
				query_str = rest;
				query_str_len = req_uri_len - (rest - req_uri);
				req_uri_len = req_uri_len - query_str_len;
			} else {
				req_uri_len = query_str - req_uri;
				query_str = NULL;
			}
		} else {
			query_str = NULL;
		}
	} else {
		query_str = NULL;
	}

	while ((pos = memchr(req_uri, YAF_ROUTER_URL_DELIMIETER, req_uri_len))) {
		size_t seg_len = pos++ - req_uri;
		if (seg_len) {
			smart_str_appendl(&route_result, req_uri, seg_len);
			smart_str_appendc(&route_result, '_');
		}
		req_uri_len -= pos - req_uri;
		req_uri = pos;
	}

	if (req_uri_len) {
		smart_str_appendl(&route_result, req_uri, req_uri_len);
		smart_str_appendc(&route_result, '_');
	}

	if (route_result.s) {
		zval rv;
		zval *ctl_prefer = yaf_read_property(yaf_route_map_ce, route, YAF_ROUTE_MAP_VAR_NAME_CTL_PREFER);
		ZSTR_LEN(route_result.s)--;
		ZSTR_VAL(route_result.s)[ZSTR_LEN(route_result.s)] = '\0';

		ZVAL_NEW_STR(&rv, route_result.s);
		if (Z_TYPE_P(ctl_prefer) == IS_TRUE) {
			yaf_write_property(yaf_request_ce, request, YAF_REQUEST_PROPERTY_NAME_CONTROLLER, &rv);
		} else {
			yaf_write_property(yaf_request_ce, request, YAF_REQUEST_PROPERTY_NAME_ACTION, &rv);
		}
		smart_str_free(&route_result);
	}

	if (query_str) {
		zval params;
		yaf_router_parse_parameters(query_str, query_str_len, &params);
		yaf_request_set_params_multi(request, &params);
		zval_ptr_dtor(&params);
	}

	return 1;
}
/* }}} */

/** {{{ proto public Yaf_Route_Simple::route(Yaf_Request $req)
*/
PHP_METHOD(yaf_route_map, route) {
	yaf_request_t *request;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "O", &request, yaf_request_ce) == FAILURE) {
		return;
	} else {
		RETURN_BOOL(yaf_route_map_route(getThis(), request));
	}
}
/* }}} */

/** {{{ zend_string * yaf_route_map_assemble(zval *info, zval *query)
 */
zend_string * yaf_route_map_assemble(yaf_route_t *this_ptr, zval *info, zval *query) {
	char *seg, *ptrptr, *pname;
	smart_str uri = {0};
	size_t seg_len;
	zend_bool has_delim = 0;
	zval *delim, *ctl_prefer, *zv;

	ctl_prefer = yaf_read_property(yaf_route_map_ce, this_ptr, YAF_ROUTE_MAP_VAR_NAME_CTL_PREFER);
	delim = yaf_read_property(yaf_route_map_ce, this_ptr, YAF_ROUTE_MAP_VAR_NAME_DELIMETER);

	if (IS_STRING == Z_TYPE_P(delim) && Z_STRLEN_P(delim)) {
		has_delim = 1;
	}

	do {
		if (Z_TYPE_P(ctl_prefer) == IS_TRUE) {
			if ((zv = zend_hash_str_find(Z_ARRVAL_P(info),
					ZEND_STRL(YAF_ROUTE_ASSEMBLE_ACTION_FORMAT))) != NULL && Z_TYPE_P(zv) == IS_STRING) {
				pname = estrndup(Z_STRVAL_P(zv), Z_STRLEN_P(zv));
			} else {
				yaf_trigger_error(YAF_ERR_TYPE_ERROR, "%s",
						"Undefined the 'action' parameter for the 1st parameter");
				break;
			}
		} else {
			if ((zv = zend_hash_str_find(Z_ARRVAL_P(info),
					ZEND_STRL(YAF_ROUTE_ASSEMBLE_CONTROLLER_FORMAT))) != NULL && Z_TYPE_P(zv) == IS_STRING) {
				pname = estrndup(Z_STRVAL_P(zv), Z_STRLEN_P(zv));
			} else {
				yaf_trigger_error(YAF_ERR_TYPE_ERROR, "%s",
						"Undefined the 'controller' parameter for the 1st parameter");
				break;
			}
		}

		seg = php_strtok_r(pname, "_", &ptrptr);	
		while (seg) {
			seg_len = strlen(seg);
			if (seg_len) {
				smart_str_appendc(&uri, '/');
				smart_str_appendl(&uri, seg, seg_len);
			}
			seg = php_strtok_r(NULL, "_", &ptrptr);
		}
		efree(pname);

		if (query && IS_ARRAY == Z_TYPE_P(query)) {
			zend_bool start = 1;
			zend_string *key, *val;

			if (has_delim) {
				smart_str_appendc(&uri, '/');
				smart_str_appendl(&uri, Z_STRVAL_P(delim), Z_STRLEN_P(delim));
			}

            ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(query), key, zv) {
				if (key) {
					val = zval_get_string(zv);
					if (has_delim) {
						smart_str_appendc(&uri, '/');
						smart_str_appendl(&uri, ZSTR_VAL(key), ZSTR_LEN(key));
						smart_str_appendc(&uri, '/');
						smart_str_appendl(&uri, ZSTR_VAL(val), ZSTR_LEN(val));
					} else {
						if (start) {
							smart_str_appendc(&uri, '?');
							smart_str_appendl(&uri, ZSTR_VAL(key), ZSTR_LEN(key));
							smart_str_appendc(&uri, '=');
							smart_str_appendl(&uri, ZSTR_VAL(val), ZSTR_LEN(val));
							start = 0;
						} else {
							smart_str_appendc(&uri, '&');
							smart_str_appendl(&uri, ZSTR_VAL(key), ZSTR_LEN(key));
							smart_str_appendc(&uri, '=');
							smart_str_appendl(&uri, ZSTR_VAL(val), ZSTR_LEN(val));
						}
					}
					zend_string_release(val);
				}
			} ZEND_HASH_FOREACH_END();
		}

		smart_str_0(&uri);
		return uri.s;
	} while (0);

	return NULL;
}
/* }}} */

/** {{{ proto public Yaf_Route_Simple::__construct(bool $controller_prefer=FALSE, string $delimer = '#!')
*/
PHP_METHOD(yaf_route_map, __construct) {
	zend_string *delim	= NULL;
	zend_bool controller_prefer = 0;
	zval *self = getThis();

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|bS", &controller_prefer, &delim) == FAILURE) {
		return;
	}

	(void)yaf_route_map_instance(self, controller_prefer, delim);
}
/* }}} */

/** {{{ proto public Yaf_Route_Map::assemble(array $info[, array $query = NULL])
*/
PHP_METHOD(yaf_route_map, assemble) {
	zval *info, *query = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "a|a", &info, &query) == FAILURE) {
        return;
    } else {
		zend_string *str;
        if ((str = yaf_route_map_assemble(getThis(), info, query)) != NULL) {
			RETURN_STR(str);
		}
		RETURN_NULL();
    }
}
/* }}} */

/** {{{ yaf_route_map_methods
*/
zend_function_entry yaf_route_map_methods[] = {
	PHP_ME(yaf_route_map, __construct, yaf_route_map_construct_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(yaf_route_map, route, yaf_route_route_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(yaf_route_map, assemble, yaf_route_assemble_arginfo, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ YAF_STARTUP_FUNCTION
*/
YAF_STARTUP_FUNCTION(route_map) {
	zval rv;
	zend_class_entry ce;

	YAF_INIT_CLASS_ENTRY(ce, "Yaf_Route_Map", "Yaf\\Route\\Map", yaf_route_map_methods);
	yaf_route_map_ce = zend_register_internal_class(&ce);
	zend_class_implements(yaf_route_map_ce, 1, yaf_route_ce);

	yaf_route_map_ce->ce_flags |= ZEND_ACC_FINAL;

	ZVAL_FALSE(&rv);
	yaf_declare_property(yaf_route_map_ce, YAF_ROUTE_MAP_VAR_NAME_CTL_PREFER, &rv, ZEND_ACC_PROTECTED);
	ZVAL_NULL(&rv);
	yaf_declare_property(yaf_route_map_ce, YAF_ROUTE_MAP_VAR_NAME_DELIMETER, &rv, ZEND_ACC_PROTECTED);

	return SUCCESS;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
