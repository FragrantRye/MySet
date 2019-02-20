#ifndef AVL_TREE_CPP
#define AVL_TREE_CPP
#include <iterator>
namespace xfz {
	template<typename T>
	struct AVL_tree_node {
		typedef  AVL_tree_node<T>*  node_ptr;
		typedef  T                  data_type;
		typedef  T*                 pointer;

		node_ptr parent;
		node_ptr lchild;
		node_ptr rchild;

		T        data;
		int      height;

		void update_height() {
			int a = lchild ? lchild->height : 0;
			int b = rchild ? rchild->height : 0;
			height = 1 + (a < b ? b : a);
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

		AVL_treeIter(Item* p = nullptr) :ptr(p) {}

		typename Item::data_type& operator*() const { return ptr->data; }
		Item* operator->() const { return ptr; }

		AVL_treeIter& operator++() {
			Item* temp = ptr;
			if (temp->rchild) {
				temp = temp->rchild;
				while (temp->lchild)
					temp = temp->lchild;
			}
			else {
				while (temp->parent && temp->parent->rchild == temp)
					temp = temp->parent;
				if (temp->parent)
					temp = temp->parent;
				else
					temp = ptr + 1;
			}
			ptr = temp;
			return *this;
		}
		AVL_treeIter operator++(int) {
			AVL_treeIter temp = *this;
			++*this;
			return temp;
		}
		AVL_treeIter& operator--() {
			if (ptr->lchild) {
				ptr = ptr->lchild;
				while (ptr->rchild)
					ptr = ptr->rchild;
			}
			else {
				while (ptr->parent->lchild == ptr)
					ptr = ptr->parent;
				ptr = ptr->parent;
			}
			return *this;
		}
		AVL_treeIter operator--(int) {
			AVL_treeIter temp = *this;
			--*this;
			return temp;
		}

		bool operator==(const AVL_treeIter& another) const { return ptr == another.ptr; }
		bool operator!=(const AVL_treeIter& another) const { return ptr != another.ptr; }
	};

	template<typename T>
	class AVL_tree {
	public:
		typedef AVL_tree_node<T>*				node_ptr;
		typedef T								data_type;
		typedef AVL_treeIter<AVL_tree_node<T> > iterator;
	private:
		typedef AVL_tree_node<T> 				node;
		node_ptr root;
		int count;
	public:
		AVL_tree() :root(nullptr), count(0) {}
		~AVL_tree() { destructor(root); count = 0; }

		void insert(data_type);
		void erase(iterator);

		iterator begin() const;
		iterator end() const;

		int size() const { return count; }
		bool empty() const { return root == nullptr; }
	private:
		void destructor(node_ptr);
		void L_Rotage(node_ptr);
		void R_Rotage(node_ptr);
		void AVL_fix_up(node_ptr&);
	};

	template<typename T>
	void AVL_tree<T>::insert(data_type _data) {
		if (root) {
			node_ptr locate = root;
			while (true) {
				if (_data < locate->data) {
					if (locate->lchild)
						locate = locate->lchild;
					else {
						locate->lchild = (node_ptr)malloc(sizeof(node));
						locate->lchild->parent = locate;
						locate->lchild->data = _data;
						locate->lchild->lchild = locate->lchild->rchild = nullptr;
						locate->lchild->height = 1;
						locate = locate->lchild;
						count++;
						break;
					}
				}
				else if (locate->data < _data) {
					if (locate->rchild)
						locate = locate->rchild;
					else {
						locate->rchild = (node_ptr)malloc(sizeof(node));
						locate->rchild->parent = locate;
						locate->rchild->data = _data;
						locate->rchild->lchild = locate->rchild->rchild = nullptr;
						locate->rchild->height = 1;
						locate = locate->rchild;
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
			root = (node_ptr)malloc(sizeof(node));
			root->lchild = root->rchild = nullptr;
			root->height = 1;
			root->parent = nullptr;
			root->data = _data;
			count++;
		}
	}

	template<typename T>
	void AVL_tree<T>::erase(iterator iter) {
		node_ptr _parent = iter.ptr->parent;
		node_ptr fixing_locate;
		if (_parent != nullptr) {
			if (iter.ptr->lchild) {
				if (iter.ptr->rchild) {
					node_ptr next = iter.ptr->rchild;
					if (next->lchild) {
						while (next->lchild)
							next = next->lchild;
						fixing_locate = next->parent;
						next->parent->lchild = nullptr;
						next->rchild = iter.ptr->rchild;
						next->rchild->parent = next;
					}
					else {
						fixing_locate = next;
					}
					next->lchild = iter.ptr->lchild;
					next->lchild->parent = next;
					next->parent = iter.ptr->parent;
					if (_parent->lchild == iter.ptr)
						_parent->lchild = next;
					else
						_parent->rchild = next;
				}
				else {
					if (_parent->lchild == iter.ptr)
						_parent->lchild = iter.ptr->lchild;
					else
						_parent->rchild = iter.ptr->lchild;
					iter.ptr->lchild->parent = _parent;
					fixing_locate = _parent;
				}
			}
			else {
				if (iter.ptr->rchild) {
					if (_parent->lchild == iter.ptr)
						_parent->lchild = iter.ptr->rchild;
					else
						_parent->rchild = iter.ptr->rchild;
					iter.ptr->rchild->parent = _parent;
					fixing_locate = _parent;
				}
				else {
					if (_parent->lchild == iter.ptr)
						_parent->lchild = nullptr;
					else
						_parent->rchild = nullptr;
					fixing_locate = _parent;
				}
			}
		}
		else {
			if (iter.ptr->lchild) {
				if (iter.ptr->rchild) {
					node_ptr next = iter.ptr->rchild;
					if (next->lchild) {
						while (next->lchild)
							next = next->lchild;
						root = next;
						next->lchild = iter.ptr->lchild;
						next->rchild = iter.ptr->rchild;
						next->lchild->parent = next;
						next->rchild->parent = next;
						next->parent->lchild = nullptr;
						fixing_locate = next->parent;
						next->parent = nullptr;
					}
					else {
						next->lchild = iter.ptr->lchild;
						next->parent = nullptr;
						next->lchild->parent = next;
						root = next;
						fixing_locate = next->lchild;
					}
				}
				else {
					root = iter.ptr->lchild;
					root->parent = nullptr;
					fixing_locate = root;
				}
			}
			else {
				if (iter.ptr->rchild) {
					root = iter.ptr->rchild;
					root->parent = nullptr;
				}
				else {
					root = nullptr;
				}
				fixing_locate = root;
			}
		}
		free(iter.ptr);
		AVL_fix_up(fixing_locate);
		count--;
	}

	template<typename T>
	typename AVL_tree<T>::iterator AVL_tree<T>::begin()const {
		node_ptr min = root;
		while (min && min->lchild)
			min = min->lchild;
		return iterator(min);
	}

	template<typename T>
	typename AVL_tree<T>::iterator AVL_tree<T>::end()const {
		node_ptr max = root;
		while (max && max->rchild)
			max = max->rchild;
		return iterator(max + 1);
	}

	template<typename T>
	void AVL_tree<T>::destructor(node_ptr pointer) {
		if (pointer) {
			destructor(pointer->lchild);
			destructor(pointer->rchild);
			free(pointer);
		}
	}

	template<typename T>
	void AVL_tree<T>::R_Rotage(node_ptr pointer) {
		node_ptr temp = pointer->lchild;
		pointer->lchild = temp->rchild;
		temp->rchild = pointer;
		if (pointer->lchild)
			pointer->lchild->parent = pointer;
		temp->parent = pointer->parent;
		pointer->parent = temp;
		if (temp->parent) {
			if (temp->parent->lchild == pointer)
				temp->parent->lchild = temp;
			else
				temp->parent->rchild = temp;
		}
		else
			root = temp;
		pointer->update_height();
		temp->update_height();
	}

	template<typename T>
	void AVL_tree<T>::L_Rotage(node_ptr pointer) {
		node_ptr temp = pointer->rchild;
		pointer->rchild = temp->lchild;
		temp->lchild = pointer;
		if (pointer->rchild)
			pointer->rchild->parent = pointer;
		temp->parent = pointer->parent;
		pointer->parent = temp;
		if (temp->parent) {
			if (temp->parent->lchild == pointer)
				temp->parent->lchild = temp;
			else
				temp->parent->rchild = temp;
		}
		else
			root = temp;
		pointer->update_height();
		temp->update_height();
	}

	template<typename T>
	void AVL_tree<T>::AVL_fix_up(node_ptr &locate) {
		unsigned route = 0u;
		while (locate) {
			if (locate->deg() < -1 || locate->deg() > 1) {
				switch (route & 3u) {
				case 2u:	L_Rotage(locate->lchild);
				case 0u:	R_Rotage(locate);
					break;
				case 1u:	R_Rotage(locate->rchild);
				case 3u:	L_Rotage(locate);
				}
			}
			locate->update_height();
			route = route << 1;
			if (locate->parent && locate->parent->rchild == locate)
				route |= 1u;
			locate = locate->parent;
		}
	}
}
#endif //AVL_TREE