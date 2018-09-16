/*
	������֤ģ�飬��֤�û�����������Ƿ���Ч
*/
#pragma once

/*
	���벢��֤���䣬����֤ʧ����Ҫ����������
	hint: ��ʾ����Ļ�ϵ���ʾ����
	out:  ����������ַ���
*/
void inputEmail(const char hint[], char out[]);

/*
	���벢��֤�ֻ��ţ�����֤ʧ����Ҫ����������
	hint: ��ʾ����Ļ�ϵ���ʾ����
	out:  ����������ַ���
*/
void inputPhone(const char hint[], char out[]);

/*
	���벢��֤ID������֤ʧ����Ҫ����������
	hint: ��ʾ����Ļ�ϵ���ʾ����
	out:  ����������ַ���
*/
void inputID(const char hint[], char out[]);

/*
	���벢��֤����ʱ���ַ���������֤ʧ����Ҫ����������
	hint: ��ʾ����Ļ�ϵ���ʾ����
	out:  ����������ַ���
*/
void inputDateTime(const char hint[], char out[]);

/*
	���벢��֤ʱ���ַ���������֤ʧ����Ҫ����������
	hint: ��ʾ����Ļ�ϵ���ʾ����
	out:  ����������ַ���
*/
void inputTime(const char hint[], char out[]);

/*
	���벢��֤һ������������֤ʧ����Ҫ����������
	hint: ��ʾ����Ļ�ϵ���ʾ����
	����ֵ: �������������
*/
int inputInteger(const char hint[]);

/*
	���벢��֤һ��������������֤ʧ����Ҫ����������
	hint: ��ʾ����Ļ�ϵ���ʾ����
	����ֵ: ��������ĸ�����
*/
float inputFloat(const char hint[]);

/*
	����һ���ַ�������֤�䳤�ȣ�������������Ҫ����������
	hint: ��ʾ����Ļ�ϵ���ʾ����
	minLength: ��С����
	maxLength: ��󳤶�
	out:       ����������ַ���
*/
void inputStringWithLengthLimit(const char hint[], int minLength, int maxLength, char out[]);

/*
	�������룬����Ļ����"*"��ʾ
	hint: ��ʾ����Ļ�ϵ���ʾ����
	out: ����������ַ���
*/
void inputPassword(const char hint[], char out[]);

/*
	�Ӽ��̶�ȡһ���ַ� (����_getch���������ԣ�ÿ�ζ���һ���ַ����ٶ�ȡ��һ���ַ�ʱ�õ��Ľ����'\0'���ʽ��˺������з�װ)
*/
char readKey();

int inputBool(const char hint[]);