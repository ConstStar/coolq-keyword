#include "iniTemplate.h"
template<class T>
inline void INIvector<T>::add(T &t) { list.push_back(t); }

template<class T>
inline bool INIvector<T>::empty() { return list.empty(); }

template<class T>
inline int INIvector<T>::has(std::string key)
{
	for (T t : list)
		if (equal(t, key))
			return true;
	return false;
}

template<class T>
inline T & INIvector<T>::get(std::string key, int index)
{
	int i = 0;
	for (auto t = list.begin(); t != list.end(); t++)
		if (equal(*t, key))
			if (i == index)
				return *t;
			else
				i++;
	if (index)
		return nullptr;
	list.push_back(New(key));
	return list.back();
}

template<class T>
inline T & INIvector<T>::operator[](std::string key) { return get(key, 0); }

template<class T>
inline void INIvector<T>::del(std::string key, int index)
{
	int i = 0;
	for (auto b = begin(); b != end(); b++)
		if (equal(*b, key))
			if (index == i) {
				list.erase(b);
				return;
			}
			else
				i++;
}

template<class T>
inline decltype(INIvector<T>::list.begin()) INIvector<T>::begin()
{
	return list.begin();
}

template<class T>
inline decltype(INIvector<T>::list.end()) INIvector<T>::end()
{
	return list.end();
}
