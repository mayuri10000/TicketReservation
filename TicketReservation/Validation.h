/*
	������֤ģ�飬��֤�û�����������Ƿ���Ч
	���ߣ���ͬ 2017212783
	����޸����ڣ�2018-9-13
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