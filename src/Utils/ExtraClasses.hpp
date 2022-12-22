#pragma once

#include <cstdint>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace pokesim::internal {
template <typename Key>
class USet : public std::unordered_set<Key> {
 public:
  bool has(Key key) const { return std::unordered_set<Key>::find(key) != std::unordered_set<Key>::end(); }

  template <typename List>
  auto insert(const List& other) {
    return std::unordered_set<Key>::insert(other.begin(), other.end());
  }

  auto insert(Key key) { return std::unordered_set<Key>::insert(key); }
  auto insert(std::initializer_list<Key> list) { return std::unordered_set<Key>::insert(list.begin(), list.end()); }
  bool notEmpty() const { return std::unordered_set<Key>::size(); }

  USet() {}
  USet(Key key) { insert(key); }
  USet(std::initializer_list<Key> list) : std::unordered_set<Key>(list) {}

  template <typename List>
  USet(const List& vec) : std::unordered_set<Key>(vec.begin(), vec.end()) {}

  template <class InputIterator>
  USet(InputIterator first, InputIterator last) : std::unordered_set<Key>(first, last) {}

  USet<Key>& operator+=(const USet<Key>& other) { return insert(other); }

  USet<Key> intersection(const USet<Key>& other) const {
    USet<Key> copy(*this);
    for (const auto& key : other) {
      if (has(key)) copy.erase(key);
      else copy.insert(key);
    }
    return copy;
  }

  operator std::vector<Key>() const {
    return std::vector<Key>(std::unordered_set<Key>::begin(), std::unordered_set<Key>::end());
  }
};

template <typename Key, typename Val>
class UMap : public std::unordered_map<Key, Val> {
 public:
  bool has(const Key key) const {
    return std::unordered_map<Key, Val>::find(key) != std::unordered_map<Key, Val>::end();
  }

  auto insert(UMap<Key, Val>& other) { return std::unordered_map<Key, Val>::insert(other.begin(), other.end()); }
  auto insert(UMap<Key, Val> other) { return std::unordered_map<Key, Val>::insert(other.begin(), other.end()); }
  auto insert(const Key& key, Val val) { return insertPair({key, val}); }
  auto insertPair(std::pair<const Key, Val> val) { return std::unordered_map<Key, Val>::insert(val); }

  bool notEmpty() const { return std::unordered_map<Key, Val>::size(); }

  template <typename Key2>
  bool doubleHas(const Key key1, const Key2 key2) const {
    return has(key1) && std::unordered_map<Key, Val>::at(key1).has(key2);
  }

  bool setIfNew(const Key key, const Val val) {
    if (!has(key)) {
      insert(key, val);
      return true;
    }
    return false;
  }

  void combine(const UMap<Key, Val>& other) {
    for (auto& change : other) {
      if (has(change.first)) {
        std::unordered_map<Key, Val>::at(change.first) += change.second;
      }
      else {
        insert(change);
      }
    }
  }

  UMap<Key, Val>& operator+=(const UMap<Key, Val>& other) {
    combine(other);
    return *this;
  }

  USet<Key> keys() const {
    USet<Key> allKeys;
    for (const auto& entry : *this) allKeys.insert(entry.first);
    return allKeys;
  }

  Val& operator[](Key key) { return std::unordered_map<Key, Val>::operator[](key); }
  Val operator[](const Key key) const { return has(key) ? std::unordered_map<Key, Val>::at(key) : Val(); }

  UMap() {}
  UMap(std::pair<const Key, Val> list) : std::unordered_map<Key, Val>({list}) {}
  UMap(std::initializer_list<std::pair<const Key, Val>> list) : std::unordered_map<Key, Val>(list) {}

  template <typename KeyList, typename ValList>
  UMap(const KeyList& keyList, const ValList& valList) {
    static_assert(keyList.size() == valList.size());
    for (size_t i = 0; i < keyList.size(); i++) {
      insert(keyList[i], valList[i]);
    }
  }

  template <typename KeyList>
  UMap(const KeyList& keyList) {
    for (size_t i = 0; i < keyList.size(); i++) {
      insert(keyList[i], static_cast<Val>(i));
    }
  }

  template <class InputIterator>
  UMap(InputIterator first, InputIterator last) : std::unordered_map<Key, Val>(first, last) {}
};

template <typename T>
struct UqPtr : public std::unique_ptr<T> {
  UqPtr() : std::unique_ptr<T>(nullptr) {}
  UqPtr(std::nullptr_t) : std::unique_ptr<T>(nullptr) {}
  UqPtr(T* ptr) : std::unique_ptr<T>(ptr) {}

  UqPtr(const UqPtr<T>& other) : std::unique_ptr<T>(other) {}
  UqPtr(UqPtr<T>&& other) noexcept : std::unique_ptr<T>(move(other)) {}

  template <typename Y>
  UqPtr(const UqPtr<Y>& other) : std::unique_ptr<T>(other) {}

  template <typename Y>
  UqPtr(UqPtr<Y>&& other) : std::unique_ptr<T>(move(other)) {}

  UqPtr(const std::unique_ptr<T>& other) : std::unique_ptr<T>(other) {}
  UqPtr(std::unique_ptr<T>&& other) noexcept : std::unique_ptr<T>(move(other)) {}

  template <typename Y>
  void reset(Y* ptr) {
    std::unique_ptr<T>::reset(ptr);
  }

  // Overload without custom deleter: std::unique_ptr<string>(...);
  template <typename Y>
  void reset(const std::false_type&, Y* ptr) {
    std::unique_ptr<T>::reset(ptr);
  }

  template <class Y, class Deleter>
  void reset(Y* ptr, Deleter deleter) {
    std::unique_ptr<T>::reset(ptr, deleter);
  }

  void reset() { std::unique_ptr<T>::reset(); }

  UqPtr& operator=(const UqPtr& other) {
    std::unique_ptr<T>::operator=(other);
    return *this;
  }
  template <typename Y>
  UqPtr& operator=(const UqPtr<Y>& other) {
    std::unique_ptr<T>::operator=(other);
    return *this;
  }

  T* operator->() const { return std::unique_ptr<T>::get(); }
  operator T*() const { return std::unique_ptr<T>::get(); }
  typename std::add_lvalue_reference<T>::type operator*() const { return *std::unique_ptr<T>::get(); }

  void release() { std::unique_ptr<T>::reset(); }
  bool empty() const { return std::unique_ptr<T>::get() == nullptr; }

  template <typename Y>
  Y* staticCast() const {
    return static_cast<Y*>(std::unique_ptr<T>::get());
  }

  template <typename Y>
  Y* dynamicCast() const {
    return dynamic_cast<Y*>(std::unique_ptr<T>::get());
  }
};

template <typename T>
struct ShPtr {
  std::shared_ptr<T>* data = nullptr;

  ShPtr() : data(new std::shared_ptr<T>(nullptr)) {}
  ShPtr(std::nullptr_t) : data(new std::shared_ptr<T>(nullptr)) {}
  ShPtr(T* ptr) : data(new std::shared_ptr<T>(ptr)) {}

  template <typename... Args>
  ShPtr(Args... args) : ShPtr(new T(args...)) {}

  template <typename D>
  ShPtr(std::nullptr_t, D deleter) : data(new std::shared_ptr<T>(nullptr, deleter)) {}

  template <typename Y>
  ShPtr(const ShPtr<Y>& other, T* ptr) : data(new std::shared_ptr<T>(*other.data, ptr)) {}

  ShPtr(const ShPtr<T>& other) : data(new std::shared_ptr<T>(*other.data)) {}
  ShPtr(ShPtr<T>&& other) noexcept : data(new std::shared_ptr<T>(move(*other.data))) {}

  template <typename Y>
  ShPtr(const ShPtr<Y>& other) : data(new std::shared_ptr<T>(*other.data)) {}

  template <typename Y>
  ShPtr(ShPtr<Y>&& other) : data(new std::shared_ptr<T>(move(*other.data))) {}

  ShPtr(const std::shared_ptr<T>& other) : data(new std::shared_ptr<T>(*other.data)) {}
  ShPtr(std::shared_ptr<T>&& other) : data(new std::shared_ptr<T>(move(*other.data))) {}

  // Overload without custom deleter: ShPtr<string>(...);
  template <typename Y>
  ShPtr(const std::false_type&, Y* ptr) : data(new std::shared_ptr<T>(ptr)) {}

  ~ShPtr() { delete data; }

  template <typename Y>
  void reset(Y* ptr) {
    data->reset(ptr);
  }

  // Overload without custom deleter: std::shared_ptr<string>(...);
  template <typename Y>
  void reset(const std::false_type&, Y* ptr) {
    data->reset(ptr);
  }

  template <class Y, class Deleter>
  void reset(Y* ptr, Deleter deleter) {
    data->reset(ptr, deleter);
  }

  void reset() { data->reset(); }

  T* get() const noexcept { return data->get(); }
  int64_t use_count() const noexcept { return data->use_count(); }
  template <class Y>
  bool owner_before(const ShPtr<Y>& other) const noexcept {
    return data->owner_before(*other.data);
  }

  ShPtr& operator=(const ShPtr& other) {
    if (this == &other) return *this;
    data->operator=(*other.data);
    return *this;
  }

  template <typename Y>
  ShPtr& operator=(const ShPtr<Y>& other) {
    if (this == &other) return *this;
    data->operator=(*other.data);
    return *this;
  }

  T* operator->() const { return data->get(); }
  operator T*() const { return data->get(); }
  typename std::add_lvalue_reference<T>::type operator*() const { return *data->get(); }

  void release() { data->reset(); }
  bool empty() const { return data->get() == nullptr; }

  template <typename Y>
  Y* staticCast() const {
    return static_cast<Y*>(data->get());
  }

  template <typename Y>
  Y* dynamicCast() const {
    return dynamic_cast<Y*>(data->get());
  }
};
}  // namespace pokesim::internal
