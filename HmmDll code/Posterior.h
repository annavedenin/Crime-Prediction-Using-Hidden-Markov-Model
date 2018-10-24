/*********************************************************************************************/
// Name: posterior.h
// 
// its calculating the conditional probabilities of being at 
// state k at step i, given the observed sequence of symbols
/*********************************************************************************************/
#pragma once

#include"hmm.h"
#include"seq.h"
#include"matrix.h"
//! calculates the postirior probabilities for each observation
/*
\ h HMM - Hidden Markov Model object
\ observations vector<Seq> - vector of sequances 
\ state integer - the state for wich we calculate the probabilities
*/
vector<double>  posterior(HMM &h, vector<Seq> observations, int state)
{
	int N = observations.size();
	vector<double> post;

	// Initialization
	vector<matrix_2D> gamma;

	for (int i = 0; i < N; i++)
	{
		int T = observations[i].T;
		gamma.push_back(matrix<double>(0, T, h.N));
	}

	for (int i = 0; i < N; i++)
	{
		Seq sequence = observations[i];
		int T = sequence.T;
		vector<double> scaling;

		double** fwd = forward(h, observations[i], scaling);
		double** bwd = backward(h, observations[i]); 

		//terminasion
		double p = 0;
		for (int i = 0; i < h.N; i++)
			p += fwd[T - 1][i];

		for (int t = 0; t < T; t++)
		{

			for (int k = 0; k < h.N; k++)
				gamma[i][t][k] = fwd[t][k] * bwd[t][k];
			if (p != 0) 
			{
				for (int k = 0; k < h.N; k++)
					gamma[i][t][k] /= p;
			}
		}

		//before calculating next seq push current postireor pobability
		for (int t = 0; t < T; t++)
			post.emplace_back(gamma[i][t][state]);
	}

	for (int i = 0; i < N; i++)
	{
		int T = observations[i].T;
		freeMatrix(gamma[i], T);
	}

	return post;
}