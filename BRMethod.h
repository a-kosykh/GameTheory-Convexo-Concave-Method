#pragma once
#include "BRStep.h"

using Matrix = std::vector<std::vector<double>>;

class BrownRobinsonAlgorithm {

private:
	Matrix m_matrix; // �������� �������

	std::vector<Step>	m_steps; // ������ ����� ������
	std::vector<double>	m_prevFirstPlayerScores; // ������� �������� ������� ������
	std::vector<double>	m_prevSecondPlayerScores; // ������� �������� ������� ������
	int					m_firstPlayerStrategy; // ������� ��������� ������� ������
	int					m_secondPlayerStrategy;  // ������� ��������� ������� ������
	double				m_error; // ������� ������������

	double				m_finalError; // �������� �����������
	double				m_minAvgUpperBound; // ����������� ������� �������� ������� ������ ���� ����
	double				m_maxAvgLowerBound; // ������������ ������� �������� ������ ������ ���� ����
	double				m_gameValue; // ���� ����
	int					m_totalSteps; // �������� ���������� ����� ���������

	std::vector<double> m_firstPlayerAnswer; // ��������� ��������� 1��� ������
	std::vector<double> m_secondPlayerAnswer; // ��������� ��������� 2��� ������

	int		GetFirstPlayerStrategy(); // ���������� ��������� ��������� 1��� ������
	int		GetSecondPlayerStrategy(); // ���������� ��������� ��������� 2��� ������

	void	Solve(); // ������ ���������
	void	GetAnswer(); // ��������� ������
	bool	SaveTableToFile(); // ���������� ������� ������

	
public:
	BrownRobinsonAlgorithm(Matrix matrix, double error);
	~BrownRobinsonAlgorithm() {}

	void iSolve(); // ������ ���������
	void iPrintAnswer(); // ����� ���������� � �������
	void iPrintMatrix();
	void iSavePlots(); // ���������� �������� � ����

	double iGetGameValue();
	double iGetError();
	std::vector<double>* iGetFirstPlayerAnswer();
	std::vector<double>* iGetSecondPlayerAnswer();
};