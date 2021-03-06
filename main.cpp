#include "CCSolver.h"
#include "Dropout.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

struct Fraction {
	Fraction() {
		isNumSet = false;
	}
	
	double numerator;
	double denominator;
	bool isNumSet;

	double getDouble() {
		return numerator / denominator;
	}
};

struct InputData {
	InputData() {}
	double a;
	double b;
	double c;
	double d;
	double e;

	int countToDrop;
	double diff;
};

InputData ReadFromFile(std::string filename) {
	InputData inputData;
	std::ifstream ifs(filename);
	if (!ifs.is_open()) {
		throw std::runtime_error("Can't open file!");
	}
	std::string tempStr;
	double tempDouble;
	std::vector<double> data;

	
	std::getline(ifs, tempStr);
	std::stringstream ss(tempStr);

	Fraction frac;
	for (int i; ss >> i;) {
		if (!frac.isNumSet) {
			frac.numerator = i;
			frac.isNumSet = true;
		}
		else {
			frac.denominator = i;
			data.push_back(frac.getDouble());
			frac.isNumSet = false;
		}
		if (ss.peek() == ';' || ss.peek() == '/') {
			ss.ignore();
		}
	}

	inputData.a = data.at(0);
	inputData.b = data.at(1);
	inputData.c = data.at(2);
	inputData.d = data.at(3);
	inputData.e = data.at(4);
	inputData.countToDrop = data.at(5);
	inputData.diff = data.at(6);

	return inputData;
}

void printInitialData(InputData data) {
	std::cout << "Kosykh A.M. IU8-104. Lab2. Variant 5" << std::endl;
	std::cout << "a = " << data.a << std::endl;
	std::cout << "b = " << data.b << std::endl;
	std::cout << "c = " << data.c << std::endl;
	std::cout << "d = " << data.d << std::endl;
	std::cout << "e = " << data.e << std::endl;
	std::cout << "Diff to drop = " << data.diff << std::endl;
	std::cout << "Count to drop = " << data.countToDrop
		<< std::endl << std::endl;
}

int main(int argc, char* argv[]) {
	InputData data = ReadFromFile(argv[1]);
	
	Dropout dropout(data.countToDrop, data.diff);
	CCSolver solver(
		(data.a), 
		(data.b), 
		(data.c),
		(data.d),
		(data.e),
		dropout
	);

	printInitialData(data);

	if (!solver.iSolveAnalytical()) {
		return 1;
	}
	solver.iPrintAnswerAnalytical();

	std::cout << std::endl;

	solver.iSolveNumerical();

	return 0;
}