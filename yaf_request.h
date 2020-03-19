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

#ifndef YAF_REQUEST_H
#define YAF_REQUEST_H

#define YAF_REQUEST_PROPERTY_NAME_MODULE      YAF_STR(YAF_STR_MODULE)
#define YAF_REQUEST_PROPERTY_NAME_CONTROLLER  YAF_STR(YAF_STR_CONTROLLER)
#define YAF_REQUEST_PROPERTY_NAME_ACTION      YAF_STR(YAF_STR_ACTION)
#define YAF_REQUEST_PROPERTY_NAME_METHOD      YAF_STR(YAF_STR_METHOD)
#define YAF_REQUEST_PROPERTY_NAME_PARAMS      YAF_STR(YAF_STR_PARAMS)
#define YAF_REQUEST_PROPERTY_NAME_URI         YAF_STR(YAF_STR_URI)
#define YAF_REQUEST_PROPERTY_NAME_STATE       YAF_STR(YAF_STR_DISPATCHED)
#define YAF_REQUEST_PROPERTY_NAME_LANG        YAF_STR(YAF_STR_LANGUAGE)
#define YAF_REQUEST_PROPERTY_NAME_ROUTED      YAF_STR(YAF_STR_ROUTED)
#define YAF_REQUEST_PROPERTY_NAME_BASE        YAF_STR(YAF_STR__BASE_URI)
#define YAF_REQUEST_PROPERTY_NAME_EXCEPTION   YAF_STR(YAF_STR__EXCEPTION)

#define YAF_REQUEST_SERVER_URI               "request_uri="

#define YAF_GLOBAL_VARS_TYPE                 unsigned int
#define YAF_GLOBAL_VARS_POST                 TRACK_VARS_POST
#define YAF_GLOBAL_VARS_GET                  TRACK_VARS_GET
#define YAF_GLOBAL_VARS_ENV                  TRACK_VARS_ENV
#define YAF_GLOBAL_VARS_FILES                TRACK_VARS_FILES
#define YAF_GLOBAL_VARS_SERVER               TRACK_VARS_SERVER
#define YAF_GLOBAL_VARS_REQUEST              TRACK_VARS_REQUEST
#define YAF_GLOBAL_VARS_COOKIE               TRACK_VARS_COOKIE

extern zend_class_entry *yaf_request_ce;

extern PHPAPI void php_session_start();

yaf_request_t *yaf_request_instance(yaf_request_t *this_ptr, zend_string *info);
int yaf_request_set_base_uri(yaf_request_t *request, zend_string *base_uri, zend_string *request_uri);

zval *yaf_request_query(unsigned type, zend_string *name);
zval *yaf_request_query_str(unsigned type, const char *name, size_t len);

zval *yaf_request_get_method(yaf_request_t *instance);
zval *yaf_request_get_param(yaf_request_t *instance, zend_string *key);
zval *yaf_request_get_language(yaf_request_t *instance, zval *accept_language);

int yaf_request_is_routed(yaf_request_t *request);
int yaf_request_is_dispatched(yaf_request_t *request);
void yaf_request_set_dispatched(yaf_request_t *request, int flag);
void yaf_request_set_routed(yaf_request_t *request, int flag);
int yaf_request_set_params_single(yaf_request_t *instance, zend_string *key, zval *value);
int yaf_request_set_params_multi(yaf_request_t *instance, zval *values);
const char *yaf_request_strip_base_uri(zend_string *uri, zend_string *base_uri, size_t *len);
const char *yaf_request_get_request_method(void);

#define YAF_REQUEST_IS_METHOD(x) \
PHP_METHOD(yaf_request, is##x) {\
	zval *method = zend_read_property_ex(Z_OBJCE_P(getThis()), \
					getThis(), YAF_REQUEST_PROPERTY_NAME_METHOD, 1, NULL); \
	if (zend_string_equals_literal_ci(Z_STR_P(method), #x)) { \
		RETURN_TRUE; \
	} \
	RETURN_FALSE; \
}

#define YAF_REQUEST_METHOD(ce, x, type) \
PHP_METHOD(ce, get##x) { \
	zend_string *name; \
	zval *ret; \
	zval *def = NULL; \
	if (ZEND_NUM_ARGS() == 0) { \
		ret = yaf_request_query(type, NULL); \
	}else if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|z", &name, &def) == FAILURE) { \
		return; \
	} else { \
		ret = yaf_request_query(type, name); \
		if (!ret) { \
			if (def != NULL) { \
				RETURN_ZVAL(def, 1, 0); \
			} \
		} \
	} \
	if (ret) { \
	    RETURN_ZVAL(ret, 1, 0); \
	} else { \
		RETURN_NULL(); \
	} \
}


YAF_STARTUP_FUNCTION(request);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
