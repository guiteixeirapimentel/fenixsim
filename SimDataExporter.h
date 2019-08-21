#pragma once
#include "RocketSim.h"

class SimDataExporter
{
public:
	SimDataExporter();
	~SimDataExporter();

	void ExportLinearPositions(const RocketSimData& rocketSimData, const std::string& filename) const;
	void ExportLinearAccelerations(const RocketSimData& rocketSimData, const std::string& filename) const;
	void ExportLinearVelocities(const RocketSimData& rocketSimData, const std::string& filename) const;

	void ExportAtitudes(const RocketSimData& rocketSimData, const std::string& filename) const;
	void ExportAngularAccelerations(const RocketSimData& rocketSimData, const std::string& filename) const;
	void ExportAngularVelocities(const RocketSimData& rocketSimData, const std::string& filename) const;

private:
	void SaveDataToFile(const std::ostringstream& buf, const std::string& filename) const;
};