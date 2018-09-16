/*
	�ṹ��ģ�飬��������Ŀ���õ��Ĳ�ͬ���͵����ݵĽṹ��
*/

#pragma once

/*
	�ο�����Ϣ
*/
typedef struct {
	char id[11];     // ���ڶ����ַ�����Ҫ���һ�ֽڣ���������ʾ���ֶ�ʱ������ַ�����β�Ժ�
	char password[20];
	int peopleCount;
	int age;
	char phone[12];   
} TouristGroup;

/*
	ϵͳ����Ա��Ϣ
*/
typedef struct {
	char id[11];
	char password[20];
	char email[30];
	char phone[12];
} SystemAdmin;

/*
	��������������Ϣ��������������ͬʱ�ε����ƣ�
*/
typedef struct {
	int id;
	char featureSpotId[7];
	int isHot;
	char timeSpanStart[6];
	char timeSpanEnd[6];
	int peopleCount;
} FeatureSpotLimitation;

/*
	������Ϣ
*/
typedef struct {
	char id[7];
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
	float maintenanceFee;
	int reservationCount;
	int visitCount;
	int totalTicketSold;
	int coldSeasonTickets;
	int hotSeasonTickets;
	int totalTicketCount;
	float totalProfit;
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
	char orderDate[18];
	FeatureSpot *featureSpots;
	int featureSpotCount;
	int isHotSeason;
	TouristGroup *touristGroup;
	char time[18];
	TouristIdantity idantity;
	float totalPrice;
} Reservation;