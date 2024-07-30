#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <stdexcept>

template <typename KeyType>
class LRUCache
{
public:
    explicit LRUCache() {}

    // Increment usage counter of a key
    void Increment(const KeyType &key)
    {
        std::lock_guard<std::mutex> lock(mutex);

        auto it = cacheMap.find(key);
        if (it == cacheMap.end())
        {
            throw std::runtime_error("Key not found");
        }

        // Move the accessed item to the front of the list
        auto listIt = it->second.listIterator;
        list.splice(list.begin(), list, listIt);

        // Update the iterator in cacheMap
        it->second.listIterator = list.begin();

        // Increment the usage count
        it->second.usageCount++;
    }

    // Add a key to the cache
    void Add(const KeyType &key)
    {
        std::lock_guard<std::mutex> lock(mutex);

        // Check if the key already exists
        auto it = cacheMap.find(key);
        if (it != cacheMap.end())
        {
            throw std::runtime_error("Key already exists");
        }

        // Add the new key to the end of the list
        auto listIt = list.insert(list.end(), key);

        // Store the iterator returned by insert in the cacheMap, initial usage count is 0
        cacheMap[key] = {0, listIt};
    }

    // Remove a key from the cache
    void Remove(const KeyType &key)
    {
        std::lock_guard<std::mutex> lock(mutex);

        auto it = cacheMap.find(key);
        if (it == cacheMap.end())
        {
            throw std::runtime_error("Key not found");
        }

        auto listIt = it->second.listIterator;

        // Ensure the iterator is valid
        if (listIt == list.end())
        {
            throw std::runtime_error("Invalid iterator");
        }

        // Remove the element from the list and map
        list.erase(listIt);
        cacheMap.erase(it);
    }

    // Get the least used key
    KeyType GetLeastUsed() const
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (list.empty())
        {
            throw std::runtime_error("Cache is empty");
        }
        return list.back();
    }

private:
    void PrintKeys() const
    {
        std::cout << "Keys in list: ";
        for (const auto &key : list)
        {
            auto it = cacheMap.find(key);

            std::cout << key << " " << it->second.usageCount << ", ";
        }
        std::cout << std::endl;
    }

    struct CacheItem
    {
        int usageCount;
        typename std::list<KeyType>::iterator listIterator;
    };

    std::list<KeyType> list;                         // List to maintain LRU order
    std::unordered_map<KeyType, CacheItem> cacheMap; // Map to store key information

    // Used in a constant method GetLeastUsed() thus mutable
    mutable std::mutex mutex;
};
