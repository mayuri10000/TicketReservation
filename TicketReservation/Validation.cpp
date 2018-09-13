/*
	������֤ģ�飬��֤�û�����������Ƿ���Ч
	���ߣ���ͬ 2017212783
	����޸����ڣ�2018-9-13
*/

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "pcre.h"                   // ��ģ��ʹ�õ�������ʽ������pcre���ṩ
#include "Validation.h"

#pragma comment(lib, "pcre.lib")

#define OVECCOUNT 30    /* should be a multiple of 3 */
#define EMAIL_RE        "^[a-zA-Z0-9_.-]+@[a-zA-Z0-9-]+(\.[a-zA-Z0-9-]+)*\.[a-zA-Z0-9]{2,6}$" // ��֤�����������ʽ
#define ID_RE           "^[A-Za-z0-9]{6,10}$"                                                 // ��֤�û�id��ϵͳ����Աid��������ʽ
#define PHONE_RE        "^[0-9]{11}$"                                                         // ��֤�ֻ��ŵ�������ʽ
#define INT_RE          "^[+-]?[0-9]*$"                                                       // ��֤������������ʽ
#define FLOAT_RE        "^[-+]?[0-9]*\.?[0-9]+$"                                              // ��֤��������������ʽ

int ovector[OVECCOUNT];


/*
	(�ڲ�����) ʹ��������ʽ��֤����
	hint: ��Ҫ��ʾ����Ļ�ϵ���ʾ����
	out: �����ȡ�����ַ���
	regex: ����������ʽ
*/
void inputValidate(const char hint[], char out[], const char regex[]) 
{
	const char *error;
	int erroffset;

	pcre *re = pcre_compile(regex, 0, &error, &erroffset, NULL);  // ��ʼ��������ʽ
	if (re == NULL)                  
		return;

	char input[256];                 // ���뻺����
	int flag = -1;
	while (flag < 0)                 // ����ѭ����ֱ��������ȷΪֹ
	{
		printf(hint);                // �����ʾ��Ϣ
		printf(" ");                 // ���һ���ո񣬽���ʾ��Ϣ�����벿�ַָ�����ʹ��������
		gets_s(input, 256);          // �ӿ���̨��ȡһ���ַ���
		flag = pcre_exec(re, NULL, input, strlen(input), 0, 0, ovector, OVECCOUNT); // ʹ��������ʽ��֤����
		if (flag < 0)
			printf("\x1b[01;31m������ĸ�ʽ����ȷ, ����������\033[0m\n");  // ���������Ϣ����ɫ���壩
	}

	strcpy(out, input);              // ����ȡ����ֵ����out����
}

// ���������ַ�������������Ҫ����������
void inputEmail(const char hint[], char out[])  
{
	inputValidate(hint, out, EMAIL_RE);
}

// �����ֻ����룬�����������Ҫ����������
void inputPhone(const char hint[], char out[])    
{
	inputValidate(hint, out, PHONE_RE);
}

// ������ID�������������Ҫ����������
void inputID(const char hint[], char out[])
{
	inputValidate(hint, out, ID_RE);
}

// ����һ�������������������Ҫ����������
int inputInteger(const char hint[])
{
	char input[10];
	inputValidate(hint, input, INT_RE);
	return atoi(input);
}

// ����һ���������������������Ҫ����������
float inputFloat(const char hint[])
{
	char input[10];
	inputValidate(hint, input, FLOAT_RE);
	return atof(input);
}

// ����һ���ַ������ж��䳤���Ƿ����Ҫ��
void inputStringWithLengthLimit(const char hint[], int minLength, int maxLength, char out[]) 
{
	char input[256];
	int flag = 0;
	while (!flag)                 
	{
		printf(hint);                
		printf(" ");                 
		gets_s(input, 256);         
		flag = strlen(input) >= minLength && strlen(input) <= maxLength; // �ж��ַ��������Ƿ����Ҫ��
		if (!flag)
			printf("\x1b[01;31m��������ı����Ȳ�����Ҫ��������%d��%d���ַ����ı�\033[0m\n", minLength, maxLength);
	}

	strcpy(out, input);              
}
