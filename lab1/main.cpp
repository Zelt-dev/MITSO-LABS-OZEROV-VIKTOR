#include iostream
#include cmath
#include stdexcept
#include string

 Пользовательский класс исключений
class CustomException  public stdexception {
private
    stdstring message;
    double value;
    int errorCode;

public
    CustomException(const stdstring& msg, double val, int code)
         message(msg), value(val), errorCode(code) {}

    const char what() const noexcept override {
        return message.c_str();
    }

    double getValue() const { return value; }
    int getErrorCode() const { return errorCode; }
};

 Функция вычисления Z1
void calculateZ1(double b, double& z1) {
    if (b  b - 4  0) {
        throw CustomException(Ошибка отрицательное значение под корнем в Z1, b, 101);
    }
    double sqrtPart = stdsqrt(b  b - 4);
    if (sqrtPart + b + 2 == 0) {
        throw CustomException(Ошибка деление на ноль в Z1, b, 102);
    }
    z1 = stdsqrt(2  b + 2  sqrtPart)  (sqrtPart + b + 2);
}

 Функция вычисления Z2
void calculateZ2(double b, double& z2) {
    if (b + 2 = 0) {
        throw CustomException(Ошибка отрицательное значение под корнем в Z2, b, 103);
    }
    z2 = 1  stdsqrt(b + 2);
}

int main() {
    setlocale(0, );
    double b, z1 = 0, z2 = 0;
    stdcout  Введите значение b ;
    stdcin  b;

    try {
        calculateZ1(b, z1);
        calculateZ2(b, z2);

        stdcout  Z1 =   z1  stdendl;
        stdcout  Z2 =   z2  stdendl;

    }
    catch (const CustomException& e) {
        stdcerr  Произошло исключение   e.what()  stdendl;
        stdcerr  Значение   e.getValue()  , Код ошибки   e.getErrorCode()  stdendl;
    }
    catch (const stdexception& e) {
        stdcerr  Общее исключение   e.what()  stdendl;
    }

    return 0;
}
