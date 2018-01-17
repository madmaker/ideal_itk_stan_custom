#ifndef REMOVE_REFERENCES_WITH_STATUS_HXX_
#define REMOVE_REFERENCES_WITH_STATUS_HXX_

#include <tc/tc.h>
#include <epm.h>

/**
 * Handler удаляет из references те объекты, у которых есть указанный статус
 * Статус задается как аргумент для хэндлера:
 * -status | TCM Released
 */

int has_release_status(tag_t,char*,bool*);
int remove_references_with_status(EPM_action_message_t);

#endif
