#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <mmsystem.h>    
#pragma comment(lib,"winmm.lib") 

#define FILENAME "leaderboard.txt"

typedef struct {
	char name[5];
	int score;
} User;

// 색상 정의
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

#define ESC 0x1b //  ESC 누르면 종료

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d
#define SPACE 0x20

#define WIDTH 12
#define HEIGHT 24

#define EMPTY 0
#define BLOCK1 1
#define BLOCK2 2
#define WALL 3
#define PREVIEW 4

int map[HEIGHT][WIDTH];
int colormap[HEIGHT][WIDTH];

int score, level, bestscore;

int block_x, block_y, block_shape, block_turn;

int next_shape, next_turn;

int current_color;

int restart;

int delay, speed;

int block[7][4][4][4] =
{
	{
		0,0,0,0,
		0,1,0,0,
		0,1,1,1,
		0,0,0,0,

		0,0,0,0,
		0,0,1,1,
		0,0,1,0,
		0,0,1,0,

		0,0,0,0,
		0,0,0,0,
		0,1,1,1,
		0,0,0,1,

		0,0,0,0,
		0,0,1,0,
		0,0,1,0,
		0,1,1,0
	},
	{
		0,0,0,0,
		0,0,0,1,
		0,1,1,1,
		0,0,0,0,

		0,0,0,0,
		0,0,1,0,
		0,0,1,0,
		0,0,1,1,

		0,0,0,0,
		0,0,0,0,
		0,1,1,1,
		0,1,0,0,

		0,0,0,0,
		0,1,1,0,
		0,0,1,0,
		0,0,1,0
	},
	{
		0,0,0,0,
		0,1,1,0,
		0,1,1,0,
		0,0,0,0,

		0,0,0,0,
		0,1,1,0,
		0,1,1,0,
		0,0,0,0,

		0,0,0,0,
		0,1,1,0,
		0,1,1,0,
		0,0,0,0,

		0,0,0,0,
		0,1,1,0,
		0,1,1,0,
		0,0,0,0
	},
	{
		0,0,0,0,
		0,0,1,0,
		0,1,1,1,
		0,0,0,0,

		0,0,0,0,
		0,0,1,0,
		0,0,1,1,
		0,0,1,0,

		0,0,0,0,
		0,0,0,0,
		0,1,1,1,
		0,0,1,0,

		0,0,0,0,
		0,0,1,0,
		0,1,1,0,
		0,0,1,0
	},
	{
		0,0,0,0,
		0,0,0,0,
		1,1,1,1,
		0,0,0,0,

		0,0,1,0,
		0,0,1,0,
		0,0,1,0,
		0,0,1,0,

		0,0,0,0,
		1,1,1,1,
		0,0,0,0,
		0,0,0,0,

		0,1,0,0,
		0,1,0,0,
		0,1,0,0,
		0,1,0,0
	},
	{
		0,0,0,0,
		0,1,1,0,
		0,0,1,1,
		0,0,0,0,

		0,0,0,1,
		0,0,1,1,
		0,0,1,0,
		0,0,0,0,

		0,1,1,0,
		0,0,1,1,
		0,0,0,0,
		0,0,0,0,

		0,0,1,0,
		0,1,1,0,
		0,1,0,0,
		0,0,0,0
	},
	{
		0,0,0,0,
		0,0,1,1,
		0,1,1,0,
		0,0,0,0,

		0,0,1,0,
		0,0,1,1,
		0,0,0,1,
		0,0,0,0,

		0,0,1,1,
		0,1,1,0,
		0,0,0,0,
		0,0,0,0,

		0,1,0,0,
		0,1,1,0,
		0,0,1,0,
		0,0,0,0
	}
};

void removeCursor(void) { // 커서를 안보이게 한다

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void gotoxy2(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x*2, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}
// 화면 지우기고 원하는 배경색으로 설정한다.
void cls(int bg_color, int text_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);

}

// box 그리기 함수, ch 문자로 (x1,y1) ~ (x2,y2) box를 그린다.
void draw_box(int x1, int y1, int x2, int y2)
{
	int x, y;
	//int len = strlen(ch);
	for (x = x1; x <= x2; x += 2) { // 한글은 2칸씩 차지한다.
		gotoxy(x, y1);
		printf("%s", "─"); 
		gotoxy(x, y2);
		printf("%s", "─");
	}
	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		printf("%s", "│");
		gotoxy(x2, y);
		printf("%s", "│");
	}
	gotoxy(x1, y1); printf("┌");
	gotoxy(x1, y2); printf("└");
	gotoxy(x2, y1); printf("┐");
	gotoxy(x2, y2); printf("┘");
}

// 메인 화면 그림
void draw_main() 
{
	draw_box(14, 6, 62, 12); Sleep(100);
	int x = 18, y = 7;
	textcolor(RED2,BLACK); Sleep(100);
	gotoxy(x, y++);
	printf("■■■");
	gotoxy(x, y++);
	printf("  ■");
	gotoxy(x, y++);
	printf("  ■");
	gotoxy(x, y++);
	printf("  ■");
	gotoxy(x, y++);
	printf("  ■");
	x += 8;
	y -= 5;
	textcolor(CYAN2, BLACK); Sleep(100);
	gotoxy(x, y++);
	printf("■■■");
	gotoxy(x, y++);
	printf("■");
	gotoxy(x, y++);
	printf("■■■");
	gotoxy(x, y++);
	printf("■");
	gotoxy(x, y++);
	printf("■■■");
	x += 8;
	y -= 5;
	textcolor(YELLOW2, BLACK); Sleep(100);
	gotoxy(x, y++);
	printf("■■■");
	gotoxy(x, y++);
	printf("  ■");
	gotoxy(x, y++);
	printf("  ■");
	gotoxy(x, y++);
	printf("  ■");
	gotoxy(x, y++);
	printf("  ■");
	x += 8;
	y -= 5;
	textcolor(GREEN2, BLACK); Sleep(100);
	gotoxy(x, y++);
	printf("■■");
	gotoxy(x, y++);
	printf("■ ■");
	gotoxy(x, y++);
	printf("■■");
	gotoxy(x, y++);
	printf("■ ■");
	gotoxy(x, y++);
	printf("■  ■");
	x += 8;
	y -= 5;
	textcolor(BLUE2, BLACK); Sleep(100);
	gotoxy(x, y++);
	printf("■");
	gotoxy(x, y++);
	printf("■");
	gotoxy(x, y++);
	printf("■");
	gotoxy(x, y++);
	printf("■");
	gotoxy(x, y++);
	printf("■");
	x += 4;
	y -= 5;
	textcolor(MAGENTA1, BLACK); Sleep(100);
	gotoxy(x, y++);
	printf("■■■");
	gotoxy(x, y++);
	printf("■");
	gotoxy(x, y++);
	printf("■■■");
	gotoxy(x, y++);
	printf("    ■");
	gotoxy(x, y++);
	printf("■■■");

	textcolor(RED2, BLACK); Sleep(100);
	x = 2, y = 0;
	gotoxy(x, y++);
	printf("■■■");
	gotoxy(x, y++);
	printf("  ■ ");
	textcolor(RED1, BLACK); Sleep(100);
	x = 26, y = 2;
	gotoxy(x, y++);
	printf("■■■■");
	textcolor(CYAN2, BLACK); Sleep(100);
	x = 42, y = 3;
	gotoxy(x, y++);
	printf("    ■");
	gotoxy(x, y++);
	printf("■■■");
	textcolor(YELLOW2, BLACK); Sleep(100);
	x = 69; y = 2;
	gotoxy(x, y++);
	printf("■■");
	gotoxy(x, y++);
	printf("■■");
	textcolor(YELLOW1, BLACK); Sleep(100);
	x = 72; y = 11;
	gotoxy(x, y++);
	printf("■");
	gotoxy(x, y++);
	printf("■■");
	gotoxy(x, y++);
	printf("  ■");
	textcolor(MAGENTA2, BLACK); Sleep(100);
	x = 65; y = 20;
	gotoxy(x, y++);
	printf("■");
	gotoxy(x, y++);
	printf("■■■");
	textcolor(GREEN2, BLACK); Sleep(100);
	x = 49; y = 17;
	gotoxy(x, y++);
	printf("■■");
	gotoxy(x, y++);
	printf("  ■");
	gotoxy(x, y++);
	printf("  ■");
	textcolor(MAGENTA1, BLACK); Sleep(100);
	x = 35; y = 19;
	gotoxy(x, y++);
	printf("■");
	gotoxy(x, y++);
	printf("■");
	gotoxy(x, y++);
	printf("■");
	gotoxy(x, y++);
	printf("■");
	textcolor(BLUE2, BLACK); Sleep(100);
	x = 21; y = 18;
	gotoxy(x, y++);
	printf("■■");
	gotoxy(x, y++);
	printf("  ■■");
	textcolor(CYAN1, BLACK); Sleep(100);
	x = 9; y = 19;
	gotoxy(x, y++);
	printf("■■");
	gotoxy(x, y++);
	printf("■■");
	textcolor(GRAY2, BLACK); Sleep(100);
	x = 3, y = 12;
	gotoxy(x, y++);
	printf("  ■");
	gotoxy(x, y++);
	printf("■■ ");
	gotoxy(x, y++);
	printf("  ■ ");

	textcolor(WHITE, BLACK); Sleep(100);
	gotoxy(26, 14);
	printf("< Press any Key to Start >");
}

// 맵 생성
void create_map() {
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			map[y][x] = EMPTY;
	for (int y = 0; y < HEIGHT; y++) {
		map[y][0] = WALL;
		map[y][WIDTH - 1] = WALL;
	}
	for (int x = 0; x < WIDTH; x++)
		map[HEIGHT-1][x] = WALL;
}

// 맵 그리기
void draw_map() {
	for(int y=0;y<HEIGHT;y++)
		for (int x = 0; x < WIDTH; x++) {
			switch (map[y][x]) {
				case EMPTY :
					gotoxy2(x, y);
					printf("  ");
					break;
				case WALL :  
					gotoxy2(x, y);
					textcolor(BLUE1, BLACK);
					printf("▩");
					textcolor(WHITE, BLACK);
					break;
				case BLOCK1 : case BLOCK2 :
					gotoxy2(x, y);
					textcolor(colormap[y][x], BLACK);
					printf("■");
					textcolor(WHITE, BLACK);
					break;
				case PREVIEW: 
					gotoxy2(x, y);
					printf("□");
					break;
			}
		}
}


// 게임 설명, 점수 및 레벨 drawing
void draw_manual() {
	gotoxy(30, 7);
	printf("SCORE : %d", score);
	gotoxy(30, 9);
	printf("LEVEL : %d", level);
	gotoxy(48, 15);
	printf("   ↑");
	gotoxy(48, 16);
	printf("← ↓ →   ＋");
	gotoxy(63, 15);
	printf("┌────┐");
	gotoxy(63, 16);
	printf("│SPACEBAR│");
	gotoxy(63, 17);
	printf("└────┘");
	gotoxy(56, 10);
	printf("P : PAUSE");

	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++) {
			gotoxy2(x + 15, y + 15);
			printf(" ");
		}
	gotoxy(30, 13);
	printf("NEXT BLOCK");
	draw_box(26, 5, 42, 19);
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			if (block[next_shape][next_turn][y][x] == 1) {
				gotoxy2(x + 15, y + 15);
				printf("■");
			}
}

// 일시 정지 메뉴 drawing
void draw_pause() {
	textcolor(WHITE, BLACK);
	for (int i = 5; i <= 20; i++) {
		gotoxy(24, i);
		printf("\t\t\t\t\t\t\t");
	}
	draw_box(38, 6, 62, 18);
	gotoxy(40, 7);
	printf("┌─────────┐");
	gotoxy(40, 8);
	printf("│     P A U S E    │");
	gotoxy(40, 9);
	printf("└─────────┘");
	gotoxy(40, 12);
	printf("  Press P to Continue");
	gotoxy(40, 14);
	printf("  Press R to Restart");
	gotoxy(40, 16);
	printf("  Press ESC to Quit ");
	while (1)
	{
		unsigned char ch;
		char c1;
		c1 = getch(); // key 값을 읽는다
		if (c1 == '\0') // VS 의 NULL 값은 Error이다.
			continue;
		else
			ch = c1;
		// ESC 누르면 프로그램 종료
		if (ch == ESC) {
			gotoxy(36, 19);
			exit(0);
		}
		if (ch == 'r') {
			restart = 1;
			break;
		}
		if (ch == 'p') {
			for (int i = 5; i <= 20; i++) {
				gotoxy(24, i);
				printf("\t\t\t\t\t\t\t");
			}
			break;
		}
	}
	draw_manual();
}

// 블록 생성
void create_block() {
	int old_color = current_color;
	do {
		current_color = rand() % 6 + 9;
	} while (old_color == current_color);
	block_x = 4;
	block_y = 0;
	block_turn = next_turn;
	block_shape = next_shape;
	do {
		next_turn = rand() % 4;
		next_shape = rand() % 7;
	} while (next_shape == block_shape);
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			if (block[block_shape][block_turn][y][x] == 1) {
				map[block_y + y][block_x + x] = BLOCK1;
				colormap[block_y + y][block_x + x] = current_color;
			}
}

// 블록 움직임
void block_move(int newx, int newy,int newturn) {
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			if (block[block_shape][block_turn][y][x] == 1) {
				map[block_y + y][block_x + x] = EMPTY;
				colormap[block_y + y][block_x + x] = 0;
			}
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			if (block[block_shape][newturn][y][x] == 1) {
				map[newy + y][newx + x] = BLOCK1;
				colormap[newy + y][newx + x] = current_color;
			}
	block_x = newx;
	block_y = newy;
	block_turn = newturn;
}

// 충돌 계산
int is_collide(int newx, int newy, int newturn) {
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			if (block[block_shape][newturn][y][x] == 1)
				if (map[newy + y][newx + x] == WALL ||
					map[newy + y][newx + x] == BLOCK2)
					return 1;
	return 0;
}

// 게임 종료 후 이름 입력 drawing -> 파일 입출력으로 점수 기록
void insert_name() {
	for (int i = 5; i <= 20; i++) {
		gotoxy(24, i);
		printf("\t\t\t\t\t\t\t");
	}
	char name[6];
	draw_box(34, 6, 66, 18);
	gotoxy(40, 9);
	printf("   BEST SCORE : %d", bestscore);
	gotoxy(40, 12);
	printf("   INSERT YOUR NAME");
	gotoxy(42, 14);
	printf("┌───────┐ ");
	gotoxy(42, 15);
	printf("│   _ _ _ _ _  │");
	gotoxy(42, 16);
	printf("└───────┘");
	for (int i = 0; i < 5; i++) {
		name[i] = toupper(getch());
		gotoxy(47 + i*2, 15);
		printf("%c", name[i]);
	}
	name[5] = '\0';
	getch();

	for (int i = 5; i <= 20; i++) {
		gotoxy(24, i);
		printf("\t\t\t\t\t\t\t");
	}

	FILE* fp;
	if ((fp = fopen(FILENAME, "r+b")) == NULL)  // 파일이 없으면
		fp = fopen(FILENAME, "w+b");
	
	int filesize;
	int ndata;
	User player, data[100] = { 0 };

	fseek(fp, 0, SEEK_END); // 끝으로 이동
	strcpy(player.name, name);
	player.score = bestscore;
	fwrite(&player, sizeof(User), 1, fp);

	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp); // 파일 크기가 return 됨
	ndata = filesize / sizeof(User);

	fseek(fp, 0, SEEK_SET); // 처음으로 이동
	for (int i = 0; i < ndata; i++) {
		fread(&data[i], sizeof(User), 1, fp);
	}

	fclose(fp);

	User temp;
	for (int i = 0; i < ndata; i++)
		for (int j = 0; j < ndata - i; j++)
			if (data[j].score < data[j + 1].score) {
				temp = data[j + 1];
				data[j + 1] = data[j];
				data[j] = temp;
			}

	draw_box(34, 6, 66, 18);
	char* spell[5] = { "st","nd","rd","th","th" };
	for (int i = 0; i < 5; i++) {
		gotoxy(44, i*2 + 8);
		printf("%d%s %s %d\n", i + 1, spell[i], data[i].name, data[i].score);
	}
	if (player.score == data[0].score) {
		while (1) {
			gotoxy(45, 7);
			printf("*NEW RECORD*");
			Sleep(500);
			gotoxy(45, 7);
			printf("            ");
			Sleep(500);
			if (kbhit()) {
				gotoxy(36, 19);
				exit(1);
			}
		}
	}
}

// 게임 종료
void gameover() {
	if (bestscore < score)
		bestscore = score;
	PlaySound(TEXT("gameover.wav"), 0, SND_ASYNC);
	textcolor(GRAY1, BLACK);
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++) {
			if (map[y][x] == BLOCK1 || map[y][x] == BLOCK2) {
				gotoxy2(x, y);
				printf("■");
				Sleep(10);
			}
		}
	for (int i = 5; i <= 20; i++) {
		gotoxy(24, i);
		printf("\t\t\t\t\t\t\t");
	}
	draw_box(34, 6, 66, 18);
	textcolor(WHITE, BLACK);
	gotoxy(40, 7);
	printf("┌─────────┐");
	gotoxy(40, 8);
	printf("│ G A M E  O V E R │");
	gotoxy(40, 9);
	printf("└─────────┘");
	gotoxy(40, 11);
	printf("     SCORE : %d", score);
	gotoxy(40, 13);
	printf("  Press R to Restart");
	gotoxy(40, 15);
	printf("  Press ESC to Quit ");
	while (1)
	{
		unsigned char ch;
		char c1;
		c1 = getch(); // key 값을 읽는다
		if (c1 == '\0') // VS 의 NULL 값은 Error이다.
			continue;
		else
			ch = c1;
		// ESC 누르면 프로그램 종료
		if (ch == ESC)
			insert_name();
		if (ch == 'r') {
			restart = 1;
			break;
		}
	}

}

// 블록 고정
void fix_block() {
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			if (map[y][x] == BLOCK1) {
				map[y][x] = BLOCK2;
				if (y == 1) {
					gameover();
					break;
				}
			}
	score += 10;
}

// 블록 한줄 파괴
void check_full() {
	int this_y = HEIGHT-2;
	int sum;
	while (this_y > 0) {
		sum = 0;
		for (int x = 1; x < WIDTH - 1; x++)
			if (map[this_y][x] == BLOCK2)
				sum++;
		if (sum == 10) {
			for (int x = 1; x < WIDTH - 1; x++) {
				gotoxy2(x, this_y);
				printf("●");
			}
			Sleep(100);
			for (int x = 1; x < WIDTH - 1; x++) {
				gotoxy2(x, this_y);
				printf("◎");
			}
			Sleep(100);
			for (int x = 1; x < WIDTH - 1; x++) {
				gotoxy2(x, this_y);
				printf("○");
			}
			Sleep(100);
			for (int y = this_y; y > 0; y--)
				for (int x = 1; x < WIDTH - 1; x++) {
					map[y][x] = map[y - 1][x];
					colormap[y][x] = colormap[y - 1][x];
				}
			score += 100;
			if (level < 5) {
				delay -= 10;
				speed -= 1;
				level += 1;
			}	
			draw_manual();
			draw_map();
		}
		else
			this_y--;
	}
}

// 땅에 닿을 곳 미리 표시
void create_preview() {
	for (int y = 0; y < HEIGHT - 1; y++)
		for (int x = 1; x < WIDTH - 1; x++)
			if (map[y][x] == PREVIEW)
				map[y][x] = EMPTY;
	
	int this_y = 0;
	while (1) {
		if (!is_collide(block_x, block_y + 1 + this_y, block_turn))
			this_y++;
		else {
			for (int y = 0; y < 4; y++)
				for (int x = 0; x < 4; x++)
					if (block[block_shape][block_turn][y][x] == 1 && map[block_y + y + this_y][block_x + x] == EMPTY)
						map[block_y + y + this_y][block_x + x] = PREVIEW;
			break;
		}
	}
}

// 게임 로직
void main()
{
	PlaySound(TEXT("start.wav"), 0, SND_ASYNC);
	unsigned char ch;
	srand(time(0));
	draw_main();
	getch();
	int flag = 0;

START:
	PlaySound(TEXT("maintheme.wav"), 0, SND_ASYNC | SND_LOOP);
	score = 0;
	level = 1;
	speed = 10;
	delay = 50;
	cls(BLACK, WHITE);
	removeCursor();
	create_map();
	create_block();
	draw_map();
	draw_manual();
	while (1)
	{
		if (kbhit() == 1) {  // 키보드가 눌려져 있으면
			char c1;
			c1 = getch(); // key 값을 읽는다
			if (c1 == '\0') // VS 의 NULL 값은 Error이다.
				continue;
			else
				ch = c1;
			if (ch == 'p')
				draw_pause();
			if (ch == SPACE) {
				while(!is_collide(block_x, block_y + 1, block_turn))
					block_move(block_x, block_y + 1, block_turn);
				fix_block();
				check_full();
				create_block();
				draw_manual();
			}
			if (ch == SPECIAL1) { // 만약 특수키
				// 예를 들어 UP key의 경우 0xe0 0x48 두개의 문자가 들어온다.
				ch = getch();
				// Player1은 방향키로 움직인다.
				switch (ch) {
				case UP: 
					if(!is_collide(block_x,block_y,(block_turn+1)%4))
						block_move(block_x, block_y, (block_turn + 1) % 4); break;
				case DOWN:
					if (!is_collide(block_x, block_y+1, block_turn))
						block_move(block_x, block_y+1, block_turn); break;
				case LEFT:
					if (!is_collide(block_x-1, block_y, block_turn))
						block_move(block_x-1, block_y, block_turn); break;
				case RIGHT:
					if (!is_collide(block_x+1, block_y, block_turn))
						block_move(block_x+1, block_y, block_turn); break;
				}
			}
		}
		if (!flag) {
			if (!is_collide(block_x, block_y + 1, block_turn))
				block_move(block_x, block_y + 1, block_turn);
			else {
				fix_block();
				check_full();
				create_block();
				draw_manual();

			}
			flag++;
		}
		else
			flag = (flag + 1) % speed;
		create_preview();
		draw_map();
		Sleep(delay);
		if (restart) {
			restart = 0;
			goto START;
		}
	}
}
