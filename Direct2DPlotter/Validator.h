#pragma once
class Validator
{
public:
	Validator();
	~Validator();
	bool checkAufFalscheSymbole(TCHAR*); //0-9 +-*/^ () sind erlaubt bis jetzt
	bool checkKlammernKorrekt(TCHAR*); //richtige anzahl und ob jede �ffnende klammer eine schlie�ende hat
	bool checkDerSyntax(TCHAR*); //zum Beispiel x^*3
};

