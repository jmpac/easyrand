# easyrand
An easy-to-use interface on top of &lt;random>, along the lines of `rand_int` (from the C++ Library Fundamentals Version 2 TS),
but with support for floating point types and distributions other than uniform.

	using namespace easyrand;
	rand(10, 20);    // a random int in the interval [10, 20] (uniform distribution).
	rand(0., 1.);    // a random double in the interval [0., 1.] (uniform distribution).

	auto normaldist = normal_distribution<>(5.0, 0.5);
	rand(normaldist);    // a random number according to the distribution passed in.

	auto rngf = make_rng(0.f, 1.f);
	rngf();    // a random float in the interval [0., 1.].

	auto bern = make_rng<bernoulli_distribution>(0.75);
	bern();    // a random number according to bernoulli_distribution.

