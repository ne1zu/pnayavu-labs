#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>

template <typename T>
class Collection {
private:
    std::vector<std::shared_ptr<T>> items;

    bool isValidIndex(int index) const;

public:
    typedef typename std::vector<std::shared_ptr<T>>::const_iterator ConstIterator;

    Collection() = default;

    bool add(std::shared_ptr<T> item);
    bool removeAt(int index);
    bool remove(const T& value);

    template <typename Predicate>
    int removeIf(Predicate condition);

    void clear();

    std::shared_ptr<T> get(int index) const;
    std::shared_ptr<T> at(int index) const;

    int indexOf(const T& value) const;
    bool contains(const T& value) const;
    std::shared_ptr<T> find(const T& value) const;

    int getCount() const;
    bool isEmpty() const;

    void print(std::ostream& os = std::cout, const std::string& indent = "") const;

    ConstIterator begin() const;
    ConstIterator end() const;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Collection<T>& collection);

template <typename T, typename Predicate>
std::shared_ptr<T> findFirst(const Collection<T>& collection, Predicate condition);

template <typename T, typename Predicate>
Collection<T> findAll(const Collection<T>& collection, Predicate condition);

template <typename T, typename Predicate>
int countIf(const Collection<T>& collection, Predicate condition);

template <typename T>
std::shared_ptr<T> findMax(const Collection<T>& collection);

#include "collection.tpp"
