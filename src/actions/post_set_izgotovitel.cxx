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

int post_set_izgotovitel(METHOD_message_t *msg, va_list args)
{
	int erc = ITK_ok;
	tag_t owning_factory_group = NULLTAG;
	tag_t pref_group = NULLTAG;
	tag_t root_group = NULLTAG;
	int props_num = 0;
	char** props_values = NULL;
	char* prop_name = "st5_Izgotovitel";

	try
	{
		WRITE_LOG("%s\n", "Setting izgotovitel");
		tag_t object = msg->object_tag;

		erc = PREF_ask_char_values("IdealPlm_Group_To_Manuf_Map", &props_num, &props_values);
		erc = SA_find_group("ŒŒŒ —“¿Õ", &root_group);
		if(root_group == NULLTAG) throw EPM_invalid_group_name;

		for(int i = 0; i < props_num; i++)
		{
			char* lov_name = strchr(props_values[i], '%');
			if(lov_name!=NULL)
			{
				lov_name++;
				size_t pref_group_name_length = strcspn(props_values[i], "%");
				char* pref_group_name = (char*) MEM_alloc(sizeof(*pref_group_name) * (pref_group_name_length + 1));
				strncpy(pref_group_name, props_values[i],pref_group_name_length);
				pref_group_name[pref_group_name_length] = '\0';
				if(lov_name!=NULL)
				{
					erc = SA_find_group(pref_group_name, &pref_group);
					erc = get_object_factory_group(object, root_group, &owning_factory_group);
					if(pref_group!=NULLTAG && pref_group==owning_factory_group)
					{
						erc = AOM_refresh(object, TRUE);
						erc = AOM_set_value_string(object, prop_name, lov_name);
						erc = AOM_save(object);
						erc = AOM_refresh(object, FALSE);
					}
				}
				MEM_free(pref_group_name); pref_group_name = NULL;
			}
		}
	}
	catch (int exfail)
	{
		return exfail;
	}

	if(props_num > 0)
	{
		MEM_free(props_values); props_values = NULL;
	}
	return ITK_ok;
}
