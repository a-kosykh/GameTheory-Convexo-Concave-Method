#pragma once
#include "BRMethod.h"
#include "Dropout.h"
#include <iostream>

struct SaddlePointAnswer {
	double x;
	double y;
	double saddlePoint;
};

class CCSolver {
private:
	double a;
	double b;
	double c;
	double d;
	double e;
	double x;
	double y;
	double saddlePoint;

	double xNum;
	double yNum;
	double saddlePointNum;
	std::vector<double> firstPlayerStrategy;
	std::vector<double> secondPlayerStrategy;

	bool CheckDerivative(double a, double b);
	double GetX(double y);
	double GetY(double x);
	double GetY();
	double GetSaddlePoint(double x, double y);

	Matrix	CreateWinningGrid(int size);
	bool	IsSaddlePointExists(Matrix mat);
	bool	SolveWithBRMethod(Matrix mat, double error);
	void	PrintCurrentAnswer(Matrix& mat);
	void	PrintStrategies();

	Dropout dropout;

	void	PrintMatrix(Matrix &m);

public:
	CCSolver(
		double a,
		double b,
		double c,
		double d,
		double e,
		Dropout dr) : a(a), b(b), c(c), d(d), e(e)
	{
		x = 0;
		y = 0;
		saddlePoint = 0;
		dropout = dr;
	}
	
	bool iSolveAnalytical();
	bool iSolveNumerical();
	void iPrintAnswerAnalytical();

};