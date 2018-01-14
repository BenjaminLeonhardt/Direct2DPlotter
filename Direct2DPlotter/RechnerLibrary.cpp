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
vector<Vector2D*> regulaFalsiNullstellen;

FunktionSyntaxbaum funktionSyntaxbaum;
vector<Vector2D> nullstellen;
vector<Vector2D> extremstellen;
vector<Vector2D> wendepunkte;

double tolleranz = 1 * pow(10, -10);
int threadReadyCounterNewton = 0;
int threadReadyCounterRegulaFalsi = 0;

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

void RechnerLibrary::parseFuntionBuffer(FunktionSyntaxbaum& neuerFunktionSyntaxbaum,WCHAR* functionAlsString) {
	neuerFunktionSyntaxbaum = FunktionSyntaxbaum();
	neuerFunktionSyntaxbaum.setLinkesChild(nullptr);
	neuerFunktionSyntaxbaum.setRechtesChild(nullptr);
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

void RechnerLibrary::resetVals() {
	nullstellen.clear();
	extremstellen.clear();
	wendepunkte.clear();

}

void RechnerLibrary::setSyntaxbaum(FunktionSyntaxbaum& neuerFunktionSyntaxbaum) {
	funktionSyntaxbaum = neuerFunktionSyntaxbaum;
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
	if (aktuellerKnoten->getLinkesChild()->getLinkesChild() != nullptr) {
		ausSyntaxbaumVektorErstellen(aktuellerKnoten->getLinkesChild(), functionAlsVectorLokal);
	}
	if (aktuellerKnoten->getRechtesChild()->getRechtesChild() != nullptr&&(aktuellerKnoten->getInhalt() == '+' || aktuellerKnoten->getInhalt() == '^' || aktuellerKnoten->getInhalt() == '*' )) {
		ausSyntaxbaumVektorErstellen(aktuellerKnoten->getRechtesChild(), functionAlsVectorLokal);
	}
	if (aktuellerKnoten->getRechtesChild()->getRechtesChild() != nullptr && aktuellerKnoten->getInhalt() == '-' ) {
		ausSyntaxbaumVektorErstellen(aktuellerKnoten->getRechtesChild(), functionAlsVectorLokal);
	}

	if (aktuellerKnoten->getInhalt() == '^') {
		if ((aktuellerKnoten->getLinkesChild()->getInhalt() == 'x' || aktuellerKnoten->getLinkesChild()->getInhalt() == 'X') && (aktuellerKnoten->getRechtesChild()->getInhalt() >= '1' && aktuellerKnoten->getRechtesChild()->getInhalt() <= '9')) {
			int hochZahl = aktuellerKnoten->getRechtesChild()->getInhalt() - 48;
			if (functionAlsVectorLokal.size() < hochZahl + 1) {
				functionAlsVectorLokal.resize(hochZahl + 1);
			}
			double multiplyer = 1.0;
			if (aktuellerKnoten->getParent()->getInhalt() == '*' && aktuellerKnoten->getParent()->getRechtesChild() == aktuellerKnoten) {
				multiplyer = aktuellerKnoten->getParent()->getLinkesChild()->getInhalt() - 48;
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
			if (aktuellerKnoten->getParent()->getRechtesChild()->getInhalt()>='1' && aktuellerKnoten->getParent()->getRechtesChild()->getInhalt() <= '9' && aktuellerKnoten->getParent()->getParent()->getInhalt()=='-') {
				functionAlsVectorLokal[1] -= aktuellerKnoten->getLinkesChild()->getInhalt() - 48;
			}
			else if (aktuellerKnoten->getParent()->getInhalt() == '-' && aktuellerKnoten->getParent()->getRechtesChild() == aktuellerKnoten) {
				functionAlsVectorLokal[1] -= aktuellerKnoten->getLinkesChild()->getInhalt() - 48;
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
	vector<double>* abgeleiteteFunktion= new vector<double>();
	abgeleiteteFunktion->resize(funktion.size() - 1);
	for (int j = funktion.size() - 1; j > 0; j--) {
		if (funktion[j] != 0) {
			abgeleiteteFunktion->at(j - 1) = funktion[j] * j;
		}
	}
	return *abgeleiteteFunktion;
}

void RechnerLibrary::setErsteAbleitung(vector<double> &funktion) {
	funktionAlsVectorErsteAbleitung = funktion;
}

void RechnerLibrary::setZweiteAbleitung(vector<double> &funktion) {
	funktionAlsVectorZweiteAbleitung = funktion;
}

void RechnerLibrary::setDritteAbleitung(vector<double> &funktion) {
	funktionAlsVectorDritteAbleitung = funktion;
}

vector<double> RechnerLibrary::getErsteAbleitung() {
	return funktionAlsVectorErsteAbleitung;
}

vector<double> RechnerLibrary::getZweiteAbleitung() {
	return funktionAlsVectorZweiteAbleitung;
}

vector<double> RechnerLibrary::getDritteAbleitung() {
	return funktionAlsVectorDritteAbleitung;
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
	for (double i = (linkeGrenze + h)+(partGroesse*id); i < (linkeGrenze + h)+(partGroesse*(id+1)); i += h) {
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


void bogenlaenge_thread(double linkeGrenze, double rechteGrenze, double h, double& fa, int id, vector<double>& funktion) {
	RechnerLibrary rechnerLibrary;
	double faLokal = 0;
	double gesamt = (rechteGrenze - h) - (linkeGrenze + h);
	double partGroesse = gesamt / (double)maxCPU;
	double dy = 0;
	for (double i = (linkeGrenze + h) + (partGroesse*id); i < (linkeGrenze + h) + (partGroesse*(id + 1)); i += h) {
		dy = abs(rechnerLibrary.f(i, &funktion) - rechnerLibrary.f(i - h, &funktion));
		faLokal += sqrt((dy *dy) +(h*h));
	}
	trapetz.lock();
	fa += abs(faLokal);
	threadCounter++;
	trapetz.unlock();
}

double RechnerLibrary::bogenlaengeMitTrapetzBerechnen(double linkeGrenze, double rechteGrenze) {

	vector<double> bogenlaengeIntegral;
	bogenlaengeIntegral.resize(funktionAlsVectorErsteAbleitung.size()+1);
	for (int i = 0; i < funktionAlsVectorErsteAbleitung.size(); i++) {
		bogenlaengeIntegral[i+1] = funktionAlsVectorErsteAbleitung[i] * funktionAlsVectorErsteAbleitung[i];
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