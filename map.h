#ifndef MAP_H
#define MAP_H
// implement hashmap, which provides fast, O(1) average-time lookups
#include <iostream>
#include <stdexcept>
#include "tree.h"
 // Add this line to include the definition of File
template <typename K, typename V>
struct HashNode {
    K key;
    V value;
    HashNode* next;
    HashNode(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
};
class File;
template <typename K, typename V>
class HashMap {
private:
    size_t capacity;
    size_t size;
    float load_factor;
    void rehash(){
        size_t new_capacity = capacity * 2;
        std::vector<HashNode<K, V>*> new_table(new_capacity, nullptr);

        for (const auto& head : table) {
            HashNode<K, V>* current = head;
            while (current) {
                size_t new_index = std::hash<K>()(current->key) % new_capacity;
                HashNode<K, V>* next = current->next;
                current->next = new_table[new_index];
                new_table[new_index] = current;
                current = next;
            }
        }

        table = std::move(new_table);
        capacity = new_capacity;
    }

public:
    std::vector<HashNode<K, V>*> table;
    HashMap(size_t initial_capacity = 16, float load_factor = 0.75)
        : capacity(initial_capacity), size(0), load_factor(load_factor) {
        table.resize(capacity, nullptr);
    }

    ~HashMap() {
        for (auto& head : table) {
            while (head) {
                HashNode<K, V>* temp = head;
                head = head->next;
                delete temp;
            }
        }
    }
    int hash(const K& key) const {
        (void)key; // Silence unused parameter warning
        return 0; // Placeholder, is specialized below, and in main.cpp
    }
    auto begin() {
        return table.begin();
    }
    auto end() {
        return table.end();
    }
    void insert(const K& key, const V& value){
        size_t index = hash(key);
        HashNode<K, V>* new_node = new HashNode<K, V>(key, value);
        new_node->next = table[index];
        table[index] = new_node;
        size++;
        if (static_cast<float>(size) / capacity > load_factor) {
            rehash();
        }
    }
    size_t getSize() const {
        return size;
    }
    V get(const K& key) const{
        size_t index = hash(key);
        HashNode<K, V>* current = table[index];
        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
       // throw std::runtime_error("Key not found");
        std::cout << "Key not found: " << key << std::endl;
        return V();
    }
    void remove(const K& key){
        size_t index = hash(key);
        HashNode<K, V>* current = table[index];
        HashNode<K, V>* prev = nullptr;
        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    table[index] = current->next;
                }
                delete current;
                size--;
                return;
            }
            prev = current;
            current = current->next;
        }
    }
    bool contains(const K& key) const {
        size_t index = hash(key);
        HashNode<K, V>* current = table[index];
        while (current) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};
template <>
int HashMap<std::string, File*>::hash(const std::string& key) const;

template <>
int HashMap<int, TreeNode*>::hash(const int& key) const;

template <>
int HashMap<File*, int>::hash(File* const& key) const;

// template specialization for File* hash function
/*template <>
int HashMap<File*, int>::hash(File* const& key) const {
    std::string filename = key->getName();
    // Use the string hash specialization for filename
    int hash = 0;
    for (char c : filename) {
        hash = (hash * 31 + (c-'a') + 17) % capacity;
    }
    return hash;
}*/
#endif