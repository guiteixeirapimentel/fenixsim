#include "RocketData.h"
#include <fstream>

RocketData::RocketData()
{}

RocketData::~RocketData()
{}

bool RocketData::Open(const std::string& filePathName)
{
	std::ifstream arq(filePathName, std::ios::binary);

	if (!arq.is_open())
	{
		return false;
	}
	arq.read(reinterpret_cast<char*>(&cData), sizeof(RD));

	const size_t lenNomeFoguete = cData.cNomeFogueteSize;
	const size_t lenPathFileMotor = cData.cPathFileMotorSize;

	char *buf = new char[lenNomeFoguete + 1];
	buf[lenNomeFoguete] = '\0';

	arq.read(buf, lenNomeFoguete);
	cNomeFoguete = (const char*)buf;

	delete[]buf;

	buf = new char[lenPathFileMotor + 1];
	buf[lenPathFileMotor] = '\0';

	arq.read(buf, lenPathFileMotor);
	cPathFileMotor = (const char*)buf;

	delete[] buf;
	buf = nullptr;
	arq.close();

	return true;
}

void RocketData::Save(const std::string& filePathName)
{
	std::ofstream arq(filePathName, std::ios::binary);

	cData.cPathFileMotorSize = cPathFileMotor.length();
	cData.cNomeFogueteSize = cNomeFoguete.length();

	arq.write(reinterpret_cast<const char*>(&cData), sizeof(RD));

	arq.write(cNomeFoguete.c_str(), cNomeFoguete.length());
	arq.write(cPathFileMotor.c_str(), cPathFileMotor.length());
	arq.close();
}