#include "easyrand.h"

#include <iostream>
using namespace easyrand;
using namespace std;

int main() {

	for (size_t i = 0; i < 5; ++i)
		cout << rand(0, 200) << endl;

	for (size_t i = 0; i < 5; ++i)
		cout << rand(0., 200.) << endl;

	for (size_t i = 0; i < 5; ++i)
		cout << rand(0.f, 200.f) << endl;

	reseed(0); // make the results repeatable.

	auto rngf = make_rng(0.f, 1.f);
	for (size_t i = 0; i < 5; ++i)
		cout << rngf() << endl;

	reseed(); // reseed randomly.

	auto normaldist = normal_distribution<>();
	for (size_t i = 0; i < 5; ++i)
		cout << rand(normaldist) << endl;

	auto bern = make_rng<bernoulli_distribution>(0.75);
	for (size_t i = 0; i < 5; ++i)
		cout << bern() << endl;
}
