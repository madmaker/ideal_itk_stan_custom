#include <epm/epm.h>
#include <sa/sa.h>
#include <epm/epm_task_template_itk.h>
#include <user_exits/epm_toolkit_utils.h>
#include <tc_util.h>
#include "post_set_izgotovitel.hxx"
#include "../action_handlers/check_user_group.hxx"
#include "../misc.hxx"
#include <base_utils/ResultCheck.hxx>

/*
 *  Automatically sets "Izgotovitel" string value based on creating group
 */

int is_subgroup_of_group(tag_t inner_group, tag_t outer_group, bool* isSubgroupOfGroup)
{
	int erc = ITK_ok;
	int child_groups_num = 0;
	tag_t *child_groups = NULL;

	try
	{
		erc = SA_ask_group_child_groups(outer_group, false, &child_groups_num, &child_groups);
		for(int i = 0; i < child_groups_num; i++)
		{
			if(child_groups[i] == inner_group)
			{
				*isSubgroupOfGroup = true;
				break;
			}
		}
	}
	catch (int exfail)
	{
		return exfail;
	}

	if(child_groups_num>0) MEM_free(child_groups); child_groups = NULL;
	return erc;
}

int post_set_izgotovitel(METHOD_message_t *msg, va_list args)
{
	int erc = ITK_ok;
	tag_t owning_group = NULLTAG;
	tag_t pref_group = NULLTAG;
	char owning_group_name[SA_name_size_c + 1];
	char* full_owning_group_name = NULL;
	int props_num = 0;
	char** props_values = NULL;

	try
	{
		WRITE_LOG("%s\n", "Setting izgotovitel");
		tag_t object = msg->object_tag;

		erc = AOM_ask_group(object, &owning_group);
		erc = SA_ask_group_name(owning_group, owning_group_name);
		erc = SA_ask_group_full_name(owning_group, &full_owning_group_name);
		erc = PREF_ask_char_values("IdealPlm_Group_To_Manuf_Map", &props_num, &props_values);

		for(int i = 0; i < props_num; i++)
		{
			char* lov_name = strchr(props_values[i], '%');
			lov_name++;
			size_t pref_group_name_length = strcspn(props_values[i], "%");
			char* pref_group_name = (char*) MEM_alloc(sizeof(*pref_group_name) * (pref_group_name_length + 1));
			strncpy(pref_group_name, props_values[i],pref_group_name_length);
			pref_group_name[pref_group_name_length] = '\0';
			if(lov_name!=NULL)
			{
				erc = SA_find_group(pref_group_name, &pref_group);
				if(pref_group!=NULLTAG)
				{
					bool is_child_group_of = false;
					is_subgroup_of_group(owning_group, pref_group, &is_child_group_of);
					if(is_child_group_of)
					{
						erc = AOM_refresh(object, TRUE);
						erc = AOM_set_value_string(object, "ip_classification", lov_name);
						erc = AOM_save(object);
						erc = AOM_refresh(object, FALSE);
					}
				}
			}
			MEM_free(pref_group_name); pref_group_name = NULL;
		}
	}
	catch (int exfail)
	{
		return exfail;
	}

	if(full_owning_group_name!=NULL) MEM_free(full_owning_group_name); full_owning_group_name = NULL;
	if(props_num > 0)
	{
		MEM_free(props_values); props_values = NULL;
	}
	return ITK_ok;
}
