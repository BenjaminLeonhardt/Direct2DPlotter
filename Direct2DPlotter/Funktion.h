#pragma once
class Funktion
{
public:
	FunktionAlsVektorSyntaxbaum eingegebeneFunktion;
	FunktionAlsVektorSyntaxbaum funktionGekuerzt;
	vector<Funktion> ableitungen;
	vector<Funktion> integrale;
	vector<Vector2D> nullstellen;
	vector<Vector2D> polstellen;
	vector<Vector2D> extremstellen;
	vector<Vector2D> wendepunkte;
	Funktion();
	~Funktion();
};

