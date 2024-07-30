#include <gtest/gtest.h>
#include "../lruCache.hpp"  // Include your LRUCache implementation

// Test suite for LRUCache
template<typename KeyType>
class LRUCacheTest : public ::testing::Test {
protected:
    LRUCache<KeyType> cache;
};

// Test case for adding and retrieving items
TEST_F(LRUCacheTest<std::string>, AddAndRetrieveItem) {
    cache.Add("key1");
    EXPECT_EQ(cache.GetLeastUsed(), "key1");
}

// Test case for incrementing usage and getting the least used item
TEST_F(LRUCacheTest<std::string>, IncrementUsage) {
    cache.Add("key1");
    cache.Add("key2");
    cache.Increment("key1");
    
    // After incrementing, "key1" should be the most recently used
    EXPECT_EQ(cache.GetLeastUsed(), "key2");
    
    // Increment key2 now
    cache.Increment("key2");
    
    // After incrementing key2, it should be the most recently used
    EXPECT_EQ(cache.GetLeastUsed(), "key1");
}

// Test case for removing an item
TEST_F(LRUCacheTest<std::string>, RemoveItem) {
    cache.Add("key1");
    cache.Add("key2");
    cache.Remove("key1");
    
    // After removing "key1", "key2" should be the only item left
    EXPECT_EQ(cache.GetLeastUsed(), "key2");
    
    // Removing "key2" should throw an exception when trying to get the least used item
    cache.Remove("key2");
    EXPECT_THROW(cache.GetLeastUsed(), std::runtime_error);
}

// Test case for handling non-existent items
TEST_F(LRUCacheTest<std::string>, IncrementNonExistentItem) {
    cache.Add("key1");
    EXPECT_THROW(cache.Increment("key2"), std::runtime_error);
}

TEST_F(LRUCacheTest<std::string>, RemoveNonExistentItem) {
    cache.Add("key1");
    EXPECT_THROW(cache.Remove("key2"), std::runtime_error);
}

// Test case for empty cache
TEST_F(LRUCacheTest<std::string>, EmptyCache) {
    EXPECT_THROW(cache.GetLeastUsed(), std::runtime_error);
}

