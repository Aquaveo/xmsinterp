#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup triangulate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Include Files ----------------------------------------------------------

// 3. Standard Library Headers

// 4. External Library Headers
#include <xmscore/points/pt.h>  // Pt*
#include <xmscore/stl/vector.h> // VecDbl

// 5. Shared Headers

// 6. Non-shared Headers

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Forward Declarations ---------------------------------------------------

//----- Typdefs / Classes ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
class TrTriangulator
{
public:
  TrTriangulator();
  virtual ~TrTriangulator();

  bool Triangulate();

  /// \cond
  virtual void SetPointLocations(const VecPt3d& /*a_locs*/){};

  // These functions are used to iterate through the point locations
  // and provide connectivity
  virtual int GetID() const = 0;
  virtual Pt3d GetLocation() const = 0;
  virtual int GetNPoints() const = 0;
  virtual void IncrementPoint() = 0;

  virtual void PrepareToReceiveTriangles() = 0;
  virtual void FinalizeTriangulation() = 0;
  virtual void ReceiveTriangle(int a_id1, int a_id2, int a_id3) = 0;

private:
  TrTriangulator(const TrTriangulator& a_rhs);
  const TrTriangulator& operator=(const TrTriangulator&);
  /// \endcond
}; // TrTriangulator

//----- Function Prototypes ----------------------------------------------------

} // namespace xms
