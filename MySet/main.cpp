#include "myset.cpp"
#include <iostream>
#include <cstdlib>
#include <set>
#include <Windows.h>
using namespace std;
using namespace xfz;
int main() {
	myset<int>*a=new myset<int>;
	set<int>*b=new set<int>;

	DWORD t1, t2, t3, t4, t5, t6;
	t1 = GetTickCount();
	for (int i = 1; i <= 15; i++) {
		a->insert(i);
	}
	t2 = GetTickCount();
	for (int i = 1; i <= 15; i++) {
		b->insert(i);
	}
	t3 = GetTickCount();
	cout << (a->size()) << ' ' << (b->size()) << endl;
	t4 = GetTickCount();
	while (!a->empty())
		a->erase(a->begin());
	t5 = GetTickCount();
	while (!b->empty())
		b->erase(b->begin());
	t6 = GetTickCount();
	cout << "Insert a: " << t2 - t1 << "ms" << endl;
	cout << "Insert b: " << t3 - t2 << "ms" << endl;
	cout << "Erase  a: " << t5 - t4 << "ms" << endl;
	cout << "Erase  b: " << t6 - t5 << "ms" << endl;
	system("pause");
	return 0;
}