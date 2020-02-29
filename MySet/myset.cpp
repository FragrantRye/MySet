#ifndef MYSET_CPP
#define MYSET_CPP
#include "AVL_tree.cpp"
namespace xfz {
	template<typename T>
	class myset {
	private:
		AVL_tree<T> _tree;
		typedef T						data_type;
		typedef typename AVL_tree<T>::iterator	iterator;
	public:
		myset() {}
		myset(const std::initializer_list<T>& list) { for (auto& i : list) _tree.insert(i); }
		int size()const { return _tree.size(); }
		bool empty()const { return _tree.empty(); }
		iterator begin()const { return _tree.begin(); }
		iterator end()const { return _tree.end(); }
		iterator rbegin()const { return _tree.rbegin(); }
		iterator rend()const { return _tree.rend(); }
		const iterator cbegin()const { return _tree.cbegin(); }
		const iterator cend()const { return _tree.cend(); }
		const iterator crbegin()const { return _tree.crbegin(); }
		const iterator crend()const { return _tree.crend(); }
		iterator find(const T& val)const { return _tree.find(val); }
		void clear() { _tree.clear(); }
		void insert(const T& data) { _tree.insert(data); }
		void erase(iterator it) { _tree.erase(it); }
		bool test_height() { return _tree.test_height(); }
	};
}
#endif