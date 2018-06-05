#include "stdafx.h"
#include "FunktionAlsVektorSyntaxbaum.h"


FunktionAlsVektorSyntaxbaum::FunktionAlsVektorSyntaxbaum(){
	parent = nullptr;
	linkesChild = nullptr;
	rechtesChild = nullptr;
	inhaltKnotenSymbol = 0;
	inhaltKnotenVektor = { 0 };
}


FunktionAlsVektorSyntaxbaum::~FunktionAlsVektorSyntaxbaum() {
}

FunktionAlsVektorSyntaxbaum* FunktionAlsVektorSyntaxbaum::getParent() {
	return parent;
}

void FunktionAlsVektorSyntaxbaum::setLinkesChild(FunktionAlsVektorSyntaxbaum* neuesChild) {
	linkesChild = neuesChild;
}

void FunktionAlsVektorSyntaxbaum::setRechtesChild(FunktionAlsVektorSyntaxbaum* neuesChild) {
	rechtesChild = neuesChild;
}

void FunktionAlsVektorSyntaxbaum::setUnaeresChild(FunktionAlsVektorSyntaxbaum* neuesChild) {
	unaeresChild = neuesChild;
}

FunktionAlsVektorSyntaxbaum* FunktionAlsVektorSyntaxbaum::getLinkesChild() {
	return linkesChild;
}

FunktionAlsVektorSyntaxbaum* FunktionAlsVektorSyntaxbaum::getRechtesChild() {
	return rechtesChild;
}

FunktionAlsVektorSyntaxbaum* FunktionAlsVektorSyntaxbaum::getUnaeresChild() {
	return unaeresChild;
}

void FunktionAlsVektorSyntaxbaum::setRoot(FunktionAlsVektorSyntaxbaum* neuesRoot) {
	rootVectoren = neuesRoot;
}

void FunktionAlsVektorSyntaxbaum::setParent(FunktionAlsVektorSyntaxbaum* neuerParent) {
	parent = neuerParent;
}

void  FunktionAlsVektorSyntaxbaum::setInhalt(TCHAR neuerInhalt) {
	inhaltKnotenSymbol = neuerInhalt;
}

TCHAR FunktionAlsVektorSyntaxbaum::getInhaltTChar() {
	return inhaltKnotenSymbol;
}

void  FunktionAlsVektorSyntaxbaum::setInhalt(vector<double> neuerInhalt) {
	inhaltKnotenVektor = neuerInhalt;
}

void  FunktionAlsVektorSyntaxbaum::setInhalt(TCHAR* neuerInhalt) {
	inhaltKnotenString = neuerInhalt;
}

vector<double> FunktionAlsVektorSyntaxbaum::getInhaltVector() {
	return inhaltKnotenVektor;
}

TCHAR* FunktionAlsVektorSyntaxbaum::getInhaltString() {
	return inhaltKnotenString;
}

void  FunktionAlsVektorSyntaxbaum::setIndex(int neuerIndex) {
	index = neuerIndex;
}

int FunktionAlsVektorSyntaxbaum::getIndex() {
	return index;
}