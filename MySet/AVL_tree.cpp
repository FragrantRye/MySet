#ifndef AVL_TREE_CPP
#define AVL_TREE_CPP
#include <iterator>
#include <memory>
#include <cassert>

namespace xfz {
	template<typename T>
	struct AVL_tree_node {
		typedef  AVL_tree_node<T>* node_ptr;
		typedef  T                  data_type;
		typedef  T* pointer;

		node_ptr parent;
		node_ptr lchild;
		node_ptr rchild;

		int      height;
		T        data;

		bool update_height() {
			int a = lchild ? lchild->height : 0;
			int b = rchild ? rchild->height : 0;
			a = 1 + (a < b ? b : a);
			if (height == a)
				return false;
			height = a;
			return true;
		}

		int deg()const {
			int a = lchild ? lchild->height : 0;
			int b = rchild ? rchild->height : 0;
			return a - b;
		}
	};

	template<typename Item>
	struct AVL_treeIter :public
		std::iterator<std::bidirectional_iterator_tag, Item> {

		Item* ptr;

		AVL_treeIter(Item* const p = nullptr) :ptr(p) {}

		typename Item::data_type& operator*() const { return ptr->data; }
		Item* operator->() const { return ptr; }

		AVL_treeIter& operator++() {
			if (!ptr)return *this;
			if (ptr->rchild) {
				ptr = ptr->rchild;
				while (ptr->lchild)
					ptr = ptr->lchild;
			}
			else {
				Item* p_node;
				while ((p_node=ptr->parent) && p_node->rchild == ptr)
					ptr = p_node;
				if (ptr->parent->parent != ptr)
					ptr = ptr->parent;
				else
					ptr = nullptr;
			}
			return *this;
		}
		AVL_treeIter operator++(int) {
			AVL_treeIter temp = *this;
			++* this;
			return temp;
		}
		AVL_treeIter& operator--() {
			if (!ptr)return *this;
			if (ptr->lchild) {
				ptr = ptr->lchild;
				while (ptr->rchild)
					ptr = ptr->rchild;
			}
			else {
				while (ptr->parent->lchild == ptr)
					ptr = ptr->parent;
				if (ptr->parent->parent != ptr)
					ptr = ptr->parent;
				else
					ptr = nullptr;
			}
			return *this;
		}
		AVL_treeIter operator--(int) {
			AVL_treeIter temp = *this;
			--* this;
			return temp;
		}

		bool operator==(const AVL_treeIter& another) const { return ptr == another.ptr; }
		bool operator!=(const AVL_treeIter& another) const { return ptr != another.ptr; }
	};

	template<typename T, typename Alloc = std::allocator<AVL_tree_node<T>>>
	class AVL_tree {
	public:
		typedef AVL_tree_node<T>*				node_ptr;
		typedef T								data_type;
		typedef AVL_treeIter<AVL_tree_node<T> > iterator;
	private:
		typedef AVL_tree_node<T> 				node;
		node root;
		int count;
		//Alloc alloc;
	public:
		AVL_tree() :count(0) { root.parent = root.lchild = root.rchild = nullptr; }
		~AVL_tree() { destructor(root.parent); count = 0; }

		void clear();
		void insert(const data_type&);
		void erase(iterator);

		iterator begin() const;
		iterator end() const;
		iterator rbegin() const;
		iterator rend() const;
		const iterator cbegin() const;
		const iterator cend() const;
		const iterator crbegin() const;
		const iterator crend() const;

		iterator find(const T& val) const;
		int size() const { return count; }
		bool empty() const { return root.parent == nullptr; }
		static bool test_height(node_ptr);
		bool test_height() { return test_height(root.parent); };
	private:
		static void destructor(node_ptr);
		void L_Rotage(node_ptr);
		void R_Rotage(node_ptr);
		void AVL_fix_up(node_ptr);
	};

	template<typename T, typename Alloc>
	void AVL_tree<T, Alloc>::clear()
	{
		destructor(root.parent);
		root.lchild = root.rchild = nullptr;
		count = 0;
	}

	template<typename T, typename Alloc>
	void AVL_tree<T, Alloc>::insert(const data_type& _data) {
		if (root.parent) {
			node_ptr locate = root.parent;
			while (true) {
				if (_data < locate->data) {
					if (locate->lchild)
						locate = locate->lchild;
					else {
						locate->lchild = (node_ptr)malloc(sizeof(node));
						//locate->lchild = alloc.allocate(sizeof(node));
						locate->lchild->parent = locate;
						locate->lchild->data = _data;
						locate->lchild->lchild = locate->lchild->rchild = nullptr;
						locate->lchild->height = 1;
						if (root.lchild == locate)
							root.lchild = locate->lchild;
						count++;
						break;
					}
				}
				else if (locate->data < _data) {
					if (locate->rchild)
						locate = locate->rchild;
					else {
						locate->rchild = (node_ptr)malloc(sizeof(node));
						//locate->rchild = alloc.allocate(sizeof(node));
						locate->rchild->parent = locate;
						locate->rchild->data = _data;
						locate->rchild->lchild = locate->rchild->rchild = nullptr;
						locate->rchild->height = 1;
						if (root.rchild == locate)
							root.rchild = locate->rchild;
						count++;
						break;
					}
				}
				else
					return;
			}
			AVL_fix_up(locate);
		}
		else {
			root.parent = (node_ptr)malloc(sizeof(node));
			root.parent->lchild = root.parent->rchild = nullptr;
			root.parent->height = 1;
			root.parent->parent = &root;
			root.parent->data = _data;
			root.lchild = root.rchild = root.parent;
			count++;
		}
	}

	template<typename T, typename Alloc>
	void AVL_tree<T, Alloc>::erase(AVL_tree<T, Alloc>::iterator it) {
		if (it.ptr == nullptr)
			return;
		node_ptr fix_location;
		if (it->lchild == nullptr) {
			if (it->rchild == nullptr) {
				if (it.ptr == root.parent)
					root.parent = nullptr;
				else {
					if (it->parent->lchild == it.ptr)
						it->parent->lchild = nullptr;
					else
						it->parent->rchild = nullptr;
				}
				if (root.lchild == root.rchild)
					root.lchild = root.rchild = nullptr;
				else {
					if (it.ptr == root.lchild)
						root.lchild = it->parent;
					else if (it.ptr == root.rchild)
						root.rchild = it->parent;
				}
				fix_location = it->parent;
			}
			else {
				if (it.ptr == root.parent)
					root.parent = it->rchild;
				else {
					if (it->parent->lchild == it.ptr)
						it->parent->lchild = it->rchild;
					else
						it->parent->rchild = it->rchild;
				}
				it->rchild->parent = it->parent;
				node_ptr next = it->rchild;
				while (next->lchild)
					next = next->lchild;
				if (it.ptr == root.lchild)
					root.lchild = next;
				else if (it.ptr == root.rchild)
					root.rchild = next;
				fix_location = it->parent;
			}
		}
		else {
			if (it->rchild == nullptr) {
				if (it.ptr == root.parent)
					root.parent = it->lchild;
				else {
					if (it->parent->lchild == it.ptr)
						it->parent->lchild = it->lchild;
					else
						it->parent->rchild = it->lchild;
				}
				it->lchild->parent = it->parent;
				node_ptr last = it->lchild;
				while (last->rchild)
					last = last->rchild;
				if (it.ptr == root.lchild)
					root.lchild = last;
				else if (it.ptr == root.rchild)
					root.rchild = last;
				fix_location = it->parent;
			}
			else {
				iterator temp(it);
				++temp;
				temp->lchild = it->lchild;
				if (it->rchild != temp.ptr) {
					fix_location = temp->parent;
					temp->parent->lchild = temp->rchild;
					if(temp->rchild)
						temp->rchild->parent = temp->parent;
					temp->rchild = it->rchild;
					it->rchild->parent = temp.ptr;
					temp->height = it->height;
				}
				else {
					fix_location = temp.ptr;
				}
				temp->parent = it->parent;
				it->lchild->parent = temp.ptr;
				if (it.ptr == root.parent)
					root.parent = temp.ptr;
				else {
					if (it->parent->lchild == it.ptr)
						it->parent->lchild = temp.ptr;
					else
						it->parent->rchild = temp.ptr;
				}
			}
		}
		AVL_fix_up(fix_location);
		free(it.ptr);
		count--;
	}

	template<typename T, typename Alloc>
	typename AVL_tree<T, Alloc>::iterator AVL_tree<T, Alloc>::begin()const {
#ifdef _DEBUG
		node_ptr min = root.parent;
		while (min && min->lchild)
			min = min->lchild;
		assert(min == root.lchild);
#endif
		return iterator(root.lchild);
	}

	template<typename T, typename Alloc>
	typename AVL_tree<T, Alloc>::iterator AVL_tree<T, Alloc>::end()const {
		return iterator(nullptr);
	}

	template<typename T, typename Alloc>
	typename AVL_tree<T, Alloc>::iterator AVL_tree<T, Alloc>::rbegin() const
	{
#ifdef _DEBUG
		node_ptr max = root.parent;
		while (max && max->rchild)
			max = max->rchild;
		assert(max == root.rchild);
#endif
		return iterator(root.rchild);
	}

	template<typename T, typename Alloc>
	typename AVL_tree<T, Alloc>::iterator AVL_tree<T, Alloc>::rend() const
	{
		return iterator(nullptr);
	}

	template<typename T, typename Alloc>
	const typename AVL_tree<T, Alloc>::iterator AVL_tree<T, Alloc>::cbegin() const
	{
		return static_cast<const iterator>(this->begin());
	}

	template<typename T, typename Alloc>
	const typename AVL_tree<T, Alloc>::iterator AVL_tree<T, Alloc>::cend() const
	{
		return static_cast<const iterator>(this->end());
	}

	template<typename T, typename Alloc>
	const typename AVL_tree<T, Alloc>::iterator AVL_tree<T, Alloc>::crbegin() const
	{
		return static_cast<const iterator>(this->cbegin());
	}

	template<typename T, typename Alloc>
	const typename AVL_tree<T, Alloc>::iterator AVL_tree<T, Alloc>::crend() const
	{
		return static_cast<const iterator>(this->cend());
	}

	template<typename T, typename Alloc>
	typename AVL_tree<T, Alloc>::iterator AVL_tree<T, Alloc>::find(const T& val)const
	{
		node_ptr p = root.parent;
		while (p) {
			if (p->data < val)
				p = p->rchild;
			else if (p->data > val)
				p = p->lchild;
			else
				return iterator(p);
		}
		return iterator(nullptr);
	}

	template<typename T, typename Alloc>
	bool AVL_tree<T, Alloc>::test_height(node_ptr p)
	{
		if (!p)return true;
		if (test_height(p->lchild) && test_height(p->rchild)) {
			int a = p->lchild ? p->lchild->height : 0;
			int b = p->rchild ? p->rchild->height : 0;
			return p->height == 1 + (a < b ? b : a);
		}
		return false;
	}

	template<typename T, typename Alloc>
	void AVL_tree<T, Alloc>::destructor(node_ptr pointer) {
		if (pointer) {
			destructor(pointer->lchild);
			destructor(pointer->rchild);
			free(pointer);
			//alloc.deallocate(pointer, sizeof(node));
		}
	}

	template<typename T, typename Alloc>
	void AVL_tree<T, Alloc>::R_Rotage(node_ptr pointer) {
		node_ptr temp = pointer->lchild;
		pointer->lchild = temp->rchild;
		temp->rchild = pointer;
		if (pointer->lchild)
			pointer->lchild->parent = pointer;
		temp->parent = pointer->parent;
		pointer->parent = temp;
		if (temp->parent != &root) {
			if (temp->parent->lchild == pointer)
				temp->parent->lchild = temp;
			else
				temp->parent->rchild = temp;
		}
		else
			root.parent = temp;
	}

	template<typename T, typename Alloc>
	void AVL_tree<T, Alloc>::L_Rotage(node_ptr pointer) {
		node_ptr temp = pointer->rchild;
		pointer->rchild = temp->lchild;
		temp->lchild = pointer;
		if (pointer->rchild)
			pointer->rchild->parent = pointer;
		temp->parent = pointer->parent;
		pointer->parent = temp;
		if (temp->parent != &root) {
			if (temp->parent->lchild == pointer)
				temp->parent->lchild = temp;
			else
				temp->parent->rchild = temp;
		}
		else
			root.parent = temp;
	}

	template<typename T, typename Alloc>
	void AVL_tree<T, Alloc>::AVL_fix_up(node_ptr locate) {
		while (locate != &root) {
			int deg = locate->deg();
			if (deg < -1) {
				if (locate->rchild->deg() > 0) {
					R_Rotage(locate->rchild);
					locate->rchild->rchild->update_height();
				}
				L_Rotage(locate);
				locate->update_height();
				locate = locate->parent;
			}
			else if (deg > 1) {
				if (locate->lchild->deg() < 0) {
					L_Rotage(locate->lchild);
					locate->lchild->lchild->update_height();
				}
				R_Rotage(locate);
				locate->update_height();
				locate = locate->parent;
			}
			if (locate->update_height() || locate->parent->update_height())
				locate = locate->parent;
			else
				break;
		}
		return;

		//unsigned int route = 0u;
		//while (locate != &root) {
		//	int deg = locate->deg();
		//	if (deg < -1 || deg > 1) {
		//		switch (route & 3u) {
		//		case 2u:	L_Rotage(locate->lchild);
		//		case 0u:	R_Rotage(locate);
		//			break;
		//		case 1u:	R_Rotage(locate->rchild);
		//		case 3u:	L_Rotage(locate);
		//		}
		//	}
		//	locate->update_height();
		//	route = route << 1;
		//	if (/*locate->parent!=&root && */locate->parent->rchild == locate)
		//		route |= 1u;
		//	locate = locate->parent;
		//}
	}
}
#endif //AVL_TREE