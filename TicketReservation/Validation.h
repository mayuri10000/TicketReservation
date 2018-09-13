/*
	输入验证模块，验证用户输入的数据是否有效
	作者：刘同 2017212783
	最后修改日期：2018-9-13
*/
#pragma once

/*
	输入并验证邮箱，如验证失败则要求重新输入
	hint: 显示在屏幕上的提示文字
	out:  传出输入的字符串
*/
void inputEmail(const char hint[], char out[]);

/*
	输入并验证手机号，如验证失败则要求重新输入
	hint: 显示在屏幕上的提示文字
	out:  传出输入的字符串
*/
void inputPhone(const char hint[], char out[]);

/*
	输入并验证ID，如验证失败则要求重新输入
	hint: 显示在屏幕上的提示文字
	out:  传出输入的字符串
*/
void inputID(const char hint[], char out[]);

/*
	输入并验证一个整数，如验证失败则要求重新输入
	hint: 显示在屏幕上的提示文字
	返回值: 返回输入的整数
*/
int inputInteger(const char hint[]);

/*
	输入并验证一个浮点数，如验证失败则要求重新输入
	hint: 显示在屏幕上的提示文字
	返回值: 返回输入的浮点数
*/
float inputFloat(const char hint[]);

/*
	输入一个字符串并验证其长度，如过长或过短则要求重新输入
	hint: 显示在屏幕上的提示文字
	minLength: 最小长度
	maxLength: 最大长度
	out:       传出输入的字符串
*/
void inputStringWithLengthLimit(const char hint[], int minLength, int maxLength, char out[]);