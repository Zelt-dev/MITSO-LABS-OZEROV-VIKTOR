#include <iostream>
#include <stdexcept>
#include <initializer_list>


class VectError : public std::exception {
public:
    const char* what() const noexcept override {
        return "VectError: Operation failed";
    }
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
            throw VectError();
        --size_;
    }


    T& operator[](size_t index) {
        if (index >= size_)
            throw VectError();
        return data[index];
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
};


class QueueEmptyError : public std::exception {
public:
    const char* what() const noexcept override {
        return "QueueEmptyError: Queue is empty";
    }
};


template <typename T>
class Queue {
private:
    Vect<T> data;
    size_t front_index;

public:

    Queue() : front_index(0) {}


    void enqueue(const T& value) {
        data.push_back(value);
    }


    void dequeue() {
        if (empty())
            throw QueueEmptyError();
        ++front_index;
    }


    T& front() {
        if (empty())
            throw QueueEmptyError();
        return data[front_index];
    }


    bool empty() const {
        return front_index >= data.size();
    }
};


int main() {
    try {
        Queue<int> q_int;
        q_int.enqueue(10);
        q_int.enqueue(20);
        std::cout << "Front: " << q_int.front() << std::endl;
        q_int.dequeue();
        std::cout << "Front after dequeue: " << q_int.front() << std::endl;
        q_int.dequeue();
        q_int.dequeue();

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    try {
        Queue<std::string> q_str;
        q_str.enqueue("Hello");
        q_str.enqueue("World");
        std::cout << "Front: " << q_str.front() << std::endl;
        q_str.dequeue();
        std::cout << "Front after dequeue: " << q_str.front() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
