#include "myset.cpp"
#include <iostream>
#include <algorithm>
#include <set>
#include <chrono>
using namespace std;
using namespace xfz;
int main() {
	const int DATA_NUM = 10 * 1024 * 1024;
	int* test_data = new int[DATA_NUM];
	for (int i = 0; i < DATA_NUM; i++)
		test_data[i] = i;
	random_shuffle(test_data, test_data + DATA_NUM);
	myset<int>* a = new myset<int>;

	set<int>* b = new set<int>;
	chrono::system_clock::time_point t1, t2, t3;

	cout << "prepered. start benchmark¡­¡­" << endl;
	{
		t1 = chrono::system_clock::now();
		for (int i = 0; i < DATA_NUM; i++) {
			a->insert(test_data[i]);
		}
		t2 = chrono::system_clock::now();
		for (int i = 0; i < DATA_NUM; i++) {
			b->insert(test_data[i]);
		}
		t3 = chrono::system_clock::now();
		cout << "size: " << (a->size()) << ' ' << (b->size()) << endl;
		long long duration1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
		long long duration2 = chrono::duration_cast<chrono::milliseconds>(t3 - t2).count();
		cout << "Insert xfz::set: " << duration1 << "ms (";
		if (duration1 > duration2)
			cout << '+';
		cout << 100.0 * (duration1 - duration2) / duration2 << "%)" << endl;
		cout << "Insert std::set: " << duration2 << "ms" << endl << endl;
	}
	{
		int sum1 = 0, sum2 = 0;
		t1 = chrono::system_clock::now();
		for (int i = 0; i < DATA_NUM; i++) {
			sum1 ^= *(a->find(test_data[i]));
		}
		t2 = chrono::system_clock::now();
		for (int i = 0; i < DATA_NUM; i++) {
			sum2 ^= *(b->find(test_data[i]));
		}
		t3 = chrono::system_clock::now();
		cout << "sum: " << sum1 << ' ' << sum2 << endl;
		long long duration1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
		long long duration2 = chrono::duration_cast<chrono::milliseconds>(t3 - t2).count();
		cout << "Find xfz::set: " << duration1 << "ms (";
		if (duration1 > duration2)
			cout << '+';
		cout << 100.0 * (duration1 - duration2) / duration2 << "%)" << endl;
		cout << "Find std::set: " << duration2 << "ms" << endl << endl;
	}
	{
		int sum1 = 0, sum2 = 0;
		t1 = chrono::system_clock::now();
		for(int j=0;j<10;j++)
			for (int i : *a) {
				sum1 ^= i;
			}
		t2 = chrono::system_clock::now();
		for (int j = 0; j < 10; j++)
			for (int i : *b) {
				sum2 ^= i;
			}
		t3 = chrono::system_clock::now();
		cout << "sum: " << sum1 << ' ' << sum2 << endl;
		long long duration1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
		long long duration2 = chrono::duration_cast<chrono::milliseconds>(t3 - t2).count();
		cout << "Travel xfz::set: " << duration1 << "ms (";
		if (duration1 > duration2)
			cout << '+';
		cout << 100.0 * (duration1 - duration2) / duration2 << "%)" << endl;
		cout << "Travel std::set: " << duration2 << "ms" << endl << endl;
	}
	{
		random_shuffle(test_data, test_data + DATA_NUM);
		t1 = chrono::system_clock::now();
		for (int i = 0; i < DATA_NUM; i++) {
			auto it = a->find(test_data[i]);
			if (it != a->end())
				a->erase(it);
		}
		t2 = chrono::system_clock::now();
		for (int i = 0; i < DATA_NUM; i++) {
			auto it = b->find(test_data[i]);
			if (it != b->end())
				b->erase(it);
		}
		t3 = chrono::system_clock::now();
		long long duration1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
		long long duration2 = chrono::duration_cast<chrono::milliseconds>(t3 - t2).count();
		cout << "Erase xfz::set: " << duration1 << "ms (";
		if (duration1 > duration2)
			cout << '+';
		cout << 100.0 * (duration1 - duration2) / duration2 << "%)" << endl;
		cout << "Erase std::set: " << duration2 << "ms" << endl << endl;
	}
	delete[]test_data;
	{
		t1 = chrono::system_clock::now();
		delete a;
		t2 = chrono::system_clock::now();
		delete b;
		t3 = chrono::system_clock::now();
		long long duration1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
		long long duration2 = chrono::duration_cast<chrono::milliseconds>(t3 - t2).count();
		cout << "Delete xfz::set: " << duration1 << "ms (";
		if (duration1 > duration2)
			cout << '+';
		if (duration2 != 0)
			cout << 100.0 * (duration1 - duration2) / duration2 << "%)" << endl;
		else
			cout << "N/A%)" << endl;
		cout << "Delete std::set: " << duration2 << "ms" << endl << endl;
	}
	system("pause");
	return 0;
}