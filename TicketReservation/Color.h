/*
	����̨��ɫ���� (�粻����ʾ��ɫ����stdafx.h��TicketReservation.cppͷ������"#define NOCOLOR"����)
	����޸�����: 2018-9-15
*/
#pragma once
#ifdef NOCOLOR 
#define COLOR_RED     ""         // ����ɫ��ʹ����ɫ��
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
#define COLOR_RED     "\x1b[31m"   // ASCII ��ɫ��
#define COLOR_GREEN   "\x1b[32m"   // ����Ϊ��ɫ
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"   // ��ɫ��λ��

#define COLOR_RED_B     "\x1b[01;31m" // ����Ϊ��ɫ
#define COLOR_GREEN_B   "\x1b[01;32m"
#define COLOR_YELLOW_B  "\x1b[01;33m"
#define COLOR_BLUE_B    "\x1b[01;34m"
#define COLOR_MAGENTA_B "\x1b[01;35m"
#define COLOR_CYAN_B    "\x1b[01;36m"
#endif