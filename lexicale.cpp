
#include "lexicale.h"

string lexicale::tab_c = "";
unsigned* lexicale::tab_f = NULL;
char lexicale::op_arth[] = { '+','-','*','/','%' };
char lexicale::op_log[] = { '^','&','~' };
char lexicale::op_cmp[] = { '>','<',':','!' };
char lexicale::sprt[] = { '(',')','[',']','{','}','=','.','|' };
unsigned** remplir(unsigned n, unsigned m, unsigned nbr_f) {
    unsigned i = 0;
    unsigned** mat = new unsigned* [n];
    for (i = 0; i < n; i++) {
        mat[i] = new unsigned[m];
        for (unsigned j = 0; j < m; j++) mat[i][j] = 0;
    }

    lexicale::tab_f = new unsigned[nbr_f];
    string line, y, res;
    ifstream keyw;
    keyw.open("KeysW.txt", ios::in);

    getline(keyw, lexicale::tab_c);

    i = 0;
    int k = 0;
    while (getline(keyw, line)) {
        if (line == "-") {
            if (k > nbr_f) return NULL;
            lexicale::tab_f[k] = i;
            k++; i++;
            continue;
        }
        stringstream var(line);
        while (getline(var, y, ',')) {
            getline(var, res, ',');
            mat[i][stoi(y)] = stoi(res);
        }
        i++;
    }
    keyw.close();

    return mat;
}
unsigned** lexicale::matKw = remplir(31, 16, 4);


void lexicale::Analyse(string str) {
    string line;
    ifstream source;
    bool b = 0;
    bool b1 = 0;
    source.open(str, ios::in);

    while (getline(source, line)) {
        int i = 0;
        string bien = "";
        while (i < line.length()) {
            if (!b) {
                for (i; i < line.length(); i++) {
                    if (line[i] == '"') b1 = !b1;
                    if (line[i] == '#' && !b1) { b = 1; i++; break; }
                    bien = bien + line[i];
                }
            }
            if (b) {
                for (i; i < line.length(); i++) if (line[i] == '#') { b = 0; i++; break; }
            }
        }
        if (!b) {
            this->append_word(bien);
            // cout << bien << endl;
        }
    }
    if (b) cout << " chaine not accepted\n";
    else cout << "chaine accepted\n";
    source.close();
}

void lexicale::insert_ent(string str, unsigned n) {
    cell elem;
    elem.nom = str;
    switch (n)
    {
    case 1: elem.type = "KeyWord (type)"; break;
    case 2: elem.type = "KeyWord"; break;
    case 3: elem.type = "KeyWord (fct_pre)"; break;
    case 4: elem.type = "KeyWord (condition)"; break;
    case 5: elem.type = "KeyWord (loop)"; break;
    case 6: elem.type = "identificateur"; break;
    case 7: elem.type = "Constante (String)"; break;
    case 8: elem.type = "Constante (Number)"; break;
    case 9: elem.type = "separator"; break;
    case 10: elem.type = "op_arth"; break;
    case 11: elem.type = "op_cmp"; break;
    case 12: elem.type = "op_log"; break;
    default:break;
    }
    tab_ent.push_back(elem);
}
void lexicale::append_word(string& str, unsigned n_lig) {
    if (str.length() == 0) return;
    bool b = 0;
    for(unsigned i = 0; i < str.length(); i++){
        if (str[i] == ' ' && !b) continue; 
        string word = "";
        unsigned nb_sep = isSeparator(str[i]);

        while (i < str.length() && nb_sep == 0) {
            if (str[i] == ' ' && !b) break;
            if (str[i] == '"') b = !b;
            word = word + str[i];
            i++;
            if(!b) nb_sep = isSeparator(str[i]);
        }
        if (!(isKeyW(word) || isIdent(word) || isConst(word))) tab_err.push_back(word);
        word = str[i];
        if (nb_sep != 0) insert_ent(word , nb_sep);
    }
}


unsigned lexicale::isSeparator(char c) {
    unsigned i = 0 ;
    for (i=0; i < 9; i++) if (c == sprt[i]) return 9;     
    for (i=0; i < 5; i++) if (c == op_arth[i]) return 10;      
    for (i=0; i < 4; i++) if (c == op_cmp[i]) return 11;      
    for (i=0; i < 3; i++) if (c == op_log[i]) return 12;      
    return 0;
}
bool lexicale::isKeyW(string &str) {
    if (str.length() == 0) return 1;
    unsigned etat_c = 0;
    for (unsigned i = 0; i < str.length(); i++) {
        bool b = 1; unsigned k = 0;
        for (k = 0; k < tab_c.length(); k++) if (tab_c[k] == str[i]) { b = 0; break;}
        if (b) return 0;

        etat_c = matKw[etat_c][k];
        if (etat_c == 0) return 0;
    }
    switch (etat_c)
    {
    case 4: insert_ent(str, 1); break;
    case 9: insert_ent(str, 2); break;
    case 14: insert_ent(str, 3); break;
    case 22: insert_ent(str, 4); break;
    case 27: insert_ent(str, 5); break;
    default: return 0;
    }
    return 1;
}
bool lexicale::isIdent(string &str) {
    if (str.length() == 0) return 1;
    if (str[0] < 'a' || str[0]>'z' || str[0] == '_') return 0;
    for (unsigned i = 0; i < str.length(); i++) {
        if (!((str[i] >= 'a' && str[i] <= 'z')|| str[i] == '_' || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')))
            return 0;
    }
    insert_ent(str, 6);
    return 1;
}
bool lexicale::isConst(string &str) {
    if (str.length() == 0) return 1;
    if (str[0] == '"') {
        for (int i = 1; i < str.length()-1; i++) if (str[i]=='"') return 0;
        if (str[str.length() - 1] != '"') return 0;
        insert_ent(str, 7);
        return 1;
    }
    else
    {
        int i = 0;
        if (str[i] < '0' || str[i]>'9') return 0;
        for (i = 1; i < str.length(); i++) {
            if (str[i] == ',') break;
            if (str[i] < '0' || str[i]>'9') return 0;
        }
        for (i++; i < str.length(); i++) 
            if (str[i] < '0' || str[i]>'9') return 0;
        insert_ent(str, 8);
        return 1;
    }
}


