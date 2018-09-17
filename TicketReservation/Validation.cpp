/*
	输入验证模块，验证用户输入的数据是否有效
	最后修改日期：2018-9-13
*/

#include "stdafx.h"
#include <conio.h>                  // "_getc" 函数所在的库
#include "pcre.h"                   // 本模块使用的正则表达式功能由pcre库提供
#include "Validation.h"

#pragma comment(lib, "pcre.lib")

#define OVECCOUNT 30    /* should be a multiple of 3 */
#define EMAIL_RE        "^[a-zA-Z0-9_.-]+@[a-zA-Z0-9-]+(\\.[a-zA-Z0-9-]+)*\\.[a-zA-Z0-9]{2,6}$"  // 验证邮箱的正则表达式
#define ID_RE           "^[A-Za-z0-9]{6,10}$"                                                    // 验证用户id、系统管理员id的正则表达式
#define PHONE_RE        "^[0-9]{11}$"                                                            // 验证手机号的正则表达式
#define INT_RE          "^[+-]?[0-9]*$"                                                          // 验证整数的正则表达式
#define FLOAT_RE        "^[-+]?[0-9]*\.?[0-9]+$"                                                 // 验证浮点数的正则表达式
#define DATETIME_RE     "^(\\d\\d\\d\\d):(0[1-9]|1[0-2]):([0-3]\\d):(20|21|22|23|[0-1]\\d):[0-5]\\d$"// 验证日期时间的正则表达式
#define TIME_RE         "^(20|21|22|23|[0-1]\\d):[0-5]\\d$"                                      // 验证时间的正则表达式

int ovector[OVECCOUNT];


/*
	(内部函数) 使用正则表达式验证数据
	hint: 需要显示在屏幕上的提示文字
	out: 输出读取到的字符串
	regex: 所用正则表达式
*/
void inputValidate(const char hint[], char out[], const char regex[]) 
{
	const char *error;
	int erroffset;

	pcre *re = pcre_compile(regex, 0, &error, &erroffset, NULL);  // 初始化正则表达式
	if (re == NULL)                  
		return;

	char input[256];                 // 输入缓冲区
	int flag = -1;
	printf(hint);                // 输出提示信息
	printf(" ");                 // 输出一个空格，将提示信息和输入部分分隔开，使界面美观
	while (flag < 0)             // 不断循环，直到输入正确为止
	{
		gets_s(input, 256);          // 从控制台读取一行字符串
		if (strlen(input) != 0) {    // 有时候读入字符串长度为0，为防止输出错误信息，在这里判断一下
			flag = pcre_exec(re, NULL, input, strlen(input), 0, 0, ovector, OVECCOUNT); // 使用正则表达式验证输入
			if (flag < 0) {
				printf("\x1b[01;31m您输入的格式不正确, 请重新输入\033[0m\n");  // 输出错误信息（红色字体）
				printf(hint);                // 再次输出提示信息
				printf(" ");
			}
		}
	}

	strcpy(out, input);              // 将读取到的值存入out参数
}

// 输入邮箱地址，如输入错误则要求重新输入
void inputEmail(const char hint[], char out[])  
{
	inputValidate(hint, out, EMAIL_RE);
}

// 输入手机号码，如输入错误则要求重新输入
void inputPhone(const char hint[], char out[])    
{
	inputValidate(hint, out, PHONE_RE);
}

// 输入ID，如输入错误则要求重新输入
void inputID(const char hint[], char out[])
{
	inputValidate(hint, out, ID_RE);
}

// 输入日期时间，如输入错误则要求重新输入
void inputDateTime(const char hint[], char out[])
{
	inputValidate(hint, out, DATETIME_RE);
}

// 输入时间，如输入错误则要求重新输入
void inputTime(const char hint[], char out[])
{
	inputValidate(hint, out, TIME_RE);
}

// 输入一个整数，如输入错误则要求重新输入
int inputInteger(const char hint[])
{
	char input[10];
	inputValidate(hint, input, INT_RE);
	return atoi(input);
}

// 输入一个浮点数，如输入错误则要求重新输入
float inputFloat(const char hint[])
{
	char input[10];
	inputValidate(hint, input, FLOAT_RE);
	return atof(input);
}

// 输入一个字符串，判断其长度是否符合要求
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
			flag = strlen(input) >= minLength && strlen(input) <= maxLength; // 判断字符串长度是否符合要求
			if (!flag) {
				printf("\x1b[01;31m您输入的文本长度不符合要求，请输入%d到%d个字符的文本\033[0m\n", minLength, maxLength);
				printf(hint);
				printf(" ");
			}
		}
	}

	strcpy(out, input);              
}

// 输入密码，在屏幕上以"*"代表字符
void inputPassword(const char hint[], char out[]) {
	char input[50];
	int index = 0;
	char ch;
	printf(hint);
	printf(" ");
	while ((ch = _getch()) != '\r') { // 读到回车为止
		if (ch == '\b' && index > 0) { // 退格键
			printf("\b \b");   // 空格+\b 是为了覆盖掉原来的字符 
			index--;
		}
		else if (ch != '\b' && ch != '\0') { // 忽略\0字符 (有些系统中每输入一个字符后面会自动加上一个\0)
			input[index++] = ch;
			printf("*");
		}
	}
	input[index] = '\0';   // 添加字符串结束符
	printf("\n");

	strcpy(out, input);
}

// 从键盘上读取一个字符
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
			printf("\n\x1b[01;31m输入有误！\033[0m\n");
	}
}