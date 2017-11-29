#include <stdlib.h>
#include <iostream>
#include"readname.h"
#include"Huffman.h"
int command()
{
	int command = 0;
	cin >> command;
	if (command == 1) //编码
	{
		readname(); //打开文件

		Huffman HF;
		HF.HFfile_read(HF.temp, HF.text);                    //读入huffman文件存入temp[]
		//cout << "1" << endl;
		HF.initialize(HF.text, HF.text_ch, HF.text_wight);   //初始化
		//cout << "2" << endl;
		HF.CreateHuffmanTree(HF.tree, HF.text_ch, HF.text_wight);   //创建huffman树tree[]
		//cout << "3" << endl;
		HF.HuffmanCode(HF.code, HF.tree);           //由huffman树tree[]得编码code[]
		//cout << "4" << endl;
		HF.infile_read(HF.temp, HF.text);                    //读入源文件文件（待编码的字符串）存入temp[]
		//cout << "5" << endl;
		HF.incode(HF.code, HF.text, HF.binarycode); //根据翻译表code[]，将待编码得temp[]编译成二进制存入binarycode[]
		//cout << "6" << endl;
		outfile.write(HF.binarycode, strlen(HF.binarycode));   //将binarycode[]输出到outoutfile
		//cout << "7" << endl;

		close_file(); //关闭文件
	}
	else if (command == 2) //译码
	{
		readname(); //打开文件

		Huffman HF;
		HF.HFfile_read(HF.temp,HF.text);                     //读入huffman文件存入text[]
		HF.initialize(HF.text, HF.text_ch, HF.text_wight);   //初始化
		HF.CreateHuffmanTree(HF.tree, HF.text_ch, HF.text_wight);   //创建huffman树tree[]
		HF.infile_read(HF.temp, HF.text);                    //读入源文件文件（待翻译得01串）存入temp[]
		HF.decode(HF.tree, HF.text, HF.decode_ch);  //根据huffman树，将待翻译的temp[]翻译成字符串存入decode_ch[]
		cout << HF.decode_ch << endl;
		outfile.write(HF.decode_ch, strlen(HF.decode_ch));   //将decode_ch[]输出到outoutfile

		close_file(); //关闭文件
	}
	else if (command == 3) //退出
	{
		exit(0);
	}
	else //输入错误指令
	{
		cout << "error command,please re-enter";
		return 1;
	}
	return 0;
}
