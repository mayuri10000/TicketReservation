/*
	���ݿ�ģ�飬ʹ��sqlite���ݿ�洢��Ŀ���õ������ݣ��ṩ��������ɾ�Ĳ鹦��
	����޸����ڣ�2018-9-13
*/

#include "stdafx.h"
#include "sqlite3.h"               // ��ģ��ʹ�õ����ݿ���ع�����sqlite3���ṩ
#include <io.h>                    // "_access" �������ڵĿ�
#include "Database.h"
#pragma comment(lib, "sqlite3.lib")

#define DB_FILENAME "Database.db"  // ���ݿ��ļ���

sqlite3 *database;
char ErrorMsg[255];

int getDataBy(const char tableName[], char condition[], int sort, const char sortBy[], char ***data, int *columns);

// ��ʼ�����ݿ⣬Ӧ�ڳ���ͷ����
int initializeDatabase() {
	int len;
	char* err = NULL;

	if (_access(DB_FILENAME, 0)) {                                                 // ������ݿ��ļ��Ƿ����
		len = sqlite3_open(DB_FILENAME, &database);                                // �������ݿ�
		if (len) {
			sprintf(ErrorMsg, "�������ݿ�ʧ�ܣ�%s\n", sqlite3_errmsg(database));    // д�������Ϣ
			sqlite3_close(database);
			return -1;
		}

		char sqlCreateTables[] =                                                    // �������sql���
			"CREATE TABLE TouristGroup (id CHAR(10) PRIMARY KEY NOT NULL, password CHAR(20), peopleCount INTEGER , ages  INTEGER, phone CHAR(11) UNIQUE);" // sql����е�UNIQUE������ֶε�ֵ�����ظ�
			"CREATE TABLE SystemAdmin  (id CHAR(10) PRIMARY KEY NOT NULL, password CHAR(20), phone CHAR(11) UNIQUE, email CHAR(30) UNIQUE);"
			"CREATE TABLE FeatureSpot  (id CHAR(6)  PRIMARY KEY NOT NULL, name CHAR(20), discription TEXT, coldSeasonPrice INTEGER, \
				hotSeasonPrice INTEGER, timeRequired INTEGER, soldierDiscount REAL, studentDiscount REAL, level INTEGER, district CHAR(20),\
				maintenanceFee REAL, reservationCount INTEGER, visitCount INTEGER, totalTicketSold INTEGER, coldSeasonTickets INTEGER,\
				hotSeasonTickets INTEGER, totalTicketCount INTEGER, totalProfit REAL, isSuitableForEldersAndChildren INTEGER );"
			"CREATE TABLE FeatureSpotLimitation (id INTEGER PRIMARY KEY AUTOINCREMENT, featureSpotId CHAR(6), isHot INTEGER, timeSpanStart CHAR(5), timeSpanEnd CHAR(5), peopleCount INTEGER);"
			"CREATE TABLE Reservation (id CHAR(30) PRIMARY KEY NOT NULL, orderDate CHAR(18), isHotSeason INTEGER, touristGroupId CHAR(10), time CHAR(18), idantity INTEGER, totalPrice REAL);"
			"CREATE TABLE ReservationFeatureSpot (reservationId CHAR(30), featureSpotId CHAR(10), PRIMARY KEY(reservationId, featureSpotId));";
		                  // ����һ����ϵ�������洢������Ϣ�뾰����Ϣ��һ�Զ��ϵ (����Ϊ�佨���ṹ��)

		sqlite3_exec(database, sqlCreateTables, NULL, NULL, &err);                  // ִ�У�������

		if (err != NULL)
			sprintf(ErrorMsg, "�������ݿ�ʧ�ܣ�%s\n", err);
	}
	else {
		len = sqlite3_open(DB_FILENAME, &database);                                 // �����ݿ��Ѵ��ڣ���ֱ�Ӵ�
		if (len) {
			sprintf(ErrorMsg, "�����ݿ��ļ�ʧ�ܣ�%s\n", sqlite3_errmsg(database));
			sqlite3_close(database);
			return -1;
		}
	}
}

// ����id��ȡ�ο�����Ϣ
TouristGroup *getTouristGroup(char id[]) { // ע��getSystemAdmin, getReservationsBy, getFeatureSpotsBy�����Ļ����ṹ�뱾����������ͬ���������ظ�ע��
	int count = 0;
	char **pResult;
	char condition[32];
	int columns = 0;
	TouristGroup *ret = NULL;

	sprintf(condition, "id = '%s'", id);  // ����sql�������
	count = getDataBy("TouristGroup", condition, 0, NULL, &pResult, &columns);  // ����getDataBy��ѯ��Ϣ
	if (count > 0) {
		ret = (TouristGroup *)malloc(sizeof(TouristGroup));
		int index = columns;						// sqlite3_get_table������������ṹΪ"�ֶ���[1], �ֶ���[2], ..., 
													// ��¼[1][1], ��¼[1][2], ..., ��¼[2][1], ...", ��Ҫ�ӵ�[�ֶ���(����)]��Ԫ�ؿ�ʼ����
		sscanf(pResult[index++], "%s", ret->id);  
		sscanf(pResult[index++], "%s", ret->password);
		sscanf(pResult[index++], "%d", &ret->peopleCount); // ����ÿ���ֶε�ֵ��������ṹ��
		sscanf(pResult[index++], "%d", &ret->age);
		sscanf(pResult[index++], "%s", ret->phone);

		sqlite3_free_table(pResult);
	}

	return ret;
}

// ����id��ȡϵͳ����Ա��Ϣ
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

// ����id��ȡ������Ϣ
FeatureSpot *getFeatureSpot(char id[]) {
	char condition[32];
	FeatureSpot *ret = NULL;

	sprintf(condition, "id = '%s'", id); // ����sql�������
	if (getFeatureSpotsBy(condition, 0, NULL, &ret) >= 1)
		return ret;
	else
		return NULL;
}

// ����id��ȡ������Ϣ
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
	(�ڲ�����) �����ݿ��в�ѯ��Ϣ
	tableName: ����
	condition: ��ѯ������� (sql�����WHERE����Ĳ��֣��� "id = '123456'", �����ѯȫ����¼����ΪNULL)
	sort: �Ƿ����� (0: ������; 1: ��������; 2: ��������)
	sortBy: �����ֶ�����, ����������ΪNULL
	data: �����¼����ָ�� (�����ַ�������)
	columns: ����ָ�룬������ȡ����¼������
	����ֵ: ��ѯ���ļ�¼��
*/
int getDataBy(const char tableName[] ,char condition[], int sort, const char sortBy[], char ***data, int *columns) {
	int count = 0;
	char *err;
	char sql[256];
	char sortStmt[60];
	char whereStmt[60];

	if (condition == NULL)
		sprintf(whereStmt, "");   // ����������Ϊ�գ��򲻲��롰WHERE��, ��ѯ��������
	else
		sprintf(whereStmt, "WHERE %s", condition); // �����������

	if (sort)  // ��������򹹽��������
		sprintf(sortStmt, "ORDER BY %s %s", sortBy, sort > 0 ? "" : "DESC");
	else
		sprintf(sortStmt, "");

	sprintf(sql, "SELECT * FROM %s %s %s", tableName, whereStmt, sortStmt); // ��������sql���
	int re = sqlite3_get_table(database, sql, data, &count, columns, &err);   // ����sqlite3_get_table��ѯ����

	if (re != SQLITE_OK) {     // �����ѯ���ɹ����򽫵�ǰ������Ϣ����ȫ��ErrorMsg������������-1
		sprintf(ErrorMsg, "%s", err);
		return -re;
	}

	return count;              // ��ѯ�ɹ��򷵻ز�ѯ���ļ�¼��
}

/*
	(�ڲ�����) ��ȡ��������������Ϣ
	id: ����ID
	out: ���ָ�룬�봫���Ż�ȡ������Ϣ������(�����ʼ��)
	����ֵ: ��������������Ϣ������
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
	(�ڲ�����) ��ȡһ�ʶ����еľ�����Ϣ
	id: ����ID
	out: ���ָ�룬�봫���Ż�ȡ������Ϣ������(�����ʼ��)
	����ֵ: ��ǰ�����о���ĸ���
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
	(�ڲ�����) ��һ�ʶ�����Ӿ�����Ϣ
	id: ����ID
	in: Ҫ��ӵľ���
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

// ��ѯ������Ϣ
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
			o[row].touristGroup = getTouristGroup(pResult[index++]); // �������ݿ��¼�е����id�ֶΣ���ѯ��������Ϣ��������ṹ��
			sscanf(pResult[index++], "%s",  o[row].time);
			sscanf(pResult[index++], "%d", &o[row].idantity);
			sscanf(pResult[index++], "%f", &o[row].totalPrice);

			o[row].featureSpotCount = getFeatureSpotsForReservation(o[row].id, &o[row].featureSpots);
		}

		sqlite3_free_table(pResult);
	}

	return count;
}

// ��ѯ������Ϣ
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

			o[row].limitationCount = getLimitations(o[row].id, &o[row].limitations); // ��ѯ����������Ϣ��������ṹ��
		}

		sqlite3_free_table(pResult);
	}

	return count;
}

// �����������Ϣ
int addTourstGroup(TouristGroup *val) { // ע��addSystemAdmin��addFeatureSpot��addLimitation��addReservation�����Ļ����ṹ�뱾������ͬ���������ظ�ע��
	char sql[100];
	char *err = NULL;
	int re = 0;

	// ���ݴ���Ľṹ�幹��sql���
	sprintf(sql, "INSERT INTO 'TouristGroup' VALUES( '%s', '%s', %d, %d, '%s' );", val->id, val->password, val->peopleCount, val->age, val->phone);

	re = sqlite3_exec(database, sql, NULL, NULL, &err); // ִ��sql�����Ӽ�¼

	if (re == SQLITE_OK)    // ��ӳɹ�������1
		return 1;

	sprintf(ErrorMsg, "%s", err);  // ʧ�ܣ���������Ϣ����ȫ��ErrorMsg����

	return -re;   // ������ֵ��Ϊ�������Ա�����״̬
}

// ���ϵͳ����Ա��Ϣ
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

// ��Ӿ�����Ϣ
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

// ��Ӿ�������������Ϣ
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

// ��Ӷ�����Ϣ
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

// ��������
int updateData(const char tableName[], char condition[], const char fieldName[], char fieldValue[], int isNumber) {
	char sql[100];
	char value[50];
	char *err = NULL;
	int re = 0;

	sprintf(value, isNumber ? "%s" : "'%s'", fieldValue); // ����������֣�����ϵ�����
	sprintf(sql, "UPDATE '%s' SET %s = %s WHERE %s", tableName, fieldName, value, condition); // ����sql���

	re = sqlite3_exec(database, sql, NULL, NULL, &err); // ִ��

	if (re == SQLITE_OK)           // ���³ɹ�������1
		return 1;

	sprintf(ErrorMsg, "%s", err);  // ʧ�ܣ���������Ϣ����ȫ��ErrorMsg����

	return -re;                    // ������ֵ��Ϊ�������Ա�����״̬
}

// ɾ������
int removeData(const char tableName[], char condition[]) {
	char sql[100];
	char where[50];
	char *err = NULL;
	int re = 0;

	if (condition != NULL)
		sprintf(where, "WHERE %s;", condition); // ���������䲻Ϊ�գ������������
	else
		sprintf(where, ";");                    // �������Ϊ�գ���������WHERE��䣬����ոñ��е���������
	
	sprintf(sql, "DELETE FROM '%s' %s", tableName, where); // ����sql���

	re = sqlite3_exec(database, sql, NULL, NULL, &err);

	if (re == SQLITE_OK)
		return 1;

	sprintf(ErrorMsg, "%s", err);

	return -re;
}

// ���Ψһ��
int checkIsUnique(const char tableName[], const char fieldName[], char fieldValue[]) {
	int count = 0;
	char **pResult;
	char condition[32];
	int columns = 0;

	sprintf(condition, "%s = '%s'", fieldName, fieldValue);
	count = getDataBy(tableName, condition, 0, NULL, &pResult, &columns); // ��ѯ

	return count == 0; // ���ѯ�����Ϊ0�������ݱ��ָ���ֶ��в����ڸ�ֵ������1�����򷵻�0
}

// ��ȡ��ǰ������Ϣ
void getError(char* pOut) {
	sprintf(pOut, "%s\n", ErrorMsg); // ��ȡ��ǰ�洢��ȫ�ִ�����Ϣ�����е��ַ���
}

// �ر����ݿ⣬Ӧ�ڳ����β������
void closeDatabase() {
	sqlite3_close(database);
}