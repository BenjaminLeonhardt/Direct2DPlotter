#pragma once
#include "stdafx.h"
#include <vector>

using namespace std;
template <class T>
class Matrix{
private:
	vector<T> matrix;
	int breite;
	int hoehe;
public:
	Matrix();
	~Matrix();
	void setMatrix(vector<T> neuerVector, int b, int h);
	vector<T> getMatrix();
	Matrix<T>& operator+=(Matrix &v2);
	Matrix<T>& operator-=(const Matrix &v2);
	Matrix<T>& operator+(const Matrix &v2);
	Matrix<T>& operator-(const Matrix &v2);
	Matrix<T> operator*( Matrix &v2);
	Matrix<T>& operator/( Matrix &v2);
	T operator[](const __int32 &value);
	T& operator[]( __int32 &value) ;
};

template <class T>
Matrix<T>::Matrix(){

}

template <class T>
Matrix<T>::~Matrix(){

}

template <class T>
void Matrix<T>::setMatrix(vector<T> neuerVector, int b, int h) {
	this->breite = b;
	this->hoehe = h;
	matrix = neuerVector;
}

template <class T>
vector<T> Matrix<T>::getMatrix() {
	return matrix;
}

template <class T>
Matrix<T>& Matrix<T>::operator+=(Matrix<T> &v2) {
	if (v2.breite != this->breite|| v2.hoehe != this->hoehe) {
		return;
	}
	vector<T> neuerVector;
	for (int i = 0;i < breite*hoehe;i++) {
		neuerVector.push_back(v2[i] + this[i]);
	}
	return neuerVector;
}

template <class T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T> &v2) {
	if (v2.breite != this->breite || v2.hoehe != this->hoehe) {
		return;
	}
	vector<T> neuerVector;
	for (int i = 0;i < breite*hoehe;i++) {
		neuerVector.push_back(v2[i] - this[i]);
	}
	return neuerVector;
}

template <class T>
Matrix<T>& Matrix<T>::operator+(const Matrix<T> &v2) {
	if (v2.breite != this->breite || v2.hoehe != this->hoehe) {
		return;
	}
	vector<T> neuerVector;
	for (int i = 0;i < breite*hoehe;i++) {
		neuerVector.push_back(v2[i] + this[i]);
	}
	return neuerVector;
}

template <class T>
Matrix<T>& Matrix<T>::operator-(const Matrix<T> &v2) {
	if (v2.breite != this->breite || v2.hoehe != this->hoehe) {
		return;
	}
	vector<T> neuerVector;
	for (int i = 0;i < breite*hoehe;i++) {
		neuerVector.push_back(v2[i] - this[i]);
	}
	return neuerVector;
}

template <class T>
Matrix<T> Matrix<T>::operator*(Matrix<T> &v2) {
	if (v2.breite != this->hoehe && v2.hoehe != this->breite) {
		return *this;
	}
	vector<T> neuerVector;
	neuerVector.resize(this->hoehe * v2.breite);
	T tmp=0;

	for (int i = 0;i < this->hoehe;i++) {
		for (int j = 0;j < v2.breite;j++) {	
			T tmp = 0;
			for (int k = 0;k < this->breite;k++) {
				T a = this->getMatrix()[k + this->breite * i];
				T b = v2.getMatrix()[j + v2.breite * k];
				tmp += a*b;
			}
			neuerVector[v2.breite*i+j]=tmp;
		}
	}
	Matrix<int> mErg;
	mErg.setMatrix(neuerVector, this->hoehe, v2.breite);
	return mErg;
}

template <class T>
Matrix<T>& Matrix<T>::operator/( Matrix<T> &v2) {
		
}

template <class T>
T Matrix<T>::operator[](const __int32 &value) {
	return this->matix[value];
}

template <class T>
T& Matrix<T>::operator[]( __int32 &value) {
	return this->matix[value];
}


