#define _CRT_SECURE_NO_WARNINGS
#define SIZE 10007//hash��ĳ��ȣ�ͨ��hash�������Ӿ�ӳ�䵽hah���У���������̽������ͻ����
#define cut_length 2//���ù�����������������Ӿ����󳤶ȣ�����ͨ������cut_length���𲽷ſ�������Գ����Ƿ��н�
//�����̻�������������Ӿ䣬����äĿ��ᡣ��������������Ӿ����󳤶ȿ����𵽼�֦�����ã���ȥ�����Ӿ䣬����äĿ��
#include<iostream>
#include<string>
#include<time.h>
using namespace std;

int n, tail, head;
//nΪ��ʼ����������������
int f[SIZE], s[SIZE];
//f������¼ÿ�����ı�Ž�С���Ǹ��Ӿ�
//s������¼ÿ�����ı�Žϴ���Ǹ��Ӿ�
int flag[SIZE] = { 0 }, nb = 0, na = 0;
char b[10][10], a[10][10];
//a��b������ν�ʵļ��ϣ�b��¼�˸�ν�ʣ�a��¼����ν��
int pa[10] = { 0 }, pb[10] = { 0 }, aa[10][SIZE] = { 0 }, bb[10][SIZE] = { 0 };
//pa(pb)���ڼ�¼���ж�Ӧ��(��)ν�ʵ��Ӿ������
//aa(bb)��ν�ʹ�ʽ"Ͱ"�����ڼ�¼���ж�Ӧν�ʹ�ʽ��ȫ���Ӿ�

class point//ν�ʹ�ʽ��(û�п��Ǻ���Ƕ�׵������f(x,g(y,h(z)))������ʽ)
{
public:
	string func;//ν�ʹ�ʽ���ƣ���Father��Kill��
	int num;//ν�ʹ�ʽԪ������
	char vari[5][10] = { 0 };//ν�ʹ�ʽ�е�Ԫ��(����/����/����)
	char type[5][10] = { 0 };//ν�ʹ�ʽ��Ԫ�ص�����(����/����/����)
	char fun[5][2][3] = { 0 };//�����еı���
	int funn[5];//�����ı�������
	int pos;//ν�ʹ�ʽ������
	point& operator=(const point& p)//����=��ν�ʹ�ʽ֮��ĸ�ֵ
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

class horn//horn�Ӿ�ṹ��
{
public:
	int num;//horn�Ӿ��ν�ʹ�ʽ����
	point p[10];//horn�Ӿ��е�ν�ʹ�ʽ����
	void sort()//��horn�Ӿ��е�ÿ��ν�ʹ�ʽ��Ԫ�ذ��ֵ���������Ȼ������ν�ʹ�ʽ���ֵ�����������(����Ƚ��Ƿ���������ȫһ�����Ӿ�)
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

	horn& operator=(const horn& h)//����=���Ӿ�֮��ĸ�ֵ
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

int operator==(const point& p1, const point& p2)//�ж�����ν�ʹ�ʽ�Ƿ���ȫһ��
{
	int i, j;
	if (p1.num != p2.num)//Ԫ�ظ���Ҫһ��
		return 0;
	if (p1.func != p2.func)//��ʽ����Ҫһ��
		return 0;
	if (p1.pos != p2.pos)//��ʽ������Ҫһ��
		return 0;
	for (i = 0; i < p1.num; i++)
	{
		if (strcmp(p1.type[i], p2.type[i]) != 0)//��Ӧλ��Ԫ������Ҫһ��
			return 0;
		if (strcmp(p1.vari[i], p2.vari[i]) != 0)//��Ӧλ��Ԫ������Ҫһ��
			return 0;
		if (strcmp(p1.type[i], "function") == 0)//���Ԫ���Ǻ���������Ҫ��ȫһ��
		{
			if (p1.funn[i] != p2.funn[i])//��������Ҫһ��
				return 0;
			for (j = 0; j < p1.funn[i]; j++)
				if (strcmp(p1.fun[i][j], p2.fun[i][j]) != 0)//�����ı���Ҫ��Ӧһ��
					return 0;
		}
	}
	return 1;
}

int operator==(horn t1, horn t2)//�ж������Ӿ��Ƿ���ȫһ��
{
	int i;
	if (t1.num != t2.num)//Ҫν�ʹ�ʽ����������һ��
		return 0;
	for (i = 0; i < t1.num; i++)
		if (!(t1.p[i] == t2.p[i]))//��Ӧ��ν�ʹ�ʽҪ��ȫһ��
			return 0;
	return 1;
}

ostream& operator<<(ostream& out, const point& p)//����<<,�������һ��ν�ʹ�ʽ
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

ostream& operator<<(ostream& out, const horn& h)//����<<,�������һ���Ӿ�
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

void deal(string st, horn& h)//��������ַ���(horn�Ӿ�)�У����﷨���������л��ÿ���Ӿ��ν�ʹ�ʽ����ʽ�ڵ�Ԫ�ص���Ϣ(���������͡����Ƶ�)
{
	int now = 0, con = 0, t, i, j;
	while (now < int(st.size()))
	{
		t = now;
		while (st[now] != '|')//��|���������ʾ��ȡ�ţ���ÿ����ȡ�����ָ��ν�ʹ�ʽ����һ������
			now++;
		now++;
		string sr = "";
		for (i = t; i < now - 1; i++)
			sr += st[i];
		string fun = "";
		for (i = 0; sr[i] != '('; i++)//���ν�ʹ�ʽ����
		{
			t++;
			fun += sr[i];
		}
		t = t + 1;
		if (fun[0] == '~')//��~����ʾ�ǣ���������ŵ�ν�ʹ�ʽΪ����������Ϊ��
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
		sr[int(sr.size()) - 1] = ',';//�����￪ʼ��������ȡ����ʽ��Ԫ��
		string ss = "";
		for (i = i + 1; i < int(sr.size()); i++)
		{
			if (sr[i] == ',')
			{
				h.p[con].num++;//ν�ʹ�ʽ��Ԫ������
				strcpy(h.p[con].vari[h.p[con].num - 1], ss.c_str());
				if ((ss[0] == 'x') || (ss[0] == 'u'))//x+һ������(x1...x100)����ʾ����������ʱ��ͬ�Ӿ��еı�������ò�ͬ����.������У�uҲ�Ǳ���
					strcpy(h.p[con].type[h.p[con].num - 1], "variable");//Ԫ������Ϊ����
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
						strcpy(h.p[con].type[h.p[con].num - 1], "constant");//Ԫ������Ϊ����
					else
					{
						cout << "hahaha" << endl;
						strcpy(h.p[con].type[h.p[con].num - 1], "function");//Ԫ������Ϊ����(�ж������Ǵ����ţ�������һ�㺯����δ���Ǻ���Ƕ��)
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
		con++;//�Ӿ��ν�ʹ�ʽ����
	}
	h.num = con;
}

void add(const horn& h, int x)//���µ��Ӿ䣬�����������е�ν�ʹ�ʽ���������Լ����ƣ���������Ӧ��(���)ν�ʹ�ʽ"Ͱ"
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

int cal(const string& st)//hash������һ���֣�����һ���ַ�����ö�Ӧ����ֵ
{
	int i, s = 0;
	for (i = 0; i<int(st.size()); i++)
		if ((st[i] < '0') || (st[i] > '9'))
			s = (s + int(st[i]) * int(st[i]) * (i + 1)) % SIZE;
	return s;
}

int HASH(const horn& h)//hash���������ڽ��Ӿ�ͨ��hash����ӳ�䵽hash��hash���������Ӿ�ĸ����ַ������㺯�������ַ���λ�ü�Ȩ֮��������
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
	if (flag[s] != 0)//������ͻ��ʹ������̽�ⷨ�����ͻ
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
				if (t[temp] == h)//���Ӿ��Ѿ����ֹ��������ظ����
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

void change(horn& t, string s1, string s2, const point& tt, int x)//�����û�,�˴�û�п��Ǻ���Ƕ�׵������f(x,g(y,h(z)))������ʽ
{
	//�û�����Ӱ�쵽һ���Ӿ������ν�ʹ�ʽ
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

int merge(int p1, int s1, int p2, int s2)//�����Ϊp1��s1���Ӿ䣬����p1�ı��Ϊp2��ν�ʹ�ʽ��s1�ı��Ϊs2��ν�ʹ�ʽ
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
				change(t1, t1.p[p2].vari[i], t2.p[s2].vari[i], t2.p[s2], i);//���û���Ӱ�������Ӿ�
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
	//��һ���ǲ�����������Ӿ�
	horn t3;
	t3.num = 0;
	int flag;
	for (i = 0; i < t1.num; i++)
		if (i != p2)			//������������
		{
			flag = 1;
			for (j = 0; j < t3.num; j++)
				if (t3.p[j] == t1.p[i])//��ȫ��ͬ�Ĺ�ʽ���Ӿ��г��ֶ�Σ�ֻ����һ��
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
		if (i != s2)//������������
		{
			flag = 1;
			for (j = 0; j < t3.num; j++)
				if (t3.p[j] == t2.p[i])//��ȫ��ͬ�Ĺ�ʽ���Ӿ��г��ֶ�Σ�ֻ����һ��
				{
					flag = 0;
					break;
				}
			if (!flag)
				continue;
			t3.p[t3.num] = t2.p[i];
			t3.num++;
		}
	if (t3.num > cut_length)//������������ν�ʹ�ʽ�����������������Ӿ䣬�����ȥ��
		return -1;
	t3.sort();
	if (HASH(t3))//�Ѿ����ֹ����Ӿ䲻���ظ����
		return -1;
	h[tail] = t3;
	add(h[tail], tail);//�����Ӿ����ν�ʹ�ʽ��Ͱ��
	tail++;
	//cout << "(" << p1 << ")" << h[p1] << "��(" << s1 << ")" << h[s1] << "���õ�(" << tail - 1 << ")" << h[tail - 1] << endl;//��ӳ���������̣�����ע�͵�ֻ����Ч�Ĳ���
	return 0;
}

void result(int x)//�����ӡ���⣬���ݱ��Ϊx���Ӿ�������������ŵ��Ӿ�����ɵģ���ͣ������Դ�����������������������ɵľ�ֱ�����
{
	static int step = 0;
	if (f[x] > n)
		result(f[x]);
	if (s[x] > n)
		result(s[x]);
	cout << "��" << ++step << "��:" << endl;//��ӡ��step�����Ĳ���
	cout << "(" << f[x] << ")" << h[f[x]] << "��(" << s[x] << ")" << h[s[x]] << "���õ�(" << x << ")" << h[x] << endl << endl;
}

int main()
{
	(void)freopen("test.txt", "r", stdin);

	int i, j, k;
	cout << "������Ҫ�����Ӿ������" << endl;
	cin >> n;
	cout << "������Ҫ����" << n << "���Ӿ�(����������xn��ʾ)" << endl;
	for (i = 0; i < n; i++)
	{
		string st;
		cin >> st;
		st = st + '|';
		h[i].num = 0;
		deal(st, h[i]);//��ȡ������Ӿ���Ϣ
		h[i].sort();
		add(h[i], i);
	}
	cout << "������Ҫ��������(u����������)��" << endl;
	string st, temp;
	cin >> st;
	int start = clock();
	temp = "~";
	temp += st;
	st = temp;
	st += "|";
	st += "ANSWER(u)|";	//������ķ񶨺ʹ�����ȡ�����뵽�Ӿ伯��
	h[n].num = 0;
	deal(st, h[n]);		//�����¼����horn�Ӿ�
	h[n].sort();
	add(h[n], n);
	tail = n + 1;
	head = 0;

	//ʹ��ǰ�����Ӷ��Ӿ伯���й��
	while (head < tail)
	{
		for (i = 0; i < h[head].num; i++)
		{
			if (h[head].num > cut_length)						//������������ν�ʹ�ʽ���ȣ���Ϊ�����ȥ
				break;
			if (h[head].p[i].pos == 1)							//��/��ν�ʹ�ʽ����Ѱ�Ҷ�Ӧ��ν�ʹ�ʽ"Ͱ"
			{
				int flag = 0;
				for (j = 0; j < nb; j++)
					if (h[head].p[i].func == b[j])				//Ѱ����Ӧ�ĸ�ν�ʹ�ʽ��Ͱ��
					{
						flag = j;
						break;
					}

				for (j = 0; j < pb[flag]; j++)					//ֻ�ڶ�Ӧ��ν�ʹ�ʽ"Ͱ"������ᣬͰ��û�пɹ���ν�ʹ�ʽ
				{
					if (bb[flag][j] >= head)					//ÿ���Ӿ�ֻ���Ժ�����֮ǰ���Ӿ��ᣬ�����ظ�����
						break;
					for (k = 0; k < h[bb[flag][j]].num; k++)
					{
						if ((h[head].p[i].func == h[bb[flag][j]].p[k].func) && (h[head].p[i].pos + h[bb[flag][j]].p[k].pos == 0) && ( merge(bb[flag][j], head, k, i) == 0))
						{
							f[tail - 1] = bb[flag][j];			//��¼�����Ӿ��б�Ž�С��һ��
							s[tail - 1] = head;					//��¼�����Ӿ��б�Žϴ��һ��
							if ((h[tail - 1].num == 1) && (h[tail - 1].p[0].func == "ANSWER") && (strcmp(h[tail - 1].p[0].vari[0], "u") != 0))//�Ѿ�����˴𰸣�����Դ�ӡ������֮���˳�
							{
								cout << "���������£�" << endl << endl;
								result(tail - 1);				//�����ӡ����
								cout << "���ɹ���" << endl;
								int end = clock();
								cout << "������ʱ��" << (end - start)  << "ms" << endl;
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
					if (h[head].p[i].func == a[j])//Ѱ����Ӧ����ν�ʹ�ʽ��Ͱ��
					{
						flag = j;
						break;
					}

				for (j = 0; j < pa[flag]; j++)//ֻ�ڶ�Ӧ��ν�ʹ�ʽ"Ͱ"������ᣬͰ��û�пɹ���ν�ʹ�ʽ
				{
					if (aa[flag][j] >= head)//ÿ���Ӿ�ֻ���Ժ�����֮ǰ���Ӿ��ᣬ�����ظ�����
						break;
					for (k = 0; k < h[aa[flag][j]].num; k++)
					{
						if ((h[head].p[i].func == h[aa[flag][j]].p[k].func) && (h[head].p[i].pos + h[aa[flag][j]].p[k].pos == 0) && (merge(aa[flag][j], head, k, i) == 0))
						{
							f[tail - 1] = aa[flag][j];//��¼�����Ӿ��б�Ž�С��һ��
							s[tail - 1] = head;//��¼�����Ӿ��б�Žϴ��һ��
							if ((h[tail - 1].num == 1) && (h[tail - 1].p[0].func == "ANSWER") && (strcmp(h[tail - 1].p[0].vari[0], "u") != 0))//�Ѿ�����˴𰸣�����Դ�ӡ������֮���˳�
							{
								cout << "���������£�" << endl << endl;
								result(tail - 1);//�����ӡ����
								cout << "���ɹ���" << endl;
								int end = clock();
								cout << "������ʱ��" << (end - start) << "ms" << endl;
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