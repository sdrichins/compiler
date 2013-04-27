#pragma once
class symbol
{
public:
	string scope;
	string symid;
	string value;
	string kind;
    string typ;
	vector<string> sdata;
    int size;
    int off;
	symbol(string sc,string sy,string v,string k,string ty,int o)
	{
		scope = sc;
		symid = sy;
		value = v;
		kind = k;
        typ = ty;
        sdata = *new vector<string>;
        off = o;
	}
	symbol()
	{
        sdata = *new vector<string>;
    }
	void addData(string d)
	{
		sdata.push_back(d);
	}
    ~symbol(void)
	{
//        sdata.clear();
    }
};

