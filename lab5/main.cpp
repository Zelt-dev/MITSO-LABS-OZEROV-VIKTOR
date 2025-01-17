#include <iostream>
#include <stdexcept>
#include <initializer_list>

class CustomException : public std::exception {
public:
    explicit CustomException(const char* message) : msg_(message) {}
    const char* what() const noexcept override {
        return msg_;
    }
private:
    const char* msg_;
};

template <typename T>
class Vect {
private:
    T* data;
    size_t size_;
    size_t capacity;

    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i)
            new_data[i] = std::move(data[i]);
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    Vect() : data(nullptr), size_(0), capacity(0) {}

    Vect(std::initializer_list<T> init) : Vect() {
        for (const auto& elem : init)
            push_back(elem);
    }

    Vect(const Vect& other) : data(new T[other.capacity]), size_(other.size_), capacity(other.capacity) {
        for (size_t i = 0; i < size_; ++i)
            data[i] = other.data[i];
    }

    Vect& operator=(const Vect& other) {
        if (this == &other) return *this;
        delete[] data;
        size_ = other.size_;
        capacity = other.capacity;
        data = new T[capacity];
        for (size_t i = 0; i < size_; ++i)
            data[i] = other.data[i];
        return *this;
    }

    ~Vect() { delete[] data; }

    void push_back(const T& value) {
        if (size_ == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[size_++] = value;
    }

    void pop_back() {
        if (size_ == 0)
            throw CustomException("VectError: Cannot pop from an empty vector");
        --size_;
    }

    T& operator[](size_t index) {
        if (index >= size_)
            throw CustomException("VectError: Index out of bounds");
        return data[index];
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // Итератор для всех элементов
    class Iterator {
    public:
        virtual bool has_next() = 0;
        virtual T& next() = 0;
        virtual ~Iterator() = default;
    };

    // Итератор для положительных элементов
    class PositiveIterator : public Iterator {
    private:
        Vect& vect;
        size_t index;

        void move_to_next_valid() {
            while (index < vect.size() && vect.data[index] <= 0)
                ++index;
        }

    public:
        explicit PositiveIterator(Vect& vect) : vect(vect), index(0) {
            move_to_next_valid();
        }

        bool has_next() override {
            return index < vect.size();
        }

        T& next() override {
            if (!has_next())
                throw CustomException("PositiveIterator: No more elements");
            T& value = vect.data[index];
            ++index;
            move_to_next_valid();
            return value;
        }
    };

    // Итератор для отрицательных элементов
    class NegativeIterator : public Iterator {
    private:
        Vect& vect;
        size_t index;

        void move_to_next_valid() {
            while (index < vect.size() && vect.data[index] >= 0)
                ++index;
        }

    public:
        explicit NegativeIterator(Vect& vect) : vect(vect), index(0) {
            move_to_next_valid();
        }

        bool has_next() override {
            return index < vect.size();
        }

        T& next() override {
            if (!has_next())
                throw CustomException("NegativeIterator: No more elements");
            T& value = vect.data[index];
            ++index;
            move_to_next_valid();
            return value;
        }
    };
};

// Тестирование
int main() {
    try {
        Vect<int> vec = { 1, -2, 3, -4, 5, -6 };

        std::cout << "Positive elements: ";
        Vect<int>::PositiveIterator pos_iter(vec);
        while (pos_iter.has_next()) {
            std::cout << pos_iter.next() << " ";
        }
        std::cout << std::endl;

        std::cout << "Negative elements: ";
        Vect<int>::NegativeIterator neg_iter(vec);
        while (neg_iter.has_next()) {
            std::cout << neg_iter.next() << " ";
        }
        std::cout << std::endl;
    }
    catch (const CustomException& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
