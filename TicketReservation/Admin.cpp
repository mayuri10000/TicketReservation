/*
	����Ա����ģ��
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

// ����Ա����ģ������ں���
void Admin() 
{
	Login();   // ��ִ�е�¼

	
	while (1) {
		system("cls");              // ÿ�λص����˵�����һ����
		printf("��ǰ����ԱID: " COLOR_GREEN_B "%s\n" COLOR_RESET , currentUser.id);  // ��ʾ����ԱID
		printf("��������������������������������������������������������������������������������������������������������\n");
		printf("��                ����Ա���ܲ˵�                    ��\n");
		printf("��������������������������������������������������������������������������������������������������������\n");
		printf("��               " COLOR_YELLOW_B "[1]" COLOR_RESET " ������Ϣ��ѯ                   ��\n");
		printf("��               " COLOR_YELLOW_B "[2]" COLOR_RESET " ������Ϣ��ѯ                   ��\n");
		printf("��               " COLOR_YELLOW_B "[3]" COLOR_RESET " ��Ӿ���                       ��\n");
		printf("��               " COLOR_YELLOW_B "[4]" COLOR_RESET " ��ӹ���Ա                     ��\n");
		printf("��               " COLOR_YELLOW_B "[5]" COLOR_RESET " ������Ϣ����                   ��\n");
		printf("��               " COLOR_YELLOW_B "[6]" COLOR_RESET " �˳���¼                       ��\n");
		printf("��������������������������������������������������������������������������������������������������������\n");
		printf(COLOR_YELLOW_B "�밴���ּ�ѡ����\n" COLOR_RESET);

		char choice = '\0';
		while (!(choice = readKey()) || choice > '6' || choice < '1')  // ��ȡ����֤�û�ID
			printf(COLOR_RED_B "���İ�������, ������\n" COLOR_RESET);
		

		switch (choice)
		{
			case '1':
			    ListFeatureSpot();  // ��ѯ����
				break;
			case '2':
				ListReservations(); // ��ѯ����
				break;
			case '3':
				AddFeatureSpot();   // ��Ӿ���
				break;
			case '4':
				AddAdmin();         // ��ӹ���Ա
				break;
			case '5':
				ModifyPersonalInfo(); // ������Ϣ�޸�
				break;
			case '6':
				return;
		}
		
	}
}

// ����Ա��¼
void Login()
{
	system("cls");
	printf("����Ա��¼\n");
	printf("========================\n\n");
	while (1) {
		char userName[10];
		char password[30];
		inputStringWithLengthLimit("����ԱID:", 6, 10, userName);  // �����û���������
		inputPassword("����:", password);

		SystemAdmin *user = getSystemAdmin(userName);
		if (user == NULL || strcmp(user->password, password))      // ��֤
			printf(COLOR_RED_B "ID���������, ������\n" COLOR_RESET);
		else {
			currentUser = *user;
			break;
		}
	}
}

// ����Ա��� ��Ϊ�˱���ϵͳ��ȫ���¹���Աע����������й���Ա�˻��½��У�
void AddAdmin() {
	system("cls");
	printf("��ӹ���Ա\n");
	printf("================\n\n");

	SystemAdmin systemAdmin;

	while(1) {
		inputID("�������¹���ԱID:", systemAdmin.id);    // �����¹���ԱID
		if (!checkIsUnique("SystemAdmin", "id", systemAdmin.id))  // ���ID�Ƿ��ѱ�ʹ��
			printf(COLOR_RED_B "��ID�Ѵ���!\n" COLOR_RESET);
		else
			break;
	}

	while (1) {
		char password[50];
		inputPassword("����������:", systemAdmin.password);  // �����¹���Ա����
		inputPassword("���ڴ���������:", password);
		if (strcmp(systemAdmin.password, password))   // ��֤���������Ƿ�һ��
			printf(COLOR_RED_B "�����������һ��!\n" COLOR_RESET);
		else
			break;
	}

	while (1) {
		inputEmail("�����������ַ:", systemAdmin.email);   // �����¹���Ա����
		if (!checkIsUnique("SystemAdmin", "email", systemAdmin.email))  // ��������Ƿ��ѱ�ʹ��
			printf(COLOR_RED_B "�������ַ�Ѵ���!\n" COLOR_RESET);
		else
			break;
	}

	while (1) {
		inputPhone("�������ֻ�����:", systemAdmin.phone);   // �����¹���Ա�ֻ���
		if (!checkIsUnique("SystemAdmin", "phone", systemAdmin.phone)) // ��֤�ֻ����Ƿ��ѱ�ʹ��
			printf(COLOR_RED_B "���ֻ������Ѵ���!\n" COLOR_RESET);
		else
			break;
	}

	if (addSystemAdmin(&systemAdmin) != 1) {   // ���¹���Ա�˺Ŵ������ݿ�, �������ݿ��������ʾ
		char error[100];
		getError(error);
		printf(COLOR_RED_B "���ݿ����: %s \n" COLOR_RESET, error);
	}
	else {
		printf(COLOR_GREEN_B "��ӳɹ�\n" COLOR_RESET);
	}

	printf("�밴�س����������˵�...\n");
	while (readKey() != '\r');
}

// ������ѯ
void ListReservations() {
	Reservation *reservations;
	int count = getReservationsBy(NULL, 0, NULL, &reservations);

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

	printf("������                     ��������      ����ʱ��         �ο����      �ܼ�\n");
	printf("================================================================================\n");

	for (int i = 0; i != count; i++) {          // �г����ж���
		Reservation reservation = reservations[i];
		char fsName[20];
		const char *idantity;
		if (reservation.featureSpotCount == 1)  // �������ֻ����һ�����㣬������þ������ƣ����������������
			strcpy(fsName, reservation.featureSpots->name);
		else
			sprintf(fsName, COLOR_GREEN "[%d������] " COLOR_RESET, reservation.featureSpotCount);

		if (reservation.idantity == STUDENT)   // �ÿ����
			idantity = "ѧ��";
		else if (reservation.idantity == SOLDIER)
			idantity = "����";
		else
			idantity = "Ⱥ��";

		printf("%-26s%-10s%-16s%10s  %6.2f\n", reservation.id, fsName, reservation.time, idantity,
			reservation.totalPrice);   // ���һ��������Ϣ
	}

	printf(COLOR_YELLOW_B "\n�� %d ����¼, �밴�س�����ѯ��������, ���������������˵�\n" COLOR_RESET, count);

	char choice = readKey();  

	while (choice == '\r') {  // ��ѯ��������
		Reservation *reservation = NULL;

		while (1) {
			char id[30];
			printf("������Ҫ��ѯ�Ķ�����: ");  // ����Ҫ��ѯ�Ķ���ID
			scanf("%s", id);
			if ((reservation = getReservation(id)) == NULL) { // ����Ƿ����
				printf(COLOR_RED_B "�ö����Ų����ڣ�" COLOR_RESET);
			}
			else break;
		}

		printf("��������\n");
		printf("=====================\n\n");

		printf("Ԥ��ʱ��: " COLOR_GREEN "%s\n" COLOR_RESET, reservation->orderDate);  // ���������Ϣ
		printf("����ʱ��: " COLOR_GREEN "%s\n" COLOR_RESET, reservation->time);
		printf("������ID: " COLOR_GREEN "%s\n" COLOR_RESET, reservation->touristGroup->id);
		printf("�ÿ����: " COLOR_GREEN);
		if (reservation->idantity == NORMAL)
			printf("Ⱥ��");
		else if (reservation->idantity == STUDENT)
			printf("ѧ��");
		else if (reservation->idantity == SOLDIER)
			printf("����");
		printf("\n" COLOR_RESET);
		printf("����: " COLOR_GREEN);
		for (int i = 0; i != reservation->featureSpotCount; i++) {  // ��������а��������о���
			printf("	%s\n", reservation->featureSpots[i].name);
		}
		printf(COLOR_YELLOW_B"	[�� %d ������]\n" COLOR_RESET, reservation->featureSpotCount);

		printf("�ܼ۸�: " COLOR_GREEN "%6.2f\n" COLOR_RESET, reservation->totalPrice);


		printf(COLOR_YELLOW_B "�밴�س���������ѯ, �������������˵�\n" COLOR_RESET);
		choice = readKey();  // �ڴζ�ȡ�û���������Ϊ�س�������ѭ��һ��
		
	}

	printf("�밴�س����������˵�...\n");
	while (readKey() != '\r');
}

// �������
void AddFeatureSpot() {
	FeatureSpot featurespot;

	system("cls");
	printf("��Ӿ���\n");
	printf("======================\n\n");

	while (1) {
		inputStringWithLengthLimit("����ID:", 6, 6, featurespot.id);  // ���벢����¾���ID
		if (!checkIsUnique("FeatureSpot", "id", featurespot.id))
			printf(COLOR_RED_B "��ID�Ѵ��ڣ�\n" COLOR_RESET );
		else
			break;
	}

	while (1) {
		inputStringWithLengthLimit("��������:", 1, 20, featurespot.name);  // ���벢����¾�������
		if (!checkIsUnique("FeatureSpot", "name", featurespot.name))
			printf(COLOR_RED_B "�������Ѵ��ڣ�\033[0m\n" COLOR_RESET);
		else
			break;
	}

	inputStringWithLengthLimit("������:", 1, 200, featurespot.discription);  // �����¾����������Ϣ

	featurespot.coldSeasonPrice = inputInteger("����Ʊ��:");

	featurespot.hotSeasonPrice = inputInteger("����Ʊ��:");

	featurespot.timeRequired = inputInteger("��������ʱ�� (Сʱ):");
	
	featurespot.soldierDiscount = inputFloat("����Ʊ���ۿ�:");

	featurespot.studentDiscount = inputFloat("ѧ��Ʊ���ۿ�:");

	featurespot.level = inputInteger("���㼶��:");

	inputStringWithLengthLimit("���ڵ���:", 1, 20, featurespot.district);

	featurespot.maintenanceFee = inputFloat("ά������:");

	

	featurespot.totalTicketCount = inputInteger("����Ʊ��:");

	featurespot.totalProfit = -featurespot.maintenanceFee;  // ��ʼʱ�������е���������Ϊά�����õĸ�ֵ���Ժ�ÿ�����¶���ʱ��ֱ�ӽ����������Ʊ�ۼ���

	featurespot.isSuitableForEldersAndChildren = inputBool("�Ƿ��ʺ����˼���ͯ");


	char option = 0;
	do {
		system("cls");
		printf("��Ӿ�������������Ϣ\n");                     // ��ʼ��Ӿ�������������Ϣ��ÿ���������10��
		printf("======================\n\n");

		printf("��ǰ����: \x1b[01;32m%s %s\033[0m\n", featurespot.id, featurespot.name);
		printf("\033[01;33m��һ��������� 10 ������������Ϣ, ��[d]�����˲���\033[0m\n");
		
		option = readKey();
		featurespot.limitationCount = 0;
		featurespot.limitations = (FeatureSpotLimitation*)malloc(sizeof(FeatureSpotLimitation) * 10);
		while (option != 'd' && option != 'D') {
			FeatureSpotLimitation limition;

			strcpy(limition.featureSpotId, featurespot.id);  

			limition.isHot = inputBool("�Ƿ�Ϊ����");   // ���뾰���������������Ϣ�����ȣ�ʱ�Σ�����������

			inputTime("��ʼʱ��:", limition.timeSpanStart);

			inputTime("����ʱ��:", limition.timeSpanEnd);

			limition.peopleCount = inputInteger("��������:");

			featurespot.limitations[featurespot.limitationCount] = limition;
			printf("\033[01;33m����������� %d ������������Ϣ, ��[d]������, [x]ȡ�����, ���س��������\033[0m\n",
				10 - (featurespot.limitationCount + 1));   // ÿ�����һ����ѯ���Ƿ�������
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

			printf("\x1b[01;31mע�⣺�ο�����������Ϣ�ύ���޷��ٽ����κ��޸���ɾ������ȷ�ϸ�����Ϣ��ȷ��\033[0m\n\n");
			int confirm = inputBool("�Ƿ�ȷ�����");  // ȷ�Ͼ�������������Ϣ
			if (confirm)
				break;
			else
				option = 'x';
		}
	} while (option == 'X' || option == 'x');

	system("cls");
	printf("��ȷ�Ͼ�����Ϣ\n");
	printf("======================\n\n");
	printf(COLOR_RED_B "ע�⣺������Ϣ�������޷��ٽ����κ��޸���ɾ������ȷ�ϸ�����Ϣ��ȷ���ٷ�����\n\n" COLOR_RESET);

	printf("����ID: "        COLOR_GREEN "%s\n"       COLOR_RESET, featurespot.id);         // �ù���Աȷ�Ͼ�����Ϣ��Ȼ��ѡ���Ƿ񷢲�
	printf("��������: "      COLOR_GREEN "%s\n"       COLOR_RESET, featurespot.name);       // �¾��������Ϣ
	printf("������: "      COLOR_GREEN "%s\n"       COLOR_RESET, featurespot.discription);
	printf("����Ʊ��: "      COLOR_GREEN "�� %d\n"    COLOR_RESET, featurespot.coldSeasonPrice);
	printf("����Ʊ��: "      COLOR_GREEN "�� %d\n"    COLOR_RESET, featurespot.hotSeasonPrice);
	printf("��������ʱ��: "  COLOR_GREEN "%d\n"       COLOR_RESET, featurespot.timeRequired);
	printf("����Ʊ���ۿ�: "  COLOR_GREEN "%1.2f\n"    COLOR_RESET, featurespot.soldierDiscount);
	printf("ѧ��Ʊ���ۿ�: "  COLOR_GREEN "%1.2f\n"    COLOR_RESET, featurespot.studentDiscount);
	printf("����ȼ�: "      COLOR_GREEN "%d\n"       COLOR_RESET, featurespot.level);
	printf("���ڵ���: "      COLOR_GREEN "%s\n"       COLOR_RESET, featurespot.district);
	printf("ά������: "      COLOR_GREEN "�� %6.2f\n" COLOR_RESET, featurespot.maintenanceFee);
	printf("����Ʊ��: "      COLOR_GREEN "%d\n"       COLOR_RESET, featurespot.totalTicketCount);
	printf("�ʺ����˼���ͯ: " COLOR_GREEN "%d\n\n"    COLOR_RESET, featurespot.isSuitableForEldersAndChildren);
	
	printf("������    ��ʼʱ��   ����ʱ��   ��������\n");
	printf("=========================================\n");            // ����������Ϣ
	for (int i = 0; i != featurespot.limitationCount; i++) {
		FeatureSpotLimitation lim = featurespot.limitations[i];
		printf(" %s      %s      %s        %d\n", lim.isHot ? "����" : "����", 
			lim.timeSpanStart, lim.timeSpanEnd, lim.peopleCount);
	}
	printf("�� %d ������������Ϣ\n", featurespot.limitationCount);

	if (inputBool("ȷ�Ϸ���")) {  // ѯ���Ƿ�ȷ�Ϸ���
		int re = addFeatureSpot(&featurespot);   // ���µľ�����Ϣ�������ݿ�
		if (re != 1) {
			char err[100];
			getError(err);
			printf(COLOR_RED_B "���ݿ����%s (������: %d)\n" COLOR_RESET, err, -re);
		}
		else {
			printf(COLOR_GREEN_B "�����ɹ�\n" COLOR_RESET);
		}
	}

	printf("\n�밴�س����ص�����Ա��ҳ...\n");
	while (readKey() != '\r');
}

// ������Ϣ��ѯ
void ListFeatureSpot() {
	system("cls");
	printf("������Ϣ��ѯ\n");
	printf("======================\n\n");

	printf(COLOR_YELLOW_B "��ѡ������ʽ\n" COLOR_RESET);         // ѯ������ʽ
	printf(COLOR_YELLOW_B "[1]" COLOR_RESET " ������\n");
	printf(COLOR_YELLOW_B "[2]" COLOR_RESET " ��Ԥ������������\n");
	printf(COLOR_YELLOW_B "[3]" COLOR_RESET " ��������������\n");
	printf(COLOR_YELLOW_B "[4]" COLOR_RESET " ����������\n");
	printf(COLOR_YELLOW_B "[5]" COLOR_RESET " ��������Ʊ������\n");
	printf(COLOR_YELLOW_B "[6]" COLOR_RESET " ��������Ʊ������\n");

	const char *sortBy = NULL;   // Ҫ������ֶ���
	int sort = 1;                // �Ƿ������Լ������ǽ���
	char option = 1;

	while (!(option = readKey()) || option > '6' || option < '1')  // ��ȡ�û�ѡ��
		printf(COLOR_RED_B "���İ�������, ������\n" COLOR_RESET);

	switch (option) {
		case '1':
			sort = 0;
			break;
		case '2':
			sortBy = "reservationCount";   // ��Ԥ��������
			break;
		case '3':
			sortBy = "visitCount";         // �������� (�û���ѯ�þ�������Ĵ���)���� 
			break;
		case '4':
			sortBy = "totalProfit";        // ������������
			break;
		case '5':
			sortBy = "coldSeasonTickets";  // ��������Ʊ������
			break;
		case '6':
			sortBy = "hotSeasonTickets";   // ��������Ʊ������
			break;
		default:
			sort = 0;
	}

	if (sort) {
		printf(COLOR_YELLOW_B "\n\n��ѡ����������\n" COLOR_RESET);  // ѯ������ʽ
		printf(COLOR_YELLOW_B "[1]" COLOR_RESET " ��������\n");
		printf(COLOR_YELLOW_B "[2]" COLOR_RESET " ��������\n");
		 
		while (!(option = readKey()) || option > '2' || option < '1') // ��ȡѡ��
			printf(COLOR_RED_B "���İ�������, ������\n" COLOR_RESET); 

		switch (option) {
			case '1':
				sort = 1;  // ��������
				break;
			case '2':
				sort = -1; // ��������
				break;
			default:
				sort = 0;
		}
	}


	system("cls");

	FeatureSpot *featurespots;  
	int featureSpotCount = getFeatureSpotsBy(NULL, sort, sortBy, &featurespots); // �����ݿ��л�ȡ���о�����Ϣ

	if (featureSpotCount < 0) {  
		char err[100];
		getError(err);
		printf(COLOR_RED_B "���ݿ����%s (������: %d)\n" COLOR_RESET, err, -featureSpotCount);
	}
	else {
		printf("ID     ��������       �ȼ�   ��Ԥ����  ������   ������Ʊ��  ������Ʊ��  ������\n");
		printf("=============================================================================\n");
		
		for (int i = 0; i != featureSpotCount; i++) {   // ���������Ϣ����ͳ����Ϣ
			FeatureSpot f = featurespots[i];
			printf(COLOR_YELLOW_B "%s" COLOR_RESET " %-14s%5d%10d%8d%14d%11d  %6.2f\n", f.id, f.name, f.level, f.reservationCount,
				f.visitCount, f.coldSeasonTickets, f.hotSeasonTickets, f.totalProfit);
		}

		free(featurespots);          // �ͷž�����Ϣ������ռ�õ��ڴ�ռ�

		printf(COLOR_YELLOW_B "�� %d ����¼, �밴[0]��ѯ��������, �����������ع���Ա�˵�\n" COLOR_RESET, featureSpotCount);
		option = readKey();
		if (option == '0') {         // ���������ѯ
			while (option != 'e') {
				char id[10];
				printf("������Ҫ��ѯ�ľ���ID: ");  // ��ȡ����ID
				scanf("%s", id);
				FeatureSpot *f = getFeatureSpot(id); // ���������ID�����ݿ��л�ȡ������Ϣ
				if (f == NULL) 
					printf(COLOR_RED_B "δ�ҵ��þ�����Ϣ��\n" COLOR_RESET);
				else {
					printf("\n������Ϣ\n");
					printf("===============\n\n");

					printf("����ID: "        COLOR_GREEN "%s\n"       COLOR_RESET, f->id);    // ������������Ϣ
					printf("��������: "      COLOR_GREEN "%s\n"       COLOR_RESET, f->name);
					printf("������: "      COLOR_GREEN "%s\n"       COLOR_RESET, f->discription);
					printf("����Ʊ��: "      COLOR_GREEN "%d\n"    COLOR_RESET, f->coldSeasonPrice);
					printf("����Ʊ��: "      COLOR_GREEN "%d\n"    COLOR_RESET, f->hotSeasonPrice);
					printf("��������ʱ��: "  COLOR_GREEN "%d\n"       COLOR_RESET, f->timeRequired);
					printf("����Ʊ���ۿ�: "  COLOR_GREEN "%1.2f\n"    COLOR_RESET, f->soldierDiscount);
					printf("ѧ��Ʊ���ۿ�: "  COLOR_GREEN "%1.2f\n"    COLOR_RESET, f->studentDiscount);
					printf("����ȼ�: "      COLOR_GREEN "%d\n"       COLOR_RESET, f->level);
					printf("���ڵ���: "      COLOR_GREEN "%s\n"       COLOR_RESET, f->district);
					printf("ά������: "      COLOR_GREEN "%6.2f\n" COLOR_RESET, f->maintenanceFee);
					printf("����Ʊ��: "      COLOR_GREEN "%d\n"       COLOR_RESET, f->totalTicketCount);
					printf("�ʺ����˼���ͯ: " COLOR_GREEN "%d\n\n"    COLOR_RESET, f->isSuitableForEldersAndChildren);

					printf("Ӫҵ����\n");
					printf("==================\n\n");

					printf("�ܶ�����: "      COLOR_GREEN "%d\n"       COLOR_RESET, f->reservationCount);  // ����Ӫҵͳ������
					printf("����Ʊ��: "      COLOR_GREEN "%d\n"       COLOR_RESET, f->totalTicketSold);
					printf("������Ʊ��: "    COLOR_GREEN "%d\n"       COLOR_RESET, f->coldSeasonTickets);
					printf("������Ʊ��: "    COLOR_GREEN "%d\n"       COLOR_RESET, f->hotSeasonTickets);
					printf("������: "        COLOR_GREEN "%6.2f\n\n"    COLOR_RESET, f->totalProfit);

					Reservation *reservations;
					int count = getReservationsForFeatureSpot(f->id, &reservations);                        // �þ����ȡ���ж���������ʾ�ÿ�����ֲ�
					if (count < 0) {
						char err[100];
						getError(err);
						printf(COLOR_RED_B "���ݿ����: %s (������: %d)\n" COLOR_RESET, err, -count);
						return;
					}

					int ageUnder14 = 0;  // 14������
					int age15To30 = 0;   // 15-30��
					int age31To40 = 0;   // 31-40��
					int age41To60 = 0;   // 41-60��
					int ageUp61 = 0;     // 61������

					for (int i = 0; i != count; i++) {  // ������������
						Reservation reservation = reservations[i];
						int age = reservation.touristGroup->age;   // �ÿ������
						int people = reservation.touristGroup->peopleCount;  // ����
						if (age <= 14)
							ageUnder14 += people;     // ����Ӧ����ε��������ϸö�������
						else if (age >= 15 && age <= 30)
							age15To30 += people;
						else if (age >= 31 && age <= 40)
							age31To40 += people;
						else if (age >= 41 && age <= 60)
							age41To60 += people;
						else if (age >= 61)
							ageUp61 += people;
					}

					printf("�����ÿ�����ηֲ�\n");
					printf("=======================\n");

					printf("14������  %d ��\n", ageUnder14);   // ��ʾ����ֲ�
					printf("15-30��   %d ��\n", age15To30);
					printf("31-40��   %d ��\n", age31To40);
					printf("41-60��   %d ��\n", age31To40);
					printf("61������  %d ��\n\n", ageUp61);

					printf(COLOR_YELLOW_B "�������������ѯ����[e]�˳�\n" COLOR_RESET);
					option = readKey();   // ��ȡ�û���������Ϊ�س�������ѯ
				}
			}
		}
	}


	printf("\n�밴�س����ص�����Ա��ҳ...\n");
	while (readKey() != '\r');
}

// ������Ϣ�޸�
void ModifyPersonalInfo() 
{
	system("cls");
	printf("������Ϣ����\n");
	printf("======================\n\n");

	printf("    ����ԱID: \x1b[32m%s\033[0m\n", currentUser.id);  // �����ǰ����Ա������Ϣ
	printf(COLOR_YELLOW_B "[1]" COLOR_RESET "     ����: " COLOR_GREEN "******\n" COLOR_RESET);  // ����ʾ����
	printf(COLOR_YELLOW_B "[2]" COLOR_RESET " ��������: " COLOR_GREEN "%s\n" COLOR_RESET, currentUser.email); 
	printf(COLOR_YELLOW_B "[3]" COLOR_RESET " �ֻ�����: " COLOR_GREEN "%s\n\n" COLOR_RESET, currentUser.phone);
	printf(COLOR_YELLOW_B "�밴��ѡ����Ҫ�޸ĵ���Ϣ, �� [4] ȡ���޸�\n" COLOR_RESET);

	char choice = 0;
	while (!(choice = readKey()) || choice > '4' || choice < '1')
		printf(COLOR_RED_B "���İ�������, ������\n" COLOR_RESET);

	char condition[50];
	sprintf(condition, "id = '%s'", currentUser.id);

	int re;
	switch (choice) 
	{
		case '1':    // ����
		{
			char password[30];
			char confirm[30];

			while (1) 
			{
				inputPassword("������������:", password);  // ��������
				inputPassword("����������������:", confirm); // �ڴ���������

				if (strcmp(password, confirm))  // ��֤�������������Ƿ�һ��
					printf(COLOR_RED_B "�����������벻һ��\n" COLOR_RESET);
				else
					break;
			}
			re = updateData("SystemAdmin", condition, "password", password, 0);  // ��������

			break;
		}
		case '2':
		{
			char email[50];

			inputEmail("�������µ������ַ:", email);

			re = updateData("SystemAdmin", condition, "email", email, 0);
			break;
		}
		case '3':
		{
			char phone[50];
			inputPhone("�������µ��ֻ�����:", phone);

			re = updateData("SystemAdmin", condition, "phone", phone, 0);
			break;
		}
		default: 
			break;
	}

	if (choice != '4') {
		if (re == 1) {
			printf(COLOR_GREEN_B "�޸ĳɹ�\n" COLOR_RESET);
			currentUser = *getSystemAdmin(currentUser.id);
		}
		else {
			char err[100];
			getError(err);
			printf(COLOR_RED_B "���ݿ����%s (������: %d)\n" COLOR_RESET, err, -re);
		}
	}

	printf("\n�밴�س����ص�����Ա��ҳ...\n");
	while (readKey() != '\r');
}
