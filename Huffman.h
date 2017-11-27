#include"node.h"
#include<stdio.h> 
#include<cstdlib>
#include<fstream>
#include<iostream>
#define N 26                //叶子总数,即需编码字符的个数  
#define M 2*N-1             //节点总数  
#define maxval 10000        //最大权值  
#define maxsize 1000        //数组大小的最大值  
#define maxlongth 1000 
using namespace std;



class Huffman
{
public:
	char temp[maxlongth];           //暂存读入字符串
	char binarycode[maxlongth];     //存放编码好的二进制串
	char decode_ch[maxlongth];      //存放翻译好的字符串
	char temp_ch[M];        //标记读入huffman文件的字符
	int temp_wight[M];      //统计读入huffman文件的字符对应权值
	HuffmanTree tree[M];    //huffman树
	CodeType code[N];       //huffman编码表
public:
	//  Huffman();
	// ~Huffman();
	void initialize(char temp[], char temp_ch[], int temp_wight[]);       //初始化temp_ch[N] temp_wight[N];统计huffman树文件的各字符权重
	void HFfile_read(char temp[]);                            //从huffman文件中读取数据,存入temp[]
	void infile_read(char temp[]);                            //从源文件中读取数据,存入temp[]

	void CreateHuffmanTree(HuffmanTree tree[], char temp_ch[], int temp_wight[]);     //创建Huffman树  
	void HuffmanCode(CodeType code[], HuffmanTree tree[]);          //根据Huffman+1树求出Huffman编码存储在code数组中  
	void incode(CodeType code[], char temp[], char binarycode[]);   //将字符串temp编码，变成“01”串保存在数组binarycode中  
	void decode(HuffmanTree tree[], char temp[], char decode_ch[]); //将“01”字符串str进行译码，直接输出  
};

void Huffman::HFfile_read(char temp[])   //读取huffman文件到缓冲区
{
	HFfile.seekg(0, ios::beg);
	HFfile.get(temp, 1000);   //读入当前行内容
}

void Huffman::infile_read(char temp[])   //读取源文件到缓冲区
{
	infile.seekg(0, ios::beg);
	infile.get(temp, 1000);   //读入当前行内容
}

void Huffman::initialize(char temp[], char temp_ch[], int temp_wight[])  //将huffman树M个结点初始化,weight全部置0
{
	for (int i = 0; i < N;i++)   //c[0~n-1]，ch='a'~'z'
	{
		temp_ch[i] = i + 'a';
		temp_wight[i] = 0;
		cout << temp_ch[i] << temp_wight[i] << endl;
	}
	for (int i = N; i < M;i++)   //c[n~m-1]，ch='0'
	{
		temp_ch[i] = '0';
		temp_wight[i] = 0;
		cout << temp_ch[i] << temp_wight[i] << endl;
	}
	for (int i = 0; temp[i] != '\0';i++)
	{
		int p = temp[i] - 'a';
		temp_wight[p]+=1;          //对应权值加1
	}
}

void Huffman::CreateHuffmanTree(HuffmanTree tree[], char temp_ch[], int temp_wight[])
{
	int i, j;
	int p1, p2;          //p1,p2记录最小权值及次小权值节点在数组中的下标  
	int min1, min2;    //min1记录最小权值，min2记录次小权值  

	for (i = 0;i<M;i++)        //初始化Huffman树的M个节点  
	{
		tree[i].Lchild = -1;
		tree[i].Rchild = -1;
		tree[i].parent = -1;
	}

	//输入Huffman树前N个节点的信息，即待编码的字符及其权值  
	for (i = 0;i<M;i++)
	{
		tree[i].ch = temp_ch[i];
		tree[i].weight = temp_wight[i];
	}

	//进行N-1次合并，生成N-1个新节点  
	//每次找到权值最小的两个单个节点（即无父节点的节点）+，合并形成新节点，更改这两个节点的父节点信息、新节点的权值及左右孩子节点信息  
	for (i = N;i<M;i++)
	{
		p1 = p2 = 0;          //最小权值节点及次小权值节点对应下标初始化为0  
		min1 = min2 = maxval; //最小权值及次小权值初始化为权值最大值  
		for (j = 0;j<i;j++)   //依次检测Huffman树的前i个节点  
		{
			if (tree[j].parent == -1)     //若该节点无父节点  
			{
				if (tree[j].weight<min1)  //若该节点的权值小于最小权值，  
				{                         //将最小权值赋给次小权值，该节点的权值赋给最小权值作为最小权值  
					min2 = min1;          //并更改对应p1,p2的值,使之指向对应节点的下标  
					min1 = tree[j].weight;
					p2 = p1;
					p1 = j;
				}
				else
				{
					if (tree[j].weight<min2) //若该节点的权值大于最小权值，小于次小权值，  
					{                        //将该节点的权值赋给次小权值，该节点的下标赋给p2  
						min2 = tree[j].weight;
						p2 = j;
					}
				}
			}
		}

		tree[p1].parent = i; //更改权值最小两个节点的父节点信息  
		tree[p2].parent = i;
		tree[i].Lchild = p1; //更改父节点左右孩子信息及权值  
		tree[i].Rchild = p2;
		tree[i].weight = tree[p1].weight + tree[p2].weight;
	}
}

void Huffman::HuffmanCode(CodeType code[], HuffmanTree tree[]) //根据Huffman树求出Huffman编码存储在code数组中  
{
	int i, c, p;
	CodeType cd;//缓冲变量  
	for (i = 0;i<N;i++)//依次检测前N个节点，前N个节点为叶子节点，即从Huffman从下往上获得单个字符的编码  
	{
		cd.start = N;
		cd.ch = tree[i].ch;
		c = i;              //c为当前节点  
		p = tree[i].parent; //p为当前  
		while (p != -1)
		{
			cd.start--;
			if (tree[p].Lchild == c)
				cd.bits[cd.start] = '0';//tree[i]是左子树，生成代码'0'  
			else
				cd.bits[cd.start] = '1';//tree[i]是右子树，生成代码'1'  
			c = p;
			p = tree[p].parent;
		}
		code[i] = cd;//第i+1个字符的编码存入code[i]  
	}
}

void Huffman::incode(CodeType code[], char temp[], char binarycode[])//编码  
{
	int i, k = 0;
	for (i = 0;temp[i] != '\0';i++)
	{
		int j = 0, p;
		while (code[j].ch != temp[i])
			j++;
		for (p = code[j].start;p<N;p++)
			binarycode[k++] = code[j].bits[p];
	}
	binarycode[k] = '\0';//注意！  
}

void Huffman::decode(HuffmanTree tree[], char temp[], char decode_ch[])//译码  
{
	int cur = 0;
	int j = 0, i = M - 1;//tree[M-1]为根节点，从根节点开始译码  
	while (temp[j] != '\0')
	{
		if (temp[j] == '0')
			i = tree[i].Lchild;//走向左孩子  
		else if (temp[j] == '1')
			i = tree[i].Rchild;//走向右孩子  
		if (tree[i].Rchild == -1)//tree[i]是叶子节点  
		{
			decode_ch[cur]=tree[i].ch;
			cur++;
			i = M - 1;//回到根节点 
		}
		j++;
	}
	if (tree[i].Lchild != -1 && temp[j] != '\0')//字符串读完，但未到叶子节点，则输入01码有错  
		printf("ERROR!");
	decode_ch[cur] = '\0';
}
