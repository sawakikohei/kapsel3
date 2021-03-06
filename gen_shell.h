/*!
  \file gen_shell.h
  \author John J. Molina
  \date 2014/08/18
  \version 1.0
  \brief Auxiliary routines to compute concentric shells of periodic images. Used for direct calculation of long-rage
  electrostatic forces/fields to compare with Ewald sums.
 */
#ifndef GEN_SHELL_H
#define GEN_SHELL_H
#include <assert.h>

#include <algorithm>
#include <iostream>
#include <vector>

#include "macro.h"

using namespace std;

bool rcompare(vector<int> a, vector<int> b);

// Generate list of cencentric spherical cells
void nshell_list(const int &                    ncut,
                 const double &                 la,
                 const double &                 lb,
                 const double &                 lc,
                 int &                          lmax,
                 int &                          mmax,
                 int &                          nmax,
                 long int &                     ntot,
                 vector<double> &               nmag,
                 vector<vector<vector<int> > > &nlist,
                 FILE *                         fout);
#endif
