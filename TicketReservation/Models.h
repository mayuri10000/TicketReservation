#pragma once

typedef struct {
	char id[10];
	char password[20];
	int peopleCount;
	int ages[50];
	char phone[11];
} TouristGroup;

typedef struct {
	char id[10];
	char password[20];
	char phone[11];
	char email[30];
} SystemAdmin;

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
	FeatureSpotLimitation *limitation;
	int limitationCount;
	char district[20];
	int maintenanceFee;
} FeatureSpot;

typedef struct {
	char featureSpotId[6];
	int isHot;
	int timeSpanStart;
	int timeSpanEnd;
	int peopleCount;
} FeatureSpotLimitation;

typedef enum {
	NORMAL = 0,
	CHILD = 1,
	ELDER = 2,
	STUDENT = 3,
	SOLDIER = 4,
} TouristIdantity;

typedef struct {
	char id[30];
	char orderDate[10];
	FeatureSpot *featureSpot;
	TouristGroup *touristGroup;
	char time[16];
	int ages[20];
	TouristIdantity idantity;
	float totalPrice;
} Reservation;