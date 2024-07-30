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
    cache.Add("key3");
    cache.Increment("key1");
    
    EXPECT_EQ(cache.GetLeastUsed(), "key3");  

    cache.Increment("key2");

    EXPECT_EQ(cache.GetLeastUsed(), "key2");
}

TEST_F(StringLRUCacheTest, RemoveItem) {
    cache.Add("key1");
    cache.Add("key2");
    cache.Remove("key1");
    
    EXPECT_EQ(cache.GetLeastUsed(), "key2");
    
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
