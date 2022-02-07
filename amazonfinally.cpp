#include <graphics.h>
#include <conio.h>
#include<stdio.h>
#include<cmath>
#include<algorithm>
#include<fstream>
int board[8][8] = { 0 };
int player = 1;
int flag = 0;
int hand[3] = { 0 };
int turn = 0;
int warns = 0;
int makemove[100][3][2] = { 0 };
int dx[] = { 0,1,0,-1,1,1,-1,-1 };
int dy[] = { 1,0,-1,0,1,-1,-1,1 };
void possiblemove(int x, int y);
void putchess(int a, int b, int c, int d, int p);
void putboundary(int a, int b, int c, int d, int p);
void cleanboard(int a, int b, int c, int d, int e, int f, int p);
int judgewinner(int p);
void cheerleader(int p);
void aimovechess(int p);
double judgevalue(int p);
void memsetdepth();
double maxmin(int p, double a, double b, int d);
int judgechess(int a, int b, int c, int d, int p);
void saveboard();
void review();
//char s[12];
//setfillcolor(BLACK);
//solidrectangle(0, 0, 75, 20);
//sprintf(s, "[%d %d] [%d %d] [%d %d]",begins[0],begins[1],afters[0],afters[1],put[0],put[1]);
//outtextxy(0, 0, s);


void initialchess() {
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j] = 0;
		}
	}
	board[2][0] = 1;
	board[5][0] = 1;
	board[0][2] = 1;
	board[7][2] = 1;
	board[2][7] = -1;
	board[5][7] = -1;
	board[0][5] = -1;
	board[7][5] = -1;
}

void printboard() {
	setbkcolor(RGB(65, 105, 225));
	cleardevice();
	char s[12];
	setfillcolor(RGB(65, 105, 225));
	solidrectangle(0, 0, 75, 20);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);

	sprintf(s, "[%d]", turn);
	outtextxy(0, 0, s);
	for (int i = 0; i < 370; i = i + 50)
	{
		for (int j = 0; j < 370; j = j + 50)
		{
			setfillcolor(RGB(176, 196, 222));
			solidroundrect(i + 33, j + 33, i + 77, j + 77, 2, 2);
		}
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 1)
			{
				setfillcolor(RGB(8, 37, 103));
				solidroundrect(i * 50 + 33, j * 50 + 33, i * 50 + 77, j * 50 + 77, 2, 2);
			}
			else if (board[i][j] == -1)
			{
				setfillcolor(RGB(240, 248, 255));
				solidroundrect(i * 50 + 33, j * 50 + 33, i * 50 + 77, j * 50 + 77, 2, 2);
			}
			else if (board[i][j] == 2)
			{
				setfillcolor(RGB(65, 105, 225));
				solidroundrect(i * 50 + 33, j * 50 + 33, i * 50 + 77, j * 50 + 77, 2, 2);
			}
		}
	}
	setfillcolor(WHITE);
	solidroundrect(530, 50, 570, 70, 2, 2);//存档
	solidroundrect(530, 130, 570, 150, 2, 2);//读档
	solidroundrect(530, 210, 570, 230, 2, 2);//欺负ai
	solidroundrect(530, 290, 570, 310, 2, 2);//悔棋
	solidroundrect(530, 370, 570, 390, 2, 2);//继续游戏

}

int finishc = 0;
void modechoose() {
	finishc++;

	//setbkcolor(RGB(65, 105, 225));
	cleardevice();
	setfillcolor(RGB(255, 77, 64));
	solidroundrect(250, 180, 300, 230, 2, 2);
	setfillcolor(RGB(0, 127, 255));
	solidroundrect(310, 180, 360, 230, 2, 2);
	MOUSEMSG choose;		// 定义鼠标消息

	while (true)
	{
		// 获取一条鼠标消息
		choose = GetMouseMsg();

		switch (choose.uMsg)
		{
		case WM_MOUSEMOVE:
			if (choose.x > 250 && choose.x < 300 && choose.y < 230 && choose.y > 180)
			{
				setfillcolor(RGB(255, 245, 238));
				solidroundrect(250, 180, 300, 230, 2, 2);
			}

			else if (choose.x > 310 && choose.x < 360 && choose.y < 230 && choose.y >180)
			{
				setfillcolor(RGB(240, 248, 255));
				solidroundrect(310, 180, 360, 230, 2, 2);
			}
			else
			{
				setfillcolor(RGB(255, 77, 64));
				solidroundrect(250, 180, 300, 230, 2, 2);
				setfillcolor(RGB(0, 127, 255));
				solidroundrect(310, 180, 360, 230, 2, 2);
			}

			break;

		case WM_LBUTTONDOWN:
		{

			if (choose.x > 250 && choose.x < 300 && choose.y < 230 && choose.y > 180)
			{
				if (finishc == 1)
					hand[finishc] = 1;
				if (finishc == 2)
					hand[finishc] = -1;
				return;

			}
			else if (choose.x > 310 && choose.x < 360 && choose.y < 230 && choose.y >180)
			{
				hand[finishc] = 0;
				return;
			}
		}

		break;

		default:
			break;
		}
	}
	//_getch();
	return;
}

int possible[8][8] = { 0 };
void possiblemove(int x, int y) {
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			possible[i][j] = 0;
		}
	}
	for (int k = 0; k < 8; k++)
	{
		for (int farth = 1; farth < 8; farth++)
		{
			int xx = x + dx[k] * farth;
			int yy = y + dy[k] * farth;
			if (xx < 0 || yy < 0 || xx>7 || yy > 7)
				break;
			if (board[xx][yy] != 0)//遇到障碍说明这个方向已经不能再走 
				break;
			possible[xx][yy] = 1;
			setfillcolor(RGB(250, 128, 114));
			solidroundrect(xx * 50 + 33, yy * 50 + 33, xx * 50 + 77, yy * 50 + 77, 2, 2);

		}
	}
	return;
}

void putchess(int a, int b, int c, int d, int p) {
	board[a][b] = 0;
	board[c][d] = p;
}

void putboundary(int a, int b, int c, int d, int p) {
	board[c][d] = 2;
}

void cleanboard(int a, int b, int c, int d, int e, int f, int p) {//清除落子 
	board[e][f] = 0;
	board[c][d] = 0;
	board[a][b] = p;
}

int judgechess(int a, int b, int c, int d, int p) {//判断落子是否合法 
	int i = 0;
	int j = 0;
	if (a < 0 || b < 0 || c < 0 || d < 0 || a >= 8 || b >= 8 || c >= 8 || d >= 8)
	{
		return 0;
	}
	if (board[a][b] != p)
	{
		return 0;
	}
	if (board[c][d] != 0)
	{
		return 0;
	}
	if (a != c && b != d && abs(a - c) != abs(b - d))
	{
		return 0;
	}
	if (a == c)
	{
		if (b < d)
		{
			for (i = b + 1; i < d; i++)
			{
				if (board[a][i] != 0)
				{
					return 0;
				}
			}
			return 1;
		}
		else
		{
			for (i = d + 1; i < b; i++)
			{
				if (board[a][i] != 0)
				{
					return 0;
				}
			}
			return 1;
		}
	}
	else if (a < c)
	{
		if (b == d)
		{
			for (i = a + 1; i < c; i++)
			{
				if (board[i][b] != 0)
				{
					return 0;
				}
			}
			return 1;
		}
		else if (b < d)
		{
			for (i = 1; i < c - a; i++)
			{
				if (board[a + i][b + i] != 0)
				{
					return 0;
				}
			}
			return 1;
		}
		else
		{
			for (i = 1; i < c - a; i++)
			{
				if (board[a + i][d + i] != 0)
				{
					return 0;
				}
			}
			return 1;
		}
	}
	else
	{
		if (b == d)
		{
			for (i = c + 1; i < a; i++)
			{
				if (board[i][b] != 0)
				{
					return 0;
				}
			}
			return 1;
		}
		else if (b < d)
		{
			for (i = 1; i < a - c; i++)
			{
				if (board[c + i][b + i] != 0)
				{
					if (flag == 0)
					{
						printf("非法棋步！棋子不能绕过障碍！\n");
						system("pause");
					}
					return 0;
				}
			}
			return 1;
		}
		else
		{
			for (i = 1; i < a - c; i++)
			{
				if (board[c + i][d + i] != 0)
				{
					if (flag == 0)
					{
						printf("非法棋步！棋子不能绕过障碍！\n");
						system("pause");
					}
					return 0;
				}
			}
			return 1;
		}
	}
}

int judgewinner(int p) {
	int i = 0;
	int j = 0;
	int k = 0;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (board[i][j] == p)
			{
				for (k = 0; k < 8; k++)
				{
					int xm = i + dx[k];
					int ym = j + dy[k];
					if (xm < 0 || ym < 0 || xm >= 8 || ym >= 8)
						continue;
					if (board[xm][ym] == 0)
					{
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

double mobility(int x) {
	int i, j, k, l = 0;
	int farth;
	int square = 0;
	int squares[4] = { 0 };
	int mark[8][8] = { 0 };
	int minsquare = 0;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (board[i][j] == x)
			{
				for (k = 0; k < 8; k++)
				{
					for (farth = 1; farth < 8; farth++)
					{
						int xm = i + dx[k] * farth;
						int ym = j + dy[k] * farth;
						if (!judgechess(i, j, xm, ym, x))//遇到障碍说明这个方向已经不能再走 
							break;
						if (mark[xm][ym] == 0)
						{
							squares[l]++;
							square++;
						}

					}
				}
				l++;
			}
		}
	}
	minsquare = squares[0];//最小灵活度 
	for (i = 0; i < 4; i++)
	{
		if (squares[i] < minsquare)
			minsquare = squares[i];
	}
	return (square * 0.8) + (minsquare * 0.2);
}

int timesq = 1;
int queen[3][8][8] = { 0 };
int xxq = 0;
int yyq = 0;
void QueenMove(int x, int y, int p) {
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int mark = 0;
	timesq = 1;
	queen[1 - p][x][y] = 0;//起点是第0步到达的地方 
	while (timesq < 100)
	{
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 8; j++)
			{
				if (queen[1 - p][i][j] == timesq - 1)//先找到上一次能达到的位置 
				{							  //他们是这一次的起点 
					for (k = 0; k < 8; k++)
					{
						for (l = 0; l < 8; l++)//遍历所有的走法 
						{
							xxq = i + dx[k] * l;
							yyq = j + dy[k] * l;
							if (xxq < 0 || yyq < 0 || xxq >= 8 || yyq >= 8)//遇到不合理的落子跳过 
								continue;
							if (board[xxq][yyq] != 0)
								break;
							if (queen[1 - p][xxq][yyq] < timesq)
								continue;
							mark = 1;//还有没走过的 
							queen[1 - p][xxq][yyq] = timesq;
						}
					}
				}
			}
		}
		timesq++;
		if (mark == 0)//如果能走的棋子已经走完 
			break;
		mark = 0;//最后mark归零，等待下一次调用 
	}
}

int king[3][8][8] = { 0 };
int xxk = 0, yyk = 0;
int timesk = 1;
void KingMove(int x, int y, int p) {
	timesk = 1;
	int i = 0;
	int j = 0;
	int k = 0;
	int mark = 0;
	king[1 - p][x][y] = 0;
	while (timesk < 100)
	{
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 8; j++)
			{
				if (king[1 - p][i][j] == timesk - 1)//寻找起点 
				{
					for (k = 0; k < 8; k++)
					{
						xxk = i + dx[k];
						yyk = j + dy[k];
						if (xxk < 0 || yyk < 0 || xxk >= 8 || yyk >= 8)
							continue;
						if (board[xxk][yyk] != 0)
							continue;
						if (king[1 - p][xxk][yyk] < timesk)//如果已经能用更少的步数到达，跳过 
							continue;
						mark = 1;
						king[1 - p][xxk][yyk] = timesk;
					}
				}
			}
		}
		timesk++;//遍历整个棋盘之后，该走下一步了 
		if (mark == 0)
			break;
		mark = 0;
	}
}

double judgeQueen(int x) {
	int i = 0;
	int j = 0;
	double control = 0;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (queen[1 - x][i][j] < queen[1 + x][i][j])//步数少，掌控力大 
			{
				control++;
			}
			if (queen[1 - x][i][j] == queen[1 + x][i][j])//如果两者一样，当前局面下先下的占优 
			{
				control = control + 0.5;
			}
			else//步数多，掌控力小 
			{
				control--;
			}
		}
	}
	return control;
}

double judgeKing(int x) {
	int i = 0;
	int j = 0;
	double control = 0;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (king[1 - x][i][j] < king[1 + x][i][j])
			{
				control++;
			}
			if (king[1 - x][i][j] == king[1 + x][i][j])
			{
				control = control + 0.5;
			}
			else
			{
				control--;
			}
		}
	}
	return control;
}

double positionk = 0;
double positionq = 0;
void position(int p) {
	int i = 0;
	int j = 0;
	positionk = 0;
	positionq = 0;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			positionk = positionk + pow(2, -king[1 - p][i][j]) - pow(2, -king[1 + p][i][j]);
			positionq = positionq + min(1, max(-1, (queen[1 + p][i][j] - queen[1 - p][i][j]) / 6));
		}
	}
}

//综合所有估值，得到最终的价值 
double judgevalue(int p) {
	int i = 0;
	int j = 0;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			king[0][i][j] = 100;
			queen[0][i][j] = 100;
			king[2][i][j] = 100;
			queen[2][i][j] = 100;
		}
	}
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (board[i][j] == 1)
			{
				QueenMove(i, j, 1);
				KingMove(i, j, 1);
			}
			if (board[i][j] == -1)
			{
				QueenMove(i, j, -1);
				KingMove(i, j, -1);
			}
		}
	}
	position(p);

	if (turn <= 16)
	{
		return 0.14 * judgeQueen(p) + 0.37 * judgeKing(p) + 0.13 * positionk + 0.13 * positionq + 0.2 * mobility(p);
	}
	if (turn > 16 && turn < 40)
	{
		return 0.3 * judgeQueen(p) + 0.25 * judgeKing(p) + 0.2 * positionk + 0.13 * positionq + 0.05 * mobility(p);
	}
	if (turn >= 40)
	{
		return 0.8 * judgeQueen(p) + 0.1 * judgeKing(p) + 0.05 * positionk + 0.05 * positionq;
	}
}

double alpha = -100000;
double beta = 100000;
int depth[100] = { 0 };
int deep = 0;
double tempvalue = 0;
double finalmax = 0;
int begins[2] = { 0 };
int afters[2] = { 0 };
int put[2] = { 0 };

void memsetdepth() {
	for (int i = 0; i <= 16; i++)
	{
		depth[i] = 1;
	}
	for (int i = 17; i < 40; i++)
	{
		depth[i] = 2;
	}
	for (int i = 40; i < 100; i++)
	{
		depth[i] = 3;
	}
	return;
}

int mark = 0;
double maxmin(int p, double a, double b, int d) {
	int i = 0, j = 0, k = 0, l = 0;
	int far1 = 0, far2 = 0;
	//	int xx = 0, yy = 0;
	//	int xxx = 0, yyy = 0;
	alpha = -100000;
	beta = 100000;
	if (d == depth[turn])//如果搜够了层数，调用估值函数返回 
	{
		double valuearn = 0;
		valuearn = judgevalue(player);
		return valuearn;
	}
	else
	{
		mark = 0;
		if (p == player)//奇数层 
		{
			for (i = 0; i < 8; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (board[i][j] == p)
					{
						for (k = 0; k < 8; k++)
						{
							for (far1 = 1; far1 <= 8; far1++)
							{
								int xx = i + dx[k] * far1;
								int yy = j + dy[k] * far1;
								if (!judgechess(i, j, xx, yy, p))
									break;
								putchess(i, j, xx, yy, p);
								for (l = 0; l < 8; l++)
								{
									for (far2 = 1; far2 <= 7; far2++)
									{
										int xxx = xx + dx[l] * far2;
										int yyy = yy + dy[l] * far2;
										if (!judgechess(xx, yy, xxx, yyy, p))
											break;
										mark = 1;
										putboundary(xx, yy, xxx, yyy, p);
										tempvalue = maxmin(-player, a, b, d + 1);
										board[xxx][yyy] = 0;
										printf("(%d %d %d %d %d %d %d)\n", board[0][0], i, j, xx, yy, xxx, yyy);
										if (tempvalue > a)//如果max层找到了更大值，替换 
										{
											a = tempvalue;
											if (d == 0)
											{
												begins[0] = i;//记录当前最优解 
												begins[1] = j;
												afters[0] = xx;
												afters[1] = yy;
												put[0] = xxx;
												put[1] = yyy;
											}
										}
										if (a > b)//取大值，剪枝 
										{
											cleanboard(i, j, xx, yy, xxx, yyy, p);
											return a;
										}
									}
								}
								board[xx][yy] = 0;
								board[i][j] = p;
							}
						}
					}
				}
			}

			return a;
		}
		else
		{
			for (i = 0; i < 8; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (board[i][j] == p)
					{
						for (k = 0; k < 8; k++)
						{
							for (far1 = 1; far1 <= 8; far1++)
							{
								int xx = i + dx[k] * far1;
								int yy = j + dy[k] * far1;
								if (!judgechess(i, j, xx, yy, p))
									break;
								putchess(i, j, xx, yy, p);
								for (l = 0; l < 8; l++)
								{
									for (far2 = 1; far2 <= 8; far2++)
									{
										int xxx = xx + dx[l] * far2;
										int yyy = yy + dy[l] * far2;
										if (!judgechess(xx, yy, xxx, yyy, p))
											break;
										putboundary(xx, yy, xxx, yyy, p);
										mark = 1;
										tempvalue = maxmin(player, alpha, beta, d + 1);
										board[xxx][yyy] = 0;
										if (tempvalue < b)//如果min层找到了更小值，替换 
											b = tempvalue;
										if (a > b)//取小值，剪枝 
										{
											cleanboard(i, j, xx, yy, xxx, yyy, p);
											return b;
										}
									}
								}
								board[xx][yy] = 0;
								board[i][j] = p;
							}
						}
					}
				}
			}
			//if (mark == 1)
				//d = depth[turn];
			return b;
		}
	}
}

void aimovechess(int p) {
	memsetdepth();//根据回合数决定搜索深度 
	maxmin(p, alpha, beta, 0);//极大极小搜索+ab剪枝 
	putchess(begins[0], begins[1], afters[0], afters[1], p);//落子 
	putboundary(afters[0], afters[1], put[0], put[1], p);//落障碍 
	makemove[turn][0][0] = begins[0];//为了复盘 
	makemove[turn][0][1] = begins[1];
	makemove[turn][1][0] = afters[0];
	makemove[turn][1][1] = afters[1];
	makemove[turn][2][0] = put[0];
	makemove[turn][2][1] = put[1];
	return;
}

void review() {
	initialchess();
	player = 1;
	int initurn = turn;
	turn = 1;
	printboard();
	setfillcolor(RGB(65, 105, 225));
	solidroundrect(530, 50, 570, 70, 2, 2);//存档
	solidroundrect(530, 130, 570, 150, 2, 2);//读档
	solidroundrect(530, 210, 570, 230, 2, 2);//欺负ai
	solidroundrect(530, 290, 570, 310, 2, 2);//悔棋
	for (int i = 1; i <= initurn; i++)
	{

		MOUSEMSG g;
		while (true)
		{
			g = GetMouseMsg();
			if (g.uMsg == WM_MOUSEMOVE)
			{
				if (g.x > 520 && g.x < 580 && g.y >360 && g.y < 410)
				{
					outtextxy(534, 372, "继续");
				}
				else
				{
					setfillcolor(WHITE);
					solidroundrect(530, 370, 570, 390, 2, 2);//继续游戏
				}
			}
			if (g.uMsg == WM_LBUTTONDOWN)
			{
				if (g.x > 520 && g.x < 580 && g.y >360 && g.y < 410)
				{
					break;
				}
			}
		}
		turn = i;
		putchess(makemove[i][0][0], makemove[i][0][1], makemove[i][1][0], makemove[i][1][1], player);
		putboundary(makemove[i][1][0], makemove[i][1][1], makemove[i][2][0], makemove[i][2][1], player);

		player = player * -1;
		printboard();
		setfillcolor(RGB(65, 105, 225));
		solidroundrect(530, 50, 570, 70, 2, 2);//存档
		solidroundrect(530, 130, 570, 150, 2, 2);//读档
		solidroundrect(530, 210, 570, 230, 2, 2);//欺负ai
		solidroundrect(530, 290, 570, 310, 2, 2);//悔棋
	}
	return;
}

void cheerleader(int p) {
	setbkcolor(RGB(65, 105, 225));
	cleardevice();
	if (p == 1)
	{
		outtext("BLACK WINS");
		MOUSEMSG e;
		while (true)
		{
			e = GetMouseMsg();
			if (e.uMsg == WM_LBUTTONDOWN)
			{
				break;
			}
		}
	}
	else if (p == -1)
	{
		outtext("WHITE WINS");
		MOUSEMSG e;
		while (true)
		{
			e = GetMouseMsg();
			if (e.uMsg == WM_LBUTTONDOWN)
			{
				break;
			}
		}

	}
	setbkcolor(RGB(65, 105, 225));
	cleardevice();
	setfillcolor(WHITE);
	solidrectangle(186, 160, 462, 320);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(276, 180, "是否复盘？");
	setfillcolor(GREEN);
	solidrectangle(250, 280, 300, 300);
	setfillcolor(RED);
	solidrectangle(330, 280, 380, 300);

	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x > 250 && m.y > 280 && m.x < 300 && m.y < 300)
			{
				review();
				break;

			}
			if (m.x > 330 && m.y > 280 && m.x < 380 && m.y < 300)
			{
				break;
			}
		}
	}
	closegraph();
	exit(0);
}


void saveboard() {
	std::ofstream outfile("cundang.txt");
	if (!outfile)
		return;
	outfile << turn << " ";
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == -1)
				outfile << i << " " << j << " ";
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 1)
				outfile << i << " " << j << " ";
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 2)
			{
				outfile << i << " " << j << " ";
			}
		}
	}
	outfile.close();
	setfillcolor(WHITE);
	solidrectangle(186, 160, 462, 320);
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	setbkmode(TRANSPARENT);
	settextcolor(GREEN);
	f.lfHeight = 18;
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	outtextxy(276, 180, _T("存档成功"));
	setfillcolor(RGB(65, 105, 225));
	solidrectangle(296, 260, 336, 280);
	MOUSEMSG g;
	while (true)
	{
		g = GetMouseMsg();

		if (g.uMsg == WM_LBUTTONDOWN)
		{
			if (g.x > 296 && g.y > 260 && g.x < 336 && g.y < 280)
			{
				printboard();
				warns = 0;
				break;
			}
		}
	}
	return;
}

void readboard() {
	std::ifstream infile("cundang.txt");
	if (!infile)
		return;
	int whitec[100][2] = { 0 };
	int blackc[100][2] = { 0 };
	int boundaryc[100][2] = { 0 };
	int turnnum = 0;
	infile >> turnnum;
	turn = turnnum;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j] = 0;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		infile >> whitec[i][0] >> whitec[i][1];
		board[whitec[i][0]][whitec[i][1]] = -1;
	}
	for (int i = 0; i < 4; i++)
	{
		infile >> blackc[i][0] >> blackc[i][1];
		board[blackc[i][0]][blackc[i][1]] = 1;
	}
	for (int i = 0; i < turn; i++)
	{
		infile >> boundaryc[i][0] >> boundaryc[i][1];
		board[boundaryc[i][0]][boundaryc[i][1]] = 2;
	}
	infile.close();
	setfillcolor(WHITE);
	solidrectangle(186, 160, 462, 320);
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	setbkmode(TRANSPARENT);
	settextcolor(GREEN);
	f.lfHeight = 18;
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	outtextxy(276, 180, _T("读档成功"));
	setfillcolor(RGB(65, 105, 225));
	solidrectangle(296, 260, 336, 280);
	MOUSEMSG g;
	while (true)
	{
		g = GetMouseMsg();

		if (g.uMsg == WM_LBUTTONDOWN)
		{
			if (g.x > 296 && g.y > 260 && g.x < 336 && g.y < 280)
			{
				printboard();
				warns = 0;
				break;
			}
		}
	}
	return;
}

void beatai() {
	int mark[8][8] = { 0 };
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == 1)
			{
				mark[i][j] = 1;
				board[i][j] = -1;
			}
			if (board[i][j] == -1 && mark[i][j] == 0)
			{
				board[i][j] = 1;
			}
		}
	}
	printboard();
	return;
}

int bondmove = 0;
void maingame() {
	turn++;
	bondmove = 0;
	warns = 0;
	printboard();

	if (player == 1 && hand[1] == 1)
	{
		if (judgewinner(player))
		{
			MOUSEMSG m;		// 定义鼠标消息
			int changedside = 0;
			while (true)
			{
				m = GetMouseMsg();
				if (m.uMsg == WM_MOUSEMOVE)
				{
					if (m.x > 520 && m.x < 580 && m.y >360 && m.y < 410)
					{
						setbkmode(TRANSPARENT);
						settextcolor(RGB(65, 105, 225));
						outtextxy(534, 372, "继续");
					}
					else
					{
						setfillcolor(WHITE);
						solidroundrect(530, 370, 570, 390, 2, 2);//继续游戏
					}
					if (m.x > 520 && m.x < 580)
					{
						if (m.y > 40 && m.y < 90) //存档
						{
							setbkmode(TRANSPARENT);
							settextcolor(RGB(65, 105, 225));
							outtextxy(534, 52, "存档");
						}
						else
						{
							setfillcolor(WHITE);
							solidroundrect(530, 50, 570, 70, 2, 2);//存档

						}
						if (m.y > 120 && m.y < 170)
						{
							setbkmode(TRANSPARENT);
							settextcolor(RGB(65, 105, 225));
							outtextxy(534, 132, "读档");
						}
						else
						{
							setfillcolor(WHITE);
							solidroundrect(530, 130, 570, 150, 2, 2);//读档
						}
						if (m.y > 200 && m.y < 250)
						{
							setbkmode(TRANSPARENT);
							settextcolor(RGB(65, 105, 225));
							outtextxy(529, 212, "欺负ai");
						}
						else
						{
							setfillcolor(WHITE);
							solidroundrect(530, 210, 570, 230, 2, 2);//欺负ai
						}
						if (m.y > 280 && m.y < 330)
						{
							setbkmode(TRANSPARENT);
							settextcolor(RGB(65, 105, 225));
							outtextxy(534, 292, "悔棋");
						}
						else
						{
							setfillcolor(WHITE);
							solidroundrect(530, 290, 570, 310, 2, 2);//悔棋
						}
					}
				}

				if (m.uMsg == WM_LBUTTONDOWN)
				{
					int i = (m.x - 33) / 50;
					int j = (m.y - 33) / 50;
					if (i >= 0 && i <= 7 && j >= 0 && j <= 7)
					{
						if (board[i][j] == player && !bondmove)
						{
							printboard();
							setfillcolor(RGB(176, 196, 222));
							solidroundrect(i * 50 + 33, j * 50 + 33, i * 50 + 77, j * 50 + 77, 2, 2);
							setfillcolor(RGB(8, 37, 103));
							solidroundrect(i * 50 + 43, j * 50 + 43, i * 50 + 67, j * 50 + 67, 2, 2);
							possiblemove(i, j);
							makemove[turn][0][0] = i;
							makemove[turn][0][1] = j;
						}
						else if (possible[i][j] && !bondmove)
						{
							bondmove++;
							putchess(makemove[turn][0][0], makemove[turn][0][1], i, j, player);
							makemove[turn][1][0] = i;
							makemove[turn][1][1] = j;
							printboard();
							possiblemove(i, j);
						}
						else if (possible[i][j] && bondmove == 1)
						{
							bondmove++;
							putboundary(makemove[turn][1][0], makemove[turn][1][1], i, j, player);
							makemove[turn][2][0] = i;
							makemove[turn][2][1] = j;
							printboard();
							//break;
						}
					}

					else if (m.x > 520 && m.x < 580 && m.y >360 && m.y < 410 && bondmove == 2)
					{
						break;
					}
					else if (m.x > 520 && m.x < 580 && bondmove == 2)
					{
						if (m.y > 40 && m.y < 90) //存档
						{
							saveboard();
						}
						if (m.y > 120 && m.y < 170)
						{
							readboard();
						}
						if (m.y > 200 && m.y < 250)
						{
							changedside = 1;
							beatai();
						}
						if (m.y > 280 && m.y < 330)
						{
							if (changedside || turn == 0)
							{
								setfillcolor(WHITE);
								solidrectangle(186, 160, 462, 320);
								LOGFONT f;
								gettextstyle(&f);                     // 获取当前字体设置
								setbkmode(TRANSPARENT);
								settextcolor(RED);
								f.lfHeight = 18;
								f.lfQuality = ANTIALIASED_QUALITY;
								settextstyle(&f);
								outtextxy(276, 180, _T("不能进行该操作"));
								setfillcolor(RGB(65, 105, 225));
								solidrectangle(296, 260, 336, 280);
								MOUSEMSG g;
								while (true)
								{
									g = GetMouseMsg();

									if (g.uMsg == WM_LBUTTONDOWN)
									{
										if (g.x > 296 && g.y > 260 && g.x < 336 && g.y < 280)
										{
											printboard();
											warns = 0;
											break;
										}
									}
								}
							}
							else
							{
								cleanboard(makemove[turn][0][0], makemove[turn][0][1], makemove[turn][1][0], makemove[turn][1][1], makemove[turn][2][0], makemove[turn][2][1], player);
								turn--;
								player = player * -1;
								printboard();
							}
						}

					}
					else if (m.x > 520 && m.x < 580 && bondmove < 2)
					{
						warns = 1;
						setfillcolor(WHITE);
						solidrectangle(186, 160, 462, 320);
						LOGFONT f;
						gettextstyle(&f);                     // 获取当前字体设置
						setbkmode(TRANSPARENT);
						settextcolor(RED);
						f.lfHeight = 18;
						f.lfQuality = ANTIALIASED_QUALITY;
						settextstyle(&f);
						outtextxy(276, 180, _T("请完成落子"));
						setfillcolor(RGB(65, 105, 225));
						solidrectangle(296, 260, 336, 280);
						MOUSEMSG g;
						while (true)
						{
							g = GetMouseMsg();

							if (g.uMsg == WM_LBUTTONDOWN)
							{
								if (g.x > 296 && g.y > 260 && g.x < 336 && g.y < 280)
								{
									printboard();
									warns = 0;
									break;
								}
							}
						}
					}
				}
			}
			player = player * -1;
			maingame();
		}
		else
		{
			cheerleader(-1);
			return;
		}
	}
	if (player == 1 && hand[1] == 0)
	{
		flag = 1;
		if (judgewinner(player))
		{
			aimovechess(player);
			MOUSEMSG g;
			while (true)
			{
				g = GetMouseMsg();
				if (g.uMsg == WM_MOUSEMOVE)
				{
					if (g.x > 520 && g.x < 580 && g.y >360 && g.y < 410)
					{
						setbkmode(TRANSPARENT);
						settextcolor(RGB(65, 105, 225));
						outtextxy(534, 372, "继续");
					}
					else
					{
						setfillcolor(WHITE);
						solidroundrect(530, 370, 570, 390, 2, 2);//继续游戏
					}
				}
				if (g.uMsg == WM_LBUTTONDOWN)
				{
					if (g.x > 520 && g.x < 580 && g.y >360 && g.y < 410)
					{
						break;
					}
				}
			}
			player = player * -1;
			maingame();
		}
		else
		{
			cheerleader(-1);
			return;
		}
	}
	if (player == -1 && hand[2] == -1)
	{
		if (judgewinner(player))
		{
			MOUSEMSG m;		// 定义鼠标消息
			int changedside = 0;
			while (true)
			{
				m = GetMouseMsg();
				if (m.uMsg == WM_MOUSEMOVE)
				{
					if (m.x > 520 && m.x < 580 && m.y >360 && m.y < 410)
					{
						setbkmode(TRANSPARENT);
						settextcolor(RGB(65, 105, 225));
						outtextxy(534, 372, "继续");
					}
					else
					{
						setfillcolor(WHITE);
						solidroundrect(530, 370, 570, 390, 2, 2);//继续游戏
					}
					if (m.x > 520 && m.x < 580)
					{
						if (m.y > 40 && m.y < 90) //存档
						{
							setbkmode(TRANSPARENT);
							settextcolor(RGB(65, 105, 225));
							outtextxy(534, 52, "存档");
						}
						else
						{
							setfillcolor(WHITE);
							solidroundrect(530, 50, 570, 70, 2, 2);//存档

						}
						if (m.y > 120 && m.y < 170)
						{
							setbkmode(TRANSPARENT);
							settextcolor(RGB(65, 105, 225));
							outtextxy(534, 132, "读档");
						}
						else
						{
							setfillcolor(WHITE);
							solidroundrect(530, 130, 570, 150, 2, 2);//读档
						}
						if (m.y > 200 && m.y < 250)
						{
							setbkmode(TRANSPARENT);
							settextcolor(RGB(65, 105, 225));
							outtextxy(529, 212, "欺负ai");
						}
						else
						{
							setfillcolor(WHITE);
							solidroundrect(530, 210, 570, 230, 2, 2);//欺负ai
						}
						if (m.y > 280 && m.y < 330)
						{
							setbkmode(TRANSPARENT);
							settextcolor(RGB(65, 105, 225));
							outtextxy(534, 292, "悔棋");
						}
						else
						{
							setfillcolor(WHITE);
							solidroundrect(530, 290, 570, 310, 2, 2);//悔棋
						}
					}
				}

				if (m.uMsg == WM_LBUTTONDOWN)//如果接收到一个点击
				{
					int i = (m.x - 33) / 50;//转化成坐标
					int j = (m.y - 33) / 50;
					if (i >= 0 && i <= 7 && j >= 0 && j <= 7)//如果在棋盘范围内
					{
						if (board[i][j] == player && !bondmove)//如果目前还没有落子，选中要动的棋子
						{
							printboard();
							setfillcolor(RGB(176, 196, 222));
							solidroundrect(i * 50 + 33, j * 50 + 33, i * 50 + 77, j * 50 + 77, 2, 2);
							setfillcolor(RGB(240, 248, 255));
							solidroundrect(i * 50 + 43, j * 50 + 43, i * 50 + 67, j * 50 + 67, 2, 2);
							possiblemove(i, j);
							makemove[turn][0][0] = i;
							makemove[turn][0][1] = j;
						}
						else if (possible[i][j] && !bondmove)//如果目前还没有落子，选择目的地
						{
							bondmove++;
							putchess(makemove[turn][0][0], makemove[turn][0][1], i, j, player);
							makemove[turn][1][0] = i;
							makemove[turn][1][1] = j;
							printboard();
							possiblemove(i, j);
						}
						else if (possible[i][j] && bondmove == 1)//已经落子，要落障碍了
						{
							bondmove++;
							putboundary(makemove[turn][1][0], makemove[turn][1][1], i, j, player);
							makemove[turn][2][0] = i;
							makemove[turn][2][1] = j;
							printboard();
							//break;
						}
					}

					else if (m.x > 520 && m.x < 580 && m.y >360 && m.y < 410 && bondmove == 2)//已经落完障碍，可以继续了
					{
						break;
					}
					else if (m.x > 520 && m.x < 580 && bondmove == 2)//如果需要特殊功能
					{
						if (m.y > 40 && m.y < 90) //存档
						{
							saveboard();
						}
						if (m.y > 120 && m.y < 170)//读档
						{
							readboard();
						}
						if (m.y > 200 && m.y < 250)//转换黑白双方
						{
							changedside = 1;
							beatai();
						}
						if (m.y > 280 && m.y < 330)//悔棋
						{
							if (changedside || turn == 0)//已经悔到开头，已经换过先后手，不能悔棋
							{
								setfillcolor(WHITE);
								solidrectangle(186, 160, 462, 320);
								LOGFONT f;
								gettextstyle(&f);
								setbkmode(TRANSPARENT);
								settextcolor(RED);
								f.lfHeight = 18;
								f.lfQuality = ANTIALIASED_QUALITY;
								settextstyle(&f);
								outtextxy(276, 180, _T("不能进行该操作"));//弹窗
								setfillcolor(RGB(65, 105, 225));
								solidrectangle(296, 260, 336, 280);
								MOUSEMSG g;
								while (true)
								{
									g = GetMouseMsg();

									if (g.uMsg == WM_LBUTTONDOWN)
									{
										if (g.x > 296 && g.y > 260 && g.x < 336 && g.y < 280)
										{
											printboard();
											warns = 0;
											break;
										}
									}
								}
							}
							else
							{
								cleanboard(makemove[turn][0][0], makemove[turn][0][1], makemove[turn][1][0], makemove[turn][1][1], makemove[turn][2][0], makemove[turn][2][1], player);
								turn--;
								player = player * -1;
								printboard();
							}
						}

					}
					else if (m.x > 520 && m.x < 580 && bondmove < 2)
					{
						warns = 1;
						setfillcolor(WHITE);
						solidrectangle(186, 160, 462, 320);
						LOGFONT f;
						gettextstyle(&f);                     // 获取当前字体设置
						setbkmode(TRANSPARENT);
						settextcolor(RED);
						f.lfHeight = 18;
						f.lfQuality = ANTIALIASED_QUALITY;
						settextstyle(&f);
						outtextxy(276, 180, _T("请完成落子"));
						setfillcolor(RGB(65, 105, 225));
						solidrectangle(296, 260, 336, 280);
						MOUSEMSG g;
						while (true)
						{
							g = GetMouseMsg();

							if (g.uMsg == WM_LBUTTONDOWN)
							{
								if (g.x > 296 && g.y > 260 && g.x < 336 && g.y < 280)
								{
									printboard();
									warns = 0;
									break;
								}
							}
						}
					}
				}
			}
			player = player * -1;
			maingame();
		}
		else
		{
			cheerleader(1);
			return;
		}
	}
	if (player == -1 && hand[2] == 0)
	{
		flag = 1;
		if (judgewinner(player))
		{
			aimovechess(player);
			MOUSEMSG g;
			while (true)
			{
				g = GetMouseMsg();
				if (g.uMsg == WM_MOUSEMOVE)
				{
					if (g.x > 520 && g.x < 580 && g.y >360 && g.y < 410)
					{
						setbkmode(TRANSPARENT);
						settextcolor(RGB(65, 105, 225));
						outtextxy(534, 372, "继续");
					}
					else
					{
						setfillcolor(WHITE);
						solidroundrect(530, 370, 570, 390, 2, 2);//继续游戏
					}
				}
				if (g.uMsg == WM_LBUTTONDOWN)
				{
					if (g.x > 520 && g.x < 580 && g.y >360 && g.y < 410)
					{
						break;
					}
				}
			}

			player = player * -1;
			maingame();
		}
		else
		{
			cheerleader(1);
			return;
		}
	}
}

int moved = 0;
int main()
{
	initgraph(640, 480);
	setbkcolor(BLACK);
	modechoose();
	setbkcolor(WHITE);
	modechoose();
	initialchess();
	printboard();
	maingame();
}

