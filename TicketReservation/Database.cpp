#include "stdafx.h"
#include <stdlib.h>
#include "sqlite3.h"
#include <io.h>
#include "Database.h"
#pragma comment(lib, "sqlite3.lib")

#define DB_FILENAME "Database.db"

sqlite3 *database;
char ErrorMsg[255];

int initializeDatabase() {
	int len;
	char* err = NULL;

	if (_access(DB_FILENAME, 0)) {
		len = sqlite3_open(DB_FILENAME, &database);
		if (len) {
			sprintf(ErrorMsg, "创建数据库失败：%s\n", sqlite3_errmsg(database));
			sqlite3_close(database);
			return -1;
		}

		char sqlCreateTables[] = 
			"CREATE TABLE TouristGroup (id CHAR(10) PRIMARY KEY NOT NULL, password CHAR(20), peopleCount INTEGER , ages  BLOB, phone CHAR(11));"
			"CREATE TABLE SystemAdmin  (id CHAR(10) PRIMARY KEY NOT NULL, password CHAR(20), phone       CHAR(11), email CHAR(11) UNIQUE);"
			"CREATE TABLE FeatureSpot  (id CHAR(6)  PRIMARY KEY NOT NULL, name CHAR(20), discription TEXT, coldSeasonPrice INTEGER, \
				hotSeasonPrice INTEGER, timeRequired INTEGER, solderDiscount REAL, studentDiscount REAL, level INTEGER, district CHAR(20), maintenanceFee INTEGER);"
			"CREATE TABLE FeatureSpotLimitation (id INTEGER PRIMARY KEY AUTOINCREMENT, featureSpotId CHAR(6), isHot INTEGER, timeSpanStart INTEGER, timeSpanEnd INTEGER, peopleCount INTEGER);"
			"CREATE TABLE Reservation (id CHAR(30) PRIMARY KEY NOT NULL, orderDate CHAR(10), featureSpotId CHAR(6), time CHAR(16), ages BLOB, idantity BLOB, totalPrice REAL);";

		sqlite3_exec(database, sqlCreateTables, NULL, NULL, &err);

		if (err != NULL)
			sprintf(ErrorMsg, "%s", err);
	}
}

void getError(char* pOut) {
	sprintf(pOut, "%s\n", ErrorMsg);
}