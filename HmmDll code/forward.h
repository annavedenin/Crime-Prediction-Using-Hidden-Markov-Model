/*********************************************************************************************/
// Name: forward.h
// 
// implimentation of the clasical forward algorythem.
//
/*********************************************************************************************/
#pragma once

#include"hmm.h"
#include"seq.h"
#include"matrix.h"

/*! Calculates the state forward probaility for each emiision in the sequance
\ h HMM - hiden markov model
\ s Seq - the emission sequance
\ coefficients vector<double>& - scaling vector for log likelihood
\returns a [sequance size][2] matrix of state probability for each emission.
*/
double** forward(HMM& h, Seq& s, vector<double>& coefficients)
{
	double **fwd = matrix<double>(0, s.T, h.N);

	/* 1. Initialization */
	for (int i = 0; i < s.T; i++)
		coefficients.push_back(0.0);

	for (int i = 0; i < h.N; i++)
		coefficients[0] += fwd[0][i] = h.pi[i] * h.B[i][s.O[0]];

	/* 2. Induction */
	for (int t = 1; t < s.T; t++)
	{
		for (int i = 0; i < h.N; i++)
		{
			double sum = 0.0;
			for (int j = 0; j < h.N; j++)
				sum += fwd[t - 1][j] * h.A[j][i];

			fwd[t][i] = sum * h.B[i][s.O[t]];

			coefficients[t] += fwd[t][i]; // scaling coefficient
		}

	}

	return fwd;
}