/*!
 \file PartGrnd_ROOT.cc

 \brief implementation file for 
 \author Sylvie Dagoret-Campagne PartGrnd_ROOT.h
 \date april 2001
*/


#include "PartGrnd_ROOT.h"
#include "Globals.h"


ClassImp(PartGrnd_ROOT)


Int_t PartGrnd_ROOT::GetId()
{
  return (Int_t) Id;
}

Float_t PartGrnd_ROOT::GetUX()
{
	return (Float_t) UX;
};

Float_t PartGrnd_ROOT::GetUY()
{
	return (Float_t) UY;
};

Float_t PartGrnd_ROOT::GetUZ()
{
	return (Float_t) UZ;
};

Float_t PartGrnd_ROOT::GetX()
{
	return (Float_t) X;
};

Float_t PartGrnd_ROOT::GetY()
{
	return (Float_t) Y;
};

Float_t PartGrnd_ROOT::GetZ()
{
	return (Float_t) Z;
};

Float_t PartGrnd_ROOT::GetT()
{
	return (Float_t) T;
};

Float_t PartGrnd_ROOT::GetW()
{
	return (Float_t) W;

};

Float_t PartGrnd_ROOT::GetE()
{
	return (Float_t) E;
};

Float_t PartGrnd_ROOT::Phi()
{
	return (Float_t) atan2(Y, X);
};

Float_t PartGrnd_ROOT::GetPhiDeg()
{
	return RAD2DEG * Phi();
};

Float_t PartGrnd_ROOT::GetUR()
{
	return sqrt(UX * UX + UY * UY);
};

Float_t PartGrnd_ROOT::Theta()
{
	Float_t Pz = GetUZ();

	if (Pz == 0)
		return PI / 2.;
	else
		return (atan2(GetUR(), Pz));
};

Float_t PartGrnd_ROOT::GetCosTheta()
{
	return TMath::Cos(this->Theta());
};


Float_t PartGrnd_ROOT::GetThetaDeg()
{
	return RAD2DEG * Theta();
};

Float_t PartGrnd_ROOT::Azim()
{
	return atan2(UY, UX);
};

Float_t PartGrnd_ROOT::GetAzimDeg()
{
	return RAD2DEG * Azim();
};

Float_t PartGrnd_ROOT::R()
{
	return (Float_t) sqrt(X * X + Y * Y);
};

Float_t PartGrnd_ROOT::GetR()
{
	return R();
};

Float_t PartGrnd_ROOT::LogR()
{
	return (Float_t) TMath::Log10(R());
};

Float_t PartGrnd_ROOT::LogW()
{
	return TMath::Log10(GetW());
};

Float_t PartGrnd_ROOT::LogE()
{
	return (Float_t) TMath::Log10(GetE());
};

Float_t PartGrnd_ROOT::LogT()
{
	return (Float_t) TMath::Log10(GetT());
};
//-------------------------------------------------------
void PartGrnd_ROOT::Print(ostream& s) const
{
  s << "PartGrnd_ROOT:: Id = "<< Id << endl;
  s << " UX UY UZ " << UX <<" " << UY <<" "<< UZ << endl;
  s << " X Y Z " << X <<" " << Y <<" "<< Z << endl;
  s << " T W E " << T <<" " << W <<" "<< E << endl;
  s << endl;
}

//-------------------------------------------------------
ostream& operator<<( ostream& s, const PartGrnd_ROOT& p)
{
  p.Print(s);
  return s;
}
//-------------------------------------------------------



