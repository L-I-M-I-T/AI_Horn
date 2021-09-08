#define _CRT_SECURE_NO_WARNINGS
#define SIZE 10007//hash表的长度，通过hash函数将子句映射到hah表中，采用线性探测解决冲突问题
#define cut_length 2//设置归结过程中允许产生的子句的最大长度，可以通过递增cut_length来逐步放宽归结过程以尝试是否有解
//归结过程会产生大量冗余子句，陷入盲目归结。设置允许产生的子句的最大长度可以起到剪枝的作用，消去冗余子句，减少盲目性
#include<iostream>
#include<string>
#include<time.h>
using namespace std;

int n, tail, head;
//n为初始给出的推理条件数
int f[SIZE], s[SIZE];
//f用来记录每步归结的编号较小的那个子句
//s用来记录每步归结的编号较大的那个子句
int flag[SIZE] = { 0 }, nb = 0, na = 0;
char b[10][10], a[10][10];
//a、b数组是谓词的集合，b记录了负谓词，a记录了正谓词
int pa[10] = { 0 }, pb[10] = { 0 }, aa[10][SIZE] = { 0 }, bb[10][SIZE] = { 0 };
//pa(pb)用于记录含有对应正(负)谓词的子句的数量
//aa(bb)是谓词公式"桶"，用于记录含有对应谓词公式的全部子句

class point//谓词公式类(没有考虑函数嵌套的情况如f(x,g(y,h(z)))这种形式)
{
public:
	string func;//谓词公式名称，如Father，Kill等
	int num;//谓词公式元素数量
	char vari[5][10] = { 0 };//谓词公式中的元素(变量/函数/常量)
	char type[5][10] = { 0 };//谓词公式的元素的类型(变量/函数/常量)
	char fun[5][2][3] = { 0 };//函数中的变量
	int funn[5];//函数的变量个数
	int pos;//谓词公式的正负
	point& operator=(const point& p)//重载=，谓词公式之间的赋值
	{
		func = p.func;
		num = p.num;
		pos = p.pos;
		int i, j;
		for (i = 0; i < num; i++)
		{
			strcpy(vari[i], p.vari[i]);
			strcpy(type[i], p.type[i]);
			if (strcmp(type[i], "function") == 0)
			{
				funn[i] = p.funn[i];
				for (j = 0; j < funn[i]; j++)
					strcpy(fun[i][j], p.fun[i][j]);
			}
		}
		return *this;
	}
};

class horn//horn子句结构体
{
public:
	int num;//horn子句的谓词公式数量
	point p[10];//horn子句中的谓词公式数量
	void sort()//将horn子句中的每个谓词公式的元素按字典增序排序，然后将所有谓词公式按字典序增序排列(方便比较是否有两个完全一样的子句)
	{
		int i, j;
		for (i = 0; i < num - 1; i++)
			for (j = i + 1; j < num; j++)
				if ((p[j].func > p[j + 1].func) || ((p[j].func == p[j + 1].func) && (p[j].pos > p[j + 1].pos)))
				{
					point t;
					t = p[i];
					p[i] = p[j];
					p[j] = t;
				}
	}

	horn& operator=(const horn& h)//重载=，子句之间的赋值
	{
		num = h.num;
		int i;
		for (i = 0; i < num; i++)
			p[i] = h.p[i];
		return *this;
	}
};

horn h[SIZE];
horn t[SIZE];

int operator==(const point& p1, const point& p2)//判断两个谓词公式是否完全一样
{
	int i, j;
	if (p1.num != p2.num)//元素个数要一样
		return 0;
	if (p1.func != p2.func)//公式名称要一样
		return 0;
	if (p1.pos != p2.pos)//公式正负性要一样
		return 0;
	for (i = 0; i < p1.num; i++)
	{
		if (strcmp(p1.type[i], p2.type[i]) != 0)//对应位置元素种类要一样
			return 0;
		if (strcmp(p1.vari[i], p2.vari[i]) != 0)//对应位置元素名称要一样
			return 0;
		if (strcmp(p1.type[i], "function") == 0)//如果元素是函数，函数要完全一样
		{
			if (p1.funn[i] != p2.funn[i])//函数名称要一样
				return 0;
			for (j = 0; j < p1.funn[i]; j++)
				if (strcmp(p1.fun[i][j], p2.fun[i][j]) != 0)//函数的变量要对应一样
					return 0;
		}
	}
	return 1;
}

int operator==(horn t1, horn t2)//判断两个子句是否完全一样
{
	int i;
	if (t1.num != t2.num)//要谓词公式数量和正负一样
		return 0;
	for (i = 0; i < t1.num; i++)
		if (!(t1.p[i] == t2.p[i]))//对应的谓词公式要完全一样
			return 0;
	return 1;
}

ostream& operator<<(ostream& out, const point& p)//重载<<,方便输出一个谓词公式
{
	int i, j;
	if (p.pos == -1)
		out << '~';
	out << p.func << "(";
	for (i = 0; i < p.num; i++)
	{
		if (i != 0)
			out << ",";
		out << p.vari[i];
		if (strcmp(p.type[i], "function") == 0)
		{
			out << "(";
			for (j = 0; j < p.funn[i]; j++)
			{
				if (j != 0)
					out << ",";
				out << p.fun[i][j];
			}
			out << ")";
		}
	}
	out << ")";
	return out;
}

ostream& operator<<(ostream& out, const horn& h)//重载<<,方便输出一个子句
{
	int i;
	for (i = 0; i < h.num; i++)
	{
		if (i != 0)
			out << "|";
		out << h.p[i];
	}
	return out;
}

void deal(string st, horn& h)//从输入的字符串(horn子句)中，做语法分析，从中获得每条子句的谓词公式、公式内的元素的信息(个数、类型、名称等)
{
	int now = 0, con = 0, t, i, j;
	while (now < int(st.size()))
	{
		t = now;
		while (st[now] != '|')//‘|’在这里表示析取号，以每个析取号来分割出谓词公式做进一步分析
			now++;
		now++;
		string sr = "";
		for (i = t; i < now - 1; i++)
			sr += st[i];
		string fun = "";
		for (i = 0; sr[i] != '('; i++)//获得谓词公式名称
		{
			t++;
			fun += sr[i];
		}
		t = t + 1;
		if (fun[0] == '~')//‘~’表示非，带这个符号的谓词公式为负，不带的为正
		{
			h.p[con].func = "";
			for (j = 1; j < int(fun.size()); j++)
				h.p[con].func += fun[j];
			h.p[con].pos = -1;
		}
		else
		{
			h.p[con].func = fun;
			h.p[con].pos = 1;
		}
		h.p[con].num = 0;
		sr[int(sr.size()) - 1] = ',';//从这里开始往下是提取出公式的元素
		string ss = "";
		for (i = i + 1; i < int(sr.size()); i++)
		{
			if (sr[i] == ',')
			{
				h.p[con].num++;//谓词公式的元素数量
				strcpy(h.p[con].vari[h.p[con].num - 1], ss.c_str());
				if ((ss[0] == 'x') || (ss[0] == 'u'))//x+一个数字(x1...x100)来表示变量，输入时不同子句中的变量最好用不同名称.特殊的有，u也是变量
					strcpy(h.p[con].type[h.p[con].num - 1], "variable");//元素类型为变量
				else
				{
					int k, flag = 0;
					string se = "";
					for (k = 0; k < int(ss.size()); k++)
					{
						if (ss[k] == '(')
						{
							flag = 1;
							break;
						}
						se += ss[k];
					}
					if (flag == 0)
						strcpy(h.p[con].type[h.p[con].num - 1], "constant");//元素类型为常量
					else
					{
						cout << "hahaha" << endl;
						strcpy(h.p[con].type[h.p[con].num - 1], "function");//元素类型为函数(判断依据是带括号，仅考虑一层函数，未考虑函数嵌套)
						string sr = "";
						h.p[con].funn[h.p[con].num] = 0;
						while (k++)
						{
							if ((ss[k] == ')') || (ss[k] == ','))
							{
								strcpy( h.p[con].fun[ h.p[con].num - 1 ] [h.p[con].funn[h.p[con].num - 1] ], sr.c_str());
								h.p[con].funn[h.p[con].num - 1]++;
								sr = "";
								if (ss[k] == ')')
									break;
							}
							else
								sr += ss[k];
						}
						strcpy(h.p[con].vari[h.p[con].num - 1], se.c_str());
					}
				}
				ss = "";
			}
			else
				ss = ss + sr[i];
		}
		con++;//子句的谓词公式数量
	}
	h.num = con;
}

void add(const horn& h, int x)//对新的子句，根据其所含有的谓词公式的正负性以及名称，将其加入对应的(多个)谓词公式"桶"
{
	int i, j, flag;
	for (i = 0; i < h.num; i++)
	{
		if (h.p[i].pos == -1)
		{
			flag = -1;
			for (j = 0; j < nb; j++)
				if (b[j] == h.p[i].func)
				{
					flag = j;
					break;
				}
			if (flag == -1)
			{
				strcpy(b[nb], h.p[i].func.c_str());
				flag = nb;
				pb[nb] = 1;
				bb[nb][0] = x;
				nb++;
			}
			else
			{
				bb[flag][pb[flag]] = x;
				pb[flag]++;
			}
		}
		else
		{
			flag = -1;
			for (j = 0; j < na; j++)
				if (a[j] == h.p[i].func)
				{
					flag = j;
					break;
				}
			if (flag == -1)
			{
				strcpy(a[na], h.p[i].func.c_str());
				flag = na;
				pa[na] = 1;
				aa[na][0] = x;
				na++;
			}
			else
			{
				aa[flag][pa[flag]] = x;
				pa[flag]++;
			}
		}
	}
}

int cal(const string& st)//hash函数的一部分，对于一个字符串求得对应函数值
{
	int i, s = 0;
	for (i = 0; i<int(st.size()); i++)
		if ((st[i] < '0') || (st[i] > '9'))
			s = (s + int(st[i]) * int(st[i]) * (i + 1)) % SIZE;
	return s;
}

int HASH(const horn& h)//hash函数，用于将子句通过hash函数映射到hash表，hash函数根据子句的各个字符串计算函数乘以字符串位置加权之和来计算
{
	int i, j, k, s = 0;
	for (i = 0; i < h.num; i++)
	{
		if (h.p[i].pos == -1)
			s = abs(s - cal(h.p[i].func) * i) % SIZE;
		else
			s = (s + cal(h.p[i].func) * i) % SIZE;
		for (j = 0; j < h.p[i].num; j++)
		{
			s = (s + cal(h.p[i].vari[j]) * (j + 1)) % SIZE;
			if (strcmp(h.p[i].type[j], "function") == 0)
			{
				for (k = 0; k < h.p[i].funn[j]; k++)
					s = (s + cal(h.p[i].fun[j][k]) * (k + 1)) % SIZE;
			}
		}
	}
	if (flag[s] != 0)//发生冲突，使用线性探测法解决冲突
	{
		if (t[s] == h)
			return -1;
		int con = 0, temp;
		while (1)
		{
			con++;
			temp = (s + con) % SIZE;
			if (flag[temp] == 0)
			{
				flag[temp] = 1;
				t[temp] = h;
				break;
			}
			else
				if (t[temp] == h)//该子句已经出现过，不再重复添加
					return -1;
		}
	}
	else
	{
		flag[s] = 1;
		t[s] = h;
	}

	return 0;
}

void change(horn& t, string s1, string s2, const point& tt, int x)//进行置换,此处没有考虑函数嵌套的情况如f(x,g(y,h(z)))这种形式
{
	//置换将会影响到一条子句的所有谓词公式
	int i, j, k;
	for (i = 0; i < t.num; i++)
		for (j = 0; j < t.p[i].num; j++)
		{
			if (t.p[i].vari[j] == s1)
			{
				strcpy(t.p[i].vari[j], s2.c_str());
				strcpy(t.p[i].type[j], tt.type[x]);
				if (tt.type[x] == "function")
				{
					t.p[i].funn[j] = tt.funn[x];
					for (k = 0; k < tt.funn[x]; k++)
						strcpy(t.p[i].fun[j][k], tt.fun[x][k]);
				}
			}
			else
				if (t.p[i].type[j] == "function")
				{
					for (k = 0; k < t.p[i].funn[j]; k++)
						if (t.p[i].fun[j][k] == s1)
							strcpy(t.p[i].fun[j][k], s2.c_str());
				}
		}
}

int merge(int p1, int s1, int p2, int s2)//归结编号为p1和s1的子句，消解p1的编号为p2的谓词公式和s1的编号为s2的谓词公式
{
	int i, j;
	static horn t1, t2;
	t1 = h[p1];
	t2 = h[s1];
	if (t1.p[p2].num != t2.p[s2].num)
		return -1;

	for (i = 0; i < t1.p[p2].num; i++)
	{
		int flag = 0;
		if (strcmp(t1.p[p2].vari[i], t2.p[s2].vari[i]) != 0)
		{
			if (strcmp(t1.p[p2].type[i], "variable") == 0)
			{
				if (strcmp(t2.p[s2].type[i], "function") == 0)
				{
					for (j = 0; j < t2.p[s2].funn[i]; j++)
						if (strcmp(t2.p[s2].fun[i][j], t1.p[p2].vari[i]) == 0)
							return -1;
				}
				change(t1, t1.p[p2].vari[i], t2.p[s2].vari[i], t2.p[s2], i);//做置换，影响整个子句
				change(t2, t1.p[p2].vari[i], t2.p[s2].vari[i], t2.p[s2], i);
			}
			else
				if (strcmp(t2.p[s2].type[i], "variable") == 0)
				{
					if (strcmp(t1.p[p2].type[i], "function") == 0)
					{
						for (j = 0; j < t1.p[p2].funn[i]; j++)
							if (strcmp(t1.p[p2].fun[i][j], t2.p[s2].vari[i]) == 0)
								return -1;
					}
					change(t1, t2.p[s2].vari[i], t1.p[p2].vari[i], t1.p[p2], i);
					change(t2, t2.p[s2].vari[i], t1.p[p2].vari[i], t1.p[p2], i);
				}
				else
					return -1;
		}
	}
	//这一段是产生归结后的新子句
	horn t3;
	t3.num = 0;
	int flag;
	for (i = 0; i < t1.num; i++)
		if (i != p2)			//这次用来消解的
		{
			flag = 1;
			for (j = 0; j < t3.num; j++)
				if (t3.p[j] == t1.p[i])//完全相同的公式在子句中出现多次，只保留一次
				{
					flag = 0;
					break;
				}
			if (!flag)
				continue;
			t3.p[t3.num] = t1.p[i];
			t3.num++;
		}
	for (i = 0; i < t2.num; i++)
		if (i != s2)//这次用来消解的
		{
			flag = 1;
			for (j = 0; j < t3.num; j++)
				if (t3.p[j] == t2.p[i])//完全相同的公式在子句中出现多次，只保留一次
				{
					flag = 0;
					break;
				}
			if (!flag)
				continue;
			t3.p[t3.num] = t2.p[i];
			t3.num++;
		}
	if (t3.num > cut_length)//超出允许的最大谓词公式个数，可能是冗余子句，将其剪去。
		return -1;
	t3.sort();
	if (HASH(t3))//已经出现过的子句不再重复添加
		return -1;
	h[tail] = t3;
	add(h[tail], tail);//将新子句加入谓词公式“桶”
	tail++;
	//cout << "(" << p1 << ")" << h[p1] << "和(" << s1 << ")" << h[s1] << "归结得到(" << tail - 1 << ")" << h[tail - 1] << endl;//反映整个归结过程，可以注释掉只看有效的步骤
	return 0;
}

void result(int x)//逆向打印出解，根据编号为x的子句是由哪两个编号的子句归结而成的，不停向上溯源，如果是由输入的条件归结而成的就直接输出
{
	static int step = 0;
	if (f[x] > n)
		result(f[x]);
	if (s[x] > n)
		result(s[x]);
	cout << "第" << ++step << "步:" << endl;//打印第step步归结的操作
	cout << "(" << f[x] << ")" << h[f[x]] << "和(" << s[x] << ")" << h[s[x]] << "归结得到(" << x << ")" << h[x] << endl << endl;
}

int main()
{
	(void)freopen("test.txt", "r", stdin);

	int i, j, k;
	cout << "请输入要归结的子句个数：" << endl;
	cin >> n;
	cout << "请输入要归结的" << n << "个子句(各个变量用xn表示)" << endl;
	for (i = 0; i < n; i++)
	{
		string st;
		cin >> st;
		st = st + '|';
		h[i].num = 0;
		deal(st, h[i]);//提取输入的子句信息
		h[i].sort();
		add(h[i], i);
	}
	cout << "请输入要求解的问题(u代表假设变量)：" << endl;
	string st, temp;
	cin >> st;
	int start = clock();
	temp = "~";
	temp += st;
	st = temp;
	st += "|";
	st += "ANSWER(u)|";	//将问题的否定和答案做析取，加入到子句集中
	h[n].num = 0;
	deal(st, h[n]);		//处理新加入的horn子句
	h[n].sort();
	add(h[n], n);
	tail = n + 1;
	head = 0;

	//使用前向连接对子句集进行归结
	while (head < tail)
	{
		for (i = 0; i < h[head].num; i++)
		{
			if (h[head].num > cut_length)						//超出允许的最大谓词公式长度，视为冗余剪去
				break;
			if (h[head].p[i].pos == 1)							//正/负谓词公式分类寻找对应的谓词公式"桶"
			{
				int flag = 0;
				for (j = 0; j < nb; j++)
					if (h[head].p[i].func == b[j])				//寻找相应的负谓词公式“桶”
					{
						flag = j;
						break;
					}

				for (j = 0; j < pb[flag]; j++)					//只在对应的谓词公式"桶"内做归结，桶外没有可归结的谓词公式
				{
					if (bb[flag][j] >= head)					//每个子句只尝试和在它之前的子句归结，避免重复计算
						break;
					for (k = 0; k < h[bb[flag][j]].num; k++)
					{
						if ((h[head].p[i].func == h[bb[flag][j]].p[k].func) && (h[head].p[i].pos + h[bb[flag][j]].p[k].pos == 0) && ( merge(bb[flag][j], head, k, i) == 0))
						{
							f[tail - 1] = bb[flag][j];			//记录归结的子句中编号较小的一个
							s[tail - 1] = head;					//记录归结的子句中编号较大的一个
							if ((h[tail - 1].num == 1) && (h[tail - 1].p[0].func == "ANSWER") && (strcmp(h[tail - 1].p[0].vari[0], "u") != 0))//已经获得了答案，则可以打印归结过程之后退出
							{
								cout << "归结过程如下：" << endl << endl;
								result(tail - 1);				//逆向打印出解
								cout << "归结成功！" << endl;
								int end = clock();
								cout << "共计用时：" << (end - start)  << "ms" << endl;
								exit(0);
							}
						}
					}
				}

			}
			else
			{
				int flag = 0;
				for (j = 0; j < na; j++)
					if (h[head].p[i].func == a[j])//寻找相应的正谓词公式“桶”
					{
						flag = j;
						break;
					}

				for (j = 0; j < pa[flag]; j++)//只在对应的谓词公式"桶"内做归结，桶外没有可归结的谓词公式
				{
					if (aa[flag][j] >= head)//每个子句只尝试和在它之前的子句归结，避免重复计算
						break;
					for (k = 0; k < h[aa[flag][j]].num; k++)
					{
						if ((h[head].p[i].func == h[aa[flag][j]].p[k].func) && (h[head].p[i].pos + h[aa[flag][j]].p[k].pos == 0) && (merge(aa[flag][j], head, k, i) == 0))
						{
							f[tail - 1] = aa[flag][j];//记录归结的子句中编号较小的一个
							s[tail - 1] = head;//记录归结的子句中编号较大的一个
							if ((h[tail - 1].num == 1) && (h[tail - 1].p[0].func == "ANSWER") && (strcmp(h[tail - 1].p[0].vari[0], "u") != 0))//已经获得了答案，则可以打印归结过程之后退出
							{
								cout << "归结过程如下：" << endl << endl;
								result(tail - 1);//逆向打印出解
								cout << "归结成功！" << endl;
								int end = clock();
								cout << "共计用时：" << (end - start) << "ms" << endl;
								exit(0);
							}
						}
					}
				}
			}
		}
		head++;
	}
	return 0;
}