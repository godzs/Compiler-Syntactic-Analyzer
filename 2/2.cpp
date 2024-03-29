// 2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;

char symbol[20];//符号栈
char expr[20];//输入的表达式
char term[20] = { 'i','+','*','(',')','#' };//终结符
char nterm[20] = { 'E','G','T','S','F' };//非终结符 
int  cur_l = 0, top = 0;
int  l_in;   //L为输入的字符长度

typedef struct type   
{
	char origin;//文法左边 
	char array[5];//文法右边 
	int length; //文法长度 
	char select[10];  //select集
}type;
type T[10];

typedef struct first
{
	char ori;
	char a[10];  //first集
	char b[10];  //follow集
}fi;
fi F[10];

type C[10][10];    //预测分析表


void gram()
{
		T[0].origin = 'E';
		strcpy(T[0].array,"TG");
		T[0].length = 2;

		T[1].origin = 'G';
		strcpy(T[1].array, "+TG");
		T[1].length = 3;

		T[2].origin = 'G';
		T[2].array[0]= '^';
		T[2].length = 1;

		T[3].origin = 'T';
		strcpy(T[3].array, "FS");
		T[3].length = 2;

		T[4].origin = 'S';
		strcpy(T[4].array, "*FS");
		T[4].length = 3;

		T[5].origin = 'S';
		T[5].array[0]='^';
		T[5].length = 1;

		T[6].origin = 'F';
		strcpy(T[6].array, "(E)");
		T[6].length = 3;

		T[7].origin = 'F';
		strcpy(T[7].array, "i");
		T[7].length = 1;
}

void fw()  //写入first集和follow集
{
	int k;
	for (k = 0; k < 5; k++)
	{
	//	cout << "请输入" << nterm[k] << "的FIRST集:";
		F[k].ori = nterm[k];
		cin >> F[k].a;
		//cout << "请输入" << nterm[k] << "的FOLLOW集:";
		cin >> F[k].b;
	}
}

int firstt(char a)  //判断文法左部是否有该字符
{
	int i;
	char c;
	for (i = 0; i < 10; i++)
	{
		c = F[i].ori;
		if (c == a)
			return i;
	}
	return 0;
}

void select()  //构建select集
{
	int i = 0;
	int p = 0;
	int g = 0;
	int flag1 = 0;
    int c;
	cout << "\n构建的SELECT集:" << endl;
	for (i = 0; i < 8; i++)
	{
		flag1 = 0;
		for (p = 0; p < T[i].length; p++)
		{
			for (g = 0; g < 5; g++)
			{
				if (T[i].array[p] == term[g])     //终结符，直接加入select
				{
					T[i].select[0] = T[i].array[p];
					flag1 = 1;
					break;
				} 
				else if (T[i].array[p] == nterm[g]) //非终结符，找到first
				{
					c = firstt(T[i].array[p]);   //找到对应的first集位置
					strcpy(T[i].select, F[c].a);
					flag1 = 1;
					break;
				}
			}
			if (flag1 == 1)
			{
				break;
			}
		}

		if (flag1 != 1)  //空，寻找follow
		{
			c = firstt(T[i].origin);
			strcpy(T[i].select, F[c].b);
		}

		cout<<i+1<<".  " << T[i].origin << "->" << T[i].array<<"\t"<<"{"<<T[i].select<<"}"<<endl;
	}
	cout << endl;
}

void tab()  //构建预测分析表
{
	int i = 0;
	int q = 0;
	int d=0;
	int u = 0;
	int w=0,y;
	char a;
	for (i = 0; i < 8; i++)
	{
		a = T[i].origin;
		for (q = 0; q <= 4; q++)
		{
			if (a == nterm[q])
			{
				for (w = 0; w <= 5; w++)
				{
					for (y = 0; y <= 5; y++)
					{
						if (T[i].select[w] == term[y])
						{
							C[q][y] = T[i];
							break;
						}

					}
				}
			}
		}
	}

}

void print_symbol() //输出符号栈
{
	int a;
	for (a = 0; a <= top + 1; a++)
		cout << symbol[a];
	cout << "\t\t";
}


void print_char()//输出剩余串
{
	int i;
	for (i = 0; i < cur_l; i++)//输出对齐符
		cout << " ";

	for (i = cur_l; i <= l_in; i++)
		cout << expr[i];
	cout << "\t\t";

}

void print_gramm(type find)
{
	cout << find.origin << "->";
		cout << find.array;
		cout << endl;
}

void analyze()  //分析过程
{
	type find;
	int m, n, k = 0;
	int j = 0;
	int flag = 0;
	int finish = 0;
	char ch, cur;  //输入字符
	cout << "请输入字符串:";
	while (1)
	{
		cin >> ch;
		if ((ch != 'i') && (ch != '+') && (ch != '*') && (ch != '(') && (ch != ')') && (ch != '#'))
		{
			cout << "输入的语法有错误！请重新输入：" << endl;
		}
		expr[j] = ch;   //存入数组expr
		j++;
		if (ch == '#')
			break;
	}

	l_in = j;     //输入串的长度
	ch = expr[0];   //当前分析的字符
	symbol[top] = '#';
	symbol[++top] = 'E';        //'#','E'进栈
	cout << "步骤\t\t符号栈\t\t 剩余字符\t\t所用产生式\n";
	do
	{
		cur = symbol[top--];     //cur为当前栈顶字符出栈
		printf_s("%d", ++k);   //步骤号 
		printf_s("\t\t");
		for (j = 0; j <= 5; j++)
		{
			if (cur == term[j])   //判断是否为终结符
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1)        //是终结符
		{
			if (cur == '#')  //如果是#，则代表分析完成
			{
				finish = 1;   //结束标记
			}
			if (cur == ch)  //如果栈顶元素等于当前分析字符，代表剩余字符已经过渡到符号栈
			{
				print_symbol();
				print_char();
				cout << endl;
				ch = expr[++cur_l];  //当前分析字符向后走一个
				flag = 0;
			}
			else  //找不到相关文法，分析出错
			{
				cout << "找不到相关分析!" << endl;
				exit(1);  //结束
			}
		}
		else if(flag==0)  //非终结符
		{
			for (j = 0; j <= 4; j++)
				if (cur == nterm[j])
				{
					m = j;//获取是哪个非终结符 
					break;
				}

			for (j = 0; j <= 5; j++)
				if (ch == term[j])
				{
					n = j;   //通过终结符获取列
					break;
				}

			find = C[m][n];   //查找预测分析表，确定对应的文法 
			if (find.origin != '\0')
			{
				print_symbol();
				print_char();
				print_gramm(find);

				for (j = (find.length - 1); j >= 0; j--)  //产生式逆序入栈
					symbol[++top] = find.array[j];
				if (symbol[top] == '^')  //为空则不进栈
					top--;
			}
			else
			{
				cout << cur << "找不到对应的文法!" << endl;
				exit(1);
			}
		}
	} while (finish == 0);
	cout << "分析完成!";
}

int main()
{
	gram();
	fw();
	select();
	tab();
	analyze();
	return 0;
}
