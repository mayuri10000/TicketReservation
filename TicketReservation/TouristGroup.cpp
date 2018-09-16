/*
	旅游团功能模块
*/

#include "stdafx.h"
#include "Color.h"
#include "Models.h"
#include "Validation.h"
#include "Database.h"
#include "TouristGroup.h"

TouristGroup currentUser;

void QueryAndOrder();
void MakeReservation(char *id);
int isHotSeason();
void TouristGroupMenu();
void ReservationManagetment();
void ModifyInfo();

void TouristGroupLogin() {
	system("cls");
	printf("游客团登录\n");
	printf("======================\n\n");

	while (1) {
		char userName[10];
		char password[30];
		inputStringWithLengthLimit("旅游团ID:", 6, 10, userName);
		inputPassword("密码:", password);

		TouristGroup *user = getTouristGroup(userName);
		if (user == NULL || strcmp(user->password, password)) 
			printf(COLOR_RED_B "ID或密码错误, 请重试\n" COLOR_RESET);
		else {
			currentUser = *user;
			TouristGroupMenu();
			break;
		}
	}
}

void TouristGroupMenu() {
	while (1) {
		system("cls");
		printf("当前旅游团: " COLOR_GREEN_B "%s\n" COLOR_RESET, currentUser.id);
		printf("┌──────────────────────────────────────────────────┐\n");
		printf("│              团体游景点门票预订系统              │\n");
		printf("├──────────────────────────────────────────────────┤\n");
		printf("│               " COLOR_YELLOW_B "[1]" COLOR_RESET " 景点查询及预定                 │\n");
		printf("│               " COLOR_YELLOW_B "[2]" COLOR_RESET " 订单管理                       │\n");
		printf("│               " COLOR_YELLOW_B "[3]" COLOR_RESET " 旅游团信息修改                 │\n");
		printf("│               " COLOR_YELLOW_B "[4]" COLOR_RESET " 退出登录                       │\n");
		printf("└──────────────────────────────────────────────────┘\n");
		printf(COLOR_YELLOW_B "请按数字键选择功能\n" COLOR_RESET);

		char choice = '\0';
		while (!(choice = readKey()) || choice > '5' || choice < '1')
			printf(COLOR_RED_B "您的按键有误, 请重试\n" COLOR_RESET);

		switch (choice)
		{
			case '1':
				QueryAndOrder();
				break;
			case '2':
				ReservationManagetment();
				break;
			case '3':
				ModifyInfo();
				break;
			default:
				return;
		}
	}
}

void QueryAndOrder() {
	system("cls");
	printf("景点查询与预定\n");
	printf("==================\n\n");

	printf(COLOR_YELLOW_B "[1]" COLOR_RESET "查看所有景点\n");
	printf(COLOR_YELLOW_B "[2]" COLOR_RESET "根据名称查询景点\n");
	printf(COLOR_YELLOW_B "[3]" COLOR_RESET "只查看有票的景点\n");
	printf(COLOR_YELLOW_B "[4]" COLOR_RESET "根据区域查询景点\n");
	printf(COLOR_YELLOW_B "[5]" COLOR_RESET "只查看有折扣的景点\n\n");

	printf(COLOR_YELLOW_B "请按数字键选择功能\n" COLOR_RESET);

	char choice = '\0';
	while (!(choice = readKey()) || choice > '5' || choice < '1')
		printf(COLOR_RED_B "您的按键有误, 请重试\n" COLOR_RESET);

	char *pCondition = NULL;
	char condition[50];
	const char *orderBy = NULL;
	int order = 0;

	switch (choice) {
	case '1':
		break;
	case '2':
		char keyword[20];
		inputStringWithLengthLimit("请输入搜索关键词:", 1, 20, keyword);
		sprintf(condition, "name LIKE '%%%s%%'", keyword);  // 在c语言的格式字符串中, 要想输出一个 %, 用两个即可
		pCondition = condition;
		break;
	case '3':
		sprintf(condition, "totalTicketSold < totalTicketCount");
		pCondition = condition;
		break;
	case '4':
		char region[20];
		inputStringWithLengthLimit("请输入区域:", 1, 20, region);
		sprintf(condition, "district = '%s'", region);
		pCondition = condition;
		break;
	case '5':
		sprintf(condition, "solderDiscount <> 1.0 OR studentDiscount <> 1.0");
		pCondition = condition;
		break;
	default:
		break;
	}
	printf(COLOR_YELLOW_B "\n请选择排序类型\n" COLOR_RESET);
	printf(COLOR_YELLOW_B "[1]" COLOR_RESET "不排序\n");
	printf(COLOR_YELLOW_B "[2]" COLOR_RESET "价格由低到高\n");
	printf(COLOR_YELLOW_B "[3]" COLOR_RESET "价格由高到低\n");
	printf(COLOR_YELLOW_B "[4]" COLOR_RESET "预订量由高到低\n");
	printf(COLOR_YELLOW_B "[5]" COLOR_RESET "预订量由低到高\n");

	while (!(choice = readKey()) || choice > '5' || choice < '1')
		printf(COLOR_RED_B "您的按键有误, 请重试\n" COLOR_RESET);

	int isHot = isHotSeason();

	switch (choice) {
	case '1':
		break;
	case '2':
		order = -1;
		orderBy = isHot ? "hotSeasonPrice" : "coldSeasonPrice";
		break;
	case '3':
		order = 1;
		orderBy = isHot ? "hotSeasonPrice" : "coldSeasonPrice";
		break;
	case '4':
		order = 1;
		orderBy = "reservationCount";
		break;
	case '5':
		order = -1;
		orderBy = "reservationCount";
		break;
	default:
		break;
	}

	FeatureSpot *featureSpots;
	int count = getFeatureSpotsBy(pCondition, order, orderBy, &featureSpots);

	if (count < 0) {
		char err[100];
		getError(err);
		printf(COLOR_RED_B "数据库错误: %s (错误码: %d)\n" COLOR_RESET, err, -count);
		printf("按回车键返回主菜单...\n");
		while (readKey() != '\r');
		return;
	}
	
	system("cls");
	printf("ID         名称         地区   总预订量   当季价格   剩余票量   折扣\n");
	printf("=======================================================================================\n");
	for (int i = 0; i != count; i++) {
		FeatureSpot f = featureSpots[i];
		printf("%-11s%-13s%-6s%7d%10d%10d        %s    %s\n", f.id, f.name, f.district, f.reservationCount,
			isHot ? f.hotSeasonPrice : f.coldSeasonPrice, f.totalTicketCount - f.totalTicketSold,
			f.soldierDiscount < 1 || f.studentDiscount < 1 ? "有" : "无", 
			(!f.isSuitableForEldersAndChildren && (currentUser.age >= 60 || currentUser.age <= 14))?
				COLOR_RED_B "不适合老年及儿童" COLOR_RESET : "");
	}
	printf(COLOR_YELLOW_B "共检索到 %d 个景点信息, 请按回车键查询景点详情或预定门票, 按其他键返回主菜单\n" COLOR_RESET, count);

	free(featureSpots);

	choice = readKey();

	if (choice == '\r')
	{
		FeatureSpot *f = NULL;
		while (1) {
			while (1)
			{
				char id[20];
				inputStringWithLengthLimit("\n请输入要查询或预定的景点ID:", 1, 20, id);
				if ((f = getFeatureSpot(id)) == NULL)
					printf(COLOR_RED_B "您输入的ID不存在, 请重试\n" COLOR_RESET);
				else
					break;
			}

			char condition[50], value[5];
			sprintf(condition, "id = '%s'", f->id);  // 刷新访问量
			_itoa(f->visitCount + 1, value, 10);
			updateData("FeatureSpot", condition, "visitCount", value, 1);

			printf("景点详情\n");
			printf("================\n\n");

			printf("景点ID: "            COLOR_GREEN "%s\n"       COLOR_RESET, f->id);
			printf("景点名称: "          COLOR_GREEN "%s\n"       COLOR_RESET, f->name);
			printf("景点简介: "          COLOR_GREEN "%s\n"       COLOR_RESET, f->discription);
			printf("本季票价: "          COLOR_GREEN "%d\n"       COLOR_RESET, isHot ? f->hotSeasonPrice : f->coldSeasonPrice);
			printf("游览所需时间: "      COLOR_GREEN "%d\n"       COLOR_RESET, f->timeRequired);
			if (f->soldierDiscount < 1)
				printf("军人票价折扣: "  COLOR_GREEN "%1.2f\n"    COLOR_RESET, f->soldierDiscount);
			if (f->studentDiscount < 1)
				printf("学生票价折扣: "  COLOR_GREEN "%1.2f\n"    COLOR_RESET, f->studentDiscount);
			printf("景点等级: "          COLOR_GREEN "%d\n"       COLOR_RESET, f->level);
			printf("所在地区: "          COLOR_GREEN "%s\n"       COLOR_RESET, f->district);
			printf("剩余门票数: "        COLOR_GREEN "%d\n\n"     COLOR_RESET, f->totalTicketCount - f->totalTicketSold);

			printf("人数限制:\n");
			printf("开始时间    结束时间   人数限制\n");
			printf("==================================\n");

			int count = 0;
			for (int i = 0; i != f->limitationCount; i++) {
				FeatureSpotLimitation l = f->limitations[i];
				if (l.isHot == isHot) {
					printf("%s          %s          %d\n", l.timeSpanStart, l.timeSpanEnd, l.peopleCount);
					count++;
				}
			}
			printf(COLOR_YELLOW_B "\n本景点在 %d 个时段内有人数限制, 如遇限流请服从工作人员安排\n\n" COLOR_RESET, count);


			if (!f->isSuitableForEldersAndChildren && (currentUser.age >= 60 || currentUser.age <= 14)) {
				printf(COLOR_RED_B "该景点不适合老年人及儿童, 请预定其他景点, 按[q]返回主菜单\n" COLOR_RESET);
				if (readKey() == 'Q' || readKey() == 'q')
					break;
			}
			else if (f->totalTicketCount - f->totalTicketSold <= 0) {
				printf(COLOR_RED_B "该景点门票已售完，请预定其他景点, 按[q]返回主菜单\n" COLOR_RESET);
				if (readKey() == 'Q' || readKey() == 'q')
					break;
			}
			else {
				printf(COLOR_YELLOW_B "请按[r]预定当前景点门票, [o]预定其他景点门票, [q]返回主菜单, 按其他键继续查询\n" COLOR_RESET);
				choice = readKey();
				if (choice == 'r' || choice == 'R') {
					MakeReservation(f->id);
					break;
				}
				else if (choice == 'o' || choice == 'O') {
					MakeReservation(NULL);
					break;
				}
				else if (choice == 'Q' || choice == 'q')
					break;
			}
		}
	}

	printf("按回车键返回主菜单...\n");
	while (readKey() != '\r');
}

void MakeReservation(char *id) {
	printf("\n景点门票预定\n");
	printf("================\n\n");

	Reservation reservation;
	FeatureSpot featurespots[10];
	int featureSpotCount = 0;

	if (id == NULL)
	{
		char input[10];
		while (1) {
			inputStringWithLengthLimit("请输入景点ID:", 3, 10, input);
			FeatureSpot *f = getFeatureSpot(input);
			if (!strcmp(input, "end"))
			{
				printf(COLOR_BLUE_B "景点输入完成\n" COLOR_RESET);
				break;
			}
			if (f == NULL)
				printf(COLOR_RED_B "景点ID不存在\n" COLOR_RESET);
			else if (f->totalTicketCount - f->totalTicketSold <= 0)
				printf(COLOR_RED_B "该景点门票已售完\n" COLOR_RESET);
			else if (!f->isSuitableForEldersAndChildren && (currentUser.age >= 60 || currentUser.age <= 14))
				printf(COLOR_RED_B "该景点不适合当前旅游团的年龄段\n" COLOR_RESET);
			else
			{
				printf(COLOR_BLUE_B "您输入的第 %d 个景点为： %s, 如需结束输入, 请在下一个景点ID中输入end\n" COLOR_RESET, featureSpotCount, f->name);
				featurespots[featureSpotCount] = *f;
				featureSpotCount++;
			}
		}
	}
	else {
		printf("景点ID: %s\n", id);
		featureSpotCount = 1;
		featurespots[0] = *getFeatureSpot(id);
	}

	reservation.featureSpotCount = featureSpotCount;
	reservation.featureSpots = featurespots;
	int isHot = isHotSeason();
	reservation.isHotSeason = isHot;
	int flag = 1;
	while (flag) {
		printf("请选择游客身份");
		printf(COLOR_YELLOW_B "([0]:普通群众, [1]:学生, [2]:军人): " COLOR_RESET);
		int choice = readKey();
		printf("%c\n", choice);
		switch (choice) {
			case '0':
				reservation.idantity = NORMAL;
				flag = 0;
				break;
			case '1':
				reservation.idantity = STUDENT;
				flag = 0;
				break;
			case '2':
				reservation.idantity = SOLDIER;
				flag = 0;
				break;
			default:
				printf(COLOR_RED_B "按键无效\n" COLOR_RESET);
		}
	}

	inputDateTime("请输入出行时间 (格式: yyyy:mm:dd:hh:mm):", reservation.time);

	float totalPrice = 0;

	for (int i = 0; i != featureSpotCount; i++) {
		FeatureSpot f = featurespots[i];
		totalPrice += currentUser.peopleCount * (isHot ? f.hotSeasonPrice : f.coldSeasonPrice) *
			(reservation.idantity == SOLDIER ? f.soldierDiscount : 1) *
			(reservation.idantity == STUDENT ? f.studentDiscount : 1);
	}

	time_t timep;    // 获取当前时间
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);

	sprintf(reservation.orderDate, "%04d:%02d:%02d:%02d:%02d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min);
	sprintf(reservation.id, "%s%s", currentUser.id, reservation.orderDate);

	reservation.touristGroup = &currentUser;
	reservation.totalPrice = totalPrice;

	printf("总价格：" COLOR_GREEN_B "%6.2f" COLOR_RESET, totalPrice);
	if (inputBool("确认预定")) {

		int ret = addReservation(&reservation);
		if (ret != 1) {
			char err[100];
			getError(err);
			printf(COLOR_RED_B "数据库错误: %s (错误码: %d)\n" COLOR_RESET, err, -ret);
		}
		else {
			printf(COLOR_GREEN_B "订单添加成功\n" COLOR_RESET);

			for (int i = 0; i != featureSpotCount; i++) {
				FeatureSpot f = featurespots[i];
				char condition[50];
				char value[50];
				sprintf(condition, "id = '%s'", f.id);
				_itoa(f.reservationCount + 1, value, 10);
				updateData("FeatureSpot", condition, "reservationCount", value, 1);     // 更新景点统计数据
				_itoa(f.totalTicketSold + currentUser.peopleCount, value, 10);
				updateData("FeatureSpot", condition, "totalTicketSold", value, 1);
				if (isHot) {
					_itoa(f.hotSeasonTickets + currentUser.peopleCount, value, 10);
					updateData("FeatureSpot", condition, "hotSeasonTickets", value, 1);
				}
				else {
					_itoa(f.coldSeasonTickets + currentUser.peopleCount, value, 10);
					updateData("FeatureSpot", condition, "coldSeasonTickets", value, 1);
				}

				float price = currentUser.peopleCount * (isHot ? f.hotSeasonPrice : f.coldSeasonPrice) *
					(reservation.idantity == SOLDIER ? f.soldierDiscount : 1) *
					(reservation.idantity == STUDENT ? f.studentDiscount : 1);

				_itoa(f.totalProfit + price, value, 10);
				updateData("FeatureSpot", condition, "totalProfit", value, 1);
			}
		}
	}
}

// 判断当前是否为旺季
int isHotSeason() {
	time_t timep;    // 获取当前时间
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);

	return p->tm_mon == 8 || p->tm_mon == 1 || p->tm_mon == 2; // 假设每年1, 2, 8月为旅游旺季
}

void ReservationManagetment() {
	Reservation *reservations;
	char condition[50];
	sprintf(condition, "touristGroupId = '%s'", currentUser.id);
	int count = getReservationsBy(condition, 0, NULL, &reservations);

	system("cls");
	printf("订单管理\n");
	printf("================\n\n");

	if (count < 0) {
		char err[100];
		getError(err);
		printf(COLOR_RED_B "数据库错误: %s (错误码: %d), 请按回车键返回\n" COLOR_RESET, err, -count);
		while (readKey() == '\r');
		return;
	}

	printf("订单号                     景点名称      出行时间         游客身份      总价\n");
	printf("================================================================================\n");

	for (int i = 0; i != count; i++) {
		Reservation reservation = reservations[i];
		char fsName[20];
		const char *idantity;
		if (reservation.featureSpotCount == 1)
			strcpy(fsName, reservation.featureSpots->name);
		else
			sprintf(fsName, COLOR_GREEN "[%d个景点]" COLOR_RESET, reservation.featureSpotCount);

		if (reservation.idantity == STUDENT)
			idantity = "学生";
		else if (reservation.idantity == SOLDIER)
			idantity = "军人";
		else
			idantity = "群众";

		printf("%-26s%-10s%-16s%10s  %6.2f\n", reservation.id, fsName, reservation.time, idantity,
			reservation.totalPrice);
	}

	printf(COLOR_YELLOW_B "\n共 %d 条记录, 请按回车键查询订单详情, 按其他键返回主菜单\n" COLOR_RESET, count);

	char choice = readKey();

	while (choice == '\r') {
		Reservation *reservation = NULL;

		while (1) {
			char id[30];
			printf("请输入要查询的订单号: ");
			scanf("%s", id);
			if ((reservation = getReservation(id)) == NULL) {
				printf(COLOR_RED_B "该订单号不存在！" COLOR_RESET);
			}
			else if (strcmp(reservation->touristGroup->id, currentUser.id))
				printf(COLOR_RED_B "不是您的订单！" COLOR_RESET);
			else break;
		}

		printf("订单详情\n");
		printf("=====================\n\n");

		printf("预定时间: " COLOR_GREEN "%s\n" COLOR_RESET, reservation->orderDate);
		printf("出行时间: " COLOR_GREEN "%s\n" COLOR_RESET, reservation->time);
		printf("旅客身份: " COLOR_GREEN);
		if (reservation->idantity == NORMAL)
			printf("群众");
		else if (reservation->idantity == STUDENT)
			printf("学生");
		else if (reservation->idantity == SOLDIER)
			printf("军人");
		printf("\n" COLOR_RESET);
		printf("景点: " COLOR_GREEN);
		for (int i = 0; i != reservation->featureSpotCount; i++) {
			printf("%s,", reservation->featureSpots[i].name);
		}
		printf(COLOR_YELLOW_B"[共 %d 个景点]\n" COLOR_RESET, reservation->featureSpotCount);

		printf("总价格: " COLOR_GREEN "%6.2f\n" COLOR_RESET, reservation->totalPrice);


		// TODO: Check whether the reservation can be cancelled

		printf(COLOR_YELLOW_B "当前订单可取消, 请按[x]取消该订单, 按回车键继续查询, 其他键返回主菜单\n" COLOR_RESET);
		choice = readKey();
		if ((choice == 'x' || choice == 'X') && inputBool("是否要取消该订单")) {
			char condition[100];
			sprintf(condition, "id = '%s'", reservation->id);
			int ret = removeData("Reservation", condition);
			if (ret == 1)
				printf(COLOR_GREEN_B "取消订单成功\n" COLOR_RESET);
			else
			{
				char err[100];
				getError(err);
				printf(COLOR_RED_B "数据库错误: %s (错误码: %d)\n" COLOR_RESET, err, -ret);
			}

			break;
		}
	}

	printf("请按回车键进入主菜单...\n");
	while (readKey() != '\r');
}

void ModifyInfo() {
	system("cls");
	printf("游客团信息修改\n");
	printf("======================\n\n");
	printf(COLOR_RED_B "注意, 您修改的信息不会影响已预定的订单\n\n" COLOR_RESET);

	printf("    游客团ID: \x1b[32m%s\033[0m\n", currentUser.id);
	printf(COLOR_YELLOW_B "[1]" COLOR_RESET "     密码: " COLOR_GREEN "******\n" COLOR_RESET);
	printf(COLOR_YELLOW_B "[2]" COLOR_RESET "     人数: " COLOR_GREEN "%d\n" COLOR_RESET, currentUser.peopleCount);
	printf(COLOR_YELLOW_B "[3]" COLOR_RESET "   年龄段: " COLOR_GREEN "%d\n\n" COLOR_RESET, currentUser.age);
	printf(COLOR_YELLOW_B "[4]" COLOR_RESET " 联系电话: " COLOR_GREEN "%s\n\n" COLOR_RESET, currentUser.phone);
	printf(COLOR_YELLOW_B "请按键选择您要修改的信息, 按 [0] 取消修改\n" COLOR_RESET);

	char choice = 0;
	while (!(choice = readKey()) || choice > '4' || choice < '0')
		printf(COLOR_RED_B "您的按键有误, 请重试\n" COLOR_RESET);

	char condition[50];
	sprintf(condition, "id = '%s'", currentUser.id);

	int re;
	switch (choice)
	{
		case '1':
		{
			char password[30];
			char confirm[30];

			while (1)
			{
				inputPassword("请输入新密码:", password);
				inputPassword("请重新输入新密码:", confirm);

				if (strcmp(password, confirm))
					printf(COLOR_RED_B "两次密码输入不一致\n" COLOR_RESET);
				else
					break;
			}
			re = updateData("TouristGroup", condition, "password", password, 0);

			break;
		}
		case '2':
		{
			char value[5];
			_itoa(inputInteger("请输入新的人数:"), value, 10);

			re = updateData("TouristGroup", condition, "peopleCount", value, 1);
			break;
		}
		case '3':
		{
			char age[5];
			_itoa(inputInteger("请输入新的年龄段:"), age, 10);

			re = updateData("TouristGroup", condition, "age", age, 1);
			break;
		}
		case '4':
		{
			char phone[50];

			inputPhone("请输入新的手机号码:", phone);
			re = updateData("TouristGroup", condition, "phone", phone, 0);
			break;
		}
		default:
			break;
	}

	if (choice != '0') {
		if (re == 1) {
			printf(COLOR_GREEN_B "修改成功\n" COLOR_RESET);
			currentUser = *getTouristGroup(currentUser.id);
		}
		else {
			char err[100];
			getError(err);
			printf(COLOR_RED_B "数据库错误：%s (错误码: %d)\n" COLOR_RESET, err, -re);
		}
	}

	printf("\n请按回车键回到管理员主页...\n");
	while (readKey() != '\r');
}

void TouristGroupRegister() {
	system("cls");
	printf("游客团注册\n");
	printf("======================\n\n");

	TouristGroup touristGroup;

	while (1) {
		inputID("请输入旅游团ID:", touristGroup.id);
		if (checkIsUnique("TouristGroup", "id", touristGroup.id))
			break;
		else
			printf(COLOR_RED_B "用户名已存在！\n" COLOR_RESET);
	}

	while (1) {
		char password[30];
		inputPassword("请输入密码:", touristGroup.password);
		inputPassword("请再次输入密码:", password);
		if (!strcmp(touristGroup.password, password))
			break;
		else
			printf(COLOR_RED_B "两次密码输入不一致!\n" COLOR_RESET);
	}

	touristGroup.peopleCount = inputInteger("请输入旅游团人数:");

	touristGroup.age = inputInteger("请输入旅客平均年龄:");

	while (1) {
		inputPhone("请输入联系人电话:", touristGroup.phone);
		if (checkIsUnique("TouristGroup", "phone", touristGroup.phone))
			break;
		else
			printf(COLOR_RED_B "该联系人电话已被使用!\n" COLOR_RESET);
	}

	int re = addTourstGroup(&touristGroup);

	if (re < 1) {
		char err[100];
		getError(err);
		printf(COLOR_RED_B "数据库错误: %s (错误码: %d )" COLOR_RESET, err, -re);
		while (readKey() != '\r');
	}
	else {
		printf(COLOR_GREEN_B "注册成功, 请按回车键进入主菜单\n" COLOR_RESET);
		while (readKey() != '\r');
		currentUser = touristGroup;
		TouristGroupMenu();
	}
}