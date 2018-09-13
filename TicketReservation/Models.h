/*
	结构体模块，定义了项目中用到的不同类型的数据的结构体
	作者：刘同 2017212783
	最后修改日期：2018-9-13
*/

#pragma once

/*
	游客团信息
*/
typedef struct {
	char id[10];
	char password[20];
	int peopleCount;
	int age;
	char phone[11];
} TouristGroup;

/*
	系统管理员信息
*/
typedef struct {
	char id[10];
	char password[20];
	char phone[11];
	char email[30];
} SystemAdmin;

/*
	景点人流限制信息（旺季、淡季不同时段的限制）
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
	景点信息
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
	游客身份
*/
typedef enum {
	NORMAL = 0,
	STUDENT = 1,
	SOLDIER = 2,
} TouristIdantity;

/*
	订单信息
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