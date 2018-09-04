#include <iostream>
#include <string>
#include <tc/tc.h>
#include <property/prop_msg.h>
#include <tccore/custom.h>
#include <tccore/method.h>
#include "properties.hxx"
#include "get_sti_classification_data.hxx"
#include "get_pki_classification_data.hxx"

int stan_custom_idealplm_register_properties(int * decision, va_list args)
{
	int erc = ITK_ok;
	try
	{
		*decision = ALL_CUSTOMIZATIONS;
		METHOD_id_t meth = { 0 };

		erc = METHOD_register_prop_method("SPB5_StdRevision", "st5_ClassRazdel", PROP_ask_value_string_msg, get_sti_classification_data, NULL, &meth);
		erc = METHOD_register_prop_method("SPB5_PKIRevision", "st5_ClassRazdel", PROP_ask_value_string_msg, get_pki_classification_data, NULL, &meth);
	}
	catch (int exfail)
	{
		erc = exfail;
	}
	return erc;
}
