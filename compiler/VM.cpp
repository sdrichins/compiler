#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>
#include <string.h>
#include <cstdlib>
using namespace std;
const int MAX = 4000;
int WS = 4;
int SB = 0;
int TS = 300;
int SP = 0;
void loadstate(int x);
void savestate(int x);
int main(int argc,char* argv[])
{
	map<string,int> lblmap;
	map<string,int>::iterator lblit;
	map<string,int> instructs;
	map<string,int>::iterator instit;
	instructs.insert(make_pair("JMP",1));
	instructs.insert(make_pair("JMR",2));
	instructs.insert(make_pair("BNZ",3));
	instructs.insert(make_pair("BGT",4));
	instructs.insert(make_pair("BLT",5));
	instructs.insert(make_pair("BRZ",6));
	instructs.insert(make_pair("MOV",7));
	instructs.insert(make_pair("LDA",8));
	instructs.insert(make_pair("STR",9));
	instructs.insert(make_pair("LDR",10));
	instructs.insert(make_pair("STB",11));
	instructs.insert(make_pair("LDB",12));
	instructs.insert(make_pair("ADD",13));
	instructs.insert(make_pair("ADI",14));
	instructs.insert(make_pair("SUB",15));
	instructs.insert(make_pair("MUL",16));
	instructs.insert(make_pair("DIV",17));
	instructs.insert(make_pair("AND",18));
	instructs.insert(make_pair("OR",19));
	instructs.insert(make_pair("CMP",20));
	instructs.insert(make_pair("TRP",21));
	instructs.insert(make_pair("RUN",26));
	instructs.insert(make_pair("END",27));
	instructs.insert(make_pair("BLK",28));
	instructs.insert(make_pair("LCK",29));
	instructs.insert(make_pair("ULK",30));
	char mem[MAX] = "";
	char *charptr;
	int *intptr;
	int memcount = 0;
	int instcount = 0;
	int reg[15]= {0};
	reg[14] = MAX - 68 -1;
	reg[13] = MAX - 68 -1;
	reg[12] = MAX - TS;
	reg[11] = MAX -1;
	int Threadcount = 0;
	int instcnt = 0;
	int currentthread = 0;
	int threads[6] = {0};
	int opcode[500][3] = {0};
	bool firstpass = true;
	bool secondpass = false;
	bool assembledone = false;
	string mychar;
	if(argc <=1)
    {
        cout << "Usage: " << argv[0] << " <Filename>" << endl;
        return 0;
    }
/*    char filename[20];
	cout <<"input filename:";
	cin>> filename;
	*/
	char* filename;
	filename = argv[1];
	ifstream myfile;
	while(!assembledone)
	{
		myfile.open(filename);
		if (!myfile.is_open())
		{
			cout <<"\nFile could not be opened"<<endl;
			cin.get();
			return 0;
		}
		else
		{
			char line[35];
			char *mytok;
			while(myfile.getline(line,200))
			{
				mytok = strtok(line," '");
				instit = instructs.find(mytok);
//				char* tmp1 = mytok;
				string name = mytok;
				char* operand1, *operand2;
				if(instit != instructs.end())
				{
					if(firstpass)
					{
						if(name == "END" || name == "BLK")
						{
							mytok = strtok(NULL, " ");
							if(mytok != NULL)
								cout << "Improper use of "<<name<<" command"<<endl;
						}
						else if(name == "JMP" || name == "JMR" || name == "TRP"|| name =="LCK" ||name =="ULK")
						{
							mytok = strtok(NULL, " ");
							operand1 = mytok;
							mytok = strtok(NULL, " ");
							if(mytok != NULL)
								cout << "Improper use of "<<name<<" command"<<endl;
						}
						else
						{
							mytok = strtok(NULL, " ");
							operand1 = mytok;
							mytok = strtok(NULL, " ");
							if(mytok == NULL)
								cout << "Missing opperand in " <<name << " command"<<endl;
							else
								operand2 = mytok;
							mytok = strtok(NULL, " ");
							if(mytok != NULL)
								cout << "Improper use of "<<name<<" command"<<endl;
						}
						instcount++;
					}
					else if(secondpass)
					{
						bytecode:
						instit = instructs.find(name);
						if(instit != instructs.end())
							opcode[instcount][0] = instit->second;
						if(name == "END" || name == "BLK")
						{
							instcount++;
						}
						else if(name == "JMP" || name == "JMR" || name == "TRP"|| name =="LCK" ||name =="ULK")
						{
							mytok = strtok(NULL, " ");
							operand1 = mytok;
							string tmp2 = operand1;
							if( name =="JMP"|| name =="LCK" ||name =="ULK")
							{
								instit = lblmap.find(operand1);
								opcode[instcount][1] = instit->second;
							}
							else if(tmp2 =="R0")
								opcode[instcount][1] = 0;
							else if(tmp2 =="R1")
								opcode[instcount][1] = 1;
							else if(tmp2 =="R2")
								opcode[instcount][1] = 2;
							else if(tmp2 =="R3")
								opcode[instcount][1] = 3;
							else if(tmp2 == "R4")
								opcode[instcount][1] = 4;
							else if(tmp2 == "R5")
								opcode[instcount][1] = 5;
							else if(tmp2 == "R6")
								opcode[instcount][1] = 6;
							else if(tmp2 == "R7")
								opcode[instcount][1] = 7;
							else if(tmp2 == "R8")
								opcode[instcount][1] = 8;
							else if(tmp2 == "R9")
								opcode[instcount][1] = 9;
							else if(tmp2 == "R10")
								opcode[instcount][1] = 10;
							else if(tmp2 == "R11")
								opcode[instcount][1] = 11;
							else if(tmp2 == "R12")
								opcode[instcount][1] = 12;
							else if(tmp2 == "R13")
								opcode[instcount][1] = 13;
							else if(tmp2 == "R14")
								opcode[instcount][1] = 14;
							else
								opcode[instcount][1] = atoi (operand1);
							instcount++;
						}
						else if(name =="BNZ" || name =="BGT" || name=="BLT" ||name =="BRZ"
					|| name =="LDA" || name =="STR" || name =="LDR"|| name =="STB"|| name =="LDB"||name =="RUN")
						{
							mytok = strtok(NULL, " ");
							operand1 = mytok;
							string tmp2 = operand1;
							if(tmp2 =="R0")
								opcode[instcount][1] = 0;
							else if(tmp2 =="R1")
								opcode[instcount][1] = 1;
							else if(tmp2 =="R2")
								opcode[instcount][1] = 2;
							else if(tmp2 =="R3")
								opcode[instcount][1] = 3;
							else if(tmp2 == "R4")
								opcode[instcount][1] = 4;
							else if(tmp2 == "R5")
								opcode[instcount][1] = 5;
							else if(tmp2 == "R6")
								opcode[instcount][1] = 6;
							else if(tmp2 == "R7")
								opcode[instcount][1] = 7;
							else if(tmp2 == "R8")
								opcode[instcount][1] = 8;
							else if(tmp2 == "R9")
								opcode[instcount][1] = 9;
							else if(tmp2 == "R10")
								opcode[instcount][1] = 10;
							else if(tmp2 == "SB")
								opcode[instcount][1] = 11;
							else if(tmp2 == "SL")
								opcode[instcount][1] = 12;
							else if(tmp2 == "FP")
								opcode[instcount][1] = 13;
							else if(tmp2 == "SP")
								opcode[instcount][1] = 14;
							mytok = strtok(NULL, " ");
							operand2 = mytok;
							string tmp3 = operand2;
							lblit = lblmap.find(operand2);
							if(lblit != lblmap.end())
								opcode[instcount][2] = lblit->second;
							else
							{
								if(name == "STR")
									opcode[instcount][0] = 22;
								if(name == "LDR")
									opcode[instcount][0] = 23;
								if(name == "STB")
									opcode[instcount][0] = 24;
								if(name == "LDB")
									opcode[instcount][0] = 25;
								if(tmp3 =="R0")
									opcode[instcount][2] = 0;
								else if(tmp3 =="R1")
									opcode[instcount][2] = 1;
								else if(tmp3 =="R2")
									opcode[instcount][2] = 2;
								else if(tmp3 =="R3")
									opcode[instcount][2] = 3;
								else if(tmp3 == "R4")
									opcode[instcount][2] = 4;
								else if(tmp3 == "R5")
									opcode[instcount][2] = 5;
								else if(tmp3 == "R6")
									opcode[instcount][2] = 6;
								else if(tmp3 == "R7")
									opcode[instcount][2] = 7;
								else if(tmp3 == "R8")
									opcode[instcount][2] = 8;
								else if(tmp3 == "R9")
									opcode[instcount][2] = 9;
								else if(tmp3 == "R10")
									opcode[instcount][2] = 10;
								else if(tmp3 == "SB")
									opcode[instcount][2] = 11;
								else if(tmp3 == "SL")
									opcode[instcount][2] = 12;
								else if(tmp3 == "FP")
									opcode[instcount][2] = 13;
								else if(tmp3 == "SP")
									opcode[instcount][2] = 14;
								else if(tmp3 == "PC")
									opcode[instcount][2] = instcount;
							}
							instcount++;
						}
						else
						{
							mytok = strtok(NULL, " ");
							operand1 = mytok;
							string tmp2 = operand1;
							if(tmp2 =="R0")
								opcode[instcount][1] = 0;
							else if(tmp2 =="R1")
								opcode[instcount][1] = 1;
							else if(tmp2 =="R2")
								opcode[instcount][1] = 2;
							else if(tmp2 =="R3")
								opcode[instcount][1] = 3;
							else if(tmp2 == "R4")
								opcode[instcount][1] = 4;
							else if(tmp2 == "R5")
								opcode[instcount][1] = 5;
							else if(tmp2 == "R6")
								opcode[instcount][1] = 6;
							else if(tmp2 == "R7")
								opcode[instcount][1] = 7;
							else if(tmp2 == "R8")
								opcode[instcount][1] = 8;
							else if(tmp2 == "R9")
								opcode[instcount][1] = 9;
							else if(tmp2 == "R10")
								opcode[instcount][1] = 10;
							else if(tmp2 == "SB")
								opcode[instcount][1] = 11;
							else if(tmp2 == "SL")
								opcode[instcount][1] = 12;
							else if(tmp2 == "FP")
								opcode[instcount][1] = 13;
							else if(tmp2 == "SP")
								opcode[instcount][1] = 14;
							mytok = strtok(NULL, " ");
							operand2 = mytok;
							string tmp3 = operand2;
							if(tmp3 =="R0")
								opcode[instcount][2] = 0;
							else if(tmp3 =="R1")
								opcode[instcount][2] = 1;
							else if(tmp3 =="R2")
								opcode[instcount][2] = 2;
							else if(tmp3 =="R3")
								opcode[instcount][2] = 3;
							else if(tmp3 == "R4")
								opcode[instcount][2] = 4;
							else if(tmp3 == "R5")
								opcode[instcount][2] = 5;
							else if(tmp3 == "R6")
								opcode[instcount][2] = 6;
							else if(tmp3 == "R7")
								opcode[instcount][2] = 7;
							else if(tmp3 == "R8")
								opcode[instcount][2] = 8;
							else if(tmp3 == "R9")
								opcode[instcount][2] = 9;
							else if(tmp3 == "R10")
								opcode[instcount][2] = 10;
							else if(tmp3 == "SB")
								opcode[instcount][2] = 11;
							else if(tmp3 == "SL")
								opcode[instcount][2] = 12;
							else if(tmp3 == "FP")
								opcode[instcount][2] = 13;
							else if(tmp3 == "SP")
								opcode[instcount][2] = 14;
							else
								opcode[instcount][2] = atoi(operand2);
							instcount++;
						}
					}

				}
				else
				{
					char* mytmpint = mytok;
					string myint = ".INT";
					if(name == ".BYT")
						goto bytadd;
					if(name ==".INT")
						goto intadd;
					if(firstpass)
					{
						char* type;
						int intval;
						char* charval;
						mytok = strtok (NULL, " . '");
						type = mytok;
						if(strcmp(type,"INT") == 0)
						{
							lblmap.insert(make_pair(name,memcount));
							intadd:
							mytok = strtok (NULL, "	'");
							intval = atoi (mytok);
							intptr = reinterpret_cast<int*>(&mem[memcount]);
							*intptr = intval;
							memcount += 4;
						}
						else if(strcmp(type,"BYT") ==0)
						{
							lblmap.insert(make_pair(name,memcount));
							bytadd:
							mytok = strtok (NULL, "	'");
							charval = mytok;
							if(charval == NULL)
								charval = " ";
							if(strcmp(charval,"\\n") ==0)
								*charval = '\n';
							charptr = &mem[memcount];
							*charptr = *charval;
							memcount++;
						}
						else
						{
							lblmap.insert(make_pair(name,instcount));
							instcount++;
						}
					}
					if(secondpass)
					{
						if(name != ".INT" && name != ".BYT")
						{
							lblit = lblmap.find(name);
							if(lblit == lblmap.end())
								cout<<"label " <<name<<" wasn't loaded"<<endl;
							mytok = strtok (NULL, ". ':");
							name = mytok;
							instit = instructs.find(name);
							if(instit != instructs.end())
								goto bytecode;
						}

					}
				}
			}
		}
		myfile.close();
		if(secondpass)
		{
//			reg[12] = memcount;
			assembledone =true;
		}
		firstpass = false;
		secondpass = true;
		instcount = 0;
	}
		int PC = 0;
		bool Running = true;
		char * tmpchar;
		int * tmpint;
		int mycount = 0;
		while(Running)
		{
			mycount++;
			int index = 0;
			switch(opcode[PC][0])
			{
			case 1:
				PC = opcode[PC][1];
				break;
			case 2:
				PC = reg[opcode[PC][1]];
				break;
			case 3:
				if(reg[opcode[PC][1]] != 0)
				 PC = opcode[PC][2];
				else
					PC++;
				break;
			case 4:
				if(reg[opcode[PC][1]] > 0)
				 PC = opcode[PC][2];
				else
					PC++;
				break;
			case 5:
				if(reg[opcode[PC][1]] < 0)
				 PC = opcode[PC][2];
				else
					PC++;
				break;
			case 6:
				if(reg[opcode[PC][1]] == 0)
				 PC = opcode[PC][2];
				else
					PC++;
				break;
			case 7:
				reg[opcode[PC][1]] = reg[opcode[PC][2]];
				PC++;
				break;
			case 8:
				reg[opcode[PC][1]] = opcode[PC][2];
				PC++;
				break;
			case 9:
				tmpint = reinterpret_cast<int*>(&mem[opcode[PC][2]]);
				*tmpint = reg[opcode[PC][1]];
				PC++;
				break;
			case 10:
				reg[opcode[PC][1]] = 0;
				tmpint = reinterpret_cast<int*>(&mem[opcode[PC][2]]);
				reg[opcode[PC][1]] = *tmpint;
				PC++;
				break;
			case 11:
				tmpchar = reinterpret_cast<char*>(&reg[opcode[PC][1]]);
				mem[opcode[PC][2]] = *tmpchar;
				PC++;
				break;
			case 12:
				reg[opcode[PC][1]] = 0;
				tmpchar = reinterpret_cast<char*>(&reg[opcode[PC][1]]);
				*tmpchar = mem[opcode[PC][2]];
				PC++;
				break;
			case 13:
				reg[opcode[PC][1]] += reg[opcode[PC][2]];
				PC++;
				break;
			case 14:
				reg[opcode[PC][1]] += opcode[PC][2];
				PC++;
				break;
			case 15:
				reg[opcode[PC][1]] -= reg[opcode[PC][2]];
				PC++;
				break;
			case 16:
				reg[opcode[PC][1]] *= reg[opcode[PC][2]];
				PC++;
				break;
			case 17:
				reg[opcode[PC][1]] /= reg[opcode[PC][2]];
				PC++;
				break;
			case 18:
				PC++;
				break;
			case 19:
				PC++;
				break;
			case 20:
				if(reg[opcode[PC][1]] ==reg[opcode[PC][2]])
					reg[opcode[PC][1]] = 0;
				else if (reg[opcode[PC][1]] > reg[opcode[PC][2]])
					reg[opcode[PC][1]] = 1;
				else
					reg[opcode[PC][1]] = -1;
				PC++;
				break;
			case 21:
				if(opcode[PC][1] == 0)
					Running = false;
				if(opcode[PC][1] == 1)
				{
					cout << reg[1];
				}
				if(opcode[PC][1] == 2)
				{
					int myint;
			//		reg[2] = getchar();
					char* tmpchar2 = reinterpret_cast<char*>(&reg[2]);
					*tmpchar2 = getchar();
					reg[2] = atoi(tmpchar2);
					myint = getchar();
					
				}
				if(opcode[PC][1] == 3)
				{
					tmpchar = reinterpret_cast<char*>(&reg[3]);
					cout << tmpchar;
				}
				if(opcode[PC][1] == 4)
				{
					char* tmpchar2 = reinterpret_cast<char*>(&reg[4]);
					*tmpchar2 = getchar();
				}
				if(opcode[PC][1] == 10)
				{
					int inttemp;
					tmpchar = reinterpret_cast<char*>(&reg[0]);
					inttemp = atoi(tmpchar);
					if(inttemp == 0)
					{
						if(*tmpchar != '0')
							inttemp = -1;
					}
					reg[0] = inttemp;
				}
				if(opcode[PC][1] == 11)
				{
					tmpchar = reinterpret_cast<char*>(&reg[0]);
					itoa(reg[0],tmpchar,10);
				}
				PC++;
				break;
			case 22: // STR R R
				tmpint = reinterpret_cast<int*>(&mem[reg[opcode[PC][2]]]);
				*tmpint = reg[opcode[PC][1]];
				PC++;
				break;
			case 23: // LDR R R
				reg[opcode[PC][1]] = 0;
				tmpint = reinterpret_cast<int*>(&mem[reg[opcode[PC][2]]]);
				reg[opcode[PC][1]] = *tmpint;
				PC++;
				break;
			case 24: // STB R R
				tmpchar = reinterpret_cast<char*>(&reg[opcode[PC][1]]);
				mem[reg[opcode[PC][2]]] = *tmpchar; 
				PC++;
				break;
			case 25: // LDB R R
				reg[opcode[PC][1]] = 0;
				tmpchar = reinterpret_cast<char*>(&reg[opcode[PC][1]]);
				*tmpchar = mem[reg[opcode[PC][2]]];
				PC++;
				break;
			case 26:
				Threadcount +=1;
				threads[Threadcount] = Threadcount;
				reg[opcode[PC][1]] = Threadcount;
				SB = MAX -1-(TS*Threadcount);
				tmpint = reinterpret_cast<int*>(&mem[SB-4]);
				*tmpint = opcode[PC][2];
				tmpint = reinterpret_cast<int*>(&mem[SB-8]);
				*tmpint = Threadcount;
				tmpint = reinterpret_cast<int*>(&mem[SB-52]);
				*tmpint = SB;
				tmpint = reinterpret_cast<int*>(&mem[SB-56]);
				*tmpint = SB-TS;
				tmpint = reinterpret_cast<int*>(&mem[SB-60]);
				*tmpint = SB -68;
				tmpint = reinterpret_cast<int*>(&mem[SB-64]);
				*tmpint = SB -68;
				PC++;
				cout<<"creating thread "<<Threadcount<<endl;
				break;
			case 27:
				if(reg[0] != 0)
				{
					for(int i = 0;i<6;i++)
					{
						if(threads[i] == reg[0])
						{
							cout<<"Ending thread "<<reg[0]<<endl;
							threads[i] = 0;
							index = i;
						}
					}
					for(int j = index;j<6;j++)
						threads[j] = threads[j+1];
					threads[6-1] = 0;
					Threadcount--;
					goto load;
				}	
				PC++;
				break;
			case 28:
				if(reg[0] ==0)
					if(Threadcount !=0)
						break;
				PC++;
				break;
			case 29:
				tmpint = reinterpret_cast<int*>(&mem[opcode[PC][1]]);
				if(*tmpint != -1)
					break;
				else
				{
					*tmpint = reg[0];
					cout <<"Thread "<<reg[0]<<" has lock "<<opcode[PC][1]<<endl;
				}
				PC++;
				break;
			case 30:
				tmpint = reinterpret_cast<int*>(&mem[opcode[PC][1]]);
				if(*tmpint != reg[0])
					break;
				else
				{
					*tmpint = -1;
					cout <<"Thread "<<reg[0]<<" unlocking "<<opcode[PC][1]<<endl;
				}
				PC++;
				break;
			}
			instcnt ++;
			if(instcnt %3 == 0)
			{
				
				SB = (MAX-1) -(threads[currentthread] *TS);
				SP = SB -4;
				tmpint = reinterpret_cast<int*>(&mem[SP]);
				*tmpint = PC;
				SP -= 4;
				for(int i = 0; i<15;i++)
				{
					tmpint = reinterpret_cast<int*>(&mem[SP- (i*4)]);
					*tmpint = reg[i];
				}

				currentthread +=1;
				load:
				if(currentthread >Threadcount)
					currentthread = 0;
				SB = (MAX-1) -(threads[currentthread] *TS);
				SP = SB -4;
				tmpint = reinterpret_cast<int*>(&mem[SP]);
				PC = *tmpint;
				SP -=4;
				for(int i = 0; i<15;i++)
				{
					tmpint = reinterpret_cast<int*>(&mem[SP- (i*4)]);
					reg[i] = *tmpint;
				}
			}
		}
		
//	system("PAUSE");
	return 0;
}

