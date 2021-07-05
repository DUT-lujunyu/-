#include "标头.h"


int LexemeAnalysis(const char* filename)
{
	fsrc = fopen(filename, "r");

	if (fsrc == NULL)
	{
		printf("无法打开程序文件!\n");
		_getch();
		return 1;
	}
	while ((c = fgetc(fsrc)) != EOF) {
		buffer = buffer + c;
		//cout << c;
	}
	int line = 1;
	int buffer_size = buffer.length();
	int lexemeBegin = 0, forword = 0;
	while (forword < buffer_size && !lexical_error) {
		//cout << "forword" << forword << "	buffer_size" << buffer_size << "	lexical_error" << lexical_error << endl;
		char c = buffer[forword];
		//过滤空格、制表符、换行
		while (c == ' ' || c == '\n' || c == '\t')
		{
			forword++;
			lexemeBegin++;
			if (c == '\n')
				line++;
			if (forword >= buffer_size || lexemeBegin >= buffer_size)
				break;
			c = buffer[forword];
			//cout << "空格";
		}
		if (IsLetter(c)) {
			while (true)
			{
				forword++;
				if (forword >= buffer_size) break;
				c = buffer[forword];
				if (!(IsLetter(c) || IsDigit(c))) break;
			}
			string tmp = buffer.substr(lexemeBegin, forword - lexemeBegin);
			int index = GetKeyIndex(tmp);
			//是关键字
			if (index != -1)
				Token_List.push_back(new Word(KEY, index, line));
			else
			{
				index = GetTypeIndex(tmp);
				//是类型名
				if (index != -1)
					Token_List.push_back(new Word(TYPE, index, line));
				//否则是变量名
				else
				{
					Token_List.push_back(new Word(ID, GetIdIndex(tmp), line));
				}
			}
			lexemeBegin = forword;
		}

		//识别数字
		else if (IsDigit(c))
		{
			bool endofinput = false;
			double ans = c - '0';
			while (true)
			{
				forword++;
				if (forword >= buffer_size)
				{
					endofinput = true;
					break;
				}
				c = buffer[forword];
				if (!IsDigit(c))
					break;
				ans = ans * 10 + c - '0';
			}
			if (endofinput)
			{
				Token_List.push_back(new Num(NUM, ans, line));
				lexemeBegin = forword;
			}
			else
			{
				//小数处理
				if (c == '.')
				{
					forword++;
					if (forword >= buffer_size || !IsDigit(buffer[forword])) {
						lexical_error = 2;
						cout << "Line " << line << "\t" << "Lexical Analysis Error!Missing figures behind '.'" << endl;
					}
					else
					{
						double w = 10;
						ans += (buffer[forword] - '0') / w;
						while (true)
						{
							forword++;
							w *= 10;
							if (forword >= buffer_size)
							{
								endofinput = true;
								break;
							}
							c = buffer[forword];
							if (!IsDigit(c))
								break;
							ans = ans + (c - '0') / w;
						}
						if (endofinput)
						{
							Token_List.push_back(new Float(FLOAT, ans, line));
							lexemeBegin = forword;
						}
						//小数的指数形式的表示
						else
						{
							if (c == 'e' || c == 'E')
							{
								double ord = 0;
								forword++;
								if (forword >= buffer_size)
								{
									lexical_error = 4;
									cout << "Line " << line << "\t" << "Lexical Analysis Error!Missing figures behind 'E' or 'e'" << endl;
								}
								else
								{
									c = buffer[forword];
									bool ispos = true;
									if (c == '+' || c == '-')
									{
										if (c == '-')
											ispos = false;
										forword++;
										if (forword >= buffer_size || !IsDigit(buffer[forword]))
										{
											lexical_error = 3;
											cout << "Line " << line << "\t" << "Lexical Analysis Error!Missing figures behind '+' or '-'" << endl;
										}
										else
										{
											c = buffer[forword];
											ord = c - '0';
										}
									}
									else if (!IsDigit(c))
									{
										lexical_error = 4;
										cout << "Line " << line << "\t" << "Lexical Analysis Error!Missing figures behind 'E' or 'e'" << endl;
									}
									else
									{
										ord = c - '0';
									}
									if (!lexical_error)
									{
										while (true)
										{
											forword++;
											if (forword >= buffer_size)   break;
											c = buffer[forword];
											if (!IsDigit(c)) break;
											ord = ord * 10 + c - '0';
										}
										if (!ispos)
											ord = -ord;
										ans = ans * pow(10, ord);
										Token_List.push_back(new Float(FLOAT, ans, line));
										lexemeBegin = forword;
									}
								}
							}
							else
							{
								Token_List.push_back(new Float(FLOAT, ans, line));
								lexemeBegin = forword;
							}
						}
					}
				}
				//整数+e
				else if (c == 'e' || c == 'E')
				{
					double ord = 0;
					forword++;
					if (forword >= buffer_size)
					{
						lexical_error = 4;
						cout << "Line " << line << "\t" << "Lexical Analysis Error!Missing figures behind 'E' or 'e'" << endl;
					}
					else
					{
						c = buffer[forword];
						bool ispos = true;
						if (c == '+' || c == '-')
						{
							if (c == '-')
								ispos = false;
							forword++;
							if (forword >= buffer_size || !IsDigit(buffer[forword]))
							{
								lexical_error = 3;
								cout << "Line " << line << "\t" << "Lexical Analysis Error!Missing figures behind '+' or '-'" << endl;
							}
							else
							{
								c = buffer[forword];
								ord = c - '0';
							}
						}
						else if (!IsDigit(c))
						{
							lexical_error = 4;
							cout << "Line " << line << "\t" << "Lexical Analysis Error!Missing figures behind 'E' or 'e'" << endl;
						}
						else
						{
							ord = c - '0';
						}
						if (!lexical_error)
						{
							while (true)
							{
								forword++;
								if (forword >= buffer_size)   break;
								c = buffer[forword];
								if (!IsDigit(c)) break;
								ord = ord * 10 + c - '0';
							}
							if (!ispos) ord = -ord;
							ans = ans * pow(10, ord);
							Token_List.push_back(new Float(FLOAT, ans, line));
							lexemeBegin = forword;
						}
					}
				}
				else
				{
					Token_List.push_back(new Num(NUM, int(ans), line));
					lexemeBegin = forword;
				}
			}
		}
		else if (c == '<' || c == '>' || c == '=' || c == '!')
		{
			forword++;
			if (forword < buffer_size&&buffer[forword] == '=')
			{
				switch (c)
				{
				case '<':
					Token_List.push_back(new CompSym(COMP, "<=", line));
					break;
				case '>':
					Token_List.push_back(new CompSym(COMP, ">=", line));
					break;
				case '!':
					Token_List.push_back(new CompSym(COMP, "!=", line));
					break;
				case '=':
					Token_List.push_back(new CompSym(COMP, "==", line));
					break;
				}
				forword++;
				lexemeBegin += 2;
			}
			else
			{
				switch (c)
				{
				case '<':
					Token_List.push_back(new CompSym(COMP, "<", line));
					break;
				case '>':
					Token_List.push_back(new CompSym(COMP, ">", line));
					break;
				case '!':
					Token_List.push_back(new BoolSym(BOSYM, "!", line));
					break;
				default:
					Token_List.push_back(new Symbol(SYM, c, line));
					break;
				}
				lexemeBegin++;
			}
		}
		else if (c == '&' || c == '|')
		{
			forword++;
			switch (c)
			{
			case '&':
				if (forword < buffer_size&&buffer[forword] == '&')
					Token_List.push_back(new BoolSym(BOSYM, "&&", line));
				else
				{
					lexical_error = 5;
					cout << "Line " << line << "\t" << "Lexical Analysis Error!Please check if the input you want is'&&'" << endl;
				}
				break;
			case '|':
				if (forword < buffer_size&&buffer[forword] == '|')
					Token_List.push_back(new BoolSym(BOSYM, "||", line));
				else
				{
					lexical_error = 6;
					cout << "Line " << line << "\t" << "Lexical Analysis Error!Please check if the input you want is'||'" << endl;
				}
				break;
			}
			if (!lexical_error) {
				forword++;
				lexemeBegin += 2;
			}
		}
		else if (IsSymbol(c))
		{
			Token_List.push_back(new Symbol(SYM, c, line));
			forword++;
			lexemeBegin++;
		}
		//无法识别
		else if (forword < buffer_size)
		{
			lexical_error = 7;
			cout << "Line " << line << "\t" << "Lexical Analysis Error!Unrecognized character occurred" << endl;
		}
		//ShowSingleToken();
	}
	if (lexical_error)
	{
		//error_message = "Lexical Analysis Error!";
		flag = 0;
		//cout << error_message << endl;
	}
	else
	{
		//cout << "======================Lexical analysis=====================" << endl;
		//ShowTokenList();
		cout << "Lexical Analysis Successfully!" << endl;
	}
	fclose(fsrc);
	return lexical_error;
}

bool IsLetter(char c)
{
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z') || (c == '_'))
		return true;
	else
		return false;
}
bool IsDigit(char c)
{
	if (c >= '0'&&c <= '9')
		return true;
	else
		return false;
}
bool IsSymbol(char c)
{
	for (int i = 0; i < symbols.length(); i++)
	{
		if (c == symbols[i])
			return true;
	}
	return false;
}
int GetKeyIndex(string a)
{
	for (int i = 0; i < k_w_len; i++)
	{
		if (keywords[i] == a)
			return i;
	}
	return -1;
}
int GetTypeIndex(string a)
{
	for (int i = 0; i < t_w_len; i++)
	{
		if (typewords[i] == a)
			return i;
	}
	return -1;
}
int GetIdIndex(string a)
{
	int len = idwords.size();
	for (int i = 0; i < len; i++)
	{
		if (idwords[i] == a)
			return i;
	}
	idwords.push_back(a);
	return len;
}
void ShowTokenList()
{
	int len = Token_List.size();
	for (int i = 0; i < len; i++)
	{
		int tmp = Token_List[i]->get_tag();
		if (tmp == SYM)
		{
			cout << Token_List[i]->get_symbol() << "\t" << Token_List[i]->get_symbol() << endl;
		}
		else if (tmp == NUM)
		{
			cout << "NUM" << "\t";
			cout << Token_List[i]->get_numvalue() << endl;
		}
		else if (tmp == FLOAT)
		{
			cout << "FLOAT" << "\t";
			cout << Token_List[i]->get_floatvalue() << endl;
		}
		else if (tmp == ID)
		{
			cout << "ID" << "\t";
			cout << Token_List[i]->get_lexeme_str() << endl;
		}
		else if (tmp == KEY)
		{
			cout << "KEY" << "\t";
			cout << Token_List[i]->get_lexeme_str() << endl;
		}
		else if (tmp == TYPE)
		{
			cout << "TYPE" << "\t";
			cout << Token_List[i]->get_lexeme_str() << endl;
		}
		else if (tmp == COMP)
		{
			cout << "COMP" << "\t";
			cout << Token_List[i]->get_compsym() << endl;
		}
		else if (tmp == BOSYM)
		{
			cout << "BOSYM" << "\t";
			cout << Token_List[i]->get_boolsym() << endl;
		}
		else cout << endl;
	}
}
void ShowSingleToken()//调试用
{
	int i = Token_List.size() - 1;
	int tmp = Token_List[i]->get_tag();
	if (tmp == SYM)
	{
		cout << "line " << Token_List[i]->line << "\t" << Token_List[i]->get_symbol() << "\t" << Token_List[i]->get_symbol() << endl;
	}
	else if (tmp == NUM)
	{
		cout << "line " << Token_List[i]->line << "\t" << "NUM" << "\t";
		cout << Token_List[i]->get_numvalue() << endl;
	}
	else if (tmp == FLOAT)
	{
		cout << "line " << Token_List[i]->line << "\t" << "FLOAT" << "\t";
		cout << Token_List[i]->get_floatvalue() << endl;
	}
	else if (tmp == ID)
	{
		cout << "line " << Token_List[i]->line << "\t" << "ID" << "\t";
		cout << Token_List[i]->get_lexeme_str() << endl;
	}
	else if (tmp == KEY)
	{
		cout << "line " << Token_List[i]->line << "\t" << "KEY" << "\t";
		cout << Token_List[i]->get_lexeme_str() << endl;
	}
	else if (tmp == TYPE)
	{
		cout << "line " << Token_List[i]->line << "\t" << "TYPE" << "\t";
		cout << Token_List[i]->get_lexeme_str() << endl;
	}
	else if (tmp == COMP)
	{
		cout << "line " << Token_List[i]->line << "\t" << "COMP" << "\t";
		cout << Token_List[i]->get_compsym() << endl;
	}
	else if (tmp == BOSYM)
	{
		cout << "line " << Token_List[i]->line << "\t" << "BOSYM" << "\t";
		cout << Token_List[i]->get_boolsym() << endl;
	}
	else cout << endl;
}
void OutLexicalResult(const char* filename)
{
	fout = fopen(filename, "w+");
	if (fout == NULL)
	{
		cout << "Output fail!" << endl;
		return;
	}
	int len = Token_List.size();
	for (int i = 0; i < len; i++)
	{

		int tmp = Token_List[i]->get_tag();
		if (tmp == SYM)
		{
			fprintf(fout, "Line %d\tSYM\t%c\n", Token_List[i]->line, Token_List[i]->get_symbol());
		}
		else if (tmp == NUM)
		{
			fprintf(fout, "Line %d\tNUM\t%d\n", Token_List[i]->line, Token_List[i]->get_numvalue());
		}
		else if (tmp == FLOAT)
		{
			fprintf(fout, "Line %d\tFLOAT\t%f\n", Token_List[i]->line, Token_List[i]->get_floatvalue());
		}
		else if (tmp == ID)
		{
			fprintf(fout, "Line %d\tID\t%s\n", Token_List[i]->line, Token_List[i]->get_lexeme_str().data());
		}
		else if (tmp == KEY)
		{
			fprintf(fout, "Line %d\tKEY\t%s\n", Token_List[i]->line, Token_List[i]->get_lexeme_str().data());
		}
		else if (tmp == TYPE)
		{
			fprintf(fout, "Line %d\tTYPE\t%s\n", Token_List[i]->line, Token_List[i]->get_lexeme_str().data());
		}
		else if (tmp == COMP)
		{
			fprintf(fout, "Line %d\tCOMP\t%s\n", Token_List[i]->line, Token_List[i]->get_compsym().data());
		}
		else if (tmp == BOSYM)
		{
			fprintf(fout, "Line %d\tBOSYM\t%s\n", Token_List[i]->line, Token_List[i]->get_boolsym().data());
		}
		else cout << endl;
	}
	fclose(fout);
}

//语法分析及其调用函数
void GetNextToken()
{
	if (Token_List[index]->get_tag() == SYM)
		cout << Token_List[index]->get_symbol() << endl;
	else if (Token_List[index]->get_tag() == FLOAT)
		cout << Token_List[index]->get_floatvalue() << endl;
	else if (Token_List[index]->get_tag() == NUM)
		cout << Token_List[index]->get_numvalue() << endl;
	else if (Token_List[index]->get_tag() == COMP)
		cout << Token_List[index]->get_compsym() << endl;
	else if (Token_List[index]->get_tag() == BOSYM)
		cout << Token_List[index]->get_boolsym() << endl;
	else if (Token_List[index]->get_tag() == TYPE || Token_List[index]->get_tag() == KEY || Token_List[index]->get_tag() == ID)
		cout << Token_List[index]->get_lexeme_str() << endl;
}

void CallError(int flag_)
{
	//cout << "index:" << index;
	cout << "Line" << Token_List[index - 1]->line << "\t";
	cout << "Syntax Analysis Error!";
	switch (flag_)
	{
	case 2:
		cout << "Extra characters at the end of the program" << endl;
		break;
	case 3:
		cout << "Missing'{'" << endl;
		break;
	case 4:
		cout << "Missing'}'" << endl;
		break;
	case 5:
		cout << "Missing'('" << endl;
		break;
	case 6:
		cout << "Missing')'" << endl;
		break;
	case 7:
		cout << "Using incorrect declaration statement" << endl;
		break;
	case 8:
		cout << "Missing';'" << endl;
		break;
	case 9:
		cout << "Using wrong compound sentences" << endl;
		break;
	case 10:
		cout << "Missing keyword 'else'" << endl;
		break;
	case 11:
		cout << "Using wrong arithmetic expression" << endl;
		break;
	case 12:
		cout << "Incorrect Boolean declaration" << endl;
		break;
	case 13:
		cout << "Extra characters in program" << endl;
		break;
	default:
		cout << "Compiler error!" << endl;
		break;
	}
}

int ParserAnalysis()  //文法分析
{
	//cout << "========================语法分析开始============================" << endl;
	compounds();
	if (flag == 1)
	{
		if (index == Token_List.size())
		{
			cout << "Syntax Analysis Successfully!" << endl;
			if (!sen_error)/////////////////////////////////////////////////////////////////////////
			{
				cout << "Semantic Analysis Successfully!" << endl;
			}
			else if (sen_error)
			{
				sen_CallError();
			}
		}
		else
		{
			CallError(flag = 2);
		}
	}
	return flag;
}
void compounds()  //复合语句
{
	Token* token = Token_List[index++];
	if (token->get_tag() == SYM && token->get_symbol() == '{')
	{
		sen_layer++;///////////////////////////////////////////////////////
		declarations();
		if (flag != 1) return;
		statements();
		if (flag != 1) return;
		if (index < Token_List.size())
		{
			token = Token_List[index];
			index++;
		}
		else
		{
			CallError(flag = 4);
			return;
		}
		if (token->get_tag() == SYM && token->get_symbol() == '}')
		{
			sen_layer--;///////////////////////////////////////////////////
		}
		else
		{
			CallError(flag = 4);
			return;
		}
	}
	else
	{
		CallError(flag = 3);
		return;
	}
}
void declarations()  //声明语句
{
	sen_start = 0;/////////////////////////
	sen_end = 0;///////////////////////////
	Token* token = Token_List[index];
	if (token->get_tag() == TYPE)
	{
		initialization();
		if (flag != 1) return;
		declarations_();
		if (flag != 1) return;
	}
}
void initialization()
{
	//cout << index << endl;
	Token* token = Token_List[index];
	index++;
	if (token->get_tag() == TYPE && strcmp(token->get_lexeme_str().data(), "void")) //判断是否为TYPE
	{
		sen_start = index - 1;/////////////////////记录赋值语句的起始下标
							  //index++;
		token = Token_List[index];
		index++;
		if (token->get_tag() == ID)
		{
			initialization_();
			if (flag != 1) return;
		}
		else
		{
			CallError(flag = 7);
			return;
		}
	}
	else
	{
		CallError(flag = 7);
		return;
	}
}
void initialization_()
{
	Token* token = Token_List[index];
	index++;
	if (token->get_tag() == SYM && token->get_symbol() == '=')         //声明时直接初始化，判断是否为等号
	{
		token = Token_List[index++];
		if (token->get_tag() == FLOAT || token->get_tag() == NUM) //判断是否为FLOAT或NUM？？？
		{
			if (!sen_error)
			{
				Assign_1(sen_start);////////////////////////////////////////////////////////////
			}
			token = Token_List[index];
			if (token->get_tag() == SYM && token->get_symbol() == ';')
			{
				index++;
			}
			else
			{
				CallError(flag = 8);
				return;
			}
		}
		else if (token->get_tag() == ID) //
		{
			if (!sen_error)
			{
				Assign_2(sen_start);////////////////////////////////////////////////////////////
			}
			token = Token_List[index++];
			if (token->get_tag() == SYM && token->get_symbol() == ';');
			else
			{
				CallError(flag = 8);
				return;
			}
		}
		else
		{
			CallError(flag = 7);
			return;
		}
	}
	else if (token->get_tag() == SYM && token->get_symbol() == ';')//普通声明
	{
		Assign_3(sen_start);
	}
	else
	{
		CallError(flag = 8);
		return;
	}
}
void declarations_()  //声明语句子语句
{
	Token* token = Token_List[index];
	if (token->get_tag() == TYPE) //判断是否为TYPE
	{
		initialization();
		if (flag != 1) return;
		declarations_();
		if (flag != 1) return;
	}
	else if ((token->get_tag() == KEY && strcmp(token->get_lexeme_str().data(), "if") == 0) ||
		(token->get_tag() == KEY && strcmp(token->get_lexeme_str().data(), "while") == 0) ||
		(token->get_tag() == SYM && token->get_symbol() == '}') ||
		(token->get_tag() == ID));
	else
	{
		CallError(flag = 13);
		return;
	}
}

void statements()  //语句
{
	Token* token = Token_List[index];
	index++;
	sen_start = index - 1;////////////////////////////////////////////////////////////////////
	if (token->get_tag() == KEY && strcmp(token->get_lexeme_str().data(), "if") == 0)
	{
		ifs();
		if (flag != 1) return;
	}
	else if (token->get_tag() == KEY && strcmp(token->get_lexeme_str().data(), "while") == 0)
	{
		whiles();
		if (flag != 1) return;
	}
	else if (token->get_tag() == ID)  //判断是否为ID
	{
		token = Token_List[index++];
		if (token->get_tag() == SYM && token->get_symbol() == '=')
		{
			int t1 = index - 2;///////////////////////////////////////////////////////////////////////
			arithmetics();
			int s2 = Arith_Table.size() - 1;//////////////////////////////////////////////////////////
			if (!sen_error)///////////////////////////////////////////////////////////////////////////
			{
				Assign_4(t1, s2);
			}
			if (flag != 1) return;
			token = Token_List[index];
			if (token->get_tag() == SYM && token->get_symbol() == ';')
				index++;
			else
			{
				CallError(flag = 8);
				return;
			}
		}
		else
		{
			CallError(flag = 9);
			return;
		}
	}
	else if (token->get_tag() == SYM && token->get_symbol() == '}')
	{
		index--;
		return;
	}
	statements_();
	if (flag != 1) return;
}

void statements_()
{
	while (index <= Token_List.size() - 2)
	{
		Token* token = Token_List[index];
		if (token->get_tag() == ID ||
			(token->get_tag() == KEY &&
			((strcmp(token->get_lexeme_str().data(), "if") == 0) ||
				strcmp(token->get_lexeme_str().data(), "while") == 0)))
		{
			statements();
			if (flag != 1) return;
		}
		else if (token->get_tag() == SYM && token->get_symbol() == '}')
		{
			return;
		}
		else
		{
			CallError(flag = 13);
			return;
		}
	}
}

void ifs() //if语句，未消除二义性
{
	is_while = 0;
	layer++;
	Token* token = Token_List[index++];
	if (token->get_tag() == SYM && token->get_symbol() == '(')
	{
		sen_logic_sig = 0;
		bools();
		if (flag != 1) return;
		token = Token_List[index++];
		if (token->get_tag() == SYM && token->get_symbol() == ')')
		{
			if ((!sen_error) && sen_logic_sig == 0)
			{
				IF_JumpIn();
			}
			if (!sen_error)
				Set_Jump(1 + 6 * (layer - 1), FourPart_Table.size() + 1);/////////////////////////////////////////////////////////////////////设置if开始的回填标号
			compounds();
			if (flag != 1) return;
			token = Token_List[index++];
			if (token->get_tag() == KEY && strcmp(token->get_lexeme_str().data(), "else") == 0)  //只能有else
			{
				if (!sen_error)
				{
					IF_JumpOut();/////////////////////////////////////////////////////////////////////////////////////////第一部分跳出
					Set_Jump(2 + 6 * (layer - 1), FourPart_Table.size() + 1);//////////////////////////////////////////////////////////////////设置if第二段的回填标号
				}
				compounds();
				if (!sen_error)
				{
					IF_JumpOut();
					Set_Jump(3 + 6 * (layer - 1), FourPart_Table.size() + 1);///////////////////////////////////////////////////////////////设置if结束的回填标号
				}
				if (flag != 1) return;
			}
			else
			{
				CallError(flag = 10);
				return;
			}
		}
		else
		{
			CallError(flag = 6);
			return;
		}
	}
	else
	{
		CallError(flag = 5);
		return;
	}
	layer--;
}
void whiles()  //while语句
{
	is_while = 1;
	layer++;
	Token* token = Token_List[index++];
	if (token->get_tag() == SYM && token->get_symbol() == '(')
	{
		sen_logic_sig = 0;
		int condition = FourPart_Table.size() + 1;////////////////////////////////////////////此处记录while的条件判定四元式起始位置
		bools();
		if (!flag) return;
		token = Token_List[index++];
		if (token->get_tag() == SYM && token->get_symbol() == ')')
		{
			if ((!sen_error) && sen_logic_sig == 0)
			{
				WHILE_JumpIn();
			}
			if (!sen_error)
				Set_Jump(4 + 6 * (layer - 1), FourPart_Table.size() + 1);/////////////////////////////////////////////////////////////////////设置while开始的回填标号
			else
				return;
			compounds();
			if (!sen_error)
			{
				WHILE_JumpOut();
				Set_Jump(6 + 6 * (layer - 1), condition);///////////////////////////////////////////////////////////////////////////////设置while条件判定回填标号
				Set_Jump(5 + 6 * (layer - 1), FourPart_Table.size() + 1);///////////////////////////////////////////////////////////////设置while结束的回填标号
			}
			if (!flag) return;
		}
		else
		{
			CallError(flag = 6);
			return;
		}
	}
	else
	{
		CallError(flag = 5);
		return;
	}
	layer--;
}
void arithmetics()  //算术表达式
{
	items();
	if (!flag) return;
	arithmetics_();
	if (!flag) return;
}
void items()  //项
{
	factors();
	if (!flag) return;
	items_();
	if (!flag) return;
}
void arithmetics_()   //T'
{
	int s1, s2;
	Token *token = Token_List[index];
	index++;
	if (token->get_tag() == SYM && (token->get_symbol() == '+' || token->get_symbol() == '-'))
	{
		int symi = index - 1;
		s1 = Arith_Table.size() - 1;
		items();
		s2 = Arith_Table.size() - 1;
		if (!sen_error)
			Add_Sub2(s1, s2, symi);
		if (!flag) return;
		if (token->get_tag() == SYM && (token->get_symbol() == '+' || token->get_symbol() == '-'))
		{
			arithmetics_();
		}
	}
	else Token_List[index--];
}
void factors()  //F  
{
	Token* token = Token_List[index++];
	if (token->get_tag() == ID || token->get_tag() == NUM || token->get_tag() == FLOAT)
	{
		if (!sen_error)
			Add_Sub1();
	}
	else if (token->get_tag() == SYM && token->get_symbol() == '(')
	{
		arithmetics();
		if (!flag) return;
		token = Token_List[index++];
		if (token->get_tag() == SYM && token->get_symbol() == ')');
		else
		{
			CallError(flag = 6);
			return;
		}
	}
	else
	{
		CallError(flag = 11);
		return;
	}
}
void items_()  //F'
{
	Token *token = Token_List[index];
	index++;
	int s1, s2;///////////////////////////////////////////////////////////////////////////////////////
	if (token->get_tag() == SYM && (token->get_symbol() == '*' || token->get_symbol() == '/'))
	{
		int symi = index - 1;/////////////////////////////////////////////////////////////////////////
		s1 = Arith_Table.size() - 1;//////////////////////////////////////////////////////////////////
		factors();
		s2 = Arith_Table.size() - 1;//////////////////////////////////////////////////////////////////
		if (!sen_error)
			Plus_Abs1(s1, s2, symi);
		if (!flag) return;
		if (token->get_tag() == SYM && (token->get_symbol() == '*' || token->get_symbol() == '/'))
		{
			items_();
		}
	}
	else Token_List[index--];
}

void bools()
{
	boolitems();
	if (flag != 1) return;
	bools_();
	if (flag != 1) return;
}
void bools_()
{
	Token *token = Token_List[index++];
	if (token->get_tag() == BOSYM && (strcmp(token->get_boolsym().data(), "||") == 0))
	{
		sen_logic_sig = 1;
		if (!sen_error)
			Bool_4();
		bools();
		if (flag != 1) return;
		if (!sen_error)
			Bool_5();
	}
	else Token_List[index--];
}
void boolitems()
{
	boolfactors();
	if (flag != 1) return;
	boolitems_();
	if (flag != 1) return;
}
void boolitems_()
{
	Token *token = Token_List[index++];
	if (token->get_tag() == BOSYM && (strcmp(token->get_boolsym().data(), "&&") == 0))
	{
		sen_logic_sig = 1;
		if (!sen_error)
			Bool_6();
		boolitems();
		if (flag != 1) return;
		if (!sen_error)
			Bool_7();
	}
	else Token_List[index--];
}
void boolfactors()
{
	Token *token = Token_List[index++];
	if (token->get_tag() == BOSYM && (strcmp(token->get_boolsym().data(), "!") == 0))
	{
		boolfactors();
		if (!sen_error)
			Bool_2();//////////////////////////////////////////////////////////
		if (flag != 1) return;
	}
	else if (token->get_tag() == SYM && token->get_symbol() == '(')
	{
		bools();
		if (flag != 1) return;
		token = Token_List[index++];
		if (token->get_tag() == SYM && token->get_symbol() == ')');
		else
		{
			CallError(flag = 6);
			return;
		}
	}
	else
	{
		Token_List[index--];
		boolean();
		if (!flag) return;
	}
}
void boolean()
{
	Token *token = Token_List[index++];
	if (token->get_tag() == NUM || token->get_tag() == FLOAT || token->get_tag() == ID)   //判断是否为ID或数据
	{
		sen_start = index - 1;/////////////////////////////////////////////////////////////////////////////////
		token = Token_List[index++];
		if (token->get_tag() == COMP)   //判断是否为COMP
		{
			token = Token_List[index++];
			if (token->get_tag() == FLOAT || token->get_tag() == NUM || token->get_tag() == ID) //判断是否为FLOAT或ID
			{
				if (!sen_error)
					Bool_1(sen_start);
			}
			else
			{
				Token_List[index--];
			}
		}
		/////////////////////////////////////////////////////////////////////////////布尔式只有一个量的情况

		else
		{
			Token_List[index--];
			if (!sen_error)
				Bool_3();
		}
	}
	else
	{
		CallError(flag = 12);
		return;
	}
}

//////////////////////////////////////////////////////////////////////////////////语义分析
int InDataTable(string name)
{
	int n = 1;
	int m = Data_Table.size();
	for (; n <= m; n++)
	{
		if (Data_Table[n - 1]->name == name)//在符号表中注册
		{
			return n - 1;
		}
	}
	return -1;
}
int sameValue_InDataTable(string name)//没什么用，考虑删除
{
	int n = Data_Table.size();
	for (; n >= 1; n--)
	{
		if (Data_Table[n - 1]->name == name)//在符号表中注册
		{
			return n - 1;
		}
	}
	return -1;
}
void Assign_1(int start)//一类声明语句 ： a = 立即数
{
	int i = start;
	Token *token = Token_List[i++];//Token->TYPE,i->ID
	FourPart temp;
	//建立临时左项
	if (token->get_lexeme_str() == "int")
	{
		temp.right = new NumItem(NUM);
	}
	else if (token->get_lexeme_str() == "float")
	{
		temp.right = new FloatItem(FLOAT);
	}
	token = Token_List[i++];//Token->ID,i->'='
	int n = InDataTable(token->get_lexeme_str());
	if (n != -1)//变量重复定义
	{
		sen_error = 4;
		sen_error_line = token->line;
		delete temp.right;
		return;
	}
	temp.right->name = token->get_lexeme_str();
	token = Token_List[i++];//Token->'=',i->NUM/FLOAT
	token = Token_List[i++];//Token->NUM/FLOAT,i->';'
	if (temp.right->datatype == NUM && token->get_tag() == FLOAT)//允许float a = 1的情况，不允许int a = 1.2的情况
	{
		sen_error = 1;
		sen_error_line = token->line;
		delete temp.left;
		return;
	}
	else if (token->get_tag() == NUM)
	{
		int v = token->get_numvalue();
		temp.op = "=";
		temp.type = ASSIGN;
		if (temp.right->datatype == NUM)
			temp.right->set_numvalue(v);
		else
			temp.right->set_floatvalue(v);
		temp.right->not_assign = false;
		temp.right->layer = sen_layer;
		temp.left = new NumItem(NUM, "", v);
		Data_Table.push_back(temp.right);
		FourPart_Table.push_back(temp);
	}
	else if (token->get_tag() == FLOAT)
	{
		float v = token->get_floatvalue();
		temp.op = "=";
		temp.type = ASSIGN;
		temp.right->set_floatvalue(v);
		temp.right->not_assign = false;
		temp.right->layer = sen_layer;
		temp.left = new FloatItem(FLOAT, "", v);
		Data_Table.push_back(temp.right);
		FourPart_Table.push_back(temp);
	}
	sen_start = 0;
	return;
}
void Assign_2(int start)//二类声明语句 ：a = b
{
	int i = start;
	Token *token = Token_List[i++];//Token->TYPE,i->ID
	FourPart temp;
	//建立临时左项
	if (token->get_lexeme_str() == "int")
	{
		temp.right = new NumItem(NUM);
	}
	else if (token->get_lexeme_str() == "float")
	{
		temp.right = new FloatItem(FLOAT);
	}
	token = Token_List[i++];//Token->ID,i->'='
	int n = InDataTable(token->get_lexeme_str());
	if (n != -1)//变量重复定义
	{
		sen_error = 4;
		sen_error_line = token->line;
		delete temp.right;
		return;
	}
	temp.right->name = token->get_lexeme_str();
	token = Token_List[i++];//Token->'=',i->ID
	token = Token_List[i++];//Token->ID,i->';'
	n = InDataTable(token->get_lexeme_str());
	if (n == -1)//符号未注册
	{
		sen_error = 2;
		sen_error_line = token->line;
		delete temp.right;
		return;
	}
	else if (Data_Table[n]->not_assign)//右侧变量未被赋值
	{
		sen_error = 3;
		sen_error_line = token->line;
		delete temp.right;
		return;
	}
	else if (Data_Table[n]->datatype != temp.right->datatype)//左右类型不匹配
	{
		sen_error = 1;
		sen_error_line = token->line;
		delete temp.right;
		return;
	}
	if (Data_Table[n - 1]->datatype == NUM)
	{
		int v = Data_Table[n - 1]->get_numvalue();
		temp.op = "=";
		temp.type = ASSIGN;
		temp.right->set_numvalue(v);
		temp.right->not_assign = false;
		temp.right->layer = sen_layer;
		temp.left = Data_Table[n - 1];
		Data_Table.push_back(temp.right);
		FourPart_Table.push_back(temp);
	}
	else if (Data_Table[n - 1]->datatype == FLOAT)
	{
		float v = Data_Table[n - 1]->get_floatvalue();
		temp.op = "=";
		temp.type = ASSIGN;
		temp.right->set_floatvalue(v);
		temp.right->not_assign = false;
		temp.right->layer = sen_layer;
		temp.left = Data_Table[n - 1];
		Data_Table.push_back(temp.right);
		FourPart_Table.push_back(temp);
	}
	sen_start = 0;
	return;
}
void Assign_3(int start)//三类声明语句 ： a 
{
	int i = start;
	Token *token = Token_List[i++];//Token->TYPE,i->ID
	FourPart temp;
	//建立临时左项
	if (token->get_lexeme_str() == "int")
	{
		temp.right = new NumItem(NUM);
		temp.right->datatype = NUM;
	}
	else if (token->get_lexeme_str() == "float")
	{
		temp.right = new FloatItem(FLOAT);
		temp.right->datatype = FLOAT;
	}
	token = Token_List[i++];//Token->ID,i->'='
	int n = InDataTable(token->get_lexeme_str());
	if (n != -1)//变量重复定义
	{
		sen_error = 4;
		sen_error_line = token->line;
		delete temp.right;
		return;
	}
	temp.right->name = token->get_lexeme_str();
	temp.right->layer = sen_layer;
	Data_Table.push_back(temp.right);
	sen_start = 0;
	return;
}
void Assign_4(int s1, int s2)//算术表达式的赋值
{
	int  n;
	Token * token = Token_List[s1];
	n = InDataTable(token->get_lexeme_str());
	if (n == -1)//符号未注册
	{
		sen_error = 1;
		sen_error_line = token->line;
		return;
	}
	Data_Table[n]->not_assign = false;
	FourPart temp;
	temp.op = "=";
	temp.type = ASSIGN;
	temp.left = Arith_Table[s2];
	temp.right = Data_Table[n];
	FourPart_Table.push_back(temp);
	return;
}
void Bool_1(int start)//普通的比较
{
	int i = start, n;
	float leftv = 0, rightv = 0;
	Token *token = Token_List[i++];//Token->NUM/FLOAT,i->COMP
	FourPart temp;
	if (token->get_tag() == NUM)
	{
		temp.left = new NumItem(NUM, "", token->get_numvalue());
		leftv = token->get_numvalue();
	}
	else if (token->get_tag() == FLOAT)
	{
		temp.left = new FloatItem(FLOAT, "", token->get_floatvalue());
		leftv = token->get_floatvalue();
	}
	else if (token->get_tag() == ID)
	{
		n = InDataTable(token->get_lexeme_str());
		if (n == -1)
		{
			sen_error = 2;
			sen_error_line = token->line;
			delete temp.left;
			return;
		}
		else if (Data_Table[n]->not_assign)//变量未被赋值
		{
			sen_error = 3;
			sen_error_line = token->line;
			delete temp.left;
			return;
		}
		else
		{
			temp.left = Data_Table[n];
			leftv = (Data_Table[n]->datatype == NUM) ? Data_Table[n]->get_numvalue() : Data_Table[n]->get_floatvalue();
		}
	}
	token = Token_List[i++];//Token->COMP,i->NUM/FLOAT
	temp.op = token->get_compsym();
	token = Token_List[i++];//Token->NUM/FLOAT,i->';'
	temp.type = BOOL;
	if (token->get_tag() == NUM)
	{
		temp.right = new NumItem(NUM, "", token->get_numvalue());
		rightv = token->get_numvalue();
	}
	else if (token->get_tag() == FLOAT)
	{
		temp.right = new FloatItem(FLOAT, "", token->get_floatvalue());
		rightv = token->get_floatvalue();
	}
	else if (token->get_tag() == ID) {
		n = InDataTable(token->get_lexeme_str());
		if (n == -1)
		{
			sen_error = 2;
			sen_error_line = token->line;
			delete temp.left;
			return;
		}
		else if (Data_Table[n]->not_assign)//变量未被赋值
		{
			sen_error = 3;
			sen_error_line = token->line;
			delete temp.left;
			return;
		}
		else
		{
			temp.right = Data_Table[n];
			rightv = (Data_Table[n]->datatype == NUM) ? Data_Table[n]->get_numvalue() : Data_Table[n]->get_floatvalue();
		}
	}
	//创建临时存储的变量
	temp.strtemp = new NumItem(NUM, "t" + to_string(temp_table_size));
	if (temp.op == ">=")
	{
		temp.strtemp->set_numvalue(leftv >= rightv);
	}
	else if (temp.op == ">")
	{
		temp.strtemp->set_numvalue(leftv > rightv);
	}
	else if (temp.op == "<=")
	{
		temp.strtemp->set_numvalue(leftv <= rightv);
	}
	else if (temp.op == "<")
	{
		temp.strtemp->set_numvalue(leftv < rightv);
	}
	else if (temp.op == "==")
	{
		temp.strtemp->set_numvalue(leftv == rightv);
	}
	else if (temp.op == "!=")
	{
		temp.strtemp->set_numvalue(leftv != rightv);
	}
	temp.strtemp->not_assign = false;
	Temp_Table.push_back(temp.strtemp);
	Bool_Table.push_back(temp.strtemp);
	temp.tempsig = temp_table_size;
	FourPart_Table.push_back(temp);
	sen_start = 0;
	temp_table_size++;
	return;
}
void Bool_2()//!的bool
{
	FourPart temp;
	temp.type = NOT;
	temp.op = "!";
	//temp.left = Temp_Table[temp_table_size - 1];//使用最新的临时变量
	temp.left = Bool_Table[Bool_Table.size() - 1];//////////////////////////
	temp.strtemp = new NumItem(NUM, "t" + to_string(temp_table_size));
	temp.strtemp->set_numvalue(!(temp.left->datatype == NUM ? temp.left->get_numvalue() : temp.left->get_floatvalue()));
	temp.strtemp->not_assign = false;
	Temp_Table.push_back(temp.strtemp);
	Bool_Table.push_back(temp.strtemp);/////////////////////////////////////
	temp.tempsig = temp_table_size;
	FourPart_Table.push_back(temp);
	sen_start = 0;
	temp_table_size++;
	return;
}
void Bool_3()//bool式是一个量
{
	int i = index - 1, n;
	float rightv;
	Token *token = Token_List[i];
	DataItem *t;
	if (token->get_tag() == NUM)
	{
		t = new NumItem(NUM, "", token->get_numvalue());
	}
	else if (token->get_tag() == FLOAT)
	{
		t = new FloatItem(FLOAT, "", token->get_floatvalue());
	}
	else if (token->get_tag() == ID) {
		n = InDataTable(token->get_lexeme_str());
		if (n == -1)
		{
			sen_error = 2;
			sen_error_line = token->line;
			return;
		}
		else
		{
			t = Data_Table[n];
		}
	}
	Bool_Table.push_back(t);
	return;
}
void Bool_4()//处理||前面的跳转的四元式
{
	FourPart temp;
	temp.op = "jnz";
	temp.type = JUMPNZ;
	//temp.left = Temp_Table[temp_table_size - 1];//使用最新的临时变量
	temp.left = Bool_Table[Bool_Table.size() - 1];
	if (is_while == 0)
	{
		temp.jumpnum = 1;//跳到if开始的地方/while开始
	}
	else if (is_while == 1)
	{
		temp.jumpnum = 4;//跳到if开始的地方/while开始
	}
	FourPart_Table.push_back(temp);
}
void Bool_5()//处理||后面的跳转的四元式
{
	FourPart temp;
	temp.op = "jz";
	temp.type = JUMPZ;
	//temp.left = Temp_Table[temp_table_size - 1];//使用最新的临时变量
	temp.left = Bool_Table[Bool_Table.size() - 1];
	if (is_while == 0)
	{
		temp.jumpnum = 2;
	}
	else if (is_while == 1)
	{
		temp.jumpnum = 5;
	}
	FourPart_Table.push_back(temp);
}
void Bool_6()//处理&&前面的跳转的四元式
{
	FourPart temp;
	temp.op = "jz";
	temp.type = JUMPZ;
	//temp.left = Temp_Table[temp_table_size - 1];//使用最新的临时变量
	temp.left = Bool_Table[Bool_Table.size() - 1];
	if (is_while == 0)
	{
		temp.jumpnum = 2;
	}
	else if (is_while == 1)
	{
		temp.jumpnum = 5;
	}
	FourPart_Table.push_back(temp);
}
void Bool_7()//处理&&后面的跳转的四元式
{
	FourPart temp;
	temp.op = "jz";
	temp.type = JUMPZ;
	//temp.left = Temp_Table[temp_table_size - 1];//使用最新的临时变量
	temp.left = Bool_Table[Bool_Table.size() - 1];
	if (is_while == 0)
	{
		temp.jumpnum = 2;
	}
	else if (is_while == 1)
	{
		temp.jumpnum = 5;
	}
	FourPart_Table.push_back(temp);
}

void Add_Sub1()//处理最小项（一个变量或者数字）
{
	int i = index - 1, n;
	float rightv;
	Token *token = Token_List[i];
	DataItem *t;
	if (token->get_tag() == NUM)
	{
		t = new NumItem(NUM, "", token->get_numvalue());
	}
	else if (token->get_tag() == FLOAT)
	{
		t = new FloatItem(FLOAT, "", token->get_floatvalue());
	}
	else if (token->get_tag() == ID) {
		n = InDataTable(token->get_lexeme_str());
		if (n == -1)
		{
			sen_error = 2;
			sen_error_line = token->line;
			return;
		}
		else if (Data_Table[n]->not_assign)//右侧变量未被赋值
		{
			sen_error = 3;
			sen_error_line = token->line;
			return;
		}
		else
		{
			t = Data_Table[n];
		}
	}
	Arith_Table.push_back(t);
	return;
}
void Add_Sub2(int s1, int s2, int symi)///加法或者减法
{
	int i = symi, n;
	float lv, rv;
	Token *token = Token_List[i];
	FourPart temp;
	temp.op = (token->get_symbol() == '+') ? "+" : "-";
	temp.type = (token->get_symbol() == '+') ? (ADD) : (SUB);
	temp.left = Arith_Table[s1];
	temp.right = Arith_Table[s2];
	lv = Arith_Table[s1]->datatype == NUM ? Arith_Table[s1]->get_numvalue() : Arith_Table[s1]->get_floatvalue();
	rv = Arith_Table[s2]->datatype == NUM ? Arith_Table[s2]->get_numvalue() : Arith_Table[s2]->get_floatvalue();
	if (Arith_Table[s1]->datatype == NUM && Arith_Table[s2]->datatype == NUM)
	{
		temp.strtemp = new NumItem(NUM, "t" + to_string(temp_table_size));
		temp.strtemp->set_floatvalue((token->get_symbol() == '+') ? (lv + rv) : (lv - rv));
	}
	else
	{
		temp.strtemp = new FloatItem(FLOAT, "t" + to_string(temp_table_size));
		temp.strtemp->set_floatvalue((token->get_symbol() == '+') ? (lv + rv) : (lv - rv));
	}
	temp.strtemp->not_assign = false;
	temp.tempsig = temp_table_size;
	Temp_Table.push_back(temp.strtemp);
	Arith_Table.push_back(temp.strtemp);
	FourPart_Table.push_back(temp);
	temp_table_size++;
	return;
}
void Plus_Abs1(int s1, int s2, int symi)
{
	int i = symi, n;
	float lv, rv;
	Token *token = Token_List[i];
	FourPart temp;
	temp.op = (token->get_symbol() == '*') ? "*" : "/";
	temp.type = (token->get_symbol() == '*') ? (PLUS) : (ABST);
	temp.left = Arith_Table[s1];
	temp.right = Arith_Table[s2];
	lv = Arith_Table[s1]->datatype == NUM ? Arith_Table[s1]->get_numvalue() : Arith_Table[s1]->get_floatvalue();
	rv = Arith_Table[s2]->datatype == NUM ? Arith_Table[s2]->get_numvalue() : Arith_Table[s2]->get_floatvalue();
	if (temp.type == ABST && rv == 0)
	{
		sen_error = 5;
		sen_error_line = token->line;
		return;
	}
	temp.strtemp = new FloatItem(FLOAT, "t" + to_string(temp_table_size));
	temp.strtemp->set_floatvalue((token->get_symbol() == '*') ? (lv * rv) : (lv / rv));
	temp.tempsig = temp_table_size;
	temp.strtemp->not_assign = false;
	Temp_Table.push_back(temp.strtemp);
	Arith_Table.push_back(temp.strtemp);
	FourPart_Table.push_back(temp);
	temp_table_size++;
	return;
}


void Set_Jump(int n, int des)
{
	int i = 1, l = FourPart_Table.size();
	for (; i <= l; i++)
	{
		if ((FourPart_Table[i - 1].jumpnum == n) && (FourPart_Table[i - 1].jumpsig == 0))
		{
			FourPart_Table[i - 1].jumpnum = des;
			FourPart_Table[i - 1].jumpsig = 1;
		}
	}
	return;
}
void sen_CallError()
{
	switch (sen_error)
	{
	case 1:
	{
		cout << "Line " << sen_error_line << "\tSemantic Analysis Error!";
		cout << "Assignment statement type mismatch!" << endl;
		break;
	}
	case 2:
	{
		cout << "Line " << sen_error_line << "\tSemantic Analysis Error!";
		cout << "Use undefined variable!" << endl;
		break;
	}
	case 3:
	{
		cout << "Line " << sen_error_line << "\tSemantic Analysis Error!";
		cout << "Unassigned variable to the right of assignment symbol!" << endl;
		break;
	}
	case 4:
	{
		cout << "Line " << sen_error_line << "\tSemantic Analysis Error!";
		cout << "Variable repeat definition!" << endl;
		break;
	}
	case 5:
	{
		cout << "Line " << sen_error_line << "\tSemantic Analysis Error!";
		cout << "Divisor cannot be zero!" << endl;
		break;
	}
	}
}
void IF_JumpOut()
{
	FourPart temp;
	temp.op = "j";
	temp.type = JUMP;
	temp.jumpnum = 3 + 6 * (layer - 1);//跳到if结束的地方
	FourPart_Table.push_back(temp);
}
void IF_JumpIn()
{
	FourPart temp;
	temp.op = "jz";
	temp.type = JUMPZ;
	temp.left = Temp_Table[Temp_Table.size() - 1];
	temp.jumpnum = 2 + 6 * (layer - 1);//跳到if的第二段
	FourPart_Table.push_back(temp);
}
void WHILE_JumpOut()
{
	FourPart temp;
	temp.op = "j";
	temp.type = JUMP;
	temp.jumpnum = 6+6*(layer-1);//跳到while的条件判定处
	FourPart_Table.push_back(temp);
}
void WHILE_JumpIn()
{
	FourPart temp;
	temp.op = "jz";
	temp.type = JUMPZ;
	temp.left = Temp_Table[Temp_Table.size() - 1];
	temp.jumpnum = 5 + 6 * (layer - 1);//跳到while结束
	FourPart_Table.push_back(temp);
}
void show_Data_Table()
{
	int len = Data_Table.size();
	int i = 1;
	for (; i <= len; i++)
	{
		cout << i << "\t" << Data_Table[i - 1]->name << "\t";
		if (Data_Table[i - 1]->datatype == NUM)
		{
			cout << "int\tLayer" << Data_Table[i - 1]->layer << "\t";
			if (Data_Table[i - 1]->not_assign)
			{
				cout << endl;
			}
			else
			{
				cout << Data_Table[i - 1]->get_numvalue() << endl;
			}
		}
		else if (Data_Table[i - 1]->datatype == FLOAT)
		{
			cout << "float\tLayer" << Data_Table[i - 1]->layer << "\t";
			if (Data_Table[i - 1]->not_assign)
			{
				cout << endl;
			}
			else
			{
				cout << Data_Table[i - 1]->get_floatvalue() << endl;
			}
		}
	}
}
void show_Temp_Table()
{
	int len = Temp_Table.size();
	int i = 1;
	for (; i <= len; i++)
	{
		cout << i << "\t" << Temp_Table[i - 1]->name << "\t";
		if (Temp_Table[i - 1]->datatype == NUM)
		{
			cout << "int\t";
			cout << Temp_Table[i - 1]->get_numvalue() << endl;
		}
		else if (Temp_Table[i - 1]->datatype == FLOAT)
		{
			cout << "float\t";
			cout << Temp_Table[i - 1]->get_floatvalue() << endl;
		}
	}
}
void show_FourPart_Table()
{
	int len = FourPart_Table.size();
	int i = 1;
	for (; i <= len; i++)
	{
		cout << "(" << i << ")\t";
		if (FourPart_Table[i - 1].type == ASSIGN)
		{
			cout << "< =,";
			if (FourPart_Table[i - 1].left->name != "")
			{
				cout << FourPart_Table[i - 1].left->name << ",_,";
			}
			else if (FourPart_Table[i - 1].left->datatype == NUM)
			{
				cout << FourPart_Table[i - 1].left->get_numvalue() << ",_,";
			}
			else if (FourPart_Table[i - 1].left->datatype == FLOAT)
			{
				cout << FourPart_Table[i - 1].left->get_floatvalue() << ",_,";
			}
			cout << FourPart_Table[i - 1].right->name << " >" << endl;
		}
		else if (FourPart_Table[i - 1].type == BOOL)
		{
			cout << "< " << FourPart_Table[i - 1].op << ",";
			if (FourPart_Table[i - 1].left->name == "")
			{
				if (FourPart_Table[i - 1].left->datatype == NUM)
				{
					cout << FourPart_Table[i - 1].left->get_numvalue() << ",";
				}
				else if (FourPart_Table[i - 1].left->datatype == FLOAT)
				{
					cout << FourPart_Table[i - 1].left->get_floatvalue() << "," << endl;
				}
			}
			else
			{
				cout << FourPart_Table[i - 1].left->name << ",";
			}
			if (FourPart_Table[i - 1].right->name == "")
			{
				if (FourPart_Table[i - 1].right->datatype == NUM)
				{
					cout << FourPart_Table[i - 1].right->get_numvalue() << ",";
				}
				else if (FourPart_Table[i - 1].right->datatype == FLOAT)
				{
					cout << FourPart_Table[i - 1].right->get_floatvalue() << ",";
				}
			}
			else
			{
				cout << FourPart_Table[i - 1].right->name << ",";
			}
			cout << Temp_Table[FourPart_Table[i - 1].tempsig]->name << " >" << endl;
		}
		else if (FourPart_Table[i - 1].type == NOT)
		{
			//cout << "测试!  " << FourPart_Table[i - 1].strtemp->name << endl;
			cout << "< !," << FourPart_Table[i - 1].left->name << ",_," << FourPart_Table[i - 1].strtemp->name << " >" << endl;

		}
		else if (FourPart_Table[i - 1].type == JUMP)
		{
			cout << "< j,_,_," << FourPart_Table[i - 1].jumpnum << " >" << endl;
		}
		else if (FourPart_Table[i - 1].type == JUMPZ)
		{
			cout << "< jz," << FourPart_Table[i - 1].left->name << ",_," << FourPart_Table[i - 1].jumpnum << " >" << endl;
		}

		else if (FourPart_Table[i - 1].type == JUMPNZ)
		{
			cout << "< jnz," << FourPart_Table[i - 1].left->name << ",_," << FourPart_Table[i - 1].jumpnum << " >" << endl;
		}
		else if (FourPart_Table[i - 1].type == ADD)
		{
			float tv;
			cout << "< +,";
			if (FourPart_Table[i - 1].left->name == "")
			{
				tv = (FourPart_Table[i - 1].left->datatype == NUM) ? FourPart_Table[i - 1].left->get_numvalue() : FourPart_Table[i - 1].left->get_floatvalue();
				cout << tv;
				cout << ",";
			}
			else
			{
				cout << FourPart_Table[i - 1].left->name << ",";
			}
			if (FourPart_Table[i - 1].right->name == "")
			{
				tv = (FourPart_Table[i - 1].right->datatype == NUM) ? FourPart_Table[i - 1].right->get_numvalue() : FourPart_Table[i - 1].right->get_floatvalue();
				cout << tv;
				cout << ",";
			}
			else
			{
				cout << FourPart_Table[i - 1].right->name << ",";
			}
			cout << Temp_Table[FourPart_Table[i - 1].tempsig]->name << " >" << endl;
		}
		else if (FourPart_Table[i - 1].type == SUB)
		{
			float tv;
			cout << "< -,";
			if (FourPart_Table[i - 1].left->name == "")
			{
				tv = (FourPart_Table[i - 1].left->datatype == NUM) ? FourPart_Table[i - 1].left->get_numvalue() : FourPart_Table[i - 1].left->get_floatvalue();
				cout << tv;
				cout << ",";
			}
			else
			{
				cout << FourPart_Table[i - 1].left->name << ",";
			}
			if (FourPart_Table[i - 1].right->name == "")
			{
				tv = (FourPart_Table[i - 1].right->datatype == NUM) ? FourPart_Table[i - 1].right->get_numvalue() : FourPart_Table[i - 1].right->get_floatvalue();
				cout << tv;
				cout << ",";
			}
			else
			{
				cout << FourPart_Table[i - 1].right->name << ",";
			}
			cout << Temp_Table[FourPart_Table[i - 1].tempsig]->name << " >" << endl;
		}
		else if (FourPart_Table[i - 1].type == PLUS)
		{
			float tv;
			cout << "< *,";
			if (FourPart_Table[i - 1].left->name == "")
			{
				tv = (FourPart_Table[i - 1].left->datatype == NUM) ? FourPart_Table[i - 1].left->get_numvalue() : FourPart_Table[i - 1].left->get_floatvalue();
				cout << tv;
				cout << ",";
			}
			else
			{
				cout << FourPart_Table[i - 1].left->name << ",";
			}
			if (FourPart_Table[i - 1].right->name == "")
			{
				tv = (FourPart_Table[i - 1].right->datatype == NUM) ? FourPart_Table[i - 1].right->get_numvalue() : FourPart_Table[i - 1].right->get_floatvalue();
				cout << tv;
				cout << ",";
			}
			else
			{
				cout << FourPart_Table[i - 1].right->name << ",";
			}
			cout << Temp_Table[FourPart_Table[i - 1].tempsig]->name << " >" << endl;
		}
		else if (FourPart_Table[i - 1].type == ABST)
		{
			float tv;
			cout << "< /,";
			if (FourPart_Table[i - 1].left->name == "")
			{
				tv = (FourPart_Table[i - 1].left->datatype == NUM) ? (FourPart_Table[i - 1].left->get_numvalue()) : (FourPart_Table[i - 1].left->get_floatvalue());
				cout << tv;
				cout << ",";
			}
			else
			{
				cout << FourPart_Table[i - 1].left->name << ",";
			}
			if (FourPart_Table[i - 1].right->name == "")
			{
				tv = (FourPart_Table[i - 1].right->datatype == NUM) ? FourPart_Table[i - 1].right->get_numvalue() : FourPart_Table[i - 1].right->get_floatvalue();
				cout << tv;
				cout << ",";
			}
			else
			{
				cout << FourPart_Table[i - 1].right->name << ",";
			}
			cout << Temp_Table[FourPart_Table[i - 1].tempsig]->name << " >" << endl;
		}
	}
}
void OutSemanticResult(const char* filename)
{
	fout = fopen(filename, "w+");
	if (fout == NULL)
	{
		cout << "Semantic Output fail!" << endl;
		return;
	}
	int len = FourPart_Table.size();
	int i = 1;
	for (; i <= len; i++)
	{
		fprintf(fout, "(%d)\t", i);
		if (FourPart_Table[i - 1].type == ASSIGN)
		{
			fprintf(fout, "< =,");
			if (FourPart_Table[i - 1].left->name != "")
			{
				fprintf(fout, "%s,_,", FourPart_Table[i - 1].left->name.data());
			}
			else if (FourPart_Table[i - 1].left->datatype == NUM)
			{
				fprintf(fout, "%d,_,", FourPart_Table[i - 1].left->get_numvalue());
			}
			else if (FourPart_Table[i - 1].left->datatype == FLOAT)
			{
				fprintf(fout, "%f,_,", FourPart_Table[i - 1].left->get_floatvalue());
			}
			fprintf(fout, "%s >\n", FourPart_Table[i - 1].right->name.data());
		}
		else if (FourPart_Table[i - 1].type == BOOL)
		{
			fprintf(fout, "< %s,", FourPart_Table[i - 1].op.data());
			if (FourPart_Table[i - 1].left->name == "")
			{
				if (FourPart_Table[i - 1].left->datatype == NUM)
				{
					fprintf(fout, "%d,", FourPart_Table[i - 1].left->get_numvalue());
				}
				else if (FourPart_Table[i - 1].left->datatype == FLOAT)
				{
					fprintf(fout, "%f,", FourPart_Table[i - 1].left->get_floatvalue());
				}
			}
			else
			{
				fprintf(fout, "%s,", FourPart_Table[i - 1].left->name.data());
			}
			if (FourPart_Table[i - 1].right->name == "")
			{
				if (FourPart_Table[i - 1].right->datatype == NUM)
				{
					fprintf(fout, "%d,", FourPart_Table[i - 1].right->get_numvalue());
				}
				else if (FourPart_Table[i - 1].right->datatype == FLOAT)
				{
					fprintf(fout, "%f,", FourPart_Table[i - 1].right->get_floatvalue());
				}
			}
			else
			{
				fprintf(fout, "%s,", FourPart_Table[i - 1].right->name.data());
			}
			fprintf(fout, "%s >\n", Temp_Table[FourPart_Table[i - 1].tempsig]->name.data());
		}
		else if (FourPart_Table[i - 1].type == NOT)
		{
			fprintf(fout, "< !,%s,_,%s >\n", FourPart_Table[i - 1].left->name.data(), FourPart_Table[i - 1].strtemp->name.data());
		}
		else if (FourPart_Table[i - 1].type == JUMP)
		{
			fprintf(fout, "< j,_,_,%d >\n", FourPart_Table[i - 1].jumpnum);
		}
		else if (FourPart_Table[i - 1].type == JUMPZ)
		{
			fprintf(fout, "< jz,%s,_,%d >\n", FourPart_Table[i - 1].left->name.data(), FourPart_Table[i - 1].jumpnum);
		}
		else if (FourPart_Table[i - 1].type == JUMPNZ)
		{
			fprintf(fout, "< jnz,%s,_,%d >\n", FourPart_Table[i - 1].left->name.data(), FourPart_Table[i - 1].jumpnum);
		}
		else if (FourPart_Table[i - 1].type == ADD)
		{
			float tv;
			fprintf(fout, "< +,");
			if (FourPart_Table[i - 1].left->name == "")
			{
				tv = (FourPart_Table[i - 1].left->datatype == NUM) ? FourPart_Table[i - 1].left->get_numvalue() : FourPart_Table[i - 1].left->get_floatvalue();
				fprintf(fout, "%f,", tv);
			}
			else
			{
				fprintf(fout, "%s,", FourPart_Table[i - 1].left->name.data());
			}
			if (FourPart_Table[i - 1].right->name == "")
			{
				tv = (FourPart_Table[i - 1].right->datatype == NUM) ? FourPart_Table[i - 1].right->get_numvalue() : FourPart_Table[i - 1].right->get_floatvalue();
				fprintf(fout, "%f,", tv);
			}
			else
			{
				fprintf(fout, "%s,", FourPart_Table[i - 1].right->name.data());
			}
			fprintf(fout, "%s >\n", Temp_Table[FourPart_Table[i - 1].tempsig]->name.data());
		}
		else if (FourPart_Table[i - 1].type == SUB)
		{
			float tv;
			fprintf(fout, "< -,");
			if (FourPart_Table[i - 1].left->name == "")
			{
				tv = (FourPart_Table[i - 1].left->datatype == NUM) ? FourPart_Table[i - 1].left->get_numvalue() : FourPart_Table[i - 1].left->get_floatvalue();
				fprintf(fout, "%f,", tv);
			}
			else
			{
				fprintf(fout, "%s,", FourPart_Table[i - 1].left->name.data());
			}
			if (FourPart_Table[i - 1].right->name == "")
			{
				tv = (FourPart_Table[i - 1].right->datatype == NUM) ? FourPart_Table[i - 1].right->get_numvalue() : FourPart_Table[i - 1].right->get_floatvalue();
				fprintf(fout, "%f,", tv);
			}
			else
			{
				fprintf(fout, "%s,", FourPart_Table[i - 1].right->name.data());
			}
			fprintf(fout, "%s >\n", Temp_Table[FourPart_Table[i - 1].tempsig]->name.data());
		}
		else if (FourPart_Table[i - 1].type == PLUS)
		{
			float tv;
			fprintf(fout, "< *,");
			if (FourPart_Table[i - 1].left->name == "")
			{
				tv = (FourPart_Table[i - 1].left->datatype == NUM) ? FourPart_Table[i - 1].left->get_numvalue() : FourPart_Table[i - 1].left->get_floatvalue();
				fprintf(fout, "%f,", tv);
			}
			else
			{
				fprintf(fout, "%s,", FourPart_Table[i - 1].left->name.data());
			}
			if (FourPart_Table[i - 1].right->name == "")
			{
				tv = (FourPart_Table[i - 1].right->datatype == NUM) ? FourPart_Table[i - 1].right->get_numvalue() : FourPart_Table[i - 1].right->get_floatvalue();
				fprintf(fout, "%f,", tv);
			}
			else
			{
				fprintf(fout, "%s,", FourPart_Table[i - 1].right->name.data());
			}
			fprintf(fout, "%s >\n", Temp_Table[FourPart_Table[i - 1].tempsig]->name.data());
		}
		else if (FourPart_Table[i - 1].type == ABST)
		{
			float tv;
			fprintf(fout, "< /,");
			if (FourPart_Table[i - 1].left->name == "")
			{
				tv = (FourPart_Table[i - 1].left->datatype == NUM) ? (FourPart_Table[i - 1].left->get_numvalue()) : (FourPart_Table[i - 1].left->get_floatvalue());
				fprintf(fout, "%f,", tv);
			}
			else
			{
				fprintf(fout, "%s,", FourPart_Table[i - 1].left->name.data());
			}
			if (FourPart_Table[i - 1].right->name == "")
			{
				tv = (FourPart_Table[i - 1].right->datatype == NUM) ? FourPart_Table[i - 1].right->get_numvalue() : FourPart_Table[i - 1].right->get_floatvalue();
				fprintf(fout, "%f,", tv);
			}
			else
			{
				fprintf(fout, "%s,", FourPart_Table[i - 1].right->name.data());
			}
			fprintf(fout, "%s >\n", Temp_Table[FourPart_Table[i - 1].tempsig]->name.data());
		}
	}
	fclose(fout);
}
void Build_FourPart_Table_()
{
	FourPart_ value;
	int len = FourPart_Table.size();
	int i = 1;
	for (; i <= len; i++)
	{
		//第一类
		if (FourPart_Table[i - 1].type == ASSIGN)
		{
			value.type = 1;
			value.command = "=";
			value.op1 = FourPart_Table[i - 1].left;
			value.dst = FourPart_Table[i - 1].right;
		}
		else if (FourPart_Table[i - 1].type == NOT)
		{
			value.type = 1;
			value.command = "!";
			value.op1 = FourPart_Table[i - 1].left;
			value.dst = FourPart_Table[i - 1].strtemp;
		}
		//第二类
		else if (FourPart_Table[i - 1].type == ADD)
		{
			value.type = 2;
			value.command = "+";
			value.op1 = FourPart_Table[i - 1].left;
			value.op2 = FourPart_Table[i - 1].right;
			value.dst = Temp_Table[FourPart_Table[i - 1].tempsig];
		}
		else if (FourPart_Table[i - 1].type == SUB)
		{
			value.type = 2;
			value.command = "-";
			value.op1 = FourPart_Table[i - 1].left;
			value.op2 = FourPart_Table[i - 1].right;
			value.dst = Temp_Table[FourPart_Table[i - 1].tempsig];
		}
		else if (FourPart_Table[i - 1].type == PLUS)
		{
			value.type = 2;
			value.command = "*";
			value.op1 = FourPart_Table[i - 1].left;
			value.op2 = FourPart_Table[i - 1].right;
			value.dst = Temp_Table[FourPart_Table[i - 1].tempsig];
		}
		else if (FourPart_Table[i - 1].type == ABST)
		{
			value.type = 2;
			value.command = "/";
			value.op1 = FourPart_Table[i - 1].left;
			value.op2 = FourPart_Table[i - 1].right;
			value.dst = Temp_Table[FourPart_Table[i - 1].tempsig];
		}
		else if (FourPart_Table[i - 1].type == BOOL)
		{
			value.type = 2;
			value.command = FourPart_Table[i - 1].op;
			value.op1 = FourPart_Table[i - 1].left;
			value.op2 = FourPart_Table[i - 1].right;
			value.dst = Temp_Table[FourPart_Table[i - 1].tempsig];
		}
		//第三类跳转
		else if (FourPart_Table[i - 1].type == JUMP)
		{
			value.type = 3;
			value.command = "j";
			value.jump = FourPart_Table[i - 1].jumpnum;
		}
		else if (FourPart_Table[i - 1].type == JUMPZ)
		{
			value.type = 3;
			value.command = "jz";
			value.jump = FourPart_Table[i - 1].jumpnum;
			value.op1 = FourPart_Table[i - 1].left;
		}
		else if (FourPart_Table[i - 1].type == JUMPNZ)
		{
			value.type = 3;
			value.command = "jnz";
			value.jump = FourPart_Table[i - 1].jumpnum;
			value.op1 = FourPart_Table[i - 1].left;
		}
		FourPart_Table_.push_back(value);
	}
	cout << "FourPart_Table has been builded!" << endl;
}

void Renew_FourPart_Table_(DataItem &data)
{
	int i;
	int len = FourPart_Table_.size();
	for (i = 1; i <= len; i++)
	{
		int flag = (FourPart_Table_[i - 1].op1->name == "") ? 1 : 0;
		if (!flag)
		{
			if (data.name == FourPart_Table_[i - 1].op1->name)
			{
				if (FourPart_Table_[i - 1].op1->datatype == NUM && data.datatype == NUM)
				{
					FourPart_Table_[i - 1].op1->set_numvalue(data.get_numvalue());
				}
				else
				{
					FourPart_Table_[i - 1].op1->set_floatvalue(data.get_floatvalue());
				}
			}
			if (data.name == FourPart_Table_[i - 1].op2->name)
			{
				if (FourPart_Table_[i - 1].op2->datatype == NUM && data.datatype == NUM)
				{
					FourPart_Table_[i - 1].op2->set_numvalue(data.get_numvalue());
				}
				else
				{
					FourPart_Table_[i - 1].op2->set_floatvalue(data.get_floatvalue());
				}
			}
		}
	}
}

void Build_Variable_Tables(const char* filename)
{
	vector<DataItem*>::iterator find_index;  //指定变量在变量表或中间变量表的指针
	int data_index;  //指定变量在变量表或中间变量表的索引
	int len = FourPart_Table_.size();
	int i = 1;   //四元组序号
	int j = 1;	 //全变量序号
	fout = fopen(filename, "w+");
	while (i <= len)
	{
		//
		if (FourPart_Table_[i - 1].type == 1)
		{
			find_index = find(Variable_Table.begin(), Variable_Table.end(), FourPart_Table_[i - 1].dst);
			if (find_index == Variable_Table.end())    //未在全变量表中找到，查找声明变量表
			{
				find_index = find(Data_Table.begin(), Data_Table.end(), FourPart_Table_[i - 1].dst);
				if (find_index == Data_Table.end())    //未在声明变量表中找到，查找中间变量表
				{
					find_index = find(Temp_Table.begin(), Temp_Table.end(), FourPart_Table_[i - 1].dst);
					if (find_index != Temp_Table.end())   //在中间变量表中找到
					{
						data_index = distance(Temp_Table.begin(), find_index);
						Variable_Table.push_back(Temp_Table[data_index]);
						fprintf(fout, "Line %d\t生成中间变量:%s = ", i, Variable_Table[j - 1]->name.data());
						if (Variable_Table[j - 1]->datatype == NUM)
						{
							fprintf(fout, "%d\t\n", Variable_Table[j - 1]->get_numvalue());
						}
						else
						{
							fprintf(fout, "%f\t\n", Variable_Table[j - 1]->get_floatvalue());
						}
						j++;
					}
				}
				else  // 在声明变量表中找到
				{
					data_index = distance(Data_Table.begin(), find_index);
					Variable_Table.push_back(Data_Table[data_index]);
					fprintf(fout, "Line %d\t生成声明变量:%s = ", i, Variable_Table[j - 1]->name.data());
					if (Variable_Table[j - 1]->datatype == NUM)
					{
						fprintf(fout, "%d\t\n", Variable_Table[j - 1]->get_numvalue());
					}
					else
					{
						fprintf(fout, "%f\t\n", Variable_Table[j - 1]->get_floatvalue());
					}
					j++;
				}
			}
			else  // 在全变量表中找到 
			{
				data_index = distance(Variable_Table.begin(), find_index);
				if (FourPart_Table_[i - 1].command == "=")
				{
					if (Variable_Table[data_index]->datatype == NUM)
					{
						Variable_Table[data_index]->set_numvalue(FourPart_Table_[i - 1].op1->get_numvalue());
					}
					else if (FourPart_Table_[i - 1].op1->datatype == FLOAT)
					{
						Variable_Table[data_index]->set_floatvalue(FourPart_Table_[i - 1].op1->get_floatvalue());
					}
					else Variable_Table[data_index]->set_floatvalue(FourPart_Table_[i - 1].op1->get_numvalue());
				}
				fprintf(fout, "Line %d\t更新变量:%s = ", i, Variable_Table[data_index]->name.data());
				if (Variable_Table[data_index]->datatype == NUM)
				{
					fprintf(fout, "%d\t\n", Variable_Table[data_index]->get_numvalue());
				}
				else
				{
					fprintf(fout, "%f\t\n", Variable_Table[data_index]->get_floatvalue());
				}
				Renew_FourPart_Table_(*Variable_Table[data_index]);
			}
			i++;
		}
		else if (FourPart_Table_[i - 1].type == 2) //计算只可能生成或更新中间变量
		{
			find_index = find(Variable_Table.begin(), Variable_Table.end(), FourPart_Table_[i - 1].dst);
			if (find_index == Variable_Table.end())    //未在全变量表中找到，查找中间变量表
			{
				find_index = find(Temp_Table.begin(), Temp_Table.end(), FourPart_Table_[i - 1].dst);
				if (find_index != Temp_Table.end())   //在中间变量表中找到
				{
					data_index = distance(Temp_Table.begin(), find_index);
					Variable_Table.push_back(Temp_Table[data_index]);

					if (FourPart_Table_[i - 1].command == ">")
					{
						float op1_value;
						float op2_value;
						op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
						op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
						if (op1_value > op2_value)
						{
							Variable_Table[j - 1]->set_numvalue(1);
						}
						else Variable_Table[j - 1]->set_numvalue(0);
					}
					else if (FourPart_Table_[i - 1].command == "<")
					{
						float op1_value;
						float op2_value;
						op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
						op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
						if (op1_value < op2_value)
						{
							Variable_Table[j - 1]->set_numvalue(1);
						}
						else Variable_Table[j - 1]->set_numvalue(0);
					}
					else if (FourPart_Table_[i - 1].command == ">=")
					{
						float op1_value;
						float op2_value;
						op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
						op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
						if (op1_value >= op2_value)
						{
							Variable_Table[j - 1]->set_numvalue(1);
						}
						else Variable_Table[j - 1]->set_numvalue(0);
					}
					else if (FourPart_Table_[i - 1].command == "<=")
					{
						float op1_value;
						float op2_value;
						op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
						op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
						if (op1_value <= op2_value)
						{
							Variable_Table[j - 1]->set_numvalue(1);
						}
						else Variable_Table[j - 1]->set_numvalue(0);
					}
					else if (FourPart_Table_[i - 1].command == "+")
					{
						float op1_value;
						float op2_value;
						op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
						op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
						if (Variable_Table[j - 1]->datatype == NUM)
						{
							Variable_Table[j - 1]->set_numvalue(int(op1_value + op2_value));
						}
						else Variable_Table[j - 1]->set_floatvalue(op1_value + op2_value);
					}
					else if (FourPart_Table_[i - 1].command == "-")
					{
						float op1_value;
						float op2_value;
						op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
						op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
						if (Variable_Table[j - 1]->datatype == NUM)
						{
							Variable_Table[j - 1]->set_numvalue(int(op1_value - op2_value));
						}
						else
						{
							Variable_Table[j - 1]->set_floatvalue(op1_value - op2_value);
						}

					}
					else if (FourPart_Table_[i - 1].command == "*")
					{
						float op1_value;
						float op2_value;
						op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
						op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
						cout << op1_value << endl;
						cout << op2_value << endl;
						if (Variable_Table[j - 1]->datatype == NUM)
						{
							Variable_Table[j - 1]->set_numvalue(int(op1_value * op2_value));
						}
						else Variable_Table[j - 1]->set_floatvalue(op1_value * op2_value);
					}
					else if (FourPart_Table_[i - 1].command == "/")
					{
						float op1_value;
						float op2_value;
						op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
						op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
						if (Variable_Table[j - 1]->datatype == NUM)
						{
							Variable_Table[j - 1]->set_numvalue(int(op1_value / op2_value));
						}
						else Variable_Table[j - 1]->set_floatvalue(op1_value / op2_value);
					}
					fprintf(fout, "Line %d\t生成中间变量:%s = ", i, Variable_Table[j - 1]->name.data());

					if (Variable_Table[j - 1]->datatype == NUM)
					{
						fprintf(fout, "%d\t\n", Variable_Table[j - 1]->get_numvalue());
					}
					else
					{
						fprintf(fout, "%f\t\n", Variable_Table[j - 1]->get_floatvalue());
					}
					j++;

				}
			}
			else  // 在全变量表中找到 
			{
				data_index = distance(Variable_Table.begin(), find_index);
				if (FourPart_Table_[i - 1].command == ">")
				{
					float op1_value;
					float op2_value;
					op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
					op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
					if (op1_value > op2_value)
					{
						Variable_Table[data_index]->set_numvalue(1);
					}
					else Variable_Table[data_index]->set_numvalue(0);
				}
				else if (FourPart_Table_[i - 1].command == "<")
				{
					float op1_value;
					float op2_value;
					op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
					op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
					if (op1_value < op2_value)
					{
						Variable_Table[data_index]->set_numvalue(1);
					}
					else Variable_Table[data_index]->set_numvalue(0);
				}
				else if (FourPart_Table_[i - 1].command == ">=")
				{
					float op1_value;
					float op2_value;
					op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
					op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
					if (op1_value >= op2_value)
					{
						Variable_Table[data_index]->set_numvalue(1);
					}
					else Variable_Table[data_index]->set_numvalue(0);
				}
				else if (FourPart_Table_[i - 1].command == "<=")
				{
					float op1_value;
					float op2_value;
					op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
					op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
					if (op1_value <= op2_value)
					{
						Variable_Table[data_index]->set_numvalue(1);
					}
					else Variable_Table[data_index]->set_numvalue(0);
				}
				else if (FourPart_Table_[i - 1].command == "+")
				{
					float op1_value;
					float op2_value;
					op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
					op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
					if (Variable_Table[data_index]->datatype == NUM)
					{
						Variable_Table[data_index]->set_numvalue(int(op1_value + op2_value));
					}
					else Variable_Table[data_index]->set_floatvalue(op1_value + op2_value);
				}
				else if (FourPart_Table_[i - 1].command == "-")
				{
					float op1_value;
					float op2_value;
					op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
					op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
					if (Variable_Table[data_index]->datatype == NUM)
					{
						Variable_Table[data_index]->set_numvalue(int(op1_value - op2_value));
					}
					else Variable_Table[data_index]->set_floatvalue(op1_value - op2_value);
				}
				else if (FourPart_Table_[i - 1].command == "*")
				{
					float op1_value;
					float op2_value;
					op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
					op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
					if (Variable_Table[data_index]->datatype == NUM)
					{
						Variable_Table[data_index]->set_numvalue(int(op1_value * op2_value));
					}
					else Variable_Table[data_index]->set_floatvalue(op1_value * op2_value);
				}
				else if (FourPart_Table_[i - 1].command == "/")
				{
					float op1_value;
					float op2_value;
					op1_value = (FourPart_Table_[i - 1].op1->datatype == NUM) ? FourPart_Table_[i - 1].op1->get_numvalue() : FourPart_Table_[i - 1].op1->get_floatvalue();
					op2_value = (FourPart_Table_[i - 1].op2->datatype == NUM) ? FourPart_Table_[i - 1].op2->get_numvalue() : FourPart_Table_[i - 1].op2->get_floatvalue();
					if (Variable_Table[data_index]->datatype == NUM)
					{
						Variable_Table[data_index]->set_numvalue(int(op1_value / op2_value));
					}
					else Variable_Table[data_index]->set_floatvalue(op1_value / op2_value);
				}
				fprintf(fout, "Line %d\t更新变量:%s = ", i, Variable_Table[data_index]->name.data());
				if (Variable_Table[data_index]->datatype == NUM)
				{
					fprintf(fout, "%d\t\n", Variable_Table[data_index]->get_numvalue());
				}
				else
				{
					fprintf(fout, "%f\t\n", Variable_Table[data_index]->get_floatvalue());
				}
				Renew_FourPart_Table_(*Variable_Table[data_index]);
			}
			i++;
		}
		else if (FourPart_Table_[i - 1].type == 3)
		{
			float tv;
			if (FourPart_Table_[i - 1].op1->datatype == NUM)
			{
				tv = FourPart_Table_[i - 1].op1->get_numvalue();
			}
			else tv = FourPart_Table_[i - 1].op1->get_floatvalue();
			if (FourPart_Table_[i - 1].command == "jz") //假转
			{
				if (!tv)
				{
					fprintf(fout, "Line %d\t执行假转指令,跳转到Line %d\n", i, FourPart_Table_[i - 1].jump);
					i = FourPart_Table_[i - 1].jump;
				}
				else
				{
					fprintf(fout, "Line %d\t假转指令无效,顺序执行下一条命令\n", i);
					i++;
				}
			}
			else if (FourPart_Table_[i - 1].command == "jnz") //真转
			{
				if (tv)
				{
					fprintf(fout, "Line %d\t执行真转指令,跳转到Line %d\n", i, FourPart_Table_[i - 1].jump);
					i = FourPart_Table_[i - 1].jump;
				}
				else
				{
					fprintf(fout, "Line %d\t真转指令无效,顺序执行下一条命令\n", i);
					i++;
				}
			}
			else //直转
			{
				fprintf(fout, "Line %d\t执行直转指令,跳转到Line %d\n", i, FourPart_Table_[i - 1].jump);
				i = FourPart_Table_[i - 1].jump;
			}
		}
	}
	fprintf(fout, " Interpretive Routine Complete!\n");
	fclose(fout);
}

void Print_Variable_Tables(const char* filename)
{
	fout = fopen(filename, "w+");
	int len = Variable_Table.size();
	int i = 1;
	for (; i <= len; i++)
	{
		fprintf(fout, "%s\t", Variable_Table[i - 1]->name.data());

		if (Variable_Table[i - 1]->datatype == NUM)
		{
			fprintf(fout, "NUM\t");
			fprintf(fout, "%d\t", Variable_Table[i - 1]->get_numvalue());
		}
		else if (Variable_Table[i - 1]->datatype == FLOAT)
		{
			fprintf(fout, "FLOAT\t");
			fprintf(fout, "%f\t", Variable_Table[i - 1]->get_floatvalue());
		}
		fprintf(fout, "\n");
	}
	fclose(fout);
}
//////////////////////////////////////////////////////////////////////////////////主函数在这里

char SourceFilePath[100] = "test.txt";
char LexicalFilePath[100] = "runtime\\LexicalResult.txt";
char SemanticFilePath[100] = "runtime\\SemanticResult.txt";
char MiddleCodeFilePath[100] = "runtime\\MidCode.txt";
char Var_TableFilePath[100] = "runtime\\VarTable.txt";

//char LexicalFilePath[100] = "LexicalResult.txt";
//char SemanticFilePath[100] = "SemanticResult.txt";
//char MiddleCodeFilePath[100] = "MidCode.txt";
//char Var_TableFilePath[100] = "VarTable.txt";
int main(int argc, char** argv)
{
	//printf("count: %d\n", argc);
	//printf("argv0:%s\n", argv[0]);
	//printf("argv1:%s\n", argv[1]);
	strcpy_s(SourceFilePath, argv[1]);
	LexemeAnalysis(SourceFilePath);
	if (!lexical_error)
	{
		OutLexicalResult(LexicalFilePath);
	}
	else
	{
		return -1;
	}
	if (flag == 1)
	{
		ParserAnalysis();
	}
	else
	{
		return -2;
	}
	if ((!sen_error) && (flag == 1))
	{
		OutSemanticResult(SemanticFilePath);
		//show_Data_Table();
		//show_FourPart_Table();
		//show_Temp_Table();
		Build_FourPart_Table_();
		Build_Variable_Tables(MiddleCodeFilePath);
		Print_Variable_Tables(Var_TableFilePath);
	}
	else
	{
		return -3;
	}
//#define DEBUG
#ifdef DEBUG
	_getch();
#endif // DEBUG

	return 0;
}