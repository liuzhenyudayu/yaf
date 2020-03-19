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

#ifndef YAF_SESSION_H
#define YAF_SESSION_H

#define YAF_SESSION_PROPERTY_NAME_STATUS    YAF_STR(YAF_STR__STARTED)
#define YAF_SESSION_PROPERTY_NAME_SESSION   YAF_STR(YAF_STR__SESSION)
#define YAF_SESSION_PROPERTY_NAME_INSTANCE  YAF_STR(YAF_STR__INSTANCE)

extern zend_class_entry *yaf_session_ce;

PHPAPI void php_session_start();
YAF_STARTUP_FUNCTION(session);
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
