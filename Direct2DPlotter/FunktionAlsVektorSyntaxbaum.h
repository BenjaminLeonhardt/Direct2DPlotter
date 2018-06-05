#pragma once
class FunktionAlsVektorSyntaxbaum
{
	TCHAR inhaltKnotenSymbol;
	TCHAR* inhaltKnotenString;
	vector<double> inhaltKnotenVektor;
	FunktionAlsVektorSyntaxbaum* linkesChild;
	FunktionAlsVektorSyntaxbaum* rechtesChild;
	FunktionAlsVektorSyntaxbaum* unaeresChild;
	FunktionAlsVektorSyntaxbaum* parent;
	int index;

public:
	FunktionAlsVektorSyntaxbaum();
	~FunktionAlsVektorSyntaxbaum();
	void setLinkesChild(FunktionAlsVektorSyntaxbaum*);
	void setRechtesChild(FunktionAlsVektorSyntaxbaum*);
	void setUnaeresChild(FunktionAlsVektorSyntaxbaum*);
	void setParent(FunktionAlsVektorSyntaxbaum*);
	FunktionAlsVektorSyntaxbaum* getParent();
	void setRoot(FunktionAlsVektorSyntaxbaum*);
	FunktionAlsVektorSyntaxbaum* getLinkesChild();
	FunktionAlsVektorSyntaxbaum* getRechtesChild();
	FunktionAlsVektorSyntaxbaum* getUnaeresChild();
	void setInhalt(TCHAR);
	void setInhalt(vector<double>);
	void setInhalt(TCHAR*);
	TCHAR getInhaltTChar();
	vector<double> getInhaltVector();
	TCHAR* getInhaltString();
	void setIndex(int);
	int getIndex();

};
static FunktionAlsVektorSyntaxbaum* rootVectoren;