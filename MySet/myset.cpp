#ifndef MYSET_CPP
#define MYSET_CPP
#include "AVL_tree.cpp"
namespace xfz {
	template<typename T>
	class myset {
	private:
		AVL_tree<T> _tree;
		typedef typename T						data_type;
		typedef typename AVL_tree<T>::iterator	iterator;
	public:
		int size()const { return _tree.size(); }
		bool empty()const { return _tree.empty(); }
		iterator begin()const { return _tree.begin(); }
		iterator end()const { return _tree.end(); }
		void insert(T __data) { _tree.insert(__data); }
		void erase(iterator it) { _tree.erase(it); }
	};
}
#endif