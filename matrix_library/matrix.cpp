#include <iomanip>
#include <vector>
#include "matrix.h"
using namespace std;

namespace err
{
	void diffSizes()
	{
		cout << "Matrix sizes do not match. Terminating..." << endl;
		_Exit(1);
	}

	void badSlice()
	{
		cout << "Right limit is not greater than the left limit. Terminating..." << endl;
		_Exit(1);
	}

	void badIndex()
	{
		cout << "Index out of bounds. Terminating..." << endl;
		_Exit(1);
	}

	void invParam()
	{
		cout << "Invalid parameter. Terminating..." << endl;
		_Exit(1);
	}

	void sqMatrix()
	{
		cout << "Matrix is not square. Terminating..." << endl;
		_Exit(1);
	}

	void multMismatch()
	{
		cout << "First matrix column and second matrix row are not equal. Terminating..." << endl;
		_Exit(1);
	}
}

namespace mat
{
	// Node

	Node::Node() :
		data(0.0),
		down(NULL),
		right(NULL)
	{
		// No-op
	}

	void Node::operator delete(void* p)
	{
		delete p;
	}


	// Matrix

	int Matrix::numOfMat = 0;
	int Matrix::matNum = 0;

	Matrix::Matrix() :
		row(0),
		column(0),
		num(0),
		head(NULL),
		tail(NULL)
	{
		// No-op
	}

	// Parametrized constructor taking number of rows and columns as parameters

	Matrix::Matrix(const int& n1, const int& n2) :
		row(n1),
		column(n2),
		num(0),
		head(NULL),
		tail(NULL)
	{
		numOfMat++;
		matNum++;
		num = matNum;

		templateList(0, NULL);
	}

	// Parametrized constructor taking number of rows and columns and external array to populate as parameters

	Matrix::Matrix(const int& n1, const int& n2, const double* arr) :
		row(n1),
		column(n2),
		num(0),
		head(NULL),
		tail(NULL)
	{
		numOfMat++;
		matNum++;
		num = matNum;

		templateList(1, arr);
	}

	// Parametrized constructor for temporary objects
	// The string copyMat::tempObject will be passed as parameter from inside memeber function
	// Difference lies in not incrementing numOfMat for temporary objects

	Matrix::Matrix(const int& n1, const int& n2, const copyMat& val) :
		row(n1),
		column(n2),
		num(0),
		head(NULL),
		tail(NULL)
	{
		templateList(0, NULL);
	}

	// Copy constructor
	// If string copyMat::tempObject is passed, numOfMat is not increased

	Matrix::Matrix(const Matrix& obj, const copyMat& val) :
		row(obj.row),
		column(obj.column),
		num(0),
		head(NULL),
		tail(NULL)
	{
		if (val == copyDef)
		{
			numOfMat++;
			matNum++;
			num = matNum;
		}

		Node* temp1 = NULL;
		Node* temp2 = NULL;
		Node* rhs = obj.head;

		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < column; j++)
			{
				Node* newNode = new Node;
				newNode->data = rhs->data;
				newNode->right = NULL;
				newNode->down = NULL;

				if (!head)
					head = newNode;

				else
					tail->right = newNode;

				tail = newNode;

				rhs = rhs->right;

				if (i)
				{
					temp1->down = newNode;
					temp1 = temp1->right;
				}
			}

			if (!i)
				temp1 = head;
		}

		if (head)
			temp2 = head->right;

		for (int i = 1; i < column; i++)
		{
			temp1->down = temp2;
			temp1 = temp1->right;
			temp2 = temp2->right;
		}
	}

	// Move constructor to avoid duplicating the nodes of the matrix from the temporary function return value
	// Unlike copying, the head and tail pointers are set/reset to the rvalue matrix

	Matrix::Matrix(Matrix&& obj) noexcept :
		row(obj.row),
		column(obj.column),
		head(obj.head),
		tail(obj.head),
		num(0)
	{
		matNum++;
		numOfMat++;
		num = matNum;

		obj.row = 0;
		obj.column = 0;
		obj.head = NULL;
		obj.tail = NULL;
		obj.num = 0;
	}

	// Destructor to delete dynamically created objects
	// This destructor will call the matDel() member function (see description for matDel())
	// numOfMat is decreased only when regular objects go out of scope (read about RVO)

	Matrix::~Matrix()
	{
		this->matDel();

		if (this->num)
			numOfMat--;
	}

	// Function to create a template matrix using linked lists 
	// If parameter trig is 0 and arr is NULL, matrix is populated with 0
	// If trig is 1 and an array is passed, matrix is populated with the array values
	// Last element in each row is connected to the first element in the next row
	// Last element in each column is connected to the first element in the next column

	void Matrix::templateList(const int& trig, const double* arr)
	{
		Node* temp1 = NULL;
		Node* temp2 = NULL;

		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < column; j++)
			{
				Node* newNode = new Node;
				newNode->data = (trig == 1 ? *(arr++) : 0);
				newNode->right = NULL;
				newNode->down = NULL;

				if (!head)
					head = newNode;

				else
					tail->right = newNode;

				tail = newNode;

				if (i)
				{
					temp1->down = newNode;
					temp1 = temp1->right;
				}
			}

			if (!i)
				temp1 = head;
		}

		if (head)
			temp2 = head->right;

		for (int i = 1; i < column; i++)
		{
			temp1->down = temp2;
			temp1 = temp1->right;
			temp2 = temp2->right;
		}
	}

	// Function to return a pointer to the first element of the Matrix

	Node* Matrix::begin()
	{
		return this->head;
	}

	// Function to return a pointer to the last element of the Matrix

	Node* Matrix::end()
	{
		return this->tail;
	}

	// Function to return the total number of matrices in the current workspace

	int Matrix::matNumber()
	{
		return numOfMat;
	}

	// Overloaded assignment operator
	// Takes care of deleting the dynamically allocated Node objects before assigning a new matrix

	void Matrix::operator=(const Matrix& obj)
	{
		this->matDel();

		this->row = obj.row;
		this->column = obj.column;

		this->templateList(0, NULL);

		Node* temp1 = this->head;
		Node* temp2 = obj.head;

		while (temp2)
		{
			temp1->data = temp2->data;
			temp2 = temp2->right;
			temp1 = temp1->right;
		}
	}

	// Move assignment operator to move matrices from temporary function return values

	void Matrix::operator=(Matrix&& obj) noexcept
	{
		this->matDel();

		this->head = obj.head;
		this->tail = obj.tail;

		obj.row = 0;
		obj.column = 0;
		obj.head = NULL;
		obj.tail = NULL;
		obj.num = 0;
	}

	// + operator overloaded to perform matrix addition directly

	Matrix Matrix::operator+(const Matrix& obj)
	{
		if ((this->row != obj.row) && (this->column != obj.column))
			err::diffSizes();

		Matrix tempObj(this->row, this->column, copyMat::tempObject);
		Node* lhs = this->head;
		Node* rhs = obj.head;
		Node* result = tempObj.head;

		while (lhs)
		{
			result->data = lhs->data + rhs->data;
			lhs = lhs->right;
			rhs = rhs->right;
			result = result->right;
		}

		return tempObj;
	}

	// - operator overloaded to perform matrix subtraction directly

	Matrix Matrix::operator-(const Matrix& obj)
	{
		if ((this->row != obj.row) && (this->column != obj.column))
			err::diffSizes();

		Matrix tempObj(this->row, this->column, copyMat::tempObject);
		Node* lhs = this->head;
		Node* rhs = obj.head;
		Node* result = tempObj.head;

		while (lhs)
		{
			result->data = lhs->data - rhs->data;
			lhs = lhs->right;
			rhs = rhs->right;
			result = result->right;
		}

		return tempObj;
	}

	// * operator overloaded to perform matrix multiplication directly

	Matrix Matrix::operator*(const Matrix& obj)
	{
		if (this->column != obj.row)
			err::multMismatch();

		Matrix tempObj(this->row, obj.column, copyMat::tempObject);
		Node* lhs = this->head;
		Node* rhs = obj.head;
		Node* result = tempObj.head;

		Node* temp1;
		Node* temp2;

		double tot;

		for (int i = 0; i < this->row; i++)
		{
			for (int j = 0; j < this->column; j++)
			{
				temp1 = lhs;
				temp2 = rhs;

				tot = 0;

				for (int k = 0; k < this->row; k++)
				{
					tot += temp1->data * temp2->data;
					temp1 = temp1->right;
					temp2 = temp2->down;
				}

				result->data = tot;
				result = result->right;

				rhs = rhs->right;
			}

			rhs = obj.head;
			lhs = lhs->down;
		}

		return tempObj;
	}

	// ^ operator overloaded to compute matrix power directly

	Matrix Matrix::operator^(const int& n)
	{
		if (this->row != this->column)
			err::sqMatrix();

		Matrix tempObj(*this, copyMat::tempObject);
		for (int i = 1; i < n; i++)
			tempObj = tempObj * (*this);

		return tempObj;
	}

	// Functor to compute inverse of a matrix is parameter passed is -1

	Matrix Matrix::operator()(const int& n)
	{
		{
			if (n != -1)
				err::invParam();
		}

		Matrix tempObj1(*this, copyMat::tempObject);
		Matrix tempObj2(this->row, this->column, copyMat::tempObject);

		int index = 0;
		double max, temp;

		const int row = this->row;
		const int column = this->column;

		vector<double> eye(row, 0);

		for (int i = 0; i < row; i++)
			eye[i] = i;

		for (int i = 1; i < row; i++)
		{
			max = abs(tempObj1(i - 1, i - 1));
			for (int k = i; k < row; k++)
			{
				if (abs(tempObj1(k, i - 1)) > max)
				{
					max = abs(tempObj1(k, i - 1));
					index = k;
				}
			}

			if (max != tempObj1(i - 1, i - 1))
			{
				temp = eye[i - 1];
				eye[i - 1] = eye[index];
				eye[index] = temp;

				for (int k = 0; k < row; k++)
				{
					temp = tempObj1(i - 1, k);
					tempObj1(i - 1, k) = tempObj1(index, k);
					tempObj1(index, k) = temp;
				}
			}

			for (int k = i; k < row; k++)
			{
				tempObj1(k, i - 1) /= tempObj1(i - 1, i - 1);
				for (int j = i; j < row; j++)
					tempObj1(k, j) -= tempObj1(k, i - 1) * tempObj1(i - 1, j);
			}
		}

		vector<double> sol(row, 0);
		double sum;

		for (int k = 0; k < this->column; k++)
		{
			for (int i = 0; i < this->row; i++)
			{
				sum = 0;
				for (int j = 0; j < i; j++)
					sum += tempObj1(i, j) * sol[j];

				sol[i] = (k == eye[i] ? 1 : 0) - sum;
			}

			for (int i = this->row - 1; i >= 0; i--)
			{
				sum = 0;
				for (int j = this->row - 1; j > i; j--)
					sum += tempObj1(i, j) * tempObj2(j, k);

				tempObj2(i, k) = (sol[i] - sum) / tempObj1(i, i);
			}
		}

		return tempObj2;
	}

	// Function to insert another matrix into the existing matrix
	// enum variable can be r or c representing row/column-wise insertion
	// 'n' represents the index where the matrix is to be inserted
	// The inserted matrix is reset as it shouldn't exist after insertion
	// The head and tail of the inserted matrix point to NULL

	void Matrix::insert(Matrix& obj, const matChar& mc, const int& n)
	{
		{
			if ((n < 0) || (n > this->row - 1) || (n > this->column - 1))
				err::badIndex();

			if ((mc == matChar::r) && (this->column != this->column))
				err::diffSizes();

			if ((mc == matChar::c) && (this->row != this->row))
				err::diffSizes();
		}

		if (mc == matChar::r)
		{
			Node* temp1 = this->head;

			for (int i = 0; i < n; i++)
				temp1 = temp1->down;

			Node* temp2 = obj.head;

			for (int i = 1; i < obj.row; i++)
				temp2 = temp2->down;

			obj.tail->right = temp1;

			for (int i = 0; i < this->column; i++)
			{
				temp2->down = temp1;
				temp2 = temp2->right;
				temp1 = temp1->right;
			}

			this->row += obj.row;

			if (!n)
			{
				this->head = obj.head;

				Node* temp3 = this->head;
				Node* temp4 = this->head->right;

				for (int i = 1; i < this->row; i++)
					temp3 = temp3->down;

				for (int i = 1; i < this->column; i++)
				{
					temp3->down = temp4;
					temp3 = temp3->right;
					temp4 = temp4->right;
				}
			}

			else
			{
				Node* temp3 = this->head;

				for (int i = 0; i < n - 1; i++)
					temp3 = temp3->down;

				Node* temp4 = obj.head;

				for (int i = 0; i < this->column - 1; i++)
				{
					temp3->down = temp4;
					temp3 = temp3->right;
					temp4 = temp4->right;
				}

				temp3->down = temp4;
				temp3->right = obj.head;
			}
		}

		else if (mc == matChar::c)
		{
			Node* temp1 = this->head;

			for (int i = 0; i < n; i++)
				temp1 = temp1->right;

			Node* temp2 = obj.head;

			for (int i = 1; i < obj.column; i++)
				temp2 = temp2->right;

			obj.tail->down = temp1;

			for (int i = 0; i < this->row; i++)
			{
				temp2->right = temp1;
				temp2 = temp2->down;
				temp1 = temp1->down;
			}

			this->column += obj.column;

			if (!n)
			{
				this->head = obj.head;

				Node* temp3 = this->head;
				Node* temp4 = this->head->down;

				for (int i = 1; i < this->column; i++)
					temp3 = temp3->right;

				for (int i = 1; i < this->row; i++)
				{
					temp3->right = temp4;
					temp3 = temp3->down;
					temp4 = temp4->down;
				}
			}

			else
			{
				Node* temp3 = this->head;

				for (int i = 0; i < n - 1; i++)
					temp3 = temp3->right;

				Node* temp4 = obj.head;

				for (int i = 0; i < this->row - 1; i++)
				{
					temp3->right = temp4;
					temp3 = temp3->down;
					temp4 = temp4->down;
				}

				temp3->right = temp4;
				temp3->down = obj.head;
			}
		}

		//resetting the inserted Matrix

		numOfMat--;

		obj.head = NULL;
		obj.tail = NULL;
		obj.num = 0;
		obj.row = 0;
		obj.column = 0;
	}

	// Function to return the determinant of a square matrix

	double Matrix::det()
	{
		{
			if (this->row != this->column)
				err::sqMatrix();
		}

		double sum = 0;
		int col = 0;

		for (int k = 0; k < column; k++)
		{
			Node* temp1 = this->head;

			Matrix tempObj(row - 1, column - 1, copyMat::tempObject);
			Node* temp2 = tempObj.head;
			Node* temp3 = this->head->down;

			for (int i = 1; i < row; i++)
			{
				col = 0;
				for (int j = 0; j < column; j++)
				{
					if (j != k)
					{
						temp2->data = temp3->data;
						temp2 = temp2->right;
					}

					temp3 = temp3->right;
				}
			}

			if (row > 1)
			{
				if (!(k % 2))
					sum += temp1->data * tempObj.det();
				else
					sum -= temp1->data * tempObj.det();

				temp1 = temp1->right;
			}

			else
				sum = temp1->data;
		}

		return sum;
	}

	// Function return the transpose of the current matrix object

	Matrix Matrix::transpose()
	{
		Matrix tempObj(this->column, this->row, copyMat::tempObject);

		Node* temp1 = this->head;
		Node* temp2 = tempObj.head;

		while (temp1)
		{
			temp2->data = temp1->data;
			temp1 = temp1->down;
			temp2 = temp2->right;
		}

		return tempObj;
	}

	// Function to return the sum of rows/columns
	// Returns a 1D matrix/array with number of columns as elements for row-wise addition
	// Returns a 1D matrix/array with number of rows as elements for column-wise addition
	// enum variable is r for rows and c for columns

	Matrix Matrix::sum(const matChar& mc)
	{
		if (mc == matChar::r)
		{
			Matrix tempObj(1, this->column, copyMat::tempObject);

			Node* temp1 = this->head;
			Node* temp2 = tempObj.head;
			double tot;

			for (int j = 0; j < this->column; j++)
			{
				tot = 0;
				for (int i = 0; i < this->row; i++)
				{
					tot += temp1->data;
					temp1 = temp1->down;
				}

				temp2->data = tot;
				temp2 = temp2->right;
			}

			return tempObj;
		}

		else if (mc == matChar::c)
		{
			Matrix tempObj(this->row, 1, copyMat::tempObject);

			Node* temp1 = this->head;
			Node* temp2 = tempObj.head;
			double tot;

			for (int i = 0; i < this->row; i++)
			{
				tot = 0;
				for (int j = 0; j < this->column; j++)
				{
					tot += temp1->data;
					temp1 = temp1->right;
				}

				temp2->data = tot;
				temp2 = temp2->down;
			}

			return tempObj;
		}
	}

	// Function to return the maximum in each row/column
	// Operation is similar to 'sum' member function 

	Matrix Matrix::max(const matChar& mc)
	{
		if (mc == matChar::r)
		{
			Matrix tempObj(1, this->column, copyMat::tempObject);

			Node* temp1 = this->head;
			Node* temp2 = tempObj.head;
			double rmax;

			for (int j = 0; j < this->column; j++)
			{
				rmax = 0;
				for (int i = 0; i < this->row; i++)
				{
					if (temp1->data > rmax)
						rmax = temp1->data;

					temp1 = temp1->down;
				}

				temp2->data = rmax;
				temp2 = temp2->right;
			}

			return tempObj;
		}

		else if (mc == matChar::c)
		{
			Matrix tempObj(this->row, 1, copyMat::tempObject);

			Node* temp1 = this->head;
			Node* temp2 = tempObj.head;
			double cmax;

			for (int i = 0; i < this->row; i++)
			{
				cmax = 0;
				for (int j = 0; j < this->column; j++)
				{
					if (temp1->data > cmax)
						cmax = temp1->data;

					temp1 = temp1->right;
				}

				temp2->data = cmax;
				temp2 = temp2->down;
			}

			return tempObj;
		}
	}

	// Function to return the minimum in each row/column
	// Operation is similar to 'sum' member function 

	Matrix Matrix::min(const matChar& mc)
	{
		if (mc == matChar::r)
		{
			Matrix tempObj(1, this->column, copyMat::tempObject);

			Node* temp1 = this->head;
			Node* temp2 = tempObj.head;
			double rmin;

			for (int j = 0; j < this->column; j++)
			{
				rmin = temp1->data;
				for (int i = 0; i < this->row; i++)
				{
					if (temp1->data < rmin)
						rmin = temp1->data;

					temp1 = temp1->down;
				}

				temp2->data = rmin;
				temp2 = temp2->right;
			}

			return tempObj;
		}

		else if (mc == matChar::c)
		{
			Matrix tempObj(this->row, 1, copyMat::tempObject);

			Node* temp1 = this->head;
			Node* temp2 = tempObj.head;
			double cmin;

			for (int i = 0; i < this->row; i++)
			{
				cmin = temp1->data;
				for (int j = 0; j < this->column; j++)
				{
					if (temp1->data < cmin)
						cmin = temp1->data;

					temp1 = temp1->right;
				}

				temp2->data = cmin;
				temp2 = temp2->down;
			}

			return tempObj;
		}
	}

	// Function to delete a row/column
	// First parameter is the index of the row/column 

	void Matrix::del(const int& n, const matChar& mc)
	{
		{
			if ((n < 0) || (n > this->row - 1) || (n > this->column - 1))
				err::badIndex();
		}

		if (mc == matChar::r)
		{
			if (n)
			{
				Node* temp1 = this->head;
				Node* temp2;
				Node* temp3;

				for (int i = 1; i < n; i++)
					temp1 = temp1->down;

				temp2 = temp1->down;
				temp3 = temp2->right;

				for (int i = 0; i < this->column - 1; i++)
				{
					temp1->down = temp2->down;
					delete temp2;
					temp2 = temp3;
					temp3 = temp3->right;
					temp1 = temp1->right;
				}

				temp1->down = temp2->down;
				delete temp2;
				temp2 = temp3;
				temp1->right = temp2;

				if (n == this->row - 1)
					this->tail = temp1;

				this->row--;
			}

			else
			{
				Node* temp2 = this->head;
				Node* temp3 = temp2->right;

				this->head = temp2->down;

				for (int i = 0; i < this->column; i++)
				{
					delete temp2;
					temp2 = temp3;
					temp3 = temp3->right;
				}

				this->row--;

				temp3 = this->head;
				temp2 = temp3->right;

				for (int i = 1; i < this->row; i++)
					temp3 = temp3->down;

				for (int i = 0; i < this->column - 1; i++)
				{
					temp3->down = temp2;
					temp2 = temp2->right;
					temp3 = temp3->right;
				}
			}
		}

		else if (mc == matChar::c)
		{
			if (n)
			{
				Node* temp1 = this->head;
				Node* temp2;
				Node* temp3;

				for (int i = 1; i < n; i++)
					temp1 = temp1->right;

				temp2 = temp1->right;
				temp3 = temp2->down;

				for (int i = 0; i < this->row - 1; i++)
				{
					temp1->right = temp2->right;
					delete temp2;
					temp2 = temp3;
					temp3 = temp3->down;
					temp1 = temp1->down;
				}

				temp1->right = temp2->right;
				delete temp2;
				temp2 = temp3;
				temp1->down = temp2;

				if (n == this->column - 1)
					this->tail = temp1;

				this->column--;
			}

			else
			{
				Node* temp2 = this->head;
				Node* temp3 = temp2->down;

				this->head = temp2->right;

				for (int i = 0; i < this->row; i++)
				{
					delete temp2;
					temp2 = temp3;
					temp3 = temp3->down;
				}

				this->column--;

				temp3 = this->head;
				temp2 = temp3->down;

				for (int i = 1; i < this->column; i++)
					temp3 = temp3->right;

				for (int i = 0; i < this->row - 1; i++)
				{
					temp3->right = temp2;
					temp2 = temp2->down;
					temp3 = temp3->down;
				}
			}
		}
	}

	// Function to delete all the dynamic Node elements in the matrix
	// Destructor call invokes this function

	void Matrix::matDel()
	{
		Node* temp = head;

		while (temp)
		{
			temp = temp->right;
			delete head;
			head = temp;
		}

		tail = NULL;

		row = 0;
		column = 0;
	}

	// Function to push another matrix row/column-wise at the back
	// The pushed matrix is reset as it shouldn't exist after insertion
	// The head and tail of the inserted matrix point to NULL

	void Matrix::push(Matrix& obj, const matChar& mc)
	{
		{
			if (
				((mc == matChar::r) && (this->column != obj.column)) ||
				((mc == matChar::c) && (this->row != obj.row))
				)
				err::diffSizes();
		}

		if (mc == matChar::r)
		{
			Node* temp1 = this->head;

			for (int i = 0; i < this->row - 1; i++)
				temp1 = temp1->down;

			Node* temp2 = obj.head;

			for (int i = 0; i < this->column - 1; i++)
			{
				temp1->down = temp2;
				temp1 = temp1->right;
				temp2 = temp2->right;
			}

			temp1->down = temp2;
			temp1->right = obj.head;

			this->tail = obj.tail;
			this->row += obj.row;

			temp1 = this->head;
			temp2 = temp1->right;

			for (int i = 0; i < this->row - 1; i++)
				temp1 = temp1->down;

			for (int i = 0; i < this->column - 1; i++)
			{
				temp1->down = temp2;
				temp1 = temp1->right;
				temp2 = temp2->right;
			}

		}

		else if (mc == matChar::c)
		{
			Node* temp1 = this->head;

			for (int i = 0; i < this->column - 1; i++)
				temp1 = temp1->right;

			Node* temp2 = obj.head;

			for (int i = 0; i < this->row - 1; i++)
			{
				temp1->right = temp2;
				temp1 = temp1->down;
				temp2 = temp2->down;
			}

			temp1->right = temp2;
			temp1->down = obj.head;

			this->tail = obj.tail;
			this->column += obj.column;

			temp1 = this->head;
			temp2 = temp1->down;

			for (int i = 0; i < this->column - 1; i++)
				temp1 = temp1->right;

			for (int i = 0; i < this->row - 1; i++)
			{
				temp1->right = temp2;
				temp1 = temp1->down;
				temp2 = temp2->down;
			}
		}

		//resetting the inserted Matrix

		numOfMat--;

		obj.head = NULL;
		obj.tail = NULL;
		obj.num = 0;
		obj.row = 0;
		obj.column = 0;
	}

	// Function to pop the last row/column from the matrix
	// Returns the last row/column
	// Existing matrix is modified as it should not contain the last row/column
	// The returned matrix is directly made to point to the last row/column

	Matrix Matrix::pop(const matChar& mc)
	{
		if (mc == matChar::r)
		{
			Matrix tempObj;

			Node* temp1 = this->head;
			Node* temp2 = temp1->right;

			for (int i = 0; i < this->row - 2; i++)
				temp1 = temp1->down;

			tempObj.head = temp1->down;
			tempObj.tail = this->tail;

			for (int i = 0; i < this->column - 1; i++)
			{
				temp1->down->down = temp1->down->right;
				temp1->down = temp2;
				temp2 = temp2->right;
				temp1 = temp1->right;
			}

			temp1->right = NULL;
			temp1->down = NULL;

			this->tail = temp1;
			this->row--;

			tempObj.row = 1;
			tempObj.column = this->column;

			return tempObj;
		}

		else if (mc == matChar::c)
		{
			Matrix tempObj;

			Node* temp1 = this->head;
			Node* temp2 = temp1->down;

			for (int i = 0; i < this->column - 2; i++)
				temp1 = temp1->right;

			tempObj.head = temp1->right;
			tempObj.tail = this->tail;

			for (int i = 0; i < this->row - 1; i++)
			{
				temp1->right->right = temp1->right->down;
				temp1->right = temp2;
				temp2 = temp2->down;
				temp1 = temp1->down;
			}

			temp1->down = NULL;
			temp1->right = NULL;

			this->tail = temp1;
			this->column--;

			tempObj.row = this->row;
			tempObj.column = 1;

			return tempObj;
		}
	}

	// Function to return the slice of a matrix, row/column-wise
	// The first 2 parameters n1 and n2 represent the lower and upper indices of slicing
	// The returned matrix slice contains the rows/columns starting from index n1 to index n2-1

	Matrix Matrix::operator()(const int& n1, const int& n2, const matChar& mc)
	{
		{
			if (
				(n1 < 0) || (n2 < 0) ||
				(mc == matChar::c) && ((n1 > this->column) || (n2 > this->column)) ||
				(mc == matChar::r) && ((n1 > this->row) || (n2 > this->row))
				)
				err::badIndex();

			if (n1 > n2)
				err::badSlice();
		}

		if (mc == matChar::r)
		{
			Matrix tempObj(n2 - n1, this->column, copyMat::tempObject);

			Node* temp1 = this->head;
			Node* temp2 = tempObj.head;

			for (int i = 0; i < n1; i++)
				temp1 = temp1->down;

			for (int i = n1; i < n2; i++)
			{
				for (int j = 0; j < this->column; j++)
				{
					temp2->data = temp1->data;
					temp1 = temp1->right;
					temp2 = temp2->right;
				}
			}

			return tempObj;
		}

		else if (mc == matChar::c)
		{
			Matrix tempObj(this->row, n2 - n1, copyMat::tempObject);

			Node* temp1 = this->head;
			Node* temp2 = tempObj.head;

			for (int i = 0; i < n1; i++)
				temp1 = temp1->right;

			for (int j = n1; j < n2; j++)
			{
				for (int i = 0; i < this->column; i++)
				{
					temp2->data = temp1->data;
					temp1 = temp1->down;
					temp2 = temp2->down;
				}
			}

			return tempObj;
		}
	}

	// Functor to return the element at the passed in indices
	// Can be used to modify any element from main() as the function returns a reference to a private variable 

	inline double& Matrix::operator()(const int& n1, const int& n2) const
	{
		{
			if (
				(n1 < 0) || (n2 < 0) ||
				(n1 > this->row - 1) || (n2 > this->column - 1)
				)
				err::badIndex();
		}

		Node* temp = head;

		for (int i = 0; i < n1; i++)
			temp = temp->down;

		for (int i = 0; i < n2; i++)
			temp = temp->right;

		return temp->data;
	}

	// Functor for sequential access of elements in the matrix
	// Inlining of the function and sequential access reduces the time it takes to display value using a functor as the previous one 

	inline double& Matrix::operator[](Node*& seqAcc) const
	{
		double& val = seqAcc->data;
		seqAcc = seqAcc->right;

		return val;
	}

	// Overloaded istream operator
	// Friend of class Matrix; hence can access private variables such as 'num'

	istream& operator>>(istream& in, const Matrix& obj)
	{
		cout << "Enter the values for matrix " << obj.num << endl;

		Node* seqAcc = obj.head;

		while (seqAcc)
			in >> obj[seqAcc];

		return in;
	}

	// Overloaded ostream operator
	// Friend of class Matrix; hence can access private variables such as 'num', 'row', 'column'

	ostream& operator<<(ostream& out, const Matrix& obj)
	{
		cout << "Matrix " << obj.num << endl;

		Node* seqAcc = obj.head;

		for (int i = 0; i < obj.row; i++)
		{
			for (int j = 0; j < obj.column; j++)
				out << setw(5) << obj[seqAcc];

			out << endl;
		}

		return out;
	}
}