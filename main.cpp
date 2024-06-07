#include<iostream>
#include<string>
#include<fstream>
#include<algorithm>
#include<string.h>
#include <conio.h>

using namespace std;

size_t strlen(const char* str){
    const char * p = str;
    while( *p++ );
    return p-str-1;
}
// Исключения
class Wrong_number : public exception{
    protected:
    char* str;
    public:
    Wrong_number() {str = NULL;}
    Wrong_number(const char* s){
        str = new char[strlen(s)+1];
        strcpy(str, s);
    }
    Wrong_number(const Wrong_number& e){
        str = new char[strlen(e.str) + 1];
        strcpy(str, e.str);
    }
    ~Wrong_number() {delete[] str;}
    void print() {cout << "Wrong_number: " << str <<";\n";}
};

class KeyOutOfRange : public exception{
    protected:
    char* str;
    public:
    KeyOutOfRange(){str = NULL;}
    KeyOutOfRange(const char* s){
        str = new char[strlen(s) + 1];
        strcpy(str, s);
    }
    KeyOutOfRange(const KeyOutOfRange& e){
        str = new char[strlen(e.str) + 1];
        strcpy(str, e.str);
    }
    ~KeyOutOfRange() {delete[] str;}
    void print() {cout << "KeyOutOfRange: " << str <<";\n";}
};

// Шифры
class BaseCipher{
    public:
    string message;

    BaseCipher() {message="hello world";}
    BaseCipher(string Message) {message=Message;}
    ~BaseCipher(){}

 
    virtual void GetMessage() {cout << message << endl;}

    

};

// Шифр Цезаря
class Caesar_cipher : public BaseCipher{
    private:
    string message;
    int step;
    
    public:
    Caesar_cipher() {message="hello world"; step=3;}
    Caesar_cipher(string Message) {message=Message, step=3;}
    Caesar_cipher(string Message, int Step) {message=Message, step=Step;}
    ~Caesar_cipher(){}

    string encode(){
        string new_message = message;
        for(char& c : new_message){
            if (isalpha(c)){
                c = tolower(c);
                c = ((c - 'a' + step) % 26) + 'a';
            }
        }
        return new_message;
    }

    string decode(){
        string new_message = message;
        for(char& c : new_message){
            if (isalpha(c)){
                c = tolower(c);
                c = (((c - 'a' - step) % 26)+ 26) % 26 + 'a';
            }
        }
        return new_message;
    }
};

//Шифр Виженера
class Vigenere_cipher : public BaseCipher{
    private:
    string message;
    string key;

    public:
    Vigenere_cipher() {message="hello world"; key="key";}
    Vigenere_cipher(string Message) {message=Message; key="key";}
    Vigenere_cipher(string Message, string Key) {message=Message; key=Key;}
    ~Vigenere_cipher(){}

    string encode(){
        string new_message = message;
        int key_size = key.size();
        int message_size = message.size();
        int cnt;
        for(char& c : new_message){
            if ((c >='A' && c <= 'Z') || (c >='a' && c <= 'z')){
            c = tolower(c);
            if (cnt>=key_size) cnt=0;
            if ('a' < (-'z' + key[cnt] + c) && (-'z' + key[cnt] + c) <= 'z')
                c = -'z' + key[cnt] + c - 1;
            else c = -'a' + key[cnt] + c;
            cnt++;
            }
        }
        return new_message;
    }

    string decode(){
        string new_message = message;
        int key_size = key.size();
        int message_size = message.size();
        int cnt;
        for(char& c: new_message){
            if ((c >='A' && c <= 'Z') || (c >='a' && c <= 'z')){
            c = tolower(c);
            if (cnt>=key_size) cnt=0;
            if ('a' <= 'z'-key[cnt]+c && 'z'-key[cnt]+c < 'z')
                c = 'z'-key[cnt]+c + 1;
            else c = 'a'-key[cnt]+c;
            cnt ++;
            }
        }
        return new_message;
    }
};

// Аффинный шифр
class Affine_cipher : public BaseCipher{
    private:
    string message;
    int step; // b
    int key; // a
    int length = 26;
    int key_for_EN[11] = {3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25};


    public:
    Affine_cipher() {message="hello world"; step=3; key=3;}
    Affine_cipher(string Message) {message=Message;step=3;key=3;}
    Affine_cipher(string Message, int Step) {message=Message;step=Step;key=3;}
    Affine_cipher(string Message, int Step, int Key) {message=Message;step=Step;key=Key;}
    ~Affine_cipher(){}

    int calc(int a){
        for (int &i : key_for_EN) if ((i*key)%26==1) return i;
        return 9;
    };

    string encode(){
        // E(x) = (ax + b) mod m
        // length of EN alphabet - m
        // x - letter
        // for 26 -> 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25
        // now it's 3
        string new_message = message;
        for(char& c : new_message){
            if ((c >='A' && c <= 'Z') || (c >='a' && c <= 'z')){
            c = tolower(c);
            c = 'a' + ((key * (c-'a') + step) % length);
            }
        }
        return new_message;
    }

    string decode(){
        // D(y) = a^-1 (y + m - b) mod m
        // a^-1 - обратное числу a по модулю m
        // a*(a^-1) mod m = 1
        // for 3 it's 9
        // for all: 9, 21, 15, 3, 19, 7, 23, 11, 5, 17, 25
        int obratnoe = calc(key);
        string new_message = message;
        for(char& c : new_message){
            if ((c >='A' && c <= 'Z') || (c >='a' && c <= 'z')){
            c = tolower(c);
            c = 'a' + (obratnoe*((c-'a') + length - step) % length);
            }
        }

        return new_message;
    }

};

// Шифр Полибия
class Polybius_cipher : public BaseCipher{
    private:
    string message;
    char polybiusSquare[5][5] = {{'a', 'b', 'c', 'd', 'e'},
                                 {'f', 'g', 'h', 'i', 'k'},
                                 {'l', 'm', 'n', 'o', 'p'},
                                 {'q', 'r', 's', 't', 'u'},
                                 {'v', 'w', 'x', 'y', 'z'}};    
    public:
    Polybius_cipher() {message="hello world";}
    Polybius_cipher(string Message) {message=Message;}
    ~Polybius_cipher(){}

    string encode() {
        string new_message = "";
        for (char c : message) if (isalpha(c)) {
                char letter = tolower(c);
                if (letter == 'j') letter = 'i'; // В шифре Полибия буквы I и J обычно заменяются друг на друга
                for (int i = 0; i < 5; i++)
                    for (int j = 0; j < 5; j++) 
                        if (polybiusSquare[i][j] == letter) 
                            new_message += to_string(i + 1) + to_string(j + 1) + " ";
                        
                    
            } else if (isspace(c)) new_message += " "; // Добавляем пробелы без шифрования
            

        return new_message;
    }
    string decode() {
        string new_message = "";
        string temp = "";
        for (char c : message) if (isdigit(c)){
                temp += c;
                if (temp.length() == 2) {
                    int row = temp[0] - '0' - 1;
                    int col = temp[1] - '0' - 1;
                    new_message += polybiusSquare[row][col];
                    temp = "";
                }
            }else if (isspace(c)) new_message += " "; // Добавляем пробелы
        return new_message;
    }
    
};

// Шифр Плейфера
class Playfair_cipher : public BaseCipher{
    private:
    string message;
    string key;

    public:
    Playfair_cipher() {message="hello world"; key="key";}
    Playfair_cipher(string Message) {message=Message; key="key";}
    Playfair_cipher(string Message, string Key) {message=Message; key=Key;}
    ~Playfair_cipher(){}

    string encode() {
        // Генерируем таблицу Playfair
        string table = "";
        string new_message = "";

        for (char c : key)
            if (isalpha(c) && table.find(tolower(c)) == string::npos) {
                table += tolower(c);
                if (c == 'i') table += 'j'; // Заменяем J на I
            }

        for (char c = 'a'; c <= 'z'; ++c)
            if (c != 'j' && table.find(c) == string::npos) table += c;

        // Формируем таблицу 5x5
        char playfairSquare[5][5];
        int index = 0;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                playfairSquare[i][j] = table[index++];

        // Удаляем все пробелы из сообщения и заменяем дубликаты исходной буквы на X
        string timeMessage = "";
        for (char c : message)
            if (isalpha(c))
                timeMessage += tolower(c);
                if (timeMessage.length() % 2 == 1 && timeMessage.back() == timeMessage[timeMessage.length() - 2])
                    timeMessage.insert(timeMessage.length() - 1, "X");

        // Шифруем пары букв
        for (size_t i = 0; i < timeMessage.length(); i += 2) {
            char a = timeMessage[i];
            char b = timeMessage[i + 1];
            int row1, col1, row2, col2;

            // Находим позиции букв в таблице
            for (int row = 0; row < 5; ++row)
                for (int col = 0; col < 5; ++col) {
                    if (playfairSquare[row][col] == a) {
                        row1 = row;
                        col1 = col;
                    }
                    if (playfairSquare[row][col] == b) {
                        row2 = row;
                        col2 = col;
                    }
                }

            // Шифруем пару букв
            if (row1 == row2) {
                new_message += playfairSquare[row1][(col1 + 1) % 5];
                new_message += playfairSquare[row2][(col2 + 1) % 5];
            } else if (col1 == col2) {
                new_message += playfairSquare[(row1 + 1) % 5][col1];
                new_message += playfairSquare[(row2 + 1) % 5][col2];
            } else {
                new_message += playfairSquare[row1][col2];
                new_message += playfairSquare[row2][col1];
            }
        }

        return new_message;
    }

    // Функция для расшифровки сообщения, зашифрованного шифром Плейфера
    string decode() {
        // Генерируем таблицу Playfair
        string table = "";
        string new_message = "";

        for (char c : key)
            if (isalpha(c) && table.find(tolower(c)) == string::npos) {
                table += tolower(c);
                if (c == 'i')table += 'j'; // Заменяем J на I
            }

        for (char c = 'a'; c <= 'z'; ++c)
            if (c != 'j' && table.find(c) == string::npos) table += c;

        // Формируем таблицу 5x5
        char playfairSquare[5][5];
        int index = 0;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                playfairSquare[i][j] = table[index++];

        // Расшифровываем пары букв
        for (size_t i = 0; i < message.length(); i += 2) {
            char a = message[i];
            char b = message[i + 1];
            int row1, col1, row2, col2;

            // Находим позиции букв в таблице
            for (int row = 0; row < 5; ++row)
                for (int col = 0; col < 5; ++col) {
                    if (playfairSquare[row][col] == a) {
                        row1 = row;
                        col1 = col;
                    }
                    if (playfairSquare[row][col] == b) {
                        row2 = row;
                        col2 = col;
                    }
                }

            // Расшифровываем пару букв
            if (row1 == row2) {
                new_message += playfairSquare[row1][(col1 + 4) % 5]; // Для расшифровки вычитаем 1, но так как у нас массивы начинаются с 0, то добавляем 4
                new_message += playfairSquare[row2][(col2 + 4) % 5];
            } else if (col1 == col2) {
                new_message += playfairSquare[(row1 + 4) % 5][col1];
                new_message += playfairSquare[(row2 + 4) % 5][col2];
            } else {
                new_message += playfairSquare[row1][col2];
                new_message += playfairSquare[row2][col1];
            }
        }

        return new_message;
    }
};


int main(){
    try{
    ifstream file("message.txt");
    string message, line;
    int cmd, cipher;    
    if (file.is_open()){
        while (getline(file, line)){
            message += line;
        }
        file.close();
    }else cout << "File error!" << endl;
    cout <<"Choose cipher (Enter number):\n \t1) Caesar\n \t2) Affine\n \t3) Vigenere\n \t4) Polybius\n \t5) Playfair\n";
    cin >> cipher;
    switch (cipher)
    {
        case 1:{
            int step;
            cout << "Enter an integer key in the range 0-25: "; cin >> step;
            if ((step < 0 ) || (step > 25)) {throw KeyOutOfRange("Key out of range 0-25");}
            Caesar_cipher C(message, step);
            cout << "Choose (Enter number):\n\t1) Encryption\n\t2) Decryption\n"; cin >> cmd;
            if (cmd == 1) {cout << C.encode() << endl;}
            else if (cmd == 2) {cout << C.decode() << endl;}
            else{throw Wrong_number("Invalid action number entered (1-2)");}
            break;
        }
        case 2:{
            int key_for_EN[11] = {3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25};
            int key, step;
            bool flag = false;
            cout << "Enter an integer key in the range 0-25: "; cin >> step;
            if ((step < 0 ) || (step > 25)) {throw KeyOutOfRange("Key out of range 0-25");}
            cout << "Enter a key (key must be relatively prime with the power of the alphabet): "; cin >> key;
            for(int i=0; i<sizeof(key_for_EN); i++) if (key == key_for_EN[i]) {flag = true; break;}
            if (!flag) throw Wrong_number("Key is not coprime with the power of the English alphabet");
            Affine_cipher A(message, step, key);
            cout << "Choose (Enter number):\n\t1) Encryption\n\t2) Decryption\n"; cin >> cmd;
            if (cmd == 1) {cout << A.encode() << endl;}
            else if (cmd == 2) {cout << A.decode() << endl;}
            else{throw Wrong_number("Invalid action number entered (1-2)");}
            break;
        }
        case 3:{
            string key;
            cout << "Enter an string key: "; cin >> key;
            Vigenere_cipher V(message, key);
            cout << "Choose (Enter number):\n\t1) Encryption\n\t2) Decryption\n"; cin >> cmd;
            if (cmd == 1) {cout << V.encode() << endl;}
            else if (cmd == 2) {cout << V.decode() << endl;}
            else{throw Wrong_number("Invalid action number entered (1-2)");}
            break;
        }
        case 4:{
            Polybius_cipher PO(message);
            cout << "Choose (Enter number):\n\t1) Encryption\n\t2) Decryption\n"; cin >> cmd;
            if (cmd == 1) {cout << PO.encode() << endl;}
            else if (cmd == 2) {cout << PO.decode() << endl;}
            else{throw Wrong_number("Invalid action number entered (1-2)");}
            break;
        }
        case 5:{
            string key;
            cout << "Enter an string key: "; cin >> key;
            Playfair_cipher PL(message, key);
            cout << "Choose (Enter number):\n\t1) Encryption\n\t2) Decryption\n"; cin >> cmd;
            if (cmd == 1) {cout << PL.encode() << endl;}
            else if (cmd == 2) {cout << PL.decode() << endl;}
            else{throw Wrong_number("Invalid action number entered (1-2)");}
            break;
        }
    default:
        break;
    }
    cout << endl;
    }
    catch(Wrong_number e){
        e.print();
    }catch(KeyOutOfRange e){
        e.print();
    }
    
    getch();
    return 0;
}
