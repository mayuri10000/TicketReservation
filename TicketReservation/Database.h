/*
	���ݿ�ģ�鵼���������ṩ�������ݿ����ɾ�Ĳ鹦��
*/

#pragma once
#include "Models.h"

/*
	��ʼ�����ݿ⣬Ӧ���ڳ���ͷ����
*/
int initializeDatabase();

/*
	����id��ȡ��������Ϣ
*/
TouristGroup *getTouristGroup(char id[]);

/*
	����id��ȡϵͳ����Ա��Ϣ
*/
SystemAdmin *getSystemAdmin(char id[]);

/*
	����id��ȡ������Ϣ
*/
FeatureSpot *getFeatureSpot(char id[]);

/*
	����id��ȡ������Ϣ
*/
Reservation *getReservation(char id[]);

/*
	��ѯ������Ϣ
	condition: ��ѯ������� (�� "id = '123456'", �����ѯȫ����¼����ΪNULL)
	sort: �Ƿ����� (0: ������; 1: ��������; 2: ��������)
	sortBy: �����ֶ�����, ����������ΪNULL
	out: �������ָ��, ������ѯ���Ķ�����Ϣ����
	����ֵ: ���ز�ѯ���Ķ�������
*/
int getReservationsBy(char condition[], int sort, const char sortBy[], Reservation** out);

/*
	��ѯ������Ϣ
	condition: ��ѯ������� (�� "id = '123456'", �����ѯȫ����¼����ΪNULL)
	sort: �Ƿ����� (0: ������; 1: ��������; 2: ��������)
	sortBy: �����ֶ�����, ����������ΪNULL
	out: �������ָ��, ������ѯ���ľ�����Ϣ����
	����ֵ: ���ز�ѯ���ľ������
*/
int getFeatureSpotsBy(char condition[], int sort, const char sortBy[], FeatureSpot** out);

/*
	����ο�����Ϣ
*/
int addTourstGroup(TouristGroup *val);

/*
	���ϵͳ����Ա��Ϣ
*/
int addSystemAdmin(SystemAdmin *val);

/*
	��Ӿ�����Ϣ
*/
int addFeatureSpot(FeatureSpot *val);

/*
	��Ӿ�������������Ϣ
*/
int addLimitation(FeatureSpotLimitation *val);

/*
	��Ӷ�����Ϣ
*/
int addReservation(Reservation *val);

/*
	��������
	tableName: Ҫ���µ��������ڱ���
	condition: ��ѯ���� (�� "id = '123456'")
	fieldName: Ҫ���µ��ֶ�����
	fieldValue:Ҫ���µ��ֶε�ֵ
	isNumber : ���ֶ��Ƿ�Ϊ����
*/
int updateData(const char tableName[], char condition[], const char fieldName[], char fieldValue[], int isNumber);

/*
	ɾ������
	tableName: Ҫɾ�����������ڱ���
	condition: ɾ������ (�� "id = '123456'", ��ΪNULL�������������)
*/
int removeData(const char tableName[], char condition[]);

/*
	���Ψһ��
	����ֵ: ����ڱ���Ϊ tableName �ı��д����ֶ� fieldName ��ֵΪ fieldValue �ļ�¼������0�����򷵻�1
*/
int checkIsUnique(const char tableName[], const char fieldName[], char fieldValue[]);

/*
	��ȡ��ǰ������Ϣ
*/
void getError(char* pOut);

/*
	�ر����ݿ⣬���ڳ����β������
*/
void closeDatabase();