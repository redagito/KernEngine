#pragma once

#include <unordered_map>
#include <functional>

template <typename Type, typename Key> class TFactory
{
public:
  using Creator = std::function<Type *()>;

  Type *create(const Key &key) const;

  bool hasCreator(const Key &key) const;
  void addCreator(const Key &key, const Creator &creator);

  void removeCreator(const Key &key);

private:
  std::unordered_map<Key, Creator> m_creators;
};

template <typename Type, typename Key>
Type *TFactory<Type, Key>::create(const Key &key) const
{
  auto creator = m_creators.find(key);
  if (creator == m_creators.end())
  {
    return nullptr;
  }
  return creator->second();
}

template <typename Type, typename Key>
bool TFactory<Type, Key>::hasCreator(const Key &key) const
{
  auto creator = m_creators.find(key);
  return creator != m_creators.end();
}

template <typename Type, typename Key>
void TFactory<Type, Key>::addCreator(const Key &key, const Creator &creator)
{
  m_creators[key] = creator;
}

template <typename Type, typename Key>
void TFactory<Type, Key>::removeCreator(const Key &key)
{
  m_creators.erase(key);
}
