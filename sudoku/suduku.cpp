#include<iostream>
#include<fstream>
#include<algorithm>
#include<cstring>
#include<ctime>
#include<time.h>
using namespace std;

int GetSudokuNumber(char * p);
void GetFinalShudu(int FinalSudoku[][10], int &num, int line, int rol, ofstream & Sudoku);
void GetRand(int FinalSudoku[][10], int line, int rol);
int main(int argc, char * argv[])
{
	clock_t start, end;
	start = clock();
	if (argc != 3)				//参数数目不足，显示异常
	{
		cout << "Input Error" << endl;
		return 0;
	}

	char *Argument = argv[1];
	char *DeputyArgument = argv[2];
	ofstream Sudoku("sudoku.txt");
	int FinalSudoku[10][10];

	if (!strcmp(Argument, "-c"))
	{
		int ShuDuNumber = GetSudokuNumber(DeputyArgument);
		if (ShuDuNumber < 1 || ShuDuNumber>1000000)			//数字不正确或超出范围，报错
		{
			cout << "Input Error" << endl;
			return 0;
		}
		srand((unsigned)time(NULL));
		memset(FinalSudoku, 0, sizeof(FinalSudoku));
		while (ShuDuNumber > 0)
		{
			GetRand(FinalSudoku, 0, 0);			//将三个无关的九宫格用随机数填满
			GetRand(FinalSudoku, 1, 2);
			GetRand(FinalSudoku, 2, 1);
			for (int i = 0; i<3; i++)		//按要求调整第一个九宫格时第一个数为（4+2）%9+1
				for (int j = 0; j < 3; j++)
				{
					if (FinalSudoku[i][j] == 7)
						FinalSudoku[i][j] = FinalSudoku[0][0];
				}
			FinalSudoku[0][0] = 7;
			GetFinalShudu(FinalSudoku, ShuDuNumber, 0, 0, Sudoku);
			/*for (int i = 0; i < 9; i++)
			{
			for (int j = 0; j < 9; j++)
			cout << ShuDu[i][j] << " ";
			cout << endl;
			}
			cout << endl;
			ShuDuNumber--;*/
		}
		Sudoku.close();
	}
	else if (!strcmp(Argument, "-s"))
	{
		ifstream  ShuDuPuzzle(DeputyArgument);
		if (!ShuDuPuzzle)								//路径错误，未打开文件，报错
		{
			cout << "Path Error" << endl;
			return 0;
		}
		char ch;
		while (!ShuDuPuzzle.eof())
		{
			ShuDuPuzzle.get(ch);

		}
		ShuDuPuzzle.close();
		Sudoku.close();
	}
	else
	{
		cout << "Input Error" << endl;
	}
	end = clock();
	double dur = (double)(end - start);
	printf("Time:%f\n", (dur / CLOCKS_PER_SEC));


	return 0;
}
int GetSudokuNumber(char * p)
{
	int number = 0;
	for (int i = 0; p[i] != '\0'; i++)
	{
		if (!(p[i] >= '0'&&p[i] <= '9'))
		{
			return -1;
		}
		number = number * 10 + p[i] - '0';
	}
	return number;
}
void GetFinalShudu(int FinalSudoku[][10], int &num, int line, int rol, ofstream & Sudoku)
{
	//cout << line << " " << rol << endl;
	/*for (int i = 0; i < 9; i++)
	{
	for (int j = 0; j < 9; j++)
	cout << ShuDu[i][j] << " ";
	cout << endl;
	}
	cout << endl;*/
	if (num <= 0)
		return;
	int nextline, nextrol;
	nextrol = rol + 1;
	nextline = line + nextrol / 9;
	nextrol %= 9;
	if (FinalSudoku[line][rol] != 0)
		GetFinalShudu(FinalSudoku, num, nextline, nextrol, Sudoku);
	else
	{
		int flag[10] = { 0 };
		for (int i = 0; i < 9; i++)
		{
			if (FinalSudoku[line][i] != 0)					//与同行不同
				flag[FinalSudoku[line][i]] = 1;
			if (FinalSudoku[i][rol] != 0)						//与同列不同
				flag[FinalSudoku[i][rol]] = 1;
		}
		int LineRegion = line / 3, RolRegion = rol / 3;			//与同块不同
		for (int i = 3 * LineRegion; i <= 3 * LineRegion + 2; i++)
		{
			for (int j = 3 * RolRegion; j <= 3 * RolRegion + 2; j++)
			{
				if (FinalSudoku[i][j] != 0)
					flag[FinalSudoku[i][j]] = 1;
			}
		}
		for (int i = 1; i < 10; i++)
		{
			if (flag[i] == 0)
			{
				FinalSudoku[line][rol] = i;
				if (line == 8 && rol == 8)
				{
					for (int i = 0; i < 9; i++)
					{
						for (int j = 0; j < 8; j++)
							Sudoku << FinalSudoku[i][j] << " ";
						Sudoku << FinalSudoku[i][8] << endl;
					}
					Sudoku << endl;
					num--;
				}
				else
					GetFinalShudu(FinalSudoku, num, nextline, nextrol, Sudoku);
				FinalSudoku[line][rol] = 0;
			}
		}
	}
}
void GetRand(int FinalSudoku[][10], int line, int rol)
{
	int flag[10] = { 0 }, num;
	for (int i = 3 * line; i <= 3 * line + 2; i++)
	{
		for (int j = 3 * rol; j <= 3 * rol + 2; j++)
		{
			num = rand() % 9 + 1;
			while (flag[num] != 0)
				num = num % 9 + 1;
			flag[num] = 1;
			FinalSudoku[i][j] = num;
		}
	}
}