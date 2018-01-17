#include <stdlib.h>
#include <item.h>
#include <grm.h>
#include <epm.h>
#include <epm_toolkit_tc_utils.h>
#include <qry_errors.h>
#include <base_utils/ResultCheck.hxx>
#include "../misc.hxx"
#include "remove_targets_from_refs.hxx"

#define NUM_ENTRIES 2

tag_t current_task = NULLTAG;
tag_t query = NULLTAG;

int is_related_to_target_object(char* item_id, char* item_revision_id, bool* isrelated)
{
	bool result = false;
	char** entryNames = NULL;
	char** entryValues = NULL;
	int resultCount;
	int erc = ITK_ok;

	try
	{
		char entryNamesArray[NUM_ENTRIES][QRY_uid_name_size_c + 1] = { "item_id_relation", "item_revision_id_relation" };
		entryNames = (char**) MEM_alloc( NUM_ENTRIES * sizeof *entryNames);

		for (int index = 0; index < NUM_ENTRIES; index++)
		{
			entryNames[index] = (char*) MEM_alloc( strlen(entryNamesArray[index]) + 1 );
			if (entryNames[index])
				strcpy(entryNames[index], entryNamesArray[index]);
		}

		entryValues = (char**) MEM_alloc(NUM_ENTRIES * sizeof *entryValues);
		entryValues[0] = (char *) MEM_alloc(strlen(item_id) + 1);
		entryValues[1] = (char *) MEM_alloc(strlen(item_revision_id) + 1);
		strcpy(entryValues[0], item_id);
		strcpy(entryValues[1], item_revision_id);


		auto_itk_mem_free<tag_t> results_found;
		erc = QRY_execute(query, NUM_ENTRIES, entryNames, entryValues, &resultCount, results_found.operator tag_t **());

		for (int i = 0; i < resultCount ; i++)
		{
			tag_t found_object = results_found.operator tag_t *()[i];

			if (found_object != NULL_TAG)
			{
				if(found_object == current_task)
				{
					result = true;
				}
			}
		}
	}
	catch(int exfail)
	{
		erc = exfail;
	}
	// Output
	*isrelated = result;
	// Memory handling
	if(entryNames!=NULL)
	{
		MEM_free(entryNames[0]);
		MEM_free(entryNames[1]);
		MEM_free(entryNames);
	}
	if(entryValues!=NULL)
	{
		MEM_free(entryValues[0]);
		MEM_free(entryValues[1]);
		MEM_free(entryValues);
	}
	return erc;
}

int remove_targets_from_refs(EPM_action_message_t msg) {
	tag_t
		root_task = NULLTAG,
		*attachments,
		*attachments_to_delete = NULL,
		item_rev_type,
		attachment_type;
	int
		num_of_args = 0,
		*attachments_types,
		attachments_count = 0,
		attachments_to_delete_count = 0;
	char
		*query_name,
		*flag = NULL,
		Flag[256],
		*value= NULL,
		*normal_value= NULL;
	bool
		bis_related_to_target_object = false,
		is_item_revision_type = false;
	int erc = ITK_ok;

	try
	{
		current_task = msg.task;
		num_of_args = TC_number_of_arguments(msg.arguments);
		if (num_of_args!=1)
		{
			throw EPM_wrong_number_of_arguments;
		}

		for (int numctr=0;numctr<num_of_args && erc==ITK_ok;numctr++)
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
			        	if (strcmp("WF_FOR_REFERENCE", Flag)==0) {
			        		if (!STR_EMPTY(normal_value)) {
			        			query_name = (char *) MEM_alloc(sizeof(char)*(strlen(normal_value)+1));
				                strcpy(query_name, normal_value);
			        		}
			        	}
			        }
			        if (normal_value != NULL)
			        {
			        	MEM_free(normal_value);
				        normal_value = NULL;
			        }
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

		erc = QRY_find(query_name, &query);
		if (query == NULL_TAG)
		{
			WRITE_LOG("Error: query %s not found!\n", query_name);
			throw QRY_invalid_query;
		}

		erc = EPM_ask_root_task(current_task, &root_task);
		erc = EPM_ask_all_attachments(root_task, &attachments_count, &attachments, &attachments_types);
		attachments_to_delete =(tag_t*) MEM_alloc(sizeof(tag_t)*attachments_count);

		erc = TCTYPE_find_type("ItemRevision", NULL, &item_rev_type);
		for(int i = 0; i < attachments_count; i++)
		{
			if(attachments_types[i]==EPM_reference_attachment)
			{
				char item_id[ITEM_id_size_c+1];
				char rev_id[ITEM_id_size_c+1];
				tag_t item = NULLTAG;
				tag_t rev = NULLTAG;

				rev = attachments[i];
				erc = TCTYPE_ask_object_type(attachments[i], &attachment_type);
				erc = TCTYPE_is_type_of(attachment_type, item_rev_type, &is_item_revision_type);
				if(is_item_revision_type)
				{
					erc = ITEM_ask_item_of_rev(rev, &item);
					erc = ITEM_ask_id(item, item_id);
					erc = ITEM_ask_rev_id(rev, rev_id);
					if(item_id!=NULL && rev_id!=NULL && !STR_EMPTY(item_id) && !STR_EMPTY(rev_id))
					{
						erc = is_related_to_target_object(item_id, rev_id, &bis_related_to_target_object);
						if(bis_related_to_target_object)
						{
							attachments_to_delete[attachments_to_delete_count] = attachments[i];
							attachments_to_delete_count++;
						}
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
	if(attachments_count>0)
	{
		MEM_free(attachments);
		MEM_free(attachments_types);
	}
	if(attachments_to_delete!=NULL) MEM_free(attachments_to_delete);

	return erc;
}
