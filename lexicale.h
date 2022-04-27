#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct cell
{
	string nom;
	string type;
};

class lexicale
{	
private:
	static unsigned** matKw;
	static string tab_c;
	static unsigned* tab_f;
	static char op_arth [];
	static char op_log [];
	static char op_cmp [];
	static char sprt [] ;
	friend unsigned** remplir(unsigned, unsigned, unsigned);

	unsigned isSeparator(char );
	bool isKeyW(string &);
	bool isIdent(string &);
	bool isConst(string &);

	void insert_sep(char, unsigned);
	void insert_ent(string str, unsigned n);
	void append_word(string&, unsigned = 0);
	
public:
	vector<cell> tab_ent;
	vector<string> tab_err;

	void Analyse(string str);
	
	void affiche() {
		cout << "///////// table of keys word //////////////// \n";
		for (int i = 0; i < tab_ent.size(); i++) {
			cout << i + 1;
			cout << " " << tab_ent[i].nom;
			cout <<" --> "<< tab_ent[i].type<<endl;
		}
		cout << "///////// table of erreurs //////////////// \n";
		for (int i = 0; i < tab_err.size(); i++) {
			cout << i + 1;
			cout <<" "<< tab_err[i]<<endl;
		}
	}
};


