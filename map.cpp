#include "map.h"
#include "file.h"
#include "tree.h"

// Template specialization for std::string hash function
template <>
int HashMap<std::string, File*>::hash(const std::string& key) const {
    int hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c + 17) % capacity;
    }
    return hash;
}

template <>
int HashMap<int, TreeNode*>::hash(const int& key) const {
    int hash = 0;
    int temp = key;
    while (temp){
        int rem = temp % 10;
        hash = (hash * 31 + rem) % capacity;
        temp /= 10;
    }
    return hash;
}

template <>
int HashMap<File*, int>::hash(File* const& key) const {
    std::string filename = key->getName();
    int hash = 0;
    for (char c : filename) {
        hash = (hash * 31 + c + 17) % capacity;
    }
    return hash;
}

// Explicit instantiation for the linker

template class HashMap<std::string, File*>;
template class HashMap<int, TreeNode*>;
template class HashMap<File*, int>;
