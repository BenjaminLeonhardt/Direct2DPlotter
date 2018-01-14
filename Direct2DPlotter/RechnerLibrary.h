#pragma once
#include "Direct2DPlotter.h"
class RechnerLibrary
{
private:

public:
	RechnerLibrary();
	~RechnerLibrary();

	double f(double x, vector<double> *funktion);

	void mitternachtsformel();
	void pqFormel();
	void newtonVerfahren();
	void regulaFalsiVerfahren();
	void extremstellenBerechnen();
	void wendepunkteBerechnen();

	void parseFuntionBuffer(FunktionSyntaxbaum& neuerFunktionSyntaxbaum, WCHAR* functionAlsString);
	void rechterPart(int index, int vorzeichenLevel, int linkeGrenze, int rechteGrenze, FunktionSyntaxbaum *aktuellerKnoten, WCHAR* functionAlsStringLokal);
	void linkerPart(int index, int vorzeichenLevel, int linkeGrenze, int rechteGrenze, FunktionSyntaxbaum *aktuellerKnoten, WCHAR* functionAlsStringLokal);
	void ausSyntaxbaumVektorErstellen(FunktionSyntaxbaum* aktuellerKnoten, vector<double>& functionAlsVectorLokal);
	void setFunctionBuffer(TCHAR* functionBuffer);
	TCHAR* getFunctionBuffer();
	vector<double>& getErsteAbleitungAlsVector();
	vector<double>& getFunktionAlsVector();
	vector<double>& getStammfunktionAlsVector();
	vector<Vector2D>& getNullstellen();
	vector<Vector2D>& getExtremstellen();
	vector<Vector2D>& getWendepunkte();
	void resetVals();
	FunktionSyntaxbaum getSyntaxbaum();
	void syntaxbaumErstellen();
	vector<double>& funktionAbleiten(vector<double> &funktion);
	void setErsteAbleitung(vector<double> &funktion);
	void setZweiteAbleitung(vector<double> &funktion);
	void setDritteAbleitung(vector<double> &funktion);
	vector<double> RechnerLibrary::getErsteAbleitung();
	vector<double> RechnerLibrary::getZweiteAbleitung();
	vector<double> RechnerLibrary::getDritteAbleitung();
	void integrieren(vector<double> &funktion);
	double flaecheninhaltBerechnen(double linkeGrenze, double rechteGrenze);
	double flaecheninhaltMitTrapetzBerechnen(double linkeGrenze, double rechteGrenze);
	double flaecheninhaltMitSimpsonBerechnen(double linkeGrenze, double rechteGrenze);
	double bogenlaengeMitTrapetzBerechnen(double linkeGrenze, double rechteGrenze);
	void setSyntaxbaum(FunktionSyntaxbaum& neuerFunktionSyntaxbaum);
	vector<vector<double>> polynomdivision(vector<double>& zaehler, vector<double>& nenner);
};

