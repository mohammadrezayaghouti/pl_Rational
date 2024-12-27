#include "BigNumber.h"

#include <stack>
#include <stdexcept>

BigNumber::BigNumber(std::string number) : head(nullptr), tail(nullptr) {
    isNegative = false;

    if (number[0] == '-') {
        isNegative = true;
        number = number.substr(1);
    }

    // Trim leading zeros
    int i = 0;
    for (; i < number.size() && number[i] == '0'; i++);
    number = number.substr(i);

    number = number.empty() ? "0" : number;

    for (auto it = number.rbegin(); it != number.rend(); it++) adddigit(*it - '0');
}

BigNumber::BigNumber(const char* str) : BigNumber(static_cast<std::string>(str)) {}

BigNumber::BigNumber(const BigNumber& other) {
    Node* iterator = other.head;
    head = tail = nullptr;

    while (iterator) {
        adddigit(iterator->value);
        iterator = iterator->next;
    }

    isNegative = other.isNegative;
}

BigNumber::~BigNumber() {
    Node* current = head;

    while (current) {
        Node* target = current;
        current = current->next;
        delete target;
    }
}

void BigNumber::adddigit(int digit) {
    Node* newnode = new Node(digit);

    if (!head) {
        head = tail = newnode;
    }
    else {
        tail->next = newnode;
        tail = newnode;
    }
}

BigNumber BigNumber::add(const BigNumber& other) const {
    Node* p1 = head;
    Node* p2 = other.head;
    BigNumber result("0");

    result.head = result.tail = nullptr;
    int carry = 0;

    while (p1 || p2 || carry) {
        int val1 = p1 ? p1->value : 0;
        int val2 = p2 ? p2->value : 0;
        int sum = val1 + val2 + carry;

        carry = sum / 10;

        int digit = sum % 10;

        result.adddigit(digit);

        if (p1) p1 = p1->next;
        if (p2) p2 = p2->next;
    }

    result.isNegative = isNegative;

    return result;
}

BigNumber BigNumber::multiply(const BigNumber& other) const {
    BigNumber result("0");

    if (isZero() || other.isZero()) return result;

    Node* left = other.head, * right = head;

    for (; right && left; right = right->next, left = left->next);

    if (!right) {
        right = head;
        left = other.head;
    }
    else {
        right = other.head;
        left = head;
    }

    for (int pos = 0; right; right = right->next, pos++) {
        BigNumber ss;
        int carry = 0;

        for (int k = 0; k < pos; k++) ss.adddigit(0);
        for (Node* j = left; j; j = j->next) {
            int value = right->value * j->value + carry;
            carry = value / 10;
            ss.adddigit(value % 10);
        }

        if (carry) ss.adddigit(carry);

        result = result.add(ss);
    }

    result.isNegative = isNegative ^ other.isNegative;

    return result;
}

BigNumber BigNumber::negate() const {
    BigNumber result = *this;
    result.isNegative = !isNegative;
    return result;
}

bool BigNumber::isZero() const {
    return head == nullptr || (head->value == 0 && head->next == nullptr);
}

bool BigNumber::operator>=(const BigNumber& other) const {
    return !(*this < other);
}

BigNumber BigNumber::divideby(const BigNumber& divisor) const {
    if (divisor.isZero()) {
        throw std::invalid_argument("Division by zero");
    }
    else if (*this < divisor) {
        return "0";
    }
    else if (divisor == "1") {
        return *this;
    }

    BigNumber current;
    BigNumber result;

    std::stack<int> dvdnt;

    for (Node* i = head; i; i = i->next) dvdnt.push(i->value);

    // 4 -> 3 -> 2 -> 1, 1 -> 2
    // 1234 (dvdnt) / 21 (divisor)
    // 123  - 105
    //  184 - 168
    //   16

    for (; !dvdnt.empty() && current < divisor; dvdnt.pop())
        current.pushFront(dvdnt.top());

    while (true) {
        int q = 1;
        BigNumber temp = divisor;

        while (temp.add(divisor) <= current) {
            temp = temp.add(divisor);
            q++;
        }

        current = current.subtract(temp);
        result.pushFront(q);

        if (!dvdnt.empty()) {
            current.pushFront(dvdnt.top());
            current.trim();
            dvdnt.pop();
        }
        else {
            break;
        }
    }

    return result;
}

BigNumber BigNumber::subtract(const BigNumber& other) const {
    if (*this < other) {
        throw std::invalid_argument("Subtraction results in negative number");
    }

    BigNumber result("0");
    result.head = result.tail = nullptr;
    Node* p1 = head;
    Node* p2 = other.head;
    int borrow = 0;

    while (p1 || p2 || borrow) {
        int val1 = p1 ? p1->value : 0;
        int val2 = p2 ? p2->value : 0;
        int diff = val1 - val2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        }
        else {
            borrow = 0;
        }
        result.adddigit(diff);
        if (p1) p1 = p1->next;
        if (p2) p2 = p2->next;
    }

    while (result.head && result.head->value == 0) {
        Node* temp = result.head;
        result.head = result.head->next;
        delete temp;
    }

    if (!result.head) {
        result.adddigit(0);
    }

    result.isNegative = isNegative;

    return result.trim();
}

bool BigNumber::operator<(const BigNumber& other) const {
    if (isNegative != other.isNegative) return isNegative;

    Node* i = head, * j = other.head;
    std::stack<std::pair<int, int>> stack;

    for (; i && j; i = i->next, j = j->next) stack.push({ i->value, j->value });

    if (!j != !i) return (!i) ^ isNegative;

    while (!stack.empty() && stack.top().first == stack.top().second) stack.pop();

    return stack.empty() ? false : (stack.top().first < stack.top().second) ^ isNegative;
}

bool BigNumber::operator==(const BigNumber& other) const {
    if (isNegative != other.isNegative) return false;

    Node* i = head, * j = other.head;

    for (; i && j && i->value == j->value; i = i->next, j = j->next);

    return !i && !j;
}

BigNumber BigNumber::modulo(const BigNumber& other) const {
    if (other.isZero()) {
        throw std::invalid_argument("Division by zero");
    }
    else if (*this < other) {
        return *this;
    }
    else if (*this == other) {
        return "0";
    }

    // x % y == x - ([x / y] * y)
    BigNumber result = this->subtract(divideby(other).multiply(other));

    return result;
}

bool BigNumber::getIsNegative() const { return isNegative; }

BigNumber::operator std::string() const {
    std::string str = this->isNegative ? "-" : "";
    Node* iterator = this->head;

    while (iterator) {
        str = static_cast<char>(iterator->value + '0') + str;
        iterator = iterator->next;
    }

    return str;
}
