#pragma once
class opr
{
public:
	int priority;
	string value;
	opr(string v)
	{
		value = v;
        if(value == "=")
            priority = 0;
		else if(value == "(" || value == "[")
			priority = 1;
		else if(value == "+" || value == "-")
			priority = 2;
		else if(value == "*" || value == "/" || value == "%")
			priority = 3;
		else if(value == ")" || value == ";"|| value =="," || value == "]")
			priority = 4;
		else
			priority = 1;
	}

	~opr(void)
	{
	}
};

