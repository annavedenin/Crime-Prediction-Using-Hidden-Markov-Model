/*********************************************************************************************/
// Name: viterbi.h
// 
// implimentation of the clasical viterbi algorythem.
//
/*********************************************************************************************/

#pragma once

#include"hmm.h"
#include"seq.h"
#include"matrix.h"

//! Calculate the most plosible sequance of states that creates the emission sequance.
/*
\ h HMM - Hiden Markov Model object.
\ s Seq - Observasion sequance of emmisions.
\ returns int*- most plosible sequance of states.
*/
int* viterbi(HMM& h, Seq& s)
{
	int T = s.T;
	int states = h.N;
	int minState;
	double minWeight;
	double weight;

	int **psi = matrix<int>(0, h.N, s.T);
	long double **delta = matrix<long double>(0, h.N, s.T);
	
	// Base
	for (int i = 0; i < states; i++)
		delta[i][0] = ((-1.0 * log10(h.pi[i])) - log10(h.B[i][s.O[0]]));

	// Induction
	for (int t = 1; t < T; t++)
	{
		for (int j = 0; j < states; j++)
		{
			minState = 0;
			minWeight = delta[0][t - 1] - log10(h.A[0][j]);

			for (int i = 1; i < states; i++)
			{
				weight = delta[i][t - 1] - log10(h.A[i][j]);

				if (weight < minWeight)
				{
					minState = i;
					minWeight = weight;
				}
			}

			delta[j][t] = minWeight - log10(h.B[j][s.O[t]]);
			psi[j][t] = minState;
		}
	}

	// Find minimum value for time T-1
	minState = 0;
	minWeight = delta[0][T - 1];

	for (int i = 1; i < states; i++)
	{
		if (delta[i][T - 1] < minWeight)
		{
			minState = i;
			minWeight = delta[i][T - 1];
		}
	}

	// Trackback
	int* path = new int[T];
	path[T - 1] = minState;

	for (int t = T - 2; t >= 0; t--)
		path[t] = psi[path[t + 1]][t + 1];


	freeMatrix<long double>(delta, h.N);
	freeMatrix<int>(psi, h.N);
	return path;
}