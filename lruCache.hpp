#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <stdexcept>

template<typename KeyType>
class LRUCache {
public:
    explicit LRUCache() {}

    // Increment usage counter of a key
    void Increment(const KeyType& key) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = cacheMap.find(key);
        if (it == cacheMap.end()) {
            throw std::runtime_error("Key not found");
        }
        
        // Move the accessed item to the front of the list
        auto listIt = it->second.listIterator;
        list.splice(list.begin(), list, listIt);
        
        // Increment the usage count
        it->second.usageCount++;
    }

    // Add a key to the cache
    void Add(const KeyType& key) {
        std::lock_guard<std::mutex> lock(mutex);
        
        list.push_back(key);
        cacheMap[key] = { 0, list.begin() }; // Initial usage count is 0
    }

    // Remove a key from the cache
    void Remove(const KeyType& key) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = cacheMap.find(key);
        if (it == cacheMap.end()) {
            throw std::runtime_error("Key not found");
        }
        
        list.erase(it->second.listIterator);
        cacheMap.erase(it);
    }

    // Get the least used key
    KeyType GetLeastUsed() const {
        std::lock_guard<std::mutex> lock(mutex);
        if (list.empty()) {
            throw std::runtime_error("Cache is empty");
        }
        return list.back();
    }

private:
    struct CacheItem {
        int usageCount;
        typename std::list<KeyType>::iterator listIterator;
    };

    std::list<KeyType> list; // List to maintain LRU order
    std::unordered_map<KeyType, CacheItem> cacheMap; // Map to store key information

    // Used in a constant method GetLeastUsed()
    mutable std::mutex mutex;
};

