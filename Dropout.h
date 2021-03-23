#pragma once
#include <cmath>

class Dropout {
private:
	double m_currentGameValue;
	double m_prevGameValue;
	int m_dropoutCount;

	int m_countToDrop;
	double m_diff;

public:
	Dropout() {}
	Dropout(int countToDrop, double diff) {
		m_countToDrop = countToDrop;
		m_diff = diff;

		m_currentGameValue = 0;
		m_prevGameValue = 0;
		m_dropoutCount = 0;
	}
	bool recalculate(double newCurrentGameValue) {
		m_prevGameValue = m_currentGameValue;
		m_currentGameValue = newCurrentGameValue;

		if (abs(newCurrentGameValue - m_prevGameValue) < m_diff) {
			m_dropoutCount++;
		}
		else {
			m_dropoutCount == 0;
		}
		
		if (m_dropoutCount == m_countToDrop) {
			return true;
		}
		else return false;
	}
	const int getCountToDrop() {
		return m_countToDrop;
	}
	const double getDiff() {
		return m_diff;
	}
};