#ifndef HEAP_H
#define HEAP_H
#include <vector>
#include <utility>
#include "map.h"
#include <string>
#include <type_traits>
//#include "map.h"
//#include "file.h"
#include <tuple>
template<typename V, typename K>
class Heap{
    std::vector<std::pair<V, K>> elements;
    HashMap<K, int>& index_map; 
public:
    Heap(HashMap<K, int>& index_map) : index_map(index_map) {}
    void insert(K value, V priority) {
        elements.push_back({priority, value});
        size_t idx = siftUp(elements.size() - 1);
        index_map.insert(value, static_cast<int>(idx));
    }
    void updateval(size_t idx, V newval) {
        if (idx >= elements.size()) return;
        elements[idx].first = newval;
        size_t nidx = siftUp(idx);
        index_map.insert(elements[nidx].second, static_cast<int>(nidx));
    }
    std::pair<V,K> extract() {
        std::pair<V,K> root = elements[0];
        elements[0] = elements.back();
        elements.pop_back();
        index_map.remove(root.second);
        if (!elements.empty()) {
            index_map.insert(elements[0].second, 0);
        }
        siftDown(0);
        return root;
    }
    bool isEmpty() const {
        return elements.empty();
    }
    // Custom comparator for pair<time_t, size_t> for RECENT_FILES
    bool isGreater(const std::pair<V, K>& a, const std::pair<V, K>& b) const {
        if constexpr (std::is_same<V, std::pair<time_t, size_t>>::value && std::is_pointer<K>::value) {
            if (a.first.first > b.first.first) return true;
            if (a.first.first < b.first.first) return false;
            // Tie-breaker: larger update_counter wins
            if (a.first.second > b.first.second) return true;
            if (a.first.second < b.first.second) return false;
            // Final tie-breaker: lex order of filename
            return a.second->getName() < b.second->getName();
        } else if constexpr (std::is_same<V, int>::value && std::is_pointer<K>::value) {
            if (a.first > b.first) return true;
            if (a.first < b.first) return false;
            return a.second->getName() < b.second->getName();
        } else {
            return a.first > b.first;
        }
    }
    size_t siftUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (isGreater(elements[index], elements[parent])) {
                std::swap(elements[index], elements[parent]);
                if (index_map.contains(elements[index].second))
                    index_map.remove(elements[index].second);
                if (index_map.contains(elements[parent].second))
                    index_map.remove(elements[parent].second);
                index_map.insert(elements[index].second, static_cast<int>(index));
                index_map.insert(elements[parent].second, static_cast<int>(parent));
                index = parent;
            } 
            else {
                return index;
            }
        }
        return index;
    }
    size_t siftDown(size_t index){
        size_t left = 2*index + 1;
        size_t right = 2*index + 2;
        size_t largest = index;
        if (left < elements.size() && isGreater(elements[left], elements[largest])) {
            largest = left;
        }
        if (right < elements.size() && isGreater(elements[right], elements[largest])) {
            largest = right;
        }
        if (largest != index) {
            std::swap(elements[index], elements[largest]);
            if (index_map.contains(elements[index].second))
                index_map.remove(elements[index].second);
            if (index_map.contains(elements[largest].second))
                index_map.remove(elements[largest].second);
            index_map.insert(elements[index].second, static_cast<int>(index));
            index_map.insert(elements[largest].second, static_cast<int>(largest));
            siftDown(largest);
        }
        return index;
    }
    void printall() const {
        for (const auto& element : elements) {
            std::cout << "Value: " << element.second << ", Priority: " << element.first << std::endl;
        }
    }
    V get(size_t idx){
        if (idx >= elements.size()) return K();
        return elements[idx].first;
    }
};
#endif