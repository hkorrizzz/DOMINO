#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

struct DominoTile {
    int leftValue;
    int rightValue;
};

DominoTile deck[28];           //массив из 28 игровых костей
DominoTile player1Hand[7];                              
DominoTile player2Hand[7];                           
DominoTile board[28];          //массив игрового стола  //элементы добавляются по мере игры       

int player1HandSize = 0;
int player2HandSize = 0;                                        
int boardSize = 0;    

int cсс = 14;                  //содержит в себе количество доступных для раздачи костей в общей колоде                            
bool chek1 = true;                                           
bool chek2 = true;             //данные переменные будут осуществлять проверку на ничью                               
                                                         
//создает 28 разных костей
void initializeDeck() {                                  
                                                             
    int index = 0;                                     
    for (int i = 0; i <= 6; i++) {                              
        for (int j = i; j <= 6; j++) {                            
            deck[index].leftValue = i;                        
            deck[index].rightValue = j;                             
            index++;                                      
        }                                               
    }                                                    
}                                                        
                                                             
//раздает в руки игроков по 7 разных костей //выбранную кость меняет местами с поседней, чтобы исключить повторение
void dealHands() {                            
    srand(time(0));

    for (int i = 0; i < 7; i++) {
        int randomIndex = rand() % (28 - i);
        player1Hand[i] = deck[randomIndex];
        deck[randomIndex] = deck[27 - i];
    }

    for (int i = 0; i < 7; i++) {
        int randomIndex = rand() % (21 - i);

        player2Hand[i] = deck[randomIndex];
        deck[randomIndex] = deck[20 - i];
    }

    player1HandSize = 7;
    player2HandSize = 7;
}

//выводит на экран состояние рук игроков
void printHand(const DominoTile hand[], int size) {

    for (int i = 0; i < size; i++) {
        cout << i  << "-[" << hand[i].leftValue << "|" << hand[i].rightValue << "]   ";
    }
    cout << endl;
}

//набирает дополнительную фишку, если игрок не может ходить
void nabor(int& playerHandSize, DominoTile playerHand[]) {            
    srand(time(0));

    int random = rand() % (cсс);
    playerHand[playerHandSize++] = deck[random];
    deck[random] = deck[cсс-1];
}

//переберает каждый элемент в руке игрока, чтобы узнать доступен ли ход
bool perebor(int& playerHandSize, DominoTile playerHand[], int leftEnd, int rightEnd, const int playerNum) {  
    int b = 0;

    for (int i = 0; i < playerHandSize; i++) {            
        if (playerHand[i].leftValue != leftEnd && playerHand[i].rightValue != leftEnd && playerHand[i].leftValue != rightEnd && playerHand[i].rightValue != rightEnd) {
            b++;        //если фишкой нельзя походить ни слева, ни справа, то b++                      
        }
    }

    if (b == playerHandSize) {         //(если количество фишек которыми нельзя ходить == количеству в руке игрока)

        if (playerHandSize == 0) {     //проверка на окончание игры
            return false;
        }

        if (cсс == 0) {                //провверка на наличие в общей колоде фишек

            cout << "Игрок " << playerNum << ", вашими костями ход невозможен. Вы пропускаете ход." << endl;

            if (playerNum == 1) {
                chek1 = false;
            }

            else {
                chek2 = false;
            }
            return false;             
        }

        cout << "Игрок " << playerNum << ", ходы закончилиcь!" << endl;
 
        int v = playerHandSize;
        int vv = 0;
            while (playerHand[v-1].leftValue != leftEnd && playerHand[v-1].rightValue != leftEnd && playerHand[v-1].leftValue != rightEnd && playerHand[v-1].rightValue != rightEnd) {

                if (cсс == 0) {          
                    cout << "Игрок " << playerNum << ", вашими костями ход невозможен. Вы пропускаете ход." << endl;  
                    break; 
                }

                nabor(v, playerHand);  
                cout << "Игрок " << playerNum << ", вы набрали кость. Ваши кости: " << endl;
                printHand(playerHand, v);            
                cсс = cсс - 1;
                vv++;
            }

            if (cсс == 0 && playerHand[v - 1].leftValue != leftEnd && playerHand[v - 1].rightValue != leftEnd && playerHand[v - 1].leftValue != rightEnd && playerHand[v - 1].rightValue != rightEnd) {
                if (playerNum == 1) {         
                    chek1 = false;
                }

                else {
                    chek2 = false;
                }
                return false;

            }

            playerHandSize = playerHandSize + vv;           
    }
    return true;
}

//добавление фишки с левой строны 
void moveLeft(DominoTile chosenTile, int leftEnd) {

    if (chosenTile.leftValue == leftEnd) {
        DominoTile temp = { chosenTile.rightValue, chosenTile.leftValue }; 
        for (int i = boardSize - 1; i >= 0; i--) {
            board[i + 1] = board[i]; 
        }
        board[0] = temp;
        boardSize++;
    }

    else {
        for (int i = boardSize - 1; i >= 0; i--) {
            board[i + 1] = board[i];
        }
        board[0] = chosenTile;
        boardSize++;
    }
}

void moveRight(DominoTile chosenTile, int rightEnd) {
    if (chosenTile.rightValue == rightEnd) {
        DominoTile temp = { chosenTile.rightValue, chosenTile.leftValue }; //
        board[boardSize++] = temp;
    }

    else {
        board[boardSize++] = chosenTile;
    }
}

bool playTurn(DominoTile playerHand[], int& playerHandSize, const int playerNum) {

    if (playerHandSize == 0) {
        return false;
    }
    printHand(playerHand, playerHandSize);

    int choice;
  
    if (boardSize == 0) {
        cout << "Игрок " << playerNum << ", выберите домино для хода (индекс): ";
        cin >> choice;

        if (choice < 0 || choice >= playerHandSize) {
            cout << "Неверный ввод. Повторите попытку." << endl;
            return false;
        }

        DominoTile chosenTile = playerHand[choice];
        board[boardSize++] = chosenTile;
    }
    else {
        int leftEnd = board[0].leftValue;
        int rightEnd = board[boardSize - 1].rightValue;

        bool chek = perebor(playerHandSize, playerHand, leftEnd, rightEnd, playerNum);
        if (chek == false) {   
            return true;          
        }
       
        cout << "Игрок " << playerNum << ", выберите кость для хода (индекс): ";
        cin >> choice;

        if (choice < 0 || choice >= playerHandSize) {
            cout << "Неверный ввод.  Повторите попытку." << endl;
            return false;
        }

        DominoTile chosenTile = playerHand[choice];

        if ((chosenTile.leftValue == leftEnd && chosenTile.rightValue == rightEnd) || (chosenTile.leftValue == rightEnd && chosenTile.rightValue == leftEnd)) {
            int lefrigh;

            cout << "Вы хотите поставить кость с левой или с правой стороны доски? (0 - слево, 1 - справа)  -  "; cin >> lefrigh;

            if (lefrigh != 0 && lefrigh != 1) {
                cout << "Ошибка ввода" << endl;
                return false;
            }

            else {
                if (lefrigh == 0) {
                    moveLeft(chosenTile, leftEnd);
                }

                else {
                    moveRight(chosenTile, rightEnd);
                }
            }
        }

        else {
            if (chosenTile.leftValue == leftEnd || chosenTile.rightValue == leftEnd) {
                moveLeft(chosenTile, leftEnd);
            }

            else if (chosenTile.leftValue == rightEnd || chosenTile.rightValue == rightEnd) {
                moveRight(chosenTile, rightEnd);
            }

            else {
                cout << "Ходить этой фишкой невозможно. Повторите попытку." << endl;
                return false;
            }
        }
    }

    for (int i = choice; i < playerHandSize - 1; i++) {
        playerHand[i] = playerHand[i + 1];
    }
    playerHandSize--;

    return true;
}


void printBoard() {

    cout << endl;
    cout << "-------------" << endl;
    cout << "Игровой стол: ";
    for (int i = 0; i < boardSize; i++) {
        cout << "[" << board[i].leftValue << "|" << board[i].rightValue << "] ";
    }
    cout << endl;
    cout << "-------------" << endl;
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "Игра началась!\n" << "\n";

    initializeDeck();
    dealHands();

    while (player1HandSize > 0 && player2HandSize > 0) {
        bool validMove = false;

        while (!validMove) {

            chek1 = true;
            validMove = playTurn(player1Hand, player1HandSize, 1);
            printBoard();
        }

        if (chek1 == false && chek2 == false) {
            break;
        }

        validMove = false;

        while (!validMove && player1HandSize > 0) {

            chek2 = true;
            validMove = playTurn(player2Hand, player2HandSize, 2);
            printBoard();
        }    

        if (chek1 == false && chek2 == false) {
            break;
        }
    }

    if (player1HandSize == 0) {
        cout << "Игрок 1 победил!" << endl;
    }
    else if (player2HandSize == 0) {
        cout << "Игрок 2 победил!" << endl;
    }
    else {
        cout << "Оба игрока не могут ходить! Ничья!";
    }

    return 0;
}
