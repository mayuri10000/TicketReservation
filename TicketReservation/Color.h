/*
	控制台颜色定义 (如不想显示颜色，在stdafx.h或TicketReservation.cpp头部加上"#define NOCOLOR"即可)
	最后修改日期: 2018-9-15
*/
#pragma once
#ifdef NOCOLOR 
#define COLOR_RED     ""         // 无颜色则不使用颜色码
#define COLOR_GREEN   ""
#define COLOR_YELLOW  ""
#define COLOR_BLUE    ""
#define COLOR_MAGENTA ""
#define COLOR_CYAN    ""
#define COLOR_RESET   ""

#define COLOR_RED_B     ""
#define COLOR_GREEN_B   ""
#define COLOR_YELLOW_B  ""
#define COLOR_BLUE_B    ""
#define COLOR_MAGENTA_B ""
#define COLOR_CYAN_B    ""
#else                              
#define COLOR_RED     "\x1b[31m"   // ASCII 颜色码
#define COLOR_GREEN   "\x1b[32m"   // 以下为暗色
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"   // 颜色复位码

#define COLOR_RED_B     "\x1b[01;31m" // 以下为亮色
#define COLOR_GREEN_B   "\x1b[01;32m"
#define COLOR_YELLOW_B  "\x1b[01;33m"
#define COLOR_BLUE_B    "\x1b[01;34m"
#define COLOR_MAGENTA_B "\x1b[01;35m"
#define COLOR_CYAN_B    "\x1b[01;36m"
#endif