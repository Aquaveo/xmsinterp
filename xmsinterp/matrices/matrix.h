#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief routines for manipulating matrices
///
///        The routines in this file come from "Numerical Recipes in C"
///        by Press et al.
///        important: to use these routines properly, the matrices
///        and vectors should be declared as pointers and initialized
///        from [1..n] using the routines defined in this file.
/// \ingroup matrices
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

// 3. Standard Library Headers

// 4. External Library Headers
#include <xmscore/points/pt.h>

// 5. Shared Headers

// 6. Non-shared Headers

//----- Namespace declaration --------------------------------------------------

namespace xms
{
int mxLUDecomp(double** mat, int n, int* indx, double* d);
int mxLUBcksub(double** mat, int n, const int* indx, double* b);
bool mxiLudcmp3(double mat[3][3], int indx[3], double* d);
void mxiLubksb3(const double mat[3][3], const int indx[3], double b[3]);
bool mxSolveNxN(double** A, double* x, double* b, int n);
bool mxSolveBandedEquations(double** a, double* x, double* b, int numeqs, int halfbandwidth);
bool mxSolve3x3(double A[3][3], double x[3], double b[3]);
int mxInvert4x4(const double matrix[4][4], double inv[4][4]);
void mxPointMult(Pt3d* pt, const double ctm[4][4]);
void mxCopy4x4(double copy[4][4], const double matrix[4][4]);
void mxIdent4x4(double matrix[4][4]);
void mxMult4x4(double product[4][4], const double matrix1[4][4], const double matrix2[4][4]);
void mxRotate4x4(double xrot, double yrot, double zrot, double rMatt[4][4]);
void mxTranslate4x4(const Pt3d& a_translation, double a_mx[4][4]);
} // namespace xms
