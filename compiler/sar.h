#pragma once
class sar
{
public:
	string action;
	string value;
	string ty;
    bool isfunc;
    bool isarr;
    vector<string> args;
	sar(string val, string act,string t ="",bool f = false, bool a = false)
	{
		action = act;
		value = val;
		ty = t;
        isfunc = f;
        isarr = a;
	}
    void addarg(string x)
    {
        args.push_back(x);
    }

	~sar(void)
	{
	}
};

