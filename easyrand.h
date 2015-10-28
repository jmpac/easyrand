/*
	easyrand provides an easy-to-use interface on top of <random>. Highlights:

	1) You don't need to worry about engines. A thread_local engine is managed for you. 
	2) You don't need to worry about seeds. The library uses a random seed automatically. (You can control the seed if you need to.)
	3) Uniform distribution is the default. Other distributions can be used.

	Usage:

		// Use rand(a, b) to select a number in the closed internal [a, b] with uniform distribution.
		rand(10, 20);		// a random int in the interval [10, 20].
		rand(0., 1.);		// a random double in the interval [0., 1.].
		rand(0.f, 1.f);	// a random float in the interval [0., 1.].

		// To use a distribution other than uniform, pass it to rand(). (...which will invoke its operator() with the internal engine.)
		auto normaldist = normal_distribution<>(5.0, 0.5);
		rand(normaldist);		// a random number according to the distribution passed in.

		// If you're making the same call repeatedly, it can be convenient to use a function object with the
		// arguments bound. make_rng helps with that:
		auto rngf = make_rng(0.f, 1.f);
		rngf();			// a random float in the interval [0., 1.].

		auto bern = make_rng<bernoulli_distribution>(0.75);
		bern();			// a random number according to bernoulli_distribution.

	Licence: Whatever.

	https://github.com/jmpac/easyrand
*/

#ifndef EASYRAND_H_INCLUDED
#define EASYRAND_H_INCLUDED

#include <random>
#include <type_traits>

namespace easyrand {
	namespace detail {

		inline auto get_random_seed() {
			return std::random_device()();
		}

		inline auto& get_engine() {
			thread_local std::default_random_engine e{ get_random_seed() };
			return e;
		}

		// uniform_int_distribution only works with these types (excludes char and bool, unlike std::is_integral).
		template <class T>
		struct is_integer : std::bool_constant<
			std::is_same<T, short>::value ||
			std::is_same<T, int>::value ||
			std::is_same<T, long>::value ||
			std::is_same<T, long long>::value ||
			std::is_same<T, unsigned short>::value ||
			std::is_same<T, unsigned int>::value ||
			std::is_same<T, unsigned long>::value ||
			std::is_same<T, unsigned long long>::value> {
		};

	} // namespace detail

	// These functions provide control over the seed, which can be useful for debugging and testing (to create a repeatable/predictable sequence of random numbers).
	// Normally, you don't need to worry about this. The engine is randomly seeded by default.
	inline void reseed() {
		detail::get_engine().seed(detail::get_random_seed());
	}
	inline void reseed(std::default_random_engine::result_type value) {
		detail::get_engine().seed(value);
	}

	// Returns a random number in the range [a, b], selected using uniform distribution.
	template <class T>
	auto rand(T a, T b) -> std::enable_if_t<detail::is_integer<T>::value, T> {
		uniform_int_distribution<T> dist(a, b);
		return dist(detail::get_engine());
	}

	// Returns a random number in the range [a, b], selected using uniform distribution.
	template <class T>
	auto rand(T a, T b) -> std::enable_if_t<std::is_floating_point<T>::value, T> {
		uniform_real_distribution<T> dist(a, b);
		return dist(detail::get_engine());
	}

	// Returns a random number generated from the given distribution.
	template <class Dist>
	auto rand(Dist& dist) {
		return dist(detail::get_engine());
	}

	// Returns a random number generated from the given distribution.
	template <class Dist>
	auto rand(Dist& dist, typename Dist::param_type const& p) {
		return dist(detail::get_engine(), p);
	}

	// Returns a random-number-generating function object that invokes rand(a, b).
	template <class T>
	auto make_rng(T a, T b) {
		return [a,b]{ return rand(a, b); };
	}

	// Returns a random-number-generating function object using the distribution specified by "Dist", which is constructed using "args".
	template <class Dist, class... Args>
	auto make_rng(Args&&... args) {
		return [dist = Dist(std::forward<Args>(args)...)]() mutable { return rand(dist); };
	}	

} // namespace easyrand


#endif

