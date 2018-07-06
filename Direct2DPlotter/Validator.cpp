#include "stdafx.h"
#include "Validator.h"


Validator::Validator(){

}


Validator::~Validator(){

}


bool Validator::checkAufFalscheSymbole(TCHAR* str) { //0-9 +-*/^ () sind erlaubt bis jetzt
	int i = 0;
	while (str[i] != 0) {
		if (str[i] != '^'&&str[i] != '*'&&str[i] != '/'&&str[i] != '+'&&str[i] != '-'&&str[i] != '('&&str[i] != ')' && !(str[i]>47 && str[i]<58) && str[i] != 'x'&&str[i] != 'X') {
			return false;
		}
		i++;
	}
	return true;
}

bool Validator::checkKlammernKorrekt(TCHAR* str) { //richtige anzahl und ob jede öffnende klammer eine schließende hat
	int klammerLevel = 0;
	int i = 0;
	while (str[i] != 0) {
		if (str[i] == '(') {
			klammerLevel++;
		}
		if (str[i] == ')') {
			klammerLevel--;
		}
		i++;
		if (klammerLevel < 0) {
			return false;
		}
	}
	if (klammerLevel != 0) {
		return false;
	}
	return true;
}

bool Validator::checkDerSyntax(TCHAR* str) { //zum Beispiel x^*3
	int i = 0;
	while (str[i] != 0) {
		if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^' ) {
			if (str[i+1] == '+' || str[i + 1] == '-' || str[i + 1] == '*' || str[i + 1] == '/' || str[i + 1] == '^' || str[i + 1] == ')') {
				return false;
			}
		}//xx fehlt noch
		i++;
	}
	return true;
}