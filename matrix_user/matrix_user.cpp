#include <iostream>
#include "matrix.h"
using namespace std;
using namespace mat;

int main()
{
	double arr1[][3] = { {1,2,3},{4,5,6},{7,8,9} };
	double arr2[][3] = { {1,2,3},{4,5,6},{7,8,9} };

	Matrix m1(3, 3, &arr1[0][0]);

	cout << Matrix::matNumber() << endl;

	Matrix m2(3, 3, &arr2[0][0]);

	cout << Matrix::matNumber() << endl;

	Matrix m3 = m1 + m2;

	cout << Matrix::matNumber() << endl;

	Matrix m4(m1);

	cout << Matrix::matNumber() << endl;

	cout << m1 << endl << endl;

	cout << Matrix::matNumber() << endl;

	cout << m2 << endl << endl;

	cout << Matrix::matNumber() << endl;

	cout << m3 << endl << endl;

	cout << Matrix::matNumber() << endl;

	cout << m4 << endl << endl;

	cout << Matrix::matNumber() << endl;

	/*forAll(m1, ip)
	{
		cout << m1[ip] << endl;
	}*/

	cout << m1.sum(c) << endl << endl;

	cout << Matrix::matNumber() << endl;

	Matrix m5 = m2.sum(c);

	cout << Matrix::matNumber() << endl;

	/*cout << m1.max(c) << endl << endl;

	cout << Matrix::matNumber() << endl;*/

	//cout << m1.sum(r) << endl << endl;

	//cout << m1.sum(c) << endl << endl;
}