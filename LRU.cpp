#include <iostream>
#include <list>
#include <unordered_map>
#include <string>
#include <iterator>

using namespace std;

class Statistics
{
private:
  int keys, hits, misses;

public:
  Statistics()
  {
    keys = 0;
    hits = 0;
    misses = 0;
  }

  template <typename K, typename V>
  friend class LruCache;
};

template <typename K, typename V>
class LruCache
{
private:
  int capacity;
  list<K> storage;
  unordered_map<K, pair<V, typename list<K>::iterator>> searcher;
  Statistics stats;

  struct StatisticsDummy
  {
    int keys, hits, misses;
    StatisticsDummy(int keys, int hits, int misses) : keys{keys}, hits{hits}, misses{misses}
    {
    }
  };

  pair<typename list<K>::iterator, V> getPositionAndValue(const K key)
  {
    auto item = searcher.find(key);
    return {item->second.second, item->second.first};
  }

  /* data */
public:
  LruCache(int capacity) : capacity(capacity)
  {
    this->capacity = capacity;
  }

  bool isHere(K key)
  {
    if (searcher.find(key) != searcher.end())
    {
      stats.hits++;
      return true;
    }
    stats.misses++;

    return false;
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

  void evict()
  {
    if (!storage.size())
      return;
    cout << storage.size() << " s " << endl;
    cout << storage.back() << " back " << endl;
    searcher.erase(storage.back());
    storage.pop_back();
    cout << storage.front() << " f" << endl;
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
    {
      update(key, value);
      return;
    }
    else
    {
      if (isFull())
        evict();

      storage.emplace_front(key);
      searcher[key] = {value, storage.begin()};
      stats.keys++;
    }
  }

  pair<K, V> peak()
  {
    const K key = storage.front();
    const V value = searcher[key].first;
    stats.hits++;

    return {key, value};
  }

  pair<K, V> getEntry(const K key)
  {
    return {key, getPositionAndValue(key).second};
  }

  void display()
  {
    for (const auto [k, v] : searcher)
    {
      cout << k << endl;
    }
  }

  StatisticsDummy getStats()
  {
    return StatisticsDummy(stats.keys, stats.hits, stats.misses);
  }
};

int main()
{
  LruCache<int, int> cache2(2);
  cache2.put(1, 2);
  cache2.put(3, 10);
  cache2.get(1);
  // if (ptr)
  //   cout << *ptr << endl;

  // cache2.display();

  // cache2.evict();

  // cache2.display();
  // //   cache2.evict();

  auto cache2Stats = cache2.getStats();
  cout << cache2Stats.keys << ' ' << cache2Stats.hits << ' ' << cache2Stats.misses << endl;
  cout << sizeof(cache2Stats) << endl;
  return 0;
}
