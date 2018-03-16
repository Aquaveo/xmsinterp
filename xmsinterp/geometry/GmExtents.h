#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard Library Headers

// 4. External Library Headers

// 5. Shared Headers
#include <xmscore/points/pt.h>

// 6. Non-shared Headers

//----- Forward declarations ---------------------------------------------------

namespace boost
{
namespace serialization
{
class access;
}
}

//----- Namespace declaration --------------------------------------------------
namespace xms
{
class GmExtents3d; // forward declaration cause we use it in GmExtents2d ctor

////////////////////////////////////////////////////////////////////////////////
class GmExtents2d
{
  /// \cond
  friend class boost::serialization::access;
  /// \endcond

public:
  GmExtents2d();
  explicit GmExtents2d(const GmExtents3d& a_extents);
  GmExtents2d(const Pt2d& a_min, const Pt2d& a_max);
  GmExtents2d(const Pt3d& a_min, const Pt3d& a_max);
  virtual ~GmExtents2d();

  void operator+=(const GmExtents2d& a_rhs);
  virtual void AddToExtents(const Pt3d& pt);
  template <class ItrType>
  void Add(ItrType a_begin, ItrType a_end);
  bool IsValid();
  static void SetTolerance(double a_Tol);
  static double GetTolerance();
  bool Overlap(const GmExtents2d& a_b) const;
  void Clear();
  void GetExtents(double* a_MinX, double* a_MaxX, double* a_MinY, double* a_MaxY);
  void GetExtents(Pt3d& a_dMin, Pt3d& a_dMax);
  double GetMinX() const;
  double GetMaxX() const;
  double GetMinY() const;
  double GetMaxY() const;
  Pt2d GetMin() const;
  Pt2d GetMax() const;
  void Set(const Pt2d& a_min, const Pt2d& a_max);

  template <typename Archive>
  void serialize(Archive& archive, const unsigned int version);

protected:
  Pt2d m_min;                ///< Minimum, maximum extents
  Pt2d m_max;                ///< Maximum, maximum extents
  static double m_tolerance; ///< Tolerance used in comparisons
};                           // class GmExtents2d

//------------------------------------------------------------------------------
/// \brief Adds a range defined by two iterators to the extents.
/// \param[in] a_begin: Start of range.
/// \param[in] a_end: End of range.
//------------------------------------------------------------------------------
template <typename ItrType>
void GmExtents2d::Add(ItrType a_begin, ItrType a_end)
{
  ItrType itr(a_begin);
  while (itr != a_end)
  {
    AddToExtents(*itr);
    ++itr;
  }
} // GmExtents2d::Add
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void GmExtents2d::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_min;
  archive& m_max;
  archive& m_tolerance;
} // GmExtents2d::serialize

////////////////////////////////////////////////////////////////////////////////
class GmExtents3d
{
  //#ifdef CXX_TEST
  // friend class GmExtents3dUnitTests; ///< For unit tests
  //#endif
  /// \cond
  friend class boost::serialization::access;
  /// \endcond

public:
  GmExtents3d();
  explicit GmExtents3d(const GmExtents2d& a_extents);
  GmExtents3d(const Pt3d& a_min, const Pt3d& a_max);
  GmExtents3d(const Pt2d& a_min, const Pt2d& a_max);

  void operator+=(const GmExtents3d& a_rhs);
  void AddToExtents(const Pt3d& pt);
  bool IsValid() const;
  static void SetTolerance(double a_Tol);
  static double GetTolerance();
  bool Overlap(GmExtents3d& a_b) const;
  void Clear();
  bool IsPointWithin(const Pt3d& pt) const;
  void GetExtents(double* a_MinX,
                  double* a_MaxX,
                  double* a_MinY,
                  double* a_MaxY,
                  double* a_MinZ,
                  double* a_MaxZ) const;
  void GetExtents(Pt3d& a_dMin, Pt3d& a_dMax) const;
  double GetMinX() const;
  double GetMaxX() const;
  double GetMinY() const;
  double GetMaxY() const;
  double GetMinZ() const;
  double GetMaxZ() const;
  Pt3d GetMin() const;
  Pt3d GetMax() const;
  void Set(const Pt3d& a_min, const Pt3d& a_max);

  template <typename Archive>
  void serialize(Archive& archive, const unsigned int version);

protected:
  Pt3d m_min;                ///< Minimum, maximum extents
  Pt3d m_max;                ///< Maximum, maximum extents
  static double m_tolerance; ///< Tolerance used in comparisons
};                           // class GmExtents3d

//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void GmExtents3d::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_min;
  archive& m_max;
  archive& m_tolerance;
} // GmExtents3d::serialize

} // namespace xms
