#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <boost/random.hpp>
#include <vector>

class RandomGenerator {
	public:
		RandomGenerator():
			generator(std::time(0)),
			bitDistribution(0, 1),
			variateReal01Generator(generator, real01Distribution),
			variateBitGenerator(generator, bitDistribution)
		{
		}

		int get_bit() {
			return variateBitGenerator();
		}

		double get_real_01() {
			return variateReal01Generator();
		}

	private:
		typedef boost::mt19937 GeneratorType;
		GeneratorType generator;
		boost::uniform_01<> real01Distribution;
		boost::uniform_int<> bitDistribution;
		boost::variate_generator<GeneratorType&, boost::uniform_01<> > variateReal01Generator;
		boost::variate_generator<GeneratorType&, boost::uniform_int<> > variateBitGenerator;
} Random;

template <typename Num> inline
Num sign(Num value) {
	if (value == 0) {
		return 0;
	} else if (value > 0) {
		return 1;
	} else {
		return -1;
	}
}

// Традиционное округление
int round_half_up(double value) {
	return int(std::floor(value + 0.5));
}

int round_half_down(double value) {
	return int(std::ceil(value - 0.5));
}

int round_half_away_from_zero(double value) {
	return int(value + 0.5 * sign(value));
}

int round_half_towards_zero(double value) {
	if (value >= 0) {
		return round_half_down(value);
	} else {
		return round_half_up(value);
	}
}

// Банковское округление
int round_half_to_even(double value) {
	double temp = std::floor(value + 0.5);
	if ((value - std::floor(value) == 0.5) && (std::fmod(temp, 2) != 0)) {
		temp -= 1.0;
	}
	return int(temp);
}

int round_half_to_odd(double value) {
	double temp = std::floor(value + 0.5);
	if ((value - std::floor(value) == 0.5) && (std::fmod(temp, 2) == 0)) {
		temp -= 1.0;
	}
	return int(temp);
}

int round_half_alternatingly(double value) {
	static int current = 0;
	int result;
	if (current == 0) {
		result = round_half_up(value);
	} else {
		result = round_half_down(value);
	}
	current = 1 - current;
	return result;
}

int round_half_randomly(double value) {
	if (Random.get_bit() == 0) {
		return round_half_up(value);
	} else {
		return round_half_down(value);
	}
}

int main(void) {
	const size_t N = 1000000000;
	const double k = 1000.0;

	std::vector<long double> results(10);
	for (size_t j = 0; j < results.size(); ++j) {
		results[j] = 0.0;
	}

	for (size_t i = 0; i < N; ++i) {
		//double x = std::floor((Random.get_real_01() * k) - k / 2 * 100) * 0.01;
		double x = std::floor((Random.get_real_01() * k) * 100) * 0.01;
		//std::cout << x << " "; //DEBUG

		results[0] += std::floor(x) - x;
		results[1] += std::ceil(x) - x;
		results[2] += round_half_up(x) - x;
		results[3] += round_half_down(x) - x;
		results[4] += round_half_away_from_zero(x) - x;
		results[5] += round_half_towards_zero(x) - x;
		results[6] += round_half_to_even(x) - x;
		results[7] += round_half_to_odd(x) - x;
		results[8] += round_half_alternatingly(x) - x;
		results[9] += round_half_randomly(x) - x;
	}
	std::cout.setf(std::ios_base::fixed, std::ios::floatfield);
	std::cout.precision(2);
	std::cout << "Round next lower integer:   "
		<< std::setw(15) << std::setfill('.') << results[0] << std::endl;
	std::cout << "Round next greater integer: "
		<< std::setw(15) << results[1] << std::endl;
	std::cout << "Round half up:              "
		<< std::setw(15) << results[2] << std::endl;
	std::cout << "Round half down:            "
		<< std::setw(15) << results[3] << std::endl;
	std::cout << "Round half away from zero:  "
		<< std::setw(15) << results[4] << std::endl;
	std::cout << "Round half towards zero:    "
		<< std::setw(15) << results[5] << std::endl;
	std::cout << "Round half to even:         "
		<< std::setw(15) << results[6] << std::endl;
	std::cout << "Round half to odd:          "
		<< std::setw(15) << results[7] << std::endl;
	std::cout << "Round half alternatingly:   "
		<< std::setw(15) << results[8] << std::endl;
	std::cout << "Round half randomly:        "
		<< std::setw(15) << results[9] << std::endl;

	return 0;
}
