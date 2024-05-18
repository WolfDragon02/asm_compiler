#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <string>
using namespace std;

void die(int line_no = 0) {
	cout << "Syntax Error on line " << line_no << endl;
	exit(1);
}

//void print_number(int x) {cout << x << endl;}
int var(string s) {
	if (s == "A") return 4;
	if (s == "B") return 5;
	if (s == "C") return 6;
	if (s == "I") return 7;
	if (s == "J") return 8;
	if (s == "X") return 9;
	if (s == "Y") return 10;
	if (s == "Z") return 11;
	else return -1;
}
int var_char(char c) {
	if (c == 'A') return 4;
	if (c == 'B') return 5;
	if (c == 'C') return 6;
	else return -1;
}
bool isNumber(const string& str) {
	for (char c : str) {
		if (!isdigit(c)) {
			return false;
		}
	}
	return true;
}

int main(int argc, char **argv) {
	//If we pass any parameters, we'll just generate an assembly file 
	//Otherwise we will generate an assembly file, assemble it, and run it
	bool assemble_only = false;
	if (argc > 1) assemble_only = true;
	ofstream outs("main.s"); //This is the assembly file we're creating
	if (!outs) {
		cout << "Error opening file.\n";
		return -1;
	}
	outs << ".global main\nmain:\n"; //Get the file ready to rock
	outs << "\tPUSH {LR}\n\tPUSH {R4-R12}\n\n";

	int line_no = 0;
	//bool loop_break = false; //for print string
	bool for_str = false; // for print string
	string print_str = "None"; // for print string
	vector<string> vec_str;
	while (cin) {
		//vector<int> numbers;
		string s;
		getline(cin,s);
		line_no++;
		if (!cin) break;
		transform(s.begin(), s.end(), s.begin(), ::toupper); //Uppercaseify
		auto it = s.find("QUIT"); //TERMINATE COMPILER
		if (it != string::npos) {
			//loop_break = true;
			break;
		}
		stringstream ss(s); //Turn s into a stringstream
		int label;
		ss >> label;
		if (!ss) die(line_no);
		outs << "line_" << label << ":\n"; //Write each line number to the file ("line_20:")
		string command;
		ss >> command;
		if (!ss) die(line_no);
		if (command.size() > 5) die(line_no);
		if (command == "REM") {
			continue;
		}
		else if (command == "GOTO") {
			int target;
			ss >> target;
			if (!ss) die(line_no);
			outs << "\tBAL line_" << target << endl;
			continue;
		}
		else if (command == "EXIT" or command == "END") {
			outs << "\tBAL quit\n";
			continue;
		}

		//YOU: Put all of your code here, interpreting the different commands in BB8
		else if (command == "PRINT") {
			string c; 
			ss >> c;
			//cout << c << endl;
			if (!ss) die(line_no);
			//bool check = find(c.begin(), c.end(), '"') != c.end();
			if (c != "A" and c != "B" and c != "C" and c != "I" and c != "J" and c != "X" and c != "Y" and c != "Z" and c.find('"') != 0) die(line_no);
			if (c == "A" or c == "B" or c == "C" or c == "I" or c == "J" or c == "X" or c == "Y" or c == "Z") {
				if (var(c) == -1) die(line_no);
				outs << "\tMOV R0, R" << var(c) << endl;
				outs << "\tBL print_number" << endl;
			}
			if (c.find('"') == 0 and c.rfind('"') == c.length() - 1) {
				//string words;
				//getline(cin,words);
				outs << "_read:" << endl;
				outs << "\tMOV R7, #3" << endl;
				outs << "\tMOV R0, #0" << endl;
				outs << "\tMOV R2, #" << c.size() << endl;
				outs << "\tLDR R0,=string" << endl;
				outs << "_write:" << endl;
				outs << "\tMOV R7, #4" << endl;
				outs << "\tMOV R0, #1" << endl;
				outs << "\tMOV R2, #" << c.size() << endl;
				outs << "\tLDR R0,=string" << endl;
				outs << "\tBL print_string" << endl;
				
				for_str = true;
				c.erase(remove(c.begin(), c.end(), '"'), c.end());
				print_str = c;
				vec_str.push_back(c);
				//cout << c << endl;
				//outs << ".data" << endl;
				//outs << "string:" << endl;
				//outs << ".asciz " << c << endl;
				
			}
			continue;
		}
		else if (command == "INPUT") {
			string s;
			ss >> s;
			if (!ss) die(line_no);
			outs << "\tBL read_number" << endl; 
		}
		else if (command == "LET") {
			string first;
			ss >> first;
			if (!ss) die(line_no);
			if (first == "A" or first == "B" or first == "C" or first == "I" or first == "J" or first == "X" or first == "Y" or first == "Z") {
				//outs << "\tMOV R" << var(c) << ", #0" << endl;
				string operand;
				ss >> operand;
				if (!ss) die(line_no);
				if (operand == "=") {
					string sec;
					ss >> sec;
					if (!ss) die(line_no);
					if (isNumber(sec)) {
						outs << "\tMOV R" << var(first) << ", #" << sec << endl;
					} else {	
						if (sec == "A" or sec == "B" or sec == "C" or sec == "I" or sec == "J" or sec == "X" or sec == "Y" or sec == "Z") {							    				
							//outs << "\tMOV R" << var(first) << ", R" << var(sec) << endl;	
							string op; 
							ss >> op; 
							if (!ss) die(line_no);
							if (op == "+") {
								string third;
								ss >> third;
								if (!ss) die(line_no);
								outs << "\tADD R" << var(first) << ", R" << var(sec) << ", R" << var(third) << endl;
							}
							if (op == "-") {
								string third;
								ss >> third;
								if (!ss) die(line_no);
								outs << "\tSUB R" << var(first) << ", R" << var(sec) << ", R" << var(third) << endl;
							}
							if (op == "*") {
								string third;
								ss >> third;
								if (!ss) die(line_no);
								outs << "\tMUL R" << var(first) << ", R" << var(sec) << ", R" << var(third) << endl;
							}
							
						}
					}
				}
			}
			continue;
		}
		else if (command == "IF") {
			string first;
			ss >> first;
			if (!ss) die(line_no);
			if (first == "A" or first == "B" or first == "C" or first == "I" or first == "J" or first == "X" or first == "Y" or first == "Z") {
				string express;
				ss >> express;
				if (!ss) die(line_no);
				if (express != ">" and express != ">=" and express != "<" and express != "<=" and express != "==" and express != "!=") die(line_no);
				if (express == ">") {
					string sec, then;
					ss >> sec;
					if (!ss) die(line_no);
					outs << "\tCMP R" << var(first) << ", R" << var(sec) << endl;
					outs << "\tBGT ";
					ss >> then;
					if (!ss) die(line_no);
					if (then == "THEN") {
						string go;
						ss >> go;
						if (!ss) die(line_no);
						if (go == "GOTO") {
							string line;
							ss >> line;
							if (!ss) die(line_no);
							outs << "line_" << line << endl;
						}
					}
				}
				if (express == ">=") {
					string sec, then;
					ss >> sec;
					if (!ss) die(line_no);
					outs << "\tCMP R" << var(first) << ", R" << var(sec) << endl;
					outs << "\tBGE ";
					ss >> then;
					if (!ss) die(line_no);
					if (then == "THEN") {
						string go;
						ss >> go;
						if (!ss) die(line_no);
						if (go == "GOTO") {
							string line;
							ss >> line;
							if (!ss) die(line_no);
							outs << "line_" << line << endl;
						}
					}
				}
				if (express == "<") {
					string sec, then;
					ss >> sec;
					if (!ss) die(line_no);
					outs << "\tCMP R" << var(first) << ", R" << var(sec) << endl;
					outs << "\tBLT ";
					ss >> then;
					if (!ss) die(line_no);
					if (then == "THEN") {
						string go;
						ss >> go;
						if (!ss) die(line_no);
						if (go == "GOTO") {
							string line;
							ss >> line;
							if (!ss) die(line_no);
							outs << "line_" << line << endl;
						}
					} 
				}
				if (express == "<=") {
					string sec, then;
					ss >> sec;
					if (!ss) die(line_no);
					outs << "\tCMP R" << var(first) << ", R" << var(sec) << endl;
					outs << "\tBLE ";
					ss >> then;
					if (!ss) die(line_no);
					if (then == "THEN") {
						string go;
						ss >> go;
						if (!ss) die(line_no);
						if (go == "GOTO") {
							string line;
							ss >> line;
							if (!ss) die(line_no);
							outs << "line_" << line << endl;
						}
					} 
				}
				if (express == "==") {
					string sec, then;
					ss >> sec;
					if (!ss) die(line_no);
					outs << "\tCMP R" << var(first) << ", R" << var(sec) << endl;
					outs << "\tBEQ ";
					ss >> then;
					if (!ss) die(line_no);
					if (then == "THEN") {
						string go;
						ss >> go;
						if (!ss) die(line_no);
						if (go == "GOTO") {
							string line;
							ss >> line;
							if (!ss) die(line_no);
							outs << "line_" << line << endl;
						}
					} 
				}
				if (express == "!=") {
					string sec, then;
					ss >> sec;
					if (!ss) die(line_no);
					outs << "\tCMP R" << var(first) << ", R" << var(sec) << endl;
					outs << "\tBNE ";
					ss >> then;
					if (!ss) die(line_no);
					if (then == "THEN") {
						string go;
						ss >> go;
						if (!ss) die(line_no);
						if (go == "GOTO") {
							string line;
							ss >> line;
							if (!ss) die(line_no);
							outs << "line_" << line << endl;
						}
					} 
				}
				
			} 
			continue;
		}
	}

	//Clean up the file at the bottom
	/*
	if (for_str) {
		outs << ".data" << endl;
		outs << "string:" << endl;
		outs << ".asciz " << print_str << endl;	
	}*/
	outs << "\nquit:\n\tMOV R0, #42\n\tPOP {R4-R12}\n\tPOP {PC}\n"; //Finish the code and return
	//outs << "\nquit:\n\tPOP {R4-R12}\n\tPOP {PC}\n";
	//cout << boolalpha << for_str << endl;
	//for_str = true;
	//cout << print_str << endl;
	if (for_str == true) {
		outs << ".data" << endl;
		outs << "string:" << endl;
		for (string &ch : vec_str) {
			if (ch == print_str) {
				string end = "\n";
				outs << ".asciz " << '"' << ch+end << '"';	
			}
		}
	}
	outs.close(); //Close the file

	if (assemble_only) return 0; //When you're debugging you should run bb8 with a parameter

	//print.o is made from the Makefile, so make sure you make your code
	if (system("g++ main.s print.o")) { //Compile your assembler code and check for errors
		cout << "Assembling failed, which means your compiler screwed up.\n";
		return 1;
	}
	//We've got an a.out now, so let's run it!
	cout << "Compilation successful. Executing program now." << endl;
	execve("a.out",NULL,NULL); //Loader - this will replace your code running with a.out
}
