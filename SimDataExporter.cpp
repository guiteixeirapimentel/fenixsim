#include "SimDataExporter.h"
#include <sstream>
#include <fstream>

#include <locale>

template <class charT, charT sep>
class Punct_facet : public std::numpunct<charT> {
protected:
	charT do_decimal_point() const { return sep; }
};

SimDataExporter::SimDataExporter()
{}

SimDataExporter::~SimDataExporter()
{}

void SimDataExporter::ExportLinearPositions(const RocketSimData& rocketSimData, const std::string& filename) const
{
	std::ostringstream outBuffer;

	outBuffer.imbue(std::locale(outBuffer.getloc(), new Punct_facet<char, ','>));

	outBuffer << "#Arquivo gerado por simulador da Fenix\n";

	outBuffer << "Tempo;Pos X;Pos Y;Pos Z;\n";

	const size_t deltaI = static_cast<size_t>(0.01 / rocketSimData.cDeltaTime);

	for (size_t i = 0; i + deltaI < rocketSimData.cPosicoes.size(); i += deltaI)
	{
		outBuffer << rocketSimData.cDeltaTime * static_cast<double>(i) << ";"
			<< rocketSimData.cPosicoes[i].x << ";" << 
			rocketSimData.cPosicoes[i].y << ";" << rocketSimData.cPosicoes[i].z << ";\n";
	}

	SaveDataToFile(outBuffer, filename);
}

void SimDataExporter::ExportLinearAccelerations(const RocketSimData& rocketSimData, const std::string& filename) const
{
	std::ostringstream outBuffer;

	outBuffer.imbue(std::locale(outBuffer.getloc(), new Punct_facet<char, ','>));

	outBuffer << "#Arquivo gerado por simulador da Fenix\n";

	outBuffer << "Tempo;Acel X;Acel Y;Acel Z;\n";

	const size_t deltaI = static_cast<size_t>(0.01 / rocketSimData.cDeltaTime);

	for (size_t i = 0; i + deltaI < rocketSimData.cPosicoes.size(); i += deltaI)
	{
		outBuffer << rocketSimData.cDeltaTime * static_cast<double>(i) << ";"
			<< rocketSimData.cAceleracoes[i].x << ";" <<
			rocketSimData.cAceleracoes[i].y << ";" << rocketSimData.cAceleracoes[i].z << ";\n";
	}
	
	SaveDataToFile(outBuffer, filename);
}

void SimDataExporter::ExportLinearVelocities(const RocketSimData& rocketSimData, const std::string& filename) const
{
	std::ostringstream outBuffer;

	outBuffer.imbue(std::locale(outBuffer.getloc(), new Punct_facet<char, ','>));

	outBuffer << "#Arquivo gerado por simulador da Fenix\n";

	outBuffer << "Tempo;Vel X;Vel Y;Vel Z;\n";
	
	const size_t deltaI = static_cast<size_t>(0.01 / rocketSimData.cDeltaTime);

	for (size_t i = 0; i + deltaI < rocketSimData.cPosicoes.size(); i += deltaI)
	{
		outBuffer << rocketSimData.cDeltaTime * static_cast<double>(i) << ";"
			<< rocketSimData.cVelocidades[i].x << ";" <<
			rocketSimData.cVelocidades[i].y << ";" << rocketSimData.cVelocidades[i].z << ";\n";
	}

	SaveDataToFile(outBuffer, filename);
}

void SimDataExporter::ExportAtitudes(const RocketSimData& rocketSimData, const std::string& filename) const
{
	std::ostringstream outBuffer;

	outBuffer.imbue(std::locale(outBuffer.getloc(), new Punct_facet<char, ','>));

	outBuffer << "#Arquivo gerado por simulador da Fenix\n";

	outBuffer << "Tempo;Ati X;Ati Y;Ati Z;\n";

	const size_t deltaI = static_cast<size_t>(0.01 / rocketSimData.cDeltaTime);

	for (size_t i = 0; i + deltaI < rocketSimData.cPosicoes.size(); i += deltaI)
	{
		outBuffer << rocketSimData.cDeltaTime * static_cast<double>(i) << ";"
			<< rocketSimData.cAtitudes[i].x << ";" <<
			rocketSimData.cAtitudes[i].y << ";" << rocketSimData.cAtitudes[i].z << ";\n";
	}

	SaveDataToFile(outBuffer, filename);
}

void SimDataExporter::ExportAngularAccelerations(const RocketSimData& rocketSimData, const std::string& filename) const
{
	std::ostringstream outBuffer;

	outBuffer.imbue(std::locale(outBuffer.getloc(), new Punct_facet<char, ','>));

	outBuffer << "#Arquivo gerado por simulador da Fenix\n";

	outBuffer << "Tempo;AngAcel X;AngAcel Y;AngAcel Z;\n";
	
	const size_t deltaI = static_cast<size_t>(0.01 / rocketSimData.cDeltaTime);

	for (size_t i = 0; i + deltaI < rocketSimData.cPosicoes.size(); i += deltaI)
	{
		outBuffer << rocketSimData.cDeltaTime * static_cast<double>(i) << ";"
			<< rocketSimData.cAceleracoesAngulares[i].x << ";" <<
			rocketSimData.cAceleracoesAngulares[i].y << ";" << rocketSimData.cAceleracoesAngulares[i].z << ";\n";
	}

	SaveDataToFile(outBuffer, filename);
}

void SimDataExporter::ExportAngularVelocities(const RocketSimData& rocketSimData, const std::string& filename) const
{
	std::ostringstream outBuffer;

	outBuffer.imbue(std::locale(outBuffer.getloc(), new Punct_facet<char, ','>));

	outBuffer << "#Arquivo gerado por simulador da Fenix\n";

	outBuffer << "Tempo;VelAng X;VelAng Y;VelAng Z;\n";
	
	const size_t deltaI = static_cast<size_t>(0.01 / rocketSimData.cDeltaTime);

	for (size_t i = 0; i + deltaI < rocketSimData.cPosicoes.size(); i += deltaI)
	{
		outBuffer << rocketSimData.cDeltaTime * static_cast<double>(i) << ";"
			<< rocketSimData.cVelocidadesAngulares[i].x << ";" <<
			rocketSimData.cVelocidadesAngulares[i].y << ";" << rocketSimData.cVelocidadesAngulares[i].z << ";\n";
	}

	SaveDataToFile(outBuffer, filename);
}

void SimDataExporter::SaveDataToFile(const std::ostringstream& outBuffer, const std::string& filename) const
{
	std::ofstream arq(filename, std::ios::binary);

	if (arq.is_open())
	{
		const std::string buf = outBuffer.str();
		arq.write(buf.c_str(), buf.size());
	}

	arq.close();
}