#include "stdafx.h"
#include "RechnerLibrary.h"

static mutex lockExtremstelle;
static mutex lockNullstelle;
static mutex newtonLock;
static mutex regulaFalsiLock;

vector<double> funktionAlsVector;
vector<double> funktionAlsVectorErsteAbleitung;
vector<double> funktionAlsVectorZweiteAbleitung;
vector<double> funktionAlsVectorDritteAbleitung;
vector<double> stammfunktionAlsVector;

vector<Vector2D*> newtonNullstellen;
vector<Vector2D*> newtonNullstellenZaehler;
vector<Vector2D*> newtonNullstellenNenner;
vector<Vector2D*> regulaFalsiNullstellen;
vector<Vector2D*> regulaFalsiNullstellenZaehlerGebrochenRational;
vector<Vector2D*> regulaFalsiNullstellenNennerGebrochenRational;

FunktionSyntaxbaum funktionSyntaxbaum;
vector<Vector2D> nullstellen;
vector<Vector2D> polstellen;
vector<Vector2D> extremstellen;
vector<Vector2D> wendepunkte;

FunktionAlsVektorSyntaxbaum funktionAlsVektorSyntaxbaum;
FunktionAlsVektorSyntaxbaum funktionAlsVektorSyntaxbaumGekürzt;
FunktionAlsVektorSyntaxbaum funktionAlsVectorErsteAbleitungGebrochenRational;
FunktionAlsVektorSyntaxbaum funktionAlsVectorZweiteAbleitungGebrochenRational;
FunktionAlsVektorSyntaxbaum funktionAlsVectorDritteAbleitungGebrochenRational;

double tolleranz = 1 * pow(10, -10);
int threadReadyCounterNewton = 0;
int threadReadyCounterRegulaFalsi = 0;
int threadReadyCounterRegulaFalsiZaehler = 0;
int threadReadyCounterRegulaFalsiNenner = 0;

const int sizeFunktionBuffer = 255;
TCHAR* functionAlsString = { 0 };

RechnerLibrary::RechnerLibrary(){
}

RechnerLibrary::~RechnerLibrary(){
}

FunktionSyntaxbaum RechnerLibrary::getSyntaxbaum() {
	return funktionSyntaxbaum;
}

vector<Vector2D>& RechnerLibrary::getNullstellen() {
	return nullstellen;
}

vector<Vector2D>& RechnerLibrary::getPolstellen() {
	return polstellen;
}

vector<Vector2D>& RechnerLibrary::getExtremstellen() {
	return extremstellen;
}

vector<Vector2D>& RechnerLibrary::getWendepunkte() {
	return wendepunkte;
}

vector<double>& RechnerLibrary::getFunktionAlsVector() {
	return funktionAlsVector;
}

vector<double>& RechnerLibrary::getStammfunktionAlsVector() {
	return stammfunktionAlsVector;
}

vector<double>& RechnerLibrary::getErsteAbleitungAlsVector() {
	return funktionAlsVectorErsteAbleitung;
}

void RechnerLibrary::setFunctionBuffer(TCHAR* newFunctionBuffer) {
	functionAlsString = newFunctionBuffer;
}

TCHAR* RechnerLibrary::getFunctionBuffer() {
	return functionAlsString;
}

double RechnerLibrary::f(double x, vector<double> *funktion) {
	double y = 0;
	for (int j = 0; j < funktion->size(); j++) {
		if (j == 0) {
			y += funktion->at(j);
		}
		else {
			if (funktion->at(j) != 0) {
				y += pow(x, j) * funktion->at(j);
			}
		}
	}
	return y;
}

double RechnerLibrary::getPunkt(double x, FunktionAlsVektorSyntaxbaum *funktion) {
	double y1 = 0;
	double y2 = 0;
	double y = 0;
	if (funktion->getInhaltTChar() == '/') {
		y1 = f(x,&funktion->getLinkesChild()->getInhaltVector());
		y2 = f(x, &funktion->getRechtesChild()->getInhaltVector());
		if (y2 != 0) {
			y = y1 / y2;
		}
	}
	if (funktion->getInhaltTChar() == '^') {
		y = f(x, &funktion->getInhaltVector());
	}
	if (funktion->getInhaltTChar() >= '0' &&funktion->getInhaltTChar() <= '9') {
		y = f(x, &funktion->getInhaltVector());
	}
	if (funktion->getInhaltTChar() == '+'|| funktion->getInhaltTChar() == '-') {
		y = f(x, &funktion->getInhaltVector());
	}
	return y;
}

void RechnerLibrary::mitternachtsformel() {
	if (funktionAlsVector.size() == 3) {
		nullstellen.clear();
		double a = funktionAlsVector[2];
		double b = funktionAlsVector[1];
		double c = funktionAlsVector[0];
		double diskriminante = (b * b) - (4 * a * c);
		double wurzel = 0;
		if (diskriminante < 0) {
			textNullstellenfeldAendern(L"f(x) ist keine Quadratische Formel!");
			return;
		}
		else if (diskriminante == 0) {
			if (funktionAlsVector[0] == 0 && funktionAlsVector[1] == 0) {
				textNullstellenfeldAendern(L"x1/2=0");
				return;
			}
			wurzel = 0;
		}
		else if (diskriminante > 0) {
			wurzel = sqrt(diskriminante);
		}
		double nullstelle1 = (((-b) + wurzel) / (2 * a));
		double nullstelle2 = (((-b) - wurzel) / (2 * a));
		Vector2D p1;
		p1.x = -nullstelle1;
		p1.y = f(nullstelle1, &funktionAlsVector);

		Vector2D p2;
		p2.x = -nullstelle2;
		p2.y = f(nullstelle2, &funktionAlsVector);

		nullstellen.push_back(p1);
		nullstellen.push_back(p2);

		std::wstring concartNullstellen;
		concartNullstellen += std::wstring(L"x1=");
		concartNullstellen += std::to_wstring(nullstelle1);
		concartNullstellen += std::wstring(L" ; x2=");
		concartNullstellen += std::to_wstring(nullstelle2);
		textNullstellenfeldAendern(concartNullstellen.c_str());
	}
	else {
		textNullstellenfeldAendern(L"f(x) ist keine Quadratische Formel!");
	
	}
}

vector<Vector2D> RechnerLibrary::mitternachtsformelGebrochenRationalFunktion(vector<double> funktion) {
	if (funktion.size() == 3) {
		nullstellen.clear();
		double a = funktion[2];
		double b = funktion[1];
		double c = funktion[0];
		double diskriminante = (b * b) - (4 * a * c);
		double wurzel = 0;
		if (diskriminante < 0) {
			textNullstellenfeldAendern(L"");
			vector<Vector2D> returnValue;
			return returnValue;
		}
		else if (diskriminante == 0) {
			if (funktion[0] == 0 && funktion[1] == 0) {
				textNullstellenfeldAendern(L"x1/2=0");
				vector<Vector2D> returnValue;
				return returnValue;
			}
			wurzel = 0;
		}
		else if (diskriminante > 0) {
			wurzel = sqrt(diskriminante);
		}
		double nullstelle1 = (((-b) + wurzel) / (2 * a));
		double nullstelle2 = (((-b) - wurzel) / (2 * a));
		Vector2D p1;
		p1.x = -nullstelle1;
		p1.y = f(nullstelle1, &funktion);

		Vector2D p2;
		p2.x = -nullstelle2;
		p2.y = f(nullstelle2, &funktion);

		vector<Vector2D> returnValue;
		returnValue.push_back(p1);
		returnValue.push_back(p2);

		return returnValue;
	}
	else {
		textNullstellenfeldAendern(L"f(x) ist keine Quadratische Formel!");
	}
	vector<Vector2D> returnValue;
	return returnValue;
}

void RechnerLibrary::mitternachtsformelGebrochenRational(){
	nullstellen.clear();
	polstellen.clear();
	RechnerLibrary rechnerLibrary;
	if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltTChar() == '/') {
		FunktionAlsVektorSyntaxbaum* zaehler=rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getLinkesChild();
		FunktionAlsVektorSyntaxbaum* nenner= rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getRechtesChild();
		if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getRechtesChild()->getInhaltVector().size() == 3) {
			vector<Vector2D> polstellenMitternachtsformel = mitternachtsformelGebrochenRationalFunktion(rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getRechtesChild()->getInhaltVector());
			if (polstellenMitternachtsformel.size() > 0) {
				polstellen.push_back(polstellenMitternachtsformel[0]);
				polstellen.push_back(polstellenMitternachtsformel[1]);

				std::wstring concartPolstellen;
				concartPolstellen += std::wstring(L"x1=");
				concartPolstellen += std::to_wstring(-polstellenMitternachtsformel[0].x);
				concartPolstellen += std::wstring(L" ; x2=");
				concartPolstellen += std::to_wstring(-polstellenMitternachtsformel[1].x);
				textPolstellenfeldAendern(concartPolstellen.c_str());
			}
			else {
				textPolstellenfeldAendern(L"");
			}
		}
		else {
			textPolstellenfeldAendern(L"f(x) ist keine Quadratische Formel!");
		}

		if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getLinkesChild()->getInhaltVector().size() == 3) {
			vector<Vector2D> nullstellenMitternachtsformel = mitternachtsformelGebrochenRationalFunktion(rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getLinkesChild()->getInhaltVector());
			if (nullstellenMitternachtsformel.size() > 0) {
				if (polstellen.size() > 1) {
					if (polstellen[0].x != nullstellen[0].x) {
						if (polstellen.size() > 2) {
							if (polstellen[1].x != nullstellen[0].x) {
								nullstellen.push_back(nullstellenMitternachtsformel[0]);
							}
						}
					}
					if (polstellen[0].x != nullstellen[1].x) {
						if (polstellen.size() > 2) {
							if (polstellen[1].x != nullstellen[1].x) {
								nullstellen.push_back(nullstellenMitternachtsformel[1]);
							}
						}
					}
				}
				
				
				

				std::wstring concartNullstellen;
				concartNullstellen += std::wstring(L"x1=");
				concartNullstellen += std::to_wstring(-nullstellenMitternachtsformel[0].x);
				concartNullstellen += std::wstring(L" ; x2=");
				concartNullstellen += std::to_wstring(-nullstellenMitternachtsformel[1].x);
				textNullstellenfeldAendern(concartNullstellen.c_str());
			}
		}
		else {
			textNullstellenfeldAendern(L"f(x) ist keine Quadratische Formel!");
		}

		
	}
	else {
		if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltVector().size() == 3) {
			vector<Vector2D> nullstellenMitternachtsformel = mitternachtsformelGebrochenRationalFunktion(rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltVector());
			if (nullstellenMitternachtsformel.size() > 0) {
				nullstellen.push_back(nullstellenMitternachtsformel[0]);
				nullstellen.push_back(nullstellenMitternachtsformel[1]);

				std::wstring concartNullstellen;
				concartNullstellen += std::wstring(L"x1=");
				concartNullstellen += std::to_wstring(-nullstellenMitternachtsformel[0].x);
				concartNullstellen += std::wstring(L" ; x2=");
				concartNullstellen += std::to_wstring(-nullstellenMitternachtsformel[1].x);
				textNullstellenfeldAendern(concartNullstellen.c_str());
			}
			
		}
		else {
			textNullstellenfeldAendern(L"f(x) ist keine Quadratische Formel!");
		}
	}
}

void RechnerLibrary::pqFormel() {
	if (funktionAlsVector.size() == 3) {
		nullstellen.clear();
		double	q = funktionAlsVector[0] / funktionAlsVector[2];
		double	p = funktionAlsVector[1] / funktionAlsVector[2];

		double diskriminante = ((p / 2)*(p / 2)) - q;
		if (diskriminante < 0) {
			textNullstellenfeldAendern(L"f(x) ist keine Quadratische Formel!");
			return;
		}
		else if (diskriminante == 0) {
			std::wstring concartNullstellen;
			concartNullstellen += std::wstring(L"x1/2=");
			concartNullstellen += std::to_wstring(-(p / 2));
			return;
		}

		double nullstelle1 = -(p / 2) + sqrt(diskriminante);
		double nullstelle2 = -(p / 2) - sqrt(diskriminante);

		Vector2D p1;
		p1.x = -nullstelle1;
		p1.y = f(nullstelle1, &funktionAlsVector);

		Vector2D p2;
		p2.x = -nullstelle2;
		p2.y = f(nullstelle2, &funktionAlsVector);

		nullstellen.push_back(p1);
		nullstellen.push_back(p2);

		std::wstring concartNullstellen;
		concartNullstellen += std::wstring(L"x1=");
		concartNullstellen += std::to_wstring(nullstelle1);
		concartNullstellen += std::wstring(L" ; x2=");
		concartNullstellen += std::to_wstring(nullstelle2);
		textNullstellenfeldAendern(concartNullstellen.c_str());
	}
	else {
		textNullstellenfeldAendern(L"f(x) ist keine Quadratische Formel!");
	}
}

vector<Vector2D> RechnerLibrary::pqFormelGebrochenRationalFunktion(vector<double> funktion) {
	vector<Vector2D> returnValue;
	double	q = funktion[0] / funktion[2];
	double	p = funktion[1] / funktion[2];

	double diskriminante = ((p / 2)*(p / 2)) - q;
	if (diskriminante < 0) {
		textNullstellenfeldAendern(L"");
		return returnValue;
	}
	else if (diskriminante == 0) {
		std::wstring concartNullstellen;
		concartNullstellen += std::wstring(L"x1/2=");
		concartNullstellen += std::to_wstring(-(p / 2));
		return returnValue;
	}

	double nullstelle1 = -(p / 2) + sqrt(diskriminante);
	double nullstelle2 = -(p / 2) - sqrt(diskriminante);

	Vector2D p1;
	p1.x = -nullstelle1;
	p1.y = f(nullstelle1, &funktionAlsVector);

	Vector2D p2;
	p2.x = -nullstelle2;
	p2.y = f(nullstelle2, &funktionAlsVector);


	returnValue.push_back(p1);
	returnValue.push_back(p2);

	return returnValue;
}

void RechnerLibrary::pqFormelGebrochenRational() {
	nullstellen.clear();
	polstellen.clear();
	RechnerLibrary rechnerLibrary;
	if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltTChar() == '/') {
		FunktionAlsVektorSyntaxbaum* zaehler = rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getLinkesChild();
		FunktionAlsVektorSyntaxbaum* nenner = rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getRechtesChild();
		if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getRechtesChild()->getInhaltVector().size() == 3) {
			vector<Vector2D> polstellenPqFormel = pqFormelGebrochenRationalFunktion(rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getRechtesChild()->getInhaltVector());
			if (polstellenPqFormel.size() > 0) {
				polstellen.push_back(polstellenPqFormel[0]);
				polstellen.push_back(polstellenPqFormel[1]);

				std::wstring concartPolstellen;
				concartPolstellen += std::wstring(L"x1=");
				concartPolstellen += std::to_wstring(-polstellenPqFormel[0].x);
				concartPolstellen += std::wstring(L" ; x2=");
				concartPolstellen += std::to_wstring(-polstellenPqFormel[1].x);
				textPolstellenfeldAendern(concartPolstellen.c_str());
			}
			else {
				textPolstellenfeldAendern(L"");
			}
		}
		else {
			textPolstellenfeldAendern(L"f(x) ist keine Quadratische Formel!");
		}

		if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getLinkesChild()->getInhaltVector().size() == 3) {
			vector<Vector2D> nullstellenPqFormel = pqFormelGebrochenRationalFunktion(rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getLinkesChild()->getInhaltVector());
			if (nullstellenPqFormel.size() > 0) {
				if (polstellen.size() > 1) {
					if (polstellen[0].x != nullstellen[0].x) {
						if (polstellen.size() > 2) {
							if (polstellen[1].x != nullstellen[0].x) {
								nullstellen.push_back(nullstellenPqFormel[0]);
							}
						}
					}
					if (polstellen[0].x != nullstellen[1].x) {
						if (polstellen.size() > 2) {
							if (polstellen[1].x != nullstellen[1].x) {
								nullstellen.push_back(nullstellenPqFormel[1]);
							}
						}
					}
				}

				std::wstring concartNullstellen;
				concartNullstellen += std::wstring(L"x1=");
				concartNullstellen += std::to_wstring(-nullstellenPqFormel[0].x);
				concartNullstellen += std::wstring(L" ; x2=");
				concartNullstellen += std::to_wstring(-nullstellenPqFormel[1].x);
				textNullstellenfeldAendern(concartNullstellen.c_str());
			}
		}
		else {
			textNullstellenfeldAendern(L"f(x) ist keine Quadratische Formel!");
		}


	}
	else {
		if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltVector().size() == 3) {
			vector<Vector2D> nullstellenMitternachtsformel = mitternachtsformelGebrochenRationalFunktion(rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltVector());
			if (nullstellenMitternachtsformel.size() > 0) {
				nullstellen.push_back(nullstellenMitternachtsformel[0]);
				nullstellen.push_back(nullstellenMitternachtsformel[1]);

				std::wstring concartNullstellen;
				concartNullstellen += std::wstring(L"x1=");
				concartNullstellen += std::to_wstring(-nullstellenMitternachtsformel[0].x);
				concartNullstellen += std::wstring(L" ; x2=");
				concartNullstellen += std::to_wstring(-nullstellenMitternachtsformel[1].x);
				textNullstellenfeldAendern(concartNullstellen.c_str());
			}

		}
		else {
			textNullstellenfeldAendern(L"f(x) ist keine Quadratische Formel!");
		}
	}
}

void newtonVerfahren_thread(int x) {
	RechnerLibrary rechnerLibrary;
	double xn = x;
	double xn1 = 0;
	int counter = 0;
	double yVonXn1 = 0;
	while ((abs(xn - xn1)>tolleranz || abs(rechnerLibrary.f(xn1, &funktionAlsVector))>tolleranz) && counter<1000) {
		if (counter > 0) {
			xn = xn1;
		}
		xn1 = xn - (rechnerLibrary.f(xn, &funktionAlsVector) / rechnerLibrary.f(xn, &funktionAlsVectorErsteAbleitung));
		yVonXn1 = rechnerLibrary.f(xn1, &funktionAlsVector);
		counter++;
	}
	Vector2D* p1 = new Vector2D();
	p1->x = -xn1;
	p1->y = rechnerLibrary.f(xn1, &funktionAlsVector);
	newtonLock.lock();
	threadReadyCounterNewton--;
	newtonLock.unlock();
	if (abs(p1->y) > tolleranz) {
		
	}
	else if (abs(p1->y) < tolleranz) {
		newtonLock.lock();
		newtonNullstellen.push_back(p1);
		newtonLock.unlock();
	}
}

void RechnerLibrary::newtonVerfahren() {
	vector<thread*>threads;
	newtonNullstellen.clear();
	nullstellen.clear();
	threadReadyCounterNewton = 0;
	for (int i = -30; i < 30; i++) {
		newtonLock.lock();
		thread* t = new thread(&newtonVerfahren_thread, i);
		threads.push_back(t);
		threadReadyCounterNewton++;
		newtonLock.unlock();
	}
	while (threadReadyCounterNewton != 0) {
		Sleep(10);
	}
	for (int i = 0; i < newtonNullstellen.size(); i++) {
		bool tolleranzErreicht = false;
		for (int j = 0; j < nullstellen.size(); j++) {
			if (abs(nullstellen[j].x - newtonNullstellen[i]->x)<tolleranz) { 
				tolleranzErreicht = true;
			}
		}
		if (!tolleranzErreicht) {
			nullstellen.push_back(*newtonNullstellen[i]);
		}
	}
	while (threads.size() > 0) {
		//delete threads[threads.size() - 1];
		threads.pop_back();
	}
	std::wstring concartNullstellen;
	for (int i = 0; i < nullstellen.size(); i++) {
		concartNullstellen += std::wstring(L"x");
		concartNullstellen += std::to_wstring(i + 1);
		concartNullstellen += std::wstring(L"=");
		concartNullstellen += std::to_wstring(-nullstellen[i].x);
		concartNullstellen += std::wstring(L" ");
		if (i != nullstellen.size() - 1) {
			concartNullstellen += std::wstring(L"; ");
		}
	}
	textNullstellenfeldAendern(concartNullstellen.c_str());
}

void newtonVerfahren_threadGebrochenRational(int x) {
	RechnerLibrary rechnerLibrary;
	double xnZaehler = x;
	double xn1Zaehler = 0;
	int counterZaehler = 0;
	double yVonXn1Zaehler = 0;
	FunktionAlsVektorSyntaxbaum* fXZaehler = new FunktionAlsVektorSyntaxbaum();
	FunktionAlsVektorSyntaxbaum* fStrichXZaehler = new FunktionAlsVektorSyntaxbaum();
	if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltTChar() == '/') {
		fXZaehler = rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getLinkesChild();
		fStrichXZaehler = &rechnerLibrary.funktionAbleitenGebrochenRational(*rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getLinkesChild());
	}
	else {
		fXZaehler = &rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt();
		fStrichXZaehler = &rechnerLibrary.funktionAbleitenGebrochenRational(rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt());
	}
	while (rechnerLibrary.getPunkt(xn1Zaehler, fStrichXZaehler) == 0) {
		xn1Zaehler++;
	}
	while ((abs(xnZaehler - xn1Zaehler)>tolleranz || abs(rechnerLibrary.getPunkt(xn1Zaehler, fXZaehler))>tolleranz) && counterZaehler<1000) {
		if (counterZaehler > 0) {
			xnZaehler = xn1Zaehler;
		}
		xn1Zaehler = xnZaehler - (rechnerLibrary.getPunkt(xn1Zaehler, fXZaehler) / rechnerLibrary.getPunkt(xn1Zaehler, fStrichXZaehler));
		yVonXn1Zaehler = rechnerLibrary.getPunkt(xn1Zaehler, fXZaehler);
		counterZaehler++;
	}
	Vector2D* p1Zaehler = new Vector2D();
	p1Zaehler->x = -xn1Zaehler;
	p1Zaehler->y = rechnerLibrary.getPunkt(xn1Zaehler, fXZaehler);
	if (abs(p1Zaehler->y) > tolleranz) {

	}
	else if (abs(p1Zaehler->y) < tolleranz) {
		newtonLock.lock();
		newtonNullstellenZaehler.push_back(p1Zaehler);
		newtonLock.unlock();
	}



	if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltTChar() == '/') {
		double xnNenner = x;
		double xn1Nenner = 0;
		int counterNenner = 0;
		double yVonXn1Nenner = 0;
		double tmp1 = 0;
		double tmp2 = 0;
		FunktionAlsVektorSyntaxbaum* fXNenner = new FunktionAlsVektorSyntaxbaum();
		FunktionAlsVektorSyntaxbaum* fStrichXNenner = new FunktionAlsVektorSyntaxbaum();
		if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltTChar() == '/') {
			fXNenner = rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getRechtesChild();
			fStrichXNenner = &rechnerLibrary.funktionAbleitenGebrochenRational(*rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getRechtesChild());
		}
		while (rechnerLibrary.getPunkt(xn1Nenner, fStrichXNenner) == 0) {
			xn1Nenner++;
		}
		while ((abs(xnNenner - xn1Nenner) > tolleranz || abs(rechnerLibrary.getPunkt(xn1Nenner, fXNenner)) > tolleranz) && counterNenner < 1000) {
			if (counterNenner > 0) {
				xnNenner = xn1Nenner;
			}
			tmp1 = rechnerLibrary.getPunkt(xn1Nenner, fXNenner);
			tmp2 = rechnerLibrary.getPunkt(xn1Nenner, fStrichXNenner);
			xn1Nenner = xnNenner - (tmp1 / tmp2);
			yVonXn1Nenner = rechnerLibrary.getPunkt(xn1Nenner, fXNenner);
			counterNenner++;
		}
		Vector2D* p1Nenner = new Vector2D();
		p1Nenner->x = -xn1Nenner;
		p1Nenner->y = rechnerLibrary.getPunkt(xn1Nenner, fXNenner);

		if (abs(p1Nenner->y) > tolleranz) {

		}
		else if (abs(p1Nenner->y) < tolleranz) {
			newtonLock.lock();
			newtonNullstellenNenner.push_back(p1Nenner);
			newtonLock.unlock();
		}
	}
	newtonLock.lock();
	threadReadyCounterNewton--;
	newtonLock.unlock();
}

void RechnerLibrary::newtonVerfahrenGebrochenRational() {
	vector<thread*>threads;
	newtonNullstellenZaehler.clear();
	newtonNullstellenNenner.clear();
	nullstellen.clear();
	polstellen.clear();
	threadReadyCounterNewton = 0;
	for (int i = -30; i < 30; i++) {
		newtonLock.lock();
		thread* t = new thread(&newtonVerfahren_threadGebrochenRational, i);
		threads.push_back(t);
		threadReadyCounterNewton++;
		newtonLock.unlock();
	}
	while (threadReadyCounterNewton != 0) {
		Sleep(10);
	}
	for (int i = 0; i < newtonNullstellenZaehler.size(); i++) {
		bool tolleranzErreicht = false;
		for (int j = 0; j < nullstellen.size(); j++) {
			if (abs(nullstellen[j].x - newtonNullstellenZaehler[i]->x)<tolleranz) {
				tolleranzErreicht = true;
			}
		}
		if (!tolleranzErreicht) {
			nullstellen.push_back(*newtonNullstellenZaehler[i]);
		}
	}
	for (int i = 0; i < newtonNullstellenNenner.size(); i++) {
		bool tolleranzErreicht = false;
		for (int j = 0; j < polstellen.size(); j++) {
			if (abs(polstellen[j].x - newtonNullstellenNenner[i]->x)<tolleranz) {
				tolleranzErreicht = true;
			}
		}
		if (!tolleranzErreicht) {
			polstellen.push_back(*newtonNullstellenNenner[i]);
		}
	}
	for (int i = 0; i < newtonNullstellenNenner.size(); i++) {
		bool tolleranzErreicht = false;
		for (int j = 0; j < nullstellen.size(); j++) {
			if (abs(nullstellen[j].x - newtonNullstellenNenner[i]->x)<tolleranz) {
				for (int k = 0; k < nullstellen.size(); k++) {
					if (k < j) {
						nullstellen[k] = nullstellen[k];
					}
					else {
						if (k + 1 < nullstellen.size()) {
							nullstellen[k] = nullstellen[k + 1];
						}
					}				
				}
				nullstellen.resize(nullstellen.size() - 1);
			}
		}
	}
	while (threads.size() > 0) {
		//delete threads[threads.size() - 1];
		threads.pop_back();
	}
	std::wstring concartNullstellen;
	for (int i = 0; i < nullstellen.size(); i++) {
		concartNullstellen += std::wstring(L"x");
		concartNullstellen += std::to_wstring(i + 1);
		concartNullstellen += std::wstring(L"=");
		concartNullstellen += std::to_wstring(-nullstellen[i].x);
		concartNullstellen += std::wstring(L" ");
		if (i != nullstellen.size() - 1) {
			concartNullstellen += std::wstring(L"; ");
		}
	}
	textNullstellenfeldAendern(concartNullstellen.c_str());

	std::wstring concartPolstellen;
	for (int i = 0; i < polstellen.size(); i++) {
		concartPolstellen += std::wstring(L"x");
		concartPolstellen += std::to_wstring(i + 1);
		concartPolstellen += std::wstring(L"=");
		concartPolstellen += std::to_wstring(-polstellen[i].x);
		concartPolstellen += std::wstring(L" ");
		if (i != polstellen.size() - 1) {
			concartPolstellen += std::wstring(L"; ");
		}
	}
	textPolstellenfeldAendern(concartPolstellen.c_str());
}

void regulaFalsi_thread(double a, double b, vector<double> &funktion) {
	RechnerLibrary rechnerLibrary;
	double c = 0;
	int counter = 0;
	double fb = rechnerLibrary.f(b, &funktion);
	double fa = rechnerLibrary.f(a, &funktion);
	double fc = 0;
	while (abs(fb) > tolleranz && abs(fa) > tolleranz && counter<1000) {
		fb = rechnerLibrary.f(b, &funktion);
		fa = rechnerLibrary.f(a, &funktion);
		c = b - (((b - a) / (fb - fa))*fb);
		double fc = rechnerLibrary.f(c, &funktion);
		if (fc*fa<0) {
			a = c;
		}
		else if (fc*fb<0) {
			b = c;
		}
		counter++;
	}
	Vector2D *p = new Vector2D();
	if (abs(fa) < tolleranz) {
		p->x = -a;
		p->y = rechnerLibrary.f(a, &funktionAlsVector);
	}
	if (abs(fb) < tolleranz) {
		p->x = -b;
		p->y = rechnerLibrary.f(b, &funktionAlsVector);
	}

	regulaFalsiLock.lock();
	regulaFalsiNullstellen.push_back(p);
	threadReadyCounterRegulaFalsi--;
	regulaFalsiLock.unlock();
}

void RechnerLibrary::regulaFalsiVerfahren() {
	regulaFalsiNullstellen.clear();
	nullstellen.clear();
	polstellen.clear();
	threadReadyCounterRegulaFalsi = 0;
	vector<thread*>threads;
	double y = 0;
	double yOld = 0;
	int rundenCounter = 0;
	for (double i = -30; i < 30; i += 0.1) {
		y = f(i, &funktionAlsVector);
		if (rundenCounter > 0) {
			if (y < 0 && yOld > 0) {
				thread *t = new thread(&regulaFalsi_thread, i, i - 0.1, std::ref(funktionAlsVector));
				threads.push_back(t);
				regulaFalsiLock.lock();
				threadReadyCounterRegulaFalsi++;
				regulaFalsiLock.unlock();
			}
			else if (y > 0 && yOld < 0) {
				thread *t = new thread(&regulaFalsi_thread, i, i - 0.1, std::ref(funktionAlsVector));
				threads.push_back(t);
				regulaFalsiLock.lock();
				threadReadyCounterRegulaFalsi++;
				regulaFalsiLock.unlock();
			}
		}

		yOld = y;
		rundenCounter++;
	}
	while (threadReadyCounterRegulaFalsi != 0) {

	}
	for (int i = 0; i < regulaFalsiNullstellen.size(); i++) {
		bool tolleranzErreicht = false;
		for (int j = 0; j < nullstellen.size(); j++) {
			if (abs(nullstellen[j].x - regulaFalsiNullstellen[i]->x)<tolleranz) {
				tolleranzErreicht = true;
			}
		}
		if (!tolleranzErreicht) {
			if (abs(regulaFalsiNullstellen[i]->y) > tolleranz) {

			}
			else {
				nullstellen.push_back(*regulaFalsiNullstellen[i]);
			}
		}
	}

	while (threads.size() > 0) {
		//delete threads[threads.size()-1];
		threads.pop_back();
	}
	std::wstring concartNullstellen;
	for (int i = 0; i < nullstellen.size(); i++) {
		concartNullstellen += std::wstring(L"x");
		concartNullstellen += std::to_wstring(i + 1);
		concartNullstellen += std::wstring(L"=");
		concartNullstellen += std::to_wstring(-nullstellen[i].x);
		concartNullstellen += std::wstring(L" ");
		if (i != nullstellen.size() - 1) {
			concartNullstellen += std::wstring(L"; ");
		}
	}
	textNullstellenfeldAendern(concartNullstellen.c_str());
}

void regulaFalsi_threadGebrochenRational(double a, double b, vector<double> &funktion, bool nullOderPolstelle) {
	RechnerLibrary rechnerLibrary;
	double c = 0;
	int counter = 0;
	double fb = rechnerLibrary.f(b, &funktion);
	double fa = rechnerLibrary.f(a, &funktion);
	double fc = 0;
	while (abs(fb) > tolleranz && abs(fa) > tolleranz && counter<1000) {
		fb = rechnerLibrary.f(b, &funktion);
		fa = rechnerLibrary.f(a, &funktion);
		c = b - (((b - a) / (fb - fa))*fb);
		double fc = rechnerLibrary.f(c, &funktion);
		if (fc*fa<0) {
			a = c;
		}
		else if (fc*fb<0) {
			b = c;
		}
		counter++;
	}
	Vector2D *p = new Vector2D();
	if (abs(fa) < tolleranz) {
		p->x = -a;
		p->y = rechnerLibrary.f(a, &funktion);
	}
	if (abs(fb) < tolleranz) {
		p->x = -b;
		p->y = rechnerLibrary.f(b, &funktion);
	}

	regulaFalsiLock.lock();
	if (nullOderPolstelle) {
		regulaFalsiNullstellenZaehlerGebrochenRational.push_back(p);
		threadReadyCounterRegulaFalsiZaehler--;
	}
	else {
		regulaFalsiNullstellenNennerGebrochenRational.push_back(p);
		threadReadyCounterRegulaFalsiNenner--;
	}
	
	regulaFalsiLock.unlock();
}

void RechnerLibrary::regulaFalsiVerfahrenGebrochenRational() {
	regulaFalsiNullstellenZaehlerGebrochenRational.clear();
	regulaFalsiNullstellenNennerGebrochenRational.clear();
	nullstellen.clear();
	polstellen.clear();
	threadReadyCounterRegulaFalsiZaehler = 0;
	threadReadyCounterRegulaFalsiNenner = 0;
	RechnerLibrary rechnerLibrary;
	vector<double> zaehler;
	vector<double> nenner;
	if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltTChar() == '/') {
		zaehler = rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getLinkesChild()->getInhaltVector();
		nenner = rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getRechtesChild()->getInhaltVector();
	}
	else {
		if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltVector().size() == 0) {
			zaehler = rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getLinkesChild()->getInhaltVector();
		}
		else {
			zaehler = rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltVector();
		}
	}
	vector<thread*>threads;
	double y = 0;
	double yOld = 0;
	int rundenCounter = 0;
	bool uebergangGefunden = false;
	for (double i = -30; i < 30; i += 0.1) {
		y = f(i, &zaehler);
		if (rundenCounter > 0) {
			if (y < 0 && yOld > 0) {
				thread *t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(zaehler), true);
				threads.push_back(t);
				regulaFalsiLock.lock();
				threadReadyCounterRegulaFalsiZaehler++;
				regulaFalsiLock.unlock();
				uebergangGefunden = true;
			}
			else if (y > 0 && yOld < 0) {
				thread *t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(zaehler), true);
				threads.push_back(t);
				regulaFalsiLock.lock();
				threadReadyCounterRegulaFalsiZaehler++;
				regulaFalsiLock.unlock();
				uebergangGefunden = true;
			}
		}

		yOld = y;
		rundenCounter++;
	}
	if (uebergangGefunden == false) {
		for (double i = -30; i < 30; i += 0.1) {
			thread *t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(zaehler), true);
			threads.push_back(t);
			regulaFalsiLock.lock();
			threadReadyCounterRegulaFalsiZaehler++;
			regulaFalsiLock.unlock();
		}
		yOld = y;
		rundenCounter++;
		
	}
	uebergangGefunden = false;
	if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltTChar() == '/') {
		double y = 0;
		double yOld = 0;
		int rundenCounter = 0;
		for (double i = -30; i < 30; i += 0.1) {
			y = f(i, &nenner);
			if (rundenCounter > 0) {
				if (y < 0 && yOld > 0) {
					thread *t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(nenner), false);
					threads.push_back(t);
					regulaFalsiLock.lock();
					threadReadyCounterRegulaFalsiNenner++;
					regulaFalsiLock.unlock();
					uebergangGefunden = true;
				}
				else if (y > 0 && yOld < 0) {
					thread *t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(nenner), false);
					threads.push_back(t);
					regulaFalsiLock.lock();
					threadReadyCounterRegulaFalsiNenner++;
					regulaFalsiLock.unlock();
					uebergangGefunden = true;
				}
			}

			yOld = y;
			rundenCounter++;
		}
	}
	if (uebergangGefunden == false) {
		for (double i = -30; i < 30; i += 0.1) {
			thread *t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(zaehler), true);
			threads.push_back(t);
			regulaFalsiLock.lock();
			threadReadyCounterRegulaFalsiZaehler++;
			regulaFalsiLock.unlock();
		}
		yOld = y;
		rundenCounter++;

	}
	while (threadReadyCounterRegulaFalsiZaehler != 0) {
		if (rechnerLibrary.getSyntaxbaumGebrochenRationalGekürzt().getInhaltTChar() == '/') {
			while (threadReadyCounterRegulaFalsiNenner != 0) {}
		}
	}

	for (int i = 0; i < regulaFalsiNullstellenZaehlerGebrochenRational.size(); i++) {
		bool tolleranzErreicht = false;
		for (int j = 0; j < nullstellen.size(); j++) {
			if (abs(nullstellen[j].x - regulaFalsiNullstellenZaehlerGebrochenRational[i]->x)<tolleranz) {
				tolleranzErreicht = true;
			}
		}
		if (!tolleranzErreicht) {
			nullstellen.push_back(*regulaFalsiNullstellenZaehlerGebrochenRational[i]);
		}
	}
	for (int i = 0; i < regulaFalsiNullstellenNennerGebrochenRational.size(); i++) {
		bool tolleranzErreicht = false;
		for (int j = 0; j < polstellen.size(); j++) {
			if (abs(polstellen[j].x - regulaFalsiNullstellenNennerGebrochenRational[i]->x)<tolleranz) {
				tolleranzErreicht = true;
			}
		}
		if (!tolleranzErreicht) {
			polstellen.push_back(*regulaFalsiNullstellenNennerGebrochenRational[i]);
		}
	}
	for (int i = 0; i < regulaFalsiNullstellenNennerGebrochenRational.size(); i++) {
		bool tolleranzErreicht = false;
		for (int j = 0; j < nullstellen.size(); j++) {
			if (abs(nullstellen[j].x - regulaFalsiNullstellenNennerGebrochenRational[i]->x)<tolleranz) {
				for (int k = 0; k < nullstellen.size(); k++) {
					if (k < j) {
						nullstellen[k] = nullstellen[k];
					}
					else {
						if (k + 1 < nullstellen.size()) {
							nullstellen[k] = nullstellen[k + 1];
						}
					}
				}
				nullstellen.resize(nullstellen.size() - 1);
			}
		}
	}
	for (int j = 0; j < nullstellen.size(); j++) {
		if (abs(nullstellen[j].x)>1000) {
			for (int k = 0; k < nullstellen.size(); k++) {
				if (k < j) {
					nullstellen[k] = nullstellen[k];
				}
				else {
					if (k + 1 < nullstellen.size()) {
						nullstellen[k] = nullstellen[k + 1];
					}
				}
			}
			nullstellen.resize(nullstellen.size() - 1);
		}
	}

	while (threads.size() > 0) {
		//delete threads[threads.size()-1];
		threads.pop_back();
	}
	std::wstring concartNullstellen;
	for (int i = 0; i < nullstellen.size(); i++) {
		concartNullstellen += std::wstring(L"x");
		concartNullstellen += std::to_wstring(i + 1);
		concartNullstellen += std::wstring(L"=");
		concartNullstellen += std::to_wstring(-nullstellen[i].x);
		concartNullstellen += std::wstring(L" ");
		if (i != nullstellen.size() - 1) {
			concartNullstellen += std::wstring(L"; ");
		}
	}
	textNullstellenfeldAendern(concartNullstellen.c_str());
	std::wstring concartPolstellen;
	for (int i = 0; i < polstellen.size(); i++) {
		concartPolstellen += std::wstring(L"x");
		concartPolstellen += std::to_wstring(i + 1);
		concartPolstellen += std::wstring(L"=");
		concartPolstellen += std::to_wstring(-polstellen[i].x);
		concartPolstellen += std::wstring(L" ");
		if (i != polstellen.size() - 1) {
			concartPolstellen += std::wstring(L"; ");
		}
	}
	textPolstellenfeldAendern(concartPolstellen.c_str());
}

void RechnerLibrary::extremstellenBerechnen() {
	regulaFalsiNullstellen.clear();
	extremstellen.clear();
	vector<thread*>threads;
	double y = f(-30, &funktionAlsVectorErsteAbleitung);
	double yOld = f(-30, &funktionAlsVectorErsteAbleitung);
	int rundenCounter = 0;
	for (double i = -30; i < 30; i += 0.1) {
		y = f(i, &funktionAlsVectorErsteAbleitung);
		if (rundenCounter > 0) {
			if (y < 0 && yOld > 0) {
				thread *t = new thread(&regulaFalsi_thread, i, i - 0.1, std::ref(funktionAlsVectorErsteAbleitung));
				threads.push_back(t);
				regulaFalsiLock.lock();
				threadReadyCounterRegulaFalsi++;
				regulaFalsiLock.unlock();
			}
			else if (y > 0 && yOld < 0) {
				thread *t = new thread(&regulaFalsi_thread, i, i - 0.1, std::ref(funktionAlsVectorErsteAbleitung));
				threads.push_back(t);
				regulaFalsiLock.lock();
				threadReadyCounterRegulaFalsi++;
				regulaFalsiLock.unlock();
			}
		}

		yOld = y;
		rundenCounter++;
	}
	while (threadReadyCounterRegulaFalsi != 0) {

	}
	for (int i = 0; i < regulaFalsiNullstellen.size(); i++) {
		bool tolleranzErreicht = false;
		for (int j = 0; j < extremstellen.size(); j++) {
			if (abs(extremstellen[j].x - regulaFalsiNullstellen[i]->x)<tolleranz) {
				tolleranzErreicht = true;
			}
		}
		if (!tolleranzErreicht) {
			extremstellen.push_back(*regulaFalsiNullstellen[i]);
		}
	}
	std::wstring concartHochpunkte;
	vector<Vector2D> hochpunkte;

	std::wstring concartTiefpunkte;
	vector<Vector2D> tiefpunkte;


	for (int i = 0; i < extremstellen.size(); i++) {
		if (abs(f((-extremstellen[i].x), &funktionAlsVectorErsteAbleitung)) < tolleranz) {
			if (f((-extremstellen[i].x), &funktionAlsVectorZweiteAbleitung) < 0) {
				hochpunkte.push_back(extremstellen[i]);
			}
			if (f((-extremstellen[i].x), &funktionAlsVectorZweiteAbleitung) > 0) {
				tiefpunkte.push_back(extremstellen[i]);
			}
		}
	}
	int countPunkte = 1;
	for (int i = 0; i < hochpunkte.size(); i++) {
		concartHochpunkte += std::wstring(L"h");
		concartHochpunkte += std::to_wstring(countPunkte++);
		concartHochpunkte += std::wstring(L"(");
		concartHochpunkte += std::to_wstring(-hochpunkte[i].x);
		concartHochpunkte += std::wstring(L" / ");
		concartHochpunkte += std::to_wstring(hochpunkte[i].y);
		concartHochpunkte += std::wstring(L") ");
		if (i != hochpunkte.size() - 1) {
			concartHochpunkte += std::wstring(L"; ");
		}
	}
	for (int i = 0; i < tiefpunkte.size(); i++) {
		concartTiefpunkte += std::wstring(L"t");
		concartTiefpunkte += std::to_wstring(countPunkte++);
		concartTiefpunkte += std::wstring(L"(");
		concartTiefpunkte += std::to_wstring(-tiefpunkte[i].x);
		concartTiefpunkte += std::wstring(L" / ");
		concartTiefpunkte += std::to_wstring(tiefpunkte[i].y);
		concartTiefpunkte += std::wstring(L") ");
		if (i != tiefpunkte.size() - 1) {
			concartTiefpunkte += std::wstring(L"; ");
		}
	}

	textMaximalstellenfeldAendern(concartHochpunkte.c_str());
	textMinimalstellenfeldAendern(concartTiefpunkte.c_str());
}

void RechnerLibrary::extremstellenBerechnenGebrochenRational() {
	regulaFalsiNullstellenZaehlerGebrochenRational.clear();
	extremstellen.clear();
	vector<thread*>threads;
	double y = 0;
	double yOld = 0;
	if (funktionAlsVektorSyntaxbaumGekürzt.getInhaltTChar() == '/') {
		y = f(-30, &funktionAlsVectorErsteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector());
		yOld = f(-30, &funktionAlsVectorErsteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector());
	}
	else {
		y = f(-30, &funktionAlsVectorErsteAbleitungGebrochenRational.getInhaltVector());
		yOld = f(-30, &funktionAlsVectorErsteAbleitungGebrochenRational.getInhaltVector());
	}

	int rundenCounter = 0;
	for (double i = -30; i < 30; i += 0.1) {
		if (i >= -0.4) {
			int a = 0;
		}
		if (funktionAlsVektorSyntaxbaumGekürzt.getInhaltTChar() == '/') {
			y = f(i, &funktionAlsVectorErsteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector());
		}
		else {
			y = f(i, &funktionAlsVectorErsteAbleitungGebrochenRational.getInhaltVector());
		}
		if (rundenCounter > 0) {
			thread *t = nullptr;
			vector<double>* zaehler = new vector<double>;
			if (y < 0 && yOld > 0) {
				if (funktionAlsVektorSyntaxbaumGekürzt.getInhaltTChar() == '/') {
					*zaehler = funktionAlsVectorErsteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector();
					t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(*zaehler), true);
				}
				else {
					*zaehler = funktionAlsVectorErsteAbleitungGebrochenRational.getInhaltVector();
					t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(*zaehler), true);
				}
				threads.push_back(t);
				regulaFalsiLock.lock();
				threadReadyCounterRegulaFalsiZaehler++;
				regulaFalsiLock.unlock();
			}
			else if (y > 0 && yOld < 0) {
				if (funktionAlsVektorSyntaxbaumGekürzt.getInhaltTChar() == '/') {
					*zaehler = funktionAlsVectorErsteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector();
					t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(*zaehler), true);
				}
				else {
					*zaehler = funktionAlsVectorErsteAbleitungGebrochenRational.getInhaltVector();
					t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(*zaehler), true);
				}
				threads.push_back(t);
				regulaFalsiLock.lock();
				threadReadyCounterRegulaFalsiZaehler++;
				regulaFalsiLock.unlock();
			}
		}

		yOld = y;
		rundenCounter++;
	}
	while (threadReadyCounterRegulaFalsiZaehler != 0) {
		Sleep(10);
	}
	for (int i = 0; i < regulaFalsiNullstellenZaehlerGebrochenRational.size(); i++) {
		bool tolleranzErreicht = false;
		for (int j = 0; j < extremstellen.size(); j++) {
			if (abs(extremstellen[j].x - regulaFalsiNullstellenZaehlerGebrochenRational[i]->x)<tolleranz) {
				tolleranzErreicht = true;
			}
		}
		if (!tolleranzErreicht) {
			extremstellen.push_back(*regulaFalsiNullstellenZaehlerGebrochenRational[i]);
		}
	}
	std::wstring concartHochpunkte;
	vector<Vector2D> hochpunkte;

	std::wstring concartTiefpunkte;
	vector<Vector2D> tiefpunkte;
	RechnerLibrary rechnerLibrary;

	for (int i = 0; i < extremstellen.size(); i++) {
		if (funktionAlsVektorSyntaxbaumGekürzt.getInhaltTChar() == '/') {
			if (abs(f((-extremstellen[i].x), &funktionAlsVectorErsteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector())) < tolleranz) {
				if (f((-extremstellen[i].x), &funktionAlsVectorZweiteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector()) < 0) {
					extremstellen[i].y = rechnerLibrary.getPunkt(-extremstellen[i].x, &funktionAlsVektorSyntaxbaumGekürzt);
					hochpunkte.push_back(extremstellen[i]);
					
				}
				if (f((-extremstellen[i].x), &funktionAlsVectorZweiteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector()) > 0) {
					extremstellen[i].y = rechnerLibrary.getPunkt(-extremstellen[i].x, &funktionAlsVektorSyntaxbaumGekürzt);
					tiefpunkte.push_back(extremstellen[i]);
				}
			}
		}
		else {
			if (abs(f((-extremstellen[i].x), &funktionAlsVectorErsteAbleitungGebrochenRational.getInhaltVector())) < tolleranz) {
				if (f((-extremstellen[i].x), &funktionAlsVectorZweiteAbleitungGebrochenRational.getInhaltVector()) < 0) {
					extremstellen[i].y = rechnerLibrary.getPunkt(-extremstellen[i].x, &funktionAlsVektorSyntaxbaumGekürzt);
					hochpunkte.push_back(extremstellen[i]);
				}
				if (f((-extremstellen[i].x), &funktionAlsVectorZweiteAbleitungGebrochenRational.getInhaltVector()) > 0) {
					extremstellen[i].y = rechnerLibrary.getPunkt(-extremstellen[i].x, &funktionAlsVektorSyntaxbaumGekürzt);
					tiefpunkte.push_back(extremstellen[i]);
				}
			}
		}
	}
	int countPunkte = 1;
	for (int i = 0; i < hochpunkte.size(); i++) {
		concartHochpunkte += std::wstring(L"h");
		concartHochpunkte += std::to_wstring(countPunkte++);
		concartHochpunkte += std::wstring(L"(");
		concartHochpunkte += std::to_wstring(-hochpunkte[i].x);
		concartHochpunkte += std::wstring(L" / ");
		concartHochpunkte += std::to_wstring(hochpunkte[i].y);
		concartHochpunkte += std::wstring(L") ");
		if (i != hochpunkte.size() - 1) {
			concartHochpunkte += std::wstring(L"; ");
		}
	}
	for (int i = 0; i < tiefpunkte.size(); i++) {
		concartTiefpunkte += std::wstring(L"t");
		concartTiefpunkte += std::to_wstring(countPunkte++);
		concartTiefpunkte += std::wstring(L"(");
		concartTiefpunkte += std::to_wstring(-tiefpunkte[i].x);
		concartTiefpunkte += std::wstring(L" / ");
		concartTiefpunkte += std::to_wstring(tiefpunkte[i].y);
		concartTiefpunkte += std::wstring(L") ");
		if (i != tiefpunkte.size() - 1) {
			concartTiefpunkte += std::wstring(L"; ");
		}
	}

	textMaximalstellenfeldAendern(concartHochpunkte.c_str());
	textMinimalstellenfeldAendern(concartTiefpunkte.c_str());
}

void textWendepunktefeldAendern(std::wstring text);

void RechnerLibrary::wendepunkteBerechnen() {
	regulaFalsiNullstellen.clear();
	wendepunkte.clear();
	vector<thread*>threads;
	double y = f(-30, &funktionAlsVectorZweiteAbleitung);
	double yOld = f(-30, &funktionAlsVectorZweiteAbleitung);
	int rundenCounter = 0;
	for (double i = -30; i < 30; i += 0.1) {
		y = f(i, &funktionAlsVectorZweiteAbleitung);
		if (rundenCounter > 0) {
			if (y < 0 && yOld > 0) {
				thread *t = new thread(&regulaFalsi_thread, i, i - 0.1, std::ref(funktionAlsVectorZweiteAbleitung));
				threads.push_back(t);
				regulaFalsiLock.lock();
				threadReadyCounterRegulaFalsi++;
				regulaFalsiLock.unlock();
			}
			else if (y > 0 && yOld < 0) {
				thread *t = new thread(&regulaFalsi_thread, i, i - 0.1, std::ref(funktionAlsVectorZweiteAbleitung));
				threads.push_back(t);
				regulaFalsiLock.lock();
				threadReadyCounterRegulaFalsi++;
				regulaFalsiLock.unlock();
			}
		}

		yOld = y;
		rundenCounter++;
	}
	while (threadReadyCounterRegulaFalsi != 0) {

	}
	for (int i = 0; i < regulaFalsiNullstellen.size(); i++) {
		bool tolleranzErreicht = false;
		for (int j = 0; j < wendepunkte.size(); j++) {
			if (abs(wendepunkte[j].x - regulaFalsiNullstellen[i]->x)<tolleranz) {
				tolleranzErreicht = true;
			}
		}
		if (!tolleranzErreicht) {
			if (abs(f((-regulaFalsiNullstellen[i]->x), &funktionAlsVectorZweiteAbleitung)) < tolleranz) {
				if (f((-regulaFalsiNullstellen[i]->x), &funktionAlsVectorDritteAbleitung) > tolleranz) {
					wendepunkte.push_back(*regulaFalsiNullstellen[i]);
				}
			}			
		}
	}
	
	std::wstring concartWendepunkte;

	for (int i = 0; i < wendepunkte.size(); i++) {
		concartWendepunkte += std::wstring(L"w");
		concartWendepunkte += std::to_wstring(i + 1);
		concartWendepunkte += std::wstring(L"(");
		concartWendepunkte += std::to_wstring(-wendepunkte[i].x);
		concartWendepunkte += std::wstring(L" / ");
		concartWendepunkte += std::to_wstring(wendepunkte[i].y);
		concartWendepunkte += std::wstring(L") ");
		if (i != wendepunkte.size() - 1) {
			concartWendepunkte += std::wstring(L"; ");
		}
	}
	textWendepunktefeldAendern(concartWendepunkte.c_str());
}

void RechnerLibrary::wendepunkteBerechnenGebrochenRational() {
	regulaFalsiNullstellenZaehlerGebrochenRational.clear();
	wendepunkte.clear();
	vector<thread*>threads;
	double y = 0;
	double yOld = 0;
	if (funktionAlsVektorSyntaxbaumGekürzt.getInhaltTChar() == '/') {
		y = f(-30, &funktionAlsVectorZweiteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector());
		yOld = f(-30, &funktionAlsVectorZweiteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector());
	}
	else {
		y = f(-30, &funktionAlsVectorZweiteAbleitungGebrochenRational.getInhaltVector());
		yOld = f(-30, &funktionAlsVectorZweiteAbleitungGebrochenRational.getInhaltVector());
	}

	int rundenCounter = 0;
	for (double i = -30; i < 30; i += 0.1) {
		if (i >= -0.4) {
			int a = 0;
		}
		if (funktionAlsVektorSyntaxbaumGekürzt.getInhaltTChar() == '/') {
			y = f(i, &funktionAlsVectorZweiteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector());
		}
		else {
			y = f(i, &funktionAlsVectorZweiteAbleitungGebrochenRational.getInhaltVector());
		}
		if (rundenCounter > 0) {
			thread *t = nullptr;
			vector<double>* zaehler = new vector<double>;
			if (y < 0 && yOld > 0) {
				if (funktionAlsVektorSyntaxbaumGekürzt.getInhaltTChar() == '/') {
					*zaehler = funktionAlsVectorZweiteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector();
					t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(*zaehler), true);
				}
				else {
					*zaehler = funktionAlsVectorZweiteAbleitungGebrochenRational.getInhaltVector();
					t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(*zaehler), true);
				}
				threads.push_back(t);
				regulaFalsiLock.lock();
				threadReadyCounterRegulaFalsiZaehler++;
				regulaFalsiLock.unlock();
			}
			else if (y > 0 && yOld < 0) {
				if (funktionAlsVektorSyntaxbaumGekürzt.getInhaltTChar() == '/') {
					*zaehler = funktionAlsVectorZweiteAbleitungGebrochenRational.getLinkesChild()->getInhaltVector();
					t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(*zaehler), true);
				}
				else {
					*zaehler = funktionAlsVectorZweiteAbleitungGebrochenRational.getInhaltVector();
					t = new thread(&regulaFalsi_threadGebrochenRational, i, i - 0.1, std::ref(*zaehler), true);
				}
				threads.push_back(t);
				regulaFalsiLock.lock();
				threadReadyCounterRegulaFalsiZaehler++;
				regulaFalsiLock.unlock();
			}
		}

		yOld = y;
		rundenCounter++;
	}
	while (threadReadyCounterRegulaFalsiZaehler != 0) {
		Sleep(10);
	}

	RechnerLibrary rechnerLibrary;
	for (int i = 0; i < regulaFalsiNullstellenZaehlerGebrochenRational.size(); i++) {
		bool tolleranzErreicht = false;
		for (int j = 0; j < wendepunkte.size(); j++) {
			if (abs(wendepunkte[j].x - regulaFalsiNullstellenZaehlerGebrochenRational[i]->x)<tolleranz) {
				tolleranzErreicht = true;
			}
		}
		if (!tolleranzErreicht) {
			if (abs(rechnerLibrary.getPunkt((-regulaFalsiNullstellenZaehlerGebrochenRational[i]->x), &funktionAlsVectorZweiteAbleitungGebrochenRational)) < tolleranz) {
				if (abs(rechnerLibrary.getPunkt((-regulaFalsiNullstellenZaehlerGebrochenRational[i]->x), &funktionAlsVectorDritteAbleitungGebrochenRational)) > tolleranz) {
					wendepunkte.push_back(*regulaFalsiNullstellenZaehlerGebrochenRational[i]);
				}
			}
		}
	}

	for (int i = 0; i < wendepunkte.size(); i++) {
		wendepunkte[i].y = rechnerLibrary.getPunkt(-wendepunkte[i].x, &funktionAlsVektorSyntaxbaumGekürzt);
	}

	std::wstring concartWendepunkte;

	for (int i = 0; i < wendepunkte.size(); i++) {
		concartWendepunkte += std::wstring(L"w");
		concartWendepunkte += std::to_wstring(i + 1);
		concartWendepunkte += std::wstring(L"(");
		concartWendepunkte += std::to_wstring(-wendepunkte[i].x);
		concartWendepunkte += std::wstring(L" / ");
		concartWendepunkte += std::to_wstring(wendepunkte[i].y);
		concartWendepunkte += std::wstring(L") ");
		if (i != wendepunkte.size() - 1) {
			concartWendepunkte += std::wstring(L"; ");
		}
	}
	textWendepunktefeldAendern(concartWendepunkte.c_str());
}

void RechnerLibrary::linkerPart(int index, int vorzeichenLevel, int linkeGrenze, int rechteGrenze, FunktionSyntaxbaum *aktuellerKnoten, WCHAR* functionAlsStringLokal) {
	if (index == 0) {
		return;
	}
	bool gefunden = false;
	while (!gefunden) {
		for (int i = rechteGrenze - 1; i >= linkeGrenze; i--) {
			if (vorzeichenLevel == 0) {
				if (functionAlsStringLokal[i] == '+' || functionAlsStringLokal[i] == '-') {
					if (i == 0) {

					}
					else if (functionAlsStringLokal[i - 1] == '+' || functionAlsStringLokal[i - 1] == '-' || functionAlsStringLokal[i - 1] == '*' || functionAlsStringLokal[i - 1] == '/') {

					}
					else {
						gefunden = true;
						FunktionSyntaxbaum* neuerLinkerKnoten = new FunktionSyntaxbaum();
						neuerLinkerKnoten->setParent(aktuellerKnoten);
						neuerLinkerKnoten->setInhalt(functionAlsStringLokal[i]);
						neuerLinkerKnoten->setIndex(i);
						aktuellerKnoten->setLinkesChild(neuerLinkerKnoten);
						linkerPart(i, vorzeichenLevel, linkeGrenze, aktuellerKnoten->getIndex(), neuerLinkerKnoten, functionAlsStringLokal);
						rechterPart(i, vorzeichenLevel, aktuellerKnoten->getIndex() + 1, rechteGrenze, neuerLinkerKnoten, functionAlsStringLokal);
					}
				}
			}
			else if (vorzeichenLevel == 1) {
				if (functionAlsStringLokal[i] == '*' || functionAlsStringLokal[i] == '/') {
					gefunden = true;
					FunktionSyntaxbaum* neuerLinkerKnoten = new FunktionSyntaxbaum();
					neuerLinkerKnoten->setParent(aktuellerKnoten);
					neuerLinkerKnoten->setInhalt(functionAlsStringLokal[i]);
					neuerLinkerKnoten->setIndex(i);
					aktuellerKnoten->setLinkesChild(neuerLinkerKnoten);
					linkerPart(i, vorzeichenLevel, linkeGrenze, neuerLinkerKnoten->getIndex(), neuerLinkerKnoten, functionAlsStringLokal);
					rechterPart(i, vorzeichenLevel, neuerLinkerKnoten->getIndex() + 1, rechteGrenze, neuerLinkerKnoten, functionAlsStringLokal);
					i = 0;
				}
			}
			else if (vorzeichenLevel == 2) {
				if (functionAlsStringLokal[i] == '^') {
					gefunden = true;
					FunktionSyntaxbaum* neuerLinkerKnoten = new FunktionSyntaxbaum();
					neuerLinkerKnoten->setParent(aktuellerKnoten);
					neuerLinkerKnoten->setInhalt(functionAlsStringLokal[i]);
					neuerLinkerKnoten->setIndex(i);
					aktuellerKnoten->setLinkesChild(neuerLinkerKnoten);
					linkerPart(i, vorzeichenLevel, linkeGrenze, neuerLinkerKnoten->getIndex(), neuerLinkerKnoten, functionAlsStringLokal);
					rechterPart(i, vorzeichenLevel, neuerLinkerKnoten->getIndex() + 1, rechteGrenze, neuerLinkerKnoten, functionAlsStringLokal);
					i = 0;
				}
			}
			else if (vorzeichenLevel == 3) {
				if ((functionAlsStringLokal[i] >= '0' && functionAlsStringLokal[i] <= '9') || functionAlsStringLokal[i] == 'x' || functionAlsStringLokal[i] == 'X') {
					gefunden = true;
					FunktionSyntaxbaum* neuerLinkerKnoten = new FunktionSyntaxbaum();
					neuerLinkerKnoten->setParent(aktuellerKnoten);
					neuerLinkerKnoten->setInhalt(functionAlsStringLokal[i]);
					neuerLinkerKnoten->setIndex(i);
					aktuellerKnoten->setLinkesChild(neuerLinkerKnoten);
					linkerPart(i, vorzeichenLevel, linkeGrenze, neuerLinkerKnoten->getIndex(), neuerLinkerKnoten, functionAlsStringLokal);
					rechterPart(i, vorzeichenLevel, neuerLinkerKnoten->getIndex() + 1, rechteGrenze, neuerLinkerKnoten, functionAlsStringLokal);
					i = 0;
				}
			}
		}
		if (vorzeichenLevel > 3) {
			return;
		}
		vorzeichenLevel++;
	}
}

void RechnerLibrary::rechterPart(int index, int vorzeichenLevel, int linkeGrenze, int rechteGrenze, FunktionSyntaxbaum *aktuellerKnoten, WCHAR* functionAlsStringLokal) {
	if (index == sizeFunktionBuffer - 1) {
		return;
	}
	bool gefunden = false;
	while (!gefunden) {
		for (int i = linkeGrenze; i <= rechteGrenze; i++) {
			if (vorzeichenLevel == 0) {
				if (functionAlsStringLokal[i] == '+' || functionAlsStringLokal[i] == '-') {
					if (i == 0) {

					}
					else if (functionAlsStringLokal[i - 1] == '+' || functionAlsStringLokal[i - 1] == '-' || functionAlsStringLokal[i - 1] == '*' || functionAlsStringLokal[i - 1] == '/') {

					}
					else {
						gefunden = true;
						FunktionSyntaxbaum *neuerRechterKnoten = new FunktionSyntaxbaum();
						neuerRechterKnoten->setParent(aktuellerKnoten);
						neuerRechterKnoten->setInhalt(functionAlsStringLokal[i]);
						neuerRechterKnoten->setIndex(i);
						aktuellerKnoten->setRechtesChild(neuerRechterKnoten);
						linkerPart(i, vorzeichenLevel, linkeGrenze, neuerRechterKnoten->getIndex(), neuerRechterKnoten, functionAlsStringLokal);
						rechterPart(i, vorzeichenLevel, neuerRechterKnoten->getIndex() + 1, rechteGrenze, neuerRechterKnoten, functionAlsStringLokal);
					}
				}
			}
			else if (vorzeichenLevel == 1) {
				if (functionAlsStringLokal[i] == '^') {
					gefunden = true;
					FunktionSyntaxbaum* neuerRechterKnoten = new FunktionSyntaxbaum();
					neuerRechterKnoten->setParent(aktuellerKnoten);
					neuerRechterKnoten->setInhalt(functionAlsStringLokal[i]);
					neuerRechterKnoten->setIndex(i);
					aktuellerKnoten->setRechtesChild(neuerRechterKnoten);
					linkerPart(i, vorzeichenLevel, linkeGrenze, neuerRechterKnoten->getIndex(), neuerRechterKnoten, functionAlsStringLokal);
					rechterPart(i, vorzeichenLevel, neuerRechterKnoten->getIndex() + 1, rechteGrenze, neuerRechterKnoten, functionAlsStringLokal);
					i = sizeFunktionBuffer;
				}
			}
			else if (vorzeichenLevel == 2) {
				if (functionAlsStringLokal[i] == '*' || functionAlsStringLokal[i] == '/') {
					gefunden = true;
					FunktionSyntaxbaum* neuerRechterKnoten = new FunktionSyntaxbaum();
					neuerRechterKnoten->setParent(aktuellerKnoten);
					neuerRechterKnoten->setInhalt(functionAlsStringLokal[i]);
					neuerRechterKnoten->setIndex(i);
					aktuellerKnoten->setRechtesChild(neuerRechterKnoten);
					linkerPart(i, vorzeichenLevel, linkeGrenze, neuerRechterKnoten->getIndex(), neuerRechterKnoten, functionAlsStringLokal);
					rechterPart(i, vorzeichenLevel, neuerRechterKnoten->getIndex() + 1, rechteGrenze, neuerRechterKnoten, functionAlsStringLokal);
					i = sizeFunktionBuffer;
				}
			}
			else if (vorzeichenLevel == 3) {
				if ((functionAlsStringLokal[i] >= '0' && functionAlsStringLokal[i] <= '9') || functionAlsStringLokal[i] == 'x' || functionAlsStringLokal[i] == 'X') {
					gefunden = true;
					FunktionSyntaxbaum* neuerRechterKnoten = new FunktionSyntaxbaum();
					neuerRechterKnoten->setParent(aktuellerKnoten);
					neuerRechterKnoten->setInhalt(functionAlsStringLokal[i]);
					neuerRechterKnoten->setIndex(i);
					aktuellerKnoten->setRechtesChild(neuerRechterKnoten);
					linkerPart(i, vorzeichenLevel, linkeGrenze, neuerRechterKnoten->getIndex(), neuerRechterKnoten, functionAlsStringLokal);
					rechterPart(i, vorzeichenLevel, neuerRechterKnoten->getIndex() + 1, rechteGrenze, neuerRechterKnoten, functionAlsStringLokal);
					i = sizeFunktionBuffer;
				}
			}
		}
		if (vorzeichenLevel > 3) {
			return;
		}
		vorzeichenLevel++;
	}
}

void laengeUndKlammerFinden(int& rechteGrenze, bool& hatKlammern, WCHAR* functionAlsStringLokal, bool schonEntfernt) {
	for (int i = schonEntfernt == false ? 1 : 0; i < sizeFunktionBuffer; i++) {
		if (functionAlsStringLokal[i] == '(') {
			hatKlammern = true;
		}
		if (functionAlsStringLokal[i] == '\0') {
			rechteGrenze = i;
			break;
		}
	}
}

FunktionAlsVektorSyntaxbaum* RechnerLibrary::knotenAnhaengen(bool links, FunktionAlsVektorSyntaxbaum *aktuellerKnoten, WCHAR* functionAlsStringLokal,int i) {
	FunktionAlsVektorSyntaxbaum* neuerKnoten = new FunktionAlsVektorSyntaxbaum();
	neuerKnoten->setParent(aktuellerKnoten);
	neuerKnoten->setInhalt(functionAlsStringLokal[i]);
	neuerKnoten->setInhalt(functionAlsStringLokal);
	neuerKnoten->setIndex(i);
	if (links) {
		aktuellerKnoten->setLinkesChild(neuerKnoten);
		
	}
	else {
		aktuellerKnoten->setRechtesChild(neuerKnoten);
	}
	return neuerKnoten;
}

void RechnerLibrary::stringSplitten(int i, int rechteGrenze, int klammerLevel, WCHAR* functionAlsStringLokal, FunktionAlsVektorSyntaxbaum* neuerKnoten) {
	WCHAR* substringLinks = new WCHAR[i];
	WCHAR* substringRechts = new WCHAR[rechteGrenze - i];
	int g = 0;
	for (int j = 0; j < i; j++, g++) {
		substringLinks[g] = functionAlsStringLokal[j];
	}substringLinks[g] = 0; g = 0;
	int bis = klammerLevel == 0 ? rechteGrenze : rechteGrenze - 1;
	for (int j = i + 1; j < bis; j++, g++) {
		substringRechts[g] = functionAlsStringLokal[j];
	}substringRechts[g] = 0;
	substringLinks = entferneUnnoetigeKlammern(substringLinks);
	substringRechts = entferneUnnoetigeKlammern(substringRechts);
	linkerPartGebrochenRational(i, 0, neuerKnoten, substringLinks);
	rechterPartGebrochenRational(i, 0, neuerKnoten, substringRechts);
}

void RechnerLibrary::vectorAnhaengen(WCHAR* functionAlsStringLokal, FunktionAlsVektorSyntaxbaum* neuerKnoten) {
	FunktionSyntaxbaum neuerFunktionSyntaxbaum;
	parseFuntionBuffer(neuerFunktionSyntaxbaum, functionAlsStringLokal);
	vector<double> vectorKnoten;
	ausSyntaxbaumVektorErstellen(&neuerFunktionSyntaxbaum, vectorKnoten);
	neuerKnoten->setInhalt(vectorKnoten);
}

void RechnerLibrary::linkerPartGebrochenRational(int index, int vorzeichenLevel, FunktionAlsVektorSyntaxbaum *aktuellerKnoten, WCHAR* functionAlsStringLokal) {
	if (index == 0) {
		return;
	}
	int rechteGrenze = 0;

	bool hatKlammern = false;
	laengeUndKlammerFinden(rechteGrenze, hatKlammern, functionAlsStringLokal, true);
	
	int klammerZaehlerAuf = 0;
	int klammerZaehlerZu = 0;
	int klammerLevel = 0;
	bool gefunden = false;
	while (!gefunden) {
		int klammerZaehlerAuf = 0;
		int klammerZaehlerZu = 0;
		int klammerLevel = 0;
		for (int i = 0; i < rechteGrenze; i++) {
			if (functionAlsStringLokal[i] == '(') {
				klammerZaehlerAuf++;
				klammerLevel++;
			}
			if (functionAlsStringLokal[i] == ')') {
				klammerZaehlerZu++;
				klammerLevel--;
			}
			if (((klammerZaehlerAuf == 1 && klammerZaehlerZu == 0) && klammerLevel == 1 && functionAlsStringLokal[0] == '(') || ((klammerZaehlerAuf == 0 && klammerZaehlerZu == 0) && klammerLevel == 0)) {
				if (vorzeichenLevel == 0) {
					if (functionAlsStringLokal[i] == '+' || functionAlsStringLokal[i] == '-') {
						if (i == 0) {

						}
						else if (functionAlsStringLokal[i - 1] == '+' || functionAlsStringLokal[i - 1] == '-' || functionAlsStringLokal[i - 1] == '*' || functionAlsStringLokal[i - 1] == '/') {

						}
						else {
							gefunden = true;
							FunktionAlsVektorSyntaxbaum* neuerLinkerKnoten = knotenAnhaengen(true, aktuellerKnoten, functionAlsStringLokal, i);
							if (hatKlammern) {
								stringSplitten(i, rechteGrenze, klammerLevel, functionAlsStringLokal, neuerLinkerKnoten);
							}
							else {
								vectorAnhaengen(functionAlsStringLokal, neuerLinkerKnoten);
							}
							return;
						}
					}
				}
				else if (vorzeichenLevel == 1) {
					if (functionAlsStringLokal[i] == '*' || functionAlsStringLokal[i] == '/') {
						gefunden = true;
						FunktionAlsVektorSyntaxbaum* neuerLinkerKnoten = knotenAnhaengen(true, aktuellerKnoten, functionAlsStringLokal, i);
						if (hatKlammern) {
							stringSplitten(i, rechteGrenze, klammerLevel, functionAlsStringLokal, neuerLinkerKnoten);
						}
						else {
							vectorAnhaengen(functionAlsStringLokal, neuerLinkerKnoten);
						}
						return;
					}
				}
				else if (vorzeichenLevel == 2) {
					if (functionAlsStringLokal[i] == '^') {
						gefunden = true;
						FunktionAlsVektorSyntaxbaum* neuerLinkerKnoten = knotenAnhaengen(true, aktuellerKnoten, functionAlsStringLokal, i);
						if (hatKlammern) {
							stringSplitten(i, rechteGrenze, klammerLevel, functionAlsStringLokal, neuerLinkerKnoten);
						}
						else {
							vectorAnhaengen(functionAlsStringLokal, neuerLinkerKnoten);
						}
						return;
					}
				}
				else if (vorzeichenLevel == 3) {
					if ((functionAlsStringLokal[i] >= '0' && functionAlsStringLokal[i] <= '9') || functionAlsStringLokal[i] == 'x' || functionAlsStringLokal[i] == 'X') {
						gefunden = true;
						FunktionAlsVektorSyntaxbaum* neuerLinkerKnoten = knotenAnhaengen(true, aktuellerKnoten, functionAlsStringLokal, i);
						vectorAnhaengen(functionAlsStringLokal, neuerLinkerKnoten);
						return;
					}
				}
			}
		}
		if (vorzeichenLevel > 3) {
			return;
		}
		vorzeichenLevel++;
	}
}

void RechnerLibrary::rechterPartGebrochenRational(int index, int vorzeichenLevel, FunktionAlsVektorSyntaxbaum *aktuellerKnoten, WCHAR* functionAlsStringLokal) {
	if (index == 0) {
		return;
	}
	bool gefunden = false;
	int klammerZaehlerAuf = 0;
	int klammerZaehlerZu = 0;
	int klammerLevel = 0;
	int rechteGrenze = 0;
	bool hatKlammern = false;
	laengeUndKlammerFinden(rechteGrenze, hatKlammern, functionAlsStringLokal, true);
	
	while (!gefunden) {
		int klammerZaehlerAuf = 0;
		int klammerZaehlerZu = 0;
		int klammerLevel = 0;
		for (int i = 0; i < rechteGrenze; i++) {
			if (functionAlsStringLokal[i] == '(') {
				klammerZaehlerAuf++;
				klammerLevel++;
			}
			if (functionAlsStringLokal[i] == ')') {
				klammerZaehlerZu++;
				klammerLevel--;
			}
			if (((klammerZaehlerAuf == 1 && klammerZaehlerZu == 0) && klammerLevel == 1 && functionAlsStringLokal[0] == '(') || ((klammerZaehlerAuf == 0 && klammerZaehlerZu == 0) && klammerLevel == 0)) {
				if (vorzeichenLevel == 0) {
					if (functionAlsStringLokal[i] == '+' || functionAlsStringLokal[i] == '-') {
						if (i == 0) {

						}
						else if (functionAlsStringLokal[i - 1] == '+' || functionAlsStringLokal[i - 1] == '-' || functionAlsStringLokal[i - 1] == '*' || functionAlsStringLokal[i - 1] == '/') {

						}
						else {
							gefunden = true;
							FunktionAlsVektorSyntaxbaum* neuerRechterKnoten = knotenAnhaengen(false, aktuellerKnoten, functionAlsStringLokal, i);
							if (hatKlammern) {
								stringSplitten(i, rechteGrenze, klammerLevel, functionAlsStringLokal, neuerRechterKnoten);
							}
							else {
								vectorAnhaengen(functionAlsStringLokal, neuerRechterKnoten);
							}
							return;
						}
					}
				}
				else if (vorzeichenLevel == 1) {
					if (functionAlsStringLokal[i] == '*' || functionAlsStringLokal[i] == '/') {
						gefunden = true;
						FunktionAlsVektorSyntaxbaum* neuerRechterKnoten = knotenAnhaengen(false, aktuellerKnoten, functionAlsStringLokal, i);
						if (hatKlammern) {
							stringSplitten(i, rechteGrenze, klammerLevel, functionAlsStringLokal, neuerRechterKnoten);
						}
						else {
							vectorAnhaengen(functionAlsStringLokal, neuerRechterKnoten);
						}
						return;
					}
				}
				else if (vorzeichenLevel == 2) {
					if (functionAlsStringLokal[i] == '^') {
						gefunden = true;
						FunktionAlsVektorSyntaxbaum* neuerRechterKnoten = knotenAnhaengen(false, aktuellerKnoten, functionAlsStringLokal, i);
						if (hatKlammern) {
							stringSplitten(i, rechteGrenze, klammerLevel, functionAlsStringLokal, neuerRechterKnoten);
						}
						else {
							vectorAnhaengen(functionAlsStringLokal, neuerRechterKnoten);
						}
						return;
					}
				}
				else if (vorzeichenLevel == 3) {
					if ((functionAlsStringLokal[i] >= '0' && functionAlsStringLokal[i] <= '9') || functionAlsStringLokal[i] == 'x' || functionAlsStringLokal[i] == 'X') {
						gefunden = true;
						FunktionAlsVektorSyntaxbaum* neuerRechterKnoten = knotenAnhaengen(false, aktuellerKnoten, functionAlsStringLokal, i);
						vectorAnhaengen(functionAlsStringLokal, neuerRechterKnoten);
						return;
					}
				}
			}
		}
		if (vorzeichenLevel > 3) {
			return;
		}
		vorzeichenLevel++;
	}
}

void RechnerLibrary::parseFuntionBuffer(FunktionSyntaxbaum& neuerFunktionSyntaxbaum,WCHAR* functionAlsString) {
	neuerFunktionSyntaxbaum = FunktionSyntaxbaum();
	neuerFunktionSyntaxbaum.setLinkesChild(nullptr);
	neuerFunktionSyntaxbaum.setRechtesChild(nullptr);
	bool gefunden = false;
	int vorzeichenLevel = 0;
	int rechteGrenze = 0;
	bool hatKlammern = false;

	for (int i = 0; i < sizeFunktionBuffer; i++) {
		if (functionAlsString[i] == '(') {
			hatKlammern = true;
		}
		if (functionAlsString[i] == '\0') {
			rechteGrenze = i - 1;
			break;
		}
	}
	while (!gefunden) {
		if (rechteGrenze == 0) {
			neuerFunktionSyntaxbaum.setRoot(&neuerFunktionSyntaxbaum);
			neuerFunktionSyntaxbaum.setInhalt(functionAlsString[0]);
			neuerFunktionSyntaxbaum.setLinkesChild(nullptr);
			neuerFunktionSyntaxbaum.setRechtesChild(nullptr);
			neuerFunktionSyntaxbaum.setIndex(0);
			
		}
		for (int i = 0; i < rechteGrenze; i++) { // ^ , vorzeichen, * /, + -
			if (vorzeichenLevel == 0) {
				if (functionAlsString[i] == '+' || functionAlsString[i] == '-') {
					if (i == 0) {

					}
					else if (functionAlsString[i - 1] == '+' || functionAlsString[i - 1] == '-' || functionAlsString[i - 1] == '*' || functionAlsString[i - 1] == '/') {

					}
					else {
						gefunden = true;
						neuerFunktionSyntaxbaum.setRoot(&neuerFunktionSyntaxbaum);
						neuerFunktionSyntaxbaum.setInhalt(functionAlsString[i]);
						neuerFunktionSyntaxbaum.setIndex(i);
						linkerPart(i, 0, 0, i, &neuerFunktionSyntaxbaum, functionAlsString);
						rechterPart(i, 0, i + 1, rechteGrenze, &neuerFunktionSyntaxbaum, functionAlsString);
						i = rechteGrenze;

					}
				}
			}
			else if (vorzeichenLevel == 1) {
				if (functionAlsString[i] == '*' || functionAlsString[i] == '/') {
					gefunden = true;
					neuerFunktionSyntaxbaum.setRoot(&neuerFunktionSyntaxbaum);
					neuerFunktionSyntaxbaum.setInhalt(functionAlsString[i]);
					neuerFunktionSyntaxbaum.setIndex(i);
					linkerPart(i, 1, 0, i, &neuerFunktionSyntaxbaum, functionAlsString);
					rechterPart(i, 1, i + 1, rechteGrenze, &neuerFunktionSyntaxbaum, functionAlsString);
					i = rechteGrenze;
				}
			}
			else if (vorzeichenLevel == 2) {
				if (functionAlsString[i] == '^') {
					gefunden = true;
					neuerFunktionSyntaxbaum.setRoot(&neuerFunktionSyntaxbaum);
					neuerFunktionSyntaxbaum.setInhalt(functionAlsString[i]);
					neuerFunktionSyntaxbaum.setIndex(i);
					linkerPart(i, 2, 0, i, &neuerFunktionSyntaxbaum, functionAlsString);
					rechterPart(i, 2, i + 1, rechteGrenze, &neuerFunktionSyntaxbaum, functionAlsString);
					i = rechteGrenze;
				}
			}
			else if (vorzeichenLevel == 3) {
				if ((functionAlsString[i] >= '0' && functionAlsString[i] <= '9') || functionAlsString[i] == 'x' || functionAlsString[i] == 'X') {
					gefunden = true;
					neuerFunktionSyntaxbaum.setRoot(&neuerFunktionSyntaxbaum);
					neuerFunktionSyntaxbaum.setInhalt(functionAlsString[i]);
					neuerFunktionSyntaxbaum.setIndex(i);
					linkerPart(i, 2, 0, i, &neuerFunktionSyntaxbaum, functionAlsString);
					rechterPart(i, 2, i + 1, rechteGrenze, &neuerFunktionSyntaxbaum, functionAlsString);
					i = rechteGrenze;
				}
			}
		}
		if (vorzeichenLevel > 3) {
			return;
		}
		vorzeichenLevel++;
	}
}

void RechnerLibrary::splitFuntionBufferGebrochenRational(FunktionAlsVektorSyntaxbaum& neuerFunktionSyntaxbaumGebrochenRational, WCHAR* functionAlsString) {
	neuerFunktionSyntaxbaumGebrochenRational = FunktionAlsVektorSyntaxbaum();
	neuerFunktionSyntaxbaumGebrochenRational.setLinkesChild(nullptr);
	neuerFunktionSyntaxbaumGebrochenRational.setRechtesChild(nullptr);
	bool gefunden = false;
	int vorzeichenLevel = 0;
	int rechteGrenze = 0;
	int klammerZaehlerAuf = 0;
	int klammerZaehlerZu = 0;
	int klammerLevel = 0;
	bool hatKlammern = false;
	laengeUndKlammerFinden(rechteGrenze, hatKlammern, functionAlsString, false);

	while (!gefunden) {
		if (rechteGrenze == 0) {
			neuerFunktionSyntaxbaumGebrochenRational.setRoot(&neuerFunktionSyntaxbaumGebrochenRational);
			neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString[0]);
			neuerFunktionSyntaxbaumGebrochenRational.setLinkesChild(nullptr);
			neuerFunktionSyntaxbaumGebrochenRational.setRechtesChild(nullptr);
			neuerFunktionSyntaxbaumGebrochenRational.setUnaeresChild(nullptr);
		}
		for (int i = 0; i < rechteGrenze; i++) { // ^ , vorzeichen, * /, + -
			if (functionAlsString[i] == '(') {
				klammerZaehlerAuf++;
				klammerLevel++;
			}
			if (functionAlsString[i] == ')') {
				klammerZaehlerZu++;
				klammerLevel--;
			}
			if ((klammerZaehlerAuf == klammerZaehlerZu) && klammerLevel == 0) {
				if (vorzeichenLevel == 0) {
					if (functionAlsString[i] == '+' || functionAlsString[i] == '-') {
						if (i == 0) {

						}
						else if (functionAlsString[i - 1] == '+' || functionAlsString[i - 1] == '-' || functionAlsString[i - 1] == '*' || functionAlsString[i - 1] == '/') {

						}
						else {
							gefunden = true;
							neuerFunktionSyntaxbaumGebrochenRational.setRoot(&neuerFunktionSyntaxbaumGebrochenRational);
							neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString[i]);
							neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString);
							if (hatKlammern) {
								WCHAR* substringLinks = new WCHAR[i];
								WCHAR* substringRechts = new WCHAR[rechteGrenze - i];
								int g = 0;
								for (int j = klammerLevel == 0 ? 0 : 1; j < i; j++, g++) {
									substringLinks[g] = functionAlsString[j];
								}substringLinks[g] = 0; g = 0;
								int bis = klammerLevel == 0 ? rechteGrenze : rechteGrenze - 1;
								for (int j = i + 1; j < bis; j++, g++) {
									substringRechts[g] = functionAlsString[j];
								}substringRechts[g] = 0;
								linkerPartGebrochenRational(i, 0, &neuerFunktionSyntaxbaumGebrochenRational, substringLinks);
								rechterPartGebrochenRational(i, 0, &neuerFunktionSyntaxbaumGebrochenRational, substringRechts);
							}
							else {
								FunktionSyntaxbaum neuerFunktionSyntaxbaum;
								parseFuntionBuffer(neuerFunktionSyntaxbaum, functionAlsString);
								vector<double> vectorKnoten;
								ausSyntaxbaumVektorErstellen(&neuerFunktionSyntaxbaum, vectorKnoten);
								neuerFunktionSyntaxbaumGebrochenRational.setInhalt(vectorKnoten);
							}
							i = rechteGrenze;

						}
					}
				}
				else if (vorzeichenLevel == 1) {
					if (functionAlsString[i] == '*' || functionAlsString[i] == '/') {
						gefunden = true;
						neuerFunktionSyntaxbaumGebrochenRational.setRoot(&neuerFunktionSyntaxbaumGebrochenRational);
						neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString[i]);
						neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString);
						if (hatKlammern) {
						
							WCHAR* substringLinks = new WCHAR[i];
							WCHAR* substringRechts = new WCHAR[rechteGrenze - i];
							int g = 0;
							for (int j = functionAlsString[0] == '(' ? 1 : 0; j < (functionAlsString[i-1] == ')' ? i - 1 : i); j++, g++) {
								substringLinks[g] = functionAlsString[j];
							}substringLinks[g] = 0; g = 0;
							int bis = functionAlsString[rechteGrenze-1] == ')' ? rechteGrenze - 1 : rechteGrenze;
							for (int j = functionAlsString[i + 1] == '(' ? i + 2 : i + 1; j < bis; j++, g++) {
								substringRechts[g] = functionAlsString[j];
							}substringRechts[g] = 0;
							linkerPartGebrochenRational(i, 0, &neuerFunktionSyntaxbaumGebrochenRational, substringLinks); //wenn 2 klammern gefunden, vorzeichenlevel wieder auf 0 für in der klammer
							rechterPartGebrochenRational(i, 0, &neuerFunktionSyntaxbaumGebrochenRational, substringRechts);
						}
						else {
							FunktionSyntaxbaum neuerFunktionSyntaxbaum;
							parseFuntionBuffer(neuerFunktionSyntaxbaum, functionAlsString);
							vector<double> vectorKnoten;
							ausSyntaxbaumVektorErstellen(&neuerFunktionSyntaxbaum, vectorKnoten);
							neuerFunktionSyntaxbaumGebrochenRational.setInhalt(vectorKnoten);
						}
						i = rechteGrenze;
					}
				}
				else if (vorzeichenLevel == 2) {
					if (functionAlsString[i] == '^') {
						gefunden = true;
						neuerFunktionSyntaxbaumGebrochenRational.setRoot(&neuerFunktionSyntaxbaumGebrochenRational);
						neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString[i]);
						neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString);
						if (hatKlammern) {
							WCHAR* substringLinks = new WCHAR[i];
							WCHAR* substringRechts = new WCHAR[rechteGrenze - i];
							int g = 0;
							for (int j = klammerLevel == 0 ? 0 : 1; j < i; j++, g++) {
								substringLinks[g] = functionAlsString[j];
							}substringLinks[g] = 0; g = 0;
							int bis = klammerLevel == 0 ? rechteGrenze : rechteGrenze - 1;
							for (int j = i + 1; j < bis; j++, g++) {
								substringRechts[g] = functionAlsString[j];
							}substringRechts[g] = 0;
							linkerPartGebrochenRational(i, 0, &neuerFunktionSyntaxbaumGebrochenRational, substringLinks);
							rechterPartGebrochenRational(i, 0, &neuerFunktionSyntaxbaumGebrochenRational, substringRechts);
						}else {
							FunktionSyntaxbaum neuerFunktionSyntaxbaum;
							parseFuntionBuffer(neuerFunktionSyntaxbaum, functionAlsString);
							vector<double> vectorKnoten;
							ausSyntaxbaumVektorErstellen(&neuerFunktionSyntaxbaum, vectorKnoten);
							neuerFunktionSyntaxbaumGebrochenRational.setInhalt(vectorKnoten);
						}
						i = rechteGrenze;
					}
				}
				else if (vorzeichenLevel == 3) {
					if ((functionAlsString[i] >= '0' && functionAlsString[i] <= '9') || functionAlsString[i] == 'x' || functionAlsString[i] == 'X') {
						gefunden = true;
						neuerFunktionSyntaxbaumGebrochenRational.setRoot(&neuerFunktionSyntaxbaumGebrochenRational);
						neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString[i]);
						neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString);
						FunktionSyntaxbaum neuerFunktionSyntaxbaum;
						parseFuntionBuffer(neuerFunktionSyntaxbaum, functionAlsString);
						vector<double> vectorKnoten;
						ausSyntaxbaumVektorErstellen(&neuerFunktionSyntaxbaum, vectorKnoten);
						neuerFunktionSyntaxbaumGebrochenRational.setInhalt(vectorKnoten);
						/*WCHAR* substringLinks = new WCHAR[i];
						WCHAR* substringRechts = new WCHAR[rechteGrenze - i];
						int g = 0;
						for (int j = klammerLevel == 0 ? 0 : 1; j < i; j++, g++) {
							substringLinks[g] = functionAlsString[j];
						}substringLinks[g] = 0; g = 0;
						int bis = klammerLevel == 0 ? rechteGrenze : rechteGrenze - 1;
						for (int j = i + 1; j < bis; j++, g++) {
							substringRechts[g] = functionAlsString[j];
						}substringRechts[g] = 0;
						linkerPartGebrochenRational(i, 0, &neuerFunktionSyntaxbaumGebrochenRational, substringLinks);
						rechterPartGebrochenRational(i, 0, &neuerFunktionSyntaxbaumGebrochenRational, substringRechts);
						i = rechteGrenze;*/
					}
				}
				if (vorzeichenLevel > 3) {
					return;
				}
			}
		}
		vorzeichenLevel++;
	}
}

WCHAR* RechnerLibrary::funktionVectorToString(vector<double> funktion, bool mitMalZeichenZwischenXundZahl) {
	wstring beschriftung;
	WCHAR* beschriftungWCHAR = new WCHAR[2048];
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
				if (mitMalZeichenZwischenXundZahl) {
					beschriftung += '*';
				}	
			}
			else {
				for (int j = 0; j < indexPeriod; j++) {
					if (i == 0) {
						beschriftung += tmpBeschriftung[j];
					}
					else {
						if (tmpBeschriftung[j] != '1') {
							beschriftung += tmpBeschriftung[j];
							if (j == indexPeriod - 1) {
								if (mitMalZeichenZwischenXundZahl) {
									beschriftung += '*';
								}
							}						
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
	int k = 0;
	while (beschriftung[k] != 0) {
		beschriftungWCHAR[k] = beschriftung[k];
		k++;
	}
	beschriftungWCHAR[k] = 0;
	return beschriftungWCHAR;
}

WCHAR* RechnerLibrary::entferneUnnoetigeKlammern(WCHAR* functionAlsStringLokal) {
	int rechteGrenze = 0;
	bool hatKlammern = true;


	int klammerZaehlerAuf = 0;
	int klammerZaehlerZu = 0;
	int klammerLevel = 0;
	int indexErsteKlammerAuf = -1;
	int indexErsteKlammerZu = -1;
	
	laengeUndKlammerFinden(rechteGrenze, hatKlammern, functionAlsStringLokal, false);

	hatKlammern = false;
	for (int i = 0; i < rechteGrenze; i++) {
		if (functionAlsStringLokal[i] == '(') {
			hatKlammern = true;
		}
	}
	WCHAR* functionAlsStringOhneKlammern=nullptr;
	bool hatteUnnoetigeKlammern = false;
	if (rechteGrenze > 2) {
		functionAlsStringOhneKlammern = new WCHAR[rechteGrenze - 2];
		
		if (hatKlammern) {
			for (int i = 0; i < rechteGrenze; i++) {
				if (functionAlsStringLokal[i] == '(') {
					if (indexErsteKlammerAuf == -1) { // (x^3+2)/(x^2+4) passt
						indexErsteKlammerAuf = i;	// ((x^3+2)+4*x^3) 
					}								// (x^3+2+(4*x^3))
					klammerZaehlerAuf++;			// (x^3+2+4*x^2) passt
					klammerLevel++;					// x^3*(2*x^3)+4
				}									// x^3*((x^3+2)+4*x^3) 
				if (functionAlsStringLokal[i] == ')') {	// ((x^3+2)+4*x^3)*3*x
					if (indexErsteKlammerZu == -1) {
						indexErsteKlammerZu = i;
					}
					klammerZaehlerZu++;
					klammerLevel--;
				}
				/*if (klammerZaehlerAuf == klammerZaehlerZu && klammerLevel == 0 && klammerZaehlerAuf == 1 && klammerZaehlerZu == 1) {
					int g = 0;
					for (int j = indexErsteKlammerAuf+1; j < indexErsteKlammerZu; j++) {
						functionAlsStringOhneKlammern[g++] = functionAlsStringLokal[j];
					}
					functionAlsStringOhneKlammern[g++] = 0;
					break;
				}*/
				if (klammerZaehlerAuf == klammerZaehlerZu && klammerLevel == 0 && indexErsteKlammerAuf == 0 && indexErsteKlammerZu == rechteGrenze - 1) {
					int g = 0;
					for (int j = indexErsteKlammerAuf + 1; j < indexErsteKlammerZu; j++) {
						functionAlsStringOhneKlammern[g++] = functionAlsStringLokal[j];
					}
					functionAlsStringOhneKlammern[g++] = 0;
					hatteUnnoetigeKlammern = true;
				}

			}
		}
		else {
			return functionAlsStringLokal;
		}
	}
	/*for (int i = 0; i < rechteGrenze; i++) { 
		if (functionAlsString[i] == '(') {
			if (indexErsteKlammerAuf - 1) { // (x^3+2)/(x^2+4) passt
				indexErsteKlammerAuf = i;	// ((x^3+2)+4*x^3) 
			}								// (x^3+2+(4*x^3))
			klammerZaehlerAuf++;			// (x^3+2+4*x^2) passt
			klammerLevel++;					// x^3*(2*x^3)+4
		}									// x^3*((x^3+2)+4*x^3) 
		if (functionAlsString[i] == ')') {	// ((x^3+2)+4*x^3)*3*x
			if (indexErsteKlammerZu - 1) {
				indexErsteKlammerZu = i;
			}
			klammerZaehlerZu++;
			klammerLevel--;
		}
		if (klammerZaehlerAuf == klammerZaehlerZu && klammerLevel == 0 && klammerZaehlerAuf == 1 && klammerZaehlerZu == 1) {
			int g = 0;
			for (int j = indexErsteKlammerAuf; j < indexErsteKlammerZu; j++) {
				functionAlsStringOhneKlammern[g++] = functionAlsStringLokal[j];
			}
		}
	}*/
	if (hatteUnnoetigeKlammern) {
		return functionAlsStringOhneKlammern;
	} else {
		return functionAlsStringLokal;
	}
}

void RechnerLibrary::parseFuntionBufferGebrochenRational(FunktionSyntaxbaum& neuerFunktionSyntaxbaumGebrochenRational,WCHAR* functionAlsString) {
	neuerFunktionSyntaxbaumGebrochenRational = FunktionSyntaxbaum();
	neuerFunktionSyntaxbaumGebrochenRational.setLinkesChild(nullptr);
	neuerFunktionSyntaxbaumGebrochenRational.setRechtesChild(nullptr);

	//Todo entferne unnötige Klammern
	bool gefunden = false;
	int vorzeichenLevel = 0;
	int rechteGrenze = 0;
	for (int i = 0; i < sizeFunktionBuffer; i++) {
		if (functionAlsString[i] == '\0') {
			rechteGrenze = i - 1;
			break;
		}
	}
	while (!gefunden) {
		if (rechteGrenze == 0) {
			neuerFunktionSyntaxbaumGebrochenRational.setRoot(&neuerFunktionSyntaxbaumGebrochenRational);
			neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString[0]);
			neuerFunktionSyntaxbaumGebrochenRational.setLinkesChild(nullptr);
			neuerFunktionSyntaxbaumGebrochenRational.setRechtesChild(nullptr);
			neuerFunktionSyntaxbaumGebrochenRational.setIndex(0);
			
		}
		for (int i = 0; i < rechteGrenze; i++) { // ^ , vorzeichen, * /, + -
			if (vorzeichenLevel == 0) {
				if (functionAlsString[i] == '+' || functionAlsString[i] == '-') {
					if (i == 0) {

					}
					else if (functionAlsString[i - 1] == '+' || functionAlsString[i - 1] == '-' || functionAlsString[i - 1] == '*' || functionAlsString[i - 1] == '/') {

					}
					else {
						gefunden = true;
						neuerFunktionSyntaxbaumGebrochenRational.setRoot(&neuerFunktionSyntaxbaumGebrochenRational);
						neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString[i]);
						neuerFunktionSyntaxbaumGebrochenRational.setIndex(i);
						linkerPart(i, 0, 0, i, &neuerFunktionSyntaxbaumGebrochenRational, functionAlsString);
						rechterPart(i, 0, i + 1, rechteGrenze, &neuerFunktionSyntaxbaumGebrochenRational, functionAlsString);
						i = rechteGrenze;

					}
				}
			}
			else if (vorzeichenLevel == 1) {
				if (functionAlsString[i] == '*' || functionAlsString[i] == '/') {
					gefunden = true;
					neuerFunktionSyntaxbaumGebrochenRational.setRoot(&neuerFunktionSyntaxbaumGebrochenRational);
					neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString[i]);
					neuerFunktionSyntaxbaumGebrochenRational.setIndex(i);
					linkerPart(i, 1, 0, i, &neuerFunktionSyntaxbaumGebrochenRational, functionAlsString);
					rechterPart(i, 1, i + 1, rechteGrenze, &neuerFunktionSyntaxbaumGebrochenRational, functionAlsString);
					i = rechteGrenze;
				}
			}
			else if (vorzeichenLevel == 2) {
				if (functionAlsString[i] == '^') {
					gefunden = true;
					neuerFunktionSyntaxbaumGebrochenRational.setRoot(&neuerFunktionSyntaxbaumGebrochenRational);
					neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString[i]);
					neuerFunktionSyntaxbaumGebrochenRational.setIndex(i);
					linkerPart(i, 2, 0, i, &neuerFunktionSyntaxbaumGebrochenRational, functionAlsString);
					rechterPart(i, 2, i + 1, rechteGrenze, &neuerFunktionSyntaxbaumGebrochenRational, functionAlsString);
					i = rechteGrenze;
				}
			}
			else if (vorzeichenLevel == 3) {
				if ((functionAlsString[i] >= '0' && functionAlsString[i] <= '9') || functionAlsString[i] == 'x' || functionAlsString[i] == 'X') {
					gefunden = true;
					neuerFunktionSyntaxbaumGebrochenRational.setRoot(&neuerFunktionSyntaxbaumGebrochenRational);
					neuerFunktionSyntaxbaumGebrochenRational.setInhalt(functionAlsString[i]);
					neuerFunktionSyntaxbaumGebrochenRational.setIndex(i);
					linkerPart(i, 2, 0, i, &neuerFunktionSyntaxbaumGebrochenRational, functionAlsString);
					rechterPart(i, 2, i + 1, rechteGrenze, &neuerFunktionSyntaxbaumGebrochenRational, functionAlsString);
					i = rechteGrenze;
				}
			}
		}
		if (vorzeichenLevel > 3) {
			return;
		}
		vorzeichenLevel++;
	}
}

void RechnerLibrary::kuerzeSyntaxbaumGebrochenRational(FunktionAlsVektorSyntaxbaum* aktuellerKnoten) {
	if (aktuellerKnoten->getLinkesChild() == nullptr && aktuellerKnoten->getRechtesChild() == nullptr) {
		return;
	}
	if (aktuellerKnoten->getLinkesChild() != nullptr) {
		if (aktuellerKnoten->getLinkesChild()->getLinkesChild() != nullptr) {
			kuerzeSyntaxbaumGebrochenRational(aktuellerKnoten->getLinkesChild());
		}
	}
	if (aktuellerKnoten->getRechtesChild() != nullptr) {
		if (aktuellerKnoten->getRechtesChild()->getRechtesChild() != nullptr) {
			kuerzeSyntaxbaumGebrochenRational(aktuellerKnoten->getRechtesChild());
		}
	}

	if (aktuellerKnoten->getInhaltTChar() == '^') {

	}


	if (aktuellerKnoten->getInhaltTChar() == '*') {
		if (aktuellerKnoten->getRechtesChild() != nullptr&&aktuellerKnoten->getLinkesChild() != nullptr) {
			if ((aktuellerKnoten->getRechtesChild()->getRechtesChild() == nullptr && aktuellerKnoten->getRechtesChild()->getLinkesChild() == nullptr) && (aktuellerKnoten->getLinkesChild()->getLinkesChild() == nullptr && aktuellerKnoten->getLinkesChild()->getRechtesChild() == nullptr)) {
				vector<double> rechtesPolynom = aktuellerKnoten->getRechtesChild()->getInhaltVector();
				vector<double> linkesPolynom = aktuellerKnoten->getLinkesChild()->getInhaltVector();
				aktuellerKnoten->setInhalt(polynomMultiplikation(linkesPolynom, rechtesPolynom));
				aktuellerKnoten->setInhalt(funktionVectorToString(aktuellerKnoten->getInhaltVector(),false));
				aktuellerKnoten->setRechtesChild(nullptr);
				aktuellerKnoten->setLinkesChild(nullptr);
			}
		}
	}

	if (aktuellerKnoten->getInhaltTChar() == '+') {
		if (aktuellerKnoten->getRechtesChild() != nullptr&&aktuellerKnoten->getLinkesChild() != nullptr) {
			if ((aktuellerKnoten->getRechtesChild()->getRechtesChild() == nullptr && aktuellerKnoten->getRechtesChild()->getLinkesChild() == nullptr)&& (aktuellerKnoten->getLinkesChild()->getLinkesChild() == nullptr && aktuellerKnoten->getLinkesChild()->getRechtesChild() == nullptr)) {
				vector<double> rechtesPolynom = aktuellerKnoten->getRechtesChild()->getInhaltVector();
				vector<double> linkesPolynom = aktuellerKnoten->getLinkesChild()->getInhaltVector();
				aktuellerKnoten->setInhalt(polynomAddition(linkesPolynom, rechtesPolynom));
				aktuellerKnoten->setInhalt(funktionVectorToString(aktuellerKnoten->getInhaltVector(), false));
				aktuellerKnoten->setRechtesChild(nullptr);
				aktuellerKnoten->setLinkesChild(nullptr);
			}
		}
	}

	if (aktuellerKnoten->getInhaltTChar() == '-') {
		if (aktuellerKnoten->getRechtesChild() != nullptr&&aktuellerKnoten->getLinkesChild() != nullptr) {
			if ((aktuellerKnoten->getRechtesChild()->getRechtesChild() == nullptr && aktuellerKnoten->getRechtesChild()->getLinkesChild() == nullptr) && (aktuellerKnoten->getLinkesChild()->getLinkesChild() == nullptr && aktuellerKnoten->getLinkesChild()->getRechtesChild() == nullptr)) {
				vector<double> rechtesPolynom = aktuellerKnoten->getRechtesChild()->getInhaltVector();
				vector<double> linkesPolynom = aktuellerKnoten->getLinkesChild()->getInhaltVector();
				aktuellerKnoten->setInhalt(polynomSubtraktion(linkesPolynom, rechtesPolynom));
				aktuellerKnoten->setInhalt(funktionVectorToString(aktuellerKnoten->getInhaltVector(), false));
				aktuellerKnoten->setRechtesChild(nullptr);
				aktuellerKnoten->setLinkesChild(nullptr);
			}
		}
	}

	if (aktuellerKnoten->getInhaltTChar() == '/') {
		aktuellerKnoten->setInhalt(stringsVerbinden(aktuellerKnoten->getLinkesChild()->getInhaltString(), aktuellerKnoten->getRechtesChild()->getInhaltString(), '/'));
	}
}

void RechnerLibrary::resetVals() {
	nullstellen.clear();
	extremstellen.clear();
	wendepunkte.clear();
}

void RechnerLibrary::setSyntaxbaum(FunktionSyntaxbaum& neuerFunktionSyntaxbaum) {
	funktionSyntaxbaum = neuerFunktionSyntaxbaum;
}

void RechnerLibrary::setSyntaxbaum(FunktionAlsVektorSyntaxbaum& neuerFunktionSyntaxbaum) {
	funktionAlsVektorSyntaxbaum = neuerFunktionSyntaxbaum;
}

FunktionAlsVektorSyntaxbaum& RechnerLibrary::getSyntaxbaumGebrochenRational() {
	return funktionAlsVektorSyntaxbaum;
}

void RechnerLibrary::setSyntaxbaumGekuerzt(FunktionAlsVektorSyntaxbaum& neuerFunktionSyntaxbaum) {
	funktionAlsVektorSyntaxbaumGekürzt = neuerFunktionSyntaxbaum;
}

FunktionAlsVektorSyntaxbaum& RechnerLibrary::getSyntaxbaumGebrochenRationalGekürzt() {
	return funktionAlsVektorSyntaxbaum;
}

void RechnerLibrary::ausSyntaxbaumVektorErstellen(FunktionSyntaxbaum* aktuellerKnoten, vector<double>& functionAlsVectorLokal) {
	if (aktuellerKnoten->getLinkesChild() == nullptr && aktuellerKnoten->getRechtesChild() == nullptr) {
		if (aktuellerKnoten->getInhalt() == 'x' || aktuellerKnoten->getInhalt() == 'X') {
			if (functionAlsVectorLokal.size() < 2) {
				functionAlsVectorLokal.resize(2);
			}
			functionAlsVectorLokal[1] += 1;
		}
		else if (aktuellerKnoten->getInhalt() >= '0' || aktuellerKnoten->getInhalt() <= '9') {
			if (functionAlsVectorLokal.size() < 1) {
				functionAlsVectorLokal.resize(1);
			}
			functionAlsVectorLokal[0] += aktuellerKnoten->getInhalt() - 48;
		}
	}
	if (aktuellerKnoten->getLinkesChild() != nullptr) {
		if (aktuellerKnoten->getLinkesChild()->getLinkesChild() != nullptr) {
			ausSyntaxbaumVektorErstellen(aktuellerKnoten->getLinkesChild(), functionAlsVectorLokal);
		}
	}
	if (aktuellerKnoten->getRechtesChild() != nullptr) {
		if (aktuellerKnoten->getRechtesChild()->getRechtesChild() != nullptr&&(aktuellerKnoten->getInhalt() == '+' || aktuellerKnoten->getInhalt() == '^' || aktuellerKnoten->getInhalt() == '*' )) {
			ausSyntaxbaumVektorErstellen(aktuellerKnoten->getRechtesChild(), functionAlsVectorLokal);
		}
	
		if (aktuellerKnoten->getRechtesChild()->getRechtesChild() != nullptr && aktuellerKnoten->getInhalt() == '-' ) {
			ausSyntaxbaumVektorErstellen(aktuellerKnoten->getRechtesChild(), functionAlsVectorLokal);
		}
	}
	if (aktuellerKnoten->getInhalt() == '^') {
		if ((aktuellerKnoten->getLinkesChild()->getInhalt() == 'x' || aktuellerKnoten->getLinkesChild()->getInhalt() == 'X') && (aktuellerKnoten->getRechtesChild()->getInhalt() >= '1' && aktuellerKnoten->getRechtesChild()->getInhalt() <= '9')) {
			int hochZahl = aktuellerKnoten->getRechtesChild()->getInhalt() - 48;
			if (functionAlsVectorLokal.size() < hochZahl + 1) {
				functionAlsVectorLokal.resize(hochZahl + 1);
			}
			double multiplyer = 1.0;
			if (aktuellerKnoten->getParent() != nullptr) {
				if (aktuellerKnoten->getParent()->getInhalt() == '*' && aktuellerKnoten->getParent()->getRechtesChild() == aktuellerKnoten) {
					multiplyer = aktuellerKnoten->getParent()->getLinkesChild()->getInhalt() - 48;
				}
			}
			bool vorzeichen = false;
			FunktionSyntaxbaum* next = aktuellerKnoten;
			while (next->getParent() != nullptr) {
				/*if ((next->getParent()->getInhalt() == '+'|| next->getParent()->getInhalt() == '-') && next->getParent()->getLinkesChild() == next) {
					next = next->getParent();
				}
				else if ((next->getParent()->getInhalt() == '*' || next->getParent()->getInhalt() == '/')) {
					next = next->getParent();
				}*/
				
				if ((next->getParent()->getInhalt() == '+' || next->getParent()->getInhalt() == '-') && next->getParent()->getRechtesChild() == next) {
					if (next->getParent()->getInhalt() == '+') {
						vorzeichen = false;
						break;
					}
					else if (next->getParent()->getInhalt() == '-') {
						vorzeichen = true;
						break;
					}
				}
				else {
					next = next->getParent();
				}
			}
			//if (aktuellerKnoten->getParent()->getRechtesChild()->getInhalt() >= '1' && aktuellerKnoten->getParent()->getRechtesChild()->getInhalt() >= '9' && aktuellerKnoten->getParent()->getParent()->getInhalt() == '-') {				
			if(vorzeichen){
				functionAlsVectorLokal[hochZahl] -= multiplyer;
			}
			else {
				functionAlsVectorLokal[hochZahl] += multiplyer;
			}			
		}
	}

	if (aktuellerKnoten->getInhalt() == '*') {
		if ((aktuellerKnoten->getRechtesChild()->getInhalt() == 'x' || aktuellerKnoten->getRechtesChild()->getInhalt() == 'X') && (aktuellerKnoten->getLinkesChild()->getInhalt() >= '1' && aktuellerKnoten->getLinkesChild()->getInhalt() <= '9')) {

			if (functionAlsVectorLokal.size() < 2) {
				functionAlsVectorLokal.resize(2);
			}
			if (aktuellerKnoten->getParent() != nullptr) {
				if (aktuellerKnoten->getParent()->getParent() != nullptr) {
					if (aktuellerKnoten->getParent()->getRechtesChild()->getInhalt() >= '1' && aktuellerKnoten->getParent()->getRechtesChild()->getInhalt() <= '9' && aktuellerKnoten->getParent()->getParent()->getInhalt() == '-') {
						functionAlsVectorLokal[1] -= aktuellerKnoten->getLinkesChild()->getInhalt() - 48;
					}
					if (aktuellerKnoten->getParent()->getRechtesChild()->getInhalt() >= '1' && aktuellerKnoten->getParent()->getRechtesChild()->getInhalt() <= '9' && aktuellerKnoten->getParent()->getParent()->getInhalt() == '+') { //hier könnte noch was falsch sein
						functionAlsVectorLokal[1] += aktuellerKnoten->getLinkesChild()->getInhalt() - 48;
					}
				}
			
				else if (aktuellerKnoten->getParent()->getInhalt() == '-' && aktuellerKnoten->getParent()->getRechtesChild() == aktuellerKnoten) {
					functionAlsVectorLokal[1] -= aktuellerKnoten->getLinkesChild()->getInhalt() - 48;
				}
			}
			else {
				functionAlsVectorLokal[1] += aktuellerKnoten->getLinkesChild()->getInhalt() - 48;
			}			
		}
	}
	if (aktuellerKnoten->getInhalt() == '+') {
		if (aktuellerKnoten->getLinkesChild()->getInhalt() == '^' && (aktuellerKnoten->getRechtesChild()->getInhalt() >= '1' && aktuellerKnoten->getRechtesChild()->getInhalt() <= '9')) {
			if (functionAlsVectorLokal.size() < 1) {
				functionAlsVectorLokal.resize(1);
			}
			functionAlsVectorLokal[0] += aktuellerKnoten->getRechtesChild()->getInhalt() - 48;
		}
		if (aktuellerKnoten->getLinkesChild()->getInhalt() == '*' && (aktuellerKnoten->getRechtesChild()->getInhalt() >= '1' && aktuellerKnoten->getRechtesChild()->getInhalt() <= '9')) {
			if (functionAlsVectorLokal.size() < 1) {
				functionAlsVectorLokal.resize(1);
			}
			functionAlsVectorLokal[0] += aktuellerKnoten->getRechtesChild()->getInhalt() - 48;
		}
		if (aktuellerKnoten->getLinkesChild()->getInhalt() == 'x' || aktuellerKnoten->getLinkesChild()->getInhalt() == 'X' && (aktuellerKnoten->getRechtesChild()->getInhalt() >= '1' && aktuellerKnoten->getRechtesChild()->getInhalt() <= '9')) {
			if (functionAlsVectorLokal.size() < 2) {
				functionAlsVectorLokal.resize(2);
			}
			functionAlsVectorLokal[0] += aktuellerKnoten->getRechtesChild()->getInhalt() - 48;
			functionAlsVectorLokal[1] += 1;
		}
		if ((aktuellerKnoten->getLinkesChild()->getInhalt() >= '1' && aktuellerKnoten->getLinkesChild()->getInhalt() <= '9') && (aktuellerKnoten->getRechtesChild()->getInhalt() >= '1' && aktuellerKnoten->getRechtesChild()->getInhalt() <= '9')) {
			if (functionAlsVectorLokal.size() < 1) {
				functionAlsVectorLokal.resize(1);
			}
			functionAlsVectorLokal[0] += (aktuellerKnoten->getLinkesChild()->getInhalt() - 48) + (aktuellerKnoten->getRechtesChild()->getInhalt() - 48);
		}
	}
	else if (aktuellerKnoten->getInhalt() == '-') {
		if (aktuellerKnoten->getLinkesChild()->getInhalt() == '^' && (aktuellerKnoten->getRechtesChild()->getInhalt() >= '1' && aktuellerKnoten->getRechtesChild()->getInhalt() <= '9')) {
			if (functionAlsVectorLokal.size() < 1) {
				functionAlsVectorLokal.resize(1);
			}
			functionAlsVectorLokal[0] -= aktuellerKnoten->getRechtesChild()->getInhalt() - 48;
		}
		if (aktuellerKnoten->getLinkesChild()->getInhalt() == '*' && (aktuellerKnoten->getRechtesChild()->getInhalt() >= '1' && aktuellerKnoten->getRechtesChild()->getInhalt() <= '9')) {
			if (functionAlsVectorLokal.size() < 1) {
				functionAlsVectorLokal.resize(1);
			}
			functionAlsVectorLokal[0] -= aktuellerKnoten->getRechtesChild()->getInhalt() - 48;
		}
		if (aktuellerKnoten->getLinkesChild()->getInhalt() == 'x' || aktuellerKnoten->getLinkesChild()->getInhalt() == 'X' && (aktuellerKnoten->getRechtesChild()->getInhalt() >= '1' && aktuellerKnoten->getRechtesChild()->getInhalt() <= '9')) {
			if (functionAlsVectorLokal.size() < 2) {
				functionAlsVectorLokal.resize(2);
			}
			functionAlsVectorLokal[0] -= aktuellerKnoten->getRechtesChild()->getInhalt() - 48;
			functionAlsVectorLokal[1] += 1;
		}
	}
}

void RechnerLibrary::syntaxbaumErstellen() {
	ausSyntaxbaumVektorErstellen(&funktionSyntaxbaum,funktionAlsVector);
}

vector<double>& RechnerLibrary::funktionAbleiten(vector<double> &funktion) {
	vector<double>* abgeleiteteFunktion = new vector<double>();
	if (funktion.size() > 0) {
		abgeleiteteFunktion->resize(funktion.size() - 1);
		for (int j = funktion.size() - 1; j > 0; j--) {
			if (funktion[j] != 0) {
				abgeleiteteFunktion->at(j - 1) = funktion[j] * j;
			}
		}
	}
	return *abgeleiteteFunktion;
}

WCHAR* RechnerLibrary::stringsVerbinden(WCHAR* str1, WCHAR* str2, WCHAR operatorZeichen) {
	int laenge = 0;
	int i=0;
	while (str1[i] != 0) {
		laenge++;
		i++;
	}
	i = 0;

	while (str2[i] != 0) {
		laenge++;
		i++;
	}
	WCHAR* tmp = new WCHAR[laenge+5];
	i = 0;
	int j = 0;
	tmp[j++] = '(';
	while (str1[i] != 0) {
		tmp[j++] = str1[i];
		i++;
	}
	tmp[j++] = ')';
	tmp[j++] = operatorZeichen;
	tmp[j++] = '(';
	i = 0;
	while (str2[i] != 0) {
		tmp[j++] = str2[i];
		i++;
	}
	tmp[j++] = ')';
	tmp[j++] = 0;
	return tmp;
}

FunktionAlsVektorSyntaxbaum& RechnerLibrary::funktionAbleitenGebrochenRational(FunktionAlsVektorSyntaxbaum &funktion) {
	if (funktion.getInhaltTChar() == '/') { //Quotientenregel
		FunktionAlsVektorSyntaxbaum* gX = new FunktionAlsVektorSyntaxbaum();
		gX->setInhalt(funktion.getLinkesChild()->getInhaltVector());
		gX->setInhalt(funktionVectorToString(gX->getInhaltVector(), false));
		gX->setInhalt(funktion.getLinkesChild()->getInhaltTChar());
		FunktionAlsVektorSyntaxbaum* gStrichX = new FunktionAlsVektorSyntaxbaum();
		gStrichX->setInhalt(funktionAbleiten(funktion.getLinkesChild()->getInhaltVector()));
		gStrichX->setInhalt(funktionVectorToString(gStrichX->getInhaltVector(), false));
		gStrichX->setInhalt(funktion.getLinkesChild()->getInhaltTChar());
		FunktionAlsVektorSyntaxbaum* hX = new FunktionAlsVektorSyntaxbaum();
		hX->setInhalt(funktion.getRechtesChild()->getInhaltVector());
		hX->setInhalt(funktionVectorToString(hX->getInhaltVector(), false));
		hX->setInhalt(funktion.getRechtesChild()->getInhaltTChar());
		FunktionAlsVektorSyntaxbaum* hStrichX = new FunktionAlsVektorSyntaxbaum();
		hStrichX->setInhalt(funktionAbleiten(funktion.getRechtesChild()->getInhaltVector()));
		hStrichX->setInhalt(funktionVectorToString(hStrichX->getInhaltVector(), false));
		hStrichX->setInhalt(funktion.getRechtesChild()->getInhaltTChar());

		FunktionAlsVektorSyntaxbaum* fStrichX = new FunktionAlsVektorSyntaxbaum();
		fStrichX->setInhalt('/');
		

		FunktionAlsVektorSyntaxbaum* hXQuadrat = new FunktionAlsVektorSyntaxbaum(); //unter dem Bruchstrich
		hXQuadrat->setInhalt('*');
		hXQuadrat->setLinkesChild(hX);
		hXQuadrat->setRechtesChild(hX);
		hXQuadrat->setInhalt(stringsVerbinden(hX->getInhaltString(), hX->getInhaltString(), '*'));
		fStrichX->setRechtesChild(hXQuadrat);
		
		FunktionAlsVektorSyntaxbaum* hXMalgStrichXMinusgXMalHStrichX = new FunktionAlsVektorSyntaxbaum();; //Über dem Bruchstrich
		hXMalgStrichXMinusgXMalHStrichX->setInhalt('-');
		FunktionAlsVektorSyntaxbaum* hXMalgStrichX = new FunktionAlsVektorSyntaxbaum();
		hXMalgStrichX->setInhalt('*');
		hXMalgStrichX->setLinkesChild(hX);
		hXMalgStrichX->setRechtesChild(gStrichX);
		hXMalgStrichX->setInhalt(stringsVerbinden(hX->getInhaltString(),gStrichX->getInhaltString(),'*'));
		hXMalgStrichXMinusgXMalHStrichX->setLinkesChild(hXMalgStrichX);

		FunktionAlsVektorSyntaxbaum* gXMalHStrichX = new FunktionAlsVektorSyntaxbaum();
		gXMalHStrichX->setInhalt('*');
		gXMalHStrichX->setLinkesChild(gX);
		gXMalHStrichX->setRechtesChild(hStrichX);
		gXMalHStrichX->setInhalt(stringsVerbinden(gX->getInhaltString(), hStrichX->getInhaltString(), '*'));
		hXMalgStrichXMinusgXMalHStrichX->setRechtesChild(gXMalHStrichX);
		hXMalgStrichXMinusgXMalHStrichX->setInhalt(stringsVerbinden(hXMalgStrichX->getInhaltString(), gXMalHStrichX->getInhaltString(), '-'));
		fStrichX->setLinkesChild(hXMalgStrichXMinusgXMalHStrichX);

		fStrichX->setInhalt(stringsVerbinden(hXMalgStrichXMinusgXMalHStrichX->getInhaltString(), hXQuadrat->getInhaltString(), '/'));
		kuerzeSyntaxbaumGebrochenRational(fStrichX);
		return *fStrichX;
	}
	else if (funktion.getInhaltTChar() == '+' || funktion.getInhaltTChar() == '-' || funktion.getInhaltTChar() == '*' || funktion.getInhaltTChar() == '^') {
		
		FunktionAlsVektorSyntaxbaum* fStrichX = new FunktionAlsVektorSyntaxbaum();
		fStrichX->setInhalt(funktion.getInhaltTChar());
		fStrichX->setInhalt(funktionAbleiten(funktion.getInhaltVector()));
		fStrichX->setInhalt(funktionVectorToString(fStrichX->getInhaltVector(), false));
		return *fStrichX;
	}
	
}

void RechnerLibrary::setErsteAbleitung(vector<double> &funktion) {
	funktionAlsVectorErsteAbleitung = funktion;
}

void RechnerLibrary::setErsteAbleitungGebrochenRational(FunktionAlsVektorSyntaxbaum& funktion) {
	funktionAlsVectorErsteAbleitungGebrochenRational = funktion;
}

void RechnerLibrary::setZweiteAbleitung(vector<double> &funktion) {
	funktionAlsVectorZweiteAbleitung = funktion;
}

void RechnerLibrary::setZweiteAbleitungGebrochenRational(FunktionAlsVektorSyntaxbaum& funktion) {
	funktionAlsVectorZweiteAbleitungGebrochenRational = funktion;
}

void RechnerLibrary::setDritteAbleitung(vector<double> &funktion) {
	funktionAlsVectorDritteAbleitung = funktion;
}

void RechnerLibrary::setDritteAbleitungGebrochenRational(FunktionAlsVektorSyntaxbaum& funktion) {
	funktionAlsVectorDritteAbleitungGebrochenRational = funktion;
}

vector<double> RechnerLibrary::getErsteAbleitung() {
	return funktionAlsVectorErsteAbleitung;
}

FunktionAlsVektorSyntaxbaum& RechnerLibrary::getErsteAbleitungGebrochenRational() {
	return funktionAlsVectorErsteAbleitungGebrochenRational;
}

vector<double> RechnerLibrary::getZweiteAbleitung() {
	return funktionAlsVectorZweiteAbleitung;
}

FunktionAlsVektorSyntaxbaum& RechnerLibrary::getZweiteAbleitungGebrochenRational() {
	return funktionAlsVectorZweiteAbleitungGebrochenRational;
}

vector<double> RechnerLibrary::getDritteAbleitung() {
	return funktionAlsVectorDritteAbleitung;
}

FunktionAlsVektorSyntaxbaum& RechnerLibrary::getDritteAbleitungGebrochenRational() {
	return funktionAlsVectorDritteAbleitungGebrochenRational;
}

void RechnerLibrary::integrieren(vector<double> &funktion) {
	stammfunktionAlsVector.clear();
	stammfunktionAlsVector.resize(funktion.size() + 1);
	for (int j = 0; j < funktion.size(); j++) {		
		stammfunktionAlsVector[j + 1] = funktion[j] / (double)(j+1);		
	}
	int i = 0;
}

double RechnerLibrary::flaecheninhaltBerechnen(double linkeGrenze, double rechteGrenze) {	
	double linkeFlaeche = 0;
	double rechteFlaeche = 0;
	double flaecheGesamt = 0;
	regulaFalsiVerfahren();

	double tmp = 0;
	vector<double> sortierteNullstellen;

	for (int i = 0; i < nullstellen.size(); i++) {
		sortierteNullstellen.push_back(-nullstellen[i].x);
	}

	for (int i = 0; i < sortierteNullstellen.size(); i++) {
		for (int j = 0; j < sortierteNullstellen.size()-1; j++) {
			if (sortierteNullstellen[j] > sortierteNullstellen[j+1]) {
				tmp = sortierteNullstellen[j];
				sortierteNullstellen[j] = sortierteNullstellen[j + 1];
				sortierteNullstellen[j + 1] = tmp;
			}
		}
	}

	if (sortierteNullstellen.size() > 0) {
		if (sortierteNullstellen[sortierteNullstellen.size() - 1] < linkeGrenze) {
			linkeFlaeche = f(linkeGrenze, &stammfunktionAlsVector);
			rechteFlaeche = f(rechteGrenze, &stammfunktionAlsVector);
			flaecheGesamt += abs(rechteFlaeche - linkeFlaeche);
		}
		else if (sortierteNullstellen[0] > rechteGrenze) {
			linkeFlaeche = f(linkeGrenze, &stammfunktionAlsVector);
			rechteFlaeche = f(rechteGrenze, &stammfunktionAlsVector);
			flaecheGesamt += abs(rechteFlaeche - linkeFlaeche);
		}else {
			for (int i = 0; i < sortierteNullstellen.size(); i++) {
				if (sortierteNullstellen[i] >= linkeGrenze && sortierteNullstellen[i] <= rechteGrenze) {
					if (i == 0) {
						linkeFlaeche = f(linkeGrenze, &stammfunktionAlsVector);
						rechteFlaeche = f(sortierteNullstellen[i], &stammfunktionAlsVector);
					}
					else {
						if (sortierteNullstellen[i - 1] >= linkeGrenze && sortierteNullstellen[i - 1] <= rechteGrenze) {
							linkeFlaeche = f(sortierteNullstellen[i - 1], &stammfunktionAlsVector);
						}
						else {
							linkeFlaeche = f(linkeGrenze, &stammfunktionAlsVector);
						}
						rechteFlaeche = f(sortierteNullstellen[i], &stammfunktionAlsVector);
					}
					flaecheGesamt += abs(rechteFlaeche - linkeFlaeche);
				}
				if (i == sortierteNullstellen.size() - 1) {
					linkeFlaeche = f(sortierteNullstellen[i], &stammfunktionAlsVector);
					rechteFlaeche = f(rechteGrenze, &stammfunktionAlsVector);
					flaecheGesamt += abs(rechteFlaeche - linkeFlaeche);
				}
				if (i + 1 < sortierteNullstellen.size()) {
					if ((sortierteNullstellen[i + 1] >= linkeGrenze && sortierteNullstellen[i + 1] >= rechteGrenze)&&(sortierteNullstellen[i] <= linkeGrenze && sortierteNullstellen[i] <= rechteGrenze)) {
						linkeFlaeche = f(linkeGrenze, &stammfunktionAlsVector);
						rechteFlaeche = f(rechteGrenze, &stammfunktionAlsVector);
						flaecheGesamt += abs(rechteFlaeche - linkeFlaeche);
						break;
					}
					if ((sortierteNullstellen[i + 1] >= linkeGrenze && sortierteNullstellen[i + 1] >= rechteGrenze) && (sortierteNullstellen[i] >= linkeGrenze && sortierteNullstellen[i] <= rechteGrenze)) {
						linkeFlaeche = f(sortierteNullstellen[i], &stammfunktionAlsVector);
						rechteFlaeche = f(rechteGrenze, &stammfunktionAlsVector);
						flaecheGesamt += abs(rechteFlaeche - linkeFlaeche);
						break;
					}
				}
			}
		}
	}
	else {
		linkeFlaeche = f(linkeGrenze, &stammfunktionAlsVector);
		rechteFlaeche = f(rechteGrenze, &stammfunktionAlsVector);
		flaecheGesamt += abs(rechteFlaeche - linkeFlaeche);
	}
	return flaecheGesamt;
}

mutex simpson;
int maxCPU = 0;
int threadCounter = 0;
void simpson_thread(double linkeGrenze, double rechteGrenze, double h, double& fa, int id) {
	RechnerLibrary rechnerLibrary;
	double faLokal = 0;
	double gesamt = (rechteGrenze - h) - (linkeGrenze + h);
	double partGroesse = gesamt / (double)maxCPU;
	for (double i = (linkeGrenze + h) + (partGroesse*id); i < (linkeGrenze + h) + (partGroesse*(id + 1)); i += h) {
		faLokal += abs(h / 6 * (2 * abs(rechnerLibrary.f(i, &funktionAlsVector)) + 4 * abs(rechnerLibrary.f((((i + h) + i) / 2), &funktionAlsVector))));
	}
	simpson.lock();
	fa += abs(faLokal);
	threadCounter++;
	simpson.unlock();
}

double RechnerLibrary::flaecheninhaltMitSimpsonBerechnen(double linkeGrenze, double rechteGrenze) {
	double n = 1000;
	double fa = 0;
	double h = 0;
	double faOld = INFINITY;
	int runde = 0;
	double tau = abs(fa - faOld);
	double fehlerTolleranz = pow(10, -5);
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	maxCPU = sysinfo.dwNumberOfProcessors;
	while (fehlerTolleranz < tau && runde < 100) {
		h = (rechteGrenze - linkeGrenze) / n;
		simpson.lock();
		threadCounter = 0;
		fa = h / 6 * (abs(f(linkeGrenze, &funktionAlsVector)) + abs(f(rechteGrenze, &funktionAlsVector)));
		simpson.unlock();
		for (int i = 0; i < maxCPU; i++) {
			thread *t = new thread(&simpson_thread, linkeGrenze, rechteGrenze, h, ref(fa), i);
		}
		while (threadCounter < maxCPU) {
			Sleep(10);
		}
		simpson.lock();
		tau = abs(fa - faOld);
		faOld = fa;
		simpson.unlock();
		n = n * 2;
		runde++;
	}

	return fa;
}

mutex simpsonGebrochenRational;
void simpson_threadGebrochenRational(double linkeGrenze, double rechteGrenze, double h, double& fa, int id) {
	RechnerLibrary rechnerLibrary;
	double faLokal = 0;
	double gesamt = (rechteGrenze - h) - (linkeGrenze + h);
	double partGroesse = gesamt / (double)maxCPU;
	for (double i = (linkeGrenze + h) + (partGroesse*id); i < (linkeGrenze + h) + (partGroesse*(id + 1)); i += h) {
		faLokal += abs(h / 6 * (2 * abs(rechnerLibrary.getPunkt(i, &funktionAlsVektorSyntaxbaum)) + 4 * abs(rechnerLibrary.getPunkt((((i + h) + i) / 2), &funktionAlsVektorSyntaxbaum))));
	}
	simpsonGebrochenRational.lock();
	fa += abs(faLokal);
	threadCounter++;
	simpsonGebrochenRational.unlock();
}

double RechnerLibrary::flaecheninhaltMitSimpsonBerechnenGebrochenRational(double linkeGrenze, double rechteGrenze) {
	double n = 1000;
	double fa = 0;
	double h = 0;
	double faOld = INFINITY;
	int runde = 0;
	double tau = abs(fa - faOld);
	double fehlerTolleranz = pow(10, -5);
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	maxCPU = sysinfo.dwNumberOfProcessors;
	RechnerLibrary rechnerlibrary;
	while (fehlerTolleranz < tau && runde < 100) {
		h = (rechteGrenze - linkeGrenze) / n;
		simpsonGebrochenRational.lock();
		threadCounter = 0;
		fa = h / 6 * (abs(rechnerlibrary.getPunkt(linkeGrenze, &funktionAlsVektorSyntaxbaum)) + abs(rechnerlibrary.getPunkt(rechteGrenze, &funktionAlsVektorSyntaxbaum)));
		simpsonGebrochenRational.unlock();
		for (int i = 0; i < maxCPU; i++) {
			thread *t = new thread(&simpson_threadGebrochenRational, linkeGrenze, rechteGrenze, h, ref(fa), i);
		}
		while (threadCounter < maxCPU) {
			Sleep(10);
		}
		simpsonGebrochenRational.lock();
		tau = abs(fa - faOld);
		faOld = fa;
		simpsonGebrochenRational.unlock();
		n = n * 2;
		runde++;
	}

	return fa;
}

mutex trapetz;
void trapetz_thread(double linkeGrenze, double rechteGrenze, double h, double& fa, int id, vector<double>& funktion) {
	RechnerLibrary rechnerLibrary;
	double faLokal = 0;
	double gesamt = (rechteGrenze - h) - (linkeGrenze + h);
	double partGroesse = gesamt / (double)maxCPU;
	for (double i = (linkeGrenze + h) + (partGroesse*id); i < (linkeGrenze + h) + (partGroesse*(id + 1)); i += h) {
		faLokal += abs(h * ((rechnerLibrary.f(i, &funktion)+ rechnerLibrary.f(i+h, &funktion))/2));
	}
	trapetz.lock();
	fa += abs(faLokal);
	threadCounter++;
	trapetz.unlock();
}

double RechnerLibrary::flaecheninhaltMitTrapetzBerechnen(double linkeGrenze, double rechteGrenze) {
	double n = 1000;
	double fa = 0;
	double h = 0;
	double faOld = INFINITY;
	int runde = 0;
	double tau = abs(fa - faOld);
	double fehlerTolleranz = pow(10, -5);
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	maxCPU = sysinfo.dwNumberOfProcessors;
	while (fehlerTolleranz < tau && runde < 100) {
		h = (rechteGrenze - linkeGrenze) / n;
		trapetz.lock();
		threadCounter = 0;
		fa = 0;
		trapetz.unlock();
		for (int i = 0; i < maxCPU; i++) {
			thread *t = new thread(&trapetz_thread, linkeGrenze, rechteGrenze, h, ref(fa), i, ref(funktionAlsVector));
		}
		while (threadCounter < maxCPU) {
			Sleep(10);
		}
		trapetz.lock();
		tau = abs(fa - faOld);
		faOld = fa;
		trapetz.unlock();
		n = n * 2;
		runde++;
	}
	return fa;
}

mutex trapetzGebrochenRational;
void trapetz_threadGebrochenRational(double linkeGrenze, double rechteGrenze, double h, double& fa, int id, FunktionAlsVektorSyntaxbaum& funktion) {
	RechnerLibrary rechnerLibrary;
	double faLokal = 0;
	double gesamt = (rechteGrenze - h) - (linkeGrenze + h);
	double partGroesse = gesamt / (double)maxCPU;
	for (double i = (linkeGrenze + h) + (partGroesse*id); i < (linkeGrenze + h) + (partGroesse*(id + 1)); i += h) {
		faLokal += abs(h * ((rechnerLibrary.getPunkt(i, &funktion) + rechnerLibrary.getPunkt(i + h, &funktion)) / 2));
	}
	trapetzGebrochenRational.lock();
	fa += abs(faLokal);
	threadCounter++;
	trapetzGebrochenRational.unlock();
}

double RechnerLibrary::flaecheninhaltMitTrapetzBerechnenGebrochenRational(double linkeGrenze, double rechteGrenze) {
	double n = 1000;
	double fa = 0;
	double h = 0;
	double faOld = INFINITY;
	int runde = 0;
	double tau = abs(fa - faOld);
	double fehlerTolleranz = pow(10, -5);
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	maxCPU = sysinfo.dwNumberOfProcessors;
	while (fehlerTolleranz < tau && runde < 100) {
		h = (rechteGrenze - linkeGrenze) / n;
		trapetzGebrochenRational.lock();
		threadCounter = 0;
		fa = 0;
		trapetzGebrochenRational.unlock();
		for (int i = 0; i < maxCPU; i++) {
			thread *t = new thread(&trapetz_threadGebrochenRational, linkeGrenze, rechteGrenze, h, ref(fa), i, ref(funktionAlsVektorSyntaxbaum));
		}
		while (threadCounter < maxCPU) {
			Sleep(10);
		}
		trapetzGebrochenRational.lock();
		tau = abs(fa - faOld);
		faOld = fa;
		trapetzGebrochenRational.unlock();
		n = n * 2;
		runde++;
	}
	return fa;
}

void bogenlaenge_thread(double linkeGrenze, double rechteGrenze, double h, double& fa, int id, vector<double>& funktion) {
	RechnerLibrary rechnerLibrary;
	double faLokal = 0;
	double gesamt = (rechteGrenze - h) - (linkeGrenze + h);
	double partGroesse = gesamt / (double)maxCPU;
	double dy = 0;
	for (double i = (linkeGrenze + h) + (partGroesse*id); i < (linkeGrenze + h) + (partGroesse*(id + 1)); i += h) {
		dy = abs(rechnerLibrary.f(i, &funktion) - rechnerLibrary.f(i - h, &funktion));
		faLokal += sqrt((dy *dy) + (h*h));
	}
	trapetz.lock();
	fa += abs(faLokal);
	threadCounter++;
	trapetz.unlock();
}

double RechnerLibrary::bogenlaengeMitTrapetzBerechnen(double linkeGrenze, double rechteGrenze) {

	vector<double> bogenlaengeIntegral;
	bogenlaengeIntegral.resize(funktionAlsVectorErsteAbleitung.size() + 1);
	for (int i = 0; i < funktionAlsVectorErsteAbleitung.size(); i++) {
		bogenlaengeIntegral[i + 1] = funktionAlsVectorErsteAbleitung[i] * funktionAlsVectorErsteAbleitung[i];
	}

	/*double linkeFlaeche = sqrt(1 + abs(f(linkeGrenze, &bogenlaengeIntegral)));
	double rechteFlaeche = sqrt(1 + abs(f(rechteGrenze, &bogenlaengeIntegral)));
	if (linkeFlaeche < 0) {
	linkeFlaeche = abs(linkeFlaeche);
	}if (rechteFlaeche < 0) {
	rechteFlaeche = abs(rechteFlaeche);
	}

	double flaecheGesamt;
	if (linkeGrenze <= 0 && rechteGrenze >= 0) {
	flaecheGesamt = rechteFlaeche + linkeFlaeche;
	}if (linkeGrenze > 0 && rechteGrenze > 0) {
	flaecheGesamt = rechteFlaeche - linkeFlaeche;
	}*/


	double n = 1000;
	double fa = 0;
	double h = 0;
	double faOld = INFINITY;
	int runde = 0;
	double tau = abs(fa - faOld);
	double fehlerTolleranz = pow(10, -5);
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	maxCPU = sysinfo.dwNumberOfProcessors;
	while (fehlerTolleranz < tau && runde < 100) {
		h = (rechteGrenze - linkeGrenze) / n;
		trapetz.lock();
		threadCounter = 0;

		fa = 0;
		trapetz.unlock();
		for (int i = 0; i < maxCPU; i++) {
			thread *t = new thread(&bogenlaenge_thread, linkeGrenze, rechteGrenze, h, ref(fa), i, ref(funktionAlsVector));
		}
		while (threadCounter < maxCPU) {
			Sleep(10);
		}
		//fa = sqrt(1 + fa);
		trapetz.lock();
		tau = abs(fa - faOld);
		faOld = fa;
		trapetz.unlock();
		n = n * 2;
		runde++;
	}
	return fa;
}


void bogenlaenge_threadGebrochenRational(double linkeGrenze, double rechteGrenze, double h, double& fa, int id, FunktionAlsVektorSyntaxbaum& funktion) {
	RechnerLibrary rechnerLibrary;
	double faLokal = 0;
	double gesamt = (rechteGrenze - h) - (linkeGrenze + h);
	double partGroesse = gesamt / (double)maxCPU;
	double dy = 0;
	for (double i = (linkeGrenze + h) + (partGroesse*id); i < (linkeGrenze + h) + (partGroesse*(id + 1)); i += h) {
		dy = abs(rechnerLibrary.getPunkt(i, &funktion) - rechnerLibrary.getPunkt(i - h, &funktion));
		faLokal += sqrt((dy *dy) + (h*h));
	}
	trapetzGebrochenRational.lock();
	fa += abs(faLokal);
	threadCounter++;
	trapetzGebrochenRational.unlock();
}

double RechnerLibrary::bogenlaengeMitTrapetzBerechnenGebrochenRational(double linkeGrenze, double rechteGrenze) {

	/*vector<double> bogenlaengeIntegral;
	bogenlaengeIntegral.resize(funktionAlsVectorErsteAbleitung.size() *2);
	for (int i = 0; i < funktionAlsVectorErsteAbleitung.size(); i++) {
		for (int j = 0; j < funktionAlsVectorErsteAbleitung.size(); j++) {
			bogenlaengeIntegral[i + j] = funktionAlsVectorErsteAbleitung[i] * funktionAlsVectorErsteAbleitung[j];
		}
	}

	double linkeFlaeche = sqrt(1 + abs(f(linkeGrenze, &bogenlaengeIntegral)));
	double rechteFlaeche = sqrt(1 + abs(f(rechteGrenze, &bogenlaengeIntegral)));
	if (linkeFlaeche < 0) {
		linkeFlaeche = abs(linkeFlaeche);
	}if (rechteFlaeche < 0) {
		rechteFlaeche = abs(rechteFlaeche);
	}

	double flaecheGesamt;
	if (linkeGrenze <= 0 && rechteGrenze >= 0) {
		flaecheGesamt = rechteFlaeche + linkeFlaeche;
	}if (linkeGrenze > 0 && rechteGrenze > 0) {
		flaecheGesamt = rechteFlaeche - linkeFlaeche;
	}*/


	double n = 1000;
	double fa = 0;
	double h = 0;
	double faOld = INFINITY;
	int runde = 0;
	double tau = abs(fa - faOld);
	double fehlerTolleranz = pow(10, -5);
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	maxCPU = sysinfo.dwNumberOfProcessors;
	while (fehlerTolleranz < tau && runde < 100) {
		h = (rechteGrenze - linkeGrenze) / n;
		trapetzGebrochenRational.lock();
		threadCounter = 0;

		fa = 0;
		trapetzGebrochenRational.unlock();
		for (int i = 0; i < maxCPU; i++) {
			thread *t = new thread(&bogenlaenge_threadGebrochenRational, linkeGrenze, rechteGrenze, h, ref(fa), i, ref(funktionAlsVektorSyntaxbaum));
		}
		while (threadCounter < maxCPU) {
			Sleep(10);
		}
		//fa = sqrt(1 + fa);
		trapetzGebrochenRational.lock();
		tau = abs(fa - faOld);
		faOld = fa;
		trapetzGebrochenRational.unlock();
		n = n * 2;
		runde++;
	}
	return fa;
}

vector<vector<double>> RechnerLibrary::polynomdivision(vector<double>& zaehler, vector<double>& nenner) {
	vector<double> ergebnis; 
	vector<double> tmp;
	vector<double> rest;
	rest.resize(zaehler.size());
	tmp.resize(zaehler.size());
	ergebnis.resize(zaehler.size() - (nenner.size()-1));
	int round = 0;
	int i, j, k;
	for ( i = zaehler.size()-1 ,  j= nenner.size() - 1 ,  k= ergebnis.size()-1; i > -1+ (nenner.size()-1); i--) {
		ergebnis[k- round] = zaehler[i] / nenner[j];
		for (int g = tmp.size()-1; j > -1 ; g--,j--) {
			tmp[g-round] = nenner[j] * ergebnis[k-round];
		}
		j = nenner.size() - 1;
		
		for (int g = zaehler.size() - 1 - round; g > zaehler.size() - 1 - round - nenner.size();g--) {
			zaehler[g] = zaehler[g] - tmp[g];
			if ((-1 + (nenner.size() - 1))<i) {
				tmp[g] = 0;
			}			
		}
		round++;
	}
	for (int i = 0; i < zaehler.size(); i++) {
		rest[i] = zaehler[i] - tmp[i];
	}
	vector<vector<double>> ErgebnisMitRest;
	ErgebnisMitRest.push_back(ergebnis);
	ErgebnisMitRest.push_back(rest);
	return ErgebnisMitRest;
}

vector<double> RechnerLibrary::polynomAddition(vector<double>& summand1, vector<double>& summand2) {
	int groesserePolynom = 0;
	if (summand1.size() > summand2.size()) {
		groesserePolynom = summand1.size();
	}
	else {
		groesserePolynom = summand2.size();
	}
	vector<double> ergebnis;
	ergebnis.resize(groesserePolynom);
	for (int i = 0; i < groesserePolynom;i++) {
		if (i>=summand1.size()) {
			ergebnis[i] = summand2[i];
		}
		else if(i>=summand2.size()) {
			ergebnis[i] = summand1[i];
		}
		else{
			ergebnis[i] = summand1[i] + summand2[i];
		}
		
	}
	return ergebnis;
}

vector<double> RechnerLibrary::polynomSubtraktion(vector<double>& subtrahend1, vector<double>& subtrahend2) {
	int groesserePolynom = 0;
	if (subtrahend1.size() > subtrahend2.size()) {
		groesserePolynom = subtrahend1.size();
	}
	else {
		groesserePolynom = subtrahend2.size();
	}
	vector<double> ergebnis;
	ergebnis.resize(groesserePolynom);
	for (int i = 0; i < groesserePolynom; i++) {
		if (i >= subtrahend1.size()) {
			ergebnis[i] = subtrahend2[i];
		}
		else if (i >= subtrahend2.size()) {
			ergebnis[i] = subtrahend1[i];
		}
		else {
			ergebnis[i] = subtrahend1[i] - subtrahend2[i];
		}
		
	}
	return ergebnis;
}

vector<double> RechnerLibrary::polynomMultiplikation(vector<double>& poly1, vector<double>& poly2) {

	vector<double> ergebnis;
	ergebnis.resize(poly1.size()+ poly2.size()-1);
	for (int i = 0; i < poly1.size(); i++) {
		for (int j = 0; j < poly2.size(); j++) {
			ergebnis[i+j] += poly1[i] * poly2[j];
		}	
	}
	return ergebnis;
}

