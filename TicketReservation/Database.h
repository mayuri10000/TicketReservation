/*
	数据库模块导出函数，提供基本数据库的增删改查功能
*/

#pragma once
#include "Models.h"

/*
	初始化数据库，应该在程序开头调用
*/
int initializeDatabase();

/*
	根据id获取旅游团信息
*/
TouristGroup *getTouristGroup(char id[]);

/*
	根据id获取系统管理员信息
*/
SystemAdmin *getSystemAdmin(char id[]);

/*
	根据id获取景点信息
*/
FeatureSpot *getFeatureSpot(char id[]);

/*
	根据id获取订单信息
*/
Reservation *getReservation(char id[]);

/*
	查询订单信息
	condition: 查询条件语句 (如 "id = '123456'", 如需查询全部记录请设为NULL)
	sort: 是否排序 (0: 不排序; 1: 升序排序; 2: 降序排序)
	sortBy: 排序字段名称, 不排序请设为NULL
	out: 输出数组指针, 传出查询到的订单信息数组
	返回值: 返回查询到的订单个数
*/
int getReservationsBy(char condition[], int sort, const char sortBy[], Reservation** out);

/*
	查询景点信息
	condition: 查询条件语句 (如 "id = '123456'", 如需查询全部记录请设为NULL)
	sort: 是否排序 (0: 不排序; 1: 升序排序; 2: 降序排序)
	sortBy: 排序字段名称, 不排序请设为NULL
	out: 输出数组指针, 传出查询到的景点信息数组
	返回值: 返回查询到的景点个数
*/
int getFeatureSpotsBy(char condition[], int sort, const char sortBy[], FeatureSpot** out);

/*
	添加游客团信息
*/
int addTourstGroup(TouristGroup *val);

/*
	添加系统管理员信息
*/
int addSystemAdmin(SystemAdmin *val);

/*
	添加景点信息
*/
int addFeatureSpot(FeatureSpot *val);

/*
	添加景点人数限制信息
*/
int addLimitation(FeatureSpotLimitation *val);

/*
	添加订单信息
*/
int addReservation(Reservation *val);

/*
	更新数据
	tableName: 要更新的数据所在表名
	condition: 查询条件 (如 "id = '123456'")
	fieldName: 要更新的字段名称
	fieldValue:要更新的字段的值
	isNumber : 该字段是否为数字
*/
int updateData(const char tableName[], char condition[], const char fieldName[], char fieldValue[], int isNumber);

/*
	删除数据
	tableName: 要删除的数据所在表名
	condition: 删除条件 (如 "id = '123456'", 设为NULL将会清空整个表)
*/
int removeData(const char tableName[], char condition[]);

/*
	检查唯一性
	返回值: 如果在表名为 tableName 的表中存在字段 fieldName 的值为 fieldValue 的记录，返回0，否则返回1
*/
int checkIsUnique(const char tableName[], const char fieldName[], char fieldValue[]);

/*
	获取当前错误信息
*/
void getError(char* pOut);

/*
	关闭数据库，请在程序结尾处调用
*/
void closeDatabase();