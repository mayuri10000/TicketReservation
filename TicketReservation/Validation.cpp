/*
	������֤ģ�飬��֤�û�����������Ƿ���Ч
	����޸����ڣ�2018-9-13
*/

#include "stdafx.h"
#include <conio.h>                  // "_getc" �������ڵĿ�
#include "pcre.h"                   // ��ģ��ʹ�õ�������ʽ������pcre���ṩ
#include "Validation.h"

#pragma comment(lib, "pcre.lib")

#define OVECCOUNT 30    /* should be a multiple of 3 */
#define EMAIL_RE        "^[a-zA-Z0-9_.-]+@[a-zA-Z0-9-]+(\\.[a-zA-Z0-9-]+)*\\.[a-zA-Z0-9]{2,6}$"  // ��֤�����������ʽ
#define ID_RE           "^[A-Za-z0-9]{6,10}$"                                                    // ��֤�û�id��ϵͳ����Աid��������ʽ
#define PHONE_RE        "^[0-9]{11}$"                                                            // ��֤�ֻ��ŵ�������ʽ
#define INT_RE          "^[+-]?[0-9]*$"                                                          // ��֤������������ʽ
#define FLOAT_RE        "^[-+]?[0-9]*\.?[0-9]+$"                                                 // ��֤��������������ʽ
#define DATETIME_RE     "^(\\d\\d\\d\\d):(0[1-9]|1[0-2]):([0-3]\\d):(20|21|22|23|[0-1]\\d):[0-5]\\d$"// ��֤����ʱ���������ʽ
#define TIME_RE         "^(20|21|22|23|[0-1]\\d):[0-5]\\d$"                                      // ��֤ʱ���������ʽ

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
	printf(hint);                // �����ʾ��Ϣ
	printf(" ");                 // ���һ���ո񣬽���ʾ��Ϣ�����벿�ַָ�����ʹ��������
	while (flag < 0)             // ����ѭ����ֱ��������ȷΪֹ
	{
		gets_s(input, 256);          // �ӿ���̨��ȡһ���ַ���
		if (strlen(input) != 0) {    // ��ʱ������ַ�������Ϊ0��Ϊ��ֹ���������Ϣ���������ж�һ��
			flag = pcre_exec(re, NULL, input, strlen(input), 0, 0, ovector, OVECCOUNT); // ʹ��������ʽ��֤����
			if (flag < 0) {
				printf("\x1b[01;31m������ĸ�ʽ����ȷ, ����������\033[0m\n");  // ���������Ϣ����ɫ���壩
				printf(hint);                // �ٴ������ʾ��Ϣ
				printf(" ");
			}
		}
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

// ����ID�������������Ҫ����������
void inputID(const char hint[], char out[])
{
	inputValidate(hint, out, ID_RE);
}

// ��������ʱ�䣬�����������Ҫ����������
void inputDateTime(const char hint[], char out[])
{
	inputValidate(hint, out, DATETIME_RE);
}

// ����ʱ�䣬�����������Ҫ����������
void inputTime(const char hint[], char out[])
{
	inputValidate(hint, out, TIME_RE);
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
	printf(hint);
	printf(" ");
	while (!flag)
	{
		gets_s(input, 256);
		if (strlen(input) != 0) {
			flag = strlen(input) >= minLength && strlen(input) <= maxLength; // �ж��ַ��������Ƿ����Ҫ��
			if (!flag) {
				printf("\x1b[01;31m��������ı����Ȳ�����Ҫ��������%d��%d���ַ����ı�\033[0m\n", minLength, maxLength);
				printf(hint);
				printf(" ");
			}
		}
	}

	strcpy(out, input);              
}

// �������룬����Ļ����"*"�����ַ�
void inputPassword(const char hint[], char out[]) {
	char input[50];
	int index = 0;
	char ch;
	printf(hint);
	printf(" ");
	while ((ch = _getch()) != '\r') { // �����س�Ϊֹ
		if (ch == '\b' && index > 0) { // �˸��
			printf("\b \b");   // �ո�+\b ��Ϊ�˸��ǵ�ԭ�����ַ� 
			index--;
		}
		else if (ch != '\b' && ch != '\0') { // ����\0�ַ� (��Щϵͳ��ÿ����һ���ַ�������Զ�����һ��\0)
			input[index++] = ch;
			printf("*");
		}
	}
	input[index] = '\0';   // ����ַ���������
	printf("\n");

	strcpy(out, input);
}

// �Ӽ����϶�ȡһ���ַ�
char readKey()
{
	char key = 0;
	while (key == 0) {
		key = _getch();
	}
	return key;
}

int inputBool(const char hint[]) {
	while (1) {
		printf("%s\033[01;33m[Y/N]\033[0m: ", hint);
		char input = readKey();
		printf("%c\n", input);
		if (input == 'y' || input == 'Y') {
			return 1;
		}
		else if (input == 'n' || input == 'N')
		{
			return 0;
		}
		else
			printf("\n\x1b[01;31m��������\033[0m\n");
	}
}