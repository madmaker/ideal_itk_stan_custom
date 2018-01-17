#include <stdlib.h>
#include <item.h>
#include <grm.h>
#include <epm.h>
#include <ics.h>
#include <ics2.h>
#include <aom.h>
#include <aom_prop.h>
#include <epm_toolkit_tc_utils.h>
#include <ug_va_copy.h>
#include "../misc.hxx"
#include "get_sti_classification_data.hxx"

#define DESIRED_STI_CLASS_NAME "STI"

char* sti_result;
int sti_count;

int read_sti_classification_object_data(tag_t object)
{
	tag_t
		object_class,
		parent_class;
	bool
		cont = true;
	char
		*name;
	int erc = ITK_ok;

	erc = ICS_ask_class_of_classification_obj(object, &object_class);
	while(cont)
	{
		erc = ICS_ask_parent(object_class, &parent_class);
		if(parent_class != NULLTAG)
		{
			erc = AOM_ask_value_string(parent_class, "name", &name);
			WRITE_LOG("Found parent with name %s\n", name);
			if(strcmp(name, DESIRED_STI_CLASS_NAME)==0)
			{
				MEM_free(name);
				erc = AOM_ask_value_string(object_class, "name", &name);
				sti_result = (char*) MEM_alloc(strlen(name) + 1);
				strcpy(sti_result, name);
				sti_count++;
				WRITE_LOG("Found required parent %s\n for %i time", name, sti_count);
				break;
			}
			object_class = parent_class;
			MEM_free(name);
		}
		else
		{
			cont = false;
		}
	}

	return erc;
}

int get_sti_classification_data(METHOD_message_t *mess, va_list args)
{
	va_list vargs;
	va_copy(vargs, args);
	char** rvalue;
	tag_t prop_tag = va_arg(vargs, tag_t);
	rvalue = va_arg(vargs, char**);
	va_end(vargs);

	tag_t* icos = NULL;
	int icos_count;
	sti_result = NULL;
	sti_count = 0;
	int erc = ITK_ok;

	try
	{
		erc = ICS_ico_find("", mess->object_tag, 1, &icos_count, &icos);

		for(int i = 0; i < icos_count; i++)
		{
			WRITE_LOG("%s\n", "Reading ico data...");
			read_sti_classification_object_data(icos[i]);
		}

		if(sti_count!=1)
		{
			if(sti_result!=NULL)
				MEM_free(sti_result);
			sti_result = (char*) MEM_alloc(sizeof(char*));
			strcpy(sti_result, "");
		}

		*rvalue = (char*) MEM_alloc(strlen(sti_result) + 1);
		strcpy(*rvalue, sti_result);
	}
	catch (int exfail)
	{
		erc = exfail;
	}
	// Memory handling
	if(sti_result!=NULL) MEM_free(sti_result); sti_result = NULL;
	if(icos!=NULL) MEM_free(icos); icos = NULL;

	return erc;
}
