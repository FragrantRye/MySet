# MySet
A set based on AVL tree. Implemented most of the interfaces of std::set.

## Usage
### basic insert & erase:
```C++
#include<myset.cpp>
#include<iostream>
int main(){
    xfz::set<int> myset = {1,6,3,5};
    myset.insert(2);
    auto it = myset.find(1);
    std::cout<<*it<<endl;
    myset.erase(it);
    for(int i:myset)
        std::cout<<i<<' ';
    return 0;
}
```
output:
```
1
2 3 5 6
```
## All interface
```C++
template<typename T>
class myset {
    myset() {}
    myset(const std::initializer_list<T>& list);
    int size()const;
    bool empty()const;
    iterator begin()const;
    iterator end()const;
    iterator rbegin()const;
    iterator rend()const;
    const iterator cbegin()const;
    const iterator cend()const;
    const iterator crbegin()const;
    const iterator crend()const;
    iterator find(const T& val)const;
    void clear();
    void insert(const T& data);
    void erase(iterator it);
    bool test_height(); //For bug location only
};
```

## Benchmark
Use random int between 0 and 10,000,000

Time cost(ms):
|   |xfz::set|std::set|Optimization ratio|
|---|---|---|---|
|Insert|17109|15534|+10.1%|
|Find|17396|20163|-13.7%|
|Travel|18479|10710|+72.5%|
|Erase|20278|27329|-25.8%|

Test CPU: Intel i7-9750H @3.2GHz
### Conclution
In the scenario of frequent query, less insertion and traversal, the std::set is better than std::set.
