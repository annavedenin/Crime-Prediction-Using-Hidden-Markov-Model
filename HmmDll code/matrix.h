/*********************************************************************************************/
// Name: matrix.h
// 
// initialzing matrix structor of diffrent size.
/*********************************************************************************************/
#pragma once

//! create matrix of size [n][m]
/*
\ u - initialization value of type T
*/
template<typename T>
T** matrix(T u, int n, int m)
{
	T **mt = new T*[n];
	for (int i = 0; i < n; ++i)
	{
		mt[i] = new T[m];
		for (int j = 0; j < m; ++j)
		{
			mt[i][j] = u;
		}
	}
	return mt;
}

//! create matrix of size [n][m][k]
/*
\ u - initialization value of type T
*/
template<typename T>
T*** matrix(T u, int n, int m, int k)
{
	T ***mt = new T**[n];
	for (int i = 0; i < n; i++)
	{
		mt[i] = new T*[m];
		for (int j = 0; j < m; j++)
		{
			mt[i][j] = new T[k];
			for (int t = 0; t < k; t++)
			{
				mt[i][j][t] = u;
			}
		}
	}
	return mt;
}

//! free matrix of size [n][m][k]
/*
\ mt - matrix with values of type T
*/
template<typename T>
void freeMatrix(T*** mt, int n, int m)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; j++)
		{
			delete[] mt[i][j];
		}
		delete[] mt[i];
	}
	delete[] mt;
}

//! free matrix of size [n][m]
/*
\ mt - matrix with values of type T
*/
template<typename T>
void freeMatrix(T** mt, int n)
{
	for (int i = 0; i < n; ++i)
	{
		delete[] mt[i];
	}
	delete[] mt;
}