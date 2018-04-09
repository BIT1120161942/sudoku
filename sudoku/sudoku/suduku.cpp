#include<iostream>
#include<fstream>
#include<algorithm>
#include<cstring>
#include<time.h>
using namespace std;

int GetSudokuNumber(char * p);
void GetFinalShudu(int FinalSudoku[][10], int &num, int line, int rol, FILE*sudoku);
void GetRand(int FinalSudoku[][10], int line, int rol);
void WriteIntoFile(int FinalSudoku[][10], FILE*sudoku, int type, int &num, int*lineorder);
void ChangeFinalSugouku(int FinalSudoku[][10], int &num, FILE*sudoku);
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
	FILE *sudoku;
	int FinalSudoku[10][10];

	sudoku = fopen("sudoku.txt", "w");
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
			GetFinalShudu(FinalSudoku, ShuDuNumber, 0, 0, sudoku);
		}
		fclose(sudoku);
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
		fclose(sudoku);
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
void WriteIntoFile(int FinalSudoku[][10], FILE*sudoku, int type, int &num, int*lineorder)
{
	if (num <= 0)
		return;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (type == 0)
				fprintf(sudoku, "%d", FinalSudoku[lineorder[i]][j]);
			else
				fprintf(sudoku, "%d", FinalSudoku[j][lineorder[i]]);
			if (j < 8)
				fprintf(sudoku, " ");
			else
				fprintf(sudoku, "\n");
		}
	}
	fprintf(sudoku, "\n");
	num--;
}
void GetFinalShudu(int FinalSudoku[][10], int &num, int line, int rol, FILE*sudoku)
{
	if (num <= 0)
		return;
	int nextline, nextrol;
	nextrol = rol + 1;
	nextline = line + nextrol / 9;
	nextrol %= 9;
	if (FinalSudoku[line][rol] != 0)
	{
		if (line == 8 && rol == 8)
		{
			ChangeFinalSugouku(FinalSudoku, num, sudoku);
		}
		else
			GetFinalShudu(FinalSudoku, num, nextline, nextrol, sudoku);
	}
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
		int LineRegion = line / 3, RolRegion = rol / 3;			//与同宫不同
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
					ChangeFinalSugouku(FinalSudoku, num, sudoku);
				}
				else
					GetFinalShudu(FinalSudoku, num, nextline, nextrol, sudoku);
				FinalSudoku[line][rol] = 0;
			}
		}
	}
}
void ChangeFinalSugouku(int FinalSudoku[][10], int &num, FILE*sudoku)
{
	int lineorder[9] = { 0,1,2,3,4,5,6,7,8 };
	do {
		do {
			WriteIntoFile(FinalSudoku, sudoku, 0, num, lineorder);
			WriteIntoFile(FinalSudoku, sudoku, 1, num, lineorder);
		} while (next_permutation(lineorder + 6, lineorder + 9) && num);
		lineorder[6] = 6, lineorder[7] = 7, lineorder[8] = 8;
	} while (next_permutation(lineorder + 3, lineorder + 6) && num);
}