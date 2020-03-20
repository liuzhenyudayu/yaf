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

#ifndef YAF_CONTROLLER_H
#define YAF_CONTROLLER_H

#define YAF_CONTROLLER_PROPERTY_NAME_MODULE		YAF_STR(YAF_STR__MODULE)
#define YAF_CONTROLLER_PROPERTY_NAME_NAME		YAF_STR(YAF_STR__NAME)
#define YAF_CONTROLLER_PROPERTY_NAME_SCRIPT		YAF_STR(YAF_STR__SCRIPT_PATH)
#define YAF_CONTROLLER_PROPERTY_NAME_RESPONSE	YAF_STR(YAF_STR__RESPONSE)
#define YAF_CONTROLLER_PROPERTY_NAME_REQUEST	YAF_STR(YAF_STR__REQUEST)
#define YAF_CONTROLLER_PROPERTY_NAME_ARGS		YAF_STR(YAF_STR__INVOKE_ARGS)
#define YAF_CONTROLLER_PROPERTY_NAME_ACTIONS	YAF_STR(YAF_STR_ACTIONS)
#define YAF_CONTROLLER_PROPERTY_NAME_VIEW		YAF_STR(YAF_STR__VIEW)
#define YAF_CONTROLLER_PROPERTY_NAME_RENDER     YAF_STR(YAF_STR_AUTORENDER)

extern zend_class_entry *yaf_controller_ce;
int yaf_controller_construct(zend_class_entry *ce, yaf_controller_t *self,
		yaf_request_t *request, yaf_response_t *response, yaf_view_t *view, zval *args);
zend_string * yaf_controller_render(zval *instance, char *action_name, int len, zval *var_array);
int yaf_controller_display(zval *instance, char *action_name, int len, zval *var_array);
YAF_STARTUP_FUNCTION(controller);
#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

