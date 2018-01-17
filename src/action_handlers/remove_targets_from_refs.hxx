#ifndef REMOVE_TARGETS_FROM_REFS_HXX_
#define REMOVE_TARGETS_FROM_REFS_HXX_

#include <tc/tc.h>
#include <epm.h>

/**
 * Handler удал€ет из references те объекты, которые €вл€ютс€ solution items извещени€ об изменении из targets
 */

int is_related_to_target_object(char*, char*);
int remove_targets_from_refs(EPM_action_message_t);

#endif
