#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief Utility functions called by interpolation code
/// \ingroup interpolate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard library headers

// 4. External library headers

// 5. Shared code headers
#include <xmscore/points/pt.h>     // for Pt3d
#include <xmscore/points/ptsfwd.h> // for Pt3d
#include <xmscore/misc/DynBitset.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Forward declarations ---------------------------------------------------

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
/// \brief Convenience struct used in interpolation utility functions
struct InterpPtInfo
{
public:
  Pt3d m_loc;      ///< location of interest
  double m_scalar; ///< scalar at location
  double m_weight; ///< weight at location
};

//----- Function prototypes ----------------------------------------------------
void inNodalFuncSetUpMatrixAndVector(double xk,
                                     double yk,
                                     double fk,
                                     const std::vector<InterpPtInfo>& closest,
                                     double** M,
                                     double* VV);
void inNodalFuncSetUpMatrixAndVector3d(double xk,
                                       double yk,
                                       double zk,
                                       double fk,
                                       const std::vector<InterpPtInfo>& closest,
                                       double** M,
                                       double* VV);
void inDistanceSquared(const Pt3d& a_pt,
                       const std::vector<int>& a_ptIdxs,
                       const std::vector<Pt3d>& a_ptLoc,
                       bool a_2d,
                       std::vector<double>& a_d2);
void inIdwWeights(const std::vector<double>& a_d2,
                  double a_power,
                  bool a_modifiedShepardWeights,
                  std::vector<double>& a_w);
bool inAllScalarsEqual(const std::vector<float>& a_scalar, const DynBitset& a_act);
std::vector<float> inGenerateRasterIdw(const std::vector<Pt3d>& a_pts,
                                       const Pt3d& a_minPt,
                                       const Pt3d& a_size,
                                       const int a_numRows,
                                       const int a_numCols,
                                       const float a_noDataVal,
                                       const std::vector<int>& a_activity);

} // namespace xms
