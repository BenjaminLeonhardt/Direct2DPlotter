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

static mutex lockFunktion;
static mutex lockAuswahlPunkt;
static mutex lockTangente;
static mutex lockExtremstelle;
static mutex lockWendepunkte;
static mutex lockNullstelle;
static mutex lockSyntaxbaum;

bool graphZeichnen = true;
bool tangenteZeichnen = false;
bool normaleZeichnen = false;
bool steigungsgraphZeichnen = false;
bool zweiteAbleitungZeichnen = false;
bool maximaMinimaZeichnen = false;
bool wendepunkteZeichnen = false;
bool nullstellenZeichnen = false;
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

HWND hMaximaMinimaButton;
HWND hMaximaEditField;
HWND hMinimaEditField;
HWND hMitternachtButton;
HWND hPQButton;
HWND hNewtonButton;
HWND hRegulaFalsiButton;
HWND hNullstellenEditField;
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
HWND hbuttonCheckboxNormale;
HWND hbuttonCheckboxkruemmungsradius;
HWND hTestField;

HWND hTextMaximas;
HWND hTextMinimas;
HWND hbuttonCheckboxExtremstellen;
HWND hTextNullstellen;
HWND hbuttonCheckboxNullstellen;
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


Graphics* graphics;
thread* plotterMainThread;
static long double PI = 3.141592653589793238463;
static mutex lockGraph;
bool run = true;



int windowSizeX = 1200;									//Fenstergröße
int windowSizeY = 850;									//Fenstergröße

int windowSizeEingabefeldX = 300;						//Fenstergröße
int windowSizeEingabefeldY = 850;						//Fenstergröße

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
						indexLetzteNachkommastelle = j+1;
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

	funktionText = L"f(x)=";
	funktionText += funktionVectorToString(rechnerLibrary.getFunktionAlsVector());

	funktionErsteAbleitungText = L"f '(x)=";
	funktionErsteAbleitungText += funktionVectorToString(rechnerLibrary.getErsteAbleitung());
	funktionZweiteAbleitungText = L"f ''(x)=";
	funktionZweiteAbleitungText += funktionVectorToString(rechnerLibrary.getZweiteAbleitung());
	funktionDritteAbleitungText = L"f '''(x)=";
	funktionDritteAbleitungText += funktionVectorToString(rechnerLibrary.getDritteAbleitung());
	funktionIntegralText = L"F(x)=";
	funktionIntegralText += funktionVectorToString(rechnerLibrary.getStammfunktionAlsVector());

	

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

double radius = 0;
double yKreis = 0;
double xKreis = 0;

void zeichneKreisAmGraph(double x) {
	aktuellerPunktAmGraph.x = -x;
	aktuellerPunktAmGraph.y = rechnerLibrary.f(-x, &rechnerLibrary.getFunktionAlsVector());
	double steigungTangente = rechnerLibrary.f(-x, &rechnerLibrary.getErsteAbleitungAlsVector());
	zeichneTangenteAmPunkt(aktuellerPunktAmGraph.y, steigungTangente);
	
	double yf = rechnerLibrary.f(-x, &rechnerLibrary.getFunktionAlsVector());
	double yfs = rechnerLibrary.f(-x, &rechnerLibrary.getErsteAbleitung());
	double yfz = rechnerLibrary.f(-x, &rechnerLibrary.getZweiteAbleitung());

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

		if (vergroesserung > 20) {
			if (counter == 0) {
				lockSyntaxbaum.lock();
				//punkte_berechnen();
				lockSyntaxbaum.unlock();
				counter++;
			}
		}
		if (vergroesserung < 20) {
			if (counter == 1) {
				lockSyntaxbaum.lock();
				//punkte_berechnen();
				lockSyntaxbaum.unlock();
				counter--;
			}
		}
		lockFunktion.lock();
		graphics->DrawCircle(-aktuellerPunktAmGraph.x, aktuellerPunktAmGraph.y, 1, 0.2f, 0.2f, 0.7f, 1.0f,0);

		graphics->DrawTextS(10, 20, 200, 40, funktionText, false, false, 0.8, 0.8, 0.8, 10.0, 0);
		graphics->DrawTextS(10, 60, 200, 20, funktionErsteAbleitungText, false, false, 0.8, 0.8, 0.8, 10.0, 0);
		graphics->DrawTextS(10, 100, 200, 20, funktionZweiteAbleitungText, false, false, 0.8, 0.8, 0.8, 10.0, 0);
		graphics->DrawTextS(10, 150, 200, 20, funktionDritteAbleitungText, false, false, 0.8, 0.8, 0.8, 10.0, 0);
		graphics->DrawTextS(10, 200, 200, 20, funktionIntegralText, false, false, 0.8, 0.8, 0.8, 10.0, 0);
		/*for (int i = 0; i < graphics->getMaxCPU(); i++) {
			threadsZeichnen[i] = new thread(&zeichneGraph, i, std::ref(punkte));
			//threadsZeichnen[i]->join();
		}*/
		
		for (int i = 1; i < punkte.size(); i++) {
			try {
				if(graphZeichnen){
					graphics->DrawLine(-punkte.at(i).x, punkte.at(i).y, -punkte.at(i-1).x, punkte.at(i-1).y, 0.8f, 0.8f, 0.8f, 10.0f ,0);
					if (bogenlaengeZeichnen) {
						if (punkte.at(i).x > linkeGrenze&&punkte.at(i).x < rechteGrenze) {
							graphics->DrawLine(-punkte.at(i).x, punkte.at(i).y, -punkte.at(i - 1).x, punkte.at(i - 1).y, 0.8f, 0.2f, 0.2f, 10.0f, 0);
						}
					}
					if (flacheninhaltZeichnen) {
						if ((-punkte.at(i).x) < (-linkeGrenze) && (-punkte.at(i).x) > (-rechteGrenze)) {
							graphics->DrawLine(-punkte.at(i).x, punkte.at(i).y, -punkte.at(i).x, 0, 0.8f, 0.8f, 0.8f, 10.0f, 0);
						}
					}
				}
			}
			catch (exception e) {}
		}
		if (steigungsgraphZeichnen) {
			for (int i = 1; i < punkteErsteAbleitung.size(); i++) {
				try {
					graphics->DrawLine(-punkteErsteAbleitung.at(i).x, punkteErsteAbleitung.at(i).y, -punkteErsteAbleitung.at(i - 1).x, punkteErsteAbleitung.at(i - 1).y, 0.8f, 0.8f, 0.8f, 10.0f, 0);

				}
				catch (exception e) {}
			}
		}
		if (zweiteAbleitungZeichnen) {
			for (int i = 1; i < punkteZweiteAbleitung.size(); i++) {
				try {
					graphics->DrawLine(-punkteZweiteAbleitung.at(i).x, punkteZweiteAbleitung.at(i).y, -punkteZweiteAbleitung.at(i - 1).x, punkteZweiteAbleitung.at(i - 1).y, 0.8f, 0.8f, 0.8f, 10.0f, 0);

				}
				catch (exception e) {}
			}
		}
		if (stammfunktionZeichnen) {
			for (int i = 1; i < punkteStammfunktion.size(); i++) {
				try {
					graphics->DrawLine(-punkteStammfunktion.at(i).x, punkteStammfunktion.at(i).y, -punkteStammfunktion.at(i - 1).x, punkteStammfunktion.at(i - 1).y, 0.8f, 0.8f, 0.8f, 10.0f, 0);

				}
				catch (exception e) {}
			}
		}
		
		lockTangente.lock();
		if (tangenteZeichnen) {
			for (int i = 1; i < punkteTangente.size(); i++) {
				try {
					graphics->DrawLine(-punkteTangente.at(i).x, punkteTangente.at(i).y, -punkteTangente.at(i - 1).x , punkteTangente.at(i - 1).y, 0.8f, 0.8f, 0.8f, 10.0f, 0);								
				}
				catch (exception e) {}
			}
		}
		if (normaleZeichnen) {
			for (int i = 1; i < punkteNormale.size(); i++) {
				try {
					graphics->DrawLine(-punkteNormale.at(i).x, punkteNormale.at(i).y, -punkteNormale.at(i - 1).x, punkteNormale.at(i - 1).y, 0.8f, 0.8f, 0.8f, 10.0f, 0);
				}
				catch (exception e) {}
			}
		}
		if (kruemmungsradiusZeichnen) {
			graphics->DrawCircle(xKreis, yKreis, radius, 0.2f, 0.7f, 0.2f, 0.7f, 0);
			graphics->DrawCircle(xKreis, yKreis, 0.2, 0.7f, 0.2f, 0.2f, 0.7f, 0);
		}
		lockTangente.unlock();

		lockNullstelle.lock();
		if (nullstellenZeichnen) {
			for (int i = 0; i < rechnerLibrary.getNullstellen().size(); i++) {
				try {
					graphics->DrawCircle(rechnerLibrary.getNullstellen().at(i).x, rechnerLibrary.getNullstellen().at(i).y, 0.5, 0.8f, 0.2f, 0.2f, 10.0f, 0);
				}
				catch (exception e) {}
			}
		}
		lockNullstelle.unlock();
		lockExtremstelle.lock();
		if (maximaMinimaZeichnen) {
			for (int i = 0; i < rechnerLibrary.getExtremstellen().size(); i++) {
				try {
					graphics->DrawCircle(rechnerLibrary.getExtremstellen().at(i).x, rechnerLibrary.getExtremstellen().at(i).y, 0.5, 0.8f, 0.2f, 0.2f, 10.0f, 0);
				}
				catch (exception e) {}
			}
		}
		lockExtremstelle.unlock();
		lockWendepunkte.lock();
		if (wendepunkteZeichnen) {
			for (int i = 0; i < rechnerLibrary.getWendepunkte().size(); i++) {
				try {
					graphics->DrawCircle(rechnerLibrary.getWendepunkte().at(i).x, rechnerLibrary.getWendepunkte().at(i).y, 0.5, 0.8f, 0.2f, 0.2f, 10.0f, 0);
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
	
    // TODO: Hier Code einfügen.

    // Globale Zeichenfolgen initialisieren
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECT2DPLOTTER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Anwendungsinitialisierung ausführen:
    if (!InitInstance (hInstance, nCmdShow))   {
        return FALSE;
    }
	GetWindowText(hEdit, functionBuffer, 20);
	for (int i = 0; i < 20 && functionBuffer[i]!=0; i++) {
		functionBufferOld[i] = functionBuffer[i];
	}
	
	rechnerLibrary.setFunctionBuffer(functionBuffer);
	lockSyntaxbaum.lock();
	FunktionSyntaxbaum baum;
	rechnerLibrary.parseFuntionBuffer(baum, functionBuffer);
	rechnerLibrary.setSyntaxbaum(baum);
	punkte_berechnen();
	lockSyntaxbaum.unlock();
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
	hEdit = CreateWindowW(L"edit", L"x^3-5*x+2", WS_VISIBLE | WS_CHILD | WS_BORDER, 45, 9 + (23 * bc), 150, 20, hwnd, NULL, NULL, NULL);
	hButton = CreateWindowW(L"button", L"ok", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 9 + (23 * bc++), 30, 20, hwnd, (HMENU)buttonOkPressed, NULL, NULL);
	hbuttonCheckboxFunktionsgraphAnzeigen = CreateWindowW(L"button", L"Graph anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxFunktionsgraphAnzeigen, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxFunktionsgraphAnzeigen, graphZeichnen);
	hbuttonCheckboxTangente = CreateWindowW(L"button", L"Tangente anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxTangente, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxTangente, tangenteZeichnen);
	hbuttonCheckboxsteigungsgraph = CreateWindowW(L"button", L"Erste Ableitung anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxsteigungsgraph, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxsteigungsgraph, steigungsgraphZeichnen);
	hbuttonCheckboxZweiteAbleitung = CreateWindowW(L"button", L"Zweite Ableitung anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxZweiteAbleitung, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxZweiteAbleitung, zweiteAbleitungZeichnen);
	hbuttonCheckboxNormale = CreateWindowW(L"button", L"Normale anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxNormale, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxNormale, normaleZeichnen);
	hbuttonCheckboxkruemmungsradius = CreateWindowW(L"button", L"Krümmungskreis anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxkruemmungsradius, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxkruemmungsradius, kruemmungsradiusZeichnen);
	


	hMitternachtButton = CreateWindowW(L"button", L"Nullstellen mit Mitternachtsformel", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonMitternachtsformelPressed, NULL, NULL);
	hPQButton = CreateWindowW(L"button", L"Nullstellen mit PQ-Formel", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonPQformelPressed, NULL, NULL);
	hNewtonButton = CreateWindowW(L"button", L"Nullstellen mit Newton Verfahren", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonNewtonVerfahrenPressed, NULL, NULL);
	hRegulaFalsiButton = CreateWindowW(L"button", L"Nullstellen mit Regula Falsi", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonRegulaFalsiVerfahrenPressed, NULL, NULL);
	hTextNullstellen = CreateWindowW(L"static", L" Nullstellen:", WS_VISIBLE | WS_CHILD, 10, 9 + (23 * bc), 80, 20, hwnd, NULL, NULL, NULL);
	hNullstellenEditField = CreateWindowW(L"edit", L"?", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 9 + (23 * bc++), 160, 20, hwnd, NULL, NULL, NULL);
	hbuttonCheckboxNullstellen = CreateWindowW(L"button", L"Nullstellen anzeigen", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 9 + (23 * bc++), 250, 20, hwnd, (HMENU)buttonCheckboxNullstellen, NULL, NULL);
	CheckDlgButton(hwnd, buttonCheckboxNullstellen, nullstellenZeichnen);



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

	DestroyWindow(hTextIntegralgrenzen);
	DestroyWindow(hTextIntegralLinkeGrenze);
	DestroyWindow(hIntegralLinkeGrenzeEditField);
	DestroyWindow(hTextIntegralRechteGrenze);
	DestroyWindow(hIntegraRechteGrenzeEditField);
	DestroyWindow(hNullstellenEditField);
	DestroyWindow(hbuttonCheckboxNullstellen);

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
					graphics->moveCenterPosition((deltaX), (deltaY));
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
				gwtstat = GetWindowText(hEdit, functionBuffer, 20);
				bool neueEingabe = true;
				for (int i = 0; i < 20 && functionBuffer[i] != 0; i++) {
					if (functionBufferOld[i] != functionBuffer[i]) {
						neueEingabe = false;
					}
				}
				if (!neueEingabe) {
					lockSyntaxbaum.lock();
					if (gwtstat != 0) {
						rechnerLibrary.setFunctionBuffer(functionBuffer);
						rechnerLibrary.parseFuntionBuffer(baum, functionBuffer);
						rechnerLibrary.setSyntaxbaum(baum);
					}
					lockSyntaxbaum.unlock();
					SetWindowText(hNullstellenEditField, L"?");
					SetWindowText(hMaximaEditField, L"?");
					SetWindowText(hMinimaEditField, L"?");
					SetWindowText(hFlaecheninhaltAusgebenField, L"?");
					rechnerLibrary.resetVals();
					lockSyntaxbaum.lock();
					punkte_berechnen();
					lockSyntaxbaum.unlock();
					for (int i = 0; i < 20 && functionBuffer[i] != 0; i++) {
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
				rechnerLibrary.extremstellenBerechnen(); 
				break;
			case buttonMitternachtsformelPressed:
				rechnerLibrary.mitternachtsformel();
				break;
			case buttonPQformelPressed:
				rechnerLibrary.pqFormel();
				break;
			case buttonNewtonVerfahrenPressed:
				rechnerLibrary.newtonVerfahren();
				break;
			case buttonRegulaFalsiVerfahrenPressed:
				rechnerLibrary.regulaFalsiVerfahren();
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
				rechnerLibrary.wendepunkteBerechnen();
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
				flaeche = rechnerLibrary.flaecheninhaltMitSimpsonBerechnen(linkeGrenze, rechteGrenze);
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
				flaeche = rechnerLibrary.flaecheninhaltMitTrapetzBerechnen(linkeGrenze, rechteGrenze);
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
				bogenlaenge = rechnerLibrary.bogenlaengeMitTrapetzBerechnen(linkeGrenze, rechteGrenze);
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
				beschriftung = funktionVectorToString(ErgebnisMitRest[0]);
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
            //TODO: Zeichencode, der hdc verwendet, hier einfügen...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
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

