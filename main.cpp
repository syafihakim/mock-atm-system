#include <iostream>
#include <windows.h>
#include <unistd.h>
#include <conio.h>
#include <fstream>
#include <string.h>
#include <bits/stdc++.h>

using namespace std;

struct user {
    string id;
    string name;
    int balance;
    vector<string> history;
} currentUser;

int screenWidth = 30;
int bannerWidth = 25;
int tries = 0;
bool needLogin = true;

void printCenter(string);
void printBanner();
void loginScreen();
void printLeft(string);
void printSuccessfulLogin(string);
void printNewLine();
void printFailLogin();
void failLoginScreen();
void printMainMenu();
void displayBalance();
void withDrawMoney();
void loadingScreen(string);
boolean checkPassword(string, string);
vector<string> adv_tokenizer(string, char);
void getBalance(string);
void updateBalance(int);
vector<vector<string>> readFile(string);
void depositMoney();
bool transferConfirmation(string);
void transferMoney();
void updateRecipientBalance(int, string);
void logout();
void addToHistory(string);
void receipt();
void createDefaultFiles();

int main() {

    // ATM MACHINEEEE

    // MALAM NI NAK TAMBAH
    // FEATURE APA???????
    createDefaultFiles();

    while(true) {
        currentUser.history.clear();
        loginScreen();
    }


    return 0;
}

void addToHistory(string activity) {
    currentUser.history.push_back(activity);
}

void receipt() {
    system("cls");
    printBanner();
    printNewLine();
    printLeft("TRANSACTION HISTORY");
    printNewLine();
    for(int i=0; i<currentUser.history.size(); i++) {
        string currHistory = currentUser.history[i];
        string c = to_string(i+1);
        printLeft(c + ". " + currHistory + "\n");
    }
    getch();
}

void loginScreen() {
    while (tries < 3 && needLogin) {
        system("cls");
        printBanner();
        string uname, pword;
        printLeft("USERNAME:");
        cin >> uname;
        printLeft("PASSWORD:");
        cin >> pword;

        if(checkPassword(uname, pword)) {
            getBalance(currentUser.id);
            needLogin = false;
            printSuccessfulLogin(uname);
            sleep(2);
            printMainMenu();
            needLogin = true;
        } else {
            failLoginScreen();
        }
    }
}

void printMainMenu() {
    while(true) {
        system("cls");
        printBanner();
        printCenter("Welcome, " + currentUser.name);
        printNewLine();
        printLeft("1 - Display Balance\n");
        printLeft("2 - Withdraw Money\n");
        printLeft("3 - Deposit Money\n");
        printLeft("4 - Transfer Money\n");
        printLeft("5 - Logout\n");

        char choice;
        choice = getch();
        cout << choice;

        if(choice == 49) {
            displayBalance();
            getch();
        } else if(choice == 50) {
            withDrawMoney();
            getch();
        } else if(choice == 51) {
            depositMoney();
            getch();
        } else if(choice == 52) {
            transferMoney();
            getch();
        } else if(choice == 53) {
            loadingScreen("LOGGING OUT");
            receipt();
            break;
        }
    }
}

void transferMoney() {
    int amount;
    string id;
    bool confirmed;

    do {
        system("cls");
        printBanner();
        printNewLine();
        printLeft("Enter amount to transfer:\n");
        printCenter("===================");
        printLeft("RM: ");
        cin >> amount;
        printLeft("Enter recipient id:\n");
        printLeft("ID : ");
        cin >> id;
        confirmed = transferConfirmation(id);
    }while(confirmed == false);

    currentUser.balance -= amount;

    updateBalance(amount);
    updateRecipientBalance(amount, id);
}

bool transferConfirmation(string id) {

    int choice;

    vector<vector<string>>everything = readFile("users.txt");
    map<string, string> allUsers;

    for(int i=0; i<everything.size(); i++) {
        allUsers[everything[i][0]] = everything[i][1];
    }

    system("cls");
    printBanner();
    printNewLine();
    printLeft("Please confirm id\n");
    printCenter("===================");
    printLeft("ID : " + id + "\n");
    printLeft("Name : " +  allUsers[id] + "\n");
    printCenter("===================");
    printNewLine();
    printCenter("Press 8 - Yes");
    printCenter("Press 9 - No");
    choice = getch();

    if(choice == 56) {
        return true;
    } else {
        return false;
    }

}

void withDrawMoney() {
    system("cls");
    printBanner();
    printNewLine();
    printLeft("Enter amount to withdraw:\n");
    printCenter("===================");
    printLeft("RM ");
    int amount;
    cin >> amount;
    printCenter("===================");
    currentUser.balance =  currentUser.balance - amount;
    updateBalance(currentUser.balance);
    printNewLine();
    loadingScreen("Please Wait");
    printLeft("WITHDRAW SUCCESSFUL!!");
    addToHistory("WITHDRAW - RM " + to_string(amount));
}

void depositMoney() {
    system("cls");
    printBanner();
    printNewLine();
    printLeft("Enter amount to deposit:\n");
    printCenter("===================");
    printLeft("RM ");
    int amount;
    cin >> amount;
    printCenter("===================");
    currentUser.balance =  currentUser.balance + amount;
    updateBalance(currentUser.balance);
    printNewLine();
    loadingScreen("Please Wait");
    printLeft("DEPOSIT SUCCESSFUL!!");
    addToHistory("DEPOSIT - RM " + to_string(amount));
}

void updateRecipientBalance(int deposit, string id) {
    map<string, string> allUsers;
    vector<vector<string>> everything;

    everything = readFile("duit.txt");

    for(int i=0; i<everything.size(); i++) {
        allUsers[everything[i][0]] = everything[i][1];
    }

    int newBalance = stoi(allUsers[id]) + deposit;

    allUsers[id] = to_string(newBalance);

    ofstream fileDuit("duit.txt");
    std::map<string,string>::iterator it;

    for (it=allUsers.begin(); it!=allUsers.end(); ++it)
    {
        fileDuit << it->first << ";";
        fileDuit << it->second << ";";
        fileDuit << "\n";
    }
    fileDuit.close();
}

void updateBalance(int newBalance) {
    map<string, int> allUsers;
    vector<vector<string>> everything;

    everything = readFile("duit.txt");

    for(int i=0; i<everything.size(); i++) {
        allUsers[everything[i][0]] = stoi(everything[i][1]);
    }

    allUsers[currentUser.id] = currentUser.balance;

    ofstream fileDuit("duit.txt");
    std::map<string,int>::iterator it;

    for (it=allUsers.begin(); it!=allUsers.end(); ++it)
    {
        fileDuit << it->first << ";";
        fileDuit << it->second << ";";
        fileDuit << "\n";
    }
    fileDuit.close();
}

void loadingScreen(string msg) {
    int perc = 0;
    while(perc < bannerWidth*4) {
        system("cls");
        printBanner();
        printNewLine();
        printCenter(msg);
        string bar = string(perc, '$');
        printLeft(bar);
        if(perc%3 == 0 || perc%4 == 0) usleep(30000);
        else usleep(50000);
        perc++;
    }
    system("cls");
    printBanner();
    printNewLine();
}

void displayBalance() {
    getBalance(currentUser.id);
    system("cls");
    printBanner();
    printNewLine();
    printLeft("TOTAL AMOUNT IN ACCOUNT\n");
    printCenter("==========");
    printCenter("RM " + to_string(currentUser.balance));
    printCenter("==========");
}

void failLoginScreen() {
    system("cls");
    printBanner();
    printNewLine();
    printCenter("ERROR");
    printCenter("LOGIN FAILED");
    printCenter("YOU HAVE " + to_string(3-tries) + " attempts");
    tries++;
    sleep(3);
}

void printSuccessfulLogin(string uname) {
    system("cls");
    printBanner();
    printNewLine();
    printCenter("WELCOME, " + uname);
    printCenter("LOGIN SUCCESSFUL");
}

void printLeft(string s) {
    // MODIFY STRING S
    // RETURN THE MODIFIED STRING

    string leftText;
    int spacesCount = bannerWidth;
    string spaces = string(spacesCount, ' ');
    leftText = spaces + s;
    cout << leftText;
}

void printNewLine() {
    printCenter("");
}

void printCenter(string s) {
    // MODIFY STRING S
    // RETURN THE MODIFIED STRING

    string centerText;
    int spacesCount = (screenWidth - s.size())/2;
    string spaces = string(spacesCount, ' ');
    centerText = spaces + s + spaces;
    cout << centerText << endl;
}

void printBanner() {
    string topBorder = "+======================+";
    printCenter(topBorder);
    printCenter("TIKTOK BANK SYSTEM");
    printCenter(topBorder);
    bannerWidth = (screenWidth - topBorder.size())/2;
}

boolean checkPassword(string uname, string pass) {
    ifstream file("users.txt");

    string currLine;
    vector<string> credentials;

    while (getline (file, currLine)) {
        credentials = adv_tokenizer(currLine, ';');
        if(uname == credentials[1]) {
            if(pass == credentials[2]) {
                currentUser.id = credentials[0];
                currentUser.name = credentials[1];
                return true;
            }
        }
    }
    return false;
}

void getBalance(string id) {

    ifstream file("duit.txt");

    string currLine;
    vector<string> balanceDetails;

    while (getline (file, currLine)) {
        balanceDetails = adv_tokenizer(currLine, ';');
        if(balanceDetails[0] == id) {
            currentUser.balance = stoi(balanceDetails[1]);
        }
    }
}

vector<string> adv_tokenizer(string s, char del)
{
    vector<string> ans;

    stringstream ss(s);
    string word;
    while (!ss.eof()) {
        getline(ss, word, del);
        ans.push_back(word);
    }
    return ans;
}

vector<vector<string>> readFile(string filename) {
    vector<vector<string>> everything;
    vector<string> temp;

    ifstream file(filename);

    string currLine;

    while (getline (file, currLine)) {
        temp = adv_tokenizer(currLine, ';');
        everything.push_back(temp);
    }
    return everything;
}

void createDefaultFiles() {
    ifstream file;
    // opening the file
    file.open("duit.txt");

    if(!file) {
        ofstream duit("duit.txt");
        duit << "001;1000;" << endl;
        duit << "002;1000;" << endl;
        duit << "003;1000;" << endl;
        duit << "004;1000;" << endl;
        duit << "005;1000;" << endl;
        duit << "006;1000;" << endl;
        duit << "007;1000;";
        ofstream users("users.txt");
        users << "001;admin;admin;" << endl;
        users << "002;syafi;123;" << endl;
        users << "003;amar;123;" << endl;
        users << "004;aizzad;000;" << endl;
        users << "005;aqil;231;" << endl;
        users << "006;munn;112;" << endl;
        users << "007;adam;321;";
    }
}




