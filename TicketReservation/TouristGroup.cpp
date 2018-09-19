/*
	�����Ź���ģ��
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

// �����ŵ�¼
void TouristGroupLogin() {
	system("cls");
	printf("�ο��ŵ�¼\n");
	printf("======================\n\n");

	while (1) {
		char userName[10];
		char password[30];
		inputStringWithLengthLimit("������ID:", 6, 10, userName);  // ����ID������
		inputPassword("����:", password);

		TouristGroup *user = getTouristGroup(userName);
		if (user == NULL || strcmp(user->password, password))     // ��֤ID������
			printf(COLOR_RED_B "ID���������, ������\n" COLOR_RESET);
		else {
			currentUser = *user;    // ��¼�ɹ�����ȫ��currentUser����Ϊ��ǰ������
			TouristGroupMenu();     // �������������˵�
			break;
		}
	}
}

void TouristGroupMenu() {
	while (1) {
		system("cls");
		printf("��ǰ������: " COLOR_GREEN_B "%s\n" COLOR_RESET, currentUser.id);
		printf("��������������������������������������������������������������������������������������������������������\n");
		printf("��              �����ξ�����ƱԤ��ϵͳ              ��\n");
		printf("��������������������������������������������������������������������������������������������������������\n");
		printf("��               " COLOR_YELLOW_B "[1]" COLOR_RESET " �����ѯ��Ԥ��                 ��\n");
		printf("��               " COLOR_YELLOW_B "[2]" COLOR_RESET " ��������                       ��\n");
		printf("��               " COLOR_YELLOW_B "[3]" COLOR_RESET " ��������Ϣ�޸�                 ��\n");
		printf("��               " COLOR_YELLOW_B "[4]" COLOR_RESET " �˳���¼                       ��\n");
		printf("��������������������������������������������������������������������������������������������������������\n");
		printf(COLOR_YELLOW_B "�밴���ּ�ѡ����\n" COLOR_RESET);

		char choice = '\0';
		while (!(choice = readKey()) || choice > '5' || choice < '1') // ��ȡ����֤�û�����
			printf(COLOR_RED_B "���İ�������, ������\n" COLOR_RESET);

		switch (choice)
		{
			case '1':  // �����ѯ��Ԥ��
				QueryAndOrder();
				break;
			case '2': // ��������
				ReservationManagetment();
				break;
			case '3': // ��������Ϣ�޸�
				ModifyInfo();
				break;
			default:
				return;
		}
	}
}

void QueryAndOrder() {
	system("cls");
	printf("�����ѯ��Ԥ��\n");
	printf("==================\n\n");
	printf(COLOR_YELLOW_B "[0]" COLOR_RESET "���ž����Ƽ�\n");
	printf(COLOR_YELLOW_B "[1]" COLOR_RESET "�鿴���о���\n");
	printf(COLOR_YELLOW_B "[2]" COLOR_RESET "�������Ʋ�ѯ����\n");
	printf(COLOR_YELLOW_B "[3]" COLOR_RESET "ֻ�鿴��Ʊ�ľ���\n");
	printf(COLOR_YELLOW_B "[4]" COLOR_RESET "���������ѯ����\n");
	printf(COLOR_YELLOW_B "[5]" COLOR_RESET "ֻ�鿴���ۿ۵ľ���\n\n");

	printf(COLOR_YELLOW_B "�밴���ּ�ѡ����\n" COLOR_RESET);

	char choice = '\0';
	while (!(choice = readKey()) || choice > '5' || choice < '0')
		printf(COLOR_RED_B "���İ�������, ������\n" COLOR_RESET);

	char *pCondition = NULL;
	char condition[50];
	const char *orderBy = NULL;
	int order = 0;

	switch (choice) {
	case '0':
		sprintf(condition, "reservationCount >= 10");    // ���ž����Ƽ�����ʾԤ��������10�ľ��㣩
		pCondition = condition;
		break;
	case '1':
		break;
	case '2':
		char keyword[20];
		inputStringWithLengthLimit("�����������ؼ���:", 1, 20, keyword); // ͨ���ؼ��ʲ�ѯ
		sprintf(condition, "name LIKE '%%%s%%'", keyword);  // ��c���Եĸ�ʽ�ַ�����, Ҫ�����һ�� %, ����������
		pCondition = condition;
		break;
	case '3':
		sprintf(condition, "totalTicketCount - totalTicketSold > %d", currentUser.peopleCount);    // ��Ʊ�Ƿ����
		pCondition = condition;
		break;
	case '4':
		char region[20];
		inputStringWithLengthLimit("����������:", 1, 20, region);  // ���������ѯ
		sprintf(condition, "district = '%s'", region);
		pCondition = condition;
		break;
	case '5':
		sprintf(condition, "solderDiscount <> 1.0 OR studentDiscount <> 1.0"); // �Ƿ����ۿ�
		pCondition = condition;
		break;
	default:
		break;
	}
	printf(COLOR_YELLOW_B "\n��ѡ����������\n" COLOR_RESET);
	printf(COLOR_YELLOW_B "[1]" COLOR_RESET "������\n");
	printf(COLOR_YELLOW_B "[2]" COLOR_RESET "�۸��ɵ͵���\n");
	printf(COLOR_YELLOW_B "[3]" COLOR_RESET "�۸��ɸߵ���\n");
	printf(COLOR_YELLOW_B "[4]" COLOR_RESET "Ԥ�����ɸߵ���\n");
	printf(COLOR_YELLOW_B "[5]" COLOR_RESET "Ԥ�����ɵ͵���\n");

	while (!(choice = readKey()) || choice > '5' || choice < '1')
		printf(COLOR_RED_B "���İ�������, ������\n" COLOR_RESET);

	int isHot = isHotSeason();

	switch (choice) {
	case '1':
		break;
	case '2':
		order = 1;
		orderBy = isHot ? "hotSeasonPrice" : "coldSeasonPrice";  // ���۸�����
		break;
	case '3':
		order = -1;
		orderBy = isHot ? "hotSeasonPrice" : "coldSeasonPrice";
		break;
	case '4':
		order = -1;
		orderBy = "reservationCount"; // ��Ԥ��������
		break;
	case '5':
		order = 1;
		orderBy = "reservationCount";
		break;
	default:
		break;
	}

	FeatureSpot *featureSpots;
	int count = getFeatureSpotsBy(pCondition, order, orderBy, &featureSpots);  // ��ѯ���ݿ�

	if (count < 0) {  // �����������������Ϣ
		char err[100];
		getError(err);
		printf(COLOR_RED_B "���ݿ����: %s (������: %d)\n" COLOR_RESET, err, -count);
		printf("���س����������˵�...\n");
		while (readKey() != '\r');
		return;
	}
	
	system("cls");
	printf("ID         ����         ����   ��Ԥ����   �����۸�   ʣ��Ʊ��   �ۿ�\n");
	printf("=======================================================================================\n");
	for (int i = 0; i != count; i++) {
		FeatureSpot f = featureSpots[i];
		printf("%-11s%-13s%-6s%7d%10d%10d        %s    %s\n", f.id, f.name, f.district, f.reservationCount, // ������о�����Ϣ
			isHot ? f.hotSeasonPrice : f.coldSeasonPrice, f.totalTicketCount - f.totalTicketSold,
			f.soldierDiscount < 1 || f.studentDiscount < 1 ? "��" : "��",
			!f.isSuitableForEldersAndChildren ? COLOR_RED_B "���ʺ����꼰��ͯ" COLOR_RESET : "");
	}
	printf(COLOR_YELLOW_B "�������� %d ��������Ϣ, �밴�س�����ѯ���������Ԥ����Ʊ, ���������������˵�\n" COLOR_RESET, count);

	free(featureSpots);

	choice = readKey();

	if (choice == '\r')   // ��ѯ������ϸ��Ϣ
	{
		FeatureSpot *f = NULL;
		while (1) {
			while (1)
			{
				char id[20];
				inputStringWithLengthLimit("\n������Ҫ��ѯ��Ԥ���ľ���ID:", 1, 20, id);
				if ((f = getFeatureSpot(id)) == NULL)
					printf(COLOR_RED_B "�������ID������, ������\n" COLOR_RESET);
				else
					break;
			}

			char condition[50], value[5];
			sprintf(condition, "id = '%s'", f->id);  // ˢ�·�����
			_itoa(f->visitCount + 1, value, 10);
			updateData("FeatureSpot", condition, "visitCount", value, 1);

			printf("��������\n");
			printf("================\n\n");

			printf("����ID: "            COLOR_GREEN "%s\n"       COLOR_RESET, f->id);
			printf("��������: "          COLOR_GREEN "%s\n"       COLOR_RESET, f->name);
			printf("������: "          COLOR_GREEN "%s\n"       COLOR_RESET, f->discription);
			printf("����Ʊ��: "          COLOR_GREEN "%d\n"       COLOR_RESET, isHot ? f->hotSeasonPrice : f->coldSeasonPrice);
			printf("��������ʱ��: "      COLOR_GREEN "%d\n"       COLOR_RESET, f->timeRequired);
			if (f->soldierDiscount < 1)
				printf("����Ʊ���ۿ�: "  COLOR_GREEN "%1.2f\n"    COLOR_RESET, f->soldierDiscount);
			if (f->studentDiscount < 1)
				printf("ѧ��Ʊ���ۿ�: "  COLOR_GREEN "%1.2f\n"    COLOR_RESET, f->studentDiscount);
			printf("����ȼ�: "          COLOR_GREEN "%d\n"       COLOR_RESET, f->level);
			printf("���ڵ���: "          COLOR_GREEN "%s\n"       COLOR_RESET, f->district);
			printf("ʣ����Ʊ��: "        COLOR_GREEN "%d\n\n"     COLOR_RESET, f->totalTicketCount - f->totalTicketSold);

			printf("��������:\n");
			printf("��ʼʱ��    ����ʱ��   ��������\n");
			printf("==================================\n");

			int count = 0;
			for (int i = 0; i != f->limitationCount; i++) {
				FeatureSpotLimitation l = f->limitations[i];
				if (l.isHot == isHot) {
					printf("%s          %s          %d\n", l.timeSpanStart, l.timeSpanEnd, l.peopleCount);
					count++;
				}
			}
			printf(COLOR_YELLOW_B "\n�������� %d ��ʱ��������������, ������������ӹ�����Ա����\n\n" COLOR_RESET, count);


			if (!f->isSuitableForEldersAndChildren && (currentUser.age >= 60 || currentUser.age <= 14)) {
				printf(COLOR_RED_B "�þ��㲻�ʺ������˼���ͯ, ��Ԥ����������, ��[q]�������˵�\n" COLOR_RESET);
				if (readKey() == 'Q' || readKey() == 'q')
					break;
			}
			else if (f->totalTicketCount - f->totalTicketSold < currentUser.peopleCount) {
				printf(COLOR_RED_B "�þ�����Ʊ���Ѳ��㣬��Ԥ����������, ��[q]�������˵�\n" COLOR_RESET);
				if (readKey() == 'Q' || readKey() == 'q')
					break;
			}
			else {
				printf(COLOR_YELLOW_B "�밴[r]Ԥ����ǰ������Ʊ, [o]Ԥ������������Ʊ, [q]�������˵�, ��������������ѯ\n" COLOR_RESET);
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

	printf("���س����������˵�...\n");
	while (readKey() != '\r');
}

/*
	Ԥ����Ʊ���ܺ���
	id: ����id��ΪNULL��Ҫ���û���������
*/
void MakeReservation(char *id) {
	printf("\n������ƱԤ��\n");
	printf("================\n\n");

	Reservation reservation;
	FeatureSpot featurespots[10];
	int featureSpotCount = 0;

	if (id == NULL)               // �������ͨ����ѯֱ��Ԥ������Ҫ���û����뾰��ID
	{
		char input[10];
		while (1) {                  
			inputStringWithLengthLimit("�����뾰��ID:", 3, 10, input);
			FeatureSpot *f = getFeatureSpot(input);  // ����ID��ȡ������Ϣ
			if (!strcmp(input, "end"))  // ��������龰��ID, �������, ����"end"
			{
				printf(COLOR_BLUE_B "�����������\n" COLOR_RESET);
				break;
			}
			if (f == NULL)        // ��֤����ID�Ƿ����
				printf(COLOR_RED_B "����ID������\n" COLOR_RESET);
			else if (f->totalTicketCount - f->totalTicketSold < currentUser.peopleCount)  // ʣ����Ʊ���Ƿ��㹻
				printf(COLOR_RED_B "�þ�����Ʊ�Ѳ���\n" COLOR_RESET);
			else if (!f->isSuitableForEldersAndChildren && (currentUser.age >= 60 || currentUser.age <= 14))  // �����Ƿ��ʺϵ�ǰ�����������
				printf(COLOR_RED_B "�þ��㲻�ʺϵ�ǰ�����ŵ������\n" COLOR_RESET);
			else
			{
				// ����������Ƽ���ʾ��Ϣ
				printf(COLOR_BLUE_B "������ĵ� %d ������Ϊ�� %s, �����������, ������һ������ID������end\n" COLOR_RESET, featureSpotCount + 1, f->name);
				featurespots[featureSpotCount] = *f; 
				featureSpotCount++;
			}
		}
	}
	else {
		printf("����ID: %s\n", id);
		featureSpotCount = 1;
		featurespots[0] = *getFeatureSpot(id);
	}

	reservation.featureSpotCount = featureSpotCount;  // ������ľ�����Ϣд�붩���ṹ��
	reservation.featureSpots = featurespots;
	int isHot = isHotSeason();          // �Ƿ�Ϊ��������
	reservation.isHotSeason = isHot;
	int flag = 1;
	while (flag) {
		printf("��ѡ���ο����");
		printf(COLOR_YELLOW_B "([0]:��ͨȺ��, [1]:ѧ��, [2]:����): " COLOR_RESET); // �����ο����, �Ա��ж��ۿ�
		int choice = readKey();
		printf("%c\n", choice);
		switch (choice) {
			case '0':
				reservation.idantity = NORMAL;  // ��ͨȺ��
				flag = 0;
				break;
			case '1':
				reservation.idantity = STUDENT;  // ѧ��
				flag = 0;
				break;
			case '2':
				reservation.idantity = SOLDIER;  // ����
				flag = 0;
				break;
			default:
				printf(COLOR_RED_B "������Ч\n" COLOR_RESET);
		}
	}

	inputDateTime("���������ʱ�� (��ʽ: yyyy:mm:dd:hh:mm):", reservation.time);  // �������ʱ��

	float totalPrice = 0;

	for (int i = 0; i != featureSpotCount; i++) {     // �����ܼ۸�
		FeatureSpot f = featurespots[i];
		totalPrice += currentUser.peopleCount * (isHot ? f.hotSeasonPrice : f.coldSeasonPrice) * 
			(reservation.idantity == SOLDIER ? f.soldierDiscount : 1) *  // �ۿ�
			(reservation.idantity == STUDENT ? f.studentDiscount : 1);
	}

	time_t timep;    // ��ȡ��ǰʱ��
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);

	// ����ǰʱ��ת��Ϊ yyyy:mm:dd:hh:mm ���붩���ṹ��
	sprintf(reservation.orderDate, "%04d:%02d:%02d:%02d:%02d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min);
	sprintf(reservation.id, "%s%s", currentUser.id, reservation.orderDate);  // ���ɶ���ID

	reservation.touristGroup = &currentUser;   // ����������Ϣд�붩����Ϣ
	reservation.totalPrice = totalPrice;   

	printf("�ܼ۸�" COLOR_GREEN_B "%6.2f\n" COLOR_RESET, totalPrice);  // ����ܼ۸�
	if (inputBool("ȷ��Ԥ��")) {

		int ret = addReservation(&reservation);
		if (ret != 1) {
			char err[100];
			getError(err);
			printf(COLOR_RED_B "\n���ݿ����: %s (������: %d)\n" COLOR_RESET, err, -ret);
		}
		else {
			printf(COLOR_GREEN_B "\n������ӳɹ�\n" COLOR_RESET);

			for (int i = 0; i != featureSpotCount; i++) { // ���¾���ͳ������
				FeatureSpot f = featurespots[i];
				char condition[50];
				char value[50];
				sprintf(condition, "id = '%s'", f.id);
				_itoa(f.reservationCount + 1, value, 10);
				updateData("FeatureSpot", condition, "reservationCount", value, 1);    // ������ += 1
				_itoa(f.totalTicketSold + currentUser.peopleCount, value, 10);
				updateData("FeatureSpot", condition, "totalTicketSold", value, 1);     // ��Ʊ�� += ����������
				if (isHot) {
					_itoa(f.hotSeasonTickets + currentUser.peopleCount, value, 10);
					updateData("FeatureSpot", condition, "hotSeasonTickets", value, 1);
				}
				else {
					_itoa(f.coldSeasonTickets + currentUser.peopleCount, value, 10);
					updateData("FeatureSpot", condition, "coldSeasonTickets", value, 1);
				}

				float price = currentUser.peopleCount * (isHot ? f.hotSeasonPrice : f.coldSeasonPrice) *
					(reservation.idantity == SOLDIER ? f.soldierDiscount : 1) *         // ���㵱ǰ������Ʊ��
					(reservation.idantity == STUDENT ? f.studentDiscount : 1);

				sprintf(value, "%f", f.totalProfit + price);
				updateData("FeatureSpot", condition, "totalProfit", value, 1);          // ������ += �ܼ�
			}
		}
	}
}

// �жϵ�ǰ�Ƿ�Ϊ����
int isHotSeason() {
	time_t timep;    // ��ȡ��ǰʱ��
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);

	return p->tm_mon == 8 || p->tm_mon == 1 || p->tm_mon == 2; // ����ÿ��1, 2, 8��Ϊ��������
}

// ����������
void ReservationManagetment() {
	Reservation *reservations;
	char condition[50];
	sprintf(condition, "touristGroupId = '%s'", currentUser.id);       // �����������
	int count = getReservationsBy(condition, 0, NULL, &reservations);  // ��ȡ���ο��ŵ����ж�����Ϣ

	system("cls");
	printf("��������\n");
	printf("================\n\n");

	if (count < 0) {
		char err[100];
		getError(err);
		printf(COLOR_RED_B "���ݿ����: %s (������: %d), �밴�س�������\n" COLOR_RESET, err, -count);
		while (readKey() == '\r');
		return;
	}
	  // ��ӡ���ж�����Ϣ
	printf("������                     ��������      ����ʱ��         �ο����      �ܼ�\n");
	printf("================================================================================\n");

	for (int i = 0; i != count; i++) {
		Reservation reservation = reservations[i];
		char fsName[20];
		const char *idantity;
		if (reservation.featureSpotCount == 1)              // ���ֻ��һ������, ��ֱ�������������, ���������ɫ�����"[n ������]"
			strcpy(fsName, reservation.featureSpots->name);
		else
			sprintf(fsName, COLOR_GREEN "[%d������] " COLOR_RESET, reservation.featureSpotCount);

		if (reservation.idantity == STUDENT)                // ����û����
			idantity = "ѧ��";
		else if (reservation.idantity == SOLDIER)
			idantity = "����";
		else
			idantity = "Ⱥ��";

		printf("%-26s%-10s%-16s%10s  %6.2f\n", reservation.id, fsName, reservation.time, idantity,
			reservation.totalPrice);
	}

	printf(COLOR_YELLOW_B "\n�� %d ����¼, �밴�س�����ѯ��������, ���������������˵�\n" COLOR_RESET, count);

	char choice = readKey();

	while (choice == '\r') { // �س���
		Reservation *reservation = NULL;

		while (1) {
			char id[30];
			printf("������Ҫ��ѯ�Ķ�����: ");           // ��֤�û�����Ķ�����, ����ѯ����
			scanf("%s", id);
			if ((reservation = getReservation(id)) == NULL) {
				printf(COLOR_RED_B "�ö����Ų����ڣ�" COLOR_RESET);
			}
			else if (strcmp(reservation->touristGroup->id, currentUser.id))
				printf(COLOR_RED_B "�������Ķ�����" COLOR_RESET);
			else break;
		}

		printf("��������\n");
		printf("=====================\n\n");          // �������������Ϣ
		
		printf("Ԥ��ʱ��: " COLOR_GREEN "%s\n" COLOR_RESET, reservation->orderDate);
		printf("����ʱ��: " COLOR_GREEN "%s\n" COLOR_RESET, reservation->time);
		printf("�ÿ����: " COLOR_GREEN);
		if (reservation->idantity == NORMAL)
			printf("Ⱥ��");
		else if (reservation->idantity == STUDENT)
			printf("ѧ��");
		else if (reservation->idantity == SOLDIER)
			printf("����");
		printf("\n" COLOR_RESET);
		printf("����: " COLOR_GREEN);                // ��������еľ�����Ϣ
		for (int i = 0; i != reservation->featureSpotCount; i++) {
			printf("	%s\n", reservation->featureSpots[i].name);
		}
		printf(COLOR_YELLOW_B"	[�� %d ������] \n" COLOR_RESET, reservation->featureSpotCount);

		printf("�ܼ۸�: " COLOR_GREEN "%6.2f\n" COLOR_RESET, reservation->totalPrice);

		
		struct tm rTime;
		rTime.tm_year = atoi(strtok(reservation->time, ":")) - 1900;        // ��������ʱ��
		rTime.tm_mon = atoi(strtok(NULL, ":")) - 1;
		rTime.tm_mday = atoi(strtok(NULL, ":"));
		rTime.tm_hour = atoi(strtok(NULL, ":"));
		rTime.tm_min = atoi(strtok(NULL, ":"));
		rTime.tm_sec = 0;
		time_t rtime = mktime(&rTime);  // �������õ�ʱ��ṹ��ת��Ϊ time_t ����
		time_t ctime = time(NULL);

		int diff = (int)difftime(ctime, rtime);     // ���㵱ǰʱ�������ʱ��Ĳ� (��λ��)

		if (diff > -864000 && diff < 0) {    // �������ʱ��24Сʱ���ڣ� ����ȡ������
			printf(COLOR_YELLOW_B "��ǰ������ȡ��, �밴[x]ȡ���ö���, ���س���������ѯ, �������������˵�\n" COLOR_RESET);
			choice = readKey();
			if ((choice == 'x' || choice == 'X') && inputBool("�Ƿ�Ҫȡ���ö���")) {  // ȡ������
				char condition[100];
				sprintf(condition, "id = '%s'", reservation->id);
				int ret = removeData("Reservation", condition);                       // �����ݿ���ɾ��������Ϣ
				if (ret == 1) 
				{
					printf(COLOR_GREEN_B "\nȡ�������ɹ�\n" COLOR_RESET);

					for (int i = 0; i != reservation->featureSpotCount; i++) {                  // ���¸������ͳ������
						FeatureSpot f = reservation->featureSpots[i];          

						char condition[50];
						char value[50];
						sprintf(condition, "id = '%s'", f.id);

						_itoa(f.reservationCount - 1, value, 10);
						updateData("FeatureSpot", condition, "reservationCount", value, 1);     // ������ -= 1

						_itoa(f.totalTicketSold - reservation->touristGroup->peopleCount, value, 10);
						updateData("FeatureSpot", condition, "totalTicketSold", value, 1);      // ��Ʊ�� -= ����������

						if (isHotSeason()) {
							_itoa(f.hotSeasonTickets - reservation->touristGroup->peopleCount, value, 10);
							updateData("FeatureSpot", condition, "hotSeasonTickets", value, 1); // ������Ʊ�� -= ����������
						}
						else {
							_itoa(f.coldSeasonTickets - reservation->touristGroup->peopleCount, value, 10);
							updateData("FeatureSpot", condition, "coldSeasonTickets", value, 1); // ������Ʊ�� -= ����������
						}

						float price = currentUser.peopleCount * (isHotSeason() ? f.hotSeasonPrice : f.coldSeasonPrice) *
							(reservation->idantity == SOLDIER ? f.soldierDiscount : 1) *         // ����۸�
							(reservation->idantity == STUDENT ? f.studentDiscount : 1);

						_itoa(f.totalProfit - price, value, 10);
						updateData("FeatureSpot", condition, "totalProfit", value, 1);           // ���������� -= �۸�
					}
				}
				else  // ���ݿ������������
				{
					char err[100];
					getError(err);
					printf(COLOR_RED_B "���ݿ����: %s (������: %d)\n" COLOR_RESET, err, -ret);
				}

				break;
			}
		}
		else {
			printf(COLOR_YELLOW_B "��ǰ�����޷�ȡ��, ���ڳ���ǰ24Сʱ��ȡ������. ���س���������ѯ, �������������˵�\n" COLOR_RESET);
			choice = readKey();
		}
	}

	printf("�밴�س����������˵�...\n");
	while (readKey() != '\r');
}

// ��������Ϣ�޸Ĺ���
void ModifyInfo() {
	system("cls");
	printf("�ο�����Ϣ�޸�\n");
	printf("======================\n\n");
	printf(COLOR_RED_B "ע��, ���޸ĵ���Ϣ����Ӱ����Ԥ���Ķ���\n\n" COLOR_RESET);   
	                                                                     // �����������Ϣ
	printf("    �ο���ID: \x1b[32m%s\033[0m\n", currentUser.id);  // �ο���ID�����޸�
	printf(COLOR_YELLOW_B "[1]" COLOR_RESET "     ����: " COLOR_GREEN "******\n" COLOR_RESET);  // ����ʾ�û�����
	printf(COLOR_YELLOW_B "[2]" COLOR_RESET "     ����: " COLOR_GREEN "%d\n" COLOR_RESET, currentUser.peopleCount);
	printf(COLOR_YELLOW_B "[3]" COLOR_RESET "   �����: " COLOR_GREEN "%d\n" COLOR_RESET, currentUser.age);
	printf(COLOR_YELLOW_B "[4]" COLOR_RESET " ��ϵ�绰: " COLOR_GREEN "%s\n\n" COLOR_RESET, currentUser.phone);
	printf(COLOR_YELLOW_B "�밴��ѡ����Ҫ�޸ĵ���Ϣ, �� [0] ȡ���޸�\n" COLOR_RESET);

	char choice = 0;
	while (!(choice = readKey()) || choice > '4' || choice < '0')   // ��ȡ����֤�û�����
		printf(COLOR_RED_B "���İ�������, ������\n" COLOR_RESET);

	char condition[50];
	sprintf(condition, "id = '%s'", currentUser.id);

	int re;
	switch (choice)
	{
		case '1':  // ����
		{
			char password[30];
			char confirm[30];

			while (1)
			{
				inputPassword("������������:", password);  // �޸�����ʱ��Ҫ���û���������������
				inputPassword("����������������:", confirm);

				if (strcmp(password, confirm))
					printf(COLOR_RED_B "�����������벻һ��\n" COLOR_RESET);
				else
					break;
			}
			re = updateData("TouristGroup", condition, "password", password, 0);// ��������

			break;
		}
		case '2':  // ����
		{
			char value[5];
			_itoa(inputInteger("�������µ�����:"), value, 10);

			re = updateData("TouristGroup", condition, "peopleCount", value, 1);// ��������
			break;
		}
		case '3':  // �����
		{
			char age[5];
			_itoa(inputInteger("�������µ������:"), age, 10);

			re = updateData("TouristGroup", condition, "ages", age, 1);// ��������
			break;
		}
		case '4':  // �ֻ���
		{
			char phone[50];

			inputPhone("�������µ��ֻ�����:", phone);
			re = updateData("TouristGroup", condition, "phone", phone, 0); // ��������
			break;
		}
		default:
			break;
	}

	if (choice != '0') { 
		if (re == 1) {
			printf(COLOR_GREEN_B "�޸ĳɹ�\n" COLOR_RESET);
			currentUser = *getTouristGroup(currentUser.id); // ˢ�µ�ǰ����������
		}
		else {
			char err[100];
			getError(err);
			printf(COLOR_RED_B "���ݿ����%s (������: %d)\n" COLOR_RESET, err, -re);
		}
	}

	printf("\n�밴�س����ص���ҳ...\n");
	while (readKey() != '\r');
}

// ������ע�Ṧ��
void TouristGroupRegister() {
	system("cls");
	printf("�ο���ע��\n");
	printf("======================\n\n");

	TouristGroup touristGroup;

	while (1) {
		inputID("������������ID:", touristGroup.id);   // ����������ID, ����֤�Ƿ����
		if (checkIsUnique("TouristGroup", "id", touristGroup.id))
			break;
		else
			printf(COLOR_RED_B "�û����Ѵ��ڣ�\n" COLOR_RESET);
	}

	while (1) {
		char password[30];
		inputPassword("����������:", touristGroup.password); // �����������룬����֤�Ƿ�һ��
		inputPassword("���ٴ���������:", password);
		if (!strcmp(touristGroup.password, password))
			break;
		else
			printf(COLOR_RED_B "�����������벻һ��!\n" COLOR_RESET);
	}

	touristGroup.peopleCount = inputInteger("����������������:");  // ��������

	touristGroup.age = inputInteger("�������ÿ�ƽ������:");       // ����ƽ������

	while (1) {
		inputPhone("��������ϵ�˵绰:", touristGroup.phone);     // ������ϵ�˵绰������֤�Ƿ����
		if (checkIsUnique("TouristGroup", "phone", touristGroup.phone))
			break;
		else
			printf(COLOR_RED_B "����ϵ�˵绰�ѱ�ʹ��!\n" COLOR_RESET);
	}

	int re = addTourstGroup(&touristGroup);               // ���µ���������Ϣ�������ݿ�

	if (re < 1) {     // ����ʱ�����������������Ϣ
		char err[100];
		getError(err);
		printf(COLOR_RED_B "���ݿ����: %s (������: %d )\n" COLOR_RESET, err, -re);
		while (readKey() != '\r');
	}
	else {          // ע��ɹ������������Ź��ܲ˵�
		printf(COLOR_GREEN_B "ע��ɹ�, �밴�س����������˵�\n" COLOR_RESET);
		while (readKey() != '\r');
		currentUser = touristGroup;
		TouristGroupMenu();
	}
}