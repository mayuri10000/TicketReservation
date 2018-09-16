/*
	数据库模块，使用sqlite数据库存储项目中用到的数据，提供基本的增删改查功能
	最后修改日期：2018-9-13
*/

#include "stdafx.h"
#include "sqlite3.h"               // 本模块使用的数据库相关功能由sqlite3库提供
#include <io.h>                    // "_access" 函数所在的库
#include "Database.h"
#pragma comment(lib, "sqlite3.lib")

#define DB_FILENAME "Database.db"  // 数据库文件名

sqlite3 *database;
char ErrorMsg[255];

int getDataBy(const char tableName[], char condition[], int sort, const char sortBy[], char ***data, int *columns);

// 初始化数据库，应在程序开头调用
int initializeDatabase() {
	int len;
	char* err = NULL;

	if (_access(DB_FILENAME, 0)) {                                                 // 检查数据库文件是否存在
		len = sqlite3_open(DB_FILENAME, &database);                                // 创建数据库
		if (len) {
			sprintf(ErrorMsg, "创建数据库失败：%s\n", sqlite3_errmsg(database));    // 写入错误信息
			sqlite3_close(database);
			return -1;
		}

		char sqlCreateTables[] =                                                    // 创建表的sql语句
			"CREATE TABLE TouristGroup (id CHAR(10) PRIMARY KEY NOT NULL, password CHAR(20), peopleCount INTEGER , ages  INTEGER, phone CHAR(11) UNIQUE);" // sql语句中的UNIQUE代表该字段的值不能重复
			"CREATE TABLE SystemAdmin  (id CHAR(10) PRIMARY KEY NOT NULL, password CHAR(20), phone CHAR(11) UNIQUE, email CHAR(30) UNIQUE);"
			"CREATE TABLE FeatureSpot  (id CHAR(6)  PRIMARY KEY NOT NULL, name CHAR(20), discription TEXT, coldSeasonPrice INTEGER, \
				hotSeasonPrice INTEGER, timeRequired INTEGER, soldierDiscount REAL, studentDiscount REAL, level INTEGER, district CHAR(20),\
				maintenanceFee REAL, reservationCount INTEGER, visitCount INTEGER, totalTicketSold INTEGER, coldSeasonTickets INTEGER,\
				hotSeasonTickets INTEGER, totalTicketCount INTEGER, totalProfit REAL, isSuitableForEldersAndChildren INTEGER );"
			"CREATE TABLE FeatureSpotLimitation (id INTEGER PRIMARY KEY AUTOINCREMENT, featureSpotId CHAR(6), isHot INTEGER, timeSpanStart CHAR(5), timeSpanEnd CHAR(5), peopleCount INTEGER);"
			"CREATE TABLE Reservation (id CHAR(30) PRIMARY KEY NOT NULL, orderDate CHAR(18), isHotSeason INTEGER, touristGroupId CHAR(10), time CHAR(18), idantity INTEGER, totalPrice REAL);"
			"CREATE TABLE ReservationFeatureSpot (reservationId CHAR(30), featureSpotId CHAR(10), PRIMARY KEY(reservationId, featureSpotId));";
		                  // 这是一个关系表，用来存储订单信息与景点信息的一对多关系 (无需为其建立结构体)

		sqlite3_exec(database, sqlCreateTables, NULL, NULL, &err);                  // 执行，创建表

		if (err != NULL)
			sprintf(ErrorMsg, "创建数据库失败：%s\n", err);
	}
	else {
		len = sqlite3_open(DB_FILENAME, &database);                                 // 如数据库已存在，则直接打开
		if (len) {
			sprintf(ErrorMsg, "打开数据库文件失败：%s\n", sqlite3_errmsg(database));
			sqlite3_close(database);
			return -1;
		}
	}
}

// 根据id获取游客团信息
TouristGroup *getTouristGroup(char id[]) { // 注：getSystemAdmin, getReservationsBy, getFeatureSpotsBy函数的基本结构与本函数部分相同，将不再重复注释
	int count = 0;
	char **pResult;
	char condition[32];
	int columns = 0;
	TouristGroup *ret = NULL;

	sprintf(condition, "id = '%s'", id);  // 构建sql条件语句
	count = getDataBy("TouristGroup", condition, 0, NULL, &pResult, &columns);  // 调用getDataBy查询信息
	if (count > 0) {
		ret = (TouristGroup *)malloc(sizeof(TouristGroup));
		int index = columns;						// sqlite3_get_table函数返回数组结构为"字段名[1], 字段名[2], ..., 
													// 记录[1][1], 记录[1][2], ..., 记录[2][1], ...", 故要从第[字段数(列数)]个元素开始解析
		sscanf(pResult[index++], "%s", ret->id);  
		sscanf(pResult[index++], "%s", ret->password);
		sscanf(pResult[index++], "%d", &ret->peopleCount); // 解析每个字段的值，并填入结构体
		sscanf(pResult[index++], "%d", &ret->age);
		sscanf(pResult[index++], "%s", ret->phone);

		sqlite3_free_table(pResult);
	}

	return ret;
}

// 根据id获取系统管理员信息
SystemAdmin *getSystemAdmin(char id[]) {
	int count = 0;
	char **pResult;
	char condition[32];
	int columns = 0;
	SystemAdmin *ret = NULL;

	sprintf(condition, "id = '%s'", id);
	count = getDataBy("SystemAdmin", condition, 0, NULL, &pResult, &columns);
	if (count > 0) {
		ret = (SystemAdmin *)malloc(sizeof(SystemAdmin));
		int index = columns;
		
		sscanf(pResult[index++], "%s", ret->id);
		sscanf(pResult[index++], "%s", ret->password);
		sscanf(pResult[index++], "%s", ret->phone);
		sscanf(pResult[index++], "%s", ret->email);

		sqlite3_free_table(pResult);
	}

	return ret;
}

// 根据id获取景点信息
FeatureSpot *getFeatureSpot(char id[]) {
	char condition[32];
	FeatureSpot *ret = NULL;

	sprintf(condition, "id = '%s'", id); // 构建sql条件语句
	if (getFeatureSpotsBy(condition, 0, NULL, &ret) >= 1)
		return ret;
	else
		return NULL;
}

// 根据id获取订单信息
Reservation *getReservation(char id[]) {
	char condition[32];
	Reservation *ret = NULL;
	
	sprintf(condition, "id = '%s'", id);
	if (getReservationsBy(condition, 0, NULL, &ret) >= 1)
		return ret;
	else
		return NULL;
}

/*
	(内部函数) 从数据库中查询信息
	tableName: 表名
	condition: 查询条件语句 (sql语句中WHERE后面的部分，如 "id = '123456'", 如需查询全部记录请设为NULL)
	sort: 是否排序 (0: 不排序; 1: 升序排序; 2: 降序排序)
	sortBy: 排序字段名称, 不排序请设为NULL
	data: 输出记录数组指针 (传出字符串数组)
	columns: 列数指针，传出获取到记录的列数
	返回值: 查询到的记录数
*/
int getDataBy(const char tableName[] ,char condition[], int sort, const char sortBy[], char ***data, int *columns) {
	int count = 0;
	char *err;
	char sql[256];
	char sortStmt[60];
	char whereStmt[60];

	if (condition == NULL)
		sprintf(whereStmt, "");   // 如果条件语句为空，则不插入“WHERE”, 查询所有数据
	else
		sprintf(whereStmt, "WHERE %s", condition); // 插入条件语句

	if (sort)  // 如果排序，则构建排序语句
		sprintf(sortStmt, "ORDER BY %s %s", sortBy, sort > 0 ? "" : "DESC");
	else
		sprintf(sortStmt, "");

	sprintf(sql, "SELECT * FROM %s %s %s", tableName, whereStmt, sortStmt); // 构建完整sql语句
	int re = sqlite3_get_table(database, sql, data, &count, columns, &err);   // 调用sqlite3_get_table查询数据

	if (re != SQLITE_OK) {     // 如果查询不成功，则将当前错误信息存入全局ErrorMsg变量，并返回-1
		sprintf(ErrorMsg, "%s", err);
		return -re;
	}

	return count;              // 查询成功则返回查询到的记录数
}

/*
	(内部函数) 获取景点人数限制信息
	id: 景点ID
	out: 输出指针，请传入存放获取到的信息的数组(无需初始化)
	返回值: 景点人数限制信息的条数
*/
int getLimitations(char id[], FeatureSpotLimitation **out)
{
	int count = 0;
	char **pResult;
	char condition[32];
	int columns = 0;

	sprintf(condition, "featureSpotId = '%s'", id);
	count = getDataBy("FeatureSpotLimitation", condition, 0, NULL, &pResult, &columns);
	if (count > -1) {
		*out = (FeatureSpotLimitation *)malloc(sizeof(FeatureSpotLimitation) * count);
		FeatureSpotLimitation *o = *out;
		int index = columns;
		for (int row = 0; row != count; row++) {
			FeatureSpotLimitation l;
			sscanf(pResult[index++], "%d", &l.id);
			sscanf(pResult[index++], "%s",  l.featureSpotId);
			sscanf(pResult[index++], "%d", &l.isHot);
			sscanf(pResult[index++], "%s",  l.timeSpanStart);
			sscanf(pResult[index++], "%s",  l.timeSpanEnd);
			sscanf(pResult[index++], "%d", &l.peopleCount);
			o[row] = l;
		}

		sqlite3_free_table(pResult);

		return count;
	}
}

/*
	(内部函数) 获取一笔订单中的景点信息
	id: 订单ID
	out: 输出指针，请传入存放获取到的信息的数组(无需初始化)
	返回值: 当前订单中景点的个数
*/
int getFeatureSpotsForReservation(char id[], FeatureSpot **out) 
{
	int count = 0;
	char **pResult;
	char condition[50];
	char tId[10];
	int columns = 0;

	sprintf(condition, "reservationId = '%s'", id);
	count = getDataBy("ReservationFeatureSpot", condition, 0, NULL, &pResult, &columns);
	if (count > -1) {
		*out = (FeatureSpot *)malloc(sizeof(FeatureSpot) * count);
		int index = columns;
		for (int row = 0; row != count; row++) {
			index++;
			sscanf(pResult[index++], "%s", tId);
			out[row] = getFeatureSpot(tId);
		}

		sqlite3_free_table(pResult);

		return count;
	}
}

/*
	(内部函数) 向一笔订单添加景点信息
	id: 订单ID
	in: 要添加的景点
*/
int addFeatureSpotsForReservation(char id[], FeatureSpot *in) {
	char sql[100];
	char *err = NULL;
	int re = 0;

	sprintf(sql, "INSERT INTO 'ReservationFeatureSpot' VALUES( '%s', '%s' );", id, in->id);

	re = sqlite3_exec(database, sql, NULL, NULL, &err); 

	if (re == SQLITE_OK)    
		return 1;

	sprintf(ErrorMsg, "%s", err);  

	return -re;   
}

// 查询订单信息
int getReservationsBy(char condition[], int sort, const char sortBy[], Reservation** out)
{
	int count = 0;
	char **pResult;
	
	int columns = 0;
	count = getDataBy("Reservation", condition, sort, sortBy, &pResult, &columns);
	if (count > -1) {
		*out = (Reservation *)malloc(sizeof(Reservation) * count);
		Reservation *o = *out;
		int index = columns;
		for (int row = 0; row != count; row++) {
			sscanf(pResult[index++], "%s",  o[row].id);
			sscanf(pResult[index++], "%s",  o[row].orderDate);
			sscanf(pResult[index++], "%d", &o[row].isHotSeason);
			o[row].touristGroup = getTouristGroup(pResult[index++]); // 根据数据库记录中的相关id字段，查询旅游团信息，并填入结构体
			sscanf(pResult[index++], "%s",  o[row].time);
			sscanf(pResult[index++], "%d", &o[row].idantity);
			sscanf(pResult[index++], "%f", &o[row].totalPrice);

			o[row].featureSpotCount = getFeatureSpotsForReservation(o[row].id, &o[row].featureSpots);
		}

		sqlite3_free_table(pResult);
	}

	return count;
}

// 查询景点信息
int getFeatureSpotsBy(char condition[], int sort, const char sortBy[], FeatureSpot** out) {
	int count = 0;
	char **pResult;

	int columns = 0;
	count = getDataBy("FeatureSpot", condition, sort, sortBy, &pResult, &columns);
	if (count > -1) {
		*out = (FeatureSpot *)malloc(sizeof(FeatureSpot) * count);
		FeatureSpot *o = *out;
		int index = columns;
		for (int row = 0; row != count; row++) {
			sscanf(pResult[index++], "%s",  o[row].id);
			sscanf(pResult[index++], "%s",  o[row].name);
			sscanf(pResult[index++], "%s",  o[row].discription);
			sscanf(pResult[index++], "%d", &o[row].coldSeasonPrice);
			sscanf(pResult[index++], "%d", &o[row].hotSeasonPrice);
			sscanf(pResult[index++], "%d", &o[row].timeRequired);
			sscanf(pResult[index++], "%f", &o[row].soldierDiscount);
			sscanf(pResult[index++], "%f", &o[row].studentDiscount);
			sscanf(pResult[index++], "%d", &o[row].level);
			sscanf(pResult[index++], "%s",  o[row].district);
			sscanf(pResult[index++], "%f", &o[row].maintenanceFee);
			sscanf(pResult[index++], "%d", &o[row].reservationCount);
			sscanf(pResult[index++], "%d", &o[row].visitCount);
			sscanf(pResult[index++], "%d", &o[row].totalTicketSold);
			sscanf(pResult[index++], "%d", &o[row].coldSeasonTickets);
			sscanf(pResult[index++], "%d", &o[row].hotSeasonTickets);
			sscanf(pResult[index++], "%d", &o[row].totalTicketCount);
			sscanf(pResult[index++], "%f", &o[row].totalProfit);
			sscanf(pResult[index++], "%d", &o[row].isSuitableForEldersAndChildren);

			o[row].limitationCount = getLimitations(o[row].id, &o[row].limitations); // 查询人数限制信息，并填入结构体
		}

		sqlite3_free_table(pResult);
	}

	return count;
}

// 添加旅游团信息
int addTourstGroup(TouristGroup *val) { // 注：addSystemAdmin，addFeatureSpot，addLimitation，addReservation函数的基本结构与本函数相同，将不再重复注释
	char sql[100];
	char *err = NULL;
	int re = 0;

	// 根据传入的结构体构建sql语句
	sprintf(sql, "INSERT INTO 'TouristGroup' VALUES( '%s', '%s', %d, %d, '%s' );", val->id, val->password, val->peopleCount, val->age, val->phone);

	re = sqlite3_exec(database, sql, NULL, NULL, &err); // 执行sql语句添加记录

	if (re == SQLITE_OK)    // 添加成功，返回1
		return 1;

	sprintf(ErrorMsg, "%s", err);  // 失败，将错误信息存入全局ErrorMsg变量

	return -re;   // 将返回值变为负数，以便区分状态
}

// 添加系统管理员信息
int addSystemAdmin(SystemAdmin *val) {
	char sql[128];
	char *err = NULL;
	int re = 0;

	sprintf(sql, "INSERT INTO 'SystemAdmin' VALUES( '%s', '%s', '%s', '%s' );", val->id, val->password, val->phone, val->email);

	re = sqlite3_exec(database, sql, NULL, NULL, &err);

	if (re == SQLITE_OK)
		return 1;

	sprintf(ErrorMsg, "%s", err);

	return -re;
}

// 添加景点信息
int addFeatureSpot(FeatureSpot *val) {
	char sql[1024];
	char *err = NULL;
	int re = 0;

	sprintf(sql, "INSERT INTO 'FeatureSpot' VALUES('%s', '%s', '%s', %d, %d, %d, %f, %f, %d, '%s', %f, 0, 0, 0, 0, 0, %d, %f, %d);",
		val->id, val->name, val->discription, val->coldSeasonPrice, val->hotSeasonPrice, val->timeRequired,
		val->soldierDiscount, val->studentDiscount, val->level, val->district, val->maintenanceFee, val->totalTicketCount,
	    val->totalProfit ,val->isSuitableForEldersAndChildren);

	re = sqlite3_exec(database, sql, NULL, NULL, &err);

	for (int i = 0; i != val->limitationCount; i++)
		addLimitation(&val->limitations[i]);

	if (re == SQLITE_OK)
		return 1;

	sprintf(ErrorMsg, "%s", err);

	return -re;
}

// 添加景点人数限制信息
int addLimitation(FeatureSpotLimitation *val) {
	char sql[1024];
	char *err = NULL;
	int re = 0;

	sprintf(sql, "INSERT INTO 'FeatureSpotLimitation' VALUES( NULL, '%s', %d, '%s', '%s', %d );",
		val->featureSpotId, val->isHot, val->timeSpanStart, val->timeSpanEnd, val->peopleCount);

	re = sqlite3_exec(database, sql, NULL, NULL, &err);

	if (re == SQLITE_OK)
		return 1;

	sprintf(ErrorMsg, "%s", err);

	return -re;
}

// 添加订单信息
int addReservation(Reservation *val) {
	char sql[256];
	char *err = NULL;
	int re = 0;

	sprintf(sql, "INSERT INTO 'Reservation' VALUES( '%s', '%s', %d, '%s', '%s', %d, %f );",
		val->id, val->orderDate, val->isHotSeason, val->touristGroup->id, val->time, val->idantity, val->totalPrice);

	re = sqlite3_exec(database, sql, NULL, NULL, &err);

	for (int i = 0; i != val->featureSpotCount; i++)
		addFeatureSpotsForReservation(val->id, &val->featureSpots[i]);

	if (re == SQLITE_OK)
		return 1;

	sprintf(ErrorMsg, "%s", err);

	return -re;
}

// 更新数据
int updateData(const char tableName[], char condition[], const char fieldName[], char fieldValue[], int isNumber) {
	char sql[100];
	char value[50];
	char *err = NULL;
	int re = 0;

	sprintf(value, isNumber ? "%s" : "'%s'", fieldValue); // 如果不是数字，则加上单引号
	sprintf(sql, "UPDATE '%s' SET %s = %s WHERE %s", tableName, fieldName, value, condition); // 构建sql语句

	re = sqlite3_exec(database, sql, NULL, NULL, &err); // 执行

	if (re == SQLITE_OK)           // 更新成功，返回1
		return 1;

	sprintf(ErrorMsg, "%s", err);  // 失败，将错误信息存入全局ErrorMsg变量

	return -re;                    // 将返回值变为负数，以便区分状态
}

// 删除数据
int removeData(const char tableName[], char condition[]) {
	char sql[100];
	char where[50];
	char *err = NULL;
	int re = 0;

	if (condition != NULL)
		sprintf(where, "WHERE %s;", condition); // 如果条件语句不为空，构建条件语句
	else
		sprintf(where, ";");                    // 条件语句为空，将不插入WHERE语句，将清空该表中的所有数据
	
	sprintf(sql, "DELETE FROM '%s' %s", tableName, where); // 构建sql语句

	re = sqlite3_exec(database, sql, NULL, NULL, &err);

	if (re == SQLITE_OK)
		return 1;

	sprintf(ErrorMsg, "%s", err);

	return -re;
}

// 检查唯一性
int checkIsUnique(const char tableName[], const char fieldName[], char fieldValue[]) {
	int count = 0;
	char **pResult;
	char condition[32];
	int columns = 0;

	sprintf(condition, "%s = '%s'", fieldName, fieldValue);
	count = getDataBy(tableName, condition, 0, NULL, &pResult, &columns); // 查询

	return count == 0; // 如查询结果数为0，即数据表的指定字段中不存在该值，返回1，否则返回0
}

// 获取当前错误信息
void getError(char* pOut) {
	sprintf(pOut, "%s\n", ErrorMsg); // 获取当前存储在全局错误信息变量中的字符串
}

// 关闭数据库，应在程序结尾处调用
void closeDatabase() {
	sqlite3_close(database);
}