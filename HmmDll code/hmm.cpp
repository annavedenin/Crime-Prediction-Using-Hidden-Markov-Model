/*********************************************************************************************/
// Name: hmm.cpp
// 
// class of the Hidden Markov Model structur
/*********************************************************************************************/
#include "stdafx.h"
#include"hmm.h"
#include"matrix.h"
#include<string>
#include<iostream>
#include<random>

using std::string;
using std::cout;
using std::endl;
using std::random_device;
using std::default_random_engine;
using std::uniform_int_distribution;

//! A constructor.
/*
\input f - filse stream of initial model.
\loading parameters from input file.
*/
HMM::HMM(std::fstream &f)
{
	string skip;
	f >> skip >> M;	// skip = "M= "

	f >> skip >> N;	// skip = "N= "

	f >> skip;	// skip = "A: "
	A = matrix<long double>(0, N, N);
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			f >> A[i][j];
		}
	}
	
	f >> skip;	// skip = "B: "
	B = matrix<long double>(0, N, M);
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			f >> B[i][j];
		}
	}

	f >> skip;	// skip = "pi: "
	pi = new long double[N];
	for (int i = 0; i < N; ++i)
	{
		f >> pi[i];
	}
}
//! A constructor.
/*
\n integer - number of emisions.
\m integer - number of states.
\ creates random model.
*/
HMM::HMM(int n, int m)
{
	N = n;
	M = m;

	random_device rd;
	default_random_engine e(rd());
	uniform_int_distribution<long long> u(0, 1);
	// creates trasnmision matrix
	A = matrix<long double>(0, N, N);
	for (int i = 0; i < N; ++i)
	{
		long double sum = 0;
		for (int j = 0; j < N; ++j)
		{
			A[i][j] = (long double)u(e);
			sum += A[i][j];
		}
		for (int j = 0; j < N; ++j)
		{
			A[i][j] /= sum;
		}
	}
	// creates emission matrix
	B = matrix<long double>(0, N, M);
	for (int i = 0; i < N; ++i)
	{
		double sum = 0;
		for (int j = 0; j < M; ++j)
		{
			B[i][j] = u(e);
			sum += B[i][j];
		}
		for (int j = 0; j < M; ++j)
		{
			B[i][j] /= sum;
		}
	}
	// creates state probability vector
	pi = new long double[N];
	for (int i = 0; i < N; ++i)
	{
		pi[i] = u(e);
	}
}
//! A constructor.
/*
\n delete created matrixes.
*/
HMM::~HMM()
{
	freeMatrix<long double>(A, N);
	freeMatrix<long double>(B, N);
	delete[] pi;
}

//! print model to screen
void HMM::print()
{
	cout << "M= " << M << endl;
	cout << "N= " << N << endl;
	cout << "A: " << endl;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			cout << A[i][j] << " ";
		}
		cout << endl;
	}

	cout << "B: " << endl;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			cout << B[i][j] << " ";
		}
		cout << endl;
	}

	cout << "pi: " << endl;
	for (int i = 0; i < N; ++i)
	{
		cout << pi[i] << " ";
	}
	cout << endl;
}
//! print model to file
/*
/ file ofstream - destination file
*/
void HMM::print(std::ofstream &file)
{
	file << "M= " << M << endl;
	file << "N= " << N << endl;
	file << "A: " << endl;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			file << A[i][j] << " ";
		}
		file << endl;
	}

	file << "B: " << endl;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			file << B[i][j] << " ";
		}
		file << endl;
	}

	file << "pi: " << endl;
	for (int i = 0; i < N; ++i)
	{
		file << pi[i] << " ";
	}
	file << endl;
}

//! return prediction based on hmm matrix information
double HMM::givePrediction()
{
	return pi[0] * A[0][0] * B[0][1] + 
		pi[0] * A[0][1] * B[1][1] + 
		pi[1] * A[1][1]* B[1][1] + 
		pi[1] * A[1][0] * B[1][1];
}

