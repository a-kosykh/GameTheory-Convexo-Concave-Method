#include "CCSolver.h"
#include <algorithm>
#include <iomanip>

bool CCSolver::CheckDerivative(double a, double b)
{
	if (2 * a >= 0 || 2 * b <= 0) {
		return false;
	}
	return true;
}

double CCSolver::GetX(double y)
{
	return -(((c * y) + d) / (a * 2));
}

double CCSolver::GetY(double x)
{
	
	return -(((c * x) + e) / (b * 2));
}

double CCSolver::GetY()
{
	return (d * c - 2 * e * a) / (4 * a * b - c * c);
}

double CCSolver::GetSaddlePoint(double x, double y)
{
	return (a * x * x + b * y * y + c * x * y + d * x + e * y);
}

Matrix CCSolver::CreateWinningGrid(int size)
{
	Matrix matrix;	
	for (size_t row = 0; row < size + 1; row++) {
		std::vector<double> rowVect;
		for (size_t column = 0; column < size + 1; column++) {
			rowVect.push_back(GetSaddlePoint(
				static_cast<double>(row) / static_cast<double>(size),
				static_cast<double>(column) / static_cast<double>(size)));
		}
		matrix.push_back(rowVect);
	}
	return matrix;
}

bool CCSolver::IsSaddlePointExists(Matrix mat)
{
	if (mat.size() < 2) return false;
	
	double maxMin = -DBL_MAX;
	double minMax = DBL_MAX;
	int maxMinIndex = -1;
	int minMaxIndex = -1;

	for (size_t row = 0; row < mat.size(); row++) {
		double maxMinTemp = *std::min_element(mat[row].begin(), mat[row].end());
		if (maxMinTemp > maxMin) {
			maxMin = maxMinTemp;
			maxMinIndex = row;
		}
	}

	std::vector<double> maxs;
	for (size_t column = 0; column < mat.size(); column++) {
		double maxOfColumn = -DBL_MAX;
		for (size_t row = 0; row < mat.size(); row++) {
			if (mat[row][column] > maxOfColumn) {
				maxOfColumn = mat[row][column];
			}
		}
		maxs.push_back(maxOfColumn);
	}
	minMax = *std::min_element(maxs.begin(), maxs.end());
	minMaxIndex = std::distance(maxs.begin(), std::min_element(maxs.begin(), maxs.end()));

	if (minMax == maxMin) {
		saddlePointNum = minMax;
		xNum = (minMax / mat.size());
		yNum = (maxMin / mat.size());
		firstPlayerStrategy = {}; // TODO
		secondPlayerStrategy = {}; // TODO
		return true;
	}

	return false;
}

void CCSolver::PrintCurrentAnswer()
{
	std::cout << "Game value: " 
		<< std::setprecision(4) 
		<< saddlePointNum << std::endl;
	
	std::cout << "x: {";
	for (const auto& strategy : firstPlayerStrategy) {
		std::cout << std::setprecision(2) << strategy << " ";
	}
	std::cout << "}" << std::endl;

	std::cout << "y: {";
	for (const auto& strategy : secondPlayerStrategy) {
		std::cout << std::setprecision(2) << strategy << " ";
	}
	std::cout << "}" << std::endl << std::endl;

}

void CCSolver::PrintMatrix(Matrix& m)
{
	for (const auto& row : m) {
		for (const auto& element : row) {
			std::cout << std::setprecision(4) << element << " ";
		}
		std::cout << std::endl;
	}
}



bool CCSolver::SolveWithBRMethod(Matrix mat, double error)
{
	BrownRobinsonAlgorithm br(mat, 0.01);
	br.iSolve();
	saddlePointNum = br.iGetGameValue();
	firstPlayerStrategy = *br.iGetFirstPlayerAnswer();
	secondPlayerStrategy = *br.iGetSecondPlayerAnswer();

	return true;
}

bool CCSolver::iSolveAnalytical()
{
	y = GetY();
	x = GetX(y);
	saddlePoint = GetSaddlePoint(x, y);

	return true;
}

bool CCSolver::iSolveNumerical()
{
	int currentStep = 2;
	
	std::cout << "****Numerical Solution****" << std::endl;

	while (!dropout.recalculate(saddlePointNum)) {
		Matrix mat = CreateWinningGrid(currentStep);
		std::cout << "step #" << currentStep << ": " << std::endl;
		if (!IsSaddlePointExists(mat)) {
			std::cout << "No saddle point: " << std::endl;
			SolveWithBRMethod(mat, 0.01);
		}
		PrintMatrix(mat);
		PrintCurrentAnswer();
		currentStep++;
	}
	std::cout << "Dropout happened!" << std::endl;
	std::cout << "For " << dropout.getCountToDrop()
		<< " iterations game value diff is less than " << dropout.getDiff()
		<< std::endl;
	return false;
}

void CCSolver::iPrintAnswerAnalytical()
{
	std::cout << "****Analytical Solution****" << std::endl;
	
	std::cout << "Kernel: " <<
		a << "*x^2 + " <<
		b << "*b^2 + " <<
		c << "*xy + " <<
		d << "*x + " <<
		e << "*y" << std::endl;

	std::cout << "f1(y) = -(" <<
		c << "*y+" << d << ") / " <<
		2 * a << std::endl;
	std::cout << "f2(x) = -(" <<
		c << "*x+" << e << ") / " <<
		2 * b << std::endl;

	std::cout << "x = " << x << std::endl;
	std::cout << "y = " << y << std::endl;

	std::cout << "H(" << x << "," << y << ") = "
		<< saddlePoint << std::endl;
}


