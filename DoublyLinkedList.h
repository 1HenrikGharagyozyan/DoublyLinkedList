#pragma once

#include <iostream>
#include <iterator>
#include <initializer_list>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <vector>

template <typename _Ty>
class DoublyLinkedList
{
private:
    struct Node
    {
        _Ty _data;
        Node* _prev;
        Node* _next;

        Node(const _Ty& value, Node* prev = nullptr, Node* next = nullptr)
            : _data(value)
            , _prev(prev)
            , _next(next)
        {
        }

        Node(_Ty&& value, Node* prev = nullptr, Node* next = nullptr)
            : _data(std::move(value))
            , _prev(prev)
            , _next(next)
        {
        }

        template <typename... Args>
        Node(Node* prev, Node* next, Args&&... args)
            : _data(std::forward<Args>(args)...)
            , _prev(prev)
            , _next(next)
        {
        }

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        Node(Node&&) = delete;
        Node& operator=(Node&&) = delete;
    };

    Node* _head = nullptr;
    Node* _tail = nullptr;
    std::size_t _size = 0;

public:
    using value_type = _Ty;
    using size_type = std::size_t;
    using reference = _Ty&;
    using const_reference = const _Ty&;
    using pointer = _Ty*;
    using const_pointer = const _Ty*;

    class Iterator
    {
    private:
        Node* node;
        friend class DoublyLinkedList;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = _Ty;
        using difference_type = std::ptrdiff_t;
        using pointer = _Ty*;
        using reference = _Ty&;

        Iterator(Node* ptr = nullptr) : node(ptr) {}
        reference operator*() const { return node->_data; }
        pointer operator->() const { return &node->_data; }

        Iterator& operator++() { node = node->_next; return *this; }
        Iterator operator++(int) { Iterator temp = *this; ++(*this); return temp; }

        Iterator& operator--() { node = node->_prev; return *this; }
        Iterator operator--(int) { Iterator temp = *this; --(*this); return temp; }

        bool operator==(const Iterator& rhs) const { return node == rhs.node; }
        bool operator!=(const Iterator& rhs) const { return node != rhs.node; }

        Iterator operator+(size_type n) const;
        Iterator operator-(size_type n) const;

        Iterator& operator+=(size_type n);
        Iterator& operator-=(size_type n);

        Node* get() const { return node; }
    };

    class ConstIterator
    {
    private:
        Node* node;
        friend class DoublyLinkedList;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = _Ty;
        using difference_type = std::ptrdiff_t;
        using pointer = const _Ty*;
        using reference = const _Ty&;

        ConstIterator(Node* ptr = nullptr) : node(ptr) {}
        ConstIterator(const Iterator& it) : node(it.node) {}

        reference operator*() const { return node->_data; }
        pointer operator->() const { return &node->_data; }

        ConstIterator& operator++() { node = node->_next; return *this; }
        ConstIterator operator++(int) { ConstIterator tmp = *this; ++(*this); return tmp; }

        ConstIterator& operator--() { node = node->_prev; return *this; }
        ConstIterator operator--(int) { ConstIterator tmp = *this; --(*this); return tmp; }

        bool operator==(const ConstIterator& rhs) const { return node == rhs.node; }
        bool operator!=(const ConstIterator& rhs) const { return node != rhs.node; }

        ConstIterator operator+(size_type n) const;
        ConstIterator operator-(size_type n) const;

        ConstIterator& operator+=(size_type n);
        ConstIterator& operator-=(size_type n);

        Node* get() const { return node; }
    };

    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    DoublyLinkedList();
    ~DoublyLinkedList();
    DoublyLinkedList(const DoublyLinkedList& other);
    DoublyLinkedList(DoublyLinkedList&& other) noexcept;
    DoublyLinkedList(std::initializer_list<value_type> init);

    DoublyLinkedList& operator=(const DoublyLinkedList& other);
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept;

    iterator begin() { return iterator{ _head }; }
    iterator end() { return iterator{ nullptr }; }
    const_iterator begin() const { return const_iterator{ _head }; }
    const_iterator end() const { return const_iterator{ nullptr }; }
    const_iterator cbegin() const { return const_iterator{ _head }; }
    const_iterator cend() const { return const_iterator{ nullptr }; }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

    bool empty() const { return _size == 0; }
    size_type size() const { return _size; }

    reference front();
    reference back();
    const_reference front() const;
    const_reference back() const;

    void push_back(const_reference value);
    void push_front(const_reference value);

    void pop_front();
    void pop_back();

    void clear();

    template <typename... Args>
    reference emplace_front(Args&&... args);

    template <typename... Args>
    reference emplace_back(Args&&... args);

    template <typename... Args>
    iterator emplace(iterator pos, Args&&... args);

    iterator insert(iterator pos, const_reference value);
    iterator erase(iterator pos);

    void print() const;

    void sort(const std::function<bool(const value_type&, const value_type&)>& predicatde = std::less<value_type>());

    void unique();

    void merge(DoublyLinkedList& other);

    void reverse();

    void splice(iterator pos, DoublyLinkedList& other);

    bool operator==(const DoublyLinkedList& other) const;
    bool operator!=(const DoublyLinkedList& other) const;
};

// Implementation

template<typename _Ty>
DoublyLinkedList<_Ty>::DoublyLinkedList() = default;

template<typename _Ty>
DoublyLinkedList<_Ty>::~DoublyLinkedList()
{
    clear();
}

template<typename _Ty>
DoublyLinkedList<_Ty>::DoublyLinkedList(const DoublyLinkedList& other)
{
    for (const auto& val : other)
        push_back(val);
}

template<typename _Ty>
DoublyLinkedList<_Ty>::DoublyLinkedList(DoublyLinkedList&& other) noexcept
    : _head(other._head)
    , _tail(other._tail)
    , _size(other._size)
{
    other._head = nullptr;
    other._tail = nullptr;
    other._size = 0;
}

template<typename _Ty>
DoublyLinkedList<_Ty>::DoublyLinkedList(std::initializer_list<value_type> init)
    : _head(nullptr)
    , _tail(nullptr)
    , _size(0)
{
    for (auto it = init.begin(); it != init.end(); ++it)
        push_back(*it);
}

template<typename _Ty>
DoublyLinkedList<_Ty>& DoublyLinkedList<_Ty>::operator=(const DoublyLinkedList& other)
{
    if (this != &other)
    {
        clear();
        for (const auto& val : other)
            push_back(val);
    }
    return *this;
}

template<typename _Ty>
DoublyLinkedList<_Ty>& DoublyLinkedList<_Ty>::operator=(DoublyLinkedList&& other) noexcept
{
    if (this != &other)
    {
        clear();
        _head = other._head;
        _tail = other._tail;
        _size = other._size;

        other._head = nullptr;
        other._tail = nullptr;
        other._size = 0;
    }
    return *this;
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::reference DoublyLinkedList<_Ty>::front()
{
    if (!_head)
        throw std::out_of_range("front() on empty list");
    return _head->_data;
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::reference DoublyLinkedList<_Ty>::back()
{
    if (!_tail)
        throw std::out_of_range("back() on empty list");
    return _tail->_data;
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::const_reference DoublyLinkedList<_Ty>::front() const
{
    if (!_head)
        throw std::out_of_range("front() on empty list");
    return _head->_data;
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::const_reference DoublyLinkedList<_Ty>::back() const
{
    if (!_tail)
        throw std::out_of_range("back() on empty list");
    return _tail->_data;
}

template<typename _Ty>
void DoublyLinkedList<_Ty>::push_back(const_reference value)
{
    emplace_back(value);
}

template<typename _Ty>
void DoublyLinkedList<_Ty>::push_front(const_reference value)
{
    emplace_front(value);
}

template<typename _Ty>
void DoublyLinkedList<_Ty>::pop_front()
{
    if (!_head)
        throw std::out_of_range("pop_front() on empty list");

    Node* temp = _head;
    _head = _head->_next;

    if (_head)
        _head->_prev = nullptr;
    else
        _tail = nullptr;

    delete temp;
    --_size;
}

template<typename _Ty>
void DoublyLinkedList<_Ty>::pop_back()
{
    if (!_tail)
        throw std::out_of_range("pop_back() on empty list");

    Node* temp = _tail;
    _tail = _tail->_prev;

    if (_tail)
        _tail->_next = nullptr;
    else
        _head = nullptr;

    delete temp;
    --_size;
}

template<typename _Ty>
void DoublyLinkedList<_Ty>::clear()
{
    while (_head)
    {
        Node* temp = _head;
        _head = _head->_next;
        delete temp;
    }
    _tail = nullptr;
    _size = 0;
}

template<typename _Ty>
template <typename... Args>
typename DoublyLinkedList<_Ty>::reference DoublyLinkedList<_Ty>::emplace_front(Args&&... args)
{
    Node* new_node = new Node(std::forward<Args>(args)..., nullptr, _head);
    if (_head)
        _head->_prev = new_node;
    _head = new_node;
    if (!_tail)
        _tail = new_node;
    ++_size;
    return _head->_data;
}

template<typename _Ty>
template <typename... Args>
typename DoublyLinkedList<_Ty>::reference DoublyLinkedList<_Ty>::emplace_back(Args&&... args)
{
    Node* new_node = new Node(std::forward<Args>(args)..., _tail, nullptr);
    if (_tail)
        _tail->_next = new_node;
    _tail = new_node;
    if (!_head)
        _head = new_node;
    ++_size;
    return _tail->_data;
}

template<typename _Ty>
template <typename... Args>
typename DoublyLinkedList<_Ty>::iterator DoublyLinkedList<_Ty>::emplace(iterator pos, Args&&... args)
{
    Node* new_node = new Node(std::forward<Args>(args)..., pos.node->_prev, pos.node);
    if (pos.node->_prev)
        pos.node->_prev->_next = new_node;
    else
        _head = new_node;

    pos.node->_prev = new_node;
    ++_size;
    return iterator(new_node);
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::iterator
DoublyLinkedList<_Ty>::insert(iterator pos, const_reference value)
{
    return emplace(pos, value);
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::iterator
DoublyLinkedList<_Ty>::erase(iterator pos)
{
    if (!pos.node)
        throw std::out_of_range("erase() on invalid iterator");

    Node* temp = pos.node;
    if (temp->_prev)
        temp->_prev->_next = temp->_next;
    else
        _head = temp->_next;

    if (temp->_next)
        temp->_next->_prev = temp->_prev;
    else
        _tail = temp->_prev;

    delete temp;
    --_size;
    return iterator(temp->_next);
}

template<typename _Ty>
void DoublyLinkedList<_Ty>::print() const
{
    for (const auto& val : *this)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

template<typename _Ty>
void DoublyLinkedList<_Ty>::sort(const std::function<bool(const value_type&, const value_type&)>& pred)
{
    if (_size > 1)
    {
        std::vector<_Ty> values;
        for (const auto& val : *this)
            values.push_back(val);

        std::sort(values.begin(), values.end(), pred);

        clear();
        for (const auto& val : values)
            push_back(val);
    }
}

template<typename _Ty>
void DoublyLinkedList<_Ty>::unique()
{
    if (_size < 2) return;
    auto it = begin();
    auto next = it;
    ++next;

    while (next != end())
    {
        if (*it == *next)
        {
            erase(next);
        }
        else
        {
            ++it;
            ++next;
        }
    }
}

template<typename _Ty>
void DoublyLinkedList<_Ty>::merge(DoublyLinkedList& other)
{
    if (this == &other)
        return;

    Node* current = _head;
    Node* otherCurrent = other._head;

    while (current && otherCurrent)
    {
        if (current->_data <= otherCurrent->_data)
            current = current->_next;
        else
        {
            Node* nextOther = otherCurrent->_next;
            otherCurrent->_next = current;
            if (current)
                current->_prev = otherCurrent;
            if (current == _head)
                _head = otherCurrent;

            otherCurrent->_prev = current->_prev;

            if (current->_prev)
                current->_prev->_next = otherCurrent;
            current = otherCurrent;
            otherCurrent = nextOther;
        }
    }

    if (otherCurrent)
    {
        if (_tail)
            _tail->_next = otherCurrent;
        otherCurrent->_prev = _tail;
        _tail = other._tail;
    }
    _size += other.size();

    other._head = nullptr;
    other._tail = nullptr;
    other._size = 0;
}

template<typename _Ty>
void DoublyLinkedList<_Ty>::reverse()
{
    if (_size < 2) return;
    Node* temp = nullptr;
    Node* current = _head;

    while (current)
    {
        temp = current->_prev;
        current->_prev = current->_next;
        current->_next = temp;
        current = current->_prev;
    }

    std::swap(_head, _tail);
}

template<typename _Ty>
void DoublyLinkedList<_Ty>::splice(iterator pos, DoublyLinkedList& other)
{
    if (other.empty())
        return;

    if (pos.node)
    {
        other._head->_prev = pos.node->_prev;
        if (pos.node->_prev)
            pos.node->_prev->_next = other._head;
        else
            _head = other._head;

        pos.node->_prev = other._tail;
        other._tail->_next = pos.node;
    }
    else
    {
        if (_tail)
        {
            _tail->_next = other._head;
            other._head->_prev = _tail;
        }
        else
        {
            _head = other._head;
        }
        _tail = other._tail;
    }
    _size += other._size;
    other._head = other._tail = nullptr;
    other._size = 0;
}

template<typename _Ty>
bool DoublyLinkedList<_Ty>::operator==(const DoublyLinkedList& other) const
{
    if (_size != other._size) return false;
    auto it1 = begin();
    auto it2 = other.begin();
    while (it1 != end())
    {
        if (*it1 != *it2)
            return false;
        ++it1;
        ++it2;
    }
    return true;
}

template<typename _Ty>
bool DoublyLinkedList<_Ty>::operator!=(const DoublyLinkedList& other) const
{
    return !(*this == other);
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::Iterator DoublyLinkedList<_Ty>::Iterator::operator+(size_type n) const
{
    Iterator it = *this;
    while (n-- && it.node)
        ++it;
    return it;
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::Iterator DoublyLinkedList<_Ty>::Iterator::operator-(size_type n) const
{
    Iterator it = *this;
    while (n-- > 0 && it.node)
        --it;
    return it;
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::Iterator& DoublyLinkedList<_Ty>::Iterator::operator+=(size_type n)
{
    while (n-- > 0 && node)
        ++(*this);
    return *this;
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::Iterator& DoublyLinkedList<_Ty>::Iterator::operator-=(size_type n)
{
    while (n-- > 0 && node)
        --(*this);
    return *this;
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::ConstIterator DoublyLinkedList<_Ty>::ConstIterator::operator+(size_type n) const
{
    ConstIterator it = *this;
    while (n-- && it.node)
        ++it;
    return it;
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::ConstIterator DoublyLinkedList<_Ty>::ConstIterator::operator-(size_type n) const
{
    ConstIterator it = *this;
    while (n-- && it.node)
        --it;
    return it;
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::ConstIterator& DoublyLinkedList<_Ty>::ConstIterator::operator+=(size_type n)
{
    while (n-- && node)
        ++(*this);
    return *this;
}

template<typename _Ty>
typename DoublyLinkedList<_Ty>::ConstIterator& DoublyLinkedList<_Ty>::ConstIterator::operator-=(size_type n)
{
    while (n-- && node)
        --(*this);
    return *this;
}