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
	void mitternachtsformelGebrochenRational();
	vector<Vector2D> mitternachtsformelGebrochenRationalFunktion(vector<double> funktion);
	vector<Vector2D> pqFormelGebrochenRationalFunktion(vector<double> funktion);
	void pqFormelGebrochenRational();
	void pqFormel();
	void newtonVerfahren();
	void newtonVerfahrenGebrochenRational();
	void regulaFalsiVerfahren();
	void regulaFalsiVerfahrenGebrochenRational();
	void extremstellenBerechnen();
	void extremstellenBerechnenGebrochenRational();
	void wendepunkteBerechnen();
	void wendepunkteBerechnenGebrochenRational();

	void parseFuntionBuffer(FunktionSyntaxbaum& neuerFunktionSyntaxbaum, WCHAR* functionAlsString);
	void parseFuntionBufferGebrochenRational(FunktionSyntaxbaum& neuerFunktionSyntaxbaum, WCHAR* functionAlsString);
	void splitFuntionBufferGebrochenRational(FunktionAlsVektorSyntaxbaum& neuerFunktionSyntaxbaumGebrochenRational, WCHAR* functionAlsString);
	void rechterPart(int index, int vorzeichenLevel, int linkeGrenze, int rechteGrenze, FunktionSyntaxbaum *aktuellerKnoten, WCHAR* functionAlsStringLokal);
	void linkerPart(int index, int vorzeichenLevel, int linkeGrenze, int rechteGrenze, FunktionSyntaxbaum *aktuellerKnoten, WCHAR* functionAlsStringLokal);
	void rechterPartGebrochenRational(int index, int vorzeichenLevel, FunktionAlsVektorSyntaxbaum *aktuellerKnoten, WCHAR* functionAlsStringLokal);
	void linkerPartGebrochenRational(int index, int vorzeichenLevel, FunktionAlsVektorSyntaxbaum *aktuellerKnoten, WCHAR* functionAlsStringLokal);
	void ausSyntaxbaumVektorErstellen(FunktionSyntaxbaum* aktuellerKnoten, vector<double>& functionAlsVectorLokal);
	void setFunctionBuffer(TCHAR* functionBuffer);
	TCHAR* getFunctionBuffer();
	vector<double>& getErsteAbleitungAlsVector(); 
	vector<double>& getFunktionAlsVector();
	vector<double>& getStammfunktionAlsVector();
	vector<Vector2D>& getNullstellen();
	vector<Vector2D>& getPolstellen();
	vector<Vector2D>& getExtremstellen();
	vector<Vector2D>& getWendepunkte();
	void resetVals();
	FunktionSyntaxbaum getSyntaxbaum();
	void syntaxbaumErstellen();

	vector<double>& funktionAbleiten(vector<double> &funktion);
	FunktionAlsVektorSyntaxbaum& funktionAbleitenGebrochenRational(FunktionAlsVektorSyntaxbaum &funktion);
	void setErsteAbleitung(vector<double> &funktion);
	void setErsteAbleitungGebrochenRational(FunktionAlsVektorSyntaxbaum& funktion);
	FunktionAlsVektorSyntaxbaum& getErsteAbleitungGebrochenRational();
	void setZweiteAbleitung(vector<double> &funktion);
	void setZweiteAbleitungGebrochenRational(FunktionAlsVektorSyntaxbaum& funktion);
	FunktionAlsVektorSyntaxbaum& getZweiteAbleitungGebrochenRational();
	void setDritteAbleitungGebrochenRational(FunktionAlsVektorSyntaxbaum& funktion);
	FunktionAlsVektorSyntaxbaum& getDritteAbleitungGebrochenRational();
	void setDritteAbleitung(vector<double> &funktion);
	vector<double> getErsteAbleitung();
	vector<double> getZweiteAbleitung();
	vector<double> getDritteAbleitung();
	void integrieren(vector<double> &funktion);
	double flaecheninhaltBerechnen(double linkeGrenze, double rechteGrenze);
	double flaecheninhaltMitTrapetzBerechnen(double linkeGrenze, double rechteGrenze);
	double flaecheninhaltMitTrapetzBerechnenGebrochenRational(double linkeGrenze, double rechteGrenze);
	double flaecheninhaltMitSimpsonBerechnen(double linkeGrenze, double rechteGrenze);
	double flaecheninhaltMitSimpsonBerechnenGebrochenRational(double linkeGrenze, double rechteGrenze);
	double bogenlaengeMitTrapetzBerechnen(double linkeGrenze, double rechteGrenze);
	double bogenlaengeMitTrapetzBerechnenGebrochenRational(double linkeGrenze, double rechteGrenze);

	void setSyntaxbaum(FunktionSyntaxbaum& neuerFunktionSyntaxbaum);
	void setSyntaxbaum(FunktionAlsVektorSyntaxbaum& neuerFunktionSyntaxbaum);
	void setSyntaxbaumGekuerzt(FunktionAlsVektorSyntaxbaum& neuerFunktionSyntaxbaum);
	FunktionAlsVektorSyntaxbaum& getSyntaxbaumGebrochenRational();
	FunktionAlsVektorSyntaxbaum& getSyntaxbaumGebrochenRationalGekürzt();

	vector<vector<double>> polynomdivision(vector<double>& zaehler, vector<double>& nenner);
	vector<double> polynomAddition(vector<double>& zaehler, vector<double>& nenner);
	vector<double> polynomSubtraktion(vector<double>& zaehler, vector<double>& nenner);
	vector<double> polynomMultiplikation(vector<double>& zaehler, vector<double>& nenner);
	WCHAR* entferneUnnoetigeKlammern(WCHAR* functionAlsStringLokal);
	double getPunkt(double x, FunktionAlsVektorSyntaxbaum *funktion);
	
	FunktionAlsVektorSyntaxbaum* knotenAnhaengen(bool links, FunktionAlsVektorSyntaxbaum *aktuellerKnoten, WCHAR* functionAlsStringLokal, int i);
	void stringSplitten(int i, int rechteGrenze, int klammerLevel, WCHAR* functionAlsStringLokal, FunktionAlsVektorSyntaxbaum* neuerKnoten);
	void vectorAnhaengen(WCHAR* functionAlsStringLokal, FunktionAlsVektorSyntaxbaum* neuerKnoten);
	void kuerzeSyntaxbaumGebrochenRational(FunktionAlsVektorSyntaxbaum* neuerFunktionSyntaxbaumGebrochenRational);
	WCHAR* funktionVectorToString(vector<double> funktion, bool mitMalZeichenZwischenXundZahl);
	WCHAR* stringsVerbinden(WCHAR* str1, WCHAR* str2, WCHAR operatorZeichen);
};

