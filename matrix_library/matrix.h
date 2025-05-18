#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <string>
using namespace std;

#define forAll(Matrix, ip) for(Node* ip = Matrix.begin(); ip;)

enum matChar { c, r };
enum copyMat {copyDef, tempObject};

namespace err
{
	void diffSizes();
	void badSlice();
	void badIndex();
	void invParam();
	void sqMatrix();
	void multMismatch();
}

namespace mat
{
	class Node							// Type of the element of the Matrix
	{
		double data;					// Holds the data
		Node* right;					// Points to the next element on the right
		Node* down;						// Points to the next element below

		Node();							// Prevents users from main() from creating a Matrix element
		void operator delete(void*);	// Prevents users from main() from deleting a Matrix element

		friend class Matrix;			// Grants access to class Matrix. Only Matrix class can be used to create/manipulate individual elements
	};

	class Matrix
	{
		int row, column;												// Number of rows and column
		static int matNum;												// Static variable to keep count of matrix number
		static int numOfMat;											// Static variable to keep count of the number of matrices in the workspace
		int num;														// Current matrix number

		Node* head;														// Head of the linked list
		Node* tail;														// Tail of the linked list

		Matrix();														// For creating a temporary object inside 'pop' which will point to the last row/column
		Matrix(const int&, const int&, const copyMat&);					// For all temporary objects created inside member functions with a template matrix using linked list
		void templateList(const int&, const double*);					// Creates a template matrix using linked list with either 0 or array values depending on parameters

		friend std::istream& operator>>(std::istream&, const Matrix&);	// Friend declaration for overloaded istream operator
		friend std::ostream& operator<<(std::ostream&, const Matrix&);	// Friend declaration for overloaded ostream operator

		// Functions and constructors accessible by user

	public:

		Matrix(const int&, const int&);									// To create a template matrix and entering values later using 'cin'
		Matrix(const int&, const int&, const double*);					// To create a matrix with values taken from a passed in 2D array (can be 1D as well)
		Matrix(const Matrix&, const copyMat& val = copyMat::copyDef);	// Copy constructor to increase matNum if no string is passed, as it is done from main()
		Matrix(Matrix&&) noexcept;										// Move constructor to avoid copying of temporary function return values
		Node* begin();													// Returns a pointer to the first element pointed to by 'head'
		Node* end();													// Returns a pointer to the last element pointed to by 'tail'
		static int matNumber();											// Returns the number of the matrices in the current workspace
		void operator=(const Matrix&);									// Overloaded assignment operator
		void operator=(Matrix&&) noexcept;								// Overloaded move assignment operator
		Matrix operator+(const Matrix&);								// Matrix addition
		Matrix operator-(const Matrix&);								// Matrix subtraction
		Matrix operator*(const Matrix&);								// Matrix multiplication
		Matrix operator^(const int&);									// Matrix power
		Matrix operator()(const int&);									// Matrix inverse, if -1 is passed to the functor
		inline double& operator[](Node*&) const;						// For sequential access
		inline double& operator()(const int&, const int&) const;		// Matrix functor to extract and change values inside the matrix from main()
		Matrix operator()(const int&, const int&, const matChar& mc);	// Slicing function using functor
		void insert(Matrix&, const matChar& mc, const int&);			// Insert another matrix either row-wise or column-wise
		double det();													// Outputs the determinant
		Matrix transpose();												// Returns the transpose matrix
		Matrix sum(const matChar& mc);									// Row-wise/column-wise sum
		Matrix max(const matChar& mc);									// Row-wise/column-wise max
		Matrix min(const matChar& mc);									// Row-wise/column-wise min
		void del(const int&, const matChar& mc);						// For deleting a row/column
		void matDel();													// For deleting dynamic Node elements and delete the entire matrix
		void push(Matrix&, const matChar& mc);							// For pushing another matrix at the back
		Matrix pop(const matChar& mc);									// For popping the last row/column out
		~Matrix();														// For deleting the dynamic Node elements when the Matrix object goes out of scope
	};
}

#endif	/* MATRIX_H */