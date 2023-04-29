/*
	   ______  ______  ______       ______  ______  ______       ______  ______  ______
	  /_  __/ /_  __/ / ____/      /_  __/ / __  / / ____/      /_  __/ / __  / / ____/
	   / /     / /   / /     ___    / /   / / / / / /     ___    / /   / / / / / /___
	  / /     / /   / /     /__/   / /   / /_/ / / /     /__/   / /   / / / / / ____/
	 / /   __/ /_  / /___         / /   / __  / / /___         / /   / /_/ / / /___
	/_/   /_____/ /_____/        /_/   /_/ /_/ /_____/        /_/   /_____/ /_____/

	by Mathshyun!
*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#define PLAYER 0
#define AI 1
#define TEST 2

//3��3ĭ������ ��ǥ�� ��Ÿ���� ����ü
struct point {
	int x, y;
};

//��, ��, ���� ������ ����ü
struct numbers {
	int Xwin, Owin, draw;
};

int title();																				//Ÿ��Ʋ �޽����� ����ϴ� �Լ�
int decideOrder();																			//PvA���� ������ ���ϴ� �Լ�
int isGameEnd(int map[3][3], int x, int y);													//������ �������� Ȯ���ϴ� �Լ�
int pauseScreen();																			//�Ͻ� ���� ȭ���� ǥ���ϴ� �Լ�
int isFork(int map[3][3], int turn);														//��ũ ����(���� �� ���� ����)���� Ȯ���ϴ� �Լ�
void startGame(int type);																	//������ �����ϴ� �Լ�
void endGame(int map[3][3], int p[2], int wp, int type, int turn, struct point pointer);	//������ ������ �Լ�
void printScreen(int map[3][3], int p[2], int type, int turn, struct point pointer);		//���� ȭ���� ǥ���ϴ� �Լ�
struct point TTTAI(int map[3][3], int turn);												//AI�� ������ �����ϴ� �Լ�

int main() {
	system("mode con:cols=100 lines=30");	//â ũ�� ����
	system("title TIC-TAC-TOE");			//â ���� ����
	srand(time(NULL));						//���� ������ �õ�(seed) ����
	while (1) {
		int x = title();					//Ÿ��Ʋ �޽����� ǥ���� ��ȯ���� x�� ����
		switch (x) {
		case 2:
			return 0;						//���� ����
		default:
			startGame(x);					//���� ����
		}
	}
	return 0;
}

//Ÿ��Ʋ �޽����� ����ϴ� �Լ�
int title() {
	int x, selected = 0;

	while (1) {
		system("cls");
		printf("����������������������������������������������������������������������������������������������������\n");
		printf("��         ______  ______  ______       ______  ______  ______       ______  ______  ______       ��\n");
		printf("��        /_  __/ /_  __/ / ____/      /_  __/ / __  / / ____/      /_  __/ / __  / / ____/       ��\n");
		printf("��         / /     / /   / /     ___    / /   / / / / / /     ___    / /   / / / / / /___         ��\n");
		printf("��        / /     / /   / /     /__/   / /   / /_/ / / /     /__/   / /   / / / / / ____/         ��\n");
		printf("��       / /   __/ /_  / /___         / /   / __  / / /___         / /   / /_/ / / /___           ��\n");
		printf("��      /_/   /_____/ /_____/        /_/   /_/ /_/ /_____/        /_/   /_____/ /_____/           ��\n");
		printf("��                                                                                                ��\n");
		printf("����������������������������������������������������������������������������������������������������\n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                    %s �÷��̾� vs. �÷��̾� %s                                     \n", (selected == 0) ? "��" : "  ", (selected == 0) ? "��" : "  ");
		printf("                                       %s �÷��̾� vs. AI %s                                        \n", (selected == 1) ? "��" : "  ", (selected == 1) ? "��" : "  ");
		printf("                                           %s ������ %s                                             \n", (selected == 2) ? "��" : "  ", (selected == 2) ? "��" : "  ");
		printf("                             ����Ű�� Ŀ���� �����̰� ���ͷ� �����ϼ���                             \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf(" by Mathshyun!                                                                                      \n");
		switch (x = getch()) {
		case 224:
			x = getch();
			printf("");
			if (x == 72) {
				if (selected == 0) selected = 2;
				else selected--;
			}
			else if (x == 80) {
				if (selected == 2) selected = 0;
				else selected++;
			}
			break;
		case 13:
			return selected;
		}
	}
}

//PvA���� ������ ���ϴ� �Լ�
int decideOrder() {
	int x, selected = 0;

	while (1) {
		system("cls");
		printf("����������������������������������������������������������������������������������������������������\n");
		printf("��         ______  ______  ______       ______  ______  ______       ______  ______  ______       ��\n");
		printf("��        /_  __/ /_  __/ / ____/      /_  __/ / __  / / ____/      /_  __/ / __  / / ____/       ��\n");
		printf("��         / /     / /   / /     ___    / /   / / / / / /     ___    / /   / / / / / /___         ��\n");
		printf("��        / /     / /   / /     /__/   / /   / /_/ / / /     /__/   / /   / / / / / ____/         ��\n");
		printf("��       / /   __/ /_  / /___         / /   / __  / / /___         / /   / /_/ / / /___           ��\n");
		printf("��      /_/   /_____/ /_____/        /_/   /_/ /_/ /_____/        /_/   /_____/ /_____/           ��\n");
		printf("��                                                                                                ��\n");
		printf("����������������������������������������������������������������������������������������������������\n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                            %s ���� %s                                              \n", (selected == 0) ? "��" : "  ", (selected == 0) ? "��" : "  ");
		printf("                                            %s �İ� %s                                              \n", (selected == 1) ? "��" : "  ", (selected == 1) ? "��" : "  ");
		printf("                                           %s ������ %s                                             \n", (selected == 2) ? "��" : "  ", (selected == 2) ? "��" : "  ");
		printf("                                            %s ��� %s                                              \n", (selected == 3) ? "��" : "  ", (selected == 3) ? "��" : "  ");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		switch (x = getch()) {
		case 224:
			x = getch();
			printf("");
			if (x == 72 && selected != 0) selected--;
			else if (x == 80 && selected != 3) selected++;
			break;
		case 13:
			switch (selected) {
			case 0:					//�÷��̾� ����
				return PLAYER;
			case 1:					//�÷��̾� �İ�
				return AI;
			case 2:					//������
				return rand() % 2;
			case 3:					//���
				return -1;
			}
		}
	}
}

//������ �������� Ȯ���ϴ� �Լ�
int isGameEnd(int map[3][3], int x, int y) {
	int cnt = 0;
	for (int i = 0; i < 3; i++) {
		if (map[i][y] != map[x][y]) break;
		cnt++;
	}
	if (cnt == 3) return map[x][y];
	cnt = 0;
	for (int i = 0; i < 3; i++) {
		if (map[x][i] != map[x][y]) break;
		cnt++;
	}
	if (cnt == 3) return map[x][y];
	cnt = 0;
	if ((x + y) % 2 == 0) {
		cnt = 0;
		if (x == y) {
			for (int i = 0; i < 3; i++) {
				if (map[i][i] != map[x][y]) break;
				cnt++;
			}
		}
		if (cnt == 3) return map[x][y];
		cnt = 0;
		if (x + y == 2) {
			for (int i = 0; i < 3; i++) {
				if (map[i][2 - i] != map[x][y]) break;
				cnt++;
			}
		}
		if (cnt == 3) return map[x][y];
	}
	return 0;
}

//������ �����ϴ� �Լ�
void startGame(int type) {
	struct point pointer;			//���� ǥ���� ĭ
	int map[3][3] = { 0 };			//���� �����ϴ� �迭
	int cnt = 0, turn = 1, p[2];
	clock_t prevTime;				//���� �ð��� ��Ÿ���� ���� ���

	switch (type) {
	case 0:							//PvP
		p[0] = p[1] = PLAYER;
		break;
	case 1:							//PvA
		p[0] = decideOrder();
		if (p[0] == -1) return;		//���� ���� â���� ����� ��� Ÿ��Ʋ��
		p[1] = (p[0] + 1) % 2;
		break;
	}

	pointer.x = 1;
	pointer.y = 1;
	printScreen(map, p, type, turn - 1, pointer);
	prevTime = clock();
	while (1) {
		if (kbhit()) {															//Ű���� �Է��� �ִ��� �˻�
			switch (getch()) {
			case 224:
				if (p[turn - 1] == PLAYER) {									//�÷��̾��� �����̸�
					switch (getch()) {
					case 72:													//��
						if (pointer.y == 0) continue;
						pointer.y--;
						break;
					case 75:													//��
						if (pointer.x == 0) continue;
						pointer.x--;
						break;
					case 77:													//��
						if (pointer.x == 2) continue;
						pointer.x++;
						break;
					case 80:													//��
						if (pointer.y == 2) continue;
						pointer.y++;
					}
				}
				printScreen(map, p, type, turn - 1, pointer);
				break;
			case 13:															//����
				if (p[turn - 1] == PLAYER && map[pointer.x][pointer.y] == 0) {
					int x;
					map[pointer.x][pointer.y] = turn;
					if (p[turn % 2] == AI) prevTime = clock();
					turn = turn % 2 + 1;
					x = isGameEnd(map, pointer.x, pointer.y);
					if (x != 0) {
						endGame(map, p, x, type, turn, pointer);
						return;
					}
					cnt++;
					if (cnt == 9) {
						endGame(map, p, 0, type, turn, pointer);
						return;
					}
					printScreen(map, p, type, turn - 1, pointer);
				}
				break;
			case 27:															//ESC
				if (pauseScreen() == 0) {
					printScreen(map, p, type, turn - 1, pointer);
					break;
				}
				else return;
			}
		}
		else {																	//Ű���� �Է��� ������
			if (p[turn - 1] == AI) {
				if ((clock() - prevTime) / CLOCKS_PER_SEC >= 1) {
					int x;
					struct point pointAI;
					pointAI = TTTAI(map, turn);
					map[pointAI.x][pointAI.y] = turn;
					turn = turn % 2 + 1;
					x = isGameEnd(map, pointAI.x, pointAI.y);
					if (x != 0) {
						endGame(map, p, x, type, turn, pointer);
						return;
					}
					cnt++;
					if (cnt == 9) {
						endGame(map, p, 0, type, turn, pointer);
						return;
					}
					printScreen(map, p, type, turn - 1, pointer);
				}
			}
		}
	}
}

//������ ������ �Լ�
void endGame(int map[3][3], int p[2], int wp, int type, int turn, struct point pointer) {
	system("cls");
	printf("����������������������������������������������������������������������������������������������������\n");
	printf("����������������������������������������������������������������������������������������������������\n");
	printf("����      %s      ��      %s      ��      %s      ����                                          ����\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "��" : "  ");
	printf("����  %s%s%s%s%s  ��  %s%s%s%s%s  ��  %s%s%s%s%s  ���� %s %s %s                            ����\n", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (turn == 0) ? "��" : "  ", (p[0] == PLAYER) ? "�÷��̾�" : "   AI   ", (type == 1) ? " " : "1");
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  %s%s  %s%s  ���� %s %s %s                            ����\n", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (turn == 1) ? "��" : "  ", (p[1] == PLAYER) ? "�÷��̾�" : "   AI   ", (type == 1) ? " " : "2");
	printf("����%s%s  %s  %s%s��%s%s  %s  %s%s��%s%s  %s  %s%s����                                          ����\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "��" : "  ");
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  %s%s  %s%s  ��������������������������������������������������\n", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ");
	printf("����  %s%s%s%s%s  ��  %s%s%s%s%s  ��  %s%s%s%s%s  ��                                              ��\n", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ");
	printf("����      %s      ��      %s      ��      %s      ��                                              ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "��" : "  ");
	printf("����������������������������������������������������                                              ��\n");
	printf("����      %s      ��      %s      ��  ������������������������                                    ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "��" : "  ");
	printf("����  %s%s%s%s%s  ��  %s%s%s%s%s  ��  ��                    ��                                    ��\n", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ");
	if (wp == 0)
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  ��       ���º�       ��                                    ��\n", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ");
	else if (p[0] == p[1])
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  ��   %s %d ��!   ��                                    ��\n", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (p[wp - 1] == PLAYER) ? "�÷��̾�" : "   AI   ", wp);
	else
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  ��    %s ��!    ��                                    ��\n", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (p[wp - 1] == PLAYER) ? "�÷��̾�" : "   AI   ");
	printf("����%s%s  %s  %s%s��%s%s  %s  %s%s��%s��                    ��                                    ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 1) ? "��" : "  ");
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  ������������������������                                    ��\n", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ");
	printf("����  %s%s%s%s%s  ��  %s%s%s%s%s  ��  %s%s%s%s%s  ��                                              ��\n", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[2][1] == 1) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (map[2][1] == 1) ? "��" : "  ");
	printf("����      %s      ��      %s      ��      %s      ��                                              ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 1) ? "��" : "  ");
	printf("����������������������������������������������������                                              ��\n");
	printf("����      %s      ��      %s      ��      %s      ��                                              ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "��" : "  ");
	printf("����  %s%s%s%s%s  ��  %s%s%s%s%s  ��  %s%s%s%s%s  ��                                              ��\n", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ");
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  %s%s  %s%s  ��                                              ��\n", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ");
	printf("����%s%s  %s  %s%s��%s%s  %s  %s%s��%s%s  %s  %s%s��                                              ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "��" : "  ");
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  %s%s  %s%s  ��                                              ��\n", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ");
	printf("����  %s%s%s%s%s  ��  %s%s%s%s%s  ��  %s%s%s%s%s  ��                                              ��\n", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ");
	printf("����      %s      ��      %s      ��      %s      ��                                              ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "��" : "  ");
	printf("����������������������������������������������������                                              ��\n");
	printf("����������������������������������������������������������������������������������������������������\n");
	printf("��                                                                                                ��\n");
	printf("����������������������������������������������������������������������������������������������������\n");
	getch();
}

//�Ͻ� ���� ȭ���� ǥ���ϴ� �Լ�
int pauseScreen() {
	int x, selected = 0;
	while (1) {
		system("cls");
		printf("����������������������������������������������������������������������������������������������������\n");
		printf("����������������������������������������������������������������������������������������������������\n");
		printf("����              ��              ��              ����                                          ����\n");
		printf("����              ��              ��              ����                                          ����\n");
		printf("����              ��              ��              ����                                          ����\n");
		printf("����              ��              ��              ����                                          ����\n");
		printf("����              ��              ��              ��������������������������������������������������\n");
		printf("����              ��              ��              ��                                              ��\n");
		printf("����              ��              ��              ��                                              ��\n");
		printf("����������������������������������������������������                                              ��\n");
		printf("����              ��              ��    ��������������������                                      ��\n");
		printf("����              ��              ��    ��                ��                                      ��\n");
		printf("����              ��              ��    ��    �Ͻ� ����   ��                                      ��\n");
		printf("����              ��              ��    ��                ��                                      ��\n");
		printf("����              ��              ��    ��                ��                                      ��\n");
		printf("����              ��              ��    �� %s ����ϱ� %s ��                                      ��\n", (selected == 0) ? "��" : "  ", (selected == 0) ? "��" : "  ");
		printf("����              ��              ��    ��  %s ������ %s  ��                                      ��\n", (selected == 1) ? "��" : "  ", (selected == 1) ? "��" : "  ");
		printf("������������������������������������������                ��                                      ��\n");
		printf("����              ��              ��    ��������������������                                      ��\n");
		printf("����              ��              ��              ��                                              ��\n");
		printf("����              ��              ��              ��                                              ��\n");
		printf("����              ��              ��              ��                                              ��\n");
		printf("����              ��              ��              ��                                              ��\n");
		printf("����              ��              ��              ��                                              ��\n");
		printf("����              ��              ��              ��                                              ��\n");
		printf("����������������������������������������������������                                              ��\n");
		printf("����������������������������������������������������������������������������������������������������\n");
		printf("��                                                                                                ��\n");
		printf("����������������������������������������������������������������������������������������������������\n");
		switch (x = getch()) {
		case 224:
			x = getch();
			if (x == 72 || x == 80) selected = (selected + 1) % 2;
			break;
		case 13:
			return selected;
		}
	}
}

//��ũ ����(���� �� ���� ����)���� Ȯ���ϴ� �Լ�
int isFork(int map[3][3], int turn) {
	int attacks = 0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (map[i][j] == map[i][(j + 1) % 3] && map[i][j] == turn && map[i][(j + 2) % 3] == 0) attacks++;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (map[j][i] == map[(j + 1) % 3][i] && map[j][i] == turn && map[(j + 2) % 3][i] == 0) attacks++;
	for (int i = 0; i < 3; i++)
		if (map[i][i] == map[(i + 1) % 3][(i + 1) % 3] && map[i][i] == turn && map[(i + 2) % 3][(i + 2) % 3] == 0) attacks++;
	for (int i = 0; i < 3; i++)
		if (map[i][2 - i] == map[(i + 1) % 3][2 - (i + 1) % 3] && map[i][2 - i] == turn && map[(i + 2) % 3][2 - (i + 2) % 3] == 0) attacks++;
	return (attacks >= 2);
}

//���� ȭ���� ǥ���ϴ� �Լ�
void printScreen(int map[3][3], int p[2], int type, int turn, struct point pointer) {
	system("cls");
	printf("����������������������������������������������������������������������������������������������������\n");
	printf("����������������������������������������������������������������������������������������������������\n");
	printf("����      %s      ��      %s      ��      %s      ����                                          ����\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "��" : "  ");
	printf("����  %s%s%s%s%s  ��  %s%s%s%s%s  ��  %s%s%s%s%s  ���� %s %s %s                            ����\n", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (turn == 0) ? "��" : "  ", (p[0] == PLAYER) ? "�÷��̾�" : "   AI   ", (type == 1) ? " " : "1");
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  %s%s  %s%s  ���� %s %s %s                            ����\n", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (turn == 1) ? "��" : "  ", (p[1] == PLAYER) ? "�÷��̾�" : "   AI   ", (type == 1) ? " " : "2");
	printf("����%s%s  %s  %s%s��%s%s  %s  %s%s��%s%s  %s  %s%s����                                          ����\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "��" : "  ");
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  %s%s  %s%s  ��������������������������������������������������\n", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ");
	printf("����  %s%s%s%s%s  ��  %s%s%s%s%s  ��  %s%s%s%s%s  ��                                              ��\n", (map[0][0] == 1) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 2) ? "��" : "  ", (map[0][0] == 1) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 2) ? "��" : "  ", (map[1][0] == 1) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 2) ? "��" : "  ", (map[2][0] == 1) ? "��" : "  ");
	printf("����      %s      ��      %s      ��      %s      ��                                              ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "��" : "  ");
	printf("����������������������������������������������������                                              ��\n");
	printf("����      %s      ��      %s      ��      %s      ��                                              ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 1) ? "��" : "  ");
	printf("����  %s%s%s%s%s  ��  %s%s%s%s%s  ��  %s%s%s%s%s  ��                                              ��\n", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[2][1] == 1) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (map[2][1] == 1) ? "��" : "  ");
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  %s%s  %s%s  ��                                              ��\n", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (map[2][1] == 1) ? "��" : "  ", (map[2][1] == 1) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ");
	printf("����%s%s  %s  %s%s��%s%s  %s  %s%s��%s%s  %s  %s%s��                                              ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 1) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (map[2][1] == 1) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 1) ? "��" : "  ");
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  %s%s  %s%s  ��                                              ��\n", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (map[2][1] == 1) ? "��" : "  ", (map[2][1] == 1) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ");
	printf("����  %s%s%s%s%s  ��  %s%s%s%s%s  ��  %s%s%s%s%s  ��                                              ��\n", (map[0][1] == 1) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 2) ? "��" : "  ", (map[0][1] == 1) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 2) ? "��" : "  ", (map[1][1] == 1) ? "��" : "  ", (map[2][1] == 1) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (map[2][1] == 2) ? "��" : "  ", (map[2][1] == 1) ? "��" : "  ");
	printf("����      %s      ��      %s      ��      %s      ��                                              ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 1) ? "��" : "  ");
	printf("����������������������������������������������������                                              ��\n");
	printf("����      %s      ��      %s      ��      %s      ��                                              ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "��" : "  ");
	printf("����  %s%s%s%s%s  ��  %s%s%s%s%s  ��  %s%s%s%s%s  ��                                              ��\n", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ");
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  %s%s  %s%s  ��                                              ��\n", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ");
	printf("����%s%s  %s  %s%s��%s%s  %s  %s%s��%s%s  %s  %s%s��                                              ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "��" : "  ");
	printf("����  %s%s  %s%s  ��  %s%s  %s%s  ��  %s%s  %s%s  ��                                              ��\n", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ");
	printf("����  %s%s%s%s%s  ��  %s%s%s%s%s  ��  %s%s%s%s%s  ��                                              ��\n", (map[0][2] == 1) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 2) ? "��" : "  ", (map[0][2] == 1) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 2) ? "��" : "  ", (map[1][2] == 1) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 2) ? "��" : "  ", (map[2][2] == 1) ? "��" : "  ");
	printf("����      %s      ��      %s      ��      %s      ��                                              ��\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "��" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "��" : "  ");
	printf("����������������������������������������������������                                              ��\n");
	printf("����������������������������������������������������������������������������������������������������\n");
	printf("��                                                                                                ��\n");
	printf("����������������������������������������������������������������������������������������������������\n");
}

//AI�� ������ �����ϴ� �Լ�
struct point TTTAI(int map[3][3], int turn) {
	struct point res;
	int temp = 0;
	//������ �ٷ� ���� �� ���� �� �����Ѵ�
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (map[i][j] == map[i][(j + 1) % 3] && map[i][j] == turn && map[i][(j + 2) % 3] == 0) {
				res.x = i;
				res.y = (j + 2) % 3;
				return res;
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (map[j][i] == map[(j + 1) % 3][i] && map[j][i] == turn && map[(j + 2) % 3][i] == 0) {
				res.x = (j + 2) % 3;
				res.y = i;
				return res;
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		if (map[i][i] == map[(i + 1) % 3][(i + 1) % 3] && map[i][i] == turn && map[(i + 2) % 3][(i + 2) % 3] == 0) {
			res.x = (i + 2) % 3;
			res.y = (i + 2) % 3;
			return res;
		}
	}
	for (int i = 0; i < 3; i++) {
		if (map[i][2 - i] == map[(i + 1) % 3][2 - (i + 1) % 3] && map[i][2 - i] == turn && map[(i + 2) % 3][2 - (i + 2) % 3] == 0) {
			res.x = (i + 2) % 3;
			res.y = 2 - (i + 2) % 3;
			return res;
		}
	}
	//������ ������ �����Ѵ�
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (map[i][j] == map[i][(j + 1) % 3] && map[i][j] == (turn % 2 + 1) && map[i][(j + 2) % 3] == 0) {
				res.x = i;
				res.y = (j + 2) % 3;
				return res;
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (map[j][i] == map[(j + 1) % 3][i] && map[j][i] == (turn % 2 + 1) && map[(j + 2) % 3][i] == 0) {
				res.x = (j + 2) % 3;
				res.y = i;
				return res;
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		if (map[i][i] == map[(i + 1) % 3][(i + 1) % 3] && map[i][i] == (turn % 2 + 1) && map[(i + 2) % 3][(i + 2) % 3] == 0) {
			res.x = (i + 2) % 3;
			res.y = (i + 2) % 3;
			return res;
		}
	}
	for (int i = 0; i < 3; i++) {
		if (map[i][2 - i] == map[(i + 1) % 3][2 - (i + 1) % 3] && map[i][2 - i] == (turn % 2 + 1) && map[(i + 2) % 3][2 - (i + 2) % 3] == 0) {
			res.x = (i + 2) % 3;
			res.y = 2 - (i + 2) % 3;
			return res;
		}
	}
	//��ũ�� �����
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (map[i][j] == 0) {
				int tempmap[3][3];
				for (int k = 0; k < 3; k++)
					for (int l = 0; l < 3; l++)
						tempmap[l][k] = map[l][k];
				tempmap[i][j] = turn;
				if (isFork(tempmap, turn)) {
					res.x = i;
					res.y = j;
					return res;
				}
			}
		}
	}
	//��ũ�� ���´�
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (map[i][j] == 0) {
				int tempmap[3][3];
				for (int k = 0; k < 3; k++)
					for (int l = 0; l < 3; l++)
						tempmap[l][k] = map[l][k];
				tempmap[i][j] = turn % 2 + 1;
				if (isFork(tempmap, turn % 2 + 1)) temp++;
			}
		}
	}
	if (temp >= 1) {
		if (temp >= 2) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (map[i][j] == turn) {
						for (int k = -1; k < 2; k++) {
							for (int l = -1; l < 2; l++) {
								if (i + k < 0 || i + k > 2 || j + l < 0 || j + l > 2) continue;
								if (map[i + k][j + l] == 0) {
									int tempmap[3][3];
									for (int m = 0; m < 3; m++)
										for (int n = 0; n < 3; n++)
											tempmap[m][n] = map[m][n];
									tempmap[i + k][j + l] = turn % 2 + 1;
									if (map[i - k][j - l] == 0 && !isFork(tempmap, turn % 2 + 1)) {
										res.x = i - k;
										res.y = j - l;
										return res;
									}
								}
							}
						}
					}
				}
			}
		}
		else {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (map[i][j] == 0) {
						int tempmap[3][3];
						for (int k = 0; k < 3; k++)
							for (int l = 0; l < 3; l++)
								tempmap[l][k] = map[l][k];
						tempmap[i][j] = turn % 2 + 1;
						if (isFork(tempmap, turn % 2 + 1)) {
							res.x = i;
							res.y = j;
							return res;
						}
					}
				}
			}
		}
	}
	//����� ����� ��� ����� �����Ѵ�
	if (map[1][1] == 0) {
		res.x = 1;
		res.y = 1;
		return res;
	}
	//������ �ڳʸ� ������ ��� ���ݴ����� �ڳʸ� �����Ѵ�
	for (int i = 0; i <= 2; i += 2) {
		for (int j = 0; j <= 2; j += 2) {
			if (map[i][j] == turn % 2 + 1 && map[2 - i][2 - j] == 0) {
				res.x = 2 - i;
				res.y = 2 - j;
				return res;
			}
		}
	}
	//�� �ڳʸ� �����Ѵ�
	for (int i = 0; i <= 2; i += 2) {
		for (int j = 0; j <= 2; j += 2) {
			if (map[i][j] == 0) {
				res.x = i;
				res.y = j;
				return res;
			}
		}
	}
	//�� ���̵带 �����Ѵ�
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			if (map[i + j][i - j + 1] == 0) {
				res.x = i + j;
				res.y = i - j + 1;
				return res;
			}
		}
	}
}