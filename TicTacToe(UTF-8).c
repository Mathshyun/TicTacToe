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

//3×3칸에서의 좌표를 나타내는 구조체
struct point {
	int x, y;
};

//승, 패, 무를 저장할 구조체
struct numbers {
	int Xwin, Owin, draw;
};

int title();																				//타이틀 메시지를 출력하는 함수
int decideOrder();																			//PvA에서 순서를 정하는 함수
int isGameEnd(int map[3][3], int x, int y);													//게임이 끝났는지 확인하는 함수
int pauseScreen();																			//일시 정지 화면을 표시하는 함수
int isFork(int map[3][3], int turn);														//포크 상태(막을 수 없는 상태)인지 확인하는 함수
void startGame(int type);																	//게임을 시작하는 함수
void endGame(int map[3][3], int p[2], int wp, int type, int turn, struct point pointer);	//게임을 끝내는 함수
void printScreen(int map[3][3], int p[2], int type, int turn, struct point pointer);		//게임 화면을 표시하는 함수
struct point TTTAI(int map[3][3], int turn);												//AI의 선택을 결정하는 함수

int main() {
	system("mode con:cols=100 lines=30");	//창 크기 조절
	system("title TIC-TAC-TOE");			//창 제목 설정
	srand(time(NULL));						//랜덤 변수의 시드(seed) 설정
	while (1) {
		int x = title();					//타이틀 메시지를 표시해 반환값을 x에 저장
		switch (x) {
		case 2:
			return 0;						//게임 종료
		default:
			startGame(x);					//게임 시작
		}
	}
	return 0;
}

//타이틀 메시지를 출력하는 함수
int title() {
	int x, selected = 0;

	while (1) {
		system("cls");
		printf("┌────────────────────────────────────────────────┐\n");
		printf("│         ______  ______  ______       ______  ______  ______       ______  ______  ______       │\n");
		printf("│        /_  __/ /_  __/ / ____/      /_  __/ / __  / / ____/      /_  __/ / __  / / ____/       │\n");
		printf("│         / /     / /   / /     ___    / /   / / / / / /     ___    / /   / / / / / /___         │\n");
		printf("│        / /     / /   / /     /__/   / /   / /_/ / / /     /__/   / /   / / / / / ____/         │\n");
		printf("│       / /   __/ /_  / /___         / /   / __  / / /___         / /   / /_/ / / /___           │\n");
		printf("│      /_/   /_____/ /_____/        /_/   /_/ /_/ /_____/        /_/   /_____/ /_____/           │\n");
		printf("│                                                                                                │\n");
		printf("└────────────────────────────────────────────────┘\n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                    %s 플레이어 vs. 플레이어 %s                                     \n", (selected == 0) ? "▶" : "  ", (selected == 0) ? "◀" : "  ");
		printf("                                       %s 플레이어 vs. AI %s                                        \n", (selected == 1) ? "▶" : "  ", (selected == 1) ? "◀" : "  ");
		printf("                                           %s 나가기 %s                                             \n", (selected == 2) ? "▶" : "  ", (selected == 2) ? "◀" : "  ");
		printf("                             방향키로 커서를 움직이고 엔터로 선택하세요                             \n");
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

//PvA에서 순서를 정하는 함수
int decideOrder() {
	int x, selected = 0;

	while (1) {
		system("cls");
		printf("┌────────────────────────────────────────────────┐\n");
		printf("│         ______  ______  ______       ______  ______  ______       ______  ______  ______       │\n");
		printf("│        /_  __/ /_  __/ / ____/      /_  __/ / __  / / ____/      /_  __/ / __  / / ____/       │\n");
		printf("│         / /     / /   / /     ___    / /   / / / / / /     ___    / /   / / / / / /___         │\n");
		printf("│        / /     / /   / /     /__/   / /   / /_/ / / /     /__/   / /   / / / / / ____/         │\n");
		printf("│       / /   __/ /_  / /___         / /   / __  / / /___         / /   / /_/ / / /___           │\n");
		printf("│      /_/   /_____/ /_____/        /_/   /_/ /_/ /_____/        /_/   /_____/ /_____/           │\n");
		printf("│                                                                                                │\n");
		printf("└────────────────────────────────────────────────┘\n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                                                                                    \n");
		printf("                                            %s 선공 %s                                              \n", (selected == 0) ? "▶" : "  ", (selected == 0) ? "◀" : "  ");
		printf("                                            %s 후공 %s                                              \n", (selected == 1) ? "▶" : "  ", (selected == 1) ? "◀" : "  ");
		printf("                                           %s 무작위 %s                                             \n", (selected == 2) ? "▶" : "  ", (selected == 2) ? "◀" : "  ");
		printf("                                            %s 취소 %s                                              \n", (selected == 3) ? "▶" : "  ", (selected == 3) ? "◀" : "  ");
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
			case 0:					//플레이어 선공
				return PLAYER;
			case 1:					//플레이어 후공
				return AI;
			case 2:					//무작위
				return rand() % 2;
			case 3:					//취소
				return -1;
			}
		}
	}
}

//게임이 끝났는지 확인하는 함수
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

//게임을 시작하는 함수
void startGame(int type) {
	struct point pointer;			//현재 표시한 칸
	int map[3][3] = { 0 };			//맵을 저장하는 배열
	int cnt = 0, turn = 1, p[2];
	clock_t prevTime;				//제한 시간을 나타내기 위해 사용

	switch (type) {
	case 0:							//PvP
		p[0] = p[1] = PLAYER;
		break;
	case 1:							//PvA
		p[0] = decideOrder();
		if (p[0] == -1) return;		//순서 선택 창에서 취소할 경우 타이틀로
		p[1] = (p[0] + 1) % 2;
		break;
	}

	pointer.x = 1;
	pointer.y = 1;
	printScreen(map, p, type, turn - 1, pointer);
	prevTime = clock();
	while (1) {
		if (kbhit()) {															//키보드 입력이 있는지 검사
			switch (getch()) {
			case 224:
				if (p[turn - 1] == PLAYER) {									//플레이어의 차례이면
					switch (getch()) {
					case 72:													//↑
						if (pointer.y == 0) continue;
						pointer.y--;
						break;
					case 75:													//←
						if (pointer.x == 0) continue;
						pointer.x--;
						break;
					case 77:													//→
						if (pointer.x == 2) continue;
						pointer.x++;
						break;
					case 80:													//↓
						if (pointer.y == 2) continue;
						pointer.y++;
					}
				}
				printScreen(map, p, type, turn - 1, pointer);
				break;
			case 13:															//엔터
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
		else {																	//키보드 입력이 없으면
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

//게임을 끝내는 함수
void endGame(int map[3][3], int p[2], int wp, int type, int turn, struct point pointer) {
	system("cls");
	printf("┌────────────────────────────────────────────────┐\n");
	printf("│┌───────┬───────┬───────┐┌─────────────────────┐│\n");
	printf("││      %s      │      %s      │      %s      ││                                          ││\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "▼" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "▼" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "▼" : "  ");
	printf("││  %s%s%s%s%s  │  %s%s%s%s%s  │  %s%s%s%s%s  ││ %s %s %s                            ││\n", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (turn == 0) ? "▶" : "  ", (p[0] == PLAYER) ? "플레이어" : "   AI   ", (type == 1) ? " " : "1");
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  %s%s  %s%s  ││ %s %s %s                            ││\n", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (turn == 1) ? "▶" : "  ", (p[1] == PLAYER) ? "플레이어" : "   AI   ", (type == 1) ? " " : "2");
	printf("││%s%s  %s  %s%s│%s%s  %s  %s%s│%s%s  %s  %s%s││                                          ││\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "▶" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "◀" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "▶" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "◀" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "▶" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "◀" : "  ");
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  %s%s  %s%s  │└─────────────────────┘│\n", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ");
	printf("││  %s%s%s%s%s  │  %s%s%s%s%s  │  %s%s%s%s%s  │                                              │\n", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ");
	printf("││      %s      │      %s      │      %s      │                                              │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "▲" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "▲" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "▲" : "  ");
	printf("│├───────┼───────┼───────┤                                              │\n");
	printf("││      %s      │      %s      │  ┌─────┴────┐                                    │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "▼" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "▼" : "  ");
	printf("││  %s%s%s%s%s  │  %s%s%s%s%s  │  │                    │                                    │\n", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ");
	if (wp == 0)
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  │       무승부       │                                    │\n", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ");
	else if (p[0] == p[1])
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  │   %s %d 승!   │                                    │\n", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (p[wp - 1] == PLAYER) ? "플레이어" : "   AI   ", wp);
	else
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  │    %s 승!    │                                    │\n", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (p[wp - 1] == PLAYER) ? "플레이어" : "   AI   ");
	printf("││%s%s  %s  %s%s│%s%s  %s  %s%s│%s│                    │                                    │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "▶" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "◀" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "▶" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "◀" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 1) ? "▶" : "  ");
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  └─────┬────┘                                    │\n", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ");
	printf("││  %s%s%s%s%s  │  %s%s%s%s%s  │  %s%s%s%s%s  │                                              │\n", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[2][1] == 1) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ", (map[2][1] == 1) ? "■" : "  ");
	printf("││      %s      │      %s      │      %s      │                                              │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "▲" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "▲" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 1) ? "▲" : "  ");
	printf("│├───────┼───────┼───────┤                                              │\n");
	printf("││      %s      │      %s      │      %s      │                                              │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "▼" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "▼" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "▼" : "  ");
	printf("││  %s%s%s%s%s  │  %s%s%s%s%s  │  %s%s%s%s%s  │                                              │\n", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ");
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  %s%s  %s%s  │                                              │\n", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ");
	printf("││%s%s  %s  %s%s│%s%s  %s  %s%s│%s%s  %s  %s%s│                                              │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "▶" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "◀" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "▶" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "◀" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "▶" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "◀" : "  ");
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  %s%s  %s%s  │                                              │\n", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ");
	printf("││  %s%s%s%s%s  │  %s%s%s%s%s  │  %s%s%s%s%s  │                                              │\n", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ");
	printf("││      %s      │      %s      │      %s      │                                              │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "▲" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "▲" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "▲" : "  ");
	printf("│└───────┴───────┴───────┘                                              │\n");
	printf("├────────────────────────────────────────────────┤\n");
	printf("│                                                                                                │\n");
	printf("└────────────────────────────────────────────────┘\n");
	getch();
}

//일시 정지 화면을 표시하는 함수
int pauseScreen() {
	int x, selected = 0;
	while (1) {
		system("cls");
		printf("┌────────────────────────────────────────────────┐\n");
		printf("│┌───────┬───────┬───────┐┌─────────────────────┐│\n");
		printf("││              │              │              ││                                          ││\n");
		printf("││              │              │              ││                                          ││\n");
		printf("││              │              │              ││                                          ││\n");
		printf("││              │              │              ││                                          ││\n");
		printf("││              │              │              │└─────────────────────┘│\n");
		printf("││              │              │              │                                              │\n");
		printf("││              │              │              │                                              │\n");
		printf("│├───────┼───────┼───────┤                                              │\n");
		printf("││              │              │    ┌────┴───┐                                      │\n");
		printf("││              │              │    │                │                                      │\n");
		printf("││              │              │    │    일시 정지   │                                      │\n");
		printf("││              │              │    │                │                                      │\n");
		printf("││              │              │    │                │                                      │\n");
		printf("││              │              │    │ %s 계속하기 %s │                                      │\n", (selected == 0) ? "▶" : "  ", (selected == 0) ? "◀" : "  ");
		printf("││              │              │    │  %s 나가기 %s  │                                      │\n", (selected == 1) ? "▶" : "  ", (selected == 1) ? "◀" : "  ");
		printf("│├───────┼───────┼──┤                │                                      │\n");
		printf("││              │              │    └────┬───┘                                      │\n");
		printf("││              │              │              │                                              │\n");
		printf("││              │              │              │                                              │\n");
		printf("││              │              │              │                                              │\n");
		printf("││              │              │              │                                              │\n");
		printf("││              │              │              │                                              │\n");
		printf("││              │              │              │                                              │\n");
		printf("│└───────┴───────┴───────┘                                              │\n");
		printf("├────────────────────────────────────────────────┤\n");
		printf("│                                                                                                │\n");
		printf("└────────────────────────────────────────────────┘\n");
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

//포크 상태(막을 수 없는 상태)인지 확인하는 함수
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

//게임 화면을 표시하는 함수
void printScreen(int map[3][3], int p[2], int type, int turn, struct point pointer) {
	system("cls");
	printf("┌────────────────────────────────────────────────┐\n");
	printf("│┌───────┬───────┬───────┐┌─────────────────────┐│\n");
	printf("││      %s      │      %s      │      %s      ││                                          ││\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "▼" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "▼" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "▼" : "  ");
	printf("││  %s%s%s%s%s  │  %s%s%s%s%s  │  %s%s%s%s%s  ││ %s %s %s                            ││\n", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (turn == 0) ? "▶" : "  ", (p[0] == PLAYER) ? "플레이어" : "   AI   ", (type == 1) ? " " : "1");
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  %s%s  %s%s  ││ %s %s %s                            ││\n", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (turn == 1) ? "▶" : "  ", (p[1] == PLAYER) ? "플레이어" : "   AI   ", (type == 1) ? " " : "2");
	printf("││%s%s  %s  %s%s│%s%s  %s  %s%s│%s%s  %s  %s%s││                                          ││\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "▶" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "◀" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "▶" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "◀" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "▶" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "◀" : "  ");
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  %s%s  %s%s  │└─────────────────────┘│\n", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ");
	printf("││  %s%s%s%s%s  │  %s%s%s%s%s  │  %s%s%s%s%s  │                                              │\n", (map[0][0] == 1) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 2) ? "■" : "  ", (map[0][0] == 1) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 2) ? "■" : "  ", (map[1][0] == 1) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 2) ? "■" : "  ", (map[2][0] == 1) ? "■" : "  ");
	printf("││      %s      │      %s      │      %s      │                                              │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 0) ? "▲" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 0) ? "▲" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 0) ? "▲" : "  ");
	printf("│├───────┼───────┼───────┤                                              │\n");
	printf("││      %s      │      %s      │      %s      │                                              │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "▼" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "▼" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 1) ? "▼" : "  ");
	printf("││  %s%s%s%s%s  │  %s%s%s%s%s  │  %s%s%s%s%s  │                                              │\n", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[2][1] == 1) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ", (map[2][1] == 1) ? "■" : "  ");
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  %s%s  %s%s  │                                              │\n", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ", (map[2][1] == 1) ? "■" : "  ", (map[2][1] == 1) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ");
	printf("││%s%s  %s  %s%s│%s%s  %s  %s%s│%s%s  %s  %s%s│                                              │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "▶" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "◀" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "▶" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "◀" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 1) ? "▶" : "  ", (map[2][1] == 2) ? "■" : "  ", (map[2][1] == 1) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 1) ? "◀" : "  ");
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  %s%s  %s%s  │                                              │\n", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ", (map[2][1] == 1) ? "■" : "  ", (map[2][1] == 1) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ");
	printf("││  %s%s%s%s%s  │  %s%s%s%s%s  │  %s%s%s%s%s  │                                              │\n", (map[0][1] == 1) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 2) ? "■" : "  ", (map[0][1] == 1) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 2) ? "■" : "  ", (map[1][1] == 1) ? "■" : "  ", (map[2][1] == 1) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ", (map[2][1] == 2) ? "■" : "  ", (map[2][1] == 1) ? "■" : "  ");
	printf("││      %s      │      %s      │      %s      │                                              │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 1) ? "▲" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 1) ? "▲" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 1) ? "▲" : "  ");
	printf("│├───────┼───────┼───────┤                                              │\n");
	printf("││      %s      │      %s      │      %s      │                                              │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "▼" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "▼" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "▼" : "  ");
	printf("││  %s%s%s%s%s  │  %s%s%s%s%s  │  %s%s%s%s%s  │                                              │\n", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ");
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  %s%s  %s%s  │                                              │\n", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ");
	printf("││%s%s  %s  %s%s│%s%s  %s  %s%s│%s%s  %s  %s%s│                                              │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "▶" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "◀" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "▶" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "◀" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "▶" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "◀" : "  ");
	printf("││  %s%s  %s%s  │  %s%s  %s%s  │  %s%s  %s%s  │                                              │\n", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ");
	printf("││  %s%s%s%s%s  │  %s%s%s%s%s  │  %s%s%s%s%s  │                                              │\n", (map[0][2] == 1) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 2) ? "■" : "  ", (map[0][2] == 1) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 2) ? "■" : "  ", (map[1][2] == 1) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 2) ? "■" : "  ", (map[2][2] == 1) ? "■" : "  ");
	printf("││      %s      │      %s      │      %s      │                                              │\n", (p[turn] == PLAYER && pointer.x == 0 && pointer.y == 2) ? "▲" : "  ", (p[turn] == PLAYER && pointer.x == 1 && pointer.y == 2) ? "▲" : "  ", (p[turn] == PLAYER && pointer.x == 2 && pointer.y == 2) ? "▲" : "  ");
	printf("│└───────┴───────┴───────┘                                              │\n");
	printf("├────────────────────────────────────────────────┤\n");
	printf("│                                                                                                │\n");
	printf("└────────────────────────────────────────────────┘\n");
}

//AI의 선택을 결정하는 함수
struct point TTTAI(int map[3][3], int turn) {
	struct point res;
	int temp = 0;
	//게임을 바로 끝낼 수 있을 때 선택한다
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
	//상대방의 공격을 차단한다
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
	//포크를 만든다
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
	//포크를 막는다
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
	//가운데가 비었을 경우 가운데를 선택한다
	if (map[1][1] == 0) {
		res.x = 1;
		res.y = 1;
		return res;
	}
	//상대방이 코너를 선택한 경우 정반대쪽의 코너를 선택한다
	for (int i = 0; i <= 2; i += 2) {
		for (int j = 0; j <= 2; j += 2) {
			if (map[i][j] == turn % 2 + 1 && map[2 - i][2 - j] == 0) {
				res.x = 2 - i;
				res.y = 2 - j;
				return res;
			}
		}
	}
	//빈 코너를 선택한다
	for (int i = 0; i <= 2; i += 2) {
		for (int j = 0; j <= 2; j += 2) {
			if (map[i][j] == 0) {
				res.x = i;
				res.y = j;
				return res;
			}
		}
	}
	//빈 사이드를 선택한다
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