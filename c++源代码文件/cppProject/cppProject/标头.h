#pragma once
#pragma once
#define UNICODE
#define _UNICODE
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <stack>
#include <sstream>
#include <conio.h>
using namespace std;


const int k_w_len = 3;
const int t_w_len = 5;
FILE * fsrc;
FILE * fout;
char c;
int lexical_error = 0;
string error_message = "";
string buffer = "";
string tmp = "";
int index = 0;
int flag = 1;
int sen_start = 0;
int sen_end = 0;
int sen_error = 0;
int sen_error_line = 0;
int sen_layer = 0;
int sen_logic_sig = 0;
int temp_table_size = 0;
int is_while = 0;
int layer = 0;


enum Tag
{
	NUM, FLOAT, ID, TYPE, KEY, SYM, COMP, BOSYM
};
enum FPType
{
	ASSIGN, BOOL, NOT, JUMP, JUMPZ, JUMPNZ, ADD, SUB, PLUS, ABST
};

string keywords[k_w_len] = { "if","else","while" };
string typewords[t_w_len] = { "int","float","char","const","void" };
string symbols = ",;+-*/(){}!=><&|";
vector<string> idwords;//词法分析中存放变量名

					   //词法分析Token类
class Token
{
private:
	int tag;
public:
	Token() {}
	Token(int i, int l)
	{
		tag = i;
		line = l;
	}
	int get_tag()
	{
		return tag;
	}
	int line;
	virtual int get_numvalue() { return -1; }
	virtual double get_floatvalue() { return -1; }
	virtual int get_lexeme() { return -1; }
	virtual string get_lexeme_str() { return "error"; }
	virtual char get_symbol() { return ' '; }
	virtual string get_compsym() { return " !!!!!"; }
	virtual string get_boolsym() { return "!!"; }
};
class Num : public Token
{
private:
	int value;
public:
	Num(int t, int v, int l) :Token(t, l), value(v) {};
	int get_numvalue()
	{
		return value;
	}
};
class Float : public Token
{
private:
	double value;
public:
	Float(int t, double v, int l) :Token(t, l), value(v) {};
	double get_floatvalue()
	{
		return value;
	}
};
class Word : public Token
{
private:
	int lexeme;
public:
	Word(int t, int v, int l) :Token(t, l), lexeme(v) {};
	int get_lexeme()
	{
		return lexeme;
	}
	string get_lexeme_str()
	{
		if ((this->get_tag()) == ID) return idwords[lexeme];
		else if ((this->get_tag()) == TYPE) return typewords[lexeme];
		else return keywords[lexeme];
	}
};
class Symbol : public Token
{
private:
	char symbol;
public:
	Symbol(int t, char c, int l) :Token(t, l), symbol(c) {};
	char get_symbol()
	{
		return symbol;
	}
};
class CompSym : public Token
{
private:
	string compsym;
public:
	CompSym(int t, string cs, int l) :Token(t, l), compsym(cs) {};
	string get_compsym()
	{
		return compsym;
	}
};
class BoolSym : public Token
{
private:
	string boolsym;
public:
	BoolSym(int t, string cs, int l) :Token(t, l), boolsym(cs) {};
	string get_boolsym()
	{
		return boolsym;
	}
};
vector<Token*> Token_List;


////////////////////////////////////////////////////////////////符号表
class DataItem
{
public:
	string name;
	int datatype;//0:NUM  1:FLOAT
	int layer;
	bool not_assign;
	DataItem(int t = 0, string n = "")
	{
		name = n;
		datatype = t;
		not_assign = true;
		name = n;
	}
	virtual int get_numvalue() { return -1; }
	virtual void set_numvalue(int v) {}
	virtual float get_floatvalue() { return -1; }
	virtual void set_floatvalue(float v) {}
};
class NumItem : public DataItem
{
	int numvalue;
public:
	NumItem(int t = 0, string n = "", int v = 0) :DataItem(t, n), numvalue(v) {};
	int get_numvalue()
	{
		return numvalue;
	}
	void set_numvalue(int v)
	{
		numvalue = v;
	}
};
class FloatItem : public DataItem
{
	float floatvalue;
public:
	FloatItem(int t = 1, string n = "", float v = 0) :DataItem(t, n), floatvalue(v) {};
	float get_floatvalue()
	{
		return floatvalue;
	}
	void set_floatvalue(float v)
	{
		floatvalue = v;
	}
};
vector<DataItem*> Data_Table;
vector<DataItem*> Temp_Table;
vector<DataItem*> Arith_Table;
vector<DataItem*> Bool_Table;

/////////////////////////////////////////////////////////////////////四元式
class FourPart
{
public:
	FPType type;
	string op;
	DataItem* left;
	DataItem* right;
	DataItem* strtemp;
	int tempsig;
	int jumpsig;
	int jumpnum;//1:if的第一部分/while的开始 2:if的第二部分/while的结束 3:if的结束/while的条件判定处
	FourPart() { tempsig = -1; jumpnum = -1; jumpsig = 0; }
};
vector<FourPart> FourPart_Table;

//////////////////////////////////////////////////////////////四元式第二代
//type = 1:赋值/取非 格式:<command, op, _, dst> 
//type = 2:各种算术运算 格式:<command, op1, op2, dst> 
//ytpe = 3:跳转 格式:<command, _, _, jump> 
class FourPart_
{
public:
	string command;
	int type;
	DataItem* op1;
	DataItem* op2;
	DataItem* dst;
	int jump;
	FourPart_() { jump = -1; }
};
vector<FourPart_> FourPart_Table_;


/////////////////////////////////////////////////////////////变量表
vector<DataItem*> Variable_Table;   //声明变量表

bool IsLetter(char c);
bool IsDigit(char c);
bool IsSymbol(char c);
int GetKeyIndex(string a);
int GetTypeIndex(string a);
int GetIdIndex(string a);
void ShowTokenList();
void ShowSingleToken();
void OutLexicalResult(const char* filename);
int LexemeAnalysis(const char* filename);

void GetNextToken();
void CallError(int flag_);

int ParserAnalysis();
void compounds();
void declarations();
void declarations_();
void initialization();
void initialization_();
void statements();
void statements_();
void ifs();
void whiles();
void arithmetics();
void items();
void items_();
void arithmetics_();
void factors();

//void compounds_();
void bools();
void bools_();
void boolitems();
void boolitems_();
void boolfactors();
void boolean();

int InDataTable(string name);
void Assign_1(int start);
void Assign_2(int start);
void Assign_3(int start);
void Assign_4(int s1, int s2);
void Bool_1(int start);
void Bool_2();
void Bool_3();
void Bool_4();
void Bool_5();
void Bool_6();
void Bool_7();
void Add_Sub1();
void Add_Sub2(int s1, int s2, int symi);
void Plus_Abs1(int s1, int s2, int symi);
void Set_Jump(int n, int des);
void sen_CallError();
void IF_JumpOut();
void IF_JumpIn();
void WHILE_JumpOut();
void WHILE_JumpIn();
void show_Data_Table();
void show_FourPart_Table();
void show_Temp_Table();
void OutSemanticResult(const char* filename);
void Build_FourPart_Table_();
void Renew_FourPart_Table_(DataItem &data);
void Build_Variable_Tables(const char* filename);
void Print_Variable_Tables(const char* filename);
