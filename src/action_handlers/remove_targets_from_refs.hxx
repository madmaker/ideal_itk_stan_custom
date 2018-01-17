#ifndef REMOVE_TARGETS_FROM_REFS_HXX_
#define REMOVE_TARGETS_FROM_REFS_HXX_

#include <tc/tc.h>
#include <epm.h>

/**
 * Handler ������� �� references �� �������, ������� �������� solution items ��������� �� ��������� �� targets
 */

int is_related_to_target_object(char*, char*);
int remove_targets_from_refs(EPM_action_message_t);

#endif
