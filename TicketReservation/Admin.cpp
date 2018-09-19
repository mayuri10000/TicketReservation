/*
	管理员功能模块
*/

#include "stdafx.h"
#include "Database.h"
#include "Models.h"
#include "Validation.h"
#include "Color.h"
#include "Admin.h"

#define PAGE_MAX_COUNT 20

SystemAdmin currentUser;

void Login();
void ListReservations();
void AddFeatureSpot();
void ModifyPersonalInfo();
void ListFeatureSpot();
void AddAdmin();

// 管理员功能模块主入口函数
void Admin() 
{
	Login();   // 先执行登录

	
	while (1) {
		system("cls");              // 每次回到主菜单都清一下屏
		printf("当前管理员ID: " COLOR_GREEN_B "%s\n" COLOR_RESET , currentUser.id);  // 显示管理员ID
		printf("┌──────────────────────────────────────────────────┐\n");
		printf("│                管理员功能菜单                    │\n");
		printf("├──────────────────────────────────────────────────┤\n");
		printf("│               " COLOR_YELLOW_B "[1]" COLOR_RESET " 景点信息查询                   │\n");
		printf("│               " COLOR_YELLOW_B "[2]" COLOR_RESET " 订单信息查询                   │\n");
		printf("│               " COLOR_YELLOW_B "[3]" COLOR_RESET " 添加景点                       │\n");
		printf("│               " COLOR_YELLOW_B "[4]" COLOR_RESET " 添加管理员                     │\n");
		printf("│               " COLOR_YELLOW_B "[5]" COLOR_RESET " 个人信息管理                   │\n");
		printf("│               " COLOR_YELLOW_B "[6]" COLOR_RESET " 退出登录                       │\n");
		printf("└──────────────────────────────────────────────────┘\n");
		printf(COLOR_YELLOW_B "请按数字键选择功能\n" COLOR_RESET);

		char choice = '\0';
		while (!(choice = readKey()) || choice > '6' || choice < '1')  // 读取并验证用户ID
			printf(COLOR_RED_B "您的按键有误, 请重试\n" COLOR_RESET);
		

		switch (choice)
		{
			case '1':
			    ListFeatureSpot();  // 查询景点
				break;
			case '2':
				ListReservations(); // 查询订单
				break;
			case '3':
				AddFeatureSpot();   // 添加景点
				break;
			case '4':
				AddAdmin();         // 添加管理员
				break;
			case '5':
				ModifyPersonalInfo(); // 个人信息修改
				break;
			case '6':
				return;
		}
		
	}
}

// 管理员登录
void Login()
{
	system("cls");
	printf("管理员登录\n");
	printf("========================\n\n");
	while (1) {
		char userName[10];
		char password[30];
		inputStringWithLengthLimit("管理员ID:", 6, 10, userName);  // 输入用户名和密码
		inputPassword("密码:", password);

		SystemAdmin *user = getSystemAdmin(userName);
		if (user == NULL || strcmp(user->password, password))      // 验证
			printf(COLOR_RED_B "ID或密码错误, 请重试\n" COLOR_RESET);
		else {
			currentUser = *user;
			break;
		}
	}
}

// 管理员添加 （为了保障系统安全，新管理员注册必须在已有管理员账户下进行）
void AddAdmin() {
	system("cls");
	printf("添加管理员\n");
	printf("================\n\n");

	SystemAdmin systemAdmin;

	while(1) {
		inputID("请输入新管理员ID:", systemAdmin.id);    // 输入新管理员ID
		if (!checkIsUnique("SystemAdmin", "id", systemAdmin.id))  // 检查ID是否已被使用
			printf(COLOR_RED_B "该ID已存在!\n" COLOR_RESET);
		else
			break;
	}

	while (1) {
		char password[50];
		inputPassword("请输入密码:", systemAdmin.password);  // 输入新管理员密码
		inputPassword("请在此输入密码:", password);
		if (strcmp(systemAdmin.password, password))   // 验证两次输入是否一致
			printf(COLOR_RED_B "两次密码错误不一致!\n" COLOR_RESET);
		else
			break;
	}

	while (1) {
		inputEmail("请输入邮箱地址:", systemAdmin.email);   // 输入新管理员邮箱
		if (!checkIsUnique("SystemAdmin", "email", systemAdmin.email))  // 检查邮箱是否已被使用
			printf(COLOR_RED_B "该邮箱地址已存在!\n" COLOR_RESET);
		else
			break;
	}

	while (1) {
		inputPhone("请输入手机号码:", systemAdmin.phone);   // 输入新管理员手机号
		if (!checkIsUnique("SystemAdmin", "phone", systemAdmin.phone)) // 验证手机号是否已被使用
			printf(COLOR_RED_B "该手机号码已存在!\n" COLOR_RESET);
		else
			break;
	}

	if (addSystemAdmin(&systemAdmin) != 1) {   // 将新管理员账号存入数据库, 如遇数据库错误，则提示
		char error[100];
		getError(error);
		printf(COLOR_RED_B "数据库错误: %s \n" COLOR_RESET, error);
	}
	else {
		printf(COLOR_GREEN_B "添加成功\n" COLOR_RESET);
	}

	printf("请按回车键进入主菜单...\n");
	while (readKey() != '\r');
}

// 订单查询
void ListReservations() {
	Reservation *reservations;
	int count = getReservationsBy(NULL, 0, NULL, &reservations);

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

	for (int i = 0; i != count; i++) {          // 列出所有订单
		Reservation reservation = reservations[i];
		char fsName[20];
		const char *idantity;
		if (reservation.featureSpotCount == 1)  // 如果订单只包含一个景点，则输出该景点名称，否则输出景点数量
			strcpy(fsName, reservation.featureSpots->name);
		else
			sprintf(fsName, COLOR_GREEN "[%d个景点] " COLOR_RESET, reservation.featureSpotCount);

		if (reservation.idantity == STUDENT)   // 旅客身份
			idantity = "学生";
		else if (reservation.idantity == SOLDIER)
			idantity = "军人";
		else
			idantity = "群众";

		printf("%-26s%-10s%-16s%10s  %6.2f\n", reservation.id, fsName, reservation.time, idantity,
			reservation.totalPrice);   // 输出一条订单信息
	}

	printf(COLOR_YELLOW_B "\n共 %d 条记录, 请按回车键查询订单详情, 按其他键返回主菜单\n" COLOR_RESET, count);

	char choice = readKey();  

	while (choice == '\r') {  // 查询订单详情
		Reservation *reservation = NULL;

		while (1) {
			char id[30];
			printf("请输入要查询的订单号: ");  // 输入要查询的订单ID
			scanf("%s", id);
			if ((reservation = getReservation(id)) == NULL) { // 检查是否存在
				printf(COLOR_RED_B "该订单号不存在！" COLOR_RESET);
			}
			else break;
		}

		printf("订单详情\n");
		printf("=====================\n\n");

		printf("预定时间: " COLOR_GREEN "%s\n" COLOR_RESET, reservation->orderDate);  // 输出订单信息
		printf("出行时间: " COLOR_GREEN "%s\n" COLOR_RESET, reservation->time);
		printf("旅游团ID: " COLOR_GREEN "%s\n" COLOR_RESET, reservation->touristGroup->id);
		printf("旅客身份: " COLOR_GREEN);
		if (reservation->idantity == NORMAL)
			printf("群众");
		else if (reservation->idantity == STUDENT)
			printf("学生");
		else if (reservation->idantity == SOLDIER)
			printf("军人");
		printf("\n" COLOR_RESET);
		printf("景点: " COLOR_GREEN);
		for (int i = 0; i != reservation->featureSpotCount; i++) {  // 输出订单中包含的所有景点
			printf("	%s\n", reservation->featureSpots[i].name);
		}
		printf(COLOR_YELLOW_B"	[共 %d 个景点]\n" COLOR_RESET, reservation->featureSpotCount);

		printf("总价格: " COLOR_GREEN "%6.2f\n" COLOR_RESET, reservation->totalPrice);


		printf(COLOR_YELLOW_B "请按回车键继续查询, 其他键返回主菜单\n" COLOR_RESET);
		choice = readKey();  // 在次读取用户按键，如为回车键，再循环一次
		
	}

	printf("请按回车键进入主菜单...\n");
	while (readKey() != '\r');
}

// 景点添加
void AddFeatureSpot() {
	FeatureSpot featurespot;

	system("cls");
	printf("添加景点\n");
	printf("======================\n\n");

	while (1) {
		inputStringWithLengthLimit("景点ID:", 6, 6, featurespot.id);  // 输入并检查新景点ID
		if (!checkIsUnique("FeatureSpot", "id", featurespot.id))
			printf(COLOR_RED_B "此ID已存在！\n" COLOR_RESET );
		else
			break;
	}

	while (1) {
		inputStringWithLengthLimit("景点名称:", 1, 20, featurespot.name);  // 输入并检查新景点名称
		if (!checkIsUnique("FeatureSpot", "name", featurespot.name))
			printf(COLOR_RED_B "此名称已存在！\033[0m\n" COLOR_RESET);
		else
			break;
	}

	inputStringWithLengthLimit("景点简介:", 1, 200, featurespot.discription);  // 关于新景点的其他信息

	featurespot.coldSeasonPrice = inputInteger("淡季票价:");

	featurespot.hotSeasonPrice = inputInteger("旺季票价:");

	featurespot.timeRequired = inputInteger("游览所需时间 (小时):");
	
	featurespot.soldierDiscount = inputFloat("军人票价折扣:");

	featurespot.studentDiscount = inputFloat("学生票价折扣:");

	featurespot.level = inputInteger("景点级别:");

	inputStringWithLengthLimit("所在地区:", 1, 20, featurespot.district);

	featurespot.maintenanceFee = inputFloat("维护费用:");

	

	featurespot.totalTicketCount = inputInteger("总门票数:");

	featurespot.totalProfit = -featurespot.maintenanceFee;  // 初始时将景点中的总利润设为维护费用的负值，以后每次有新订单时都直接将总利润加上票价即可

	featurespot.isSuitableForEldersAndChildren = inputBool("是否适合老人及儿童");


	char option = 0;
	do {
		system("cls");
		printf("添加景点人数限制信息\n");                     // 开始添加景点人数限制信息，每个景点最多10条
		printf("======================\n\n");

		printf("当前景点: \x1b[01;32m%s %s\033[0m\n", featurespot.id, featurespot.name);
		printf("\033[01;33m您一共可以添加 10 条人数限制信息, 按[d]跳过此步骤\033[0m\n");
		
		option = readKey();
		featurespot.limitationCount = 0;
		featurespot.limitations = (FeatureSpotLimitation*)malloc(sizeof(FeatureSpotLimitation) * 10);
		while (option != 'd' && option != 'D') {
			FeatureSpotLimitation limition;

			strcpy(limition.featureSpotId, featurespot.id);  

			limition.isHot = inputBool("是否为旺季");   // 输入景点人数限制相关信息（季度，时段，限制人数）

			inputTime("开始时间:", limition.timeSpanStart);

			inputTime("结束时间:", limition.timeSpanEnd);

			limition.peopleCount = inputInteger("限制人数:");

			featurespot.limitations[featurespot.limitationCount] = limition;
			printf("\033[01;33m您还可以添加 %d 条人数限制信息, 按[d]完成添加, [x]取消添加, 按回车继续添加\033[0m\n",
				10 - (featurespot.limitationCount + 1));   // 每添加完一条，询问是否继续添加
			while (1) {
				option = readKey();
				if (option == 'd' || option == 'D' || option == '\r' || option == 'x' || option == 'X')
					break;
			}

			if (option == 'd' || option == 'D' || option == '\r')
				featurespot.limitationCount++;

			if (option == 'X' || option == 'x')
				break;
		}

		if ((option == 'd' || option == 'D') && featurespot.limitationCount) {

			printf("\x1b[01;31m注意：游客人数限制信息提交后将无法再进行任何修改与删除，请确认各项信息正确！\033[0m\n\n");
			int confirm = inputBool("是否确认添加");  // 确认景点人数限制信息
			if (confirm)
				break;
			else
				option = 'x';
		}
	} while (option == 'X' || option == 'x');

	system("cls");
	printf("请确认景点信息\n");
	printf("======================\n\n");
	printf(COLOR_RED_B "注意：景点信息发布后将无法再进行任何修改与删除，请确认各项信息正确后再发布！\n\n" COLOR_RESET);

	printf("景点ID: "        COLOR_GREEN "%s\n"       COLOR_RESET, featurespot.id);         // 让管理员确认景点信息，然后选择是否发布
	printf("景点名称: "      COLOR_GREEN "%s\n"       COLOR_RESET, featurespot.name);       // 新景点基本信息
	printf("景点简介: "      COLOR_GREEN "%s\n"       COLOR_RESET, featurespot.discription);
	printf("淡季票价: "      COLOR_GREEN "￥ %d\n"    COLOR_RESET, featurespot.coldSeasonPrice);
	printf("旺季票价: "      COLOR_GREEN "￥ %d\n"    COLOR_RESET, featurespot.hotSeasonPrice);
	printf("游览所需时间: "  COLOR_GREEN "%d\n"       COLOR_RESET, featurespot.timeRequired);
	printf("军人票价折扣: "  COLOR_GREEN "%1.2f\n"    COLOR_RESET, featurespot.soldierDiscount);
	printf("学生票价折扣: "  COLOR_GREEN "%1.2f\n"    COLOR_RESET, featurespot.studentDiscount);
	printf("景点等级: "      COLOR_GREEN "%d\n"       COLOR_RESET, featurespot.level);
	printf("所在地区: "      COLOR_GREEN "%s\n"       COLOR_RESET, featurespot.district);
	printf("维护费用: "      COLOR_GREEN "￥ %6.2f\n" COLOR_RESET, featurespot.maintenanceFee);
	printf("总门票数: "      COLOR_GREEN "%d\n"       COLOR_RESET, featurespot.totalTicketCount);
	printf("适合老人及儿童: " COLOR_GREEN "%d\n\n"    COLOR_RESET, featurespot.isSuitableForEldersAndChildren);
	
	printf("淡旺季    开始时间   结束时间   限制人数\n");
	printf("=========================================\n");            // 人数限制信息
	for (int i = 0; i != featurespot.limitationCount; i++) {
		FeatureSpotLimitation lim = featurespot.limitations[i];
		printf(" %s      %s      %s        %d\n", lim.isHot ? "旺季" : "淡季", 
			lim.timeSpanStart, lim.timeSpanEnd, lim.peopleCount);
	}
	printf("共 %d 条人数限制信息\n", featurespot.limitationCount);

	if (inputBool("确认发布")) {  // 询问是否确认发布
		int re = addFeatureSpot(&featurespot);   // 将新的景点信息存入数据库
		if (re != 1) {
			char err[100];
			getError(err);
			printf(COLOR_RED_B "数据库错误：%s (错误码: %d)\n" COLOR_RESET, err, -re);
		}
		else {
			printf(COLOR_GREEN_B "发布成功\n" COLOR_RESET);
		}
	}

	printf("\n请按回车键回到管理员主页...\n");
	while (readKey() != '\r');
}

// 景点信息查询
void ListFeatureSpot() {
	system("cls");
	printf("景点信息查询\n");
	printf("======================\n\n");

	printf(COLOR_YELLOW_B "请选择排序方式\n" COLOR_RESET);         // 询问排序方式
	printf(COLOR_YELLOW_B "[1]" COLOR_RESET " 不排序\n");
	printf(COLOR_YELLOW_B "[2]" COLOR_RESET " 按预订量进行排序\n");
	printf(COLOR_YELLOW_B "[3]" COLOR_RESET " 按访问人数排序\n");
	printf(COLOR_YELLOW_B "[4]" COLOR_RESET " 按利润排序\n");
	printf(COLOR_YELLOW_B "[5]" COLOR_RESET " 按淡季售票量排序\n");
	printf(COLOR_YELLOW_B "[6]" COLOR_RESET " 按旺季售票量排序\n");

	const char *sortBy = NULL;   // 要排序的字段名
	int sort = 1;                // 是否排序，以及升序还是降序
	char option = 1;

	while (!(option = readKey()) || option > '6' || option < '1')  // 读取用户选项
		printf(COLOR_RED_B "您的按键有误, 请重试\n" COLOR_RESET);

	switch (option) {
		case '1':
			sort = 0;
			break;
		case '2':
			sortBy = "reservationCount";   // 按预定量排序
			break;
		case '3':
			sortBy = "visitCount";         // 按访问量 (用户查询该景点详情的次数)排序 
			break;
		case '4':
			sortBy = "totalProfit";        // 按总利润排序
			break;
		case '5':
			sortBy = "coldSeasonTickets";  // 按淡季售票量排序
			break;
		case '6':
			sortBy = "hotSeasonTickets";   // 按旺季售票量排序
			break;
		default:
			sort = 0;
	}

	if (sort) {
		printf(COLOR_YELLOW_B "\n\n请选择排序类型\n" COLOR_RESET);  // 询问排序方式
		printf(COLOR_YELLOW_B "[1]" COLOR_RESET " 升序排序\n");
		printf(COLOR_YELLOW_B "[2]" COLOR_RESET " 降序排序\n");
		 
		while (!(option = readKey()) || option > '2' || option < '1') // 读取选项
			printf(COLOR_RED_B "您的按键有误, 请重试\n" COLOR_RESET); 

		switch (option) {
			case '1':
				sort = 1;  // 升序排序
				break;
			case '2':
				sort = -1; // 降序排序
				break;
			default:
				sort = 0;
		}
	}


	system("cls");

	FeatureSpot *featurespots;  
	int featureSpotCount = getFeatureSpotsBy(NULL, sort, sortBy, &featurespots); // 从数据库中获取所有景点信息

	if (featureSpotCount < 0) {  
		char err[100];
		getError(err);
		printf(COLOR_RED_B "数据库错误：%s (错误码: %d)\n" COLOR_RESET, err, -featureSpotCount);
	}
	else {
		printf("ID     景点名称       等级   总预定量  访问量   淡季售票量  旺季售票量  总利润\n");
		printf("=============================================================================\n");
		
		for (int i = 0; i != featureSpotCount; i++) {   // 输出景点信息及其统计信息
			FeatureSpot f = featurespots[i];
			printf(COLOR_YELLOW_B "%s" COLOR_RESET " %-14s%5d%10d%8d%14d%11d  %6.2f\n", f.id, f.name, f.level, f.reservationCount,
				f.visitCount, f.coldSeasonTickets, f.hotSeasonTickets, f.totalProfit);
		}

		free(featurespots);          // 释放景点信息数组所占用的内存空间

		printf(COLOR_YELLOW_B "共 %d 条记录, 请按[0]查询景点详情, 按其他键返回管理员菜单\n" COLOR_RESET, featureSpotCount);
		option = readKey();
		if (option == '0') {         // 景点详情查询
			while (option != 'e') {
				char id[10];
				printf("请输入要查询的景点ID: ");  // 读取景点ID
				scanf("%s", id);
				FeatureSpot *f = getFeatureSpot(id); // 根据输入的ID从数据库中获取景点信息
				if (f == NULL) 
					printf(COLOR_RED_B "未找到该景点信息！\n" COLOR_RESET);
				else {
					printf("\n基本信息\n");
					printf("===============\n\n");

					printf("景点ID: "        COLOR_GREEN "%s\n"       COLOR_RESET, f->id);    // 输出景点基本信息
					printf("景点名称: "      COLOR_GREEN "%s\n"       COLOR_RESET, f->name);
					printf("景点简介: "      COLOR_GREEN "%s\n"       COLOR_RESET, f->discription);
					printf("淡季票价: "      COLOR_GREEN "%d\n"    COLOR_RESET, f->coldSeasonPrice);
					printf("旺季票价: "      COLOR_GREEN "%d\n"    COLOR_RESET, f->hotSeasonPrice);
					printf("游览所需时间: "  COLOR_GREEN "%d\n"       COLOR_RESET, f->timeRequired);
					printf("军人票价折扣: "  COLOR_GREEN "%1.2f\n"    COLOR_RESET, f->soldierDiscount);
					printf("学生票价折扣: "  COLOR_GREEN "%1.2f\n"    COLOR_RESET, f->studentDiscount);
					printf("景点等级: "      COLOR_GREEN "%d\n"       COLOR_RESET, f->level);
					printf("所在地区: "      COLOR_GREEN "%s\n"       COLOR_RESET, f->district);
					printf("维护费用: "      COLOR_GREEN "%6.2f\n" COLOR_RESET, f->maintenanceFee);
					printf("总门票数: "      COLOR_GREEN "%d\n"       COLOR_RESET, f->totalTicketCount);
					printf("适合老人及儿童: " COLOR_GREEN "%d\n\n"    COLOR_RESET, f->isSuitableForEldersAndChildren);

					printf("营业数据\n");
					printf("==================\n\n");

					printf("总订单数: "      COLOR_GREEN "%d\n"       COLOR_RESET, f->reservationCount);  // 景点营业统计数据
					printf("总售票量: "      COLOR_GREEN "%d\n"       COLOR_RESET, f->totalTicketSold);
					printf("淡季售票量: "    COLOR_GREEN "%d\n"       COLOR_RESET, f->coldSeasonTickets);
					printf("旺季售票量: "    COLOR_GREEN "%d\n"       COLOR_RESET, f->hotSeasonTickets);
					printf("总利润: "        COLOR_GREEN "%6.2f\n\n"    COLOR_RESET, f->totalProfit);

					Reservation *reservations;
					int count = getReservationsForFeatureSpot(f->id, &reservations);                        // 该景点获取所有订单，以显示旅客年龄分布
					if (count < 0) {
						char err[100];
						getError(err);
						printf(COLOR_RED_B "数据库错误: %s (错误码: %d)\n" COLOR_RESET, err, -count);
						return;
					}

					int ageUnder14 = 0;  // 14岁以下
					int age15To30 = 0;   // 15-30岁
					int age31To40 = 0;   // 31-40岁
					int age41To60 = 0;   // 41-60岁
					int ageUp61 = 0;     // 61岁以上

					for (int i = 0; i != count; i++) {  // 遍历订单数据
						Reservation reservation = reservations[i];
						int age = reservation.touristGroup->age;   // 旅客年龄段
						int people = reservation.touristGroup->peopleCount;  // 人数
						if (age <= 14)
							ageUnder14 += people;     // 将对应年龄段的人数加上该订单人数
						else if (age >= 15 && age <= 30)
							age15To30 += people;
						else if (age >= 31 && age <= 40)
							age31To40 += people;
						else if (age >= 41 && age <= 60)
							age41To60 += people;
						else if (age >= 61)
							ageUp61 += people;
					}

					printf("景点旅客年龄段分布\n");
					printf("=======================\n");

					printf("14岁以下  %d 人\n", ageUnder14);   // 显示年龄分布
					printf("15-30岁   %d 人\n", age15To30);
					printf("31-40岁   %d 人\n", age31To40);
					printf("41-60岁   %d 人\n", age31To40);
					printf("61岁以上  %d 人\n\n", ageUp61);

					printf(COLOR_YELLOW_B "按任意键继续查询，按[e]退出\n" COLOR_RESET);
					option = readKey();   // 读取用户按键，如为回车继续查询
				}
			}
		}
	}


	printf("\n请按回车键回到管理员主页...\n");
	while (readKey() != '\r');
}

// 个人信息修改
void ModifyPersonalInfo() 
{
	system("cls");
	printf("个人信息管理\n");
	printf("======================\n\n");

	printf("    管理员ID: \x1b[32m%s\033[0m\n", currentUser.id);  // 输出当前管理员基本信息
	printf(COLOR_YELLOW_B "[1]" COLOR_RESET "     密码: " COLOR_GREEN "******\n" COLOR_RESET);  // 不显示密码
	printf(COLOR_YELLOW_B "[2]" COLOR_RESET " 电子邮箱: " COLOR_GREEN "%s\n" COLOR_RESET, currentUser.email); 
	printf(COLOR_YELLOW_B "[3]" COLOR_RESET " 手机号码: " COLOR_GREEN "%s\n\n" COLOR_RESET, currentUser.phone);
	printf(COLOR_YELLOW_B "请按键选择您要修改的信息, 按 [4] 取消修改\n" COLOR_RESET);

	char choice = 0;
	while (!(choice = readKey()) || choice > '4' || choice < '1')
		printf(COLOR_RED_B "您的按键有误, 请重试\n" COLOR_RESET);

	char condition[50];
	sprintf(condition, "id = '%s'", currentUser.id);

	int re;
	switch (choice) 
	{
		case '1':    // 密码
		{
			char password[30];
			char confirm[30];

			while (1) 
			{
				inputPassword("请输入新密码:", password);  // 输入密码
				inputPassword("请重新输入新密码:", confirm); // 在次输入密码

				if (strcmp(password, confirm))  // 验证两次输入密码是否一致
					printf(COLOR_RED_B "两次密码输入不一致\n" COLOR_RESET);
				else
					break;
			}
			re = updateData("SystemAdmin", condition, "password", password, 0);  // 更新数据

			break;
		}
		case '2':
		{
			char email[50];

			inputEmail("请输入新的邮箱地址:", email);

			re = updateData("SystemAdmin", condition, "email", email, 0);
			break;
		}
		case '3':
		{
			char phone[50];
			inputPhone("请输入新的手机号码:", phone);

			re = updateData("SystemAdmin", condition, "phone", phone, 0);
			break;
		}
		default: 
			break;
	}

	if (choice != '4') {
		if (re == 1) {
			printf(COLOR_GREEN_B "修改成功\n" COLOR_RESET);
			currentUser = *getSystemAdmin(currentUser.id);
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
