#include "BRMethod.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <limits.h>
#include <fstream>
#include <time.h>


BrownRobinsonAlgorithm::BrownRobinsonAlgorithm(Matrix matrix, double error)
{
	// значения по умолчанию
	m_matrix = matrix;
	m_error = error;
	m_firstPlayerStrategy = 0;
	m_secondPlayerStrategy = 0;
	m_minAvgUpperBound = DBL_MAX;
	m_maxAvgLowerBound = -DBL_MAX;
	m_totalSteps = 0;

	for (size_t iter = 0; iter < m_matrix.size(); ++iter) {
		m_prevFirstPlayerScores.push_back(0);
	}

	for (size_t iter = 0; iter < m_matrix[m_secondPlayerStrategy].size(); ++iter) {
		m_prevSecondPlayerScores.push_back(0);
	}
}

int BrownRobinsonAlgorithm::GetFirstPlayerStrategy()
{
	// определение наилучшей стратегии первого игрока
	// при нескольких возможных стратегиях, выбирается случайная 
	std::vector<int> indexesOfMaxElements;
	double maxElement = *std::max_element(m_prevFirstPlayerScores.begin(), m_prevFirstPlayerScores.end());
	for (auto iter = m_prevFirstPlayerScores.begin(); iter != m_prevFirstPlayerScores.end(); iter++) {
		if (*iter >= maxElement) {
			indexesOfMaxElements.push_back(std::distance(m_prevFirstPlayerScores.begin(), iter));
			maxElement = *iter;
		}
	}

	srand(time(NULL));
	int randomIndex = rand() % indexesOfMaxElements.size();
	return indexesOfMaxElements[randomIndex];
}

int BrownRobinsonAlgorithm::GetSecondPlayerStrategy()
{
	// определение наилучшей стратегии второго игрока
	// при нескольких возможных стратегиях, выбирается случайная 
	std::vector<int> indexesOfMinElements;
	double minElement = *std::min_element(m_prevSecondPlayerScores.begin(), m_prevSecondPlayerScores.end());
	for (auto iter = m_prevSecondPlayerScores.begin(); iter != m_prevSecondPlayerScores.end(); iter++) {
		if (*iter <= minElement) {
			indexesOfMinElements.push_back(std::distance(m_prevSecondPlayerScores.begin(), iter));
			minElement = *iter;
		}
	}
	srand(time(NULL));
	int randomIndex = rand() % indexesOfMinElements.size();
	return indexesOfMinElements[randomIndex];
}

void BrownRobinsonAlgorithm::GetAnswer()
{
	size_t numberOfStrategies = m_matrix.size();
	
	std::map<int, size_t> firstPlayerStrategyCounts;
	for (size_t i = 0; i < numberOfStrategies; ++i) {
		firstPlayerStrategyCounts.insert(std::pair<int, size_t>(i, 0));
	}
	for (const auto& step : m_steps) {
		++firstPlayerStrategyCounts[step.firstPlayerStrategy];
	}
	for (const auto& strategy : firstPlayerStrategyCounts) {
		m_firstPlayerAnswer.push_back(static_cast<double>(strategy.second) / (static_cast<double>(m_totalSteps) + 1));
	}

	std::map<int, size_t> secondPlayerStrategyCounts;
	for (size_t i = 0; i < numberOfStrategies; ++i) {
		secondPlayerStrategyCounts.insert(std::pair<int, size_t>(i, 0));
	}
	for (const auto& step : m_steps) {
		++secondPlayerStrategyCounts[step.secondPlayerStrategy];
	}
	for (const auto& strategy : secondPlayerStrategyCounts) {
		m_secondPlayerAnswer.push_back(static_cast<double>(strategy.second) / (static_cast<double>(m_totalSteps) + 1));
	}
	m_gameValue = (m_minAvgUpperBound + m_maxAvgLowerBound) / 2;
}

void BrownRobinsonAlgorithm::Solve()
{
	int stepIter = 0;
	while(true) {
		// создание объекта "Шаг"
		Step currentStep(stepIter);

		// сохранение вычисленного номера стратегии для каждого из игроков из предыдущего шага
		currentStep.firstPlayerStrategy = m_firstPlayerStrategy;
		currentStep.secondPlayerStrategy = m_secondPlayerStrategy;

		// прибавление к текущим значениям выигрыша выбранной стратегии
		for (size_t iter = 0; iter < m_matrix.size(); ++iter) {
			currentStep.firstPlayerScores.push_back(m_matrix[iter][currentStep.secondPlayerStrategy] + m_prevFirstPlayerScores[iter]);
		}
		for (size_t iter = 0; iter < m_matrix[currentStep.secondPlayerStrategy].size(); ++iter) {
			currentStep.secondPlayerScores.push_back(m_matrix[currentStep.firstPlayerStrategy][iter] + m_prevSecondPlayerScores[iter]);
		}

		// вычисление оценок верхней и нижней цен игры
		currentStep.avgUpperBound = *std::max_element(currentStep.firstPlayerScores.begin(), currentStep.firstPlayerScores.end()) / static_cast<double>(stepIter+1);
		currentStep.avgLowerBound = *std::min_element(currentStep.secondPlayerScores.begin(), currentStep.secondPlayerScores.end()) / static_cast<double>(stepIter+1);
		
		// обновление максимальной нижней оценки и минимальной верхней
		if (currentStep.avgUpperBound <= m_minAvgUpperBound) {
			m_minAvgUpperBound = currentStep.avgUpperBound;
		}
		if (currentStep.avgLowerBound >= m_maxAvgLowerBound) {
			m_maxAvgLowerBound = currentStep.avgLowerBound;
		}

		// вычисление ошибки
		currentStep.error = m_minAvgUpperBound - m_maxAvgLowerBound;

		// сохранание шага
		m_steps.push_back(currentStep);

		// выход из цикла при преодолении порога погрешнности
		if (abs(currentStep.error) < m_error) {
			m_totalSteps = currentStep.stepNumber;
			m_finalError = currentStep.error;
			break;
		}
		// иначе....
		// обновление текущих выигрышей игроков и вычисление наилучшей стратегии
		else {
			m_prevFirstPlayerScores = currentStep.firstPlayerScores;
			m_prevSecondPlayerScores = currentStep.secondPlayerScores;

			m_firstPlayerStrategy = GetFirstPlayerStrategy();
			m_secondPlayerStrategy = GetSecondPlayerStrategy();
		}
		stepIter++;
	}
	GetAnswer(); // сохранение ответа
}

bool BrownRobinsonAlgorithm::SaveTableToFile()
{
	std::ofstream file;
	file.open("BRTable.csv");
	if (!file.is_open()) {
		return false;
	}

	for (const auto& step : m_steps) {
		file << "=\"" << step.stepNumber + 1 << "\";"
			<< "=\"" << step.firstPlayerStrategy + 1 << "\";"
			<< "=\"" << step.secondPlayerStrategy + 1 << "\";";
		for (const auto& value : step.firstPlayerScores) {
			file << "=\"" << value << "\";";
		}
		for (const auto& value : step.secondPlayerScores) {
			file << "=\"" << value << "\";";
		}
		file << "=\"" << step.avgUpperBound << "\";"
			<< "=\"" << step.avgLowerBound << "\";"
			<< "=\"" << step.error << "\";" << std::endl;
	}
	return true;
}

void BrownRobinsonAlgorithm::iSolve()
{
	Solve();
}

void BrownRobinsonAlgorithm::iPrintAnswer()
{
	std::cout << "*****Brown-Robinson method*****" << std::endl;
	for (auto& step : m_steps) {
		step.Print();
	}
	std::cout << "x[" << m_steps.size() << "]:{";
	for (const auto firstPlayerStrategy : m_firstPlayerAnswer) {
		std::cout << firstPlayerStrategy << " ";
	}
	std::cout << "}" << std::endl;

	std::cout << "y[" << m_steps.size() << "]:{";
	for (const auto secondPlayerStrategy : m_secondPlayerAnswer) {
		std::cout << secondPlayerStrategy << " ";
	}
	std::cout << "}" << std::endl;
	std::cout << "Error: " << m_finalError << std::endl;
	std::cout << "Game value:" << m_gameValue << std::endl;
	std::cout << std::endl;
}

void BrownRobinsonAlgorithm::iPrintMatrix()
{
	for (const auto& row : m_matrix) {
		for (const auto& element : row) {
			std::cout << std::setprecision(4) << element << " ";
		}
		std::cout << std::endl;
	}
}

void BrownRobinsonAlgorithm::iSavePlots()
{
	//SaveTableToFile();
}

double BrownRobinsonAlgorithm::iGetGameValue()
{
	return m_gameValue;
}

double BrownRobinsonAlgorithm::iGetError()
{
	return m_error;
}

std::vector<double>* BrownRobinsonAlgorithm::iGetFirstPlayerAnswer()
{
	return &m_firstPlayerAnswer;
}

std::vector<double>* BrownRobinsonAlgorithm::iGetSecondPlayerAnswer()
{
	return &m_secondPlayerAnswer;
}
