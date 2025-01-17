#include <iostream>
#include <string>
#include <cctype> // для функции isdigit и isupper
#include <windows.h>
#include <conio.h>

class CustomString {
private:
    std::string str;
public:
    // Перегрузка оператора ввода
    friend std::istream& operator>>(std::istream& in, CustomString& obj) {
        std::getline(in, obj.str);
        return in;
    }

    // Перегрузка оператора вывода
    friend std::ostream& operator<<(std::ostream& out, const CustomString& obj) {
        for (char c : obj.str) {
            if (!std::isupper(c) && !std::isdigit(c)) {
                out << c;
            }
        }
        return out;
    }
};

int main() {

    setlocale(LC_ALL, "Russian");
    CustomString customStr;

    std::cout << "Введите строку: ";
    std::cin >> customStr;

    std::cout << "Обработанная строка: " << customStr << std::endl;

    return 0;
}
