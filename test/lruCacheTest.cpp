#include <gtest/gtest.h>
#include "../lruCache.hpp"

using namespace std;

template<typename KeyType>
class LRUCacheTest : public ::testing::Test {
protected:
    LRUCache<KeyType> cache;
};

// Explicit instantiation for std::string
using StringLRUCacheTest = LRUCacheTest<std::string>;

TEST_F(StringLRUCacheTest, AddAndRetrieveItem) {
    cache.Add("key1");
    EXPECT_EQ(cache.GetLeastUsed(), "key1");
}

TEST_F(StringLRUCacheTest, IncrementUsage) {
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

TEST_F(StringLRUCacheTest, RemoveItem) {
    cache.Add("key1");
    cache.Add("key2");
    cache.Remove("key1");
    
    // After removing "key1", "key2" should be the only item left
    EXPECT_EQ(cache.GetLeastUsed(), "key2");
    
    // Removing "key2" should throw an exception when trying to get the least used item
    cache.Remove("key2");
    EXPECT_THROW(cache.GetLeastUsed(), std::runtime_error);
}

TEST_F(StringLRUCacheTest, IncrementNonExistentItem) {
    cache.Add("key1");
    EXPECT_THROW(cache.Increment("key2"), std::runtime_error);
}

TEST_F(StringLRUCacheTest, RemoveNonExistentItem) {
    cache.Add("key1");
    EXPECT_THROW(cache.Remove("key2"), std::runtime_error);
}

TEST_F(StringLRUCacheTest, EmptyCache) {
    EXPECT_THROW(cache.GetLeastUsed(), std::runtime_error);
}
