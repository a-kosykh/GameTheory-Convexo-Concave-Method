#pragma once
#include <vector>


// ����� "��� ��������� ������-��������"
class Step {
private:

public:
	Step(int stepNumber): stepNumber(stepNumber) {}
	~Step() {}

	int					stepNumber;				// ����� ����
	int					firstPlayerStrategy;	// ��������� 1��� ������
	int					secondPlayerStrategy;	// ��������� 2��� ������
	std::vector<double>	firstPlayerScores;		// ������ �������� 1��� ������
	std::vector<double>	secondPlayerScores;		// ������ �������� 2��� ������
	double				avgUpperBound;			// ������� ������� ���� ����
	double				avgLowerBound;			// ������ ������� ���� ����
	double				error;					// ������������

	void Print(); // ����� ���� �� �����
};