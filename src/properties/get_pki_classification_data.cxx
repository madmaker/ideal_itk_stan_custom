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
#include "get_pki_classification_data.hxx"

#define DESIRED_PKI_CLASS_NAME "Покупные изделия"

char* pki_result;
int pki_count;

int read_pki_classification_object_data(tag_t object)
{
	tag_t
		object_class,
		parent_class,
		parent_parent_class;
	bool
		cont = true;
	char
		*name = NULL;
	int erc = ITK_ok;

	erc = ICS_ask_class_of_classification_obj(object, &object_class);
	while(cont)
	{
		erc = ICS_ask_parent(object_class, &parent_class);
		if(parent_class != NULLTAG)
		{
			erc = ICS_ask_parent(parent_class, &parent_parent_class);
			if(parent_parent_class != NULLTAG)
			{
				erc = AOM_ask_value_string(parent_parent_class, "name", &name);
				WRITE_LOG("Found parent with name %s\n", name);
				if(strcmp(name, DESIRED_PKI_CLASS_NAME)==0)
				{
					MEM_free(name);
					erc = AOM_ask_value_string(object_class, "name", &name);
					pki_result = (char*) MEM_alloc(strlen(name) + 1);
					strcpy(pki_result, name);
					pki_count++;
					WRITE_LOG("Found required parent %s\n for %i time", name, pki_count);
					break;
				}
				object_class = parent_class;
				MEM_free(name);
			}
		}
		else
		{
			cont = false;
		}
	}

	return erc;
}

int get_pki_classification_data(METHOD_message_t *mess, va_list args)
{
	va_list vargs;
	va_copy(vargs, args);
	char** rvalue;
	tag_t prop_tag = va_arg(vargs, tag_t);
	rvalue = va_arg(vargs, char**);
	va_end(vargs);

	tag_t* icos = NULL;
	int icos_count;
	pki_result = NULL;
	pki_count = 0;
	int erc = ITK_ok;

	try
	{
		erc = ICS_ico_find("", mess->object_tag, 1, &icos_count, &icos);

		for(int i = 0; i < icos_count; i++)
		{
			WRITE_LOG("%s\n", "Reading ico data...");
			read_pki_classification_object_data(icos[i]);
		}

		if(pki_count!=1)
		{
			if(pki_result!=NULL)
				MEM_free(pki_result);
			pki_result = (char*) MEM_alloc(sizeof(char*));
			strcpy(pki_result, "");
		}

		*rvalue = (char*) MEM_alloc(strlen(pki_result) + 1);
		strcpy(*rvalue, pki_result);
	}
	catch (int exfail)
	{
		erc = exfail;
	}
	// Memory handling
	if(pki_result!=NULL) MEM_free(pki_result); pki_result = NULL;
	if(icos!=NULL) MEM_free(icos); icos = NULL;

	return erc;
}
