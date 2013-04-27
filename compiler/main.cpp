#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <vector>
//#include <hash_map>
#include <stack>
#include "token.h"
#include "symbol.h"
#include "symtab.h"
#include "sar.h"
#include "opr.h"

using namespace std;
ifstream myfile;
token tok(" "," ",0);
token nexttok(" "," ",0);
char line[35];
char *mytok;
bool incomment = false;
int linenum = 0;
void nextToken();
token peekToken();
token getToken();
string modifier();
void compilation_unit();
void class_declaration();
void class_member_declaration();
void field_declaration(string,string,string);
void constructor_declaration();
void method_body();
void variable_declaration();
void parameter_list();
void parameter();
void expression();
void statement();
void expressionz();
void assignment_expression();
void new_declaration();
string type();
string class_name();
string number();
void argument_list();
void numeric_literal();
void character_literal ();
string identifier();
void fn_arr_member();
void member_refz();
string ipush(string);
string lpush(string);
string opush(string);
string tpush(string);
string vpush(string);
string iexist(string);
string rexists(string);
string texists (string);
string bal();
string eal();
string func();
string arrr();
string ifr();
string whiler();
string returnr();
string coutr();
string cinr();
string atoir();
string itoar();
string newobj();
string newarr();
string cd(string,string);
string cp();
string cb();
string com();
string eoe();
string addop();
string subop();
string multop();
string divop();
string assop();
string modop();
string ltop();
string gtop();
string eqop();
string lteop();
string gteop();
string andop();
string orop();
string neop();
string tcodegen();
string sstate();
string lstate();
string keys[] ={"if","atoi","bool","class","char","cin","cout"
            ,"else","false","int","itoa","main","new","null","object","public","private","return"
            ,"string","this","true","void","while"};
vector<string> keywords( keys, keys + sizeof(keys) / sizeof(keys[0]));
string scope = "g";
int sid = 100;
int tid = 1;
int lblid = 1;
symtab stable;
vector<string> params;
stack<sar> SAS;
stack<opr> OS;
bool first = true;
bool second = false;
bool err = false;
vector<vector <string> > quad;
vector<vector <string> > locals;
int quadtop = 0;
int size = 0;
string symid(string s)
{
	string Res;
	ostringstream convert;
	convert << sid;
	Res = convert.str();
	sid++;
	return s+Res;
}
string gentemp()
{
	string Res;
	ostringstream convert;
	convert << tid;
	Res = convert.str();
	tid++;
	return "t"+Res;
}
string lblgen()
{
	string Res;
	ostringstream convert;
	convert << lblid;
	Res = convert.str();
	lblid++;
	return Res;
}
string inttostring(int x)
{
	ostringstream ss;
     ss << x;
     return ss.str();
}
int stringtoint(string x)
{
	int number;
	istringstream ss(x);
     ss >> number;
     return number;
}
map<string,vector<string> > reg;
vector<vector<string> > tcode;
string getregister()
{
    map<string,vector<string> >::iterator mi;
    for(mi = reg.begin(); mi != reg.end(); ++mi)
    {
        if(mi->second.size() == 0)
            return mi->first;
    }
    return "registers are full";
}
string getlocation(string x)
{
    string result;
    map<string,vector<string> >::iterator mi;
    for(mi = reg.begin(); mi != reg.end(); ++mi)
    {
        for(int i = 0; i < mi->second.size(); i ++)
            if(mi->second[i] == x)
                return mi->first;
    }
    symbol y = stable.getsym(x, "g");
    if(y.scope == "g")
        return y.symid;
    else
    {
        return inttostring(y.off);
    }
	if(result.size() == 0)
		return "not found";
    return result;
}
void backpatch(string oldlable, string newlabel)
{
    for(int i = quad.size() -1; i > -1; i--)
    {
        for(int j = 0; j< quad[i].size();j++)
        {
            if ( quad[i][j] == oldlable)
                quad[i][j] = newlabel;
        }
    }
}

int main()
{
    vector<string> asdf;
    reg["R5"] = asdf;
    reg["R6"] = asdf;
    reg["R7"] = asdf;
    reg["R8"] = asdf;
    reg["R9"] = asdf;
    reg["R10"] = asdf;
	string file = "DemoCV2.kxi";
//	cout << "Enter a filename:";
//	cin >> file;
	myfile.open(file);
	if (!myfile.is_open())
		{
			cout <<"\nFile could not be opened"<<endl;
			cin.get();
			return 0;
		}
	else
	{
		string x;
		cout <<"\n File was found and opened\n";
		nextToken();
		nextToken();
		compilation_unit();
        if(err == false)
        {
            myfile.clear();
            myfile.seekg(0, ios_base::beg);
            second = true;
            first = false;
            linenum = 0;
            scope = "g";
            nextToken();
            nextToken();
            vector<string> tmp;
            tmp.push_back("");
            tmp.push_back("JMP");
            tmp.push_back("Main");
            quad.push_back(tmp);
            compilation_unit();
        }
        if(err == false)
        {
            tcodegen();
        }
		//nextToken();
		//while(!myfile.eof())
		//{
		//nextToken();
		//token mine = getToken();
		//cout << mine.toString();
		//}
		cout <<"Done"<<endl;
		ofstream toutfile;
        ofstream outfile;
        outfile.open("icode.asm");
        for(int i = 0;i < quad.size();i++)
        {
            for(int j = 0; j < quad[i].size(); j++)
            {
                outfile << quad[i][j] + ", ";
            }
            outfile << "\n";
        }
        outfile.close();
		toutfile.open("tcode.asm");
		for(int i = 0; i < tcode.size(); i++)
		{
			for(int j = 0; j < tcode[i].size(); j++)
			{
				toutfile << tcode[i][j] + " ";
			}
			toutfile << "\n";
		}
		toutfile.close();
        cout << "ASM written" << endl;
	}
}

token getToken()
{
	return tok;
}
token peekToken()
{
	return nexttok;
}
void nextToken()
{
	tok = nexttok;
	if(nexttok.getType() != "EOF")
	{	
		char s,t;
		string tmpname,tmpstr;
		if(myfile.get(s))
		{
			tmpname = s;
			t = myfile.peek();
			if(isalpha(s))
			{
				while(isalnum(t))
				{
					myfile.get(t);
					tmpstr = t;
					tmpname += tmpstr;
					t = myfile.peek();
				}
				bool found = false;
				for(int i = 0; i < keywords.size();i++)
				{
					if (keywords[i] == tmpname)
					{
						found = true;
					}
				}
				if(found == true)
				{
					token tmptok(tmpname,tmpname,linenum);
					nexttok = tmptok;
				}
				else
				{
					token tmptok(tmpname,"identifier",linenum);
					nexttok = tmptok;
				}
			}
			else if(isdigit(s))
			{
				while(isdigit(t))
				{
					myfile.get(t);
					tmpstr = t;
					tmpname += tmpstr;
					t = myfile.peek();
				}
				token tmptok(tmpname,"number",linenum);
				nexttok = tmptok;
			}
			else if(ispunct(s))
			{
				if(s == ';')
				{
					token tmptok(tmpname,"EOL",linenum);
					nexttok = tmptok;
				}
				else if(s == '\'')
				{
					if(t=='\\')
					{
						myfile.get(t);
						tmpstr = t;
						tmpname += tmpstr;
						t = myfile.peek();
						if(isalpha(t))
						{
							myfile.get(t);
							tmpstr = t;
							tmpname += tmpstr;
							t = myfile.peek();
							if(t == '\'')
							{
								myfile.get(t);
								tmpstr = t;
								tmpname += tmpstr;
								t = myfile.peek();
								token tmptok(tmpname,"character",linenum);
								nexttok = tmptok;
							}
							else
							{
								token tmptok(tmpname,"unkown",linenum);
								nexttok = tmptok;
							}
						}
						else
						{
								token tmptok(tmpname,"unkown",linenum);
								nexttok = tmptok;
						}
					}
					else if(isprint(t))
					{
						myfile.get(t);
						tmpstr = t;
						tmpname += tmpstr;
						t = myfile.peek();
						if(t == '\'')
						{
							myfile.get(t);
							tmpstr = t;
							tmpname += tmpstr;
							t = myfile.peek();
							token tmptok(tmpname,"character",linenum);
							nexttok = tmptok;
						}
						else
						{
							token tmptok(tmpname,"unkown",linenum);
							nexttok = tmptok;
						}
					}
					else
					{
						token tmptok(tmpname,"unkown",linenum);
						nexttok = tmptok;
					}
					
	
	
				}
				else if(s== '+' || s== '-' || s =='/' || s== '*')
				{
					if(s== '+' || s=='-')
					{
							token tmptok(tmpname,"mathsym",linenum);
							nexttok = tmptok;
					}
					else if(s=='/' && t =='/')
					{
							while(s!= '\n')
								myfile.get(s);
							linenum++;
							nextToken();
					}
					else
					{
						token tmptok(tmpname,"mathsym",linenum);
						nexttok = tmptok;
					}
				}
				else if(s== '<' || s == '>')
				{
					if(t == '=')
					{
						myfile.get(t);
						tmpstr = t;
						tmpname += tmpstr;
						t = myfile.peek();
						token tmptok(tmpname,"relationalsym",linenum);
						nexttok = tmptok;
					}
					else if(s == '<' && t == '<')
					{
						myfile.get(t);
						tmpstr = t;
						tmpname += tmpstr;
						t = myfile.peek();
						token tmptok(tmpname,"IOsym",linenum);
						nexttok = tmptok;
					}
					else if(s == '>' && t == '>')
					{
						myfile.get(t);
						tmpstr = t;
						tmpname += tmpstr;
						t = myfile.peek();
						token tmptok(tmpname,"IOsym",linenum);
						nexttok = tmptok;
					}
					else
					{
						token tmptok(tmpname,"relationalsym",linenum);
						nexttok = tmptok;
					}
				}
				else if(s == '&')
				{
					if(t == '&')
					{
						myfile.get(t);
						tmpstr = t;
						tmpname += tmpstr;
						t = myfile.peek();
						token tmptok(tmpname,"booleansym",linenum);
						nexttok = tmptok;
					}
					else
					{
						token tmptok(tmpname,"punctuation",linenum);
						nexttok = tmptok;
					}
				}
				else if(s == '|')
				{
					if(t == '|')
					{
						myfile.get(t);
						tmpstr = t;
						tmpname += tmpstr;
						t = myfile.peek();
						token tmptok(tmpname,"booleansym",linenum);
						nexttok = tmptok;
					}
					else
					{
						token tmptok(tmpname,"punctuation",linenum);
						nexttok = tmptok;
					}
				}
				else if(s== '=')
				{
					if(t == '=')
					{
						myfile.get(t);
						tmpstr = t;
						tmpname += tmpstr;
						t = myfile.peek();
						token tmptok(tmpname,"relationalsym",linenum);
						nexttok = tmptok;
					}
					else
					{
						token tmptok(tmpname,"assignmentsym",linenum);
						nexttok = tmptok;
					}
				}
				else if(s== '[' || s== ']')
				{
					token tmptok(tmpname,"arraysym",linenum);
					nexttok = tmptok;
				}
				else if(s =='{' || s == '}')
				{
					token tmptok(tmpname,"blocksym",linenum);
					nexttok = tmptok;
				}
				else if(s =='(' || s ==')')
				{
					token tmptok(tmpname,"parensym",linenum);
					nexttok = tmptok;
				}
				else if(s == '!' && t == '=')
				{
					myfile.get(t);
					tmpstr = t;
					tmpname += tmpstr;
					t = myfile.peek();
					token tmptok(tmpname,"relationalsym",linenum);
					nexttok = tmptok;
				}
				else
				{
					token tmptok(tmpname,"puncutation",linenum);
					nexttok = tmptok;
				}
	
			}
			else if(s=='\n')
			{
				linenum++;
				nextToken();
			}
            else if(s=='\r')
            {
                nextToken();
            }
			else if(myfile.eof())
			{
				token tmptok(tmpname,"EOF",linenum);
				nexttok = tmptok;
			}
			else if(s == ' ')
			{
				nextToken();
			}
			else if(s == '\t')
			{
				nextToken();
			}
			else
			{
				token tmptok(tmpname,"Unkown",linenum);
				nexttok = tmptok;
			}
		}
	}
}
string modifier()
{
	token t = getToken();
	if(t.getLexim() == "public" || t.getLexim() == "private")
		nextToken();
	else
		cout << "Invalid modifier: " << t.getLexim() <<" found Line: " <<t.getLine()<<endl;
	string name = t.getLexim();
	return name;
}
string class_name()
{
	string name = identifier();
	return name;
}
string type()
{
	string name;
	token t = getToken();
	if(t.getLexim() == "int" || t.getLexim() == "char" || t.getLexim() == "bool"
		|| t.getLexim() == "void")
	{
		name = t.getLexim();
		nextToken();
	}
	else
		name = class_name();
	if(second)
		tpush(name);
	return name;
}
void character_literal()
{
	token t = getToken();
	string name = t.getLexim();
    if(first)
    {
        symbol s("g",symid("G"),name,"char","char",0);
        s.addData("type: char");
        stable.addsym(s);
    }
	nextToken();
	if(second)
		lpush(name);
}
void numeric_literal()
{
	token t = getToken();
    string sym;
	if(t.getLexim() == "+" || t.getLexim() == "-")
    {
        sym = t.getLexim();
		nextToken();
    }
	string name = sym + number();
    if(first)
    {
        symbol s("g",symid("G"),name,"int","int",0);
        s.addData("type: int");
        stable.addsym(s);
    }
	if(second)
		lpush(name);
}
string number()
{
	token t = getToken();
// 	int i;

	if(isdigit(*t.getLexim().c_str())|| t.getLexim() =="0")
	{
		nextToken();
	}
	else
	{
        err = true;
		cout << "Expected: number Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}
	return t.getLexim();
}
void compilation_unit()
{
	token t = getToken();
	while(true)
	{
		t= getToken();
		if(t.getLexim() == "class")
			class_declaration();
		else
			break;
	}
	t = getToken();
	if(t.getLexim() != "void")
	{
        err = true;
		cout << "Expected: void Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}
	nextToken();
	t = getToken();
	if(t.getLexim() != "main")
	{
        err = true;
		cout << "Expected: main Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}
	nextToken();
	t = getToken();
	if(t.getLexim() != "(")
	{
        err = true;
		cout << "Expected: ( Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}
	nextToken();
	t = getToken();
	if(t.getLexim() != ")")
	{
        err = true;
		cout << "Expected: ) Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}
	nextToken();
	t = getToken();
    scope += ".main";
    if(second)
    {
        vector<string> tmp;
        tmp.push_back("Main");
        quad.push_back(tmp);
    }
	method_body();
    if(second)
    {
        if(quad.back().size() > 1)
        {
            vector<string> tmp;
            tmp.push_back("");
            tmp.push_back("TRP");
            tmp.push_back("0");
            quad.push_back(tmp);
        }
        else
        {
            quad.back().push_back("TRP");
            quad.back().push_back("0");
        }
        for(int i = 0; i < locals.size(); i++)
            quad.push_back(locals[i]);
    }
}
void class_declaration()
{
	token t= getToken();
	if(t.getLexim() != "class")
	{
        err = true;
		cout << "Expected: class Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}
	nextToken();
	t = getToken();
	string name = class_name();
    if(first)
    {
        symbol s(scope,symid("C"),name,"Class","",8);
        stable.addsym(s);
    }
	scope = scope +"."+name;
	t = getToken();
	if(t.getLexim() !="{")
	{
        err = true;
		cout << "Expected: { Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}
	nextToken();
	t = getToken();
	while(true)
	{
		t= getToken();
		if(t.getLexim() == "public" || t.getLexim() =="private" || t.getType()== "identifier")
		{
			class_member_declaration();
		}
		else
			break;
	}
	if(t.getLexim() !="}")
	{
        err = true;
		cout << "Expected: } Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}
	nextToken();
	scope = scope.substr(0,scope.find_last_of('.'));
}
void class_member_declaration()
{
	token t = getToken();
	if(t.getType() == "identifier")
	{
		constructor_declaration();
	}
	else
	{
		string mod =modifier();
		string ty =type();
		if(second)
			texists(ty);
		string name = identifier();
		field_declaration(name,mod,ty);
	}
}
void field_declaration(string name,string mod, string ty)
{
	token t = getToken();
	if(t.getLexim() == "(")
	{
        size = 0;
		nextToken();
		t = getToken();
		symbol s(scope,symid("M"),name,"method",ty,0);
		s.addData("returnType: "+ ty);
		s.addData("accessMod: " +mod);
        scope = scope + "."+name;
        string lbl;
        if(second)
        {
            symbol x = stable.getsym(name, scope);
            lbl = x.symid;
            if(quad.back().size() == 1)
            {
                backpatch(quad.back()[0], lbl);
            }
            else
            {
                vector<string> tmp;
                tmp.push_back(lbl);
                quad.push_back(tmp);
            }
        }
		if(t.getLexim() != ")")
		{
			parameter_list();
			string fparams = "Param: " + params[0];
			for(int i = 1;i<params.size();i++)
			{
				fparams += " " + params[i];
			}
			params.clear();
			s.addData(fparams);
			nextToken();
		}
		else
			nextToken();
        if(first)
        {
            stable.addsym(s);
        }
		t= getToken();
		method_body();
        if(first)
        {
            stable.addSize(s.symid, size);
            size = 0;
        }
        if(second)
        {
            symbol x = stable.getsym(name, scope);
            stable.updateSize(x.symid, size);
            size = 0;
			if(quad.back().size() > 1)
			{
				vector<string> tmp;
				tmp.push_back("");
				tmp.push_back("RTN");
				quad.push_back(tmp);
			}
			else
			{
				quad.back().push_back("RTN");
			}
        }
		scope = scope.substr(0,scope.find_last_of('.'));
	}
	else
	{
		t=getToken();
		if(t.getLexim() == "[")
		{
			nextToken();
			t=getToken();
			if(t.getLexim() != "]")
			{
                err = true;
				cout << "Expected: ] Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
			}
			nextToken();
		}
		t= getToken();
		if(t.getLexim() == "=")
		{
			nextToken();
			if(second)
				opush(t.getLexim());
			assignment_expression();
		}
		t = getToken();
		if(t.getLexim() != ";")
		{
            err = true;
			cout << "Expected: ; Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		nextToken();
		symbol s(scope,symid("V"),name,"ivar",ty,0);
		if(second)
			vpush(s.symid);
		s.addData("type: " +ty);
		s.addData("accessMod: " +mod);
        if(first)
        {
            if(ty == "int")
                size += 4;
            if(ty == "char")
                size += 1;
            stable.addsym(s);
        }
		if(second)
			eoe();
	}
}
void constructor_declaration()
{
	token t = getToken();
	string name = class_name();
	if(second)
		cd(name,scope);
	t = getToken();
	symbol s(scope,symid("M"),name,"Constructor","",0);
	s.addData("returnType: void");
	s.addData("accessMod: public");
	if(t.getLexim() != "(")
	{
        err = true;
		cout << "Expected: ( Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}
	else
	{ 
        size = 0;
		nextToken();
		t = getToken();
        scope = scope + "."+name;
		if(t.getLexim() != ")")
		{
			parameter_list();
			string fparams = "Param: " + params[0];
			for(int i = 1;i<params.size();i++)
			{
				fparams += " " + params[i];
			}
			params.clear();
			//fparams += "]";
			s.addData(fparams);
			nextToken();
		}
		else
			nextToken();
        if (first)
        {
            stable.addsym(s);
        }
		t= getToken();
		method_body();
        if(first)
        {
            stable.addSize(s.symid,size);
            size = 0;
        }
        scope = scope.substr(0,scope.find_last_of('.'));
	}
}
void method_body()
{
	token t = getToken();
	if(t.getLexim() != "{")
	{
        err = true;
		cout << "Expected: { Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}
	nextToken();
	while(true)
	{
		t = getToken();
		token tp = peekToken();
		if(t.getLexim() == "int" || t.getLexim() == "char" || t.getLexim() == "bool"
			|| t.getLexim() == "void" || t.getType() == "identifier")
		{
			if(t.getType() != "identifier")
			{
				variable_declaration();
				t= getToken();
			}
			else if(t.getType() == "identifier" && tp.getType() == "identifier")
			{
				variable_declaration();
				t = getToken();
			}
			else
				break;
		}
		else
			break;
	}
	while(true)
	{
		t = getToken();
		if(t.isexp() || t.getLexim() == "{" || t.getLexim() == "if" || t.getLexim() == "while"
			|| t.getLexim() == "return" || t.getLexim() == "cout" || t.getLexim() == "cin")
		{
			statement();
			t= getToken();
		}
		else
			break;
	}
	t = getToken();
	if(t.getLexim() != "}")
	{
        err = true;
		cout << "Expected: } Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}
	nextToken();
}
void variable_declaration()
{
	token t = getToken();
	string ty =type();
	if(second)
		texists(ty);
	string name = identifier();
	
	t = getToken();
	if(t.getLexim() == "[")
	{
		symbol s(scope,symid("L"),"@"+name,"lvar",ty,0);
		s.addData("type: "+ ty +" arr");
		s.addData("accessMod: private");
		nextToken();
		t = getToken();
		if(t.getLexim() != "]")
		{
            err = true;
			cout << "Expected: ] Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
		}
        if(first)
        {
            stable.addsym(s);
        }
		nextToken();
		if(second)
			vpush(s.symid);
	}
	else
	{
		symbol s(scope,symid("L"),name,"lvar",ty,size+12);
		s.addData("type: "+ ty);
		s.addData("accessMod: private");
        if(first)
        {
            if(ty == "int")
                size += 4;
            if(ty == "char")
                size += 1;
            stable.addsym(s);
        }
		if(second)
        {
            symbol y = stable.getsym(name, scope);
			vpush(y.symid);
            vector<string> tmp;
            tmp.push_back(y.symid);
            if(y.typ == "char")
            {
                tmp.push_back(".BYT");
                tmp.push_back(" ");
            }
            else
            {
                tmp.push_back(".INT");
                tmp.push_back("0");
            }
            locals.push_back(tmp);
            
        }
	}

	t = getToken();
	if(t.getLexim() == "=")
	{
		nextToken();
		if(second)
			opush(t.getLexim());
		assignment_expression();
	}
	t = getToken();
	if(t.getLexim() !=";")
	{
        err = true;
		cout << "Expected: ; Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}
	nextToken();
	if(second)
		eoe();
}
void parameter_list()
{
	parameter();
	token t = getToken();
	while(true)
	{
		t = getToken();
		if(t.getLexim() == ",")
		{
			nextToken();
			parameter();
		}
		else
			break;
	}
}
void parameter()
{
	token t = getToken();
	string ty =type();
	if(second)
		texists(ty);
	string i = identifier();
	t = getToken();
	if(t.getLexim() == "[")
	{
		nextToken();
		if(t.getLexim() !="]")
		{
            err = true;
			cout << "Expected: ] Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		nextToken();
	}
	string id = symid("P");
	symbol s(scope,id,i,"param",ty,12+size);
	s.addData("type: " +ty);
	s.addData("accessMod: private");
    if(first)
    {
        if(ty == "int")
            size += 4;
        if(ty == "char")
            size += 1;
        stable.addsym(s);
    }
	params.push_back(id);
    if(second)
    {
/*        symbol x = stable.getsym(i, scope);
        if(quad.back().size() > 1)
        {
            vector<string> tmp;
            tmp.push_back("");
            tmp.push_back("POP");
            tmp.push_back(x.symid);
            quad.push_back(tmp);
        }
        else
        {
            quad.back().push_back("POP");
            quad.back().push_back(x.symid);
        }
    */}
}
void statement()
{
	token t = getToken();
	if( t.getLexim() == "{")
	{
		nextToken();
		while(true)
		{
			t = getToken();
			if(t.getLexim() == "}")
			{
				nextToken();
				break;
			}
			statement();
		}
	}
	else if(t.getLexim() == "if")
	{
		nextToken();
		t = getToken();
		if(t.getLexim() != "(")
		{
            err = true;
			cout << "expected: ( received: " <<t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		if(second)
			opush(t.getLexim());
		nextToken();
		expression();
		t = getToken();
		if(t.getLexim() != ")")
		{
            err = true;
			cout << "expected: ) received: " <<t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		
		nextToken();
        string lbl;
		if(second)
		{
			cp();
			lbl = ifr();
		}
		statement();
		if(getToken().getLexim() == "else")
		{
            string lbl2;
            if(second)
            {
                lbl2 = "skipelse" + lblgen();
                vector<string> jmp;
                jmp.push_back("");
                jmp.push_back("JMP");
                jmp.push_back(lbl2);
                quad.push_back(jmp);
                if(quad.back().size() == 1)
                {
                    backpatch(quad.back()[0], lbl);
                }
                else
                {
                    vector<string> tmp;
                    tmp.push_back(lbl);
                    quad.push_back(tmp);
                }
            }
			nextToken();
			statement();
            if(second)
            {
                if(quad.back().size() == 1)
                {
                    backpatch(quad.back()[0], lbl2);
                }
                else
                {
                    vector<string> tmp2;
                    tmp2.push_back(lbl2);
                    quad.push_back(tmp2);
                }
            }
		}
        else
        {
            if(second)
            {
                if(quad.back().size() == 1)
                {
                    backpatch(quad.back()[0], lbl);
                }
                else
                {
                    vector<string> tmp2;
                    tmp2.push_back(lbl);
                    quad.push_back(tmp2);
                }
            }
        }
	}
	else if(t.getLexim() == "while")
	{
        string lbl;
        if(second)
        {
            lbl = "beginwhile" + lblgen();
            if(quad.back().size() == 1)
            {
                backpatch(quad.back()[0], lbl);
            }
            else
            {
                vector<string> tmp;
                tmp.push_back(lbl);
                quad.push_back(tmp);
            }
        }
        string lbl2;
		nextToken();
		t = getToken();
		if(t.getLexim() != "(")
		{
            err = true;
			cout << "expected: ( received: " <<t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		if(second)
			opush(t.getLexim());
		nextToken();
		expression();
		t = getToken();
		if(t.getLexim() != ")")
		{
            err = true;
			cout << "expected: ) received: " <<t.getLexim() << " Line: " <<t.getLine() << endl;
		}
        
		if(second)
		{
			cp();
			lbl2 = whiler();
		}
		nextToken();
		statement();
        if(second)
        {
            vector<string> jmp;
			jmp.push_back("");
            jmp.push_back("JMP");
            jmp.push_back(lbl);
            quad.push_back(jmp);
            if(quad.back().size() == 1)
            {
                backpatch(quad.back()[0], lbl);
            }
            else
            {
                vector<string> tmp;
                tmp.push_back(lbl2);
                quad.push_back(tmp);
            }
            
        }
	}
	else if(t.getLexim() == "return")
	{
		nextToken();
		t = getToken();
		if(t.isexp())
			expression();
		t = getToken();
		if(t.getLexim() != ";")
		{
            err = true;
			cout << "expected: ; received: " <<t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		nextToken();
		if(second)
			returnr();
	}
	else if(t.getLexim() == "cout")
	{
		nextToken();
		t = getToken();
		if(t.getLexim() != "<<")
		{
            err = true;
			cout << "expected: << received: " <<t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		nextToken();
		expression();
		t = getToken();
		if(t.getLexim() != ";")
		{
            err = true;
			cout << "expected: ; received: " <<t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		nextToken();
		if(second)
			coutr();
	}
	else if(t.getLexim() == "cin")
	{
		nextToken();
		t = getToken();
		if(t.getLexim() != ">>")
		{
            err = true;
			cout << "expected: << received: " <<t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		nextToken();
		expression();
		t = getToken();
		if(t.getLexim() != ";")
		{
            err = true;
			cout << "expected: ; received: " <<t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		nextToken();
		if(second)
			cinr();
	}
	else
	{
		expression();
		t = getToken();
		if(t.getLexim() != ";")
		{
            err = true;
			cout << "expected: ; received: " <<t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		nextToken();
		if(second)
			eoe();
	}

}
void expression()
{
	token t = getToken();
	if( t.getLexim() == "(")
	{
		if(second)
			opush(t.getLexim());
		nextToken();
		expression();
		t = getToken();
		if(t.getLexim() != ")")
		{
            err = true;
			cout << "expected: ) received: " <<t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		if(second)
			cp();
		nextToken();
		t = getToken();
		if(t.isexpz())
			expressionz();
	}
	else if(t.getLexim() == "true")
	{
		if(second)
			lpush(t.getLexim());
		nextToken();
		if(getToken().isexpz())
			expressionz();
	}
	else if(t.getLexim() == "false")
	{
		if(second)
			lpush(t.getLexim());
		nextToken();
		if(getToken().isexpz())
			expressionz();
	}
	else if(t.getLexim() == "null")
	{
		if(second)
			lpush(t.getLexim());
		nextToken();
		if(getToken().isexpz())
			expressionz();
	}
	else if(t.getType() == "number" || t.getLexim() == "-" || t.getLexim() == "+")
	{
		numeric_literal();
		t = getToken();
		if(getToken().isexpz())
			expressionz();
	}
	else if(t.getType() == "identifier")
	{
		string name =identifier();
		if(second)
			ipush(name);
		if(getToken().getLexim() == "(" || getToken().getLexim() == "[")
			fn_arr_member();
		if(second)
			iexist(name);
		if(getToken().getLexim() == ".")
			member_refz();
		if(getToken().isexpz())
			expressionz();
	}
	else if(t.getType() == "character")
	{
		character_literal();
		t = getToken();
		if(getToken().isexpz())
			expressionz();
	}
}
void fn_arr_member()
{
	token t = getToken();
	if(t.getLexim() == "[")
	{
		if(second)
			opush(t.getLexim());
		nextToken();
		t = getToken();
		expression();
		t = getToken();
		if(t.getLexim() != "]")
		{
            err = true;
			cout << "Expected: ] Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		if(second)
		{
			cb();
			arrr();
		}
		nextToken();
	}
	else if(t.getLexim() == "(")
	{
		if(second)
		{
			opush(t.getLexim());
			bal();
		}
		nextToken();
		t = getToken();
		if(t.isexp())
			argument_list();
		t = getToken();
		if(t.getLexim() != ")")
		{
            err = true;
			cout << "Expected: ) Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		if(second)
		{
			cp();
			eal();
			func();
		}
		nextToken();
	}

}
void argument_list()
{
	expression();
	token t = getToken();
	while(true)
	{
		t = getToken();
		if(t.getLexim() == ",")
		{
			if(second)
				com();
			nextToken();
			expression();
		}
		else
			break;
	}
}
void member_refz()
{
	token t = getToken();
	if(t.getLexim() != ".")
		{
            err = true;
			cout << "Expected: . Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		nextToken();
		string name = identifier();
		if(second)
			ipush(name);
		t = getToken();
		if(t.getLexim() == "(" || t.getLexim() == "[")
		{
			fn_arr_member();
			t = getToken();
		}
		if(second)
			rexists(name);
		if(t.getLexim() == ".")
			member_refz();
}
void expressionz()
{
	token t = getToken();
	if(t.isexpz())
	{
		if(t.getLexim() == "=")
		{
			if(second)
				opush(t.getLexim());
			nextToken();
			assignment_expression();
		}
		else if(t.getLexim() =="&&" || t.getLexim() == "||" || t.getLexim() == "==" || t.getLexim() =="!="
			|| t.getLexim() == "<=" || t.getLexim() == ">=" || t.getLexim() == "<" || t.getLexim() == ">"
			|| t.getLexim() == "+" || t.getLexim() == "-" || t.getLexim() == "*" || t.getLexim() =="/")
		{
			if(second)
				opush(t.getLexim());
			nextToken();
			expression();
		}
	}
}
void assignment_expression()
{
	token t = getToken();
	if(t.getLexim() == "this")
	{
			nextToken();
	}
	else if(t.getLexim() == "new")
	{
			nextToken();
			type();
			new_declaration();
	}
	else if(t.getLexim() == "atoi")
	{
		nextToken();
		t = getToken();
		if(t.getLexim() != "(")
		{
            err = true;
			cout << "Expected: ( Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		if(second)
			opush(t.getLexim());
		nextToken();
		expression();
		t = getToken();
		if(t.getLexim() != ")")
		{
            err = true;
			cout << "Expected: ) Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		if(second)
		{
			cp();
			atoir();
		}
	}
	else if(t.getLexim() == "itoa")
	{
		nextToken();
		t = getToken();
		if(t.getLexim() != "(")
		{
            err = true;
			cout << "Expected: ( Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		if(second)
			opush(t.getLexim());
		nextToken();
		expression();
		t = getToken();
		if(t.getLexim() != ")")
		{
            err = true;
			cout << "Expected: ) Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		if(second)
		{
			cp();
			itoar();
		}
	}
	else
		expression();
}
void new_declaration()
{
	token t = getToken();
	if(t.getLexim() == "(")
	{
		if(second)
		{
			opush(t.getLexim());
			bal();
		}
		nextToken();
		t = getToken();
		if(t.getLexim() == "(" || t.getLexim() =="true" || t.getLexim() == "false" || t.getLexim() == "null"
			|| t.getType() == "number" || t.getType() == "character" || t.getType() == "identifier")
		{
			argument_list();
		}
		t = getToken();
		if(t.getLexim() != ")")
		{
            err = true;
			cout << "Expected: ) Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		if(second)
		{
			cp();
			eal();
			newobj();
		}
		nextToken();
	}
	else if(t.getLexim() == "[")
	{
		if(second)
			opush(t.getLexim());
		nextToken();
		expression();
		t = getToken();
		if(t.getLexim() != "]")
		{
            err = true;
			cout << "Expected: ] Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
		}
		if(second)
		{
			cb();
			newarr();
		}
		nextToken();
	}
	else
	{
        err = true;
		cout << "Expected: ( or [ Recieved: " << t.getLexim() << " Line: " <<t.getLine() << endl;
	}

}
string identifier()
{
	token t = getToken();
	nextToken();
	return t.getLexim();
}
string ipush(string ident)
{
	sar s(ident,"#ipush");
	SAS.push(s);
	return "done";
}
string lpush(string ident)
{
	sar s(ident,"#lpush");
	SAS.push(s);
	return "done";
}
string opush(string opp)
{
	opr o(opp);
    if(opp == "(")
        OS.push(o);
    else if(!OS.empty())
    {
        opr t = OS.top();
        if(t.value == "(")
            OS.push(o);
        else if(o.priority < t.priority)
        {
            OS.pop();
            string op = t.value;
            if(op == "+")
                addop();
            else if(op == "-")
                subop();
            else if(op == "*")
                multop();
            else if(op == "/")
                divop();
            else if(op == "%")
                modop();
            else if(op == "<")
                ltop();
            else if(op == ">")
                gtop();
            else if(op == "<=")
                lteop();
            else if(op == ">=")
                gteop();
            else if(op == "==")
                eqop();
            else if(op == "&&")
                andop();
            else if(op == "||")
                orop();
            else if(op == "!=")
                neop();
            else if(op == "=")
                assop();
        }
        else
            OS.push(o);
    }
    else
        OS.push(o);
	return "done";
}
string tpush(string ty)
{
	sar s(ty,"#tpush",ty);
	SAS.push(s);
	return "done";
}
string iexist(string id)
{
	sar s = SAS.top();
	SAS.pop();
    if(s.isfunc == true)
    {
        string sid = stable.iexistf(scope,s.value, linenum);
        symbol f = stable.getsym(sid, scope);
        string tsym = gentemp();
        symbol t(scope, tsym, f.value, f.kind, f.typ, 0);
        stable.addsym(t);
        sar x(t.symid,"#iexist");
        SAS.push(x);
        if(quad.back().size() > 1)
        {
            vector<string> tmp;
            tmp.push_back("");
            tmp.push_back("FRAME");
            tmp.push_back(sid);
            tmp.push_back("this");
            quad.push_back(tmp);
        }
        else
        {
            quad.back().push_back("FRAME");
            quad.back().push_back(sid);
            quad.back().push_back("this");
        }
        stack<string> args;
        string buff;
        stringstream ss(s.value);
        string z;
        ss >> z;
        vector<string> tmp2;
        while(ss >> buff)
        {
            args.push(buff);
        }
        while(!args.empty())
        {
            tmp2.push_back("");
            tmp2.push_back("PUSH");
            tmp2.push_back(args.top());
            quad.push_back(tmp2);
            args.pop();            
        }
        vector<string> tmp3;
        tmp3.push_back("");
        tmp3.push_back("CALL");
        tmp3.push_back(sid);
        quad.push_back(tmp3);
    }
    else if (s.isarr == true)
    {
        string sid = stable.iexist(scope,s.value, linenum);
        sar x(sid,"#iexist");
        SAS.push(x);
    }
    else
    {
        string sid = stable.iexist(scope,s.value,linenum);
        sar x(sid,"#iexist");
        SAS.push(x);
    }
	return "done";
}
string vpush(string id)
{
	sar x(id,"#vpush");
	SAS.push(x);
	return "done";
}
string rexists(string id)
{
	sar s = SAS.top();
	SAS.pop();
	sar n = SAS.top();
	SAS.pop();
	symbol x = stable.getsym(n.value,scope);
    string sid;
    if(s.isfunc == true)
    {
        sid = stable.rexistf(scope,"g."+x.typ,s.value,linenum);
//        sar y(sid,"#rexist");
//        SAS.push(y);
        if(quad.back().size() > 1)
        {
            vector<string> tmp;
            tmp.push_back("");
            tmp.push_back("FRAME");
            tmp.push_back(sid);
            tmp.push_back(x.symid);
            quad.push_back(tmp);
        }
        else
        {
            quad.back().push_back("FRAME");
            quad.back().push_back(sid);
            quad.back().push_back(x.symid);
        }
        stack<string> args;
        string buff;
        stringstream ss(s.value);
        string z;
        ss >> z;
        
        while(ss >> buff)
        {
            args.push(buff);
        }
        while(!args.empty())
        {
            vector<string> tmp2;
            tmp2.push_back("");
            tmp2.push_back("PUSH");
            tmp2.push_back(args.top());
            quad.push_back(tmp2);
            args.pop();
        }
        vector<string> tmp3;
        tmp3.push_back("");
        tmp3.push_back("CALL");
        tmp3.push_back(sid);
        quad.push_back(tmp3);
    }
    else if (s.isarr == true)
    {
        sid = stable.iexist("g."+x.typ,s.value,linenum);
//        sar y(sid,"#rexist");
//        SAS.push(y);
    }
    else
    {
        sid = stable.iexist("g."+x.typ,s.value,linenum);
//        sar y(sid,"#rexist");
//       SAS.push(y);
    }
    symbol y = stable.getsym(sid, scope);
    string sim = gentemp();
    symbol z(scope, sim, y.value, "#ref", y.typ,0);
    stable.addsym(z);
    sar p(z.symid,"#rexist");
    SAS.push(p);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("PEEK");
        tmp.push_back(z.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("PEEK");
        quad.back().push_back(z.symid);
    }
    
	return "done";
}
string texists(string ty)
{
	sar s = SAS.top();
	SAS.pop();
	if(!stable.texist(s.ty))
        cout << "line: " << linenum <<endl;
	return "done";
}
string bal()
{
	sar s("(","#bal");
	SAS.push(s);
	return "done";
}
string eal()
{
	string al;
	while(SAS.top().value != "(")
	{
		al = SAS.top().value +" " + al;
		SAS.pop();
	}
	SAS.pop();
	sar s(al,"#al");
	SAS.push(s);
	return "done";
}
string func()
{
	sar al = SAS.top();
	SAS.pop();
	sar id = SAS.top();
	SAS.pop();
    
	sar fun(id.value + " " + al.value, "#func","",true);
	SAS.push(fun);
	return "done";
}
string arrr()
{
	sar exp = SAS.top();
	SAS.pop();
    symbol a = stable.getsym(exp.value,scope);
	if(a.typ != "int")
	{
        err = true;
		cout << "Int expected in array refference line:"<< linenum << endl;
	}
	sar id = SAS.top();
	sar ar(id.value,"#arr",id.ty,false,true);
    SAS.push(ar);
	// push arr sar
	return "done";
}
string ifr()
{
	sar exp = SAS.top();
	SAS.pop();
    symbol y = stable.getsym(exp.value,scope);
	if(y.typ != "bool")
    {
        err = true;
		cout << "Bool expected in if line:" << linenum << endl;
    }
    string lbl = "skipif"+lblgen();
    if(quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("BF");
        tmp.push_back(y.symid);
        tmp.push_back(lbl);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("BF");
        quad.back().push_back(y.symid);
        quad.back().push_back(lbl);
    }
	return lbl;
}
string whiler()
{
	sar exp = SAS.top();
	SAS.pop();
    symbol y = stable.getsym(exp.value, scope);
	if(y.typ != "bool")
    {
        err = true;
		cout << "Bool expected in while line:" << linenum << endl;
    }
    string lbl = "endwhile"+lblgen();
    if(quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("BF");
        tmp.push_back(y.symid);
        tmp.push_back(lbl);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("BF");
        quad.back().push_back(y.symid);
        quad.back().push_back(lbl);
    }
	return lbl;
}
string returnr()
{
    while(!OS.empty())
	{
		opr o = OS.top();
		OS.pop();
		string op = o.value;
		if(op == "+")
			addop();
		else if(op == "-")
			subop();
		else if(op == "*")
			multop();
		else if(op == "/")
			divop();
		else if(op == "%")
			modop();
		else if(op == "<")
			ltop();
		else if(op == ">")
			gtop();
		else if(op == "<=")
			lteop();
		else if(op == ">=")
			gteop();
		else if(op == "==")
			eqop();
		else if(op == "&&")
			andop();
		else if(op == "||")
			orop();
		else if(op == "!=")
			neop();
		else if(op == "=")
			assop();
	}
    if(!SAS.empty())
    {
        sar exp = SAS.top();
        SAS.pop();
        symbol s = stable.getsym(exp.value,scope);
        if(quad.back().size() > 1)
        {
            vector<string> tmp;
            tmp.push_back("");
            tmp.push_back("RETURN");
            tmp.push_back(s.symid);
            quad.push_back(tmp);
        }
        else
        {
            quad.back().push_back("RETURN");
            quad.back().push_back(s.symid);
        }
    }
    else
    {
        if(quad.back().size() > 1)
        {
            vector<string> tmp;
            tmp.push_back("");
            tmp.push_back("RTN");
            quad.push_back(tmp);
        }
        else
        {
            quad.back().push_back("RTN");
        }
    }
    
    //if(s.kind == same as function)
	return "done";
}
string coutr()
{
    while(!OS.empty())
	{
		opr o = OS.top();
		OS.pop();
		string op = o.value;
		if(op == "+")
			addop();
		else if(op == "-")
			subop();
		else if(op == "*")
			multop();
		else if(op == "/")
			divop();
		else if(op == "%")
			modop();
		else if(op == "<")
			ltop();
		else if(op == ">")
			gtop();
		else if(op == "<=")
			lteop();
		else if(op == ">=")
			gteop();
		else if(op == "==")
			eqop();
		else if(op == "&&")
			andop();
		else if(op == "||")
			orop();
		else if(op == "!=")
			neop();
		else if(op == "=")
			assop();
	}
    sar exp = SAS.top();
    SAS.pop();
    symbol s = stable.getsym(exp.value,scope);
    if(s.typ != "char" && s.typ != "int")
    {
        err = true;
        cout << s.value << " cannot be printed out with cout line:" <<linenum << endl;
    }
    if(quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("WRITE");
		if(s.typ == "int")
            tmp.push_back("1");
        if(s.typ == "char")
            tmp.push_back("2");
        tmp.push_back(s.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("WRITE");
        if(s.typ == "int")
            quad.back().push_back("1");
        if(s.typ == "char")
            quad.back().push_back("2");
        quad.back().push_back(s.symid);
    }
	return "done";
}
string cinr()
{
    while(!OS.empty())
	{
		opr o = OS.top();
		OS.pop();
		string op = o.value;
		if(op == "+")
			addop();
		else if(op == "-")
			subop();
		else if(op == "*")
			multop();
		else if(op == "/")
			divop();
		else if(op == "%")
			modop();
		else if(op == "<")
			ltop();
		else if(op == ">")
			gtop();
		else if(op == "<=")
			lteop();
		else if(op == ">=")
			gteop();
		else if(op == "==")
			eqop();
		else if(op == "&&")
			andop();
		else if(op == "||")
			orop();
		else if(op == "!=")
			neop();
		else if(op == "=")
			assop();
	}
    sar exp = SAS.top();
    SAS.pop();
    symbol s = stable.getsym(exp.value,scope);
    if(s.typ != "char" && s.typ != "int")
    {
        err = true;
        cout << s.value << " cannot be read in with cin line:" << linenum << endl;
    }
    if(quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("READ");
        if(s.typ == "int")
            tmp.push_back("1");
        if(s.typ == "char")
            tmp.push_back("2");
        tmp.push_back(s.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("READ");
        if(s.typ == "int")
            quad.back().push_back("1");
        if(s.typ == "char")
            quad.back().push_back("2");
        quad.back().push_back(s.symid);
    }
    return "done";
}
string atoir()
{
    sar exp = SAS.top();
    SAS.pop();
    symbol s = stable.getsym(exp.value,scope);
    if(isdigit(*s.value.c_str()))
    {
        symbol x(s.scope, gentemp(), s.value, "int","int",0);
        stable.addsym(x);
        size += 4;
        sar t(exp.value,"#atoi","int");
        SAS.push(t);
        if(quad.back().size() > 1)
        {
            vector<string> tmp;
            tmp.push_back("");
            tmp.push_back("ATOI");
            tmp.push_back(s.symid);
            tmp.push_back(x.symid);
            quad.push_back(tmp);
        }
        else
        {
            quad.back().push_back("ATOI");
            quad.back().push_back(s.symid);
            quad.back().push_back(x.symid);
        }
    }
    else
    {
        err = true;
        cout << exp.value << " is not able to be converted to an int line:" << linenum <<endl;
    }
	return "done";
}
string itoar()
{
    sar exp = SAS.top();
    SAS.pop();
    symbol s = stable.getsym(exp.value,scope);
    if(isdigit(*s.value.c_str()))
    {
        symbol x(s.scope, gentemp(), s.value, "char","char",0);
        stable.addsym(x);
        size += 1;
        sar t(exp.value,"#itoa","char");
        SAS.push(t);
        
        if(quad.back().size() > 1)
        {
            vector<string> tmp;
            tmp.push_back("");
            tmp.push_back("ITOA");
            tmp.push_back(s.symid);
            tmp.push_back(x.symid);
            quad.push_back(tmp);
        }
        else
        {
            quad.back().push_back("ITOA");
            quad.back().push_back(s.symid);
            quad.back().push_back(x.symid);
        }
    }
    else
    {
        err = true;
        cout << exp.value << " is not able to be converted to an int line:" << linenum << endl;
    }
	return "done";
}
string newobj()
{
    sar arg = SAS.top();
    SAS.pop();
    sar ty = SAS.top();
    SAS.pop();
    string ob = ty.value + " " + arg.value;
    string x = gentemp();
    symbol s(scope,x, ob,"Obj",ty.value,0);
    stable.addsym(s);
    sar t(x,"#newObj",ty.value);
    SAS.push(t);
	return "done";
}
string newarr()
{
    sar exp = SAS.top();
    SAS.pop();
    sar ty = SAS.top();
    SAS.pop();
    string x = gentemp();
    symbol s(scope,x,x,"Arr", ty.value,0);
    stable.addsym(s);
    sar t(x,"#new[]",ty.value);
    SAS.push(t);
	return "done";
}
string cd(string name, string sc)
{
    if (name == sc.substr(sc.find_last_of(".")+1))
        return "done";
    else
    {
        cout <<"No class found for this constructor " + name + " line:"<< linenum << endl;
        return "not found";
    }
}
string cp()
{
	while(OS.top().value != "(")
	{
		opr o = OS.top();
		OS.pop();
		string op = o.value;
		if(op == "+")
			addop();
		else if(op == "-")
			subop();
		else if(op == "*")
			multop();
		else if(op == "/")
			divop();
		else if(op == "%")
			modop();
		else if(op == "<")
			ltop();
		else if(op == ">")
			gtop();
		else if(op == "<=")
			lteop();
		else if(op == ">=")
			gteop();
		else if(op == "==")
			eqop();
		else if(op == "&&")
			andop();
		else if(op == "||")
			orop();
		else if(op == "!=")
			neop();
		else if(op == "=")
			assop();
	}
    if(OS.top().value == "(")
    {
        OS.pop();
    }
	return "done";
}
string cb()
{
	while(OS.top().value != "[")
	{
		opr o = OS.top();
		OS.pop();
		string op = o.value;
		if(op == "+")
			addop();
		else if(op == "-")
			subop();
		else if(op == "*")
			multop();
		else if(op == "/")
			divop();
		else if(op == "%")
			modop();
		else if(op == "<")
			ltop();
		else if(op == ">")
			gtop();
		else if(op == "<=")
			lteop();
		else if(op == ">=")
			gteop();
		else if(op == "==")
			eqop();
		else if(op == "&&")
			andop();
		else if(op == "||")
			orop();
		else if(op == "!=")
			neop();
		else if(op == "=")
			assop();
	}
    if(OS.top().value == "(")
    {
        OS.pop();
    }
	return "done";
}
string com()
{
    while(OS.top().value != "(")
	{
		opr o = OS.top();
		OS.pop();
		string op = o.value;
		if(op == "+")
			addop();
		else if(op == "-")
			subop();
		else if(op == "*")
			multop();
		else if(op == "/")
			divop();
		else if(op == "%")
			modop();
		else if(op == "<")
			ltop();
		else if(op == ">")
			gtop();
		else if(op == "<=")
			lteop();
		else if(op == ">=")
			gteop();
		else if(op == "==")
			eqop();
		else if(op == "&&")
			andop();
		else if(op == "||")
			orop();
		else if(op == "!=")
			neop();
		else if(op == "=")
			assop();
	}
	return "done";
}
string eoe()
{
	while(!OS.empty())
	{
		opr o = OS.top();
		OS.pop();
		string op = o.value;
		if(op == "+")
			addop();
		else if(op == "-")
			subop();
		else if(op == "*")
			multop();
		else if(op == "/")
			divop();
		else if(op == "%")
			modop();
		else if(op == "<")
			ltop();
		else if(op == ">")
			gtop();
		else if(op == "<=")
			lteop();
		else if(op == ">=")
			gteop();
		else if(op == "==")
			eqop();
		else if(op == "&&")
			andop();
		else if(op == "||")
			orop();
		else if(op == "!=")
			neop();
		else if(op == "=")
			assop();
	}
	return "done";
}
string addop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
    if(a.typ != b.typ)
    {
        cout <<"types are not equal line:" << linenum << endl;
    }
    if(a.typ != "int")
    {
        err = true;
        cout << "these types cannot be added line:" << linenum << endl;
    }
	symbol y(scope,x,inttostring(atoi(a.value.c_str())+atoi(b.value.c_str())),a.kind,a.typ,size+12);
	stable.addsym(y);
    size += 4;
	sar s(x,"#+");
	SAS.push(s);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("ADD");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("ADD");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
	return "done";
}
string subop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
    if(a.typ != b.typ)
    {
        cout <<"types are not equal line:" << linenum << endl;
    }
    if(a.typ != "int")
    {
        err = true;
        cout << "these types cannot be subtracted line:" << linenum << endl;
    }
	symbol y(scope,x,inttostring(atoi(a.value.c_str())-atoi(b.value.c_str())),a.kind,a.typ, size+12);
	stable.addsym(y);
    size +=4;
	sar s(x,"#-");
	SAS.push(s);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("SUB");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("SUB");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
    
	return "done";
}
string multop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
    if(a.typ != b.typ)
    {
        cout <<"types are not equal line:" << linenum << endl;
    }
    if(a.typ != "int")
    {
        err = true;
        cout << "these types cannot be multiplied line:" << linenum << endl;
    }
	symbol y(scope,x,inttostring(atoi(a.value.c_str())*atoi(b.value.c_str())),a.kind,a.typ,size+12);
	stable.addsym(y);
    size +=4;
	sar s(x,"#*");
	SAS.push(s);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("MUL");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("MUL");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
    
	return "done";
}
string divop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
    if(a.typ != b.typ)
    {
        cout <<"types are not equal line:" << linenum << endl;
    }
    if(a.typ != "int")
    {
        err = true;
        cout << "these types cannot be divided line:" << linenum << endl;
    }
	symbol y(scope,x,inttostring(atoi(a.value.c_str())/atoi(b.value.c_str())),a.kind,a.typ,size+12);
	stable.addsym(y);
    size += 4;
	sar s(x,"#/");
	SAS.push(s);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("DIV");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("DIV");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
	return "done";
}
string assop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
    symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
	if(a.typ != b.typ)
	{
        if(b.value != "null")
        {
            err = true;
            cout << "Types cannot be assigned line:" << linenum <<endl;
        }
//        else
            
	}
    
    if(quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("MOV");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("MOV");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
    }
    
	return "done";
}
string modop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
    if(a.typ != b.typ)
    {
        cout <<"types are not equal line:" << linenum << endl;
    }
    if(a.typ != "int")
    {
        err = true;
        cout << "these types cannot be modulused line:" << linenum << endl;
    }
	symbol y(scope,x,inttostring(atoi(a.value.c_str())%atoi(b.value.c_str())),a.kind,a.typ, size + 12);
	stable.addsym(y);
    size += 4;
	sar s(x,"#%");
	SAS.push(s);
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("MOD");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("MOD");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
	return "done";
}
string ltop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
    if(a.typ != b.typ)
    {
        cout <<"types are not equal in < line:" << linenum << endl;
    }
    if(a.typ != "int")
    {
        err = true;
        cout << "these types cannot be compared in < line:" << linenum << endl;
    }
	string result = "false";
	if(atoi(a.value.c_str()) < atoi(b.value.c_str()))
	{
		result = "true";
	}
	symbol y(scope,x,result,"bool","bool",size +12);
	stable.addsym(y);
    size += 4;
	sar s(x,"#<","bool");
	SAS.push(s);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("LT");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("LT");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
    
	return "done";
}
string gtop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
	if(a.typ != b.typ)
    {
        cout <<"types are not equal in > line:" << linenum << endl;
    }
    if(a.typ != "int")
    {
        err = true;
        cout << "these types cannot be compared in > line:" << linenum << endl;
    }
	string result = "false";
	if(atoi(a.value.c_str()) > atoi(b.value.c_str()))
	{
		result = "true";
	}
	symbol y = *new symbol(scope,x,result,"bool","bool", size+12);
	stable.addsym(y);
    size += 4;
	sar s(x,"#>","bool");
	SAS.push(s);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("GT");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("GT");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
	return "done";
}
string eqop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
	if(a.typ != b.typ)
    {
        cout <<"types are not equal in == line:" << linenum << endl;
    }
    if(a.typ != "int" && a.typ != "bool" && a.typ != "char")
    {
        err = true;
        cout << "these types cannot be compared in == line:" << linenum << endl;
    }
	string result = "false";
	if(atoi(a.value.c_str()) == atoi(b.value.c_str()))
	{
		result = "true";
	}
	symbol y(scope,x,result,"bool","bool", size + 12);
	stable.addsym(y);
    size += 4;
	sar s(x,"#==","bool");
	SAS.push(s);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("EQ");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("EQ");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
	return "done";
}
string lteop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
	if(a.typ != b.typ)
    {
        cout <<"types are not equal in <= line:" << linenum << endl;
    }
    if(a.typ != "int")
    {
        err = true;
        cout << "these types cannot be compared in <= line:" << linenum << endl;
    }
	string result = "false";
	if(atoi(a.value.c_str()) <= atoi(b.value.c_str()))
	{
		result = "true";
	}
	symbol y(scope,x,result,"bool","bool", size + 12);
	stable.addsym(y);
    size += 4;
	sar s(x,"#<=","bool");
	SAS.push(s);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("LE");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("LE");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
	return "done";
}
string gteop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
	if(a.typ != b.typ)
    {
        cout <<"types are not equal in >= line:" << linenum << endl;
    }
    if(a.typ != "int")
    {
        err = true;
        cout << "these types cannot be compared in >= line:" << linenum << endl;
    }
	string result = "false";
	if(atoi(a.value.c_str()) >= atoi(b.value.c_str()))
	{
		result = "true";
	}
	symbol y(scope,x,result,"bool","bool", size + 12);
	stable.addsym(y);
    size += 4;
	sar s(x,"#>=","bool");
	SAS.push(s);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("GE");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("GE");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
	return "done";
}
string andop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
	if(a.typ != b.typ)
    {
        cout <<"types are not equal in && line:" << linenum << endl;
    }
    if(a.typ != "bool")
    {
        err = true;
        cout << "these types cannot be compared in && line:" << linenum << endl;
    }
	bool value1 = false;
	bool value2 = false;
	string result = "false";
	if(a.value == "true")
		value1 = true;
	if(b.value == "true")
		value2 = true;
	if(value1 && value2)
	{
		result = "true";
	}
	symbol y(scope,x,result,"bool","bool", size + 12);
	stable.addsym(y);
    size += 4;
	sar s(x,"#&&","bool");
	SAS.push(s);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("AND");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("AND");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
    
	return "done";
}
string orop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
	if(a.typ != b.typ)
    {
        cout <<"types are not equal in || line:" << linenum << endl;
    }
    if(a.typ != "bool")
    {
        err = true;
        cout << "these types cannot be compared in || line:" << linenum << endl;
    }
	bool value1 = false;
	bool value2 = false;
	string result = "false";
	if(a.value == "true")
		value1 = true;
	if(b.value == "true")
		value2 = true;
	if(value1 || value2)
	{
		result = "true";
	}
	symbol y(scope,x,result,"bool","bool", size + 12);
	stable.addsym(y);
    size += 4;
	sar s(x,"#||","bool");
	SAS.push(s);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("OR");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("OR");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
	return "done";
}
string neop()
{
	sar exp2 = SAS.top();
	SAS.pop();
	sar exp1 = SAS.top();
	SAS.pop();
	string x = gentemp();
	symbol b = stable.getsym(exp2.value,scope);
	symbol a = stable.getsym(exp1.value,scope);
	if(a.typ != b.typ)
    {
        cout <<"types are not equal in != line:" << linenum << endl;
    }
    if(a.typ != "int" && a.typ != "bool" && a.typ != "char")
    {
        err = true;
        cout << "these types cannot be compared in != line:" << linenum << endl;
    }
	bool value1 = false;
	bool value2 = false;
	string result = "false";
	if(a.value == "true")
		value1 = true;
	if(b.value == "true")
		value2 = true;
	if(value1 != value2)
	{
		result = "true";
	}
	symbol y(scope,x,result,"bool","bool", size +12);
	stable.addsym(y);
    size += 4;
	sar s(x,"#!=","bool");
	SAS.push(s);
    
    if (quad.back().size() > 1)
    {
        vector<string> tmp;
        tmp.push_back("");
        tmp.push_back("NE");
        tmp.push_back(a.symid);
        tmp.push_back(b.symid);
        tmp.push_back(y.symid);
        quad.push_back(tmp);
    }
    else
    {
        quad.back().push_back("NE");
        quad.back().push_back(a.symid);
        quad.back().push_back(b.symid);
        quad.back().push_back(y.symid);
    }
	return "done";
}
string tcodegen()
{
    for (int i = 0; i < quad.size(); i++)
    {
        vector<string> tmp;
        string r1;
        string r2;
        string r3;
        string r4;
		if(quad[i][0].size() > 0)
			tmp.push_back(quad[i][0]);
		if(quad[i][1] == ".BYT" || quad[i][1] == ".INT")
			tcode.push_back(quad[i]);
        else if(quad[i][1] == "ADD" || quad[i][1] == "SUB" || quad[i][1] == "MUL" || quad[i][1] == "DIV" || quad[i][1] == "MOD")
        {
            string loc1 = getlocation(quad[i][2]);
            string loc2 = getlocation(quad[i][3]);
            string loc3 = getlocation(quad[i][4]);
            if(isdigit(loc1[0]))
            {
                r2 = getregister();
                tmp.push_back("MOV");
                tmp.push_back(r2);
                tmp.push_back("FP");
                tcode.push_back(tmp);
                reg[r2].push_back("FP");
                vector<string> tmp5;
                tmp5.push_back("ADI");
                tmp5.push_back(r2);
                tmp5.push_back("-"+loc1);
                tcode.push_back(tmp5);
                r1 = getregister();
                vector<string> tmp6;
                tmp6.push_back("LDR");
                tmp6.push_back(r1);
                tmp6.push_back(r2);
                tcode.push_back(tmp6);
                reg[r2].clear();
            }
            else
            {
                r1 = getregister();
                tmp.push_back("LDR");
                tmp.push_back(r1);
                tmp.push_back(loc1);
                tcode.push_back(tmp);
            }
            reg[r1].push_back(quad[i][2]);
            if(isdigit(loc2[0]))
            {
                r3 = getregister();
                vector<string> tmp9;
                tmp9.push_back("MOV");
                tmp9.push_back(r3);
                tmp9.push_back("FP");
                tcode.push_back(tmp9);
                reg[r3].push_back("FP");
                vector<string> tmp10;
                tmp10.push_back("ADI");
                tmp10.push_back(r3);
                tmp10.push_back("-"+loc2);
                tcode.push_back(tmp10);
                r2 = getregister();
                vector<string> tmp11;
                tmp11.push_back("LDR");
                tmp11.push_back(r2);
                tmp11.push_back(r3);
                tcode.push_back(tmp11);
                reg[r3].clear();
            }
            else
            {
                vector<string> tmp2;
                r2 = getregister();
                tmp2.push_back("LDR");
                tmp2.push_back(r2);
                tmp2.push_back(quad[i][3]);
                tcode.push_back(tmp2);
            }
            reg[r2].push_back(quad[i][3]);
            vector<string> tmp3;
			tmp3.push_back(quad[i][1]);
			tmp3.push_back(getlocation(quad[i][2]));
			tmp3.push_back(getlocation(quad[i][3]));
			tcode.push_back(tmp3);
            r3 = getregister();
            vector<string> tmp7;
            tmp7.push_back("MOV");
            tmp7.push_back(r3);
            tmp7.push_back("FP");
            tcode.push_back(tmp7);
            reg[r3].push_back("FP");
            vector<string> tmp8;
            tmp8.push_back("ADI");
            tmp8.push_back(r3);
            tmp8.push_back("-"+loc3);
            tcode.push_back(tmp8);
			vector<string> tmp4;
			tmp4.push_back("STR");
			tmp4.push_back(getlocation(quad[i][2]));
			tmp4.push_back(r3);
			tcode.push_back(tmp4);
			reg[r1].clear();
			reg[r2].clear();
            reg[r3].clear();
        }
		else if(quad[i][1] == "ADI")
		{
            string loc1 = getlocation(quad[i][2]);
            string loc2 = getlocation(quad[i][4]);
            if(isdigit(loc1[0]))
            {
                r2 = getregister();
                tmp.push_back("MOV");
                tmp.push_back(r1);
                tmp.push_back("FP");
                tcode.push_back(tmp);
                reg[r2].push_back("FP");
                vector<string> tmp4;
                tmp4.push_back("ADI");
                tmp4.push_back(r2);
                tmp4.push_back(loc1);
                tcode.push_back(tmp4);
                r1 = getregister();
                vector<string> tmp5;
                tmp5.push_back("LDR");
                tmp5.push_back(r1);
                tmp5.push_back(r2);
                tcode.push_back(tmp5);
                reg[r2].clear();
                
            }
            else
            {
                r1 = getregister();
                tmp.push_back("LDR");
                tmp.push_back(r1);
                tmp.push_back(loc1);
                tcode.push_back(tmp);
            }
            reg[r1].push_back(quad[i][2]);
            vector<string> tmp2;
            tmp2.push_back("ADI");
			tmp2.push_back(r1);
            tmp2.push_back(quad[i][3]);
            tcode.push_back(tmp2);
            vector<string> tmp3;
			tmp3.push_back("STR");
			tmp3.push_back(r1);
			tmp3.push_back(quad[i][4]);
			tcode.push_back(tmp3);
			reg[r1].clear();
		}
/*		else if(quad[i][1] == "SUB")
		{
			r1 = getregister();
            tmp.push_back("LDR");
            tmp.push_back(r1);
            tmp.push_back(quad[i][2]);
            tcode.push_back(tmp);
            reg[r1].push_back(quad[i][2]);
            vector<string> tmp2;
            r2 = getregister();
            tmp2.push_back("LDR");
            tmp2.push_back(r2);
            tmp2.push_back(quad[i][3]);
            tcode.push_back(tmp2);
            reg[r2].push_back(quad[i][3]);
            vector<string> tmp3;
			tmp3.push_back("SUB");
			tmp3.push_back(r1);
			tmp3.push_back(r2);
			tcode.push_back(tmp3);
			vector<string> tmp4;
			tmp4.push_back("STR");
			tmp4.push_back(r1);
			tmp4.push_back(quad[i][4]);
			tcode.push_back(tmp4);
			reg[r1].clear();
			reg[r2].clear();
		}
		else if(quad[i][1] == "MUL")
		{
			r1 = getregister();
            tmp.push_back("LDR");
            tmp.push_back(r1);
            tmp.push_back(quad[i][2]);
            tcode.push_back(tmp);
            reg[r1].push_back(quad[i][2]);
            vector<string> tmp2;
            r2 = getregister();
            tmp2.push_back("LDR");
            tmp2.push_back(r2);
            tmp2.push_back(quad[i][3]);
            tcode.push_back(tmp2);
            reg[r2].push_back(quad[i][3]);
            vector<string> tmp3;
			tmp3.push_back("MUL");
			tmp3.push_back(r1);
			tmp3.push_back(r2);
			tcode.push_back(tmp3);
			vector<string> tmp4;
			tmp4.push_back("STR");
			tmp4.push_back(r1);
			tmp4.push_back(quad[i][4]);
			tcode.push_back(tmp4);
			reg[r1].clear();
			reg[r2].clear();
		}
		else if(quad[i][1] == "DIV")
		{
			r1 = getregister();
            tmp.push_back("LDR");
            tmp.push_back(r1);
            tmp.push_back(quad[i][2]);
            tcode.push_back(tmp);
            reg[r1].push_back(quad[i][2]);
            vector<string> tmp2;
            r2 = getregister();
            tmp2.push_back("LDR");
            tmp2.push_back(r2);
            tmp2.push_back(quad[i][3]);
            tcode.push_back(tmp2);
            reg[r2].push_back(quad[i][3]);
            vector<string> tmp3;
			tmp3.push_back("DIV");
			tmp3.push_back(r1);
			tmp3.push_back(r2);
			tcode.push_back(tmp3);
			vector<string> tmp4;
			tmp4.push_back("STR");
			tmp4.push_back(r1);
			tmp4.push_back(quad[i][4]);
			tcode.push_back(tmp4);
			reg[r1].clear();
			reg[r2].clear();
		}
        else if(quad[i][1] == "MOD")
		{
			r1 = getregister();
            tmp.push_back("LDR");
            tmp.push_back(r1);
            tmp.push_back(quad[i][2]);
            tcode.push_back(tmp);
            reg[r1].push_back(quad[i][2]);
            vector<string> tmp2;
            r2 = getregister();
            tmp2.push_back("LDR");
            tmp2.push_back(r2);
            tmp2.push_back(quad[i][3]);
            tcode.push_back(tmp2);
            reg[r2].push_back(quad[i][3]);
            vector<string> tmp3;
			tmp3.push_back("MOD");
			tmp3.push_back(r1);
			tmp3.push_back(r2);
			tcode.push_back(tmp3);
			vector<string> tmp4;
			tmp4.push_back("STR");
			tmp4.push_back(r1);
			tmp4.push_back(quad[i][4]);
			tcode.push_back(tmp4);
			reg[r1].clear();
			reg[r2].clear();
		}
*/		else if(quad[i][1] == "EQ")
		{
			r1 = getregister();
            tmp.push_back("LDR");
            tmp.push_back(r1);
            tmp.push_back(quad[i][2]);
            tcode.push_back(tmp);
            reg[r1].push_back(quad[i][2]);
            vector<string> tmp2;
            r2 = getregister();
            tmp2.push_back("LDR");
            tmp2.push_back(r2);
            tmp2.push_back(quad[i][3]);
            tcode.push_back(tmp2);
            reg[r2].push_back(quad[i][3]);
            vector<string> tmp3;
			tmp3.push_back("CMP");
			tmp3.push_back(r1);
			tmp3.push_back(r2);
			tcode.push_back(tmp3);
			vector<string> tmp4;
			tmp4.push_back("BRZ");
			tmp4.push_back(r1);
			string lbl1 = "l"+lblgen();
			string lbl2 = "l"+lblgen();
			tmp4.push_back(lbl1);
			tcode.push_back(tmp4);
			vector<string> tmp5;
			tmp5.push_back("LDR");
            tmp5.push_back(r1);
            tmp5.push_back("zero");
            tcode.push_back(tmp5);
			vector<string> tmp9;
			tmp9.push_back("STR");
			tmp9.push_back(r1);
			tmp9.push_back(quad[i][4]);
			tcode.push_back(tmp9);
			reg[r1].push_back(quad[i][4]);
			vector<string> tmp6;
			tmp6.push_back("JMP");
			tmp6.push_back(lbl2);
			tcode.push_back(tmp6);
			vector<string> tmp7;
			tmp7.push_back(lbl1);
			tmp7.push_back("LDR");
			tmp7.push_back(r1);
			tmp7.push_back("one");
			tcode.push_back(tmp7);
			vector<string> tmp8;
			tmp8.push_back("STR");
			tmp8.push_back(r1);
			tmp8.push_back(quad[i][4]);
			tcode.push_back(tmp8);
			vector<string> tmp10;
			tmp10.push_back(lbl2);
			tcode.push_back(tmp10);
//			reg[stringtoint(r1)].clear();
			reg[r2].clear();
		}
        else if(quad[i][1] == "LT")
		{
			r1 = getregister();
            tmp.push_back("LDR");
            tmp.push_back(r1);
            tmp.push_back(quad[i][2]);
            tcode.push_back(tmp);
            reg[r1].push_back(quad[i][2]);
            vector<string> tmp2;
            r2 = getregister();
            tmp2.push_back("LDR");
            tmp2.push_back(r2);
            tmp2.push_back(quad[i][3]);
            tcode.push_back(tmp2);
            reg[r2].push_back(quad[i][3]);
            vector<string> tmp3;
			tmp3.push_back("CMP");
			tmp3.push_back(r1);
			tmp3.push_back(r2);
			tcode.push_back(tmp3);
			vector<string> tmp4;
			tmp4.push_back("BLT");
			tmp4.push_back(r1);
			string lbl1 = "l"+lblgen();
			string lbl2 = "l"+lblgen();
			tmp4.push_back(lbl1);
			tcode.push_back(tmp4);
			vector<string> tmp5;
			tmp5.push_back("LDR");
            tmp5.push_back(r1);
            tmp5.push_back("zero");
            tcode.push_back(tmp5);
			vector<string> tmp9;
			tmp9.push_back("STR");
			tmp9.push_back(r1);
			tmp9.push_back(quad[i][4]);
			tcode.push_back(tmp9);
			vector<string> tmp6;
			tmp6.push_back("JMP");
			tmp6.push_back(lbl2);
			tcode.push_back(tmp6);
			vector<string> tmp7;
			tmp7.push_back(lbl1);
			tmp7.push_back("LDR");
			tmp7.push_back(r1);
			tmp7.push_back("one");
			tcode.push_back(tmp7);
			vector<string> tmp8;
			tmp8.push_back("STR");
			tmp8.push_back(r1);
			tmp8.push_back(quad[i][4]);
			tcode.push_back(tmp8);
			vector<string> tmp10;
			tmp10.push_back(lbl2);
			tcode.push_back(tmp10);
			reg[r1].push_back(quad[i][4]);
//			reg[stringtoint(r1)].clear();
			reg[r2].clear();
		}
		else if(quad[i][1] == "GT")
		{
			r1 = getregister();
            tmp.push_back("LDR");
            tmp.push_back(r1);
            tmp.push_back(quad[i][2]);
            tcode.push_back(tmp);
            reg[r1].push_back(quad[i][2]);
            vector<string> tmp2;
            r2 = getregister();
            tmp2.push_back("LDR");
            tmp2.push_back(r2);
            tmp2.push_back(quad[i][3]);
            tcode.push_back(tmp2);
            reg[r2].push_back(quad[i][3]);
            vector<string> tmp3;
			tmp3.push_back("CMP");
			tmp3.push_back(r1);
			tmp3.push_back(r2);
			tcode.push_back(tmp3);
			vector<string> tmp4;
			tmp4.push_back("BGT");
			tmp4.push_back(r1);
			string lbl1 = "l"+lblgen();
			string lbl2 = "l"+lblgen();
			tmp4.push_back(lbl1);
			tcode.push_back(tmp4);
			vector<string> tmp5;
			tmp5.push_back("LDR");
            tmp5.push_back(r1);
            tmp5.push_back("zero");
            tcode.push_back(tmp5);
			vector<string> tmp9;
			tmp9.push_back("STR");
			tmp9.push_back(r1);
			tmp9.push_back(quad[i][4]);
			tcode.push_back(tmp9);
			vector<string> tmp6;
			tmp6.push_back("JMP");
			tmp6.push_back(lbl2);
			tcode.push_back(tmp6);
			vector<string> tmp7;
			tmp7.push_back(lbl1);
			tmp7.push_back("LDR");
			tmp7.push_back(r1);
			tmp7.push_back("one");
			tcode.push_back(tmp7);
			vector<string> tmp8;
			tmp8.push_back("STR");
			tmp8.push_back(r1);
			tmp8.push_back(quad[i][4]);
			tcode.push_back(tmp8);
			vector<string> tmp10;
			tmp10.push_back(lbl2);
			tcode.push_back(tmp10);
			reg[r1].push_back(quad[i][4]);
//			reg[stringtoint(r1)].clear();
			reg[r2].clear();
		}
		else if(quad[i][1] == "NE")
		{
			r1 = getregister();
            tmp.push_back("LDR");
            tmp.push_back(r1);
            tmp.push_back(quad[i][2]);
            tcode.push_back(tmp);
            reg[r1].push_back(quad[i][2]);
            vector<string> tmp2;
            r2 = getregister();
            tmp2.push_back("LDR");
            tmp2.push_back(r2);
            tmp2.push_back(quad[i][3]);
            tcode.push_back(tmp2);
            reg[r2].push_back(quad[i][3]);
            vector<string> tmp3;
			tmp3.push_back("CMP");
			tmp3.push_back(r1);
			tmp3.push_back(r2);
			tcode.push_back(tmp3);
			vector<string> tmp4;
			tmp4.push_back("BNZ");
			tmp4.push_back(r1);
			string lbl1 = "l"+lblgen();
			string lbl2 = "l"+lblgen();
			tmp4.push_back(lbl1);
			tcode.push_back(tmp4);
			vector<string> tmp5;
			tmp5.push_back("LDR");
            tmp5.push_back(r1);
            tmp5.push_back("zero");
            tcode.push_back(tmp5);
			vector<string> tmp9;
			tmp9.push_back("STR");
			tmp9.push_back(r1);
			tmp9.push_back(quad[i][4]);
			tcode.push_back(tmp9);
			vector<string> tmp6;
			tmp6.push_back("JMP");
			tmp6.push_back(lbl2);
			tcode.push_back(tmp6);
			vector<string> tmp7;
			tmp7.push_back(lbl1);
			tmp7.push_back("LDR");
			tmp7.push_back(r1);
			tmp7.push_back("one");
			tcode.push_back(tmp7);
			vector<string> tmp8;
			tmp8.push_back("STR");
			tmp8.push_back(r1);
			tmp8.push_back(quad[i][4]);
			tcode.push_back(tmp8);
			vector<string> tmp10;
			tmp10.push_back(lbl2);
			tcode.push_back(tmp10);
			reg[r1].push_back(quad[i][4]);
//			reg[stringtoint(r1)].clear();
			reg[r2].clear();
		}
		else if(quad[i][1] == "LE")
		{
			r1 = getregister();
            tmp.push_back("LDR");
            tmp.push_back(r1);
            tmp.push_back(quad[i][2]);
            tcode.push_back(tmp);
            reg[r1].push_back(quad[i][2]);
            vector<string> tmp2;
            r2 = getregister();
            tmp2.push_back("LDR");
            tmp2.push_back(r2);
            tmp2.push_back(quad[i][3]);
            tcode.push_back(tmp2);
            reg[r2].push_back(quad[i][3]);
            vector<string> tmp3;
			tmp3.push_back("CMP");
			tmp3.push_back(r1);
			tmp3.push_back(r2);
			tcode.push_back(tmp3);
			string lbl1 = "l"+lblgen();
			string lbl2 = "l"+lblgen();
			vector<string> tmp11;
			tmp11.push_back("BLT");
			tmp11.push_back(r1);
			tmp11.push_back(lbl1);
			tcode.push_back(tmp11);
			vector<string> tmp4;
			tmp4.push_back("BRZ");
			tmp4.push_back(r1);
			tmp4.push_back(lbl1);
			tcode.push_back(tmp4);
			vector<string> tmp5;
			tmp5.push_back("LDR");
            tmp5.push_back(r1);
            tmp5.push_back("zero");
            tcode.push_back(tmp5);
			vector<string> tmp9;
			tmp9.push_back("STR");
			tmp9.push_back(r1);
			tmp9.push_back(quad[i][4]);
			tcode.push_back(tmp9);
			vector<string> tmp6;
			tmp6.push_back("JMP");
			tmp6.push_back(lbl2);
			tcode.push_back(tmp6);
			vector<string> tmp7;
			tmp7.push_back(lbl1);
			tmp7.push_back("LDR");
			tmp7.push_back(r1);
			tmp7.push_back("one");
			tcode.push_back(tmp7);
			vector<string> tmp8;
			tmp8.push_back("STR");
			tmp8.push_back(r1);
			tmp8.push_back(quad[i][4]);
			tcode.push_back(tmp8);
			vector<string> tmp10;
			tmp10.push_back(lbl2);
			tcode.push_back(tmp10);
			reg[r1].push_back(quad[i][4]);
//			reg[stringtoint(r1)].clear();
			reg[r2].clear();
		}
		else if(quad[i][1] == "GE")
		{
			r1 = getregister();
            tmp.push_back("LDR");
            tmp.push_back(r1);
            tmp.push_back(quad[i][2]);
            tcode.push_back(tmp);
            reg[r1].push_back(quad[i][2]);
            vector<string> tmp2;
            r2 = getregister();
            tmp2.push_back("LDR");
            tmp2.push_back(r2);
            tmp2.push_back(quad[i][3]);
            tcode.push_back(tmp2);
            reg[r2].push_back(quad[i][3]);
            vector<string> tmp3;
			tmp3.push_back("CMP");
			tmp3.push_back(r1);
			tmp3.push_back(r2);
			tcode.push_back(tmp3);
			string lbl1 = "l"+lblgen();
			string lbl2 = "l"+lblgen();
			vector<string> tmp11;
			tmp11.push_back("BGT");
			tmp11.push_back(r1);
			tmp11.push_back(lbl1);
			tcode.push_back(tmp11);
			vector<string> tmp4;
			tmp4.push_back("BRZ");
			tmp4.push_back(r1);
			tmp4.push_back(lbl1);
			tcode.push_back(tmp4);
			vector<string> tmp5;
			tmp5.push_back("LDR");
            tmp5.push_back(r1);
            tmp5.push_back("zero");
            tcode.push_back(tmp5);
			vector<string> tmp9;
			tmp9.push_back("STR");
			tmp9.push_back(r1);
			tmp9.push_back(quad[i][4]);
			tcode.push_back(tmp9);
			vector<string> tmp6;
			tmp6.push_back("JMP");
			tmp6.push_back(lbl2);
			tcode.push_back(tmp6);
			vector<string> tmp7;
			tmp7.push_back(lbl1);
			tmp7.push_back("LDR");
			tmp7.push_back(r1);
			tmp7.push_back("one");
			tcode.push_back(tmp7);
			vector<string> tmp8;
			tmp8.push_back("STR");
			tmp8.push_back(r1);
			tmp8.push_back(quad[i][4]);
			tcode.push_back(tmp8);
			vector<string> tmp10;
			tmp10.push_back(lbl2);
			tcode.push_back(tmp10);
			reg[r1].push_back(quad[i][4]);
//			reg[stringtoint(r1)].clear();
			reg[r2].clear();
		}
		else if(quad[i][1] == "AND" || quad[i][1] == "OR")
		{
			cout << quad[i][1] << " not implemented in VM" << endl;
		}
		else if(quad[i][1] == "BF")
		{
			string loc = getlocation(quad[i][2]);
			tcode.back().push_back("BRZ");
			tcode.back().push_back(loc);
			tcode.back().push_back(quad[i][3]);
			reg[loc].clear();

		}
		else if(quad[i][1] == "BT")
		{
			string loc = getlocation(quad[i][2]);
			tcode.back().push_back("BNZ");
			tcode.back().push_back(loc);
			tcode.back().push_back(quad[i][3]);
			reg[loc].clear();
		}
		else if(quad[i][1] == "JMP")
		{
			vector<string> tmp;
			tmp.push_back("JMP");
			tmp.push_back(quad[i][2]);
			tcode.push_back(tmp);
		}
		else if(quad[i][1] == "PUSH")
		{
			r1 = getregister();
            tmp.push_back("LDR");
            tmp.push_back(r1);
            tmp.push_back(quad[i][2]);
            tcode.push_back(tmp);
			vector<string> tmp2;
			tmp2.push_back("STR");
			tmp2.push_back(r1);
			tmp2.push_back("SP");
			tcode.push_back(tmp2);
			vector<string> tmp3;
			tmp3.push_back("ADI");
			tmp3.push_back("SP");
			tmp3.push_back("-4");
			tcode.push_back(tmp3);
			reg[r1].clear();
		}
		else if(quad[i][1] == "POP")
		{
			r1 = getregister();
			vector<string> tmp3;
			tmp.push_back("ADI");
			tmp.push_back("SP");
			tmp.push_back("4");
			tcode.push_back(tmp);
            tmp3.push_back("LDR");
            tmp3.push_back(r1);
            tmp3.push_back("SP");
            tcode.push_back(tmp3);
			vector<string> tmp2;
			tmp2.push_back("STR");
			tmp2.push_back(r1);
			tmp2.push_back(quad[i][2]);
			tcode.push_back(tmp2);
			reg[r1].clear();
		}
		else if(quad[i][1] == "PEEK")
		{
			r1 = getregister();
			vector<string> tmp4;
			tmp.push_back("MOV");
			tmp.push_back(r1);
			tmp.push_back("SP");
			tcode.push_back(tmp);
            reg[r1].push_back("SP");
            r2 = getregister();
            vector<string> tmp3;
			tmp3.push_back("ADI");
			tmp3.push_back(r1);
			tmp3.push_back("4");
			tcode.push_back(tmp3);
            tmp4.push_back("LDR");
            tmp4.push_back(r2);
            tmp4.push_back(r1);
            tcode.push_back(tmp4);
			vector<string> tmp2;
			tmp2.push_back("LDR");
			tmp2.push_back(r1);
			tmp2.push_back(quad[i][2]);
			tcode.push_back(tmp2);
			reg[r1].clear();
			reg[r2].clear();
		}
		else if(quad[i][1] == "MOV")
		{
            if(quad[i][2][0] == 'R' || quad[i][2][0] == 'S' || quad[i][2][0] == 'F')
            {
                tmp.push_back("MOV");
                tmp.push_back(quad[i][3]);
                tmp.push_back(quad[i][2]);
                tcode.push_back(tmp);
            }
            else
            {
                r1 = getregister();
                tmp.push_back("LDR");
                tmp.push_back(r1);
                tmp.push_back(quad[i][3]);
                tcode.push_back(tmp);
                vector<string> tmp2;
                tmp2.push_back("STR");
                tmp2.push_back(r1);
                tmp2.push_back(quad[i][2]);
                tcode.push_back(tmp2);
            }
			
		}
		else if(quad[i][1] == "WRITE")
		{
			if(quad[i][2] == "1")
			{
				tmp.push_back("LDR");
				tmp.push_back("R1");
				tmp.push_back(quad[i][3]);
				tcode.push_back(tmp);
				vector<string> tmp2;
				tmp2.push_back("TRP");
				tmp2.push_back("1");
				tcode.push_back(tmp2);
			}
			else
			{
				tmp.push_back("LDB");
				tmp.push_back("R3");
				tmp.push_back(quad[i][3]);
				tcode.push_back(tmp);
				vector<string> tmp2;
				tmp2.push_back("TRP");
				tmp2.push_back("3");
				tcode.push_back(tmp2);
			}
		}
		else if(quad[i][1] == "READ")
		{
			if(quad[i][2] == "1")
			{
				vector<string> tmp2;
				tmp.push_back("TRP");
				tmp.push_back("2");
				tcode.push_back(tmp);
				tmp2.push_back("STR");
				tmp2.push_back("R2");
				tmp2.push_back(quad[i][3]);
				tcode.push_back(tmp2);
				
			}
			else
			{
				vector<string> tmp2;
				tmp.push_back("TRP");
				tmp.push_back("4");
				tcode.push_back(tmp);
				tmp2.push_back("STB");
				tmp2.push_back("R4");
				tmp2.push_back(quad[i][3]);
				tcode.push_back(tmp2);
				
			}
		}
		else if(quad[i][1] == "CALL")
		{
			r1 = getregister();
			tmp.push_back("LDA");
			tmp.push_back(r1);
			tmp.push_back("PC");
			tcode.push_back(tmp);
			vector<string> tmp2;
			tmp2.push_back("ADI");
			tmp2.push_back(r1);
			tmp2.push_back("4");
			tcode.push_back(tmp2);
			vector<string> tmp3;
			tmp3.push_back("STR");
			tmp3.push_back(r1);
			tmp3.push_back("FP");
			tcode.push_back(tmp3);
			vector<string> tmp4;
			tmp4.push_back("JMP");
			tmp4.push_back(quad[i][2]);
			tcode.push_back(tmp4);
			reg[r1].clear();
		}
		else if(quad[i][1] == "FRAME")
		{
			r1 = getregister();
			tmp.push_back("MOV");
			tmp.push_back(r1);
			tmp.push_back("FP");
			tcode.push_back(tmp);
			vector<string> tmp2;
			tmp2.push_back("MOV");
			tmp2.push_back("FP");
			tmp2.push_back("SP");
			tcode.push_back(tmp2);
			vector<string> tmp3;
			tmp3.push_back("ADI");
			tmp3.push_back("SP");
			tmp3.push_back("-4");
			tcode.push_back(tmp3);
			vector<string> tmp4;
			tmp4.push_back("STR");
			tmp4.push_back(r1);
			tmp4.push_back("SP");
			tcode.push_back(tmp4);
			vector<string> tmp5;
			tmp5.push_back("ADI");
			tmp5.push_back("SP");
			tmp5.push_back("-4");
			tcode.push_back(tmp5);
			vector<string> tmp6;
			tmp6.push_back("LDA");
			tmp6.push_back(r1);
			if(quad[i][3] == "this")
			{
				tmp6.push_back(quad[i][2]);
			}
			else
			{
				tmp6.push_back(quad[i][3]);
			}
			tcode.push_back(tmp6);
			vector<string> tmp7;
			tmp7.push_back("STR");
			tmp7.push_back(r1);
			tmp7.push_back("SP");
			tcode.push_back(tmp7);
			vector<string> tmp8;
			tmp8.push_back("ADI");
			tmp8.push_back("SP");
			tmp8.push_back("-4");
			tcode.push_back(tmp8);
			reg[r1].clear();
		}
		else if(quad[i][1] == "RTN")
		{
			r1 = getregister();
			tmp.push_back("MOV");
			tmp.push_back("SP");
			tmp.push_back("FP");
			tcode.push_back(tmp);
			vector<string> tmp2;
			tmp2.push_back("LDR");
			tmp2.push_back(r1);
			tmp2.push_back("FP");
			tcode.push_back(tmp2);
            reg[r1].push_back("FP");
            r2 = getregister();
			vector<string> tmp3;
			tmp3.push_back("MOV");
			tmp3.push_back(r2);
			tmp3.push_back("FP");
			tcode.push_back(tmp3);
			vector<string> tmp4;
			tmp4.push_back("ADI");
			tmp4.push_back(r2);
			tmp4.push_back("-4");
			tcode.push_back(tmp4);
            vector<string> tmp5;
            tmp5.push_back("LDR");
            tmp5.push_back("FP");
            tmp5.push_back(r2);
            tcode.push_back(tmp5);
			vector<string> tmp8;
			tmp8.push_back("JMR");
			tmp8.push_back(r1);
			tcode.push_back(tmp8);
			reg[r1].clear();
			reg[r2].clear();
		}
		else if(quad[i][1] == "RETURN")
		{
			r1 = getregister();
			r2 = getregister();
			tmp.push_back("MOV");
			tmp.push_back("SP");
			tmp.push_back("FP");
			tcode.push_back(tmp);
			vector<string> tmp2;
			tmp2.push_back("LDR");
			tmp2.push_back(r1);
			tmp2.push_back("FP");
			tcode.push_back(tmp2);
            reg[r1].push_back("FP");
            r2 = getregister();
			vector<string> tmp3;
			tmp3.push_back("MOV");
			tmp3.push_back(r2);
			tmp3.push_back("FP");
			tcode.push_back(tmp3);
			vector<string> tmp4;
			tmp4.push_back("ADI");
			tmp4.push_back(r2);
			tmp4.push_back("-4");
			tcode.push_back(tmp4);
			vector<string> tmp5;
			tmp5.push_back("LDR");
            tmp5.push_back("FP");
            tmp5.push_back(r2);
			tcode.push_back(tmp5);
            reg[r2].push_back("FP");
            string r3 = getregister();
			vector<string> tmp6;
			tmp6.push_back("LDR");
			tmp6.push_back(r3);
			tmp6.push_back(quad[i][2]);
			tcode.push_back(tmp6);
			vector<string> tmp7;
			tmp7.push_back("STR");
			tmp7.push_back(r3);
			tmp7.push_back("SP");
			tcode.push_back(tmp7);
			vector<string> tmp8;
			tmp8.push_back("JMR");
			tmp8.push_back(r1);
			tcode.push_back(tmp8);
			reg[r1].clear();
			reg[r2].clear();
			reg[r3].clear();
		}
		else if (quad[i].size() ==1)
			tcode.push_back(tmp);
		else if (quad[i][1] == "TRP")
		{
			tmp.push_back("TRP");
			tmp.push_back(quad[i][2]);
			tcode.push_back(tmp);
		}
		else
			cout << "you are missing " << quad[i][1] << endl;

    }
	vector<string> temp;
	temp.push_back("one");
	temp.push_back(".INT");
	temp.push_back("1");
	tcode.push_back(temp);
	vector<string> temp2;
	temp2.push_back("zero");
	temp2.push_back(".INT");
	temp2.push_back("0");
	tcode.push_back(temp2);
	string tvars = stable.gettvars();
	string gvars = stable.getgvars();
	vector<vector<string> > myvars;
	string x;
	string y;
	string z;
    stringstream tss(tvars);
    while(tss >> x)
        {
			vector<string> tm;
			tm.push_back(x);
			tss >> y;
			if(y == "int" || y == "bool")
			{
				tm.push_back(".INT");
				tm.push_back("0");
			}
			else
			{
				tm.push_back(".BYT");
				tm.push_back("' '");
			}
			myvars.push_back(tm);
        }
	stringstream gss(gvars);
    while(gss >> x)
        {
			vector<string> tm;
			tm.push_back(x);
			gss >> y;
			gss >> z;
			if(y == "int" || y == "bool")
			{
				tm.push_back(".INT");
				tm.push_back(z);
			}
			else
			{
				tm.push_back(".BYT");
				tm.push_back(z);
			}
			myvars.push_back(tm);
        }
	for(int i = 0; i< myvars.size(); i ++)
	{
		tcode.push_back(myvars[i]);
	}
    return "done";
}