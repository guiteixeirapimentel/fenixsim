#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

struct VETOR;

class MATRIXTRANS
{
public:
	MATRIXTRANS(const MATRIXTRANS& mt)
	{
		std::memcpy(cPComp, mt.cPComp, sizeof(double) * 9);
	}
	MATRIXTRANS(const double* pVals)
	{
		std::memcpy(cPComp, pVals, sizeof(double) * 9);
	}
	MATRIXTRANS()
	{
		
	}

	void Zero()
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cPComp[i][j] = 0.0;
			}
		}
	}

	void Identity()
	{
		cPComp[0][0] = 1.0;
		cPComp[0][1] = 0.0;
		cPComp[0][2] = 0.0;

		cPComp[1][0] = 0.0;
		cPComp[1][1] = 1.0;
		cPComp[1][2] = 0.0;

		cPComp[2][0] = 0.0;
		cPComp[2][1] = 0.0;
		cPComp[2][2] = 1.0;
	}
	void RotationFromAxisNormalized(double theta, const VETOR& axis);
	
	void RotationFromAxis(double theta, const VETOR& axis);

public:
	double cPComp[3][3];
};

struct VETOR
{
	double x;
	double y;
	double z;
public:
	VETOR operator /(double rhs) const
	{
		return { x / rhs, y / rhs, z / rhs };
	}

	VETOR& operator/=(double rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	VETOR operator *(double rhs)const
	{
		return { x * rhs, y * rhs, z * rhs };
		return *this;
	}

	VETOR& operator*=(double rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	VETOR operator+(const VETOR& rhs) const
	{
		return { x + rhs.x, y + rhs.y, z + rhs.z };
	}

	VETOR& operator+=(const VETOR& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	VETOR operator-(const VETOR& rhs) const
	{
		return { x - rhs.x, y - rhs.y, z - rhs.z };
	}

	VETOR& operator-=(const VETOR& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	VETOR& operator*=(const MATRIXTRANS& rhs)
	{
		double xx = (rhs.cPComp[0][0] * x) + (rhs.cPComp[0][1] * y) + (rhs.cPComp[0][2] * z);
		double yy = (rhs.cPComp[1][0] * x) + (rhs.cPComp[1][1] * y) + (rhs.cPComp[1][2] * z);
		double zz = (rhs.cPComp[2][0] * x) + (rhs.cPComp[2][1] * y) + (rhs.cPComp[2][2] * z);

		x = xx;
		y = yy;
		z = zz;
		return *this;
	}

	VETOR operator*(const MATRIXTRANS& rhs) const
	{
		VETOR res = { (rhs.cPComp[0][0] * x) + (rhs.cPComp[0][1] * y) + (rhs.cPComp[0][2] * z),
		(rhs.cPComp[1][0] * x) + (rhs.cPComp[1][1] * y) + (rhs.cPComp[1][2] * z),
		(rhs.cPComp[2][0] * x) + (rhs.cPComp[2][1] * y) + (rhs.cPComp[2][2] * z) };

		return res;
	}

};

inline double ModulusSquared(const VETOR& v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}


inline double Modulus(const VETOR& v)
{
	return sqrt(ModulusSquared(v));
}


inline double DotProduct(const VETOR& v1, const VETOR& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

inline double AngleBetween(const VETOR& v1, const VETOR& v2)
{
	double dot = DotProduct(v1, v2) / (Modulus(v1) * Modulus(v2));

	if (dot > 1.0)
		dot = 1.0;
	else if (dot < -1.0)
		dot = -1.0;

	return acos(dot);
}

inline double AngleBetweenNormalized(const VETOR& v1, const VETOR& v2)
{	
	double dot = DotProduct(v1, v2);
	if (dot > 1.0)
		dot = 1.0;
	else if (dot < -1.0)
		dot = -1.0;

	return acos(dot);
}


inline VETOR CrossProduct(const VETOR& u, const VETOR& v)
{
	VETOR res = { ((u.y * v.z) - (u.z * v.y)), ((u.z*v.x) - (u.x*v.z)), ((u.x*v.y) - (u.y*v.x)) };
	
	return res;
}