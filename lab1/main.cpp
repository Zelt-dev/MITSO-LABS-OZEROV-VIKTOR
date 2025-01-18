#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>

// Пользовательский класс исключений
class CustomException : public std::exception {
private:
    std::string message;
    double value;
    int errorCode;

public:
    CustomException(const std::string& msg, double val, int code)
        : message(msg), value(val), errorCode(code) {}

    const char* what() const noexcept override {
        return message.c_str();
    }

    double getValue() const { return value; }
    int getErrorCode() const { return errorCode; }
};

// Функция вычисления Z1
void calculateZ1(double b, double& z1) {
    if (b * b - 4 < 0) {
        throw CustomException("Ошибка: отрицательное значение под корнем в Z1", b, 101);
    }
    double sqrtPart = std::sqrt(b * b - 4);
    if (sqrtPart + b + 2 == 0) {
        throw CustomException("Ошибка: деление на ноль в Z1", b, 102);
    }
    z1 = std::sqrt(2 * b + 2 * sqrtPart) / (sqrtPart + b + 2);
}

// Функция вычисления Z2
void calculateZ2(double b, double& z2) {
    if (b + 2 <= 0) {
        throw CustomException("Ошибка: отрицательное значение под корнем в Z2", b, 103);
    }
    z2 = 1 / std::sqrt(b + 2);
}

int main() {
    setlocale(LC_ALL, "");

    double b, z1 = 0, z2 = 0;
    std::cout << "Введите значение b: ";
    std::cin >> b;

    try {
        calculateZ1(b, z1);
        calculateZ2(b, z2);

        std::cout << "Z1 = " << z1 << std::endl;
        std::cout << "Z2 = " << z2 << std::endl;
    }
    catch (const CustomException& e) {
        std::cerr << "Произошло исключение: " << e.what() << std::endl;
        std::cerr << "Значение: " << e.getValue() << ", Код ошибки: " << e.getErrorCode() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Общее исключение: " << e.what() << std::endl;
    }

    return 0;
}
