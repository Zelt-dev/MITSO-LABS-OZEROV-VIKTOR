#include <iostream>
#include <initializer_list>
#include <string>

namespace containers {

    template <typename T>
    class DRList {
    public:
        // Вложенная структура Node
        struct Node {
            T value;
        private:
            Node* next;
            Node* prev;

            // Приватный конструктор, доступен только DRList
            Node(const T& value) : value(value), next(nullptr), prev(nullptr) {}
            Node(T&& value) : value(std::move(value)), next(nullptr), prev(nullptr) {}

            friend class DRList<T>; // DRList имеет доступ к Node
        };

        // Конструктор по умолчанию
        DRList() : head(nullptr), size(0) {}

        // Конструктор от initializer_list
        DRList(std::initializer_list<T> init_list) : DRList() {
            for (const T& value : init_list) {
                push_back(value);
            }
        }

        // Конструктор копирования
        DRList(const DRList& other) : DRList() {
            Node* current = other.head;
            if (!current) return;

            do {
                push_back(current->value);
                current = current->next;
            } while (current != other.head);
        }

        // Оператор копирующего присваивания
        DRList& operator=(const DRList& other) {
            if (this == &other) return *this;

            clear();
            Node* current = other.head;
            if (!current) return *this;

            do {
                push_back(current->value);
                current = current->next;
            } while (current != other.head);

            return *this;
        }

        // Конструктор перемещения
        DRList(DRList&& other) noexcept : head(other.head), size(other.size) {
            other.head = nullptr;
            other.size = 0;
        }

        // Оператор перемещающего присваивания
        DRList& operator=(DRList&& other) noexcept {
            if (this == &other) return *this;

            clear();
            head = other.head;
            size = other.size;

            other.head = nullptr;
            other.size = 0;

            return *this;
        }

        // Деструктор
        ~DRList() {
            clear();
        }

        // Метод добавления элемента в конец
        void push_back(const T& value) {
            Node* new_node = new Node(value);
            if (!head) {
                head = new_node;
                head->next = head;
                head->prev = head;
            }
            else {
                Node* tail = head->prev;
                tail->next = new_node;
                new_node->prev = tail;
                new_node->next = head;
                head->prev = new_node;
            }
            ++size;
        }

        // Метод удаления элемента из конца
        void pop_back() {
            if (!head) return;

            if (head == head->next) { // Один элемент
                delete head;
                head = nullptr;
            }
            else {
                Node* tail = head->prev;
                tail->prev->next = head;
                head->prev = tail->prev;
                delete tail;
            }
            --size;
        }

        // Метод добавления элемента в произвольную позицию
        void insert(size_t pos, const T& value) {
            if (pos > size) return;

            if (pos == size) {
                push_back(value);
                return;
            }

            Node* new_node = new Node(value);
            Node* current = head;

            for (size_t i = 0; i < pos; ++i) {
                current = current->next;
            }

            new_node->prev = current->prev;
            new_node->next = current;
            current->prev->next = new_node;
            current->prev = new_node;

            if (pos == 0) {
                head = new_node;
            }

            ++size;
        }

        // Оператор индексирования
        T& operator[](size_t index) {
            Node* current = head;
            for (size_t i = 0; i < index; ++i) {
                current = current->next;
            }
            return current->value;
        }

        // Получение размера
        size_t get_size() const {
            return size;
        }

        // Метод очистки списка
        void clear() {
            while (size > 0) {
                pop_back();
            }
        }

        // Вывод элементов списка
        void print() const {
            if (!head) {
                std::cout << "Список пуст" << std::endl;
                return;
            }

            Node* current = head;
            do {
                std::cout << current->value << " ";
                current = current->next;
            } while (current != head);
            std::cout << std::endl;
        }

    private:
        Node* head; // Указатель на начало списка
        size_t size; // Размер списка
    };

} // namespace containers

int main() {
    using namespace containers;
    setlocale(0, "");

    DRList<int> intList = { 1, 2, 3 };
    DRList<double> doubleList = { 1.1, 2.2, 3.3 };
    DRList<std::string> stringList = { "hello", "world" };

    std::cout << "Работа с int:" << std::endl;
    intList.print();
    intList.push_back(4);
    intList.insert(2, 10);
    intList.print();
    intList.pop_back();
    intList.print();

    std::cout << "\nРабота с double:" << std::endl;
    doubleList.print();

    std::cout << "\nРабота с string:" << std::endl;
    stringList.print();

    return 0;
}
