#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "pcre.h"
#include "Validation.h"

#pragma comment(lib, "pcre.lib")

#define OVECCOUNT 30    /* should be a multiple of 3 */
#define EMAIL_RE        "^[a-zA-Z0-9_.-]+@[a-zA-Z0-9-]+(\.[a-zA-Z0-9-]+)*\.[a-zA-Z0-9]{2,6}$"
#define ID_RE           "^[A-Za-z0-9]{6,10}$"
#define PHONE_RE        "^[0-9]{11}$"

pcre *re_email;
pcre *re_id;
pcre *re_phone;

int ovector[OVECCOUNT];

int initializeRegex() 
{
	const char      *error;
	int             erroffset;

	re_email = pcre_compile(EMAIL_RE, 0, &error, &erroffset, NULL);
	if (re_email == NULL)
		return -1;

	re_id = pcre_compile(ID_RE, 0, &error, &erroffset, NULL);
	if (re_id == NULL)
		return -1;

	re_phone = pcre_compile(PHONE_RE, 0, &error, &erroffset, NULL);
	if (re_phone == NULL)
		return -1;

	return 1;
}

int vaildateEmail(char val[]) 
{
	if (re_email == NULL)
		initializeRegex();
	return pcre_exec(re_email, NULL, val, strlen(val), 0, 0, ovector, OVECCOUNT);
}

