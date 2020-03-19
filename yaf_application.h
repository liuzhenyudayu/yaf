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

#ifndef PHP_YAF_APPLICATION_H
#define PHP_YAF_APPLICATION_H

#define YAF_APPLICATION_PROPERTY_NAME_CONFIG		YAF_STR(YAF_STR_CONFIG)
#define YAF_APPLICATION_PROPERTY_NAME_DISPATCHER	YAF_STR(YAF_STR_DISPATCHER)
#define YAF_APPLICATION_PROPERTY_NAME_RUN			YAF_STR(YAF_STR__RUNNING)
#define YAF_APPLICATION_PROPERTY_NAME_APP			YAF_STR(YAF_STR__APP)
#define YAF_APPLICATION_PROPERTY_NAME_ENV			YAF_STR(YAF_STR__ENVIRON)
#define YAF_APPLICATION_PROPERTY_NAME_MODULES		YAF_STR(YAF_STR__MODULES)
#define YAF_APPLICATION_PROPERTY_NAME_ERRNO			YAF_STR(YAF_STR__ERR_NO)
#define YAF_APPLICATION_PROPERTY_NAME_ERRMSG		YAF_STR(YAF_STR__ERR_MSG)

extern zend_class_entry *yaf_application_ce;

int yaf_application_is_module_name(zend_string *name);
int yaf_application_is_module_name_str(const char *name, size_t len);

YAF_STARTUP_FUNCTION(application);
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
