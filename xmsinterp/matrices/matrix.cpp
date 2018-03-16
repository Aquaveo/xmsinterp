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
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/matrices/matrix.h>

// 3. Standard library headers
#include <cmath>

// 4. External library headers
#include <xmscore/points/pt.h> // Pt3d

// 5. Shared code headers
#include <xmscore/misc/XmLog.h>   // XM_LOG
#include <xmscore/misc/xmstype.h> // XM_PI, XM_SUCCESS, XM_FAILURE

// 6. Non-shared code headers

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//-------------------------------------------------------------------------------
/// \brief    Decompose an n x n matrix into Upper & Lower trianglar (in place),
/// from "Numerical Recipes in C" p 43.
/// \param  mat matrix[n][n]
/// \param  n dimension of matrix mat
/// \param  indx
/// \param  d matrix determinant
/// \return XM_FAILURE if errors encountered else XM_SUCCESS
//-------------------------------------------------------------------------------
int mxLUDecomp(double** mat, int n, int* indx, double* d)
{
  int i, j, k, imax = 0;
  double big, tmp, sum;
  // allocate implicit scales for each row
  std::vector<double> vv(n, 0);
  *d = 1.0; // no row interchanges yet
  // loop over rows to get implicit scale info
  for (i = 0; i < n; ++i)
  {
    big = 0.0;
    for (j = 0; j < n; ++j)
      if ((tmp = fabs(mat[i][j])) > big)
        big = tmp;
    if (big == 0.0)
    {
      return (XM_FAILURE);
    }
    vv[i] = 1.0 / big;
  }
  // loop over columns-Crout's method
  for (j = 0; j < n; ++j)
  {
    for (i = 0; i < j; ++i)
    {
      sum = mat[i][j];
      for (k = 0; k < i; ++k)
        sum -= mat[i][k] * mat[k][j];
      mat[i][j] = sum;
    }
    big = 0.0; // initialize pivot search
    for (i = j; i < n; ++i)
    {
      sum = mat[i][j];
      for (k = 0; k < j; ++k)
        sum -= mat[i][k] * mat[k][j];
      mat[i][j] = sum;
      if ((tmp = vv[i] * fabs(sum)) > big)
      {
        big = tmp;
        imax = i;
      }
    }
    if (j != imax)
    { // do we need to change rows
      for (k = 0; k < n; ++k)
      {
        tmp = mat[imax][k];
        mat[imax][k] = mat[j][k];
        mat[j][k] = tmp;
      }
      *d = -(*d);
      vv[imax] = vv[j];
    }
    indx[j] = imax;
    if (mat[j][j] == 0.0)
      mat[0][0] = 1.0e-20;
    if (j != n)
    { // divide by the pivot element
      tmp = 1.0 / mat[j][j];
      for (i = j + 1; i < n; ++i)
        mat[i][j] *= tmp;
    }
  } // go back for next column
  return XM_SUCCESS;
} // mxLUDecomp
//-------------------------------------------------------------------------------
/// \brief    solve [mat]{x} = {b} by back substitution (mat = LU of mat),
/// from "Numerical Recipes in C" p 44.
/// \param  mat matrix[n][n]
/// \param  n dimension of matrix mat
/// \param  indx
/// \param  b vector of solution [mat]{x} = {b}
/// \return XM_SUCCESS
//-------------------------------------------------------------------------------
int mxLUBcksub(double** mat, int n, const int* indx, double* b)
{
  int i, j, ii, ip;
  double sum;
  // do the forward substitution
  ii = -1;
  for (i = 0; i < n; ++i)
  {
    ip = indx[i];
    sum = b[ip];
    b[ip] = b[i];
    if (ii >= 0)
      for (j = ii; j <= i - 1; ++j)
        sum -= mat[i][j] * b[j];
    else if (sum)
      ii = i;
    b[i] = sum;
  }
  // do the back substitution
  for (i = n - 1; i >= 0; --i)
  {
    sum = b[i];
    for (j = i + 1; j < n; ++j)
      sum -= mat[i][j] * b[j];
    b[i] = sum / mat[i][i]; // store a component of solution
  }
  return XM_SUCCESS;
} // mxLUBcksub
//-------------------------------------------------------------------------------
/// \brief    Decomposes a 3 x 3 matrix into Upper & Lower trianglar(in place)
/// \param  mat matrix[3][3]
/// \param  indx
/// \param  d determinant of matrix
/// \return false if errors are encountered else true
//-------------------------------------------------------------------------------
bool mxiLudcmp3(double mat[3][3], int indx[3], double* d)
{
  int i, j, k, imax;
  double vv[3], big, tmp, sum;

  imax = 0; // there is a slight chance that leaving this uninitialized can
            // lead to a UMR below.
  *d = 1.0; // no row interchanges yet
  // loop over rows to get implicit scale info
  for (i = 0; i < 3; ++i)
  {
    big = 0.0;
    for (j = 0; j < 3; ++j)
      if ((tmp = fabs(mat[i][j])) > big)
        big = tmp;
    if (big == 0.0)
    {
      XM_LOG(xmlog::debug, "Singular Matrix");
      return false;
    }
    vv[i] = 1.0 / big;
  }
  // loop over columns-Crout's method
  for (j = 0; j < 3; ++j)
  {
    for (i = 0; i < j; ++i)
    {
      sum = mat[i][j];
      for (k = 0; k < i; ++k)
        sum -= mat[i][k] * mat[k][j];
      mat[i][j] = sum;
    }
    big = 0.0; // initialize pivot search
    for (i = j; i < 3; ++i)
    {
      sum = mat[i][j];
      for (k = 0; k < j; ++k)
        sum -= mat[i][k] * mat[k][j];
      mat[i][j] = sum;
      if ((tmp = vv[i] * fabs(sum)) > big)
      {
        big = tmp;
        imax = i;
      }
    }
    if (j != imax)
    { // do we need to change rows
      for (k = 0; k < 3; ++k)
      {
        tmp = mat[imax][k];
        mat[imax][k] = mat[j][k];
        mat[j][k] = tmp;
      }
      *d = -(*d);
      vv[imax] = vv[j];
    }
    indx[j] = imax;
    if (mat[j][j] == 0.0)
      mat[j][j] = 1.0e-20;
    if (j != 3)
    { // divide by the pivot element
      tmp = 1.0 / mat[j][j];
      for (i = j + 1; i < 3; ++i)
        mat[i][j] *= tmp;
    }
  } // go back for next column
  return true;
} // mxiLudcmp3
//-------------------------------------------------------------------------------
/// \brief    solve [mat]{x} = {b} by back substitution (mat = LU of mat)
/// \param  mat matrix[3][3]
/// \param  indx
/// \param  b vector of solution [mat]{x} = {b}
//-------------------------------------------------------------------------------
void mxiLubksb3(const double mat[3][3], const int indx[3], double b[3])
{
  int i, j, ii, ip;
  double sum;
  // do the forward substitution
  ii = -1;
  for (i = 0; i < 3; ++i)
  {
    ip = indx[i];
    sum = b[ip];
    b[ip] = b[i];
    if (ii >= 0)
      for (j = ii; j <= i - 1; ++j)
        sum -= mat[i][j] * b[j];
    else if (sum)
      ii = i;
    b[i] = sum;
  }
  // do the back substitution
  for (i = 2; i >= 0; --i)
  {
    sum = b[i];
    for (j = i + 1; j < 3; ++j)
      sum -= mat[i][j] * b[j];
    b[i] = sum / mat[i][i]; // store a component of solution
  }
} // mxiLubksb3
//-------------------------------------------------------------------------------
/// \brief  Solves the matrix equation [A]*{x}={b} for {x} where [A] is NxN
/// and {x} and {b} are Nx1 vectors. The matrix A is replaced with the LU
/// decomposition of A.
/// \param  A matrix[n][n]
/// \param  x vector nx1
/// \param  b vector of solution [A]{x} = {b}
/// \param  n size of matrix A
/// \return false if errors encountered else true
//-------------------------------------------------------------------------------
bool mxSolveNxN(double** A, double* x, double* b, int n)
{
  int *indx, i;
  double d;

  switch (n)
  {
  case 1:
    if (fabs(A[0][0]) < 0.000000001)
      return false;
    x[0] = b[0] / A[0][0];
    return true;
  case 2:
    if (fabs(A[0][1]) >= fabs(A[0][0]))
    {
      if (fabs(A[0][1]) < 0.000000001)
        return false;
      d = A[1][1] / A[0][1];
      x[0] = (b[1] - d * b[0]) / (A[1][0] - d * A[0][0]);
      x[1] = (b[0] - A[0][0] * x[0]) / A[0][1];
    }
    else
    {
      d = A[1][0] / A[0][0];
      x[1] = (b[1] - d * b[0]) / (A[1][1] - d * A[0][1]);
      x[0] = (b[0] - A[0][1] * x[1]) / A[0][0];
    }
    return true;
  default:
    indx = (int*)malloc(n * sizeof(int));
    if (mxLUDecomp(A, n, indx, &d) == XM_FAILURE)
    {
      free(indx);
      return false;
    }
    for (i = 0; i < n; ++i)
      x[i] = b[i];
    mxLUBcksub(A, n, indx, x);
    free(indx);
    return true;
  }
} // mxSolveNxN
//-------------------------------------------------------------------------------
/// \brief  Solves the matrix equation [a][x]=[b] using gauss elimination
/// \param  a matrix[n][n]
/// \param  x vector nx1
/// \param  b vector of solution [A]{x} = {b}
/// \param  numeqs number of equations
/// \param  halfbandwidth
/// \note   Assumes that [a] is a banded symmetric matrix with only the upper
/// half stored.  If a matrix is banded and symmetric, only values at
/// or above the main diagonal need to be stored.  When setting up the
/// matrix, the indices in the equivalent full matrix are mapped to
/// the indices in the banded matrix with the following formula
/// (i,j)=(i,j-i+1) where j is the row number and i is the column number.
/// \return false if errors encountered else true
//-------------------------------------------------------------------------------
bool mxSolveBandedEquations(double** a, double* x, double* b, int numeqs, int halfbandwidth)

{
  int i, ib, m1, j, k, k1, j1, j2, mM, np1;
  double sum;

  for (i = 0; i < numeqs; i++)
    x[i] = 0.0;

  ib = halfbandwidth;
  for (i = 1; i < numeqs; i++)
  {
    m1 = std::min(ib - 1, numeqs - i);
    for (j = 0; j < m1; j++)
    {
      sum = 0.0;
      k1 = std::min(i, ib - j - 1);
      for (k = 0; k < k1; k++)
      {
        if (fabs(a[i - k - 1][0]) < 1e-20)
          return false;
        else
        {
          if (fabs(a[i - k - 1][0]) < 1e-20)
            return false;
          sum += a[i - k - 1][k + 1] * a[i - k - 1][j + k + 1] / a[i - k - 1][0];
        }
      }
      a[i][j] -= sum;
    }
  }

  for (i = 1; i < numeqs; i++)
  {
    sum = 0.0;
    k1 = std::min(ib - 1, i);
    for (k = 0; k < k1; k++)
    {
      if (fabs(a[i - k - 1][0] * b[i - k - 1]) < 1e-20)
        return false;
      sum += a[i - k - 1][k + 1] / a[i - k - 1][0] * b[i - k - 1];
    }
    b[i] -= sum;
  }

  if (fabs(a[numeqs - 1][0]) < 1e-20)
    return false;
  x[numeqs - 1] = b[numeqs - 1] / a[numeqs - 1][0];
  np1 = numeqs;
  for (k = 1; k < numeqs; k++)
  {
    i = np1 - k;
    j1 = i + 1;
    j2 = std::min(numeqs, i + ib - 1);
    sum = 0.0;
    for (j = j1; j <= j2; j++)
    {
      mM = j - j1 + 2;
      sum += x[j - 1] * a[i - 1][mM - 1];
    }
    if (fabs(a[i - 1][0]) < 1e-20)
      return false;
    x[i - 1] = (b[i - 1] - sum) / a[i - 1][0];
  }
  return true;
} // mxSolveBandedEquations
//-------------------------------------------------------------------------------
/// \brief  Solves the matrix equation [A]*{x}={b} for {x} where [A] is 3x3
/// and {x} and {b} are 3x1 vectors.
/// \param  A matrix[3][3]
/// \param  x vector[3]
/// \param  b vector[3] of solution [A]{x} = {b}
/// \return XM_FAILURE if errors are encountered else XM_SUCCESS
//-------------------------------------------------------------------------------
bool mxSolve3x3(double A[3][3], double x[3], double b[3])
{
  int indx[3];
  double d;

  if (!mxiLudcmp3(A, indx, &d))
    return false;
  mxiLubksb3(A, indx, b);

  x[0] = b[0];
  x[1] = b[1];
  x[2] = b[2];
  return true;
} /* mxSolve3x3 */
//-------------------------------------------------------------------------------
/// \brief    Compute the inverse of a transformation matrix. Checks for special
/// case of Orthogonal 4x4 and does easy inverse otherwise uses Gauss-Jordan
/// Elimination with full pivoting and scaling with partial unraveling of loops
/// for speed. See Numerical Recipes in C pages 36-37
/// \param    matrix matrix[4][4] to invert
/// \param    inv inverse of matrix
/// \return XM_FAILURE if errors are encountered else XM_SUCCESS
//-------------------------------------------------------------------------------
int mxInvert4x4(const double matrix[4][4], double inv[4][4])
{
  double tmp[4][4], scale[4];
  // Make local copy of matrix
  mxCopy4x4(tmp, matrix);
  // generate scaling factors
  double dummy;
  int i, j, k, l;
  for (i = 0; i < 4; i++)
  {
    double biggest = 0.0;
    for (j = 0; j < 4; j++)
      if ((dummy = fabs(tmp[i][j])) > biggest)
        biggest = dummy;
    if (biggest == 0.0)
    {
      XM_LOG(xmlog::error, "(mxInvert4x4) Singular Matrix");
      return XM_FAILURE;
    }
    scale[i] = 1.0 / biggest;
  }

  int index_col[4], index_row[4], index_pivot[4];
  index_pivot[0] = 0;
  index_pivot[1] = 0;
  index_pivot[2] = 0;
  index_pivot[3] = 0;
  // main loop, columns to be reduced
  int irow = 0, icol = 0;
  for (i = 0; i < 4; i++)
  {
    // get pivot element
    double biggest = 0.0;
    for (j = 0; j < 4; j++)
      if (index_pivot[j] != 1)
        for (k = 0; k < 4; k++)
        {
          if (index_pivot[k] == 0)
          {
            if ((dummy = scale[j] * fabs(tmp[j][k])) >= biggest)
            {
              biggest = dummy;
              irow = j;
              icol = k;
            }
          }
          else if (index_pivot[k] > 1)
          {
            XM_LOG(xmlog::error, "(mxInvert4x4) Singular Matrix");
            return XM_FAILURE;
          }
        }
    ++(index_pivot[icol]);
    // interchange rows
    if (irow != icol)
    {
      double temp = tmp[irow][0];
      tmp[irow][0] = tmp[icol][0];
      tmp[icol][0] = temp;
      temp = tmp[irow][1];
      tmp[irow][1] = tmp[icol][1];
      tmp[icol][1] = temp;
      temp = tmp[irow][2];
      tmp[irow][2] = tmp[icol][2];
      tmp[icol][2] = temp;
      temp = tmp[irow][3];
      tmp[irow][3] = tmp[icol][3];
      tmp[icol][3] = temp;
      temp = scale[irow];
      scale[irow] = scale[icol];
      scale[icol] = temp;
    }
    index_row[i] = irow;
    index_col[i] = icol;
    if (tmp[icol][icol] == 0.0)
    {
      XM_LOG(xmlog::error, "(mxInvert4x4) Singular Matrix");
      return XM_FAILURE;
    }
    // divide pivot row by pivot
    double pivot_inverse = 1.0 / tmp[icol][icol];
    tmp[icol][icol] = 1.0;
    tmp[icol][0] *= pivot_inverse;
    tmp[icol][1] *= pivot_inverse;
    tmp[icol][2] *= pivot_inverse;
    tmp[icol][3] *= pivot_inverse;
    // reduce all rows except pivot row
    for (l = 0; l < 4; l++)
      if (l != icol)
      {
        dummy = tmp[l][icol];
        tmp[l][icol] = 0.0;
        tmp[l][0] -= tmp[icol][0] * dummy;
        tmp[l][1] -= tmp[icol][1] * dummy;
        tmp[l][2] -= tmp[icol][2] * dummy;
        tmp[l][3] -= tmp[icol][3] * dummy;
      }
  }
  // end of reduction algorithm
  // unscramble solution
  for (l = 3; l >= 0; l--)
  {
    if (index_row[l] != index_col[l])
    {
      double temp = tmp[0][index_row[l]];
      tmp[0][index_row[l]] = tmp[0][index_col[l]];
      tmp[0][index_col[l]] = temp;
      temp = tmp[1][index_row[l]];
      tmp[1][index_row[l]] = tmp[1][index_col[l]];
      tmp[1][index_col[l]] = temp;
      temp = tmp[2][index_row[l]];
      tmp[2][index_row[l]] = tmp[2][index_col[l]];
      tmp[2][index_col[l]] = temp;
      temp = tmp[3][index_row[l]];
      tmp[3][index_row[l]] = tmp[3][index_col[l]];
      tmp[3][index_col[l]] = temp;
    }
  }
  // copy solution to output
  mxCopy4x4(inv, tmp);
  return XM_SUCCESS;
} // mxInvert4x4
//-------------------------------------------------------------------------------
/// \brief    Multiplies a point by a transformation matrix
/// \param    pt point to multiply
/// \param    ctm transform matrix[4][4].
//-------------------------------------------------------------------------------
void mxPointMult(Pt3d* pt, const double ctm[4][4])
{
  double tx = pt->x;
  double ty = pt->y;
  double tz = pt->z;
  pt->x = tx * ctm[0][0] + ty * ctm[0][1] + tz * ctm[0][2] + ctm[0][3];
  pt->y = tx * ctm[1][0] + ty * ctm[1][1] + tz * ctm[1][2] + ctm[1][3];
  pt->z = tx * ctm[2][0] + ty * ctm[2][1] + tz * ctm[2][2] + ctm[2][3];
} // mxPointMult
//-------------------------------------------------------------------------------
/// \brief    Copy a transformation matrix. Simply uses memcpy
/// \param    copy matrix that will have result of copy
/// \param    matrix 4x4 matrix to copy
//-------------------------------------------------------------------------------
void mxCopy4x4(double copy[4][4], const double matrix[4][4])
{
  memcpy((char*)&(copy[0][0]), &(matrix[0][0]), 16 * sizeof(double));
} // mxCopy4x4
//-------------------------------------------------------------------------------
/// \brief    Sets a transformation matrix to identity
/// \param    matrix matrix to change
//-------------------------------------------------------------------------------
void mxIdent4x4(double matrix[4][4])
{
  matrix[0][1] = matrix[0][2] = matrix[0][3] = 0.0;
  matrix[1][0] = matrix[1][2] = matrix[1][3] = 0.0;
  matrix[2][0] = matrix[2][1] = matrix[2][3] = 0.0;
  matrix[3][0] = matrix[3][1] = matrix[3][2] = 0.0;
  matrix[0][0] = matrix[1][1] = matrix[2][2] = matrix[3][3] = 1.0;
} // mxIdent4x4
//-------------------------------------------------------------------------------
/// \brief    Multiplies two transformation matrices
/// \param  product result of matrix1*matrix2
/// \param  matrix1 [4][4] matrix multiplied by matrix2
/// \param  matrix2 [4][4] matrix multiplied by matrix1
//-------------------------------------------------------------------------------
void mxMult4x4(double product[4][4], const double matrix1[4][4], const double matrix2[4][4])
{
  int i, j, k;
  // We make internal copies of the matrices to allow you to pass the same
  // matrix as both the product and either matrix1 or matrix2.
  double temp1[4][4], temp2[4][4];
  mxCopy4x4(temp1, matrix1);
  mxCopy4x4(temp2, matrix2);
  for (i = 3; i >= 0; i--)
  {
    for (j = 3; j >= 0; j--)
    {
      product[i][j] = 0.;
      for (k = 3; k >= 0; k--)
        product[i][j] += (temp1[i][k] * temp2[k][j]);
    }
  }
} // mxMult4x4
//-------------------------------------------------------------------------------
/// \brief    return a rotation matrix with specified x, y, z rotations
/// z - y - x order
/// \param xrot x rotation amount
/// \param yrot y rotation amount
/// \param zrot z rotation amount
/// \param rMatt matrix to be built
//-------------------------------------------------------------------------------
void mxRotate4x4(double xrot, double yrot, double zrot, double rMatt[4][4])
{
  // rotation cosines
  double cosX = cos(xrot * XM_PI / 180);
  double cosY = cos(yrot * XM_PI / 180);
  double cosZ = cos(zrot * XM_PI / 180);
  // rotation sines
  double sinX = sin(xrot * XM_PI / 180);
  double sinY = sin(yrot * XM_PI / 180);
  double sinZ = sin(zrot * XM_PI / 180);
  mxIdent4x4(rMatt);
  rMatt[0][0] = cosY * cosZ;
  rMatt[1][0] = cosX * sinZ + sinX * sinY * cosZ;
  rMatt[2][0] = sinX * sinZ - cosX * sinY * cosZ;
  rMatt[0][1] = -cosY * sinZ;
  rMatt[1][1] = cosX * cosZ - sinX * sinY * sinZ;
  rMatt[2][1] = sinX * cosZ + cosX * sinY * sinZ;
  rMatt[0][2] = sinY;
  rMatt[1][2] = -sinX * cosY;
  rMatt[2][2] = cosX * cosY;
} // mxRotate4x4
//-------------------------------------------------------------------------------
/// \brief    return a translation matrix with specified translation
/// z - y - x order
/// \param    a_translation translation distances in x, y, and z.
/// \param    a_mx matrix to be built
//-------------------------------------------------------------------------------
void mxTranslate4x4(const Pt3d& a_translation, double a_mx[4][4])
{
  mxIdent4x4(a_mx);
  // build scale matrix
  a_mx[0][3] = a_translation.x;
  a_mx[1][3] = a_translation.y;
  a_mx[2][3] = a_translation.z;
} // mxTranslate4x4
} // namespace xms
