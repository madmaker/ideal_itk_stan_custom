#ifndef CHECK_USER_GROUP_HXX_
#define CHECK_USER_GROUP_HXX_

#include <tc/tc.h>
#include <epm.h>

/**
 * Handler проверяет, принадлежат ли таргеты и пользователь к одной группе из подгрупп ООО СТАН
 */

int get_object_factory_group(tag_t object, tag_t root_group, tag_t* user_factory_group);
EPM_decision_t check_user_group(EPM_rule_message_t);

#endif
