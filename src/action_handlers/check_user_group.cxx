#include <stdlib.h>
#include <item.h>
#include <aom.h>
#include <grm.h>
#include <epm.h>
#include <sa/sa.h>
#include <epm_toolkit_tc_utils.h>
#include <qry_errors.h>
#include <base_utils/ResultCheck.hxx>
#include "../misc.hxx"
#include "check_user_group.hxx"

#define NUM_ENTRIES 2

int get_object_factory_group(tag_t object, tag_t root_group, tag_t* user_factory_group)
{
	int erc;
	tag_t temp_group = NULLTAG;
	tag_t parent_group = NULLTAG;
	bool flag = true;

	try
	{
		WRITE_LOG(" %s\n", "asking object group");
		erc = AOM_ask_group(object, &temp_group);

		while(flag)
		{
			erc = SA_ask_group_parent(temp_group, &parent_group);
			if(parent_group == root_group || parent_group == NULLTAG)
			{
				WRITE_LOG("%s\n", "found true parent group");
				*user_factory_group = temp_group;
				flag = false;
			}
			temp_group = parent_group;
		}
	}
	catch(int exfail)
	{
		erc = exfail;
	}

	return erc;
}

EPM_decision_t check_user_group(EPM_rule_message_t msg)
{
	tag_t
		target_group = NULLTAG,
		root_task_group = NULLTAG,
		root_group = NULLTAG,
		dba_group,
		current_task = NULLTAG,
		root_task = NULLTAG,
		*attachments,
		attachment_type;
	int
		num_of_foreign_targets = 0,
		num_of_args = 0,
		*attachments_types,
		attachments_count = 0;
	char
		*query_name,
		*flag = NULL,
		Flag[256],
		*value= NULL,
		*normal_value= NULL;
	int erc = ITK_ok;
	EPM_decision_t decision = EPM_go;

	try
	{
		current_task = msg.task;

		WRITE_LOG(" %s\n", "looking for root group");
		erc = SA_find_group("ŒŒŒ —“¿Õ", &root_group);
		erc = SA_find_group("DBA", &dba_group);
		if(root_group == NULLTAG) throw EPM_invalid_group_name;
		WRITE_LOG(" %s\n", "asking root task");
		erc = EPM_ask_root_task(current_task, &root_task);
		erc = get_object_factory_group(root_task, root_group, &root_task_group);
		erc = EPM_ask_all_attachments(root_task, &attachments_count, &attachments, &attachments_types);
		for(int i = 0; i < attachments_count; i++)
		{
			if(attachments_types[i]==EPM_target_attachment)
			{
				WRITE_LOG(" %s\n", "cheking attachment");
				erc = get_object_factory_group(attachments[i], root_group, &target_group);
				if(root_task_group != target_group)
				{
					num_of_foreign_targets++;
					WRITE_LOG("%s\n", "errors++");
				}
			}
		}
	}
	catch (int exfail)
	{
		erc = exfail;
	}

	// Memory handling
	if(attachments_count>0)
	{
		MEM_free(attachments);
		MEM_free(attachments_types);
	}

	if(num_of_foreign_targets > 0)
	{
		decision = EPM_nogo;
		WRITE_LOG("%s\n", "nogo");
	}

	return decision;
}
