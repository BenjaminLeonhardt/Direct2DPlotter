// Direct2DPlotter.cpp : Definiert den Einstiegspunkt für die Anwendung.
//

#include "stdafx.h"
#include "Direct2DPlotter.h"

#define MAX_LOADSTRING 100

// Globale Variablen:
HINSTANCE hInst;                                // Aktuelle Instanz
WCHAR szTitle[MAX_LOADSTRING];                  // Titelleistentext
WCHAR szWindowClass[MAX_LOADSTRING];            // Klassenname des Hauptfensters

HINSTANCE hInstEingabe;                                // Aktuelle Instanz
WCHAR szTitleEingabe[MAX_LOADSTRING];                  // Titelleistentext
WCHAR szWindowClassEingabe[MAX_LOADSTRING];            // Klassenname des Hauptfensters

HWND hWndEingabe;
LPWSTR *funktionAlsText;
HWND hEdit = 0;
HWND hButton = 0;
RechnerLibrary rechnerLibrary;
const int sizeFunktionBuffer = 255;
TCHAR functionBuffer[sizeFunktionBuffer] = { 0 };
TCHAR functionBufferOld[sizeFunktionBuffer] = { 0 };
Validator validator;

static mutex lockFunktion;
static mutex lockAuswahlPunkt;
static mutex lockTangente;
static mutex lockExtremstelle;
static mutex lockWendepunkte;
static mutex lockNullstelle;
static mutex lockPolstelle;
static mutex lockSyntaxbaum;

bool graphZeichnen = true;
bool tangenteZeichnen = false;
bool normaleZeichnen = false;
bool steigungsgraphZeichnen = false;
bool zweiteAbleitungZeichnen = false;
bool dritteAbleitungZeichnen = false;
bool maximaMinimaZeichnen = false;
bool wendepunkteZeichnen = false;
bool nullstellenZeichnen = false;
bool polstellenZeichnen = false;
bool MitternachsformelZeichnen = false;
bool pqZeichnen = false;
bool newtonregelZeichnen = false;
bool simpsonregelZeichnen = false;
bool trapetzregelZeichnen = false;
bool flacheninhaltZeichnen = false;
bool stammfunktionZeichnen = false;
bool kruemmungsradiusZeichnen = false;
bool bogenlaengeZeichnen = false;
bool test = false;
double testx = 10.0;
double testy = 10.0;
double gravi = 9.81;
double geschw = 0.0;
clock_t testtime = 0;


int toggel = 0;
int posXAlt = 0;
int posYAlt = 0;


vector<Vector2D> punkte;
vector<Vector2D> punkteErsteAbleitung;
vector<Vector2D> punkteZweiteAbleitung;
vector<Vector2D> punkteDritteAbleitung;
vector<Vector2D> punkteTangente;
vector<Vector2D> punkteNormale;
vector<Vector2D> punkteStammfunktion;

Vector2D aktuellerPunktAmGraph;
void zeichneTangenteAmPunkt(double x, double y);
HWND hWnd2;
//HWND checkboxTangente;

/*
bool maximaMinimaZeichnen = false;
bool nullstellenZeichnen = false;
bool MitternachsformelZeichnen = false;
bool pqZeichnen = false;
bool newtonregelZeichnen = false;
bool simpsonregelZeichnen = false;
bool trapetzregelZeichnen = false;
bool flacheninhaltZeichnen = false;
bool kruemmungsradiusZeichnen = false;
bool bogenlaengeZeichnen = false;*/

#define buttonOkPressed 1
#define buttonCheckboxTangente 2
#define buttonCheckboxsteigungsgraph 3
#define buttonMaximaMinimaPressed 4
#define buttonCheckboxExtremstellen 13
#define buttonMitternachtsformelPressed 5
#define buttonPQformelPressed 6
#define buttonNewtonVerfahrenPressed 7
#define buttonRegulaFalsiVerfahrenPressed 8
#define buttonCheckboxNullstellen 9
#define buttonCheckboxBogenlaenge 10
#define buttonCheckboxkruemmungsradius 11
#define buttonflaecheninhaltMitIntegralBerechnen 12
#define buttonCheckboxflaecheninhalt 14
#define buttonPolynomdivisionBerechnen 15
#define buttonflaecheninhaltMitSimpsonBerechnen 16
#define buttonflaecheninhaltMitTrapetzBerechnen 17
#define buttonCheckboxIntegral 18
#define buttonCheckboxNormale 19
#define buttonCheckboxZweiteAbleitung 20
#define buttonBogenlaengeMitTrapetzBerechnen 21
#define buttonCheckboxFunktionsgraphAnzeigen 22
#define buttonSeite2Anzeigen 23
#define buttonSeite1Anzeigen 24
#define buttonCheckboxWendepunkte 25
#define buttonWendepunktePressed 26
#define buttonTestPressed 27
#define buttonCheckboxPolstellen 28
#define buttonCheckboxDritteAbleitung 29

HWND hMaximaMinimaButton;
HWND hMaximaEditField;
HWND hMinimaEditField;
HWND hMitternachtButton;
HWND hPQButton;
HWND hNewtonButton;
HWND hRegulaFalsiButton;
HWND hNullstellenEditField;
HWND hPolstellenEditField;
HWND hWendepunkteButton;
HWND hTextWendepunkte;
HWND hWendepunkteEditField;
HWND hbuttonCheckboxWendepunkte;

HWND hIntegralLinkeGrenzeEditField;
HWND hIntegraRechteGrenzeEditField;
HWND hFlaecheninhaltMitIntegralBerechnen;
HWND hFlaecheninhaltMitSimpsonBerechnen;
HWND hFlaecheninhaltMitTrapetzBerechnen;
HWND hFlaecheninhaltAusgebenField;
HWND hpolynomDivisorZaehlerEditField;
HWND hpolynomDivisorNennerEditField;
HWND hpolynompolynomDivisorButton;
HWND hpolynomDivisorErgebnisEditField; 
HWND hBogenlaengeMitTrapetzBerechnen;
HWND hBogenlaengeAusgebenField;
HWND EingabefeldCheckbox;
HWND hbuttonCheckboxFunktionsgraphAnzeigen;
HWND hbuttonCheckboxTangente;
HWND hbuttonCheckboxsteigungsgraph;
HWND hbuttonCheckboxZweiteAbleitung;
HWND hbuttonCheckboxDritteAbleitung;
HWND hbuttonCheckboxNormale;
HWND hbuttonCheckboxkruemmungsradius;
HWND hTestField;

HWND hTextMaximas;
HWND hTextMinimas;
HWND hbuttonCheckboxExtremstellen;
HWND hTextNullstellen;
HWND hbuttonCheckboxNullstellen;
HWND hTextPolstellen;
HWND hbuttonCheckboxPolstellen;
HWND hTextIntegralgrenzen;
HWND hTextIntegralLinkeGrenze;
HWND hTextIntegralRechteGrenze;
HWND hTextIntegralFlaeche;
HWND hbuttonCheckboxflaecheninhalt;
HWND hbuttonCheckboxIntegral;
HWND hTextBogenlaenge;
HWND hbuttonCheckboxBogenlaenge;

HWND hButtonSeite2;


HWND hButtonSeite1;

HWND hTextPolynomdivision;
HWND hTextPolynomdivisionDoppelpunkt;
HWND hTextPolynomdivisionErgebnis;


wstring funktionText;
wstring funktionErsteAbleitungText;
wstring funktionZweiteAbleitungText;
wstring funktionDritteAbleitungText;
wstring funktionIntegralText;


wstring funktionTextGebrochenRationalZaehler;
wstring funktionTextGebrochenRationalNenner;
wstring funktionErsteAbleitungTextGebrochenRationalZaehler;
wstring funktionErsteAbleitungTextGebrochenRationalNenner;
wstring funktionZweiteAbleitungTextGebrochenRationalZaehler;
wstring funktionZweiteAbleitungTextGebrochenRationalNenner;
wstring funktionDritteAbleitungTextGebrochenRationalZaehler;
wstring funktionDritteAbleitungTextGebrochenRationalNenner;
wstring funktionIntegralTextGebrochenRationalZaehler;
wstring funktionIntegralTextGebrochenRationalNenner;

struct Farbe {
	double rot;
	double gruen;
	double blau;
	double alpha;
};

Farbe farbeAchsenkreuz = { 1,1,1 };
Farbe farbeKoordinatenZahlen = { 1,1,1 };


Farbe farbeFunktion = {1,1,1};
Farbe farbeErsteAbleitung = { 0.9,0.8,0.8 };
Farbe farbeZweiteAbleitung = { 0.6,0.7,0.6 };
Farbe farbeDritteAbleitung = { 0.4,0.4,0.5 };
Farbe farbeIntegral = { 0.3,0.2,0.3 };

Farbe farbePositionsPunkt = { 0.2,0.2,0.8, 0.6 };

Farbe farbeTangente = { 0.8,0.8,0.8 };
Farbe farbeNormale = { 0.8,0.8,0.8 };
Farbe farbeIntegralFläche = { 0.8,0.8,0.8 };
Farbe farbeBogenlaenge = { 0.8,0.2,0.2 };
Farbe farbeKrümmungskreis = { 0.2f, 0.7f, 0.2f};
Farbe farbeKrümmungskreisPunkt = { 0.7f, 0.2f, 0.2f };

Farbe farbePolstelle = { 0.8,0.2,0.3, 0.8 };
Farbe farbeNullstelle = { 0.8,0.2,0.4, 0.8 };
Farbe farbeWendepunkt = { 0.8,0.4,0.2, 0.8 };
Farbe farbeExtremstelle = { 0.8,0.1,0.1, 0.8 };

Graphics* graphics;
thread* plotterMainThread;
static long double PI = 3.141592653589793238463;
static mutex lockGraph;
static mutex lockRun;
bool run = true;



int windowSizeX = 1200;									//Fenstergröße
int windowSizeY = 900;									//Fenstergröße

int windowSizeEingabefeldX = 300;						//Fenstergröße
int windowSizeEingabefeldY = 900;						//Fenstergröße

double posX = 600;										//Position des geplotteten Teils (mittelpunkt)
double posY = 400;										//Position des geplotteten Teils (mittelpunkt)
int vergroesserung = 8;									//Vergrößerungsfaktor des Teils
int vergroesserungNormale = 1;							//Vergrößerungsfaktor der Normalen an den Punkten
int windowPosX = 100;//-1920;							//Fensterposition
int windowPosY = 100;									//Fensterposition
double linkeGrenze;
double rechteGrenze;


// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    CB_Eingabefeld(HWND, UINT, WPARAM, LPARAM);

thread** threadsZeichnen;

void zeichneKoordinatenkreuz(int cpuID) {
	int steps = 10;
	float strichhoehe1 = 1.0f;
	float strichhoehe2 = -1.0f;
	float textEntfernung = 4.0f;
	int von = -1000;
	int bis = 1000;
	/*double step = 5;
	if (vergroesserung != 0) {
		step = 5 * (double)vergroesserung;
	}*/
	if (vergroesserung >= 20&& vergroesserung < 35) {
		steps = 5;
		von = -100;
		bis = 100;
		strichhoehe1 = 0.5f;
		strichhoehe2 = -0.5f;
		textEntfernung = 2.0f;
	}
	if (vergroesserung >= 35) {
		steps = 2;
		von = -50;
		bis = 50;
		strichhoehe1 = 0.2f;
		strichhoehe2 = -0.2f;
		textEntfernung = 1.0f;
	}if (vergroesserung >= 60) {
		steps = 1;
		von = -20;
		bis = 20;
		strichhoehe1 = 0.2f;
		strichhoehe2 = -0.2f;
		textEntfernung = 1.0f;
	}
	for (int i = von; i < bis; i += steps) {
		if (i != 0) {
			std::wstring beschriftung;
			beschriftung += std::to_wstring(-i);

			graphics->DrawLine(10000.0f, -i, -10000.0f, -i, 0.9f, 0.9f, 0.9f, 0.2f, cpuID);
			graphics->DrawTextS(textEntfernung, -i, 30.0f, 30.0f, beschriftung, true, true, 0.8, 0.8, 0.8, 10.0, cpuID);
			graphics->DrawLine(strichhoehe1, -i, strichhoehe2, -i, 0.9f, 0.9f, 0.9f, 10.0f, cpuID);
			
			
			graphics->DrawLine(i, 10000.0f, i, -10000.0f, 0.9f, 0.9f, 0.9f, 0.2f, cpuID);
			graphics->DrawTextS(i, textEntfernung, 30.0f, 30.0f, beschriftung, true,true,0.8,0.8,0.8,10.0, cpuID);
			graphics->DrawLine(i, strichhoehe2, i, strichhoehe1, 0.9f, 0.9f, 0.9f, 10.0f, cpuID);
			
		}
	}
	if (cpuID == 0) {
		graphics->DrawLine(0.0f, 10000.0f, 0.0f, -10000.0f, 0.9f, 0.9f, 0.9f, 10.0f, cpuID);
		graphics->DrawLine(10000.0f, 0.0f, -10000.0f, 0.0f, 0.9f, 0.9f, 0.9f, 10.0f, cpuID);
	}
}

void clearScreen() {
	lockGraph.lock();
	if (run) {
		graphics->BeginDraw();
		graphics->ClearScreen(0.0f, 0.0f, 0.1f);
		/*for (int i = 0; i < graphics->getMaxCPU(); i++) {
			threadsZeichnen[i] = new thread(&zeichneKoordinatenkreuz,i);
			//threadsZeichnen[i]->join();
		}*/
		zeichneKoordinatenkreuz(0);
		/*graphics->DrawLine(0.0f, 10000.0f, 0.0f, -10000.0f, 0.9f, 0.9f, 0.9f, 10.0f, 0);
		graphics->DrawLine(10000.0f, 0.0f, -10000.0f, 0.0f, 0.9f, 0.9f, 0.9f, 10.0f, 0);
		for (int i = 0; i < graphics->getMaxCPU(); i++) {
			if (threadsZeichnen[i]->joinable()) {
				threadsZeichnen[i]->join();
			}
		}*/
	}
	lockGraph.unlock();
	lockGraph.lock();
	if (run) {
		graphics->EndDraw();
	}
	lockGraph.unlock();
}

void punkte_berechnen() {
	lockFunktion.lock(); cout << "lockFunction lock in punkte berechnen" << endl;
	rechnerLibrary.getFunktionAlsVector().clear();
	rechnerLibrary.setFunctionBuffer(functionBuffer);
	//rechnerLibrary.parseFuntionBuffer(baum, functionBuffer);
	rechnerLibrary.syntaxbaumErstellen();
	punkte.clear();
	punkteErsteAbleitung.clear();
	punkteZweiteAbleitung.clear();
	punkteStammfunktion.clear();
	rechnerLibrary.setErsteAbleitung(rechnerLibrary.funktionAbleiten(rechnerLibrary.getFunktionAlsVector()));
	rechnerLibrary.setZweiteAbleitung(rechnerLibrary.funktionAbleiten(rechnerLibrary.getErsteAbleitung()));
	rechnerLibrary.setDritteAbleitung(rechnerLibrary.funktionAbleiten(rechnerLibrary.getZweiteAbleitung()));
	rechnerLibrary.integrieren(rechnerLibrary.getFunktionAlsVector());

	/*funktionText = L"f(x)=";
	funktionText += funktionVectorToString(rechnerLibrary.getFunktionAlsVector());

	funktionErsteAbleitungText = L"f '(x)=";
	funktionErsteAbleitungText += funktionVectorToString(rechnerLibrary.getErsteAbleitung());
	funktionZweiteAbleitungText = L"f ''(x)=";
	funktionZweiteAbleitungText += funktionVectorToString(rechnerLibrary.getZweiteAbleitung());
	funktionDritteAbleitungText = L"f '''(x)=";
	funktionDritteAbleitungText += funktionVectorToString(rechnerLibrary.getDritteAbleitung());
	funktionIntegralText = L"F(x)=";
	funktionIntegralText += funktionVectorToString(rechnerLibrary.getStammfunktionAlsVector());*/

	

	double xOld = 0;
	double yOld = 0;
	bool round2 = false;
	double x = 0, y = 0;
	double x2 = 0, yErsteAbleitung = 0, yZweiteAbleitung = 0;
	double yStammfunktion = 0;
	double tmp = 0;
	double step = 0.2;
	int von = -1000;
	int bis = 1000;
	if (vergroesserung > 20) {
		step = 0.02;
		von = -100;
		bis = 100;
	}
	for (double i = von; i < bis; i += step) {

		y = rechnerLibrary.f(i, &rechnerLibrary.getFunktionAlsVector());
		yErsteAbleitung = rechnerLibrary.f(i, &rechnerLibrary.getErsteAbleitungAlsVector());
		yZweiteAbleitung = rechnerLibrary.f(i, &rechnerLibrary.getZweiteAbleitung());
		yStammfunktion = rechnerLibrary.f(i, &rechnerLibrary.getStammfunktionAlsVector());
		//y = i*i*(i - 4) / (i + 4);
		if (y < 500 && y > -500) {
			if (round2) {
				Vector2D p;
				p.x = i;
				p.y = y;
				punkte.push_back(p);
			}
		}
		if (yErsteAbleitung < 500 && yErsteAbleitung > -500) {
			if (round2) {
				Vector2D p2;
				p2.x = i;
				p2.y = yErsteAbleitung;
				punkteErsteAbleitung.push_back(p2);
			}
		}
		if (yZweiteAbleitung < 500 && yZweiteAbleitung > -500) {
			if (round2) {
				Vector2D p2;
				p2.x = i;
				p2.y = yZweiteAbleitung;
				punkteZweiteAbleitung.push_back(p2);
			}
		}
		if (yStammfunktion < 500 && yStammfunktion > -500) {
			if (round2) {
				Vector2D p2;
				p2.x = i;
				p2.y = yStammfunktion;
				punkteStammfunktion.push_back(p2);
			}
		}
		xOld = x;
		yOld = y;
		round2 = true;
	}
	lockFunktion.unlock();
}

wstring funktionVectorToString(vector<double> funktion) {
	wstring beschriftung;
	bool periodErkannt = false;
	wstring tmpBeschriftung;
	int indexLetzteNachkommastelle = 0;
	bool hatNachkommastellen = false;
	int indexPeriod = 0;

	for (int i = funktion.size() - 1; i > -1; i--) {
		if (funktion[i] != 0) {
			if (i != funktion.size() - 1 && funktion[i] > 0) {
				beschriftung += '+';
			}
			tmpBeschriftung = std::to_wstring(funktion[i]);
			periodErkannt = false;
			hatNachkommastellen = false;
			indexLetzteNachkommastelle = 0;
			for (int j = 0; j < tmpBeschriftung.size(); j++) {
				if (periodErkannt) {
					if (tmpBeschriftung[j] != '0') {
						indexLetzteNachkommastelle = j + 1;
						hatNachkommastellen = true;
					}
				}
				if (tmpBeschriftung[j] == '.') {
					periodErkannt = true;
					indexPeriod = j;
				}
			}
			if (hatNachkommastellen) {
				for (int j = 0; j < indexLetzteNachkommastelle; j++) {
					beschriftung += tmpBeschriftung[j];
				}
				beschriftung += '*';
			}
			else {
				for (int j = 0; j < indexPeriod; j++) {
					if (i == 0) {
						beschriftung += tmpBeschriftung[j];
					}
					else {
						if (tmpBeschriftung[j] != '1') {
							beschriftung += tmpBeschriftung[j];
							//beschriftung += '*';
						}
					}
				}
			}


			if (i >= 2) {
				beschriftung += L"x^";
				beschriftung += std::to_wstring(i);
			}
			if (i == 1) {
				beschriftung += L"x";
			}
		}
	}

	return beschriftung;
}

vector<Vector2D>polstellenPunkteBerechnen;
bool polstellenPunkteBerechnenErledigt=false;
void punkte_berechnenGebrochenRational() {
	lockFunktion.lock(); cout << "lockFunction lock in punkte berechnen" << endl;
	//rechnerLibrary.getFunktionAlsVector().clear();
	//rechnerLibrary.setFunctionBuffer(functionBuffer);
	//rechnerLibrary.parseFuntionBuffer(baum, functionBuffer);
	//rechnerLibrary.syntaxbaumErstellen();
	punkte.clear();
	punkteErsteAbleitung.clear();
	punkteZweiteAbleitung.clear();
	punkteDritteAbleitung.clear();
	punkteStammfunktion.clear();
	rechnerLibrary.setErsteAbleitungGebrochenRational(rechnerLibrary.funktionAbleitenGebrochenRational(rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt()));
	if (&rechnerLibrary.getErsteAbleitungGebrochenRational() != nullptr) {
		rechnerLibrary.setZweiteAbleitungGebrochenRational(rechnerLibrary.funktionAbleitenGebrochenRational(rechnerLibrary.getErsteAbleitungGebrochenRational()));
		if (&rechnerLibrary.getZweiteAbleitungGebrochenRational() != nullptr) {
			rechnerLibrary.setDritteAbleitungGebrochenRational(rechnerLibrary.funktionAbleitenGebrochenRational(rechnerLibrary.getZweiteAbleitungGebrochenRational()));
		}
	}

	if (rechnerLibrary.getSyntaxbaumGebrochenRational().getInhaltTChar() == '/') {
		funktionText = L"f(x)=";
		funktionTextGebrochenRationalZaehler = L"";
		funktionTextGebrochenRationalZaehler += rechnerLibrary.getSyntaxbaumGebrochenRational().getLinkesChild()->getInhaltString();
		funktionTextGebrochenRationalNenner = L"";
		funktionTextGebrochenRationalNenner += rechnerLibrary.getSyntaxbaumGebrochenRational().getRechtesChild()->getInhaltString();

		funktionErsteAbleitungText = L"f'(x)=";
		funktionErsteAbleitungTextGebrochenRationalZaehler = rechnerLibrary.getErsteAbleitungGebrochenRational().getLinkesChild()->getInhaltString();
		funktionErsteAbleitungTextGebrochenRationalNenner = rechnerLibrary.getErsteAbleitungGebrochenRational().getRechtesChild()->getInhaltString();

		funktionZweiteAbleitungText = L"f''(x)=";
		funktionZweiteAbleitungTextGebrochenRationalZaehler = rechnerLibrary.getZweiteAbleitungGebrochenRational().getLinkesChild()->getInhaltString();
		funktionZweiteAbleitungTextGebrochenRationalNenner = rechnerLibrary.getZweiteAbleitungGebrochenRational().getRechtesChild()->getInhaltString();


		funktionDritteAbleitungText = L"f'''(x)=";
		funktionDritteAbleitungTextGebrochenRationalZaehler = rechnerLibrary.getDritteAbleitungGebrochenRational().getLinkesChild()->getInhaltString();
		funktionDritteAbleitungTextGebrochenRationalNenner = rechnerLibrary.getDritteAbleitungGebrochenRational().getRechtesChild()->getInhaltString();


		funktionIntegralText = L"F(x)=";
	}
	else {
		funktionText = L"f(x)=";
		funktionText += rechnerLibrary.getSyntaxbaumGebrochenRational().getInhaltString();
		funktionErsteAbleitungText = L"f'(x)=";
		funktionErsteAbleitungText += rechnerLibrary.getErsteAbleitungGebrochenRational().getInhaltString();
		funktionZweiteAbleitungText = L"f''(x)=";
		funktionZweiteAbleitungText += rechnerLibrary.getZweiteAbleitungGebrochenRational().getInhaltString();
		funktionDritteAbleitungText = L"f'''(x)=";
		funktionDritteAbleitungText += rechnerLibrary.getDritteAbleitungGebrochenRational().getInhaltString();
		funktionIntegralText = L"F(x)=";
		funktionIntegralText += funktionVectorToString(rechnerLibrary.getStammfunktionAlsVector());
	}

	double xOld = 0;
	double yOld = 0;
	bool round2 = false;
	double x = 0, y = 0;
	double x2 = 0, yErsteAbleitung = 0, yZweiteAbleitung = 0, yDritteAbleitung = 0;
	double yStammfunktion = 0;
	double tmp = 0;
	double step = 0.2;
	int von = -1000;
	int bis = 1000;
	if (vergroesserung > 20) {
		step = 0.02;
		von = -100;
		bis = 100;
	}
	vector<Vector2D>kurvenDiskusionsPunktePole;
	vector<Vector2D>kurvenDiskusionsPunkteRest;
	if (rechnerLibrary.getPolstellen().size() < 1&& polstellenPunkteBerechnenErledigt==false) {
		rechnerLibrary.regulaFalsiVerfahrenGebrochenRational();
		rechnerLibrary.extremstellenBerechnenGebrochenRational();
		rechnerLibrary.wendepunkteBerechnenGebrochenRational();
		kurvenDiskusionsPunktePole = rechnerLibrary.getPolstellen();
		for (int i = 0; i < rechnerLibrary.getNullstellen().size(); i++) {
			kurvenDiskusionsPunkteRest.push_back(rechnerLibrary.getNullstellen()[i]);
		}
		for (int i = 0; i < rechnerLibrary.getWendepunkte().size(); i++) {
			kurvenDiskusionsPunkteRest.push_back(rechnerLibrary.getWendepunkte()[i]);
		}
		for (int i = 0; i < rechnerLibrary.getExtremstellen().size(); i++) {
			kurvenDiskusionsPunkteRest.push_back(rechnerLibrary.getExtremstellen()[i]);
		}
		polstellenPunkteBerechnenErledigt = true;
		rechnerLibrary.getPolstellen().clear();
		rechnerLibrary.getNullstellen().clear();
		textNullstellenfeldAendern(L"?");
		textPolstellenfeldAendern(L"?");
	}else if (rechnerLibrary.getPolstellen().size() < 1 && polstellenPunkteBerechnenErledigt == true) {

	}
	else {
		kurvenDiskusionsPunktePole = rechnerLibrary.getPolstellen();
		
	}
	for (double i = von; i < bis; i += step) {

		y = rechnerLibrary.getPunkt(i, &rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt());
		yErsteAbleitung = rechnerLibrary.getPunkt(i, &rechnerLibrary.getErsteAbleitungGebrochenRational());
		yZweiteAbleitung = rechnerLibrary.getPunkt(i, &rechnerLibrary.getZweiteAbleitungGebrochenRational());
		yDritteAbleitung = rechnerLibrary.getPunkt(i, &rechnerLibrary.getDritteAbleitungGebrochenRational());
		/*yErsteAbleitung = rechnerLibrary.f(i, &rechnerLibrary.getErsteAbleitungAlsVector());
		yZweiteAbleitung = rechnerLibrary.f(i, &rechnerLibrary.getZweiteAbleitung());
		yStammfunktion = rechnerLibrary.f(i, &rechnerLibrary.getStammfunktionAlsVector());*/
		//y = i*i*(i - 4) / (i + 4);
		if (y < 500 && y > -500) {
			if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltTChar() == '/') {
				for (int j = 0; j < kurvenDiskusionsPunkteRest.size(); j++) {
					if ((i >(kurvenDiskusionsPunkteRest[j].x - 1.5)) && (i < (kurvenDiskusionsPunkteRest[j].x + 1.5))) {
						step = 0.02;
					}else {
						step = 0.2;
					}
				}
				for (int j = 0; j < kurvenDiskusionsPunktePole.size(); j++) {
					if (kurvenDiskusionsPunktePole[j].x < 0) {
						if ((i > (kurvenDiskusionsPunktePole[j].x - 0.2)) && (i < (kurvenDiskusionsPunktePole[j].x + 0.2))) {
							step = 0.1;
							if ((i > (kurvenDiskusionsPunktePole[j].x - 0.1)) && (i < (kurvenDiskusionsPunktePole[j].x + 0.1))) {
								step = 0.05;
								if ((i > (kurvenDiskusionsPunktePole[j].x - 0.05)) && (i < (kurvenDiskusionsPunktePole[j].x + 0.05))) {
									step = 0.02;
									if ((i > (kurvenDiskusionsPunktePole[j].x - 0.02)) && (i < (kurvenDiskusionsPunktePole[j].x + 0.02))) {
										step = 0.001;

										if ((i > (kurvenDiskusionsPunktePole[j].x - 0.001)) && (i < (kurvenDiskusionsPunktePole[j].x + 0.001))) {
											step = 0.0001;
										}
									}
								}
							}
						}
						else {
							double step = 0.2;
							if (vergroesserung > 20) {
								step = 0.02;
								if ((i >(kurvenDiskusionsPunktePole[j].x - 0.02)) && (i < (kurvenDiskusionsPunktePole[j].x + 0.02))) {
									step = 0.001;
									
									
								}
							}
						}
					}
					else {
						if ((i >(kurvenDiskusionsPunktePole[j].x - 0.2)) && (i < (kurvenDiskusionsPunktePole[j].x + 0.2))) {
							step = 0.1;
							if ((i >(kurvenDiskusionsPunktePole[j].x - 0.1)) && (i < (kurvenDiskusionsPunktePole[j].x + 0.1))) {
								step = 0.05;
								if ((i >(kurvenDiskusionsPunktePole[j].x - 0.05)) && (i < (kurvenDiskusionsPunktePole[j].x + 0.05))) {
									step = 0.02;
									if ((i >(kurvenDiskusionsPunktePole[j].x - 0.02)) && (i < (kurvenDiskusionsPunktePole[j].x + 0.02))) {
										step = 0.001;
										if ((i >(kurvenDiskusionsPunktePole[j].x - 0.001)) && (i < (kurvenDiskusionsPunktePole[j].x + 0.001))) {
											step = 0.0001;
										}
									}
								}
							}
						}
						else {
							double step = 0.2;
							if (vergroesserung > 20) {
								step = 0.02;
								if ((i >(kurvenDiskusionsPunktePole[j].x - 0.02)) && (i < (kurvenDiskusionsPunktePole[j].x + 0.02))) {
									step = 0.001;
								}
							}
						}
					}
				}
				
			}
			if (round2) {
				Vector2D p;
				p.x = i;
				p.y = y;
				punkte.push_back(p);
			}
		}
		if (yErsteAbleitung < 500 && yErsteAbleitung > -500) {
			if (round2) {
				Vector2D p2;
				p2.x = i;
				p2.y = yErsteAbleitung;
				punkteErsteAbleitung.push_back(p2);
			}
		}
		if (yZweiteAbleitung < 500 && yZweiteAbleitung > -500) {
			if (round2) {
				Vector2D p2;
				p2.x = i;
				p2.y = yZweiteAbleitung;
				punkteZweiteAbleitung.push_back(p2);
			}
		}
		if (yDritteAbleitung < 500 && yDritteAbleitung > -500) {
			if (round2) {
				Vector2D p2;
				p2.x = i;
				p2.y = yDritteAbleitung;
				punkteDritteAbleitung.push_back(p2);
			}
		}

		/*if (yStammfunktion < 500 && yStammfunktion > -500) {
			if (round2) {
				Vector2D p2;
				p2.x = i;
				p2.y = yStammfunktion;
				punkteStammfunktion.push_back(p2);
			}
		}*/
		xOld = x;
		yOld = y;
		round2 = true;
	}
	lockFunktion.unlock();
}

double radius = 0;
double yKreis = 0;
double xKreis = 0;

void zeichneKreisAmGraph(double x) {
	aktuellerPunktAmGraph.x = -x;
	aktuellerPunktAmGraph.y = rechnerLibrary.getPunkt(-x, &rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt());
	double steigungTangente = rechnerLibrary.getPunkt(-x, &rechnerLibrary.getErsteAbleitungGebrochenRational());
	/*aktuellerPunktAmGraph.y = rechnerLibrary.f(-x, &rechnerLibrary.getFunktionAlsVector());
	double steigungTangente = rechnerLibrary.f(-x, &rechnerLibrary.getErsteAbleitungAlsVector());*/
	zeichneTangenteAmPunkt(aktuellerPunktAmGraph.y, steigungTangente);
	
	double yf = rechnerLibrary.getPunkt(-x, &rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt());
	double yfs = rechnerLibrary.getPunkt(-x, &rechnerLibrary.getErsteAbleitungGebrochenRational());
	double yfz = rechnerLibrary.getPunkt(-x, &rechnerLibrary.getZweiteAbleitungGebrochenRational());

	/*double yf = rechnerLibrary.f(-x, &rechnerLibrary.getFunktionAlsVector());
	double yfs = rechnerLibrary.f(-x, &rechnerLibrary.getErsteAbleitung());
	double yfz = rechnerLibrary.f(-x, &rechnerLibrary.getZweiteAbleitung());*/

	radius = (pow(pow(yfs, 2) + 1, 1.5) / abs(yfz))*2;

	xKreis = x + (yfs*((pow(yfs, 2) + 1) / yfz));
	yKreis = yf + ((pow(yfs, 2) + 1) / yfz);
}

void zeichneTangenteAmPunkt(double hoehe, double steigung) {
	lockTangente.lock();
	punkteTangente.clear();
	punkteNormale.clear();
	vector<double> tangente;
	tangente.push_back(hoehe);
	tangente.push_back(steigung);
	vector<double> normale;
	normale.push_back(hoehe);
	normale.push_back(-(1/steigung));
	double ytangente = 0;
	double yNormale = 0;
	bool round2 = false;


	for (double i = -1000; i < 1000; i += 1) {
		if (i >= -1) {
			int j = 0;
		}
		ytangente = rechnerLibrary.f(i, &tangente);
		yNormale = rechnerLibrary.f(i, &normale);
		if (ytangente < 500 && ytangente > -500) {
			if (round2) {
				Vector2D p;
				p.x = (i + aktuellerPunktAmGraph.x);
				p.y = ytangente;
				punkteTangente.push_back(p);
			}
		}
		if (yNormale < 500 && yNormale > -500) {
			if (round2) {
				Vector2D p;
				p.x = (i + aktuellerPunktAmGraph.x);
				p.y = yNormale;
				punkteNormale.push_back(p);
			}
		}
		round2 = true;
	}
	lockTangente.unlock();
}

int counter = 0;

int zeichenZaehler(wstring text) {
	int anzahl = 0;
	int i = 0;
	while (text[i] != 0) {
		anzahl++;
		i++;
	}
	return anzahl;
}

void plotter_thread() {
	graphics->SetRotationsWinkel(180);

	while (run) {
		lockGraph.lock();
		if (run) {
			graphics->BeginDraw();
			graphics->ClearScreen(0.0f, 0.0f, 0.1f);
			/*for (int i = 0; i < graphics->getMaxCPU(); i++) {
			threadsZeichnen[i] = new thread(&zeichneKoordinatenkreuz,i);
			//threadsZeichnen[i]->join();
			}*/
			zeichneKoordinatenkreuz(0);
		}
		lockGraph.unlock();

		/*if (vergroesserung > 20) {
			if (counter == 0) {
				lockSyntaxbaum.lock();
				FunktionSyntaxbaum baum;
				rechnerLibrary.setFunctionBuffer(functionBuffer);
				rechnerLibrary.parseFuntionBuffer(baum, functionBuffer);
				rechnerLibrary.setSyntaxbaum(baum);
				//punkte_berechnen();
				punkte_berechnenGebrochenRational();
				lockSyntaxbaum.unlock();
				counter++;
			}
		}
		if (vergroesserung < 20) {
			if (counter == 1) {
				lockSyntaxbaum.lock();
				FunktionSyntaxbaum baum;
				rechnerLibrary.setFunctionBuffer(functionBuffer);
				rechnerLibrary.parseFuntionBuffer(baum, functionBuffer);
				rechnerLibrary.setSyntaxbaum(baum);
				//punkte_berechnen();
				punkte_berechnenGebrochenRational();
				lockSyntaxbaum.unlock();
				counter--;
			}
		}*/
		lockFunktion.lock();
		graphics->DrawCircle(-aktuellerPunktAmGraph.x, aktuellerPunktAmGraph.y, 1, farbePositionsPunkt.rot, farbePositionsPunkt.gruen, farbePositionsPunkt.blau, farbePositionsPunkt.alpha,0);
		if (rechnerLibrary.getSyntaxbaumGebrochenRational().getInhaltTChar() == '/') {
			double schriftWeiteInPixel = 8.6;
			double schrittweiteZwischenDenFunktionen = 50;
			int zeichenCounterFunktionZaehler = 0;
			int zeichenCounterFunktionNenner = 0;
			int maxZeichenFunktionCounter = 0;
			zeichenCounterFunktionZaehler=zeichenZaehler(funktionTextGebrochenRationalZaehler);
			zeichenCounterFunktionNenner = zeichenZaehler(funktionTextGebrochenRationalNenner);
			maxZeichenFunktionCounter = zeichenCounterFunktionZaehler > zeichenCounterFunktionNenner ? zeichenCounterFunktionZaehler : zeichenCounterFunktionNenner;
			graphics->DrawTextS(5, 20, 45, 40, funktionText, false, false, farbeFunktion.rot, farbeFunktion.gruen, farbeFunktion.blau, 10.0, 0);
			graphics->DrawTextS(60, 10, ((double)maxZeichenFunktionCounter)*schriftWeiteInPixel, 40, funktionTextGebrochenRationalZaehler, false, false, farbeFunktion.rot, farbeFunktion.gruen, farbeFunktion.blau, 10.0, 0);
			graphics->DrawTextS(60, 33, ((double)maxZeichenFunktionCounter)*schriftWeiteInPixel, 40, funktionTextGebrochenRationalNenner, false, false, farbeFunktion.rot, farbeFunktion.gruen, farbeFunktion.blau, 10.0, 0);
			graphics->DrawLineS(60, 30, 60+((double)maxZeichenFunktionCounter)*schriftWeiteInPixel, 30, farbeFunktion.rot, farbeFunktion.gruen, farbeFunktion.blau, 10.0f,0);




			int zeichenCounterFunktionErsteAbleitungZaehler = 0;
			int zeichenCounterFunktionErsteAbleitungNenner = 0;
			int maxZeichenFunktionErsteAbleitungCounter = 0;
			zeichenCounterFunktionErsteAbleitungZaehler = zeichenZaehler(funktionErsteAbleitungTextGebrochenRationalZaehler);
			zeichenCounterFunktionErsteAbleitungNenner = zeichenZaehler(funktionErsteAbleitungTextGebrochenRationalNenner);
			maxZeichenFunktionErsteAbleitungCounter = zeichenCounterFunktionErsteAbleitungZaehler > zeichenCounterFunktionErsteAbleitungNenner ? zeichenCounterFunktionErsteAbleitungZaehler : zeichenCounterFunktionErsteAbleitungNenner;

			graphics->DrawTextS(5, 20+ schrittweiteZwischenDenFunktionen, 45, 20, funktionErsteAbleitungText, false, false, farbeErsteAbleitung.rot, farbeErsteAbleitung.gruen, farbeErsteAbleitung.blau, 10.0, 0);
			graphics->DrawTextS(60, 10 + schrittweiteZwischenDenFunktionen, ((double)maxZeichenFunktionErsteAbleitungCounter) * schriftWeiteInPixel, 40, funktionErsteAbleitungTextGebrochenRationalZaehler, false, false, farbeErsteAbleitung.rot, farbeErsteAbleitung.gruen, farbeErsteAbleitung.blau, 10.0, 0);
			graphics->DrawTextS(60, 33 + schrittweiteZwischenDenFunktionen, ((double)maxZeichenFunktionErsteAbleitungCounter) * schriftWeiteInPixel, 40, funktionErsteAbleitungTextGebrochenRationalNenner, false, false, farbeErsteAbleitung.rot, farbeErsteAbleitung.gruen, farbeErsteAbleitung.blau, 10.0, 0);
			graphics->DrawLineS(60, 30 + schrittweiteZwischenDenFunktionen, 60 + ((double)maxZeichenFunktionErsteAbleitungCounter) * schriftWeiteInPixel, 30 + schrittweiteZwischenDenFunktionen, farbeErsteAbleitung.rot, farbeErsteAbleitung.gruen, farbeErsteAbleitung.blau, 10.0f, 0);




			int zeichenCounterFunktionZweiteAbleitungZaehler = 0;
			int zeichenCounterFunktionZweiteAbleitungNenner = 0;
			int maxZeichenFunktionZweiteAbleitungCounter = 0;
			zeichenCounterFunktionZweiteAbleitungZaehler = zeichenZaehler(funktionZweiteAbleitungTextGebrochenRationalZaehler);
			zeichenCounterFunktionZweiteAbleitungNenner = zeichenZaehler(funktionZweiteAbleitungTextGebrochenRationalNenner);
			maxZeichenFunktionZweiteAbleitungCounter = zeichenCounterFunktionZweiteAbleitungZaehler > zeichenCounterFunktionZweiteAbleitungNenner ? zeichenCounterFunktionZweiteAbleitungZaehler : zeichenCounterFunktionZweiteAbleitungNenner;

			graphics->DrawTextS(5, 20 + schrittweiteZwischenDenFunktionen*2, 45, 20, funktionZweiteAbleitungText, false, false, farbeZweiteAbleitung.rot, farbeZweiteAbleitung.gruen, farbeZweiteAbleitung.blau, 10.0, 0);
			graphics->DrawTextS(60, 10 + schrittweiteZwischenDenFunktionen * 2, ((double)maxZeichenFunktionZweiteAbleitungCounter) * schriftWeiteInPixel, 40, funktionZweiteAbleitungTextGebrochenRationalZaehler, false, false, farbeZweiteAbleitung.rot, farbeZweiteAbleitung.gruen, farbeZweiteAbleitung.blau, 10.0, 0);
			graphics->DrawTextS(60, 33 + schrittweiteZwischenDenFunktionen * 2, ((double)maxZeichenFunktionZweiteAbleitungCounter) * schriftWeiteInPixel, 40, funktionZweiteAbleitungTextGebrochenRationalNenner, false, false, farbeZweiteAbleitung.rot, farbeZweiteAbleitung.gruen, farbeZweiteAbleitung.blau, 10.0, 0);
			graphics->DrawLineS(60, 30 + schrittweiteZwischenDenFunktionen * 2, 60 + ((double)maxZeichenFunktionZweiteAbleitungCounter) * schriftWeiteInPixel, 30 + schrittweiteZwischenDenFunktionen * 2, farbeZweiteAbleitung.rot, farbeZweiteAbleitung.gruen, farbeZweiteAbleitung.blau, 10.0f, 0);




			int zeichenCounterFunktionDritteAbleitungZaehler = 0;
			int zeichenCounterFunktionDritteAbleitungNenner = 0;
			int maxZeichenFunktionDritteAbleitungCounter = 0;
			zeichenCounterFunktionDritteAbleitungZaehler = zeichenZaehler(funktionDritteAbleitungTextGebrochenRationalZaehler);
			zeichenCounterFunktionDritteAbleitungNenner = zeichenZaehler(funktionDritteAbleitungTextGebrochenRationalNenner);
			maxZeichenFunktionDritteAbleitungCounter = zeichenCounterFunktionDritteAbleitungZaehler > zeichenCounterFunktionDritteAbleitungNenner ? zeichenCounterFunktionDritteAbleitungZaehler : zeichenCounterFunktionDritteAbleitungNenner;

			graphics->DrawTextS(5, 20 + schrittweiteZwischenDenFunktionen * 3, 45, 20, funktionDritteAbleitungText, false, false, farbeDritteAbleitung.rot, farbeDritteAbleitung.gruen, farbeDritteAbleitung.blau, 10.0, 0);
			graphics->DrawTextS(60, 10 + schrittweiteZwischenDenFunktionen * 3, ((double)maxZeichenFunktionDritteAbleitungCounter) * schriftWeiteInPixel, 40, funktionDritteAbleitungTextGebrochenRationalZaehler, false, false, farbeDritteAbleitung.rot, farbeDritteAbleitung.gruen, farbeDritteAbleitung.blau, 10.0, 0);
			graphics->DrawTextS(60, 33 + schrittweiteZwischenDenFunktionen * 3, ((double)maxZeichenFunktionDritteAbleitungCounter) * schriftWeiteInPixel, 40, funktionDritteAbleitungTextGebrochenRationalNenner, false, false, farbeDritteAbleitung.rot, farbeDritteAbleitung.gruen, farbeDritteAbleitung.blau, 10.0, 0);
			graphics->DrawLineS(60, 30 + schrittweiteZwischenDenFunktionen * 3, 60 + ((double)maxZeichenFunktionDritteAbleitungCounter) * schriftWeiteInPixel, 30 + schrittweiteZwischenDenFunktionen * 3, farbeDritteAbleitung.rot, farbeDritteAbleitung.gruen, farbeDritteAbleitung.blau, 10.0f, 0);

			graphics->DrawTextS(5, 20 + schrittweiteZwischenDenFunktionen * 4, 45, 20, funktionIntegralText, false, false, farbeIntegral.rot, farbeIntegral.gruen, farbeIntegral.blau, 10.0, 0);
		}
		else {
			graphics->DrawTextS(10, 20, 400, 40, funktionText, false, false, farbeFunktion.rot, farbeFunktion.gruen, farbeFunktion.blau, 10.0, 0);
			graphics->DrawTextS(10, 60, 400, 20, funktionErsteAbleitungText, false, false, farbeErsteAbleitung.rot, farbeErsteAbleitung.gruen, farbeErsteAbleitung.blau, 10.0, 0);
			graphics->DrawTextS(10, 100, 400, 20, funktionZweiteAbleitungText, false, false, farbeZweiteAbleitung.rot, farbeZweiteAbleitung.gruen, farbeZweiteAbleitung.blau, 10.0, 0);
			graphics->DrawTextS(10, 150, 400, 20, funktionDritteAbleitungText, false, false, farbeDritteAbleitung.rot, farbeDritteAbleitung.gruen, farbeDritteAbleitung.blau, 10.0, 0);
			graphics->DrawTextS(10, 200, 400, 20, funktionIntegralText, false, false, farbeIntegral.rot, farbeIntegral.gruen, farbeIntegral.blau, 10.0, 0);
		}

		/*for (int i = 0; i < graphics->getMaxCPU(); i++) {
			threadsZeichnen[i] = new thread(&zeichneGraph, i, std::ref(punkte));
			//threadsZeichnen[i]->join();
		}*/
		if (run) {
			for (int i = 1; i < punkte.size(); i++) {
				try {
					if (graphZeichnen) {
						if ((punkte.at(i).y > 30 && punkte.at(i - 1).y < -30) || (punkte.at(i).y < -30 && punkte.at(i - 1).y > 30)) {

						}
						else {
							graphics->DrawLine(-punkte.at(i).x, punkte.at(i).y, -punkte.at(i - 1).x, punkte.at(i - 1).y, farbeFunktion.rot, farbeFunktion.gruen, farbeFunktion.blau, 10.0f, 0);
						}

						if (bogenlaengeZeichnen) {
							if (punkte.at(i).x > linkeGrenze&&punkte.at(i).x < rechteGrenze) {
								graphics->DrawLine(-punkte.at(i).x, punkte.at(i).y, -punkte.at(i - 1).x, punkte.at(i - 1).y, farbeBogenlaenge.rot, farbeBogenlaenge.gruen, farbeBogenlaenge.blau, 10.0f, 0);
							}
						}
						if (flacheninhaltZeichnen) {
							if ((-punkte.at(i).x) < (-linkeGrenze) && (-punkte.at(i).x) > (-rechteGrenze)) {
								graphics->DrawLine(-punkte.at(i).x, punkte.at(i).y, -punkte.at(i).x, 0, farbeIntegralFläche.rot, farbeIntegralFläche.gruen, farbeIntegralFläche.blau, 10.0f, 0);
							}
						}
					}
				}
				catch (exception e) {}
			}
		
			if (steigungsgraphZeichnen) {
				for (int i = 1; i < punkteErsteAbleitung.size(); i++) {
					try {
						graphics->DrawLine(-punkteErsteAbleitung.at(i).x, punkteErsteAbleitung.at(i).y, -punkteErsteAbleitung.at(i - 1).x, punkteErsteAbleitung.at(i - 1).y, farbeErsteAbleitung.rot, farbeErsteAbleitung.gruen, farbeErsteAbleitung.blau, 10.0f, 0);

					}
					catch (exception e) {}
				}
			}
			if (zweiteAbleitungZeichnen) {
				for (int i = 1; i < punkteZweiteAbleitung.size(); i++) {
					try {
						graphics->DrawLine(-punkteZweiteAbleitung.at(i).x, punkteZweiteAbleitung.at(i).y, -punkteZweiteAbleitung.at(i - 1).x, punkteZweiteAbleitung.at(i - 1).y, farbeZweiteAbleitung.rot, farbeZweiteAbleitung.gruen, farbeZweiteAbleitung.blau, 10.0f, 0);

					}
					catch (exception e) {}
				}
			}
			if (dritteAbleitungZeichnen) {
				for (int i = 1; i < punkteDritteAbleitung.size(); i++) {
					try {
						graphics->DrawLine(-punkteDritteAbleitung.at(i).x, punkteDritteAbleitung.at(i).y, -punkteDritteAbleitung.at(i - 1).x, punkteDritteAbleitung.at(i - 1).y, farbeDritteAbleitung.rot, farbeDritteAbleitung.gruen, farbeDritteAbleitung.blau, 10.0f, 0);

					}
					catch (exception e) {}
				}
			}
			if (stammfunktionZeichnen) {
				for (int i = 1; i < punkteStammfunktion.size(); i++) {
					try {
						graphics->DrawLine(-punkteStammfunktion.at(i).x, punkteStammfunktion.at(i).y, -punkteStammfunktion.at(i - 1).x, punkteStammfunktion.at(i - 1).y, farbeIntegral.rot, farbeIntegral.gruen, farbeIntegral.blau, 10.0f, 0);

					}
					catch (exception e) {}
				}
			}
		
			lockTangente.lock();
			if (tangenteZeichnen) {
				for (int i = 1; i < punkteTangente.size(); i++) {
					try {
						graphics->DrawLine(-punkteTangente.at(i).x, punkteTangente.at(i).y, -punkteTangente.at(i - 1).x , punkteTangente.at(i - 1).y, farbeTangente.rot, farbeTangente.gruen, farbeTangente.blau, 10.0f, 0);
					}
					catch (exception e) {}
				}
			}
			if (normaleZeichnen) {
				for (int i = 1; i < punkteNormale.size(); i++) {
					try {
						graphics->DrawLine(-punkteNormale.at(i).x, punkteNormale.at(i).y, -punkteNormale.at(i - 1).x, punkteNormale.at(i - 1).y, farbeNormale.rot, farbeNormale.gruen, farbeNormale.blau, 10.0f, 0);
					}
					catch (exception e) {}
				}
			}
			if (kruemmungsradiusZeichnen) {
				graphics->DrawCircle(xKreis, yKreis, radius, farbeKrümmungskreis.rot, farbeKrümmungskreis.gruen, farbeKrümmungskreis.blau, 0.7f, 0);
				graphics->DrawCircle(xKreis, yKreis, 0.2, farbeKrümmungskreisPunkt.rot, farbeKrümmungskreisPunkt.gruen, farbeKrümmungskreisPunkt.blau, 0.7f, 0);
			}
			lockTangente.unlock();

			lockNullstelle.lock();
			if (nullstellenZeichnen) {
				for (int i = 0; i < rechnerLibrary.getNullstellen().size(); i++) {
					try {
						graphics->DrawCircle(rechnerLibrary.getNullstellen().at(i).x, rechnerLibrary.getNullstellen().at(i).y, 0.5, farbeNullstelle.rot, farbeNullstelle.gruen, farbeNullstelle.blau, farbeNullstelle.alpha, 0);
					}
					catch (exception e) {}
				}
			}
			lockNullstelle.unlock();
			lockPolstelle.lock();
			if (polstellenZeichnen) {
				vector<Vector2D> tmp = rechnerLibrary.getPolstellen();
				for (int i = 0; i < rechnerLibrary.getPolstellen().size(); i++) {
					try {
						graphics->DrawCircle(rechnerLibrary.getPolstellen().at(i).x, rechnerLibrary.getPolstellen().at(i).y, 0.5, farbePolstelle.rot, farbePolstelle.gruen, farbePolstelle.blau, farbePolstelle.alpha, 0);
					}
					catch (exception e) {}
				}
			}
			lockPolstelle.unlock();
			lockExtremstelle.lock();
			if (maximaMinimaZeichnen) {
				for (int i = 0; i < rechnerLibrary.getExtremstellen().size(); i++) {
					try {
						graphics->DrawCircle(rechnerLibrary.getExtremstellen().at(i).x, rechnerLibrary.getExtremstellen().at(i).y, 0.5, farbeExtremstelle.rot, farbeExtremstelle.gruen, farbeExtremstelle.blau, farbeExtremstelle.alpha, 0);
					}
					catch (exception e) {}
				}
			}
			lockExtremstelle.unlock();
			lockWendepunkte.lock();
			if (wendepunkteZeichnen) {
				for (int i = 0; i < rechnerLibrary.getWendepunkte().size(); i++) {
					try {
						graphics->DrawCircle(rechnerLibrary.getWendepunkte().at(i).x, rechnerLibrary.getWendepunkte().at(i).y, 0.5, farbeWendepunkt.rot, farbeWendepunkt.gruen, farbeWendepunkt.blau, farbeWendepunkt.alpha, 0);
					}
					catch (exception e) {}
				}
			}
			if (test) {
				bool unten = false;
				if (testx-0.5 < 0) {
					if (geschw < 0.002) {
						geschw = 0;
						unten = true;
					}
					else {
						geschw = (-1) * geschw * (0.8);
					}
				
				}
			
				clock_t tmpTime = clock();
				clock_t difftime = tmpTime - testtime;
				//difftime = 20;
				double graviFuerVergangeneZeit;
				if (!unten) {
					graviFuerVergangeneZeit = gravi * ((double)difftime / 1000);
					geschw = geschw + graviFuerVergangeneZeit;
					double tmp = testx - geschw;
					if (tmp-0.5 > 0) {
						testx = testx - geschw;
					}
					else {
						testx = 0;
					}
				
				}
				graphics->DrawCircle(testy,testx, 0.5, 0.8f, 0.8f, 0.2f, 10.0f, 0);
			}
			testtime = clock();
		}
		lockWendepunkte.unlock();
		lockFunktion.unlock();
		lockGraph.lock();
		if (run) {
			graphics->EndDraw();
		}
		lockGraph.unlock();
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow){
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	funktionAlsText = new LPWSTR();
	

    // Globale Zeichenfolgen initialisieren
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECT2DPLOTTER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Anwendungsinitialisierung ausführen:
    if (!InitInstance (hInstance, nCmdShow))   {
        return FALSE;
    }
	GetWindowText(hEdit, functionBuffer, 255);
	for (int i = 0; i < 255 && functionBuffer[i]!=0; i++) {
		functionBufferOld[i] = functionBuffer[i];
	}
	FunktionAlsVektorSyntaxbaum baumGebrochenRational;
	rechnerLibrary.splitFuntionBufferGebrochenRational(baumGebrochenRational, functionBuffer);
	rechnerLibrary.setSyntaxbaum(baumGebrochenRational);
	FunktionAlsVektorSyntaxbaum gekuerzterBaum = baumGebrochenRational;
	rechnerLibrary.kuerzeSyntaxbaumGebrochenRational(&gekuerzterBaum);
	rechnerLibrary.setSyntaxbaumGekuerzt(gekuerzterBaum);
	
	/*rechnerLibrary.setFunctionBuffer(functionBuffer);
	lockSyntaxbaum.lock();
	FunktionSyntaxbaum baum;
	//rechnerLibrary.parseFuntionBuffer(baum, functionBuffer);
	rechnerLibrary.setSyntaxbaum(baum);
	//punkte_berechnen();*/
	punkte_berechnenGebrochenRational();
	//lockSyntaxbaum.unlock(); 
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECT2DPLOTTER));
	clearScreen();
    MSG msg;
	

    // Hauptnachrichtenschleife:
    while (GetMessage(&msg, nullptr, 0, 0)){
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNKTION: MyRegisterClass()
//
//  ZWECK: Registriert die Fensterklasse.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECT2DPLOTTER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIRECT2DPLOTTER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	

    return RegisterClassExW(&wcex);
}

void textEingabefeldAendern(std::wstring text) {
	SetWindowText(hEdit, text.c_str());
}

void textNullstellenfeldAendern(std::wstring text) {
	SetWindowText(hNullstellenEditField, text.c_str());
}

void textPolstellenfeldAendern(std::wstring text) {
	SetWindowText(hPolstellenEditField, text.c_str());
}

void textMaximalstellenfeldAendern(std::wstring text) {
	SetWindowText(hMaximaEditField, text.c_str());
}

void textMinimalstellenfeldAendern(std::wstring text) {
	SetWindowText(hMinimaEditField, text.c_str());
}

void textWendepunktefeldAendern(std::wstring text) {
	SetWindowText(hWendepunkteEditField, text.c_str());
}

void buttonsZeichnenSeite1(HWND hwnd) {

	DestroyWindow(hButtonSeite1);

	DestroyWindow(hTextPolynomdivision);
	DestroyWindow(hTextPolynomdivisionDoppelpunkt);
	DestroyWindow(hTextPolynomdivisionErgebnis);

	DestroyWindow(hpolynomDivisorZaehlerEditField);
	DestroyWindow(hpolynomDivisorNennerEditField);
	DestroyWindow(hpolynompolynomDivisorButton);
	DestroyWindow(hpolynomDivisorErgebnisEditField);



	int bc = 0;
	EingabefeldCheckbox = CreateWindowW(L"static", L"f(x)=", WS_VISIBLE | WS_CHILD , 10, 9 + (23 * bc), 35, 20, hwnd, NULL, NULL, NULL);
	hEdit = CreateWindowW(L"edit", L"(x^2+(4*x^3-2*x+5))/(3*x^2+2*(2*x^4+3))", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 45, 9 + (23 * bc), 180, 20, hwnd, NULL, NULL, NULL);
	//hEdit = CreateWindowW(L"edit", L"(x^3+4)/(x^2-3)", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 45, 9 + (23 * bc), 150, 20, hwnd, NULL, NULL, NULL);
	//hEdit = CreateWindowW(L"edit", L"x^3-5*x+2", WS_VISIBLE | WS_CHILD | WS_BORDER, 45, 9 + (23 * bc), 150, 20, hwnd, NULL, NULL, NULL);
	hButton = CreateWindowW(L"button", L"ok", WS_VISIBLE | WS_CHILD | WS_BORDER, 230, 9 + (23 * bc++), 30, 20, hwnd, (HMENU)buttonOkPressed, NULL, NULL);
	hbuttonCheckboxFunktionsgraphAnzeigen = CreateWindowW(L"button", L"Graph anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxFunktionsgraphAnzeigen, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxFunktionsgraphAnzeigen, graphZeichnen);
	hbuttonCheckboxTangente = CreateWindowW(L"button", L"Tangente anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxTangente, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxTangente, tangenteZeichnen);
	hbuttonCheckboxsteigungsgraph = CreateWindowW(L"button", L"Erste Ableitung anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxsteigungsgraph, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxsteigungsgraph, steigungsgraphZeichnen);
	hbuttonCheckboxZweiteAbleitung = CreateWindowW(L"button", L"Zweite Ableitung anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxZweiteAbleitung, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxZweiteAbleitung, zweiteAbleitungZeichnen);
	hbuttonCheckboxDritteAbleitung = CreateWindowW(L"button", L"Dritte Ableitung anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxDritteAbleitung, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxDritteAbleitung, dritteAbleitungZeichnen);
	hbuttonCheckboxNormale = CreateWindowW(L"button", L"Normale anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxNormale, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxNormale, normaleZeichnen);
	hbuttonCheckboxkruemmungsradius = CreateWindowW(L"button", L"Krümmungskreis anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxkruemmungsradius, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxkruemmungsradius, kruemmungsradiusZeichnen);
	


	hMitternachtButton = CreateWindowW(L"button", L"Pol/Nullstellen mit Mitternachtsformel", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonMitternachtsformelPressed, NULL, NULL);
	hPQButton = CreateWindowW(L"button", L"Pol/Nullstellen mit PQ-Formel", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonPQformelPressed, NULL, NULL);
	hNewtonButton = CreateWindowW(L"button", L"Pol/Nullstellen mit Newton Verfahren", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonNewtonVerfahrenPressed, NULL, NULL);
	hRegulaFalsiButton = CreateWindowW(L"button", L"Pol/Nullstellen mit Regula Falsi", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonRegulaFalsiVerfahrenPressed, NULL, NULL);
	hTextNullstellen = CreateWindowW(L"static", L" Nullstellen:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc), 80, 20, hwnd, NULL, NULL, NULL);
	hNullstellenEditField = CreateWindowW(L"edit", L"?", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 9 + (23 * bc++), 160, 20, hwnd, NULL, NULL, NULL);
	hbuttonCheckboxNullstellen = CreateWindowW(L"button", L"Nullstellen anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxNullstellen, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxNullstellen, nullstellenZeichnen);
	hTextPolstellen = CreateWindowW(L"static", L" Polstellen:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc), 80, 20, hwnd, NULL, NULL, NULL);
	hPolstellenEditField = CreateWindowW(L"edit", L"?", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 9 + (23 * bc++), 160, 20, hwnd, NULL, NULL, NULL);
	hbuttonCheckboxPolstellen = CreateWindowW(L"button", L"Polstellen anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxPolstellen, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxPolstellen, polstellenZeichnen);



	hMaximaMinimaButton = CreateWindowW(L"button", L"Maxima/Minima berechnen", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonMaximaMinimaPressed, NULL, NULL);
	hTextMaximas = CreateWindowW(L"static", L" Maximas:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc), 70, 20, hwnd, NULL, NULL, NULL);
	hMaximaEditField = CreateWindowW(L"edit", L"?", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 90, 9 + (23 * bc++), 170, 20, hwnd, NULL, NULL, NULL);
	hTextMinimas = CreateWindowW(L"static", L" Minimas:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc), 70, 20, hwnd, NULL, NULL, NULL);
	hMinimaEditField = CreateWindowW(L"edit", L"?", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 90, 9 + (23 * bc++), 170, 20, hwnd, NULL, NULL, NULL);
	hbuttonCheckboxExtremstellen = CreateWindowW(L"button", L"Extremstellen anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxExtremstellen, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxExtremstellen, maximaMinimaZeichnen);



	hWendepunkteButton = CreateWindowW(L"button", L"Wendepunkte berechnen", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonWendepunktePressed, NULL, NULL);
	hTextWendepunkte = CreateWindowW(L"static", L" Wendepunkte:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc), 100, 20, hwnd, NULL, NULL, NULL);
	hWendepunkteEditField = CreateWindowW(L"edit", L"?", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 120, 9 + (23 * bc++), 140, 20, hwnd, NULL, NULL, NULL);
	hbuttonCheckboxWendepunkte = CreateWindowW(L"button", L"Wendepunkte anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxWendepunkte, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxWendepunkte, wendepunkteZeichnen);

	hTextIntegralgrenzen = CreateWindowW(L"static", L" Integralintervallgrenzen:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc++), 250, 20, hwnd, NULL, NULL, NULL);
	hTextIntegralLinkeGrenze = CreateWindowW(L"static", L" Linke Grenze:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc), 120, 20, hwnd, NULL, NULL, NULL);
	hIntegralLinkeGrenzeEditField = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 9 + (23 * bc++), 120, 20, hwnd, NULL, NULL, NULL);
	hTextIntegralRechteGrenze = CreateWindowW(L"static", L" Rechte Grenze:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc), 120, 20, hwnd, NULL, NULL, NULL);
	hIntegraRechteGrenzeEditField = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 9 + (23 * bc++), 120, 20, hwnd, NULL, NULL, NULL);
	hFlaecheninhaltMitIntegralBerechnen = CreateWindowW(L"button", L"Flächeninhalt mit Integral berechnen", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonflaecheninhaltMitIntegralBerechnen, NULL, NULL);
	hFlaecheninhaltMitSimpsonBerechnen = CreateWindowW(L"button", L"Flächeninhalt mit Simpsonregel ber.", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonflaecheninhaltMitSimpsonBerechnen, NULL, NULL);
	hFlaecheninhaltMitTrapetzBerechnen = CreateWindowW(L"button", L"Flächeninhalt mit Trapetsregel ber.", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonflaecheninhaltMitTrapetzBerechnen, NULL, NULL);
	hTextIntegralFlaeche = CreateWindowW(L"static", L" Fläche:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc), 120, 20, hwnd, NULL, NULL, NULL);
	hFlaecheninhaltAusgebenField = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 9 + (23 * bc++), 120, 20, hwnd, NULL, NULL, NULL);
	hbuttonCheckboxflaecheninhalt = CreateWindowW(L"button", L"Fläche einzeichnen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxflaecheninhalt, NULL, NULL);
	hbuttonCheckboxIntegral = CreateWindowW(L"button", L"Integral einzeichnen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxIntegral, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxflaecheninhalt, flacheninhaltZeichnen);
	CheckDlgButton(hwnd, buttonCheckboxIntegral, stammfunktionZeichnen);

	hBogenlaengeMitTrapetzBerechnen = CreateWindowW(L"button", L"Bogenlänge mit Trapetsregel ber.", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonBogenlaengeMitTrapetzBerechnen, NULL, NULL);
	hTextBogenlaenge = CreateWindowW(L"static", L" Bogenlänge:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc), 120, 20, hwnd, NULL, NULL, NULL);
	hBogenlaengeAusgebenField = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 9 + (23 * bc++), 120, 20, hwnd, NULL, NULL, NULL);
	hbuttonCheckboxBogenlaenge = CreateWindowW(L"button", L"Bogenlänge einzeichnen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxBogenlaenge, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxBogenlaenge, bogenlaengeZeichnen);

	hButtonSeite2 = CreateWindowW(L"button", L"->", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 9 + (23 * bc++), 60, 20, hwnd, (HMENU)buttonSeite2Anzeigen, NULL, NULL);
}

void buttonsZeichnenSeite2(HWND hwnd) {

	DestroyWindow(EingabefeldCheckbox);
	DestroyWindow(hEdit);
	DestroyWindow(hButton);
	DestroyWindow(hbuttonCheckboxFunktionsgraphAnzeigen);
	DestroyWindow(hbuttonCheckboxTangente);
	DestroyWindow(hbuttonCheckboxsteigungsgraph);
	DestroyWindow(hbuttonCheckboxZweiteAbleitung);
	DestroyWindow(hbuttonCheckboxNormale);
	DestroyWindow(hbuttonCheckboxkruemmungsradius);

	DestroyWindow(hMaximaMinimaButton);
	DestroyWindow(hTextMaximas);
	DestroyWindow(hMaximaEditField);
	DestroyWindow(hTextMinimas);
	DestroyWindow(hMinimaEditField);
	DestroyWindow(hbuttonCheckboxExtremstellen);

	DestroyWindow(hMitternachtButton);
	DestroyWindow(hPQButton);
	DestroyWindow(hNewtonButton);
	DestroyWindow(hRegulaFalsiButton);
	DestroyWindow(hTextNullstellen);
	DestroyWindow(hNullstellenEditField);
	DestroyWindow(hbuttonCheckboxNullstellen);
	DestroyWindow(hTextPolstellen);
	DestroyWindow(hPolstellenEditField);
	DestroyWindow(hbuttonCheckboxPolstellen);

	DestroyWindow(hTextIntegralgrenzen);
	DestroyWindow(hTextIntegralLinkeGrenze);
	DestroyWindow(hIntegralLinkeGrenzeEditField);
	DestroyWindow(hTextIntegralRechteGrenze);
	DestroyWindow(hIntegraRechteGrenzeEditField);
	DestroyWindow(hNullstellenEditField);
	DestroyWindow(hbuttonCheckboxNullstellen);

	DestroyWindow(hbuttonCheckboxWendepunkte); 
	DestroyWindow(hWendepunkteButton);
	DestroyWindow(hWendepunkteEditField);

	DestroyWindow(hFlaecheninhaltMitIntegralBerechnen);
	DestroyWindow(hFlaecheninhaltMitSimpsonBerechnen);
	DestroyWindow(hFlaecheninhaltMitTrapetzBerechnen);
	DestroyWindow(hTextIntegralFlaeche);
	DestroyWindow(hFlaecheninhaltAusgebenField);
	DestroyWindow(hbuttonCheckboxflaecheninhalt);
	DestroyWindow(hbuttonCheckboxIntegral);

	DestroyWindow(hBogenlaengeMitTrapetzBerechnen);
	DestroyWindow(hTextBogenlaenge);
	DestroyWindow(hBogenlaengeAusgebenField);
	DestroyWindow(hbuttonCheckboxBogenlaenge);

	DestroyWindow(hButtonSeite2);

	int bc = 0;
	hTextPolynomdivision = CreateWindowW(L"static", L" Polynomdivisor:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc++), 250, 20, hwnd, NULL, NULL, NULL);
	hpolynomDivisorZaehlerEditField = CreateWindowW(L"edit", L"x^3-5*x+2", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 10, 9 + (23 * bc), 200, 20, hwnd, NULL, NULL, NULL);
	hpolynomDivisorNennerEditField = CreateWindowW(L"edit", L"x-2", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 220, 9 + (23 * bc), 40, 20, hwnd, NULL, NULL, NULL);
	hTextPolynomdivisionDoppelpunkt = CreateWindowW(L"static", L" :", WS_VISIBLE | WS_CHILD, 210, 9 + (23 * bc++), 10, 20, hwnd, NULL, NULL, NULL);
	hpolynompolynomDivisorButton = CreateWindowW(L"button", L"Polynomdivision berechnen", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonPolynomdivisionBerechnen, NULL, NULL);
	hTextPolynomdivisionErgebnis = CreateWindowW(L"static", L" Ergebnis:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc), 70, 20, hwnd, NULL, NULL, NULL);
	hpolynomDivisorErgebnisEditField = CreateWindowW(L"edit", L"?", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 80, 9 + (23 * bc++), 180, 20, hwnd, NULL, NULL, NULL);
	hTestField = CreateWindowW(L"button", L"Test", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonTestPressed, NULL, NULL);

	hButtonSeite1 = CreateWindowW(L"button", L"<-", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 9 + (23 * bc++), 60, 20, hwnd, (HMENU)buttonSeite1Anzeigen, NULL, NULL);
}

bool steuerfensterErstellen(int nCmdShow) {
	hWnd2 = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		windowPosX + windowSizeX + 10, windowPosY, windowSizeEingabefeldX, windowSizeEingabefeldY, nullptr, nullptr, hInstEingabe, nullptr);
	if (!hWnd2) {
		return FALSE;
	}
	buttonsZeichnenSeite1(hWnd2);
	
	ShowWindow(hWnd2, nCmdShow);
	UpdateWindow(hWnd2);/**/
}

//
//   FUNKTION: InitInstance(HINSTANCE, int)
//
//   ZWECK: Speichert das Instanzenhandle und erstellt das Hauptfenster.
//
//   KOMMENTARE:
//
//        In dieser Funktion wird das Instanzenhandle in einer globalen Variablen gespeichert, und das
//        Hauptprogrammfenster wird erstellt und angezeigt.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow){
   hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern

   HWND hWnd = CreateWindowEx(0,szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   windowPosX, windowPosY, windowSizeX, windowSizeY, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)   {
      return FALSE;
   }

   graphics = new Graphics();
   threadsZeichnen = new thread*[graphics->getMaxCPU()];

   if (!graphics->Init(hWnd)) {
	   delete graphics;
	   return -1;
   }

   graphics->setCenterPosition(posX, posY);
   graphics->setVergroesserung(vergroesserung);
   graphics->setVergroesserungNormale(vergroesserungNormale);
   
   plotterMainThread = new thread(&plotter_thread);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
 
   if (!steuerfensterErstellen(nCmdShow)) {
	   return FALSE;
   }

   return TRUE;
}



//
//  FUNKTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ZWECK:  Verarbeitet Meldungen vom Hauptfenster.
//
//  WM_COMMAND  - Verarbeiten des Anwendungsmenüs
//  WM_PAINT    - Darstellen des Hauptfensters
//  WM_DESTROY  - Ausgeben einer Beendenmeldung und zurückkehren
//
//

/*double posX = 600;									//Position des geplotteten Teils (mittelpunkt)
double posY = 400;										//Position des geplotteten Teils (mittelpunkt)

int windowSizeX = 1200;									//Fenstergröße
int windowSizeY = 850;									//Fenstergröße*/

int getLaenge(TCHAR* functionbufferL) {
	int i = 0;
	for (i; i < 20 && functionbufferL[i] != 0; i++) {
		
	}
	return i;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	int gwtstat;
	BOOL checkboxChecked=false;
	TCHAR integralText[255];
	std::wstring beschriftung;
	std::wstring tmpBeschriftung;
	LPTSTR endPtr;
	TCHAR polynomdivisionZaehlerText[255];
	TCHAR polynomdivisionNennerText[255];
	FunktionSyntaxbaum baum;
	FunktionAlsVektorSyntaxbaum baumGebrochenRational;
	double flaeche;
	double bogenlaenge;
	vector<double> functionAlsVectorPolynomdivisionZaehler;
	vector<double> functionAlsVectorPolynomdivisionNenner;
	vector<vector<double>> ErgebnisMitRest;
	bool periodErkannt = false;
	int indexLetzteNachkommastelle=0;
	bool hatNachkommastellen=false;
	int indexPeriod = 0;

    switch (message)
    {
	case WM_CREATE:		
		break;
	case WM_LBUTTONDOWN: {
		SetCapture(hWnd);
		posXAlt = LOWORD(lParam);
		posYAlt = HIWORD(lParam);
		//new thread(&thread_mouse_clicked, std::ref(hWnd));
		break;
	}
	case WM_MOUSEMOVE: {
		if (wParam & MK_LBUTTON){
			POINT pt = { LOWORD(lParam), HIWORD(lParam) };
			if (DragDetect(hWnd, pt)) {
				if (toggel == 1) {

					int deltaX = 0;
					int deltaY = 0;
					deltaX = LOWORD(lParam) - posXAlt;
					deltaY = HIWORD(lParam) - posYAlt;
					graphics->moveCenterPosition((deltaX*2), (deltaY*2));
					toggel = 0;
				}
				else if (toggel == 0) {
					SetCapture(hWnd);
					posXAlt = LOWORD(lParam);
					posYAlt = HIWORD(lParam);
					toggel = 1;
				}
			}			
		}
		if (wParam & MK_RBUTTON) {
			if (toggel == 1) {
				int deltaY = 0;
				deltaY = HIWORD(lParam) - posYAlt;
				graphics->SetRotationsWinkel(deltaY);
				toggel = 0;
			}
			else if (toggel == 0) {
				SetCapture(hWnd);
				posYAlt = HIWORD(lParam);
				toggel = 1;
			}
		}
		if (!((wParam & MK_RBUTTON) && (wParam & MK_LBUTTON))) {
			double x = (LOWORD(lParam) - graphics->getCenterPosition().x)/vergroesserung;
			zeichneKreisAmGraph(-x);
		}
	}
	case WM_LBUTTONUP: {
		ReleaseCapture();
	}
	case WM_RBUTTONUP: {
		ReleaseCapture();
	}
	//case WM_CREATE: {
		//DialogBox(hInst, MAKEINTRESOURCE(107), hWnd, About);
	//}
	
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Menüauswahl bearbeiten:
            switch (wmId)
            {
			case 1:
			{
				gwtstat = 0;
				gwtstat = GetWindowText(hEdit, functionBuffer, 255);
				bool neueEingabe = false;
				int zeichenGleich = 0;
				int laengeAlt = getLaenge(functionBufferOld);
				int laengeNeu = getLaenge(functionBuffer);
				int laengste = 0;
				int i = 0;
				if (laengeAlt == laengeNeu) {
					for (i; i < 255 && functionBuffer[i] != 0; i++) {
						if (functionBufferOld[i] == functionBuffer[i]) {
							zeichenGleich++;
						}
					}
					if (i != zeichenGleich) {
						neueEingabe = true;
					}
				}
				else {
					neueEingabe = true;
				}
				if (neueEingabe) {
					if (!validator.checkAufFalscheSymbole(functionBuffer)) {
						MessageBox(
							NULL,
							(LPCWSTR)L"Der Ausdruck enthält ein falsches Symbol. Erlaubt sind +,-,*,/,^,(,),0-9. \r\nBsp: (x^2+3*x)/(x^2-3)\r\nBitte geben Sie einen korrekten Ausdruck ein.",
							(LPCWSTR)L"Fehler",
							MB_ICONWARNING
						);
						break;
					}

					if (!validator.checkKlammernKorrekt(functionBuffer)) {
						MessageBox(
							NULL,
							(LPCWSTR)L"Klammer Fehler! Prüfen Sie, ob zu jeder geöffneten Klammer eine geschlossene eingegeben wurde.",
							(LPCWSTR)L"Fehler",
							MB_ICONWARNING
						);
						break;
					}
					if (!validator.checkDerSyntax(functionBuffer)) {
						MessageBox(
							NULL,
							(LPCWSTR)L"Syntax Fehler! Prüfen Sie, ob der Ausdruck eine korrekte Syntax hat.",
							(LPCWSTR)L"Fehler",
							MB_ICONWARNING
						);
						break;
					}
					
					lockSyntaxbaum.lock();
					if (gwtstat != 0) {
						/*rechnerLibrary.setFunctionBuffer(functionBuffer);
						rechnerLibrary.parseFuntionBuffer(baum, functionBuffer);
						rechnerLibrary.setSyntaxbaum(baum);*/

						// (x^3+2)/(x^2+4) passt
						// ((x^3+2)+4*x^3) 
						// (x^3+2+(4*x^3))
						// (x^3+2+4*x^2) passt
						// x^3*(2*x^3)+4
						// x^3*((x^3+2)+4*x^3) 
						// ((x^3+2)+4*x^3)*3*x
						polstellenPunkteBerechnenErledigt = false;
						rechnerLibrary.splitFuntionBufferGebrochenRational(baumGebrochenRational, functionBuffer);
						rechnerLibrary.setSyntaxbaum(baumGebrochenRational);
						FunktionAlsVektorSyntaxbaum gekuerzterBaum = baumGebrochenRational;
						rechnerLibrary.kuerzeSyntaxbaumGebrochenRational(&gekuerzterBaum);
						rechnerLibrary.setSyntaxbaumGekuerzt(gekuerzterBaum);
					}
					
					lockSyntaxbaum.unlock();
					SetWindowText(hNullstellenEditField, L"?");
					SetWindowText(hPolstellenEditField, L"?");
					SetWindowText(hMaximaEditField, L"?");
					SetWindowText(hMinimaEditField, L"?");
					SetWindowText(hWendepunkteEditField, L"?");
					SetWindowText(hFlaecheninhaltAusgebenField, L"?");
					SetWindowText(hBogenlaengeAusgebenField, L"?");
					rechnerLibrary.resetVals();
					lockSyntaxbaum.lock();
					//punkte_berechnen();
					punkte_berechnenGebrochenRational();
					lockSyntaxbaum.unlock();
					for (int i = 0; i < 20; i++) {
						functionBufferOld[i] = functionBuffer[i];
					}
				}
				break;
			}
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case buttonCheckboxTangente:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxTangente);
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxTangente, BST_UNCHECKED);
					tangenteZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxTangente, BST_CHECKED);
					tangenteZeichnen = true;
				}
				break; 
			case buttonCheckboxNormale:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxNormale);
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxNormale, BST_UNCHECKED);
					normaleZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxNormale, BST_CHECKED);
					normaleZeichnen = true;
				}
				break; 
			case buttonCheckboxsteigungsgraph:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxsteigungsgraph);
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxsteigungsgraph, BST_UNCHECKED);
					steigungsgraphZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxsteigungsgraph, BST_CHECKED);
					steigungsgraphZeichnen = true;
				}
				break;
			case buttonCheckboxZweiteAbleitung:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxZweiteAbleitung);
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxZweiteAbleitung, BST_UNCHECKED);
					zweiteAbleitungZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxZweiteAbleitung, BST_CHECKED);
					zweiteAbleitungZeichnen = true;
				}
				break;
			case buttonCheckboxDritteAbleitung:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxDritteAbleitung);
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxDritteAbleitung, BST_UNCHECKED);
					dritteAbleitungZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxDritteAbleitung, BST_CHECKED);
					dritteAbleitungZeichnen = true;
				}
				break;
			case buttonCheckboxkruemmungsradius:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxkruemmungsradius);
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxkruemmungsradius, BST_UNCHECKED);
					kruemmungsradiusZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxkruemmungsradius, BST_CHECKED);
					kruemmungsradiusZeichnen = true;
				}
				break;

			case buttonMaximaMinimaPressed:
				rechnerLibrary.extremstellenBerechnenGebrochenRational();
				break;
			case buttonMitternachtsformelPressed:
				//rechnerLibrary.mitternachtsformel();
				rechnerLibrary.mitternachtsformelGebrochenRational();
				break;
			case buttonPQformelPressed:
				rechnerLibrary.pqFormelGebrochenRational();
				break;
			case buttonNewtonVerfahrenPressed:
				rechnerLibrary.newtonVerfahrenGebrochenRational();
				//rechnerLibrary.newtonVerfahren();
				break;
			case buttonRegulaFalsiVerfahrenPressed:
				//rechnerLibrary.regulaFalsiVerfahren();
				rechnerLibrary.regulaFalsiVerfahrenGebrochenRational();
				break;
			case buttonCheckboxNullstellen:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxNullstellen);
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxNullstellen, BST_UNCHECKED);
					nullstellenZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxNullstellen, BST_CHECKED);
					nullstellenZeichnen = true;
				}
				break;
			case buttonCheckboxPolstellen:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxPolstellen);
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxPolstellen, BST_UNCHECKED);
					polstellenZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxPolstellen, BST_CHECKED);
					polstellenZeichnen = true;
				}
				break;
			case buttonCheckboxWendepunkte:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxWendepunkte);
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxWendepunkte, BST_UNCHECKED);
					wendepunkteZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxWendepunkte, BST_CHECKED);
					wendepunkteZeichnen = true;
				}
				break;
			case buttonWendepunktePressed:
				rechnerLibrary.wendepunkteBerechnenGebrochenRational();
				break; 
			case buttonCheckboxExtremstellen:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxExtremstellen);
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxExtremstellen, BST_UNCHECKED);
					maximaMinimaZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxExtremstellen, BST_CHECKED);
					maximaMinimaZeichnen = true;
				}
				break;
			case buttonflaecheninhaltMitIntegralBerechnen:
				GetWindowText(hIntegralLinkeGrenzeEditField, integralText, 20);

				linkeGrenze = _tcstod(integralText, &endPtr);
				GetWindowText(hIntegraRechteGrenzeEditField, integralText, 20);
				rechteGrenze = _tcstod(integralText, &endPtr);
				if (linkeGrenze > rechteGrenze) {
					MessageBox(hWnd, L"Fehler! Die linke Grenze ist größer als die rechte Grenze. Bitte korrigieren", L"Fehler.", MB_OK);
					break;
				}
				rechnerLibrary.integrieren(rechnerLibrary.getFunktionAlsVector());
				flaeche = rechnerLibrary.flaecheninhaltBerechnen(linkeGrenze, rechteGrenze);
				beschriftung = std::to_wstring(flaeche);
				SetWindowText(hFlaecheninhaltAusgebenField, beschriftung.c_str());
				break;  
			case buttonCheckboxFunktionsgraphAnzeigen:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxFunktionsgraphAnzeigen);
				rechnerLibrary.integrieren(rechnerLibrary.getFunktionAlsVector());
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxFunktionsgraphAnzeigen, BST_UNCHECKED);
					graphZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxFunktionsgraphAnzeigen, BST_CHECKED);
					graphZeichnen = true;
				}
				break;
			case buttonCheckboxIntegral:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxIntegral);
				rechnerLibrary.integrieren(rechnerLibrary.getFunktionAlsVector());
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxIntegral, BST_UNCHECKED);
					stammfunktionZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxIntegral, BST_CHECKED);
					stammfunktionZeichnen = true;
				}
				break;
			case buttonflaecheninhaltMitSimpsonBerechnen:
				GetWindowText(hIntegralLinkeGrenzeEditField, integralText, 20);

				linkeGrenze = _tcstod(integralText, &endPtr);
				GetWindowText(hIntegraRechteGrenzeEditField, integralText, 20);
				rechteGrenze = _tcstod(integralText, &endPtr);
				if (linkeGrenze > rechteGrenze) {
					MessageBox(hWnd, L"Fehler! Die linke Grenze ist größer als die rechte Grenze. Bitte korrigieren", L"Fehler.", MB_OK);
				}
				flaeche = rechnerLibrary.flaecheninhaltMitSimpsonBerechnenGebrochenRational(linkeGrenze, rechteGrenze);
				beschriftung = std::to_wstring(flaeche);
				SetWindowText(hFlaecheninhaltAusgebenField, beschriftung.c_str());
				break;
			case buttonflaecheninhaltMitTrapetzBerechnen:
				GetWindowText(hIntegralLinkeGrenzeEditField, integralText, 20);

				linkeGrenze = _tcstod(integralText, &endPtr);
				GetWindowText(hIntegraRechteGrenzeEditField, integralText, 20);
				rechteGrenze = _tcstod(integralText, &endPtr);
				if (linkeGrenze > rechteGrenze) {
					MessageBox(hWnd, L"Fehler! Die linke Grenze ist größer als die rechte Grenze. Bitte korrigieren", L"Fehler.", MB_OK);
				}
				flaeche = rechnerLibrary.flaecheninhaltMitTrapetzBerechnenGebrochenRational(linkeGrenze, rechteGrenze);
				beschriftung = std::to_wstring(flaeche);
				SetWindowText(hFlaecheninhaltAusgebenField, beschriftung.c_str());
				break;
			case buttonCheckboxflaecheninhalt:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxflaecheninhalt);
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxflaecheninhalt, BST_UNCHECKED);
					flacheninhaltZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxflaecheninhalt, BST_CHECKED);
					flacheninhaltZeichnen = true;
				}
				break; 
			case buttonBogenlaengeMitTrapetzBerechnen:
				GetWindowText(hIntegralLinkeGrenzeEditField, integralText, 20);

				linkeGrenze = _tcstod(integralText, &endPtr);
				GetWindowText(hIntegraRechteGrenzeEditField, integralText, 20);
				rechteGrenze = _tcstod(integralText, &endPtr);
				if (linkeGrenze > rechteGrenze) {
					MessageBox(hWnd, L"Fehler! Die linke Grenze ist größer als die rechte Grenze. Bitte korrigieren", L"Fehler.", MB_OK);
				}
				bogenlaenge = rechnerLibrary.bogenlaengeMitTrapetzBerechnenGebrochenRational(linkeGrenze, rechteGrenze);
				beschriftung = std::to_wstring(bogenlaenge);
				SetWindowText(hBogenlaengeAusgebenField, beschriftung.c_str());
				break;
			case buttonCheckboxBogenlaenge:
				checkboxChecked = IsDlgButtonChecked(hWnd, buttonCheckboxBogenlaenge);
				if (checkboxChecked) {
					CheckDlgButton(hWnd, buttonCheckboxBogenlaenge, BST_UNCHECKED);
					bogenlaengeZeichnen = false;
				}
				else {
					CheckDlgButton(hWnd, buttonCheckboxBogenlaenge, BST_CHECKED);
					bogenlaengeZeichnen = true;
				}
				break; 
			case buttonPolynomdivisionBerechnen:
				GetWindowText(hpolynomDivisorZaehlerEditField, polynomdivisionZaehlerText, 20);				
				rechnerLibrary.parseFuntionBuffer(baum, polynomdivisionZaehlerText);
				rechnerLibrary.ausSyntaxbaumVektorErstellen(&baum, functionAlsVectorPolynomdivisionZaehler);
				
				GetWindowText(hpolynomDivisorNennerEditField, polynomdivisionNennerText, 20);
				rechnerLibrary.parseFuntionBuffer(baum, polynomdivisionNennerText);
				rechnerLibrary.ausSyntaxbaumVektorErstellen(&baum, functionAlsVectorPolynomdivisionNenner);
				
				ErgebnisMitRest = rechnerLibrary.polynomdivision(functionAlsVectorPolynomdivisionZaehler, functionAlsVectorPolynomdivisionNenner);
				beschriftung = L"";
				//beschriftung = funktionVectorToString(ErgebnisMitRest[0]); todo
				/*for (int i = ErgebnisMitRest[0].size() - 1; i > -1; i--) {
					if (i != ErgebnisMitRest[0].size() - 1 && ErgebnisMitRest[0][i] > 0) {
						beschriftung += '+';
					}
					tmpBeschriftung = std::to_wstring(ErgebnisMitRest[0][i]);
					periodErkannt = false;
					for (int j = 0; j < tmpBeschriftung.size(); j++) {
						if (periodErkannt) {
							if (tmpBeschriftung[j] != '0') {
								indexLetzteNachkommastelle=i;
								hatNachkommastellen=true;
							}
						}
						if (tmpBeschriftung[j] == '.') {
							periodErkannt = true;
							indexPeriod = j;
						}					
					}
					if (hatNachkommastellen) {
						for (int j = 0; j < indexLetzteNachkommastelle; j++) {
							beschriftung += tmpBeschriftung[j];
							beschriftung += '*';
						}
					}
					else {
						for (int j = 0; j < indexPeriod; j++) {							
							if (i == 0) {
								beschriftung += tmpBeschriftung[j];							
							}
							else {
								if (tmpBeschriftung[j] > '1') {
									beschriftung += tmpBeschriftung[j];
									beschriftung += '*';
								}
							}
						}
					}
					

					if (i >= 2) {
						beschriftung += L"x^";
						beschriftung += std::to_wstring(i);
					}
					if (i == 1) {
						beschriftung += L"x";
					}					
				}*/
				{
					bool rest = false;
					for (int i = 0; i < ErgebnisMitRest[1].size(); i++) {
						if (ErgebnisMitRest[1][i] != 0) {
							rest = true;
						}
					}
					if (rest) {
						beschriftung += L" Rest:(";
						wstring tmp = std::to_wstring(ErgebnisMitRest[1][0]);
						for (int i = 0; i < tmp.length(); i++) {
							if (tmp[i] == '.') {
								break;
							}
							beschriftung += tmp[i];
						}

						beschriftung += L")/(";
						beschriftung += polynomdivisionNennerText;
						beschriftung += L")";
					}
					SetWindowText(hpolynomDivisorErgebnisEditField, beschriftung.c_str());
				}
				break;
			case buttonTestPressed:
				testx = 50;
				test = true;
			case buttonSeite2Anzeigen:
			{
				Graphics g;
				g.Init(hWnd2);
				g.BeginDraw();
				g.ClearScreen(1.0f, 1.0f, 1.0f);
				g.EndDraw();
				buttonsZeichnenSeite2(hWnd2);

			}
				break;
			case buttonSeite1Anzeigen:
			{
				Graphics g;
				g.Init(hWnd2);
				g.BeginDraw();
				g.ClearScreen(1.0f, 1.0f, 1.0f);
				g.EndDraw();
				buttonsZeichnenSeite1(hWnd2);

			}
			break;
/*bool tangenteZeichnen = false;
bool steigungsgraphZeichnen = false;
bool maximaMinimaZeichnen = false;
bool nullstellenZeichnen = false;
bool MitternachsformelZeichnen = false;
bool pqZeichnen = false;
bool newtonregelZeichnen = false;
bool simpsonregelZeichnen = false;
bool trapetzregelZeichnen = false;
bool flacheninhaltZeichnen = false;
bool kruemmungsradiusZeichnen = false;
bool bogenlaengeZeichnen = false;
				
HWND hMaximaMinimaButton;
HWND hMaximaEditField;
HWND hMinimaEditField;
HWND hMitternachtButton;
HWND hPQButton;
HWND hNewtonButton;
HWND hRegulaFalsiButton;
HWND hNullstellenEditField;
*/

				
			case IDM_EXIT:
				DestroyWindow(hWndEingabe);
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_MOUSEWHEEL: {
		int tmp = wParam;
		if (tmp >= 0) {
			graphics->setVergroesserung(vergroesserung++);
		}
		else {
			if (vergroesserung > 1) {
				graphics->setVergroesserung(vergroesserung--);
			}
		}
		break;
	}

    case WM_PAINT:
        {
			//clearScreen();
		
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		run = false;
		punkte.clear();
		punkteErsteAbleitung.clear();
		punkteZweiteAbleitung.clear();
		punkteStammfunktion.clear();
		punkteTangente.clear();
		DestroyWindow(hWndEingabe);
        PostQuitMessage(0);
		
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// Meldungshandler für Infofeld.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

