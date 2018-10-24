/*********************************************************************************************/
// Name: baumWelch.h
// 
// header file of implimentation of the clasical BaumWelch algorythem.
/*********************************************************************************************/
#pragma once
#include<vector>
#include"hmm.h"
#include"seq.h"

bool checkConvergence(double , double , int , int , double );
double baumWelch(HMM&, vector<Seq>, int, double);
