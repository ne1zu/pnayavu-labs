#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>

template <typename T>
class Collection {
private:
    std::vector<std::shared_ptr<T>> items;

public:
    Collection() = default;

    bool add(std::shared_ptr<T> item) {
        if (!item) return false;
        items.push_back(item);
        return true;
    }

    bool removeAt(int index) {
        if (index < 0 || index >= static_cast<int>(items.size())) return false;
        items.erase(items.begin() + index);
        return true;
    }

    bool remove(const T& value) {
        return removeAt(indexOf(value));
    }

    template <typename Predicate>
    int removeIf(Predicate condition) {
        int removed = 0;
        for (auto it = items.begin(); it != items.end(); ) {
            if (condition(**it)) {
                it = items.erase(it);
                removed++;
            }
            else {
                ++it;
            }
        }
        return removed;
    }

    void clear() { items.clear(); }

    std::shared_ptr<T> get(int index) const {
        if (index < 0 || index >= static_cast<int>(items.size())) return nullptr;
        return items[index];
    }

    std::shared_ptr<T> at(int index) const {
        return items.at(index);
    }

    int indexOf(const T& value) const {
        int count = static_cast<int>(items.size());
        for (int i = 0; i < count; ++i) {
            if (*items[i] == value) return i;
        }
        return -1;
    }

    bool contains(const T& value) const {
        return indexOf(value) >= 0;
    }

    std::shared_ptr<T> find(const T& value) const {
        return get(indexOf(value));
    }

    int getCount() const {
        return static_cast<int>(items.size());
    }

    bool isEmpty() const {
        return items.empty();
    }

    void print(std::ostream& os = std::cout, const std::string& indent = "") const {
        if (items.empty()) {
            os << indent << "Empty\n";
            return;
        }
        int count = static_cast<int>(items.size());
        for (int i = 0; i < count; ++i)
            os << indent << i + 1 << ". " << *items[i] << "\n";
    }

    typename std::vector<std::shared_ptr<T>>::const_iterator begin() const {
        return items.begin();
    }

    typename std::vector<std::shared_ptr<T>>::const_iterator end() const {
        return items.end();
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Collection<T>& collection) {
    collection.print(os);
    return os;
}

template <typename T, typename Predicate>
std::shared_ptr<T> findFirst(const Collection<T>& collection, Predicate condition) {
    for (const auto& item : collection) {
        if (condition(*item)) return item;
    }
    return nullptr;
}

template <typename T, typename Predicate>
Collection<T> findAll(const Collection<T>& collection, Predicate condition) {
    Collection<T> result;
    for (const auto& item : collection) {
        if (condition(*item)) result.add(item);
    }
    return result;
}

template <typename T, typename Predicate>
int countIf(const Collection<T>& collection, Predicate condition) {
    int total = 0;
    for (const auto& item : collection) {
        if (condition(*item)) total++;
    }
    return total;
}

template <typename T>
std::shared_ptr<T> findMax(const Collection<T>& collection) {
    if (collection.isEmpty()) return nullptr;
    std::shared_ptr<T> best = collection.get(0);
    for (const auto& item : collection) {
        if (*item > *best) best = item;
    }
    return best;
}