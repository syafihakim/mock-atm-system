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
bool atMainMenu = false;
int menuChoice = 0;
int moneyChoice = 0;

vector<string> menu = { "Display Balance\n",
                        "Withdraw Money\n",
                        "Deposit Money\n",
                        "Transfer Money\n",
                        "Logout\n"};

vector<vector<string>> money = {{"RM 100", "RM 200", "RM 300"},
                                {"RM 400", "RM 500", "Other"}};

vector<string> adv_tokenizer(string, char);
void updateRecipientBalance(int, string);
vector<vector<string>> readFile(string);
vector<string> readCredentials();
boolean checkPassword(string, string);
void printSuccessfulLogin(string);
bool transferConfirmation(string);
void loadingScreen(string);
void addToHistory(string);
void printCenter(string);
void getBalance(string);
void printLeft(string);
void updateBalance(int);
void createDefaultFiles();
void failLoginScreen();
void printFailLogin();
void displayBalance();
void displayBalance();
void transferMoney();
void printMainMenu();
void withDrawMoney();
void depositMoney();
void printNewLine();
void printBanner();
void loginScreen();
void printWarning(string text);
void mainFunc();
void notification();
void receipt();
void logout();
void randomColor();
int getArrowKeys();
void printWithdrawAmount();

int main() {
    randomColor();
    createDefaultFiles();
    mainFunc();

    return 0;
}

void randomColor() {
    srand(time(0));
    // string colors[4] = {"Color E0", "Color A0", };
    // string x = colors[rand()%4];
    system("Color 60");
}

void mainFunc() {
    currentUser.history.clear();
    loginScreen();
}

int getArrowKeys() {
    int c = getch();
    if(c == 224) {
        return getch();
    } else {
        return c;
    }

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
        string uname, pword;
        vector<string> credentials = readCredentials();
        uname = credentials[0];
        pword = credentials[1];

        if(checkPassword(uname, pword)) {
            getBalance(currentUser.id);
            needLogin = false;
            printSuccessfulLogin(uname);
            sleep(2);
            printMainMenu();
        } else {
            failLoginScreen();
        }
    }
}

void waitForMoney() {
    for(int i=20; i>-20; i--) {
        COORD c;
        c.X = i >= 3 ? i : 3;
        c.Y = 11;

        SetConsoleCursorPosition(
            GetStdHandle(STD_OUTPUT_HANDLE), c);

        string txt = "Notification : RM100";

        if(txt.size() + i >= screenWidth) {
            cout << txt.substr(0, screenWidth-i);
        } else if(i < 0) {
            cout << txt.substr(abs(i), txt.size()-abs(i));
        } else {
            cout << txt;
        }
        SetConsoleCursorPosition(
            GetStdHandle(STD_OUTPUT_HANDLE), c);
        usleep(100000*2);
        cout << string(screenWidth, ' ');
    }
}

vector<string> readCredentials() {
    string uname, pword;
    system("cls");
    printBanner();
    printLeft("USERNAME:");
    cin >> uname;
    printLeft("PASSWORD:");
    char c = getch();
    while(c != 13) {
        pword += c;
        system("cls");
        printBanner();
        printLeft("USERNAME:" + uname + "\n");
        printLeft("PASSWORD:" + string(pword.size(), '*'));
        printNewLine();
        c = getch();
    }
    vector<string> res;
    res.push_back(uname);
    res.push_back(pword);
    return res;
}

void printMainMenu() {
    int input = 0;
    while(input != 13) {
        system("cls");
        printBanner();
        printCenter("Welcome, " + currentUser.name);
        printNewLine();
        for(int i=0; i<menu.size(); i++) {
            if(menuChoice == i) {
                printLeft("> " + menu[i]);
            } else {
                printLeft("  " + menu[i]);
            }
        }
        input = getArrowKeys();

        if(input == 72 && menuChoice > 0) {
            menuChoice--;
        } else if(input == 80 && menuChoice < menu.size()) {
            menuChoice++;
        } else if(input == 13){
            if(menuChoice == 0) {
                displayBalance();
            } else if(menuChoice == 1) {
                withDrawMoney();
            } else if(menuChoice == 2) {
                depositMoney();
            } else if(menuChoice == 3) {
                transferMoney();
            } else if(menuChoice == 4) {
                loadingScreen("LOGGING OUT");
                receipt();
                needLogin = true;
                mainFunc();
            } else {
                printMainMenu();
            }
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
    printMainMenu();
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
    printLeft("Press 8 - Yes \n");
    printLeft("Press 9 - No");
    choice = getch();

    if(choice == 56) {
        return true;
    } else {
        return false;
    }

}

void printWithdrawAmount(int row, int col) {
    for(int i=0; i<money.size(); i++) {
        printLeft("");
        for(int j=0; j<money[i].size(); j++) {
            if( j != 0) cout << "  ";

            if(row == i && j == col)
                cout << "[" + money[i][j] + "]";
            else
                cout << " " + money[i][j] + " ";
        }
        cout << endl;
    }
    cout << endl;
}

void withDrawMoney() {
    vector<vector<int>> amounts = {
    {100,200,300},
    {400,500}};
    int amount = 0;
    int input = 0;
    int row = 0;
    int col = 0;
    while(input != 13) {
        system("cls");
        printBanner();
        printWithdrawAmount(row, col);
        input = getArrowKeys();
        if(input == 72) {
            row--;
        } else if(input == 80) {
            row++;
        } else if(input == 77) {
            col++;
        } else if(input == 75) {
            col--;
        } else if(input == 13) {
            break;
        }
        amount = amounts[row][col];
    }
    printLeft("RM " + to_string(amount));

    printCenter("===================");
    currentUser.balance =  currentUser.balance - amount;
    updateBalance(currentUser.balance);
    printNewLine();
    loadingScreen("Please Wait");
    printLeft("WITHDRAW SUCCESSFUL!!");
    addToHistory("WITHDRAW - RM " + to_string(amount));
    getch();
    printMainMenu();
}

void printWarning(string text) {
    printCenter("+" + string(25,
                             '-') + "+");
    printCenter(text);
    printCenter("+" + string(25,
                             '-') + "+");
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
    getch();
    printMainMenu();
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
    getch();
    printMainMenu();
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
        users << "001;yana;yana;" << endl;
        users << "002;syafi;123;" << endl;
        users << "003;amar;123;" << endl;
        users << "004;aizzad;000;" << endl;
        users << "005;aqil;231;" << endl;
        users << "006;munn;112;" << endl;
        users << "007;adam;321;";
    }
}




