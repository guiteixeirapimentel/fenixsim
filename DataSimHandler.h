#pragma once
#include "RocketSim.h"

class DataSimHandler
{
public:
	DataSimHandler();
	~DataSimHandler();

	void SaveCSVPositionInfo(const std::string& filePath, const RocketSimData* pRSD, size_t numPontos = 0) const;
	//void SaveCSVVelocityInfo(const std::string& fileName, const RocketSimData& rsd) const;
	//void SaveCSVAccelerationInfo(const std::string& fileName, const RocketSimData& rsd) const;
private:

};