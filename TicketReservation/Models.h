/*
	�ṹ��ģ�飬��������Ŀ���õ��Ĳ�ͬ���͵����ݵĽṹ��
	���ߣ���ͬ 2017212783
	����޸����ڣ�2018-9-13
*/

#pragma once

/*
	�ο�����Ϣ
*/
typedef struct {
	char id[10];
	char password[20];
	int peopleCount;
	int age;
	char phone[11];
} TouristGroup;

/*
	ϵͳ����Ա��Ϣ
*/
typedef struct {
	char id[10];
	char password[20];
	char phone[11];
	char email[30];
} SystemAdmin;

/*
	��������������Ϣ��������������ͬʱ�ε����ƣ�
*/
typedef struct {
	int id;
	char featureSpotId[6];
	int isHot;
	int timeSpanStart;
	int timeSpanEnd;
	int peopleCount;
} FeatureSpotLimitation;

/*
	������Ϣ
*/
typedef struct {
	char id[6];
	char name[20];
	char discription[200];
	int coldSeasonPrice;
	int hotSeasonPrice;
	int timeRequired;
	float soldierDiscount;
	float studentDiscount;
	int level;
	FeatureSpotLimitation *limitations;
	int limitationCount;
	char district[20];
	int maintenanceFee;
	int reservationCount;
	int totalTicketCount;
	int totalProfit;
	int isSuitableForEldersAndChildren;
} FeatureSpot;

/*
	�ο����
*/
typedef enum {
	NORMAL = 0,
	STUDENT = 1,
	SOLDIER = 2,
} TouristIdantity;

/*
	������Ϣ
*/
typedef struct {
	char id[30];
	char orderDate[10];
	FeatureSpot *featureSpot;
	TouristGroup *touristGroup;
	char time[16];
	TouristIdantity idantity;
	float totalPrice;
} Reservation;