/*
	结构体模块，定义了项目中用到的不同类型的数据的结构体
*/

#pragma once

/*
	游客团信息
*/
typedef struct {
	char id[11];     // 对于定长字符串，要多给一字节，否则在显示该字段时会读到字符串结尾以后
	char password[20];
	int peopleCount;
	int age;
	char phone[12];   
} TouristGroup;

/*
	系统管理员信息
*/
typedef struct {
	char id[11];
	char password[20];
	char email[30];
	char phone[12];
} SystemAdmin;

/*
	景点人流限制信息（旺季、淡季不同时段的限制）
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
	景点信息
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
	char orderDate[18];
	FeatureSpot *featureSpots;
	int featureSpotCount;
	int isHotSeason;
	TouristGroup *touristGroup;
	char time[18];
	TouristIdantity idantity;
	float totalPrice;
} Reservation;