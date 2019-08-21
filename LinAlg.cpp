#include "LinAlg.h"

void MATRIXTRANS::RotationFromAxisNormalized(double theta, const VETOR& axis)
{
		const double costheta = cos(theta);
		const double sintheta = sin(theta);
		const double k1 = 1.0 - costheta;

		cPComp[0][0] = costheta + (axis.x * axis.x * k1);
		cPComp[0][1] = (axis.x * axis.y * k1) - (axis.z*sintheta);
		cPComp[0][2] = (axis.x * axis.z * k1) + (axis.y*sintheta);

		cPComp[1][0] = (axis.y * axis.x * k1) + (axis.z * sintheta);
		cPComp[1][1] = costheta + (axis.y * axis.y * k1);
		cPComp[1][2] = (axis.y * axis.z * k1) - (axis.x * sintheta);

		cPComp[2][0] = (axis.z*axis.x*k1) - (axis.y*sintheta);
		cPComp[2][1] = (axis.z*axis.y*k1) + (axis.x*sintheta);
		cPComp[2][2] = costheta + (axis.z*axis.z*k1);
}

void MATRIXTRANS::RotationFromAxis(double theta, const VETOR& axis)
{
	const double modAxis = Modulus(axis);
	VETOR axisNormal = axis / modAxis;

	RotationFromAxisNormalized(theta, axisNormal);
}