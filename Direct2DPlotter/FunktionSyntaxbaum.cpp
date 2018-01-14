#include "stdafx.h"
#include "FunktionSyntaxbaum.h"



FunktionSyntaxbaum::FunktionSyntaxbaum(){
	parent = nullptr;
	linkesChild = nullptr;
	rechtesChild = nullptr;
	inhaltKnoten = 0;
}


FunktionSyntaxbaum::~FunktionSyntaxbaum(){

}

FunktionSyntaxbaum* FunktionSyntaxbaum::getParent() {
	return parent;
}

void FunktionSyntaxbaum::setLinkesChild(FunktionSyntaxbaum* neuesChild) {
	linkesChild = neuesChild;
}

void FunktionSyntaxbaum::setRechtesChild(FunktionSyntaxbaum* neuesChild) {
	rechtesChild = neuesChild;
}

FunktionSyntaxbaum* FunktionSyntaxbaum::getLinkesChild() {
	return linkesChild;
}

FunktionSyntaxbaum* FunktionSyntaxbaum::getRechtesChild() {
	return rechtesChild;
}

void FunktionSyntaxbaum::setRoot(FunktionSyntaxbaum* neuesRoot) {
	root = neuesRoot;
}

void FunktionSyntaxbaum::setParent(FunktionSyntaxbaum* neuerParent) {
	parent = neuerParent;
}

void  FunktionSyntaxbaum::setInhalt(TCHAR neuerInhalt) {
	inhaltKnoten = neuerInhalt;
}

TCHAR FunktionSyntaxbaum::getInhalt() {
	return inhaltKnoten;
}

void  FunktionSyntaxbaum::setIndex(int neuerIndex) {
	index = neuerIndex;
}

int FunktionSyntaxbaum::getIndex() {
	return index;
}