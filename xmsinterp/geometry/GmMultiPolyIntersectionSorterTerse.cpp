//------------------------------------------------------------------------------
/// \file
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My header
#include <xmsinterp/geometry/GmMultiPolyIntersectionSorterTerse.h>

// 3. Standard Library Headers
#include <cfloat>

// 4. External Library Headers
#include <xmscore/math/math.h>                             // EQ_EPS
#include <xmscore/misc/XmError.h>                          // XM_*
#include <xmscore/misc/xmstype.h>                          // XM_NONE
#include <xmscore/stl/set.h>                               // Set*
#include <xmscore/stl/vector.h>                            // Vec*
#include <xmsinterp/geometry/GmMultiPolyIntersectorData.h> // GmMultiPolyIntersectorData
#include <xmsinterp/geometry/geoms.h> // gmComputeCentroid, gmTurn

// 5. Shared Headers

// 6. Non-shared Headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms {
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

namespace { // unnamed namespace
} // unnamed namespace

////////////////////////////////////////////////////////////////////////////////
/// \class GmMultiPolyIntersectionSorterTerse
/// \brief Class for sorting intersections from GmMultiPolyIntersector in a
///        terse way (with no duplicate info).
///
///        This class removes corner touches, duplicate edges (by keeping
///        the one on the right), and, ... well it's hard to define what it
///        does here but suffice it to say that it causes all the unit
///        tests in GmMultiPolyIntersector.cpp to pass.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Sort the intersections, remove redundant info.
/// \param[in,out] a_data: The intersection data from GmMultiPolyIntersector.
/// \param[out] polyids: Intersected polygons.
/// \param[out] tvalues: Percent distance (0.0 - 1.0) from segment starting
///                      location to all intersections.
/// \param[out] a_pts:   Points of intersection.
/// \param[in] a_tol:    Tolerance used when comparing t values.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectionSorterTerse::Sort(
    GmMultiPolyIntersectorData &a_data, std::vector<int> &polyids,
    std::vector<double> &tvalues, std::vector<Pt3d> &a_pts, double a_tol) {
  m_d = &a_data;
  m_tol = a_tol;
  RemoveCornerTouches();
  RemoveDuplicateEdges();
  SwapAdjacents();
  IntersectionsToPolyIdsAndTValues(polyids, tvalues, a_pts);
  FixArrays(polyids, tvalues, a_pts);
} // GmMultiPolyIntersectionSorterTerse::Sort
//------------------------------------------------------------------------------
/// \brief Remove polys that only touch the line at a corner. We try to
///        identify these corner polys using the following criteria:
///        1. There are > 1 intersection with the same t value.
///        2. The corner poly doesn't appear in the previous or next
///           group of intersections that have the same t values.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectionSorterTerse::RemoveCornerTouches() {
  XM_ENSURE_TRUE_VOID_NO_ASSERT(m_d->m_ixs.size() > 1);

  VecInt tChange;
  FindWhereTValuesChange(tChange);

  VecInt toRemove;

  // Loop through tChanges
  for (size_t i = 0; i < tChange.size() - 1; ++i) {
    if (tChange[i + 1] - tChange[i] > 1) {
      VecInt inNeither;
      FindPreviousNextNeither(tChange, (int)i, nullptr, nullptr, &inNeither);

      if ((i > 0 && i + 2 < tChange.size()) || inNeither.size() > 1 ||
          (i == 0 && tChange[1] - tChange[0] > 1)) {
        toRemove.insert(toRemove.end(), inNeither.begin(), inNeither.end());
      }
    }
  }

  // Remove them, working backwards
  for (size_t i = toRemove.size(); i-- > 0;) {
    m_d->m_ixs.erase(m_d->m_ixs.begin() + toRemove[i]);
  }
} // GmMultiPolyIntersectionSorterTerse::RemoveCornerTouches
//------------------------------------------------------------------------------
/// \brief If the line goes along an edge we may have polygons on both sides
///        of the edge. Perhaps we want only the polygons on one side .... ?
//------------------------------------------------------------------------------
void GmMultiPolyIntersectionSorterTerse::RemoveDuplicateEdges() {
  XM_ENSURE_TRUE_VOID_NO_ASSERT(m_d->m_ixs.size() > 1);

  VecInt tChange;
  FindWhereTValuesChange(tChange);

  VecInt toRemove;

  // Loop through tChanges
  for (size_t i = 0; i < tChange.size() - 1; ++i) {
    if (tChange[i + 1] - tChange[i] > 1) {
      VecInt inNext;
      // Get intersections in this tvalue group that also appear in next group.
      // These are the first points of the edges.
      FindPreviousNextNeither(tChange, (int)i, nullptr, &inNext, nullptr);

      if (inNext.size() > 1) {
        // Get intersections in next tvalue group that match those in current.
        // These are the last points of the edges.
        VecInt inPrev;
        FindPreviousNextNeither(tChange, (int)i + 1, &inPrev, nullptr, nullptr);
        // XM_ASSERT(inPrev.size() == inNext.size());
        // Not sure why the assert above was put in. Even when it asserts,
        // we still seem to get good results in the end.

        // Keep only the polygons that are on the right of the line
        bool found = false; // Should be just 2 so stop if we find one
        for (size_t j = 0; j < inNext.size() && !found; ++j) {
          // Find the other end of this edge
          int edgeBeg = inNext[j]; // index into m_d->m_ixs of beginning of edge
          int edgeEnd = XM_NONE;   // index into m_d->m_ixs of end of edge
          for (size_t k = 0; k < inPrev.size() && edgeEnd == XM_NONE; ++k) {
            if (m_d->m_ixs[inPrev[k]].m_i == m_d->m_ixs[edgeBeg].m_i)
              edgeEnd = inPrev[k];
          }
          XM_ASSERT(edgeEnd != XM_NONE);

          int polyIdx = m_d->m_ixs[edgeBeg].m_i - 1;
          // Create the poly as a vector of Pt3ds for gmComputeCentroid
          VecPt3d poly;
          for (size_t k = 0; k < m_d->m_polys[polyIdx].size(); ++k) {
            poly.push_back(m_d->m_points[m_d->m_polys[polyIdx][k]]);
          }
          Pt3d centroid = gmComputeCentroid(poly);
          Turn_enum turn = gmTurn(m_d->m_ixs[edgeBeg].m_pt,
                                  m_d->m_ixs[edgeEnd].m_pt, centroid);
          if (turn != TURN_RIGHT) {
            toRemove.push_back(edgeBeg);
            toRemove.push_back(edgeEnd);
            found = true;
          }
        }
      }
    }
  }

  // Remove them, working backwards
  std::sort(toRemove.begin(), toRemove.end());
  for (size_t i = toRemove.size(); i-- > 0;) {
    m_d->m_ixs.erase(m_d->m_ixs.begin() + toRemove[i]);
  }
} // GmMultiPolyIntersectionSorterTerse::RemoveDuplicateEdges
//------------------------------------------------------------------------------
/// \brief Swap adjacent identical intersections if necessary.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectionSorterTerse::SwapAdjacents() {
  XM_ENSURE_TRUE_VOID_NO_ASSERT(m_d->m_ixs.size() > 1);

  // 2 intersections
  if (m_d->m_ixs.size() == 2) {
    if ((!m_d->m_polys1.empty() &&
         m_d->m_polys1.find(m_d->m_ixs[0].m_i) == m_d->m_polys1.end()) ||
        (!m_d->m_polys2.empty() &&
         m_d->m_polys2.find(m_d->m_ixs[1].m_i) == m_d->m_polys2.end())) {
      Swap(0, 1);
    }
    return;
  }

  // 3 or more intersections
  int i = 1;
  // Do first one outside of loop
  if (EQ_EPS(m_d->m_ixs[1].m_t, m_d->m_ixs[0].m_t, FLT_EPSILON) &&
      (m_d->m_ixs[0].m_i == m_d->m_ixs[2].m_i ||
       (m_d->m_ixs.size() > 3 && m_d->m_ixs[0].m_i == m_d->m_ixs[3].m_i))) {
    Swap(0, 1);
    ++i;
  }

  for (; i < (int)m_d->m_ixs.size(); ++i) {
    if (EQ_EPS(m_d->m_ixs[i].m_t, m_d->m_ixs[i - 1].m_t, FLT_EPSILON)) {
      // look back
      if (i - 2 >= 0 && m_d->m_ixs[i].m_i == m_d->m_ixs[i - 2].m_i) {
        Swap(i, i - 1);
      }
    }
  }
} // GmMultiPolyIntersectionSorterTerse::SwapAdjacents
//------------------------------------------------------------------------------
/// \brief Copy intersections to polyids and tvalues when there are
///        2 intersections.
/// \param polyids: Poly IDs.
/// \param tvalues: tvalues.
/// \param a_pts: Points.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectionSorterTerse::IntersectionsToPolyIdsAndTValuesFor2(
    VecInt &polyids, VecDbl &tvalues, std::vector<Pt3d> &a_pts) const {
  if (m_d->m_ixs.size() == 2) {
    if (EQ_EPS(m_d->m_ixs[0].m_t, m_d->m_ixs[1].m_t, FLT_EPSILON)) {
      if (!m_d->m_polys1.empty() && !m_d->m_polys2.empty() &&
          m_d->m_polys1 != m_d->m_polys2) {
        // test2InIn. Combine 2 into 1 getting entrance into second poly
        if (m_d->m_polys1.find(m_d->m_ixs[0].m_i) != m_d->m_polys1.end()) {
          AddToPolyIdsAndTValues(m_d->m_ixs[1], polyids, tvalues, a_pts);
        } else {
          AddToPolyIdsAndTValues(m_d->m_ixs[0], polyids, tvalues, a_pts);
        }
      } else {
        AddToPolyIdsAndTValues(m_d->m_ixs[0], polyids, tvalues, a_pts);
        AddToPolyIdsAndTValues(m_d->m_ixs[1], polyids, tvalues, a_pts);
      }
    } else {
      AddToPolyIdsAndTValues(m_d->m_ixs[0], polyids, tvalues, a_pts);
      AddToPolyIdsAndTValues(m_d->m_ixs[1], polyids, tvalues, a_pts);
    }
    return;
  }
} // GmMultiPolyIntersectionSorterTerse::IntersectionsToPolyIdsAndTValuesFor2
//------------------------------------------------------------------------------
/// \brief Copy intersections to polyids and tvalues when there are
///        3 or more intersections.
/// \param polyids: Poly IDs.
/// \param tvalues: tvalues.
/// \param a_pts: Points.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectionSorterTerse::
    IntersectionsToPolyIdsAndTValuesFor3OrMore(VecInt &polyids, VecDbl &tvalues,
                                               std::vector<Pt3d> &a_pts) const {
  XM_ENSURE_TRUE_VOID_NO_ASSERT(m_d->m_ixs.size() > 1);

  // Mark where we have a change in t values
  VecInt tChange;
  FindWhereTValuesChange(tChange);

  SetInt setIds, setIdsPrev; // ids we're adding now, & that we added previously
  // Loop through tChanges
  for (int i = 0; i < (int)tChange.size() - 1; ++i) {
    if (tChange[i + 1] - tChange[i] > 1) {
      VecInt inPrev, inNext, inNeither;
      FindPreviousNextNeither(tChange, i, &inPrev, &inNext, &inNeither);

      if (i == 0) {
        // If we don't have any in "inNeither", use next as inNeither.
        if (inPrev.empty() && inNeither.empty())
          inNeither = inNext;

        // Take the ones that are not found
        for (size_t j = 0; j < inNeither.size(); ++j) {
          setIds.insert(m_d->m_ixs[inNeither[j]].m_i);
          AddToPolyIdsAndTValues(m_d->m_ixs[inNeither[j]], polyids, tvalues,
                                 a_pts);
        }
      } else {
        // Take the ones that were found in the previous group, that were
        // not added previously
        for (size_t j = 0; j < inPrev.size(); ++j) {
          if (setIdsPrev.find(m_d->m_ixs[inPrev[j]].m_i) == setIdsPrev.end()) {
            setIds.insert(m_d->m_ixs[inPrev[j]].m_i);
            AddToPolyIdsAndTValues(m_d->m_ixs[inPrev[j]], polyids, tvalues,
                                   a_pts);
          }
        }

        if (setIds.empty()) {
          // Since we haven't added any yet, add those from next that weren't
          // added previously
          for (size_t j = 0; j < inNext.size(); ++j) {
            if (setIdsPrev.find(m_d->m_ixs[inNext[j]].m_i) ==
                setIdsPrev.end()) {
              setIds.insert(m_d->m_ixs[inNext[j]].m_i);
              AddToPolyIdsAndTValues(m_d->m_ixs[inNext[j]], polyids, tvalues,
                                     a_pts);
            }
          }
        }

        // Then take the ones that were not found
        for (size_t j = 0; j < inNeither.size(); ++j) {
          setIds.insert(m_d->m_ixs[inNeither[j]].m_i);
          AddToPolyIdsAndTValues(m_d->m_ixs[inNeither[j]], polyids, tvalues,
                                 a_pts);
        }

        if (setIds.empty()) {
          // Since we haven't added any yet, add those from previous group
          for (size_t j = 0; j < inPrev.size(); ++j) {
            setIds.insert(m_d->m_ixs[inPrev[j]].m_i);
            AddToPolyIdsAndTValues(m_d->m_ixs[inPrev[j]], polyids, tvalues,
                                   a_pts);
          }
        }
      }
    } else {
      setIds.insert(m_d->m_ixs[tChange[i]].m_i);
      AddToPolyIdsAndTValues(m_d->m_ixs[tChange[i]], polyids, tvalues, a_pts);
    }

    setIdsPrev = setIds;
    setIds.clear();
  }

} // IntersectionsToPolyIdsAndTValuesFor3OrMore
//------------------------------------------------------------------------------
/// \brief Copy intersections to polyids and tvalues, removing duplicates.
/// \param polyids: Poly IDs.
/// \param tvalues: tvalues.
/// \param a_pts: Points.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectionSorterTerse::IntersectionsToPolyIdsAndTValues(
    VecInt &polyids, VecDbl &tvalues, std::vector<Pt3d> &a_pts) const {
  polyids.clear();
  tvalues.clear();
  a_pts.clear();

  XM_ENSURE_TRUE_VOID_NO_ASSERT(!m_d->m_ixs.empty());

  // 1 intersection
  if (m_d->m_ixs.size() == 1) {
    polyids.push_back(m_d->m_ixs[0].m_i);
    tvalues.push_back(m_d->m_ixs[0].m_t);
    a_pts.push_back(m_d->m_ixs[0].m_pt);
    return;
  }

  // 2 intersections
  if (m_d->m_ixs.size() == 2) {
    IntersectionsToPolyIdsAndTValuesFor2(polyids, tvalues, a_pts);
    return;
  }

  // 3 or more intersections
  IntersectionsToPolyIdsAndTValuesFor3OrMore(polyids, tvalues, a_pts);

} // GmMultiPolyIntersectionSorterTerse::IntersectionsToPolyIdsAndTValues
//------------------------------------------------------------------------------
/// \brief Make sure we always have at least two tvalues, an entrance and
///        an exit, and that the last polyid = -1.
/// \param polyids: Poly IDs.
/// \param tvalues: tvalues.
/// \param a_pts: Point list.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectionSorterTerse::FixArrays(VecInt &polyids,
                                                   VecDbl &tvalues,
                                                   VecPt3d &a_pts) const {
  XM_ENSURE_TRUE_VOID_NO_ASSERT(!polyids.empty());

  // Make sure we have at least two values - an entrance and exit
  if (polyids.size() == 1) {
    // Only one intersection. Add an exit that is the same
    polyids.push_back(polyids.back());
    tvalues.push_back(tvalues.back());
    a_pts.push_back(a_pts.back());
  }

  // If crossing a vertex, make sure we have an exit
  if (m_d->m_polys2.empty() && polyids.back() != polyids[polyids.size() - 2]) {
    polyids.push_back(polyids.back());
    tvalues.push_back(tvalues.back());
    a_pts.push_back(a_pts.back());
  }

  // Make sure we always end with -1
  if (polyids.size() > 1) {
    polyids.back() = -1;
  }

  // If we leave and reenter, set exit to -1
  for (size_t i = 1; i < polyids.size(); ++i) {
    if (polyids[i] == polyids[i - 1]) {
      polyids[i] = -1;
    }
  }

} // GmMultiPolyIntersectionSorterTerse::FixArrays
//------------------------------------------------------------------------------
/// \brief Add the intersection info to the arrays.
/// \param[in] a_ix: Intersection info.
/// \param[in,out] polyids: Intersected polygons.
/// \param[in,out] tvalues: Percent distance (0.0 - 1.0) from beginning of line
///                         segment to each intersection.
/// \param[in,out] a_pts: Points of intersection.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectionSorterTerse::AddToPolyIdsAndTValues(
    const ix &a_ix, VecInt &polyids, VecDbl &tvalues,
    std::vector<Pt3d> &a_pts) const {
  polyids.push_back(a_ix.m_i);
  tvalues.push_back(a_ix.m_t);
  a_pts.push_back(a_ix.m_pt);
} // GmMultiPolyIntersectionSorterTerse::AddToPolyIdsAndTValues
//------------------------------------------------------------------------------
/// \brief Look at each poly in this set of t values and see if it exists
///        in the previous or next set of t values.
/// \param tChange
/// \param idx index
/// \param inPrev = indices into m_ix of intersections in this group that also
///               appear in the previous group.
/// \param inNext = indices into m_ix of intersections in this group that also
///               appear in the next group.
/// \param inNeither = indices into m_ix of intersections in this group that
/// don't
///               appear in the previous or next group.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectionSorterTerse::FindPreviousNextNeither(
    const VecInt &tChange, const int idx, VecInt *inPrev, VecInt *inNext,
    VecInt *inNeither) const {
  if (inPrev)
    inPrev->clear();
  if (inNext)
    inNext->clear();
  if (inNeither)
    inNeither->clear();
  for (int j = tChange[idx]; j < tChange[idx + 1]; ++j) {
    // Corners at the start of the list of intersections appear the same
    // as edges so we can't remove them because we don't have a previous
    // set of tvalues to look at.
    bool foundPrev = false;
    if (idx > 0) {
      for (int k = tChange[idx - 1]; k < tChange[idx] && !foundPrev; ++k) {
        if (m_d->m_ixs[j].m_i == m_d->m_ixs[k].m_i) {
          foundPrev = true;
        }
      }
    }

    // Corners at the end of the list of intersections appear the same
    // as edges so we can't remove them because we don't have a inNext set
    // of tvalues to look at.
    bool foundNext = false;
    if (!foundPrev && idx + 2 < (int)tChange.size()) {
      for (int k = tChange[idx + 1]; k < (int)tChange[idx + 2] && !foundNext;
           ++k) {
        if (m_d->m_ixs[j].m_i == m_d->m_ixs[k].m_i) {
          foundNext = true;
        }
      }
    }

    if (foundPrev) {
      if (inPrev)
        inPrev->push_back(j);
    } else if (foundNext) {
      if (inNext)
        inNext->push_back(j);
    } else {
      if (inNeither)
        inNeither->push_back(j);
    }
  }
} // GmMultiPolyIntersectionSorterTerse::FindPreviousNextNeither
//------------------------------------------------------------------------------
/// \brief Swap the order of the intersections a and b.
/// \param[in] a: An intersection.
/// \param[in] b: An intersection.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectionSorterTerse::Swap(int a, int b) {
  ix temp = m_d->m_ixs[a];
  m_d->m_ixs[a] = m_d->m_ixs[b];
  m_d->m_ixs[b] = temp;
} // GmMultiPolyIntersectionSorterTerse::Swap
//------------------------------------------------------------------------------
/// \brief Mark where we have a change in t values
/// \param tChange: Where tvalues change.
//------------------------------------------------------------------------------
void GmMultiPolyIntersectionSorterTerse::FindWhereTValuesChange(
    VecInt &tChange) const {
  tChange.assign(1, 0);
  for (size_t i = 1; i < m_d->m_ixs.size(); ++i) {
    if (!EQ_TOL(m_d->m_ixs[i].m_t, m_d->m_ixs[i - 1].m_t, m_tol)) {
      tChange.push_back((int)i);
    }
  }
  tChange.push_back((int)m_d->m_ixs.size()); // for convenience
} // GmMultiPolyIntersectionSorterTerse::FindWhereTValuesChange

} // namespace xms
