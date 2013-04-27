#pragma once
class symtab
{
public:
	map<string,symbol> symtable;
	symtab(void)
	{
	}
	void addsym(symbol s)
	{
/*        if(s.symid == "P103")
            cout << "I am here" << endl;
        string tmp = s.symid + "." + s.scope;
*/
        if(s.symid[0] == 'G')
        {
            map<string,symbol>::iterator it;
            for (it = symtable.begin(); it != symtable.end(); ++it)
            {
                if (it->second.value == s.value)
                {
//                    cout << "literal " + s.value + " already in symobl table" << endl;
					return;
                }
            }

        }
        if(symtable.find(s.symid) == symtable.end())
            symtable[s.symid]=s;
        else
            cout << "element already found in this scope: " << s.symid + " " + s.value <<endl;
	}
    void addSize(string symid, int x)
    {
        symtable[symid].size = x;
    }
    void updateSize(string symid, int x)
    {
        symtable[symid].size += x;
    }
	~symtab(void)
	{
	}
	symbol getsym(string x, string sc)
	{
        if(isalpha(x[0]) && isdigit(x[1]))
        {
//            string tmp = x + "."+sc;
            symbol s =symtable[x];
//            while(sc != "g")
//            {
//                if(s.scope == sc)
                    return s;
//               else
//                   sc = sc.substr(0,sc.find_last_of('.'));
//           }
//           if(s.scope == sc)
//              return s;
//            else
//                cout << "symbol not in scope"<<endl;
        }
        else if(x == "null")
        {
            return symbol("g", "null", "null", "null", "null",0);
        }
        else if(x == "true")
        {
            return symbol("g", "true", "true", "bool", "bool",0);
        }
        else if(x == "false")
        {
            return symbol("g", "false", "false", "bool", "bool",0);
        }
        else
        {
            string key;
            map<string,symbol>::iterator it;
            string tmpscope = sc;
            for (it = symtable.begin(); it != symtable.end(); ++it)
            {
                if (it->second.value == x)
                {
                    while(tmpscope != "g")
                    {
                        if(it->second.scope == tmpscope)
                            return it->second;
                        else
                            tmpscope = tmpscope.substr(0,tmpscope.find_last_of('.'));
                    }
                    if(it->second.scope == tmpscope)
                        return it->second;
                }
                tmpscope = sc;
            }
            cout << "symbol not found for " + x << endl;
            return symbol("", "", "", "", "",0);
        }
	}
	string iexist(string sc ,string x, int ln)
	{
//		map<string,symbol>::iterator it = symtable.begin();
		for(map<string,symbol>::iterator it = symtable.begin(); it != symtable.end(); it++)
		{
			string tmpscope = sc;
			symbol s = it->second;
			string value = s.value;
			if(s.value == x)
			{
				while(tmpscope != "g")
				{
					if(tmpscope == s.scope)
						return s.symid;
					else
						tmpscope = tmpscope.substr(0,tmpscope.find_last_of('.'));
				}
                if(tmpscope == s.scope)
                    return s.symid;
			}

		}
		cout <<"Element " << x <<" not found in scope line:" << ln << endl;
        return "0";
	}
    string iexistf(string sc, string z, int ln)
    {
        vector<string> args;
        string buff;
        stringstream ss(z);
        string x;
        ss >> x;
        while(ss >> buff)
        {
            args.push_back(buff);
        }
        for(map<string,symbol>::iterator it = symtable.begin(); it != symtable.end(); it++)
		{
			string tmpscope = sc;
			symbol s = it->second;
			string value = s.value;
			if(s.value == x)
			{
				while(tmpscope != "g")
				{
					if(tmpscope == s.scope)
                    {
                        int index = 0;
                        for(int i = 0; i<s.sdata.size();i++)
                        {
                            if(s.sdata[i].substr(0,5) == "Param")
                                index = i;
                        }
                        vector<string> sargs;
                        stringstream sss(s.sdata[index]);
                        string tmp;
                        string buf;
                        sss >> tmp;
                        while(sss >>buf)
                        {
                            sargs.push_back(buf);
                        }
                        if(sargs.size() != args.size())
                        {
                            cout << "Number of elements doesn't match in call to " << s.value << " line:"<<ln<<endl;
                            return "0";
                        }
                        for(int i = 0; i< args.size();i++)
                        {
                            symbol first = getsym(args[i],tmpscope);
                            symbol second = getsym(sargs[i],tmpscope);
                            if(first.typ != second.typ)
                            {
                                cout << "parameter types don't match in call to " << s.value << " line:" << ln << endl;;
                                return "0";
                            }
                        }
						return s.symid;
                    }
					else
						tmpscope = tmpscope.substr(0,tmpscope.find_last_of('.'));
				}
                if(tmpscope == s.scope)
                {
                    int index = 0;
                    for(int i = 0; i<s.sdata.size();i++)
                    {
                        if(s.sdata[i].substr(0,5) == "Param")
                            index = i;
                    }
                    vector<string> sargs;
                    stringstream sss(s.sdata[index]);
                    string tmp;
                    string buf;
                    sss >> tmp;
                    while(sss >>buf)
                    {
                        sargs.push_back(buf);
                    }
                    if(sargs.size() != args.size())
                    {
                        cout << "Number of elements doesn't match in call to " << s.value << " line:" << ln << endl;
                    }
                    for(int i = 0; i< sargs.size();i++)
                    {
                        symbol first = getsym(args[i],tmpscope);
                        symbol second = getsym(sargs[i],tmpscope);
                        if(first.typ != second.typ)
                        {
                            cout << "parameter types don't match in call to " << s.value << " line:" << ln << endl;
                            return "0";
                        }
                    }
                }
                else
                    cout << "Element " + x +" not in this scope line:" << ln <<endl;
			}
            
		}
		cout << "Element " + x + " not found line:"<< ln << endl;
        return "0";
    }
	bool texist(string x)
	{
        if(x == "int" || x == "char" || x == "bool" || x == "void")
            return true;
		for(map<string,symbol>::iterator it = symtable.begin(); it != symtable.end(); it++)
		{
			symbol s = it->second;
			string value = s.value;
			if(s.value == x)
			{
				return true;
			}
		}
        cout << " type " << x <<" not found" << endl;
		return false;
	}
	string rexistf(string cs, string sc,string z, int ln)
	{
        vector<string> args;
        string buff;
        stringstream ss(z);
        string x;
        ss >> x;
        while(ss >> buff)
        {
            args.push_back(buff);
        }
        for(map<string,symbol>::iterator it = symtable.begin(); it != symtable.end(); it++)
		{
			string tmpscope = sc;
			symbol s = it->second;
			string value = s.value;
			if(s.value == x)
			{
				while(tmpscope != "g")
				{
					if(tmpscope == s.scope)
                    {
                        int index = -1;
                        for(int i = 0; i<s.sdata.size();i++)
                        {
                            if(s.sdata[i].substr(0,5) == "Param")
                                index = i;
                        }
                        vector<string> sargs;
                        if(index != -1)
                        {
                            stringstream sss(s.sdata[index]);
                            string tmp;
                            string buf;
                            sss >> tmp;
                            while(sss >>buf)
                        {
                                sargs.push_back(buf);
                            }
                        }
                        if(sargs.size() != args.size())
                        {
                            cout << "Number of elements doesn't match in call to " << s.value << " line:"<< ln <<endl;
                            return "0";
                        }
                        for(int i = 0; i< args.size();i++)
                        {
                            symbol first = getsym(args[i],cs);
                            symbol second = getsym(sargs[i],sc+"."+s.value);
                            if(first.typ != second.typ)
                            {
                                cout << "parameter types don't match in call to " << s.value << " line" << ln << endl;
                                return "0";
                            }
                        }
						return s.symid;
                    }
					else
						tmpscope = tmpscope.substr(0,tmpscope.find_last_of('.'));
				}
                if(tmpscope == s.scope)
                {
                    int index = 0;
                    for(int i = 0; i<s.sdata.size();i++)
                    {
                        if(s.sdata[i].substr(0,5) == "Param")
                            index = i;
                    }
                    vector<string> sargs;
                    stringstream sss(s.sdata[index]);
                    string tmp;
                    string buf;
                    sss >> tmp;
                    while(sss >>buf)
                    {
                        sargs.push_back(buf);
                    }
                    if(sargs.size() != args.size())
                    {
                        cout << "Number of elements doesn't match in call to " << s.value << " line:" << ln <<endl;
                    }
                    for(int i = 0; i< sargs.size();i++)
                    {
                        if(getsym(args[i],cs).kind != getsym(sargs[i],tmpscope).kind)
                        {
                            cout << "parameter types don't match in call to " << s.value << " line:" << ln << endl;
                            return 0;
                        }
                    }
                }
                else
                    cout << "Element " << x << " not in this scope line:" << ln <<endl;
			}
            
		}
		cout << "Element " << x << " not found line:" << ln << endl;
        return "0";
	}
	string getgvars()
	{
		string v = "";
		for(map<string,symbol>::iterator it = symtable.begin(); it != symtable.end(); it++)
		{
			symbol s = it->second;
			if(s.symid[0] == 'G')
				v += s.symid + " " + s.typ + " " + s.value + " ";
		}
		return v;
	}
	string gettvars()
	{
		string v = "";
		for(map<string,symbol>::iterator it = symtable.begin(); it != symtable.end(); it++)
		{
			symbol s = it->second;
			if(s.symid[0] == 't')
				v += s.symid + " " + s.typ + " ";
            if(s.symid[0] == 'P')
                v += s.symid + " " + s.typ + " ";
		}
		return v;
	}

};
