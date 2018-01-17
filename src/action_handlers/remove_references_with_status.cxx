#include <stdlib.h>
#include <item.h>
#include <grm.h>
#include <epm.h>
#include <aom.h>
#include <epm_toolkit_tc_utils.h>
#include <base_utils/ResultCheck.hxx>
#include "../misc.hxx"
#include "remove_targets_from_refs.hxx"

#define NUM_ENTRIES 2

int has_release_status(tag_t object, char* status_name, bool* has_release_status)
{
	bool result = false;
	tag_t *statuses = NULL;
	int statuses_count = 0;
	char* status_name_temp = NULL;
	int erc;

	try
	{
		erc = WSOM_ask_release_status_list(object, &statuses_count, &statuses);

		if(!STR_EMPTY(status_name))
		{
			for (int i = 0; i < statuses_count; i++)
			{
				erc = AOM_ask_name(statuses[i], &status_name_temp);
				if(strcmp(status_name, status_name_temp)==0)
				{
					result = true;
					break;
				}
			}
		}
		else if (statuses_count>0)
		{
			result = true;
		}
	}
	catch(int exfail)
	{
		erc = exfail;
	}

	// Output
	*has_release_status = result;
	// Memory handling
	if (status_name_temp) MEM_free (status_name_temp);
	if (statuses_count>0) MEM_free (statuses);

	return erc;
}

int remove_references_with_status(EPM_action_message_t msg)
{
	tag_t
		root_task = NULLTAG,
		*attachments,
		*attachments_to_delete,
		workspace_object_type,
		attachment_type;
	int
		num_of_args = 0,
		*attachments_types,
		attachments_count = 0,
		attachments_to_delete_count = 0;
	char
		*status = NULL,
		*flag = NULL,
		Flag[256],
		*value= NULL,
		*normal_value= NULL;
	bool
		is_workspace_object_type = false,
		bhas_release_status = false;
	int erc = ITK_ok;

	try
	{
		num_of_args = TC_number_of_arguments(msg.arguments);
		if (num_of_args!=1)
		{
			throw EPM_wrong_number_of_arguments;
		}

		for (int numctr=0; numctr<num_of_args && erc==ITK_ok;numctr++)
		{
			erc = ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &flag, &value);
			if (erc == ITK_ok)
			{
				erc = EPM_substitute_keyword(value,&normal_value);
			    if (normal_value != NULL)
			    {
			    	StrCpy(Flag,256,flag);
			        StrUpr(Flag);
			        if (erc == ITK_ok) {
			        	if (strcmp("STATUS", Flag)==0) {
			        		if (!STR_EMPTY(normal_value)) {
			        			status = (char *) MEM_alloc(sizeof(char)*(strlen(normal_value)+1));
				                strcpy(status, normal_value);
			        		} else {
			        			status = "";
			        		}
			        	}
			        }
			        if (normal_value != NULL)
			        {
			        	MEM_free(normal_value);
				        normal_value = NULL;
			        }
			    } else {
			    	WRITE_LOG("%s\n", "Empty status");
        			status = "";
        		}
				if (flag != NULL) {
					MEM_free(flag);
					flag = NULL;
				}
				if (value != NULL) {
					MEM_free(value);
					value = NULL;
				}
			} else {
				WRITE_LOG("Error: rcode1=%i\n", erc);
			}
		}

		erc = EPM_ask_root_task(msg.task, &root_task);
		erc = EPM_ask_all_attachments(root_task, &attachments_count, &attachments, &attachments_types);
		attachments_to_delete =(tag_t*) MEM_alloc(sizeof(tag_t)*attachments_count);

		erc = TCTYPE_find_type("WorkspaceObject", NULL, &workspace_object_type);
		for(int i = 0; i < attachments_count; i++)
		{
			if(attachments_types[i]==EPM_reference_attachment)
			{
				erc = TCTYPE_ask_object_type(attachments[i], &attachment_type);
				erc = TCTYPE_is_type_of(attachment_type, workspace_object_type, &is_workspace_object_type);
				if(is_workspace_object_type)
				{
					erc = has_release_status(attachments[i], status, &bhas_release_status);
					if(bhas_release_status)
					{
						attachments_to_delete[attachments_to_delete_count] = attachments[i];
						attachments_to_delete_count++;
					}
				}
			}
		}

		erc = EPM_remove_attachments(root_task, attachments_to_delete_count, attachments_to_delete);

	}
	catch (int exfail)
	{
		erc = exfail;
	}
	// Memory handling
	if(attachments_count>0){ MEM_free(attachments);	MEM_free(attachments_types); }
	if(attachments_to_delete_count>0) MEM_free(attachments_to_delete);

	return erc;
}
