/*********************************************************************************************/
// Name: backward.h
// 
// implimentation of the clasical backward algorythem.
//
/*********************************************************************************************/
#pragma once

#include"hmm.h"
#include"seq.h"
#include"matrix.h"

/*! Calculates the state backward probaility for each emiision in the sequance
\ h HMM - hiden markov model
\ s Seq - the emission sequance
\returns a [sequance size][2] matrix of state probability for each emission. 
*/
double** backward(HMM &h, Seq &s)
{
	double **bwd = matrix<double>(0, s.T, h.N);

	/* 1. Initialization */
	for (int i = 0; i < h.N; i++)
		bwd[s.T - 1][i] = 1.0;

	/* 2. Induction */
	for (int t = s.T - 2; t >= 0; t--)
	{
		for (int i = 0; i < h.N; i++)
		{
			double sum = 0;
			for (int j = 0; j < h.N; j++)
				sum += h.A[i][j] * h.B[j][s.O[t + 1]] * bwd[t + 1][j];

			bwd[t][i] = sum;
		}
	}

	return bwd;
}