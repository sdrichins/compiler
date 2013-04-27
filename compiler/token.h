#pragma once
using namespace std;
class token
{
public:
	string ltype;
	string lexeme;
	int linenum;
	token(string lex, string t, int ln)
	{
		ltype = t;
		lexeme = lex;
		linenum = ln;
	}
	string getType()
	{
		return ltype;
	}
	string getLexim()
	{
		return lexeme;
	}
	string getLine()
	{
		string Result;
		ostringstream convert;
		convert << linenum;
		Result = convert.str();
		return Result;
	}
	~token(void)
	{
	}
	const string toString()
	{
		return "Type: " + ltype + " lexeme: " + lexeme + "\n";
	}
	bool isexpz()
	{
		if(lexeme == "=" || lexeme == "&&" || lexeme == "||" || lexeme == "=="
			|| lexeme == "!=" || lexeme == "<=" || lexeme == ">=" || lexeme == "<"
			|| lexeme == ">" || lexeme == "+" || lexeme == "-" || lexeme == "*"
			|| lexeme == "/")
			return true;
		else
			return false;
	}
	bool isexp()
	{
		if(lexeme == "(" || lexeme == "true" || lexeme == "false" || lexeme == "null" || ltype == "number" || ltype == "character" || ltype == "identifier")
			return true;
		else
			return false;
	}
};