#include <iostream>
#include <cmath>
#include <ctime>
#include <boost/random.hpp>

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
		return int(std::ceil(value - 0.5));
	} else {
		return int(std::floor(value + 0.5));
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
		result = int(std::floor(value + 0.5));
	} else {
		result = int(std::ceil(value - 0.5));
	}
	current = 1 - current;
	return result;
}

int round_half_randomly(double value) {
	if (Random.get_bit() == 0) {
		return int(std::floor(value + 0.5));
	} else {
		return int(std::ceil(value - 0.5));
	}
}

int main(void) {
	double value;
	std::cin >> value;

	srand(time(NULL));

	std::cout << "Round next lower integer:   " << int(floor(value)) << std::endl;
	std::cout << "Round next greater integer: " << int(ceil(value)) << std::endl;
	std::cout << "Round half up:              " << round_half_up(value) << std::endl;
	std::cout << "Round half douwn:           " << round_half_down(value) << std::endl;
	std::cout << "Round half away from zero:  " << round_half_away_from_zero(value) << std::endl;
	std::cout << "Round half towards zero:    " << round_half_towards_zero(value) << std::endl;
	std::cout << "Round half to even:         " << round_half_to_even(value) << std::endl;
	std::cout << "Round half to odd:          " << round_half_to_odd(value) << std::endl;
	std::cout << "Round half alternatingly:   " << round_half_alternatingly(value) << std::endl;
	std::cout << "Round half randomly:        " << round_half_randomly(value) << std::endl;
	
	return 0;
}
