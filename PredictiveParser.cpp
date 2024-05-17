#include<iostream>
#include<string>
#include<deque>
#include<set>
#include<vector>
using namespace std;

int n, n1, n2;

// Function to get position of a string in an array
int getPosition(string arr[], string q, int size) {
    for(int i = 0; i < size; i++) {
        if(q == arr[i])
            return i;
    }
    return -1;
}

// Function to calculate the FIRST set for a given non-terminal
set<string> calculateFirst(string nonterm, string prods[], string first[]) {
    set<string> firstSet;
    for(int i = 0; i < n; i++) {
        if(prods[i].substr(0, 1) == nonterm) {
            string rhs = prods[i].substr(3);
            if(rhs[0] >= 'a' && rhs[0] <= 'z') { // Terminal symbol
                firstSet.insert(rhs.substr(0, 1));
            } else { // Non-terminal symbol
                set<string> tempSet = calculateFirst(rhs.substr(0, 1), prods, first);
                firstSet.insert(tempSet.begin(), tempSet.end());
            }
        }
    }
    return firstSet;
}

int main() {
    string prods[10], first[10], follow[10], nonterms[10], terms[10];
    string pp_table[20][20] = {};

    // Input grammar productions
    cout << "Enter the number of productions : ";
    cin >> n;
    cin.ignore();
    cout << "Enter the productions" << endl;
    for(int i = 0; i < n; i++) {
        getline(cin, prods[i]);
    }

    // Calculate FIRST set for each non-terminal
    for(int i = 0; i < n; i++) {
        string nonterm = prods[i].substr(0, 1);
        set<string> firstSet = calculateFirst(nonterm, prods, first);
        first[i] = "";
        for(const auto& symbol : firstSet) {
            first[i] += symbol;
        }
    }

    cout << "Enter the number of Terminals : ";
    cin >> n2;
    cin.ignore();
    cout << "Enter the Terminals" << endl;
    for(int i = 0; i < n2; i++) {
        cin >> terms[i];
    }
    terms[n2] = "$";
    n2++;

    cout << "Enter the number of Non-Terminals : ";
    cin >> n1;
    cin.ignore();
    for(int i = 0; i < n1; i++) {
        cout << "Enter Non-Terminal : ";
        getline(cin, nonterms[i]);
        cout << "Enter follow of " << nonterms[i] << " : ";
        getline(cin, follow[i]);
    }

    cout << endl;
    cout << "Grammar" << endl;
    for(int i = 0; i < n; i++) {
        cout << prods[i] << endl;
    }

    // Generate parsing table
    for(int j = 0; j < n; j++) {
        int row = getPosition(nonterms, prods[j].substr(0, 1), n1);
        if(prods[j].at(3) != '#') {
            for(int i = 0; i < first[j].length(); i++) {
                int col = getPosition(terms, first[j].substr(i, 1), n2);
                pp_table[row][col] = prods[j];
            }
        } else {
            for(int i = 0; i < follow[row].length(); i++) {
                int col = getPosition(terms, follow[row].substr(i, 1), n2);
                pp_table[row][col] = prods[j];
            }
        }
    }

    // Display parsing table
    for(int j = 0; j < n2; j++) {
        cout << "\t" << terms[j];
    }
    cout << endl;
    for(int i = 0; i < n1; i++) {
        cout << nonterms[i] << "\t";
        for(int j = 0; j < n2; j++) {
            cout << pp_table[i][j] << "\t";
        }
        cout << endl;
    }

    // Parsing String
    char c;
    do {
        string ip;
        deque<string> pp_stack;
        pp_stack.push_front("$");
        pp_stack.push_front(prods[0].substr(0, 1));
        cout << "Enter the string to be parsed : ";
        getline(cin, ip);
        ip.push_back('$');
        cout << "Stack\tInput\tAction" << endl;
        while(true) {
            for(int i = 0; i < pp_stack.size(); i++)
                cout << pp_stack[i];
            cout << "\t" << ip << "\t";
            int row1 = getPosition(nonterms, pp_stack.front(), n1);
            int row2 = getPosition(terms, pp_stack.front(), n2);
            int column = getPosition(terms, ip.substr(0, 1), n2);
            if(row1 != -1 && column != -1) {
                string p = pp_table[row1][column];
                if(p.empty()) {
                    cout << endl << "String cannot be Parsed." << endl;
                    break;
                }
                pp_stack.pop_front();
                if(p[3] != '#') {
                    for(int x = p.size() - 1; x > 2; x--) {
                        pp_stack.push_front(p.substr(x, 1));
                    }
                }
                cout << p;
            } else {
                if(ip.substr(0, 1) == pp_stack.front()) {
                    if(pp_stack.front() == "$") {
                        cout << endl << "String Parsed." << endl;
                        break;
                    }
                    cout << "Match " << ip[0];
                    pp_stack.pop_front();
                    ip = ip.substr(1);
                } else {
                    cout << endl << "String cannot be Parsed." << endl;
                    break;
                }
            }
            cout << endl;
        }
        cout << "Continue?(Y/N) ";
        cin >> c;
        cin.ignore();
    } while(c == 'y' || c == 'Y');
    return 0;
}
