/*********************************************************************************************/
// Name: hmm.h
// 
// class of the Hidden Markov Model structur
//
/*********************************************************************************************/
#pragma once
#include <iostream>
#include <fstream>
#include<vector>
#include"seq.h"

using std::fstream;
typedef double** matrix_2D;
typedef double*** matrix_3D;

class HMM
{
private:
	int N;	// number of states; Q={1,2,...,N}
	int M;	// number of observation symbols; V={1,2,...,M}
	long double **A;	/* A[1..N][1..N]. a[i][j] is the transition prob
						of going from state i at time t to state j
						at time t+1 */
	long double **B;	/* B[1..N][1..M]. b[j][k] is the probability of
						of observing symbol k in state j */
	long double *pi;	/* pi[1..N] pi[i] is the initial state distribution. */

public:
	HMM(std::fstream&);	// initialize the hmm model from a file
	HMM(int, int);	// initialize a random hmm model by specifying N and M
	~HMM();
	void print();
	void print(std::ofstream&);
	double givePrediction();
	friend double** forward(HMM&, Seq&, vector<double>&);
	friend double** backward(HMM&, Seq&);
	friend int* viterbi(HMM&, Seq&);
	friend double baumWelch(HMM&, vector<Seq>, int, double);
	friend vector<double>  posterior(HMM&, vector<Seq>, int);
};