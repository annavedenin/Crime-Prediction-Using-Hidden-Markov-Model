/*********************************************************************************************/
// Name: baumWelch.cpp
// 
// implimentation of the clasical BaumWelch algorythem.
/*********************************************************************************************/
#include "stdafx.h"
#include"baumwelch.h"
#include"matrix.h"
#undef min

//! Prerforming unlabeld learning of the model by given observasions
/*
\h HMM - hmm object.
\observations  vector<Seq>- vector of learning sequances.
\iterations integer - maximum number of iteration in algorythem.
\tolerance double - stoping value for log likelihood diffrence.
\
*/
double baumWelch(HMM &h, vector<Seq> observations, int iterations, double tolerance)
{
	if (iterations == 0 && tolerance == 0)
		std::cerr << "Iterations and limit cannot be both zero.";

	int N = observations.size();
	int currentIteration = 1;
	bool stop = false;

	// Initialization
	vector<matrix_3D> epsilon; // also referred as ksi or psi
	vector<matrix_2D> gamma;

	for (int i = 0; i < N; i++)
	{
		int T = observations[i].T;
		epsilon.push_back(matrix<double>(0, T, h.N, h.N));

		gamma.push_back(matrix<double>(0, T, h.N));
	}


	// Calculate initial model log-likelihood
	double oldLikelihood = std::numeric_limits<double>::min();
	double newLikelihood = 0;

	do // Until convergence or max iterations is reached
	{
		// For each sequence in the observations input
		for (int i = 0; i < N; i++)
		{
			Seq sequence = observations[i];
			int T = sequence.T;
			vector<double> scaling;

			// 1st step - Calculating the forward probability and the
			//            backward probability for each HMM state.
			
			double** fwd = forward(h, observations[i], scaling);
			double** bwd = backward(h, observations[i]);

			// 2nd step - Determining the frequency of the transition-emission pair values
			//            and dividing it by the probability of the entire string.


			// Calculate gamma values for next computations
			for (int t = 0; t < T; t++)
			{
				double s = 0;

				for (int k = 0; k < h.N; k++)
					s += gamma[i][t][k] = fwd[t][k] * bwd[t][k];

				if (s != 0) // Scaling
				{
					for (int k = 0; k < h.N; k++)
						gamma[i][t][k] /= s;
				}
			}

			// Calculate epsilon values for next computations
			for (int t = 0; t < T - 1; t++)
			{
				double s = 0;

				for (int k = 0; k < h.N; k++)
					for (int l = 0; l < h.N; l++)
						s += epsilon[i][t][k][l] = fwd[t][k] * h.A[k][l] *
						bwd[t + 1][l] * h.B[l][sequence.O[t + 1]];

				if (s != 0) // Scaling
				{
					for (int k = 0; k < h.N; k++)
						for (int l = 0; l < h.N; l++)
							epsilon[i][t][k][l] /= s;
				}
			}

			// Compute log-likelihood for the given sequence
			for (int t = 0; t < scaling.size() ; t++)
				newLikelihood += log10(scaling[t]);

			freeMatrix(fwd, observations[i].T);
			freeMatrix(bwd, observations[i].T);

		}

		// Average the likelihood for all sequences
		newLikelihood /= observations.size();


		// Check if the model has converged or we should stop
		if (checkConvergence(oldLikelihood, newLikelihood, currentIteration, iterations, tolerance))
			stop = true;

		else
		{
			// 3. Continue with parameter re-estimation
			currentIteration++;
			oldLikelihood = newLikelihood;
			newLikelihood = 0.0;

			// 3.1 Re-estimation of initial state probabilities
			for (int k = 0; k < h.N; k++)
			{
				double sum = 0;
				for (int i = 0; i < N; i++)
					sum += gamma[i][0][k];
				h.pi[k] = sum / N;
			}

			// 3.2 Re-estimation of transition probabilities
			for (int i = 0; i < h.N; i++)
			{
				for (int j = 0; j < h.N; j++)
				{
					double den = 0, num = 0;

					for (int k = 0; k < N; k++)
					{
						int T = observations[k].T;

						for (int l = 0; l < T - 1; l++)
							num += epsilon[k][l][i][j];

						for (int l = 0; l < T - 1; l++)
							den += gamma[k][l][i];
					}

					h.A[i][j] = (den != 0) ? num / den : 0.0;
				}
			}

			// 3.3 Re-estimation of emission probabilities
			for (int i = 0; i < h.N; i++)
			{
				for (int j = 0; j < h.M; j++)
				{
					double den = 0, num = 0;

					for (int k = 0; k < N; k++)
					{
						int T = observations[k].T;

						for (int l = 0; l < T; l++)
						{
							if (observations[k].O[l] == j)
								num += gamma[k][l][i];
						}

						for (int l = 0; l < T; l++)
							den += gamma[k][l][i];
					}

					// avoid locking a parameter in zero.
					h.B[i][j] = (num == 0) ? 1e-10 : num / den;
				}
			}

		}

	} while (!stop);

	for (int i = 0; i < N; i++)
	{
		int T = observations[i].T;
		freeMatrix(epsilon[i], T, h.N);
		freeMatrix(gamma[i], T);
	}
	return 0.0;
}
//Check if the model has converged or we should stop
/*
\oldLikelihood double - likelihood from previus iteration.
\newLikelihood  double- likelihood from current iteration.
\iterations integer - maximum number of iteration in algorythem.
\tolerance double - stoping value for log likelihood diffrence.
\returns true if max number of iterations was reached or the diffrance in likelihood is smaler then tolerance
*/
bool checkConvergence(double oldLikelihood,double newLikelihood, int currentIteration, int iterations, double tolerance) 
{
	return (fabs(oldLikelihood-newLikelihood)<tolerance || currentIteration == iterations) ? true : false;
}