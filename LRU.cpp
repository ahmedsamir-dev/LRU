#include <iostream>
#include <list>
#include <unordered_map>
#include <string>
#include <iterator>

using namespace std;

template <typename K, typename V>
class LruCache
{
private:
  int capacity;
  list<K> storage;
  unordered_map<K, pair<V, typename list<K>::iterator>> searcher;

  bool isHere(K key)
  {
    return searcher.find(key) != searcher.end();
  }

  pair<typename list<K>::iterator, V> getPositionAndValue(const K key)
  {
    auto item = searcher.find(key);

    return {item->second.second, item->second.first};
  }

  void deleteLeastVisited()
  {
    searcher.erase(storage.back());
    storage.pop_back();
  }

  /* data */
public:
  LruCache(int capacity) : capacity(capacity)
  {
    this->capacity = capacity;
  }

  bool isFull()
  {
    return storage.size() == capacity;
  }

  void update(const K key, const V value)
  {
    storage.erase(getPositionAndValue(key).first);
    storage.emplace_front(key);
    searcher[key] = {value, storage.begin()};
  }

  V *get(K key)
  {
    if (isHere(key))
    {
      V value = getPositionAndValue(key).second;
      update(key, value);
      V *ptr;
      *ptr = value;
      return ptr;
    }

    return nullptr;
  }

  void put(const K key, const V value)
  {
    if (isHere(key))
      update(key, value);
    else
    {
      if (isFull())
        deleteLeastVisited();

      storage.emplace_front(key);
      searcher[key] = {value, storage.begin()};
    }
  }

  pair<K, V> mostRecentlyUsed()
  {
    const K key = storage.front();
    const V value = searcher[key].first;
    return {key, value};
  }

  pair<K, V> getEntry(const K key)
  {
    return {key, getPositionAndValue(key).second};
  }
};

int main()
{
  LruCache<string, float> cache(3);

  cache.put("hello", 89.888);
  auto it = cache.get("hello");
  if (it)
    cout << *it << endl;

  LruCache<int, int> cache2(2);
  cache2.put(1, 2);
  cache2.put(3, 10);
  int *ptr = cache2.get(3);
  if (ptr)
    cout << *ptr << endl;

  return 0;
}
