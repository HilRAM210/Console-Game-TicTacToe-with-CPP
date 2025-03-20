#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

int getValidIntInput() {
    int choice;
    while (true) {
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
        } else {
            return choice;
        }
    }
}

vector<string> USERNAME;
vector<string> PASSWORD;

void loadUserData() {
    ifstream file("userdata.txt");
    if (file.is_open()) {
        string username, password;
        while (file >> username >> password) {
            USERNAME.push_back(username);
            PASSWORD.push_back(password);
        }
        file.close();
    }
}

void saveUserData(const string& username, const string& password) {
    ofstream file("userdata.txt", ios::app);
    if (file.is_open()) {
        file << username << " " << password << endl;
        file.close();
    }
}

bool login(const string& username, const string& password) {
    for (size_t i = 0; i < USERNAME.size(); i++) {
        if (USERNAME[i] == username && PASSWORD[i] == password) {
            return true;
        }
    }
    return false;
}

void loginSystem() {
    string inputUsername, inputPassword;

    bool setLogin = false;
    while (!setLogin) {
        cout << "================================" << endl;
        cout << " ===>      LOGIN PAGE      <=== " << endl;
        cout << "================================" << endl;
        cout << " -Enter Username: ";
        cin >> inputUsername;
        cout << " -Enter Password: ";
        cin >> inputPassword;
        cout << "================================" << endl;

        if (login(inputUsername, inputPassword)) {
            cout << "Login Successful! Welcome, " << inputUsername << "!" << endl;
            getch();
            setLogin = true;
            system("cls");
        } else {
            cout << "Login Failed! Incorrect Username or Password." << endl;
            getch();
            system("cls");
        }
    }
}

void registerUser() {
    string newUsername, newPassword;
    cout << "================================" << endl;
    cout << " ===>       REGISTER       <===" << endl;
    cout << "================================" << endl;
    cout << " -Enter New Username: ";
    cin >> newUsername;
    cout << " -Enter New Password: ";
    cin >> newPassword;
    cout << "================================" << endl;

    USERNAME.push_back(newUsername);
    PASSWORD.push_back(newPassword);
    saveUserData(newUsername, newPassword);

    cout << "Registration Successful!" << endl;
    getch();
    system("cls");
}

void mainMenu() {
    loadUserData();

    int choice;
    bool setChoice = false;
    
    while (!setChoice) {
        cout << "================================" << endl;
        cout << " ===>         MENU         <=== " << endl;
        cout << "================================" << endl;
        cout << "  -> 1. Login" << endl;
        cout << "  -> 2. Register" << endl;
        cout << "  -> 3. Exit" << endl;
        cout << "================================" << endl;
        cout << "Please Choose a Menu: ";
        
        choice = getValidIntInput();

        system("cls");

        switch (choice) {
        case 1:
            loginSystem();
            setChoice = true;
            break;
        case 2:
            registerUser();
            break;
        case 3:
            cout << "Thank you!" << endl;
            exit(0);
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    }
}

void loadingScreen() {
    const char a = 178;
    int i = 0;
        
    cout << "\t\t\t Preparing Content\n" << endl;
    cout << "	      +==================================================+" << endl;
    
    while (i <= 100) {
        cout << "\rLoading: " << i << "% |";
        for (int x = 0; x < i / 2; x++) {
            cout << a;
        }
        for (int x = i / 2; x < 50; x++) { 
            cout << " ";
        }
        cout << "|";
        Sleep(1);
        i++;
    }
    cout << "\n	      +==================================================+" << endl;
    cout << "\nLoading Complete!" << endl;
    system("pause");
    system("cls");
}

void drawBoard(char board[3][3]) { 
    cout << "-------------\n"; 
    for (int i = 0; i < 3; i++) { 
        cout << "| "; 
        for (int j = 0; j < 3; j++) { 
            cout << board[i][j] << " | "; 
        } 
        cout << "\n-------------\n"; 
    } 
} 

bool checkWin(char board[3][3], char player) { 
    for (int i = 0; i < 3; i++) { 
        if (board[i][0] == player && board[i][1] == player 
            && board[i][2] == player) 
            return true; 
        if (board[0][i] == player && board[1][i] == player 
            && board[2][i] == player) 
            return true; 
    } 
    if (board[0][0] == player && board[1][1] == player 
        && board[2][2] == player) 
        return true; 
    if (board[0][2] == player && board[1][1] == player 
        && board[2][0] == player) 
        return true; 
    return false; 
}

bool validMove(char board[3][3], int row, int col) {
    return board[row][col] == ' ' && row >= 0 && row < 3 && col >= 0 && col < 3;
}

void computerMoveEasy(char board[3][3], char player) {
    srand(time(0));
    int row, col;

    do {
        row = rand() % 3;
        col = rand() % 3;
    } while (!validMove(board, row, col));
    
    board[row][col] = player;
}

void computerMoveMedium(char board[3][3], char player) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' '){
                board[i][j] = player;
                if (checkWin(board, player)) return;
                board[i][j] = (player == 'X') ? 'O' : 'X';
                if (checkWin(board, (player == 'X' ? 'O' : 'X'))) {
                    board[i][j] = player;
                    return;
                }
                board[i][j] = ' ';
            }
        }
    }
    computerMoveEasy(board, player);
}

int minimax(char board[3][3], int depth, bool isMaximizing, char player) {
    char opponent = (player == 'X') ? 'O' : 'X';

    if (checkWin(board, player)) return 10 - depth;
    if (checkWin(board, opponent)) return depth - 10;
    bool isFull = true;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                isFull = false;
                break;
            }
        }
    }
    if (isFull) return 0;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = player;
                    int score = minimax(board, depth + 1, false, player);
                    board[i][j] = ' ';
                    bestScore = max(bestScore, score);
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = opponent;
                    int score = minimax(board, depth + 1, true, player);
                    board[i][j] = ' ';
                    bestScore = min(bestScore, score);
                }
            }
        }
        return bestScore;
    }
}

void computerMoveHard(char board[3][3], char player) {
    int bestScore = -1000;
    int bestMoveRow = -1;
    int bestMoveCol = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = player;
                int score = minimax(board, 0, false, player);
                board[i][j] = ' ';
                if (score > bestScore) {
                    bestScore = score;
                    bestMoveRow = i;
                    bestMoveCol = j;
                }
            }
        }
    }
    board[bestMoveRow][bestMoveCol] = player;
}

bool validateChoice(char choice) {
    return (choice == '1' || choice == '2');
}

int chooseDifficulty() {
    int difficulty;
    while (true) {
        cout << "================================" << endl;
        cout << " ===>   Difficulty Level    <===" << endl;
        cout << "================================" << endl;
        cout << "  -> 1. Easy\n";
        cout << "  -> 2. Medium\n";
        cout << "  -> 3. Hard\n";
        cout << "  -> 4. Back to Mode Selection\n";
        cout << "================================" << endl;
        cout << "Choose Difficulty Level: ";
        
        difficulty = getValidIntInput();
        
        if (difficulty == 4) {
            system("cls");
            return -1;
        } else if (difficulty >= 1 && difficulty <= 3) {
        	system("cls");
            return difficulty;
        } else {
            cout << "Invalid choice! Please choose 1, 2, 3, or 4 to go back.\n";
            getch();
            system("cls");
        }
    }
}

void tictactoe() {
    char board[3][3] = { { ' ', ' ', ' ' }, 
                         { ' ', ' ', ' ' }, 
                         { ' ', ' ', ' ' } }; 
    char player = 'X';
    int row, col;
    int turn;
    bool againstComputer;
    char choice;
    int difficulty;

    while (true) {
        cout << "================================" << endl;
        cout << " ===>      Choose Mode     <=== " << endl;
        cout << "================================" << endl;
        cout << "  -> 1. Computer" << endl;
        cout << "  -> 2. Player" << endl;
        cout << "  -> 3. Back to Main Menu" << endl;
        cout << "================================" << endl;
        cout << "Please Choose Mode: ";
        cin >> choice;
        
        if (choice == '3') {
            system("cls");
            return;
        } else if (validateChoice(choice)) {
            againstComputer = (choice == '1');
            system("cls");
            break;
        } else {
            cout << "Invalid choice! Please choose '1' to play against the computer, '2' to play with another player, or '3' to go back.\n";
            getch();
            system("cls");
        }
    }

    if (againstComputer) {
        while (true) {
            difficulty = chooseDifficulty();
            if (difficulty == -1) {
                system("cls");
                return;
            }

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    board[i][j] = ' ';
                }
            }

            for (turn = 0; turn < 9; turn++) { 
                cout << "Welcome to Tic-Tac-Toe!\n\n";
                drawBoard(board);
                cout << endl; 
                
                if (player == 'O') {
                    cout << "Computer's Turn!\n";
                    if (difficulty == 1) {
                        computerMoveEasy(board, player);
                    } else if (difficulty == 2) {
                        computerMoveMedium(board, player);
                    } else if (difficulty == 3) {
                        computerMoveHard(board, player);
                    }
                    system("pause");
                    system("cls");
                } else {
                    while (true) {
                        cout << "Player " << player << ", enter row (1-3) and column (1-3): "; 
                        
                        row = getValidIntInput();
                        col = getValidIntInput();
                        
                        system("cls"); 
                        row -= 1;
                        col -= 1;

                        if (!validMove(board, row, col)) {
                            cout << "Welcome to Tic-Tac-Toe!\n\n";
                            drawBoard(board);
                            cout << "\nInvalid move. Try again.\n"; 
                        } else {
                            break;
                        }
                    }
                    board[row][col] = player;
                }

                if (checkWin(board, player)) {
                    cout << "PLAYER " <<  player << " WINS!\n\n";
                    drawBoard(board);
                    if (player == 'X') {
                        cout << "\nCongratulations! Player X, you are the Tic-Tac-Toe Champion!\n";
                    } else {
                        cout << "\nYou lost to a bot, LOL!\n";
                    }
                    break; 
                }
                player = (player == 'X') ? 'O' : 'X'; 
            }
            
            if (turn == 9 && !checkWin(board, 'X') && !checkWin(board, 'O')) { 
                cout << "IT'S A DRAW!!!\n\n";
                drawBoard(board);
                cout << "\nNo Winner!\n";
            }
            
            system("pause");
            system("cls");

            char playAgain;
            cout << "Do you want to play again? (y/n): ";
            cin >> playAgain;
            system("cls");
            if (playAgain != 'y' && playAgain != 'Y') {
                break;
            }
        }
    } else {
        for (turn = 0; turn < 9; turn++) { 
            cout << "Welcome to Tic-Tac-Toe!\n\n";
            drawBoard(board);
            cout << endl; 
            
            while (true) {
                cout << "Player " << player << ", enter row (1-3) and column (1-3): "; 
                
                row = getValidIntInput();
                col = getValidIntInput();
                
                system("cls"); 
                row -= 1;
                col -= 1;

                if (!validMove(board, row, col)) {
                    cout << "Welcome to Tic-Tac-Toe!\n\n";
                    drawBoard(board);
                    cout << "\nInvalid move. Try again.\n"; 
                } else {
                    break;
                }
            }
            board[row][col] = player;

            if (checkWin(board, player)) {
                cout << "PLAYER " <<  player << " WINS!\n\n";
                drawBoard(board);
                cout << "\nCongratulations! Player " << player << ", you are the Tic-Tac-Toe Champion!\n";
                break; 
            }
            player = (player == 'X') ? 'O' : 'X'; 
        }
        
        if (turn == 9 && !checkWin(board, 'X') && !checkWin(board, 'O')) { 
            cout << "IT'S A DRAW!!!\n\n";
            drawBoard(board);
            cout << "\nNo Winner!\n";
        }
        
        system("pause");
        system("cls");
    }
}

void Start_Play(){
    int menu;
    bool setMenu = false;
    while(!setMenu){
        cout << "================================" << endl;
        cout << " ===>     Tic Tac Toe      <=== " << endl;
        cout << "================================" << endl;
        cout << "  -> 1. Start Game" << endl;
        cout << "  -> 2. Return to Main Menu" << endl;
        cout << "================================" << endl;
        cout << "Please Choose Menu: ";
        
        menu = getValidIntInput();
        
        if(menu == 1){
            system("cls");
            tictactoe();
            system("cls");
        }

        else if (menu == 2){
            system("cls");
            setMenu = true;
        }
        else {
            cout << "Invalid. Try Again!" << endl;
            getch();
            system("cls");
        }
    }
}

int main() {
    string choice;
    cout << "Do you want to start the program? (y/n): ";
    cin >> choice;
    system("cls");
    
    if (choice == "y" || choice == "Y") {
        loadingScreen();
    }
    else {
        cout << "Exiting Program." << endl;
        exit(0);
    }

    mainMenu();
    loadingScreen();
    
    int menu;
    do{
        cout << "============================================" << endl;
        cout << " ===>     Welcome to HilRAM's Games    <=== " << endl;
        cout << "============================================" << endl;
        cout << "  -> 1. Tic Tac Toe" << endl;
        cout << "  -> 2. Exit" << endl;
        cout << "============================================" << endl;
        cout << " Please Choose Menu: ";
        
        menu = getValidIntInput();
        
        if (menu == 1) {
            system("cls");
            Start_Play();
        }
        else if (menu == 2){
            system ("cls");
            cout << "Exiting Program" << endl;
        }
        else {
            cout << "Invalid Input. Try Again!" << endl;
            getch();
            system ("cls");
        }
    }while(menu != 2);
    
    return 0;
}
