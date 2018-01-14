#pragma once
class FunktionSyntaxbaum{
	

	TCHAR inhaltKnoten;
	FunktionSyntaxbaum* linkesChild;
	FunktionSyntaxbaum* rechtesChild;
	FunktionSyntaxbaum* parent;
	int index;

public:
	FunktionSyntaxbaum();
	~FunktionSyntaxbaum();
	void setLinkesChild(FunktionSyntaxbaum*);
	void setRechtesChild(FunktionSyntaxbaum*);
	void setParent(FunktionSyntaxbaum*);
	FunktionSyntaxbaum* getParent();
	void setRoot(FunktionSyntaxbaum*);
	FunktionSyntaxbaum* getLinkesChild();
	FunktionSyntaxbaum* getRechtesChild();
	void setInhalt(TCHAR);
	TCHAR getInhalt();
	void setIndex(int);
	int getIndex();
};
static FunktionSyntaxbaum* root;
