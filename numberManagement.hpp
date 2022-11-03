// Number Display Math
// By Half-Qilin AKA Hail

#include <iostream>

int getFirstDigit(unsigned long long n) {
	while (n >= 10) n /= 10;
	return (int)n;
}

int getFirstDigitIndex(unsigned long long n) {
	int a = 0;
	while (n >= 10) {
		n /= 10;
		a++;
	}
	return a;
}

int getDigitAtIndex(unsigned long long n, unsigned int index) {
	unsigned int a = 0;
	while (a < index) {
		n /= 10;
		a++;
	}
	return n % 10;
}

unsigned long long getPowerOf10(unsigned int power) {
	unsigned long long p = 1;
	while (power) {
		power--;
		p *= 10;
	}
	return p;
}

unsigned long long pow(unsigned long long n, unsigned int p) {
	unsigned long long d = n;
	while (p) {
		p--;
		n *= d;
	}
	return n;
}

std::string displayNumbMaxDigits(unsigned long long number, unsigned int digits) {
	if (number / getPowerOf10(digits) == 0) return std::to_string(number);
	int dI = getFirstDigitIndex(number);
	std::string str = std::to_string(getFirstDigit(number));
	str += ".";
	str += std::to_string(getDigitAtIndex(number, dI-1)*100+getDigitAtIndex(number, dI-2)*10+getDigitAtIndex(number, dI-3));
	str += "e";
	return str.append(std::to_string(dI));
}