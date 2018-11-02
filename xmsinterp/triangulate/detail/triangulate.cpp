//------------------------------------------------------------------------------
/// \file
/// \brief Code that creates a Delauney triangulation from points.
/// \ingroup triangulate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Include Files ----------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/triangulate/detail/triangulate.h>

// 3. Standard library headers
#include <cmath>

// 4. External library headers

// 5. Shared code headers
#include <xmscore/misc/Progress.h>
#include <xmscore/misc/XmLog.h> // XM_LOG
#include <xmsinterp/triangulate/TrTriangulator.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

/// \cond
namespace
{
using namespace xms;
//----- Constants / Enumerations -----------------------------------------------

const int PLUS1MOD3[3] = {1, 2, 0};
const int MINUS1MOD3[3] = {2, 0, 1};

#define pointmark(pt, idx) ((int*)(pt))[idx]
#define setpointmark(pt, value, idx) ((int*)(pt))[idx] = value

// converts a ptr to an oriented triangle. The orientation is extracted from the
//   two least significant bits of the pointer
#define decode(ptr, tedge)                                                    \
  (tedge).orient = (int)((unsigned long long)(ptr) & (unsigned long long)3l); \
  (tedge).tri = (Ttri*)((unsigned long long)(ptr) ^ (unsigned long long)(tedge).orient)

// compresses an oriented triangle into a single ptr. Uses assumption that all
//   triangles are aligned to four-byte boundaries, so the two least significant
//   bits of (tedge).tri are 0
#define encode(tedge) (Ttri)((unsigned long long)(tedge).tri | (unsigned long long)(tedge).orient)

// finds the abutting tri on the same edge.  the edge direction is necessarily
//   reversed, since triangle/edge handles are always directed counterclockwise
//   around the triangle
#define sym(tedge1, tedge2)            \
  ptr = (tedge1).tri[(tedge1).orient]; \
  decode(ptr, tedge2);

#define symself(tedge)               \
  ptr = (tedge).tri[(tedge).orient]; \
  decode(ptr, tedge);

// finds the next edge (ccw) of a triangle
#define lnext(tedge1, tedge2)  \
  (tedge2).tri = (tedge1).tri; \
  (tedge2).orient = PLUS1MOD3[(tedge1).orient]

#define lnextself(tedge) (tedge).orient = PLUS1MOD3[(tedge).orient]

// finds the previous edge (cw) of a triangle
#define lprev(tedge1, tedge2)  \
  (tedge2).tri = (tedge1).tri; \
  (tedge2).orient = MINUS1MOD3[(tedge1).orient]

#define lprevself(tedge) (tedge).orient = MINUS1MOD3[(tedge).orient]

// These primitives determine or set the origin, destination, or apex of a
//   triangle
#define org(tedge, pointptr) pointptr = (Tpt)(tedge).tri[PLUS1MOD3[(tedge).orient] + 3]

#define dest(tedge, pointptr) pointptr = (Tpt)(tedge).tri[MINUS1MOD3[(tedge).orient] + 3]

#define apex(tedge, pointptr) pointptr = (Tpt)(tedge).tri[(tedge).orient + 3]

#define setorg(tedge, pointptr) (tedge).tri[PLUS1MOD3[(tedge).orient] + 3] = (Ttri)pointptr

#define setdest(tedge, pointptr) (tedge).tri[MINUS1MOD3[(tedge).orient] + 3] = (Ttri)pointptr

#define setapex(tedge, pointptr) (tedge).tri[(tedge).orient + 3] = (Ttri)pointptr

// Bond two triangles together
#define bond(tedge1, tedge2)                      \
  (tedge1).tri[(tedge1).orient] = encode(tedge2); \
  (tedge2).tri[(tedge2).orient] = encode(tedge1)

// Copy a triangle/edge handle
#define tedgecopy(tedge1, tedge2) \
  (tedge2).tri = (tedge1).tri;    \
  (tedge2).orient = (tedge1).orient

// Test for equality of triangle/edge handles
#define tedgeequal(tedge1, tedge2) \
  (((tedge1).tri == (tedge2).tri) && ((tedge1).orient == (tedge2).orient))

#define Fast_Two_Sum_Tail(a, b, x, y) \
  bvirt = x - a;                      \
  y = b - bvirt

#define Fast_Two_Sum(a, b, x, y) \
  x = (double)(a + b);           \
  Fast_Two_Sum_Tail(a, b, x, y)

#define Two_Sum_Tail(a, b, x, y) \
  bvirt = (double)(x - a);       \
  avirt = x - bvirt;             \
  bround = b - bvirt;            \
  around = a - avirt;            \
  y = around + bround

#define Two_Sum(a, b, x, y) \
  x = (double)(a + b);      \
  Two_Sum_Tail(a, b, x, y)

#define Two_Diff_Tail(a, b, x, y) \
  bvirt = (double)(a - x);        \
  avirt = x + bvirt;              \
  bround = bvirt - b;             \
  around = a - avirt;             \
  y = around + bround

#define Two_Diff(a, b, x, y) \
  x = (double)(a - b);       \
  Two_Diff_Tail(a, b, x, y)

#define Split(splitter, a, ahi, alo) \
  c = (double)(splitter * a);        \
  abig = (double)(c - a);            \
  ahi = c - abig;                    \
  alo = a - ahi

#define Two_Product_Tail(splitter, a, b, x, y) \
  Split(splitter, a, ahi, alo);                \
  Split(splitter, b, bhi, blo);                \
  err1 = x - (ahi * bhi);                      \
  err2 = err1 - (alo * bhi);                   \
  err3 = err2 - (ahi * blo);                   \
  y = (alo * blo) - err3

#define Two_Product(splitter, a, b, x, y) \
  x = (double)(a * b);                    \
  Two_Product_Tail(splitter, a, b, x, y)

// Two_Product_Presplit() is Two_Product() where one of the inputs has already
//   been split.  Avoids redundant splitting
#define Two_Product_Presplit(splitter, a, b, bhi, blo, x, y) \
  x = (double)(a * b);                                       \
  Split(splitter, a, ahi, alo);                              \
  err1 = x - (ahi * bhi);                                    \
  err2 = err1 - (alo * bhi);                                 \
  err3 = err2 - (ahi * blo);                                 \
  y = (alo * blo) - err3

#define Square_Tail(splitter, a, x, y) \
  Split(splitter, a, ahi, alo);        \
  err1 = x - (ahi * ahi);              \
  err3 = err1 - ((ahi + ahi) * alo);   \
  y = (alo * alo) - err3

// done more quickly than Two_Product()
#define Square(splitter, a, x, y) \
  x = (double)(a * a);            \
  Square_Tail(splitter, a, x, y)

#define Two_One_Sum(a1, a0, b, x2, x1, x0) \
  Two_Sum(a0, b, _i, x0);                  \
  Two_Sum(a1, _i, x2, x1)

#define Two_One_Diff(a1, a0, b, x2, x1, x0) \
  Two_Diff(a0, b, _i, x0);                  \
  Two_Sum(a1, _i, x2, x1)

#define Two_Two_Sum(a1, a0, b1, b0, x3, x2, x1, x0) \
  Two_One_Sum(a1, a0, b0, _j, _0, x0);              \
  Two_One_Sum(_j, _0, b1, x3, x2, x1)

#define Two_Two_Diff(a1, a0, b1, b0, x3, x2, x1, x0) \
  Two_One_Diff(a1, a0, b0, _j, _0, x0);              \
  Two_One_Diff(_j, _0, b1, x3, x2, x1)

//----- Typdefs / Classes ------------------------------------------------------

typedef double* Tpt;
typedef double** Ttri;

typedef struct Tpool* Tmemtype;
typedef struct Tedge* Tedgetype;

typedef struct Tedge
{
  Ttri* tri;
  int orient;
} Tedge;

typedef struct Tpool
{
  Tpool()
  : firstblock(nullptr)
  , nowblock(nullptr)
  , nextitem(nullptr)
  , deaditemstack(nullptr)
  , pathblock(nullptr)
  , pathitem(nullptr)
  {
  }
  int **firstblock, **nowblock;
  int *nextitem, *deaditemstack;
  int **pathblock, *pathitem;
  int alignbytes = 0;
  int itembytes = 0;
  int itemsperblock = 0;
  int itemsfirstblock = 0;
  long items = 0L;
  long maxitems = 0L;
  int unallocateditems = 0;
  int pathitemsleft = 0;
} Tpool;

// Label-signifies whether a record consists primarily of pointers or of
//   floating-point words. Used align data.
enum Twordtype { TPOINTER, TFLOATINGPOINT };

struct TriVars
{
public:
  Tpool m_triangles;
  Tpool m_points;

  Ttri* m_dummytri;
  Ttri* m_dummytribase; // Keep base address so can free it later

  int m_numpoints;      // Number of input points
  int m_pointmarkindex; // Index in pt of boundary marker
  // pt marker must be aligned to a sizeof(int)-byte address

  double m_resulterrbound;
  double m_ccwerrboundA, m_ccwerrboundB, m_ccwerrboundC;
  double m_iccerrboundA, m_iccerrboundB, m_iccerrboundC;

  double m_splitter;

private:
};

//----- Internal Function Prototypes -------------------------------------------

static void triAlternateAxes(Tpt*, int, int);
static double triCounterClockwise(Tpt, Tpt, Tpt, TriVars&);
static double triCounterClockwiseAdapt(Tpt, Tpt, Tpt, double, TriVars&);
static bool triDivConqDelaunay(TriVars&);
static bool triDivConqRecurse(Tpt*, int, int, Tedgetype, Tedgetype, TriVars&);
static void triDummyInit(int, TriVars&);
static double triEstimate(int, double*);
static void triExactInit(TriVars&);
static int triFastExpansionSumZeroElim(int, double*, int, double*, double*);
static void triFillTriList(TrTriangulator& a_Client, TriVars&);
static bool triGetPoints(TrTriangulator&, TriVars&);
static double triInCircle(Tpt, Tpt, Tpt, Tpt, TriVars&);
static double triInCircleAdapt(Tpt, Tpt, Tpt, Tpt, double, TriVars&);
static void triInitTrianglePool(TriVars&);
static bool triMakeTriangle(Tedgetype, TriVars&);
static bool triMergeHulls(Tedgetype, Tedgetype, Tedgetype, Tedgetype, int, TriVars&);
static void triNumberNodes(TriVars&);
static void triPointMedian(Tpt*, int, int, int);
static void triPointSort(Tpt*, int);
static Tpt triPointTraverse(TriVars&);
static int* triPoolAlloc(Tmemtype);
static void triPoolDealloc(Tmemtype, int*);
static void triPoolDeinit(Tmemtype);
static bool triPoolInit(Tmemtype, int, int, int, int);
static void triPoolRestart(Tmemtype);
static unsigned long triRandomnation(unsigned int);
static void triRemoveGhosts(Tedgetype, TriVars&);
static int triScaleExpansionZeroElim(int, double*, double, double*, TriVars&);
static void triTraversalInit(Tmemtype);
static int* triTraverse(Tmemtype);
static void triTriangleDealloc(Ttri*, TriVars&);
static Ttri* triTriangleTraverse(TriVars&);

//----- Internal Function Definitions ------------------------------------------

//------------------------------------------------------------------------------
// FUNCTION triAlternateAxes
/// \brief  Sorts the points as appropriate for the divide-and-conquer
//          algorithm with alternating cuts.
// NOTES
//  Partitions by x-coordinate if axis == 0; by y-coordinate if axis == 1.
//  For the base case, subsets containing only two or three points are
//  always sorted by x-coordinate.
//------------------------------------------------------------------------------
static void triAlternateAxes(Tpt* sortarray, int arraysize, int axis)
{
  int divider;
  divider = arraysize >> 1;
  if (arraysize <= 3)
  {
    /* Recursive base case:  subsets of two or three points will be      */
    /*   handled specially, and should always be sorted by x-coordinate. */
    axis = 0;
  }
  /* Partition w/ a horizontal or vertical cut */
  triPointMedian(sortarray, arraysize, divider, axis);
  /* Recursively partition subsets w/ a cross cut */
  if (arraysize - divider >= 2)
  {
    if (divider >= 2)
      triAlternateAxes(sortarray, divider, 1 - axis);
    triAlternateAxes(&sortarray[divider], arraysize - divider, 1 - axis);
  }
} // triAlternateAxes
//------------------------------------------------------------------------------
// FUNCTION triCounterClockwise
/// \brief
// NOTES
//------------------------------------------------------------------------------
static double triCounterClockwise(Tpt pa, Tpt pb, Tpt pc, TriVars& t)
{
  double detleft, detright, det, detsum, errbound;

  detleft = (pa[0] - pc[0]) * (pb[1] - pc[1]);
  detright = (pa[1] - pc[1]) * (pb[0] - pc[0]);
  det = detleft - detright;

  if (detleft > 0.0)
  {
    if (detright <= 0.0)
      return det;
    else
      detsum = detleft + detright;
  }
  else if (detleft < 0.0)
  {
    if (detright >= 0.0)
      return det;
    else
      detsum = -detleft - detright;
  }
  else
    return det;

  errbound = t.m_ccwerrboundA * detsum;
  if ((det >= errbound) || (-det >= errbound))
    return det;

  return triCounterClockwiseAdapt(pa, pb, pc, detsum, t);
} // triCounterClockwise
//------------------------------------------------------------------------------
// FUNCTION triCounterClockwiseAdapt
/// \brief  Return a positive value if the points pa, pb, and pc occur in
//          counterclockwise order; a negative value if they occur in clockwise
//          order; and zero if they are collinear.  The result is also a rough
//          approximation of twice the signed area of the triangle defined by
//          the three points.
// NOTES
//  Uses exact arithmetic if necessary to ensure a correct answer.  The
//  result returned is the determinant of a matrix.  This determinant is
//  computed adaptively, in the sense that exact arithmetic is used only to
//  the degree it is needed to ensure that the returned value has the
//  correct sign.  Hence, this function is usually quite fast, but will run
//  more slowly when the input points are collinear or nearly so.
//------------------------------------------------------------------------------
static double triCounterClockwiseAdapt(Tpt pa, Tpt pb, Tpt pc, double detsum, TriVars& t)
{
  int C1length, C2length, Dlength;
  double acx, acy, bcx, bcy;
  double acxtail, acytail, bcxtail, bcytail;
  double detleft, detright;
  double detlefttail, detrighttail;
  double det, errbound;
  double B[4], C1[8], C2[12], D[16];
  double B3;
  double u[4];
  double u3;
  double s1, t1, s0, t0;
  double bvirt, avirt, bround, around;
  double c, abig, ahi, alo, bhi, blo;
  double err1, err2, err3;
  double _i, _j, _0;

  acx = (double)(pa[0] - pc[0]);
  bcx = (double)(pb[0] - pc[0]);
  acy = (double)(pa[1] - pc[1]);
  bcy = (double)(pb[1] - pc[1]);

  Two_Product(t.m_splitter, acx, bcy, detleft, detlefttail);
  Two_Product(t.m_splitter, acy, bcx, detright, detrighttail);

  Two_Two_Diff(detleft, detlefttail, detright, detrighttail, B3, B[2], B[1], B[0]);
  B[3] = B3;

  det = triEstimate(4, B);
  errbound = t.m_ccwerrboundB * detsum;
  if ((det >= errbound) || (-det >= errbound))
    return det;

  Two_Diff_Tail(pa[0], pc[0], acx, acxtail);
  Two_Diff_Tail(pb[0], pc[0], bcx, bcxtail);
  Two_Diff_Tail(pa[1], pc[1], acy, acytail);
  Two_Diff_Tail(pb[1], pc[1], bcy, bcytail);

  if ((acxtail == 0.0) && (acytail == 0.0) && (bcxtail == 0.0) && (bcytail == 0.0))
  {
    return det;
  }

  errbound = t.m_ccwerrboundC * detsum + t.m_resulterrbound * fabs(det);
  det += (acx * bcytail + bcy * acxtail) - (acy * bcxtail + bcx * acytail);
  if ((det >= errbound) || (-det >= errbound))
    return det;

  Two_Product(t.m_splitter, acxtail, bcy, s1, s0);
  Two_Product(t.m_splitter, acytail, bcx, t1, t0);
  Two_Two_Diff(s1, s0, t1, t0, u3, u[2], u[1], u[0]);
  u[3] = u3;
  C1length = triFastExpansionSumZeroElim(4, B, 4, u, C1);

  Two_Product(t.m_splitter, acx, bcytail, s1, s0);
  Two_Product(t.m_splitter, acy, bcxtail, t1, t0);
  Two_Two_Diff(s1, s0, t1, t0, u3, u[2], u[1], u[0]);
  u[3] = u3;
  C2length = triFastExpansionSumZeroElim(C1length, C1, 4, u, C2);

  Two_Product(t.m_splitter, acxtail, bcytail, s1, s0);
  Two_Product(t.m_splitter, acytail, bcxtail, t1, t0);
  Two_Two_Diff(s1, s0, t1, t0, u3, u[2], u[1], u[0]);
  u[3] = u3;
  Dlength = triFastExpansionSumZeroElim(C2length, C2, 4, u, D);

  return (D[Dlength - 1]);
} // triCounterClockwiseAdapt
//------------------------------------------------------------------------------
// FUNCTION triDivConqDelaunay
/// \brief  Form a Delaunay triangulation by the divide-and-conquer method.
// NOTES    Sorts the points, calls a recursive procedure to triangulate them,
//          and removes the bounding box, setting boundary markers as
//          appropriate.
//------------------------------------------------------------------------------
static bool triDivConqDelaunay(TriVars& t)
{
  int divider, i, j;
  Tpt* sortarray;
  Tedge hullleft = {0}, hullright = {0};
  /* Allocate array of ptrs to pts for sorting. */
  int size = t.m_numpoints * (int)sizeof(Tpt);

  sortarray = (Tpt*)malloc((unsigned int)size);

  if (sortarray == nullptr)
  {
    std::string s = "malloc failed: ";
    s += __FUNCTION__;
    XM_LOG(xmlog::error, s);
    return false;
  }
  triTraversalInit(&t.m_points);
  for (i = 0; i < t.m_numpoints; i++)
  {
    sortarray[i] = triPointTraverse(t);
  }
  /* Sort the points. */
  triPointSort(sortarray, t.m_numpoints);
  /* Discard duplicate pts (can mess up algorithm) */
  i = 0;
  for (j = 1; j < t.m_numpoints; j++)
  {
    if ((sortarray[i][0] == sortarray[j][0] && sortarray[i][1] == sortarray[j][1]))
    {
      // setpointmark(sortarray[j], -999);
    }
    else
    {
      i++;
      sortarray[i] = sortarray[j];
    }
  }
  i++;
  /* Re-sort pts to accommodate alternating cuts */
  divider = i >> 1;
  // if divider is 0, this causes problems
  if (divider == 0)
  {
    free(sortarray);
    return false;
  }
  else if (i - divider >= 2)
  {
    if (divider >= 2)
    {
      triAlternateAxes(sortarray, divider, 1);
    }
    triAlternateAxes(&sortarray[divider], i - divider, 1);
  }
  /* Form the Delaunay triangulation. */
  bool canTriangulate = triDivConqRecurse(sortarray, i, 0, &hullleft, &hullright, t);
  if (!canTriangulate)
  {
    std::string str =
      "Triangulation cannot be completed due to memory constraints. "
      "A suggested work-around would be to divide the scattersets, "
      "triangulate them seperately, then merge back into one scatterset.  "
      "It is suggested that the scatter set be broken into contiguous regions "
      "of less than 5000000 points.";
    XM_LOG(xmlog::error, str);
    triPoolDeinit(&t.m_triangles); // clear the triangles before being added
    free(sortarray);
    if (t.m_dummytribase)
    {
      free(t.m_dummytribase);
    }
    return false;
  }
  free(sortarray);

  triRemoveGhosts(&hullleft, t);
  return true;
} // triDivConqDelaunay
//------------------------------------------------------------------------------
// FUNCTION triDivConqRecurse
/// \brief  Recursively form a Delaunay triangulation by the divide-and-conquer
//          method.
// NOTES
//  Recursively breaks down the problem into smaller pieces, which are
//  knitted together by triMergeHulls().  The base cases (problems of two or
//  three points) are handled specially here.
//
//  On completion, `farleft' and `farright' are bounding triangles such that
//  the origin of `farleft' is the leftmost vertex (breaking ties by
//  choosing the highest leftmost vertex), and the destination of
//  `farright' is the rightmost vertex (breaking ties by choosing the
//  lowest rightmost vertex).
//------------------------------------------------------------------------------
static bool triDivConqRecurse(Tpt* sortarray,
                              int vertices,
                              int axis,
                              Tedgetype farleft,
                              Tedgetype farright,
                              TriVars& t)
{
  int divider;
  double area;
  Tedge midtri, tri1, tri2, tri3, innerleft, innerright;
  bool canMakeTriangles = true;

  if (vertices == 2)
  {
    /* Triangulation of two vertices is an edge. *
     * (represented by two bounding triangles)   */
    canMakeTriangles &= triMakeTriangle(farleft, t);
    canMakeTriangles &= triMakeTriangle(farright, t);
    if (!canMakeTriangles) // past 32-bit limit
    {
      return false;
    }
    setorg(*farleft, sortarray[0]);
    setdest(*farleft, sortarray[1]);
    /* The apex is intentionally left NULL. */
    setorg(*farright, sortarray[1]);
    setdest(*farright, sortarray[0]);
    /* The apex is intentionally left NULL. */
    bond(*farleft, *farright);
    lprevself(*farleft);
    lnextself(*farright);
    bond(*farleft, *farright);
    lprevself(*farleft);
    lnextself(*farright);
    bond(*farleft, *farright);
    /* Ensure origin of `farleft' is sortarray[0] */
    lprev(*farright, *farleft);

    return true;
  }
  else if (vertices == 3)
  {
    /* Triangulation of three vertices is either: *
     * a triangle (with three bounding triangles) *
     * two edges (with four bounding triangles).  *
     * In either case, four triangles are created */
    canMakeTriangles &= triMakeTriangle(&midtri, t);
    canMakeTriangles &= triMakeTriangle(&tri1, t);
    canMakeTriangles &= triMakeTriangle(&tri2, t);
    canMakeTriangles &= triMakeTriangle(&tri3, t);
    if (!canMakeTriangles) // past 32-bit limit
    {
      return false;
    }
    area = triCounterClockwise(sortarray[0], sortarray[1], sortarray[2], t);
    if (area == 0.0)
    {
      /* Three collinear points -> two edges */
      setorg(midtri, sortarray[0]);
      setdest(midtri, sortarray[1]);
      setorg(tri1, sortarray[1]);
      setdest(tri1, sortarray[0]);
      setorg(tri2, sortarray[2]);
      setdest(tri2, sortarray[1]);
      setorg(tri3, sortarray[1]);
      setdest(tri3, sortarray[2]);
      /* All apices are intentionally left NULL. */
      bond(midtri, tri1);
      bond(tri2, tri3);
      lnextself(midtri);
      lprevself(tri1);
      lnextself(tri2);
      lprevself(tri3);
      bond(midtri, tri3);
      bond(tri1, tri2);
      lnextself(midtri);
      lprevself(tri1);
      lnextself(tri2);
      lprevself(tri3);
      bond(midtri, tri1);
      bond(tri2, tri3);
      /* Ensure origin of `farleft' is sortarray[0]. */
      tedgecopy(tri1, *farleft);
      /* Ensure dest of `farright' is sortarray[2]. */
      tedgecopy(tri2, *farright);
    }
    else
    {
      /* Three noncollinear pts; 1 triangle (midtri) */
      setorg(midtri, sortarray[0]);
      setdest(tri1, sortarray[0]);
      setorg(tri3, sortarray[0]);
      /* Apices of tri1, tri2, & tri3 are left NULL. */
      if (area > 0.0)
      {
        /* The vertices are in ccw order. */
        setdest(midtri, sortarray[1]);
        setorg(tri1, sortarray[1]);
        setdest(tri2, sortarray[1]);
        setapex(midtri, sortarray[2]);
        setorg(tri2, sortarray[2]);
        setdest(tri3, sortarray[2]);
      }
      else
      {
        /* The vertices are in cw order. */
        setdest(midtri, sortarray[2]);
        setorg(tri1, sortarray[2]);
        setdest(tri2, sortarray[2]);
        setapex(midtri, sortarray[1]);
        setorg(tri2, sortarray[1]);
        setdest(tri3, sortarray[1]);
      }
      /* Topology does not depend on vertex ordering */
      bond(midtri, tri1);
      lnextself(midtri);
      bond(midtri, tri2);
      lnextself(midtri);
      bond(midtri, tri3);
      lprevself(tri1);
      lnextself(tri2);
      bond(tri1, tri2);
      lprevself(tri1);
      lprevself(tri3);
      bond(tri1, tri3);
      lnextself(tri2);
      lprevself(tri3);
      bond(tri2, tri3);
      /* Ensure origin of `farleft' is sortarray[0]. */
      tedgecopy(tri1, *farleft);
      /* Ensure dest of `farright' is sortarray[2]. */
      if (area > 0.0)
      {
        tedgecopy(tri2, *farright);
      }
      else
      {
        lnext(*farleft, *farright);
      }
    }
    return true;
  }
  else
  {
    /* Split the vertices in half. */
    divider = vertices >> 1;
    /* Recursively triangulate each half. */
    canMakeTriangles = triDivConqRecurse(sortarray, divider, 1 - axis, farleft, &innerleft, t);
    if (!canMakeTriangles)
    {
      return false;
    }
    canMakeTriangles = triDivConqRecurse(&sortarray[divider], vertices - divider, 1 - axis,
                                         &innerright, farright, t);
    if (!canMakeTriangles)
    {
      return false;
    }
    /* Merge the two triangulations into one. */
    canMakeTriangles = triMergeHulls(farleft, &innerleft, &innerright, farright, axis, t);
    if (canMakeTriangles)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
} // triDivConqRecurse
//------------------------------------------------------------------------------
// FUNCTION triDummyInit
/// \brief  Initialize the triangle that fills "outer space" and the
//          omnipresent shell edge.
// NOTES
//  The triangle that fills "outer space", called `dummytri', is pointed to
//  by every triangle and shell edge on a boundary (be it outer or inner) of
//  the triangulation.  Also, `dummytri' points to one of the triangles on
//  the convex hull (until the concavities are carved), making it
//  possible to find a starting triangle for point location.
// `trianglewords' is used by the mesh manipulation primitives
//   to extract orientations of triangles and shell edges from pointers.
//------------------------------------------------------------------------------
static void triDummyInit(int trianglewords, TriVars& t)
{
  unsigned long long alignptr;
  /* Set up `dummytri', (occupies "outer space") */
  t.m_dummytribase = (Ttri*)malloc(trianglewords * sizeof(Ttri) + t.m_triangles.alignbytes);
  if (t.m_dummytribase == nullptr)
  {
    std::string s = "malloc failed: ";
    s += __FUNCTION__;
    XM_LOG(xmlog::error, s);
    return;
  }
  /* Align `dummytri' on a
   * `triangles.alignbytes'-byte boundary. */
  alignptr = (unsigned long long)t.m_dummytribase;
  t.m_dummytri = (Ttri*)(alignptr + (unsigned long long)t.m_triangles.alignbytes -
                         (alignptr % (unsigned long long)t.m_triangles.alignbytes));
  /* Initialize the three adjoining triangles to be "outer space".  These  */
  /*   will eventually be changed by various bonding operations, but their */
  /*   values don't really matter, as long as they can legally be          */
  /*   dereferenced.                                                       */
  t.m_dummytri[0] = (Ttri)t.m_dummytri;
  t.m_dummytri[1] = (Ttri)t.m_dummytri;
  t.m_dummytri[2] = (Ttri)t.m_dummytri;
  /* Three NULL vertex points. */
  t.m_dummytri[3] = nullptr;
  t.m_dummytri[4] = nullptr;
  t.m_dummytri[5] = nullptr;
} // triDummyInit
//------------------------------------------------------------------------------
// FUNCTION triEstimate
/// \brief  Produce a one-word estimate of an expansion's value.
// NOTES
//------------------------------------------------------------------------------
static double triEstimate(int elen, double* e)
{
  int eindex;
  double Q;

  Q = e[0];
  for (eindex = 1; eindex < elen; eindex++)
    Q += e[eindex];

  return Q;
} // triEstimate
//------------------------------------------------------------------------------
// FUNCTION  triExactInit
/// \brief   Initialize the variables used for exact arithmetic.
// NOTES
//  `epsilon' is the largest power of two such that 1.0 + epsilon = 1.0 in
//  floating-point arithmetic.  `epsilon' bounds the relative roundoff
//  error.  It is used for floating-point error analysis.
//
//  `splitter' is used to split floating-point numbers into two half-
//  length significands for exact multiplication.
//
//  I imagine that a highly optimizing compiler might be too smart for its
//  own good, and somehow cause this routine to fail, if it pretends that
//  floating-point arithmetic is too much like real arithmetic.
//
//  Don't change this routine unless you fully understand it.
//------------------------------------------------------------------------------
static void triExactInit(TriVars& t)
{
  double half, check, lastcheck, epsilon;
  int every_other;
  /* initialize variables */
  t.m_splitter = 1.0; // AKZ - I added this initialization
  every_other = 1;
  half = 0.5;
  epsilon = check = 1.0;
  /* Repeatedly divide `epsilon' by two until it is too small to add to      */
  /*   one without causing roundoff.  (Also check if the sum is equal to     */
  /*   the previous sum, for machines that round up instead of using exact   */
  /*   rounding.  Not that these routines will work on such machines anyway. */
  do
  {
    lastcheck = check;
    epsilon *= half;
    if (every_other)
      t.m_splitter *= 2.0;
    every_other = !every_other;
    check = 1.0 + epsilon;
  } while ((check != 1.0) && (check != lastcheck));
  t.m_splitter += 1.0;
  /* Error bounds for orientation & incircle tests */
  t.m_resulterrbound = (3.0 + 8.0 * epsilon) * epsilon;
  t.m_ccwerrboundA = (3.0 + 16.0 * epsilon) * epsilon;
  t.m_ccwerrboundB = (2.0 + 12.0 * epsilon) * epsilon;
  t.m_ccwerrboundC = (9.0 + 64.0 * epsilon) * epsilon * epsilon;
  t.m_iccerrboundA = (10.0 + 96.0 * epsilon) * epsilon;
  t.m_iccerrboundB = (4.0 + 48.0 * epsilon) * epsilon;
  t.m_iccerrboundC = (44.0 + 576.0 * epsilon) * epsilon * epsilon;
} // triExactInit
//------------------------------------------------------------------------------
// FUNCTION triFastExpansionSumZeroElim
/// \brief  Sum two expansions, eliminating zero components from the output
//          expansion.
// NOTES
//  Sets h = e + f.
//  If round-to-even is used (as with IEEE 754), maintains the strongly
//  nonoverlapping property.  (That is, if e is strongly nonoverlapping, h
//  will be also.)  Does NOT maintain the nonoverlapping or nonadjacent
//  properties.  h cannot be e or f.
//------------------------------------------------------------------------------
static int triFastExpansionSumZeroElim(int elen, double* e, int flen, double* f, double* h)
{
  int eindex, findex, hindex;
  double Q, Qnew, hh, bvirt, avirt, bround, around, enow, fnow;
  /* initialize Q */
  enow = e[0];
  fnow = f[0];
  eindex = findex = 0;
  if ((fnow > enow) == (fnow > -enow))
  {
    Q = enow;
    enow = e[++eindex];
  }
  else
  {
    Q = fnow;
    fnow = f[++findex];
  }
  hindex = 0;
  if ((eindex < elen) && (findex < flen))
  {
    if ((fnow > enow) == (fnow > -enow))
    {
      Fast_Two_Sum(enow, Q, Qnew, hh);
      enow = e[++eindex];
    }
    else
    {
      Fast_Two_Sum(fnow, Q, Qnew, hh);
      fnow = f[++findex];
    }
    Q = Qnew;
    if (hh != 0.0)
      h[hindex++] = hh;
    while ((eindex < elen) && (findex < flen))
    {
      if ((fnow > enow) == (fnow > -enow))
      {
        Two_Sum(Q, enow, Qnew, hh);
        ++eindex;
        if (eindex < elen)
          enow = e[eindex];
        else
          enow = 0.0;
      }
      else
      {
        Two_Sum(Q, fnow, Qnew, hh);
        ++findex;
        if (findex < flen)
          fnow = f[findex];
        else
          fnow = 0.0;
      }
      Q = Qnew;
      if (hh != 0.0)
        h[hindex++] = hh;
    }
  }
  while (eindex < elen)
  {
    Two_Sum(Q, enow, Qnew, hh);
    enow = e[++eindex];
    Q = Qnew;
    if (hh != 0.0)
      h[hindex++] = hh;
  }
  while (findex < flen)
  {
    Two_Sum(Q, fnow, Qnew, hh);
    fnow = f[++findex];
    Q = Qnew;
    if (hh != 0.0)
      h[hindex++] = hh;
  }
  if ((Q != 0.0) || (hindex == 0))
    h[hindex++] = Q;
  return hindex;
} // triFastExpansionSumZeroElim
//------------------------------------------------------------------------------
// FUNCTION  triFillTriList
/// \brief
// NOTES
//------------------------------------------------------------------------------
static void triFillTriList(TrTriangulator& a_Client, TriVars& t)
{
  int id1, id2, id3;
  Tedge triangleloop;
  Tpt p1, p2, p3;

  Progress prog("Triangulating Points");
  // prepare to recieve trianlges
  a_Client.PrepareToReceiveTriangles();
  double triCount(0.0);
  double total = static_cast<double>(a_Client.GetNPoints());

  triTraversalInit(&t.m_triangles);
  triangleloop.tri = triTriangleTraverse(t);
  triangleloop.orient = 0;
  while (triangleloop.tri != nullptr)
  {
    // get the ids
    org(triangleloop, p1);
    dest(triangleloop, p2);
    apex(triangleloop, p3);
    id1 = pointmark(p1, t.m_pointmarkindex);
    id2 = pointmark(p2, t.m_pointmarkindex);
    id3 = pointmark(p3, t.m_pointmarkindex);

    // recieve the triangle
    a_Client.ReceiveTriangle(id1, id2, id3);
    triCount += 2.0;
    prog.ProgressStatus(std::min(1.0, triCount / total));

    triangleloop.tri = triTriangleTraverse(t);
  }
} // triFillTriList
//------------------------------------------------------------------------------
// FUNCTION  triGetPoints
/// \brief
// NOTES
//------------------------------------------------------------------------------
static bool triGetPoints(TrTriangulator& a_Client, TriVars& t)
{
#define POINTPERBLOCK 4092

  int i, pointsize;
  Pt3d loc;
  Tpt pointloop;

  // number of points allocated at once
  t.m_pointmarkindex = (3 * sizeof(double) + sizeof(int) - 1) / sizeof(int);
  pointsize = (t.m_pointmarkindex + 1) * sizeof(int);

  // initialize the pool of points
  // get the number of points
  t.m_numpoints = a_Client.GetNPoints();

  if (!triPoolInit(&t.m_points, pointsize, POINTPERBLOCK,
                   t.m_numpoints > POINTPERBLOCK ? t.m_numpoints : POINTPERBLOCK, 0))
  {
    return false;
  }

  // get the points
  for (i = 0; i < t.m_numpoints; i++)
  {
    pointloop = (Tpt)triPoolAlloc(&t.m_points);

    // set the location
    loc = a_Client.GetLocation();
    pointloop[0] = loc.x;
    pointloop[1] = loc.y;

    // save id for pt (used after triangulation)
    pointloop[2] = a_Client.GetID();

    setpointmark(pointloop, 0, t.m_pointmarkindex);

    // go to the next point
    a_Client.IncrementPoint();
  }
  return true;
} // triGetPoints
//------------------------------------------------------------------------------
// FUNCTION triInCircle
/// \brief
// NOTES
//------------------------------------------------------------------------------
static double triInCircle(Tpt pa, Tpt pb, Tpt pc, Tpt pd, TriVars& t)
{
  double adx, bdx, cdx, ady, bdy, cdy;
  double bdxcdy, cdxbdy, cdxady, adxcdy, adxbdy, bdxady;
  double alift, blift, clift;
  double det;
  double permanent, errbound;

  adx = pa[0] - pd[0];
  bdx = pb[0] - pd[0];
  cdx = pc[0] - pd[0];
  ady = pa[1] - pd[1];
  bdy = pb[1] - pd[1];
  cdy = pc[1] - pd[1];

  bdxcdy = bdx * cdy;
  cdxbdy = cdx * bdy;
  alift = adx * adx + ady * ady;

  cdxady = cdx * ady;
  adxcdy = adx * cdy;
  blift = bdx * bdx + bdy * bdy;

  adxbdy = adx * bdy;
  bdxady = bdx * ady;
  clift = cdx * cdx + cdy * cdy;

  det = alift * (bdxcdy - cdxbdy) + blift * (cdxady - adxcdy) + clift * (adxbdy - bdxady);

  permanent = (fabs(bdxcdy) + fabs(cdxbdy)) * alift + (fabs(cdxady) + fabs(adxcdy)) * blift +
              (fabs(adxbdy) + fabs(bdxady)) * clift;
  errbound = t.m_iccerrboundA * permanent;
  if ((det > errbound) || (-det > errbound))
    return det;

  return triInCircleAdapt(pa, pb, pc, pd, permanent, t);
} // triInCircle
//------------------------------------------------------------------------------
// FUNCTION triInCircleAdapt
/// \brief  Return a positive value if the point pd lies inside the circle
//          passing through pa, pb, and pc; a negative value if it lies
//          outside; and zero if the four points are cocircular. The points pa,
//          pb, and pc must be in counterclockwise order, or the sign of the
//          result will be reversed.
// NOTES    Uses exact arithmetic if necessary to ensure a correct answer.  The
//  result returned is the determinant of a matrix.  This determinant is
//  computed adaptively, in the sense that exact arithmetic is used only to
//  the degree it is needed to ensure that the returned value has the
//  correct sign.  Hence, this function is usually quite fast, but will run
//  more slowly when the input points are cocircular or nearly so.
//------------------------------------------------------------------------------
static double triInCircleAdapt(Tpt pa, Tpt pb, Tpt pc, Tpt pd, double permanent, TriVars& t)
{
  int axbclen, axxbclen, aybclen, ayybclen, alen;
  int bxcalen, bxxcalen, bycalen, byycalen, blen;
  int cxablen, cxxablen, cyablen, cyyablen, clen;
  int ablen;
  int finlength;
  double adx, bdx, cdx, ady, bdy, cdy;
  double det, errbound;
  double bdxcdy1, cdxbdy1, cdxady1, adxcdy1, adxbdy1, bdxady1;
  double bdxcdy0, cdxbdy0, cdxady0, adxcdy0, adxbdy0, bdxady0;
  double bc[4], ca[4], ab[4];
  double bc3, ca3, ab3;
  double axbc[8], axxbc[16], aybc[8], ayybc[16], adet[32];
  double bxca[8], bxxca[16], byca[8], byyca[16], bdet[32];
  double cxab[8], cxxab[16], cyab[8], cyyab[16], cdet[32];
  double abdet[64];
  double fin1[1152], fin2[1152];
  double *finnow, *finother, *finswap;

  int temp8len, temp16alen, temp16blen, temp16clen;
  int temp32alen, temp32blen, temp48len, temp64len;
  int axtbblen, axtcclen, aytbblen, aytcclen;
  int bxtaalen, bxtcclen, bytaalen, bytcclen;
  int cxtaalen, cxtbblen, cytaalen, cytbblen;
  int axtbclen = 0, aytbclen = 0, bxtcalen = 0;
  int bytcalen = 0, cxtablen = 0, cytablen = 0;
  int axtbctlen, bxtcatlen, cxtabtlen;
  int aytbctlen, bytcatlen, cytabtlen;
  int axtbcttlen, bxtcattlen, cxtabttlen;
  int aytbcttlen, bytcattlen, cytabttlen;
  int abtlen, bctlen, catlen;
  int abttlen, bcttlen, cattlen;
  double adxtail, bdxtail, cdxtail, adytail, bdytail, cdytail;
  double adxadx1, adyady1, bdxbdx1, bdybdy1, cdxcdx1, cdycdy1;
  double adxadx0, adyady0, bdxbdx0, bdybdy0, cdxcdx0, cdycdy0;
  double aa[4], bb[4], cc[4];
  double aa3, bb3, cc3;
  double ti1, tj1;
  double ti0, tj0;
  double u[4], v[4];
  double u3, v3;
  double temp8[8], temp16a[16], temp16b[16], temp16c[16];
  double temp32a[32], temp32b[32], temp48[48], temp64[64];
  double axtbb[8], axtcc[8], aytbb[8], aytcc[8];
  double bxtaa[8], bxtcc[8], bytaa[8], bytcc[8];
  double cxtaa[8], cxtbb[8], cytaa[8], cytbb[8];
  double axtbc[8], aytbc[8], bxtca[8], bytca[8], cxtab[8], cytab[8];
  double axtbct[16], bxtcat[16], cxtabt[16];
  double aytbct[16], bytcat[16], cytabt[16];
  double axtbctt[8], aytbctt[8], bxtcatt[8];
  double bytcatt[8], cxtabtt[8], cytabtt[8];
  double abt[8], bct[8], cat[8];
  double abtt[4], bctt[4], catt[4];
  double abtt3, bctt3, catt3;
  double negate;

  double bvirt;
  double avirt, bround, around;
  double c;
  double abig, ahi, alo, bhi, blo;
  double err1, err2, err3;
  double _i, _j, _0;

  // Initialize all memory
  memset(bc, 0, 4 * sizeof(double));
  memset(ca, 0, 4 * sizeof(double));
  memset(ab, 0, 4 * sizeof(double));

  memset(axbc, 0, 8 * sizeof(double));
  memset(axxbc, 0, 16 * sizeof(double));
  memset(aybc, 0, 8 * sizeof(double));
  memset(ayybc, 0, 16 * sizeof(double));
  memset(adet, 0, 32 * sizeof(double));

  memset(bxca, 0, 8 * sizeof(double));
  memset(bxxca, 0, 16 * sizeof(double));
  memset(byca, 0, 8 * sizeof(double));
  memset(byyca, 0, 16 * sizeof(double));
  memset(bdet, 0, 32 * sizeof(double));

  memset(cxab, 0, 8 * sizeof(double));
  memset(cxxab, 0, 16 * sizeof(double));
  memset(cyab, 0, 8 * sizeof(double));
  memset(cyyab, 0, 16 * sizeof(double));
  memset(cdet, 0, 32 * sizeof(double));

  memset(abdet, 0, 64 * sizeof(double));
  memset(fin1, 0, 1152 * sizeof(double));
  memset(fin2, 0, 1152 * sizeof(double));

  memset(aa, 0, 4 * sizeof(double));
  memset(bb, 0, 4 * sizeof(double));
  memset(cc, 0, 4 * sizeof(double));

  memset(u, 0, 4 * sizeof(double));
  memset(v, 0, 4 * sizeof(double));

  memset(temp8, 0, 8 * sizeof(double));
  memset(temp16a, 0, 16 * sizeof(double));
  memset(temp16b, 0, 16 * sizeof(double));
  memset(temp16c, 0, 16 * sizeof(double));

  memset(temp32a, 0, 32 * sizeof(double));
  memset(temp32b, 0, 32 * sizeof(double));
  memset(temp48, 0, 48 * sizeof(double));
  memset(temp64, 0, 64 * sizeof(double));

  memset(axtbb, 0, 8 * sizeof(double));
  memset(axtcc, 0, 8 * sizeof(double));
  memset(aytbb, 0, 8 * sizeof(double));
  memset(aytcc, 0, 8 * sizeof(double));

  memset(bxtaa, 0, 8 * sizeof(double));
  memset(bxtcc, 0, 8 * sizeof(double));
  memset(bytaa, 0, 8 * sizeof(double));
  memset(bytcc, 0, 8 * sizeof(double));

  memset(cxtaa, 0, 8 * sizeof(double));
  memset(cxtbb, 0, 8 * sizeof(double));
  memset(cytaa, 0, 8 * sizeof(double));
  memset(cytbb, 0, 8 * sizeof(double));

  memset(axtbc, 0, 8 * sizeof(double));
  memset(aytbc, 0, 8 * sizeof(double));
  memset(bxtca, 0, 8 * sizeof(double));
  memset(bytca, 0, 8 * sizeof(double));
  memset(cxtab, 0, 8 * sizeof(double));
  memset(cytab, 0, 8 * sizeof(double));

  memset(axtbct, 0, 16 * sizeof(double));
  memset(bxtcat, 0, 16 * sizeof(double));
  memset(cxtabt, 0, 16 * sizeof(double));

  memset(aytbct, 0, 16 * sizeof(double));
  memset(bytcat, 0, 16 * sizeof(double));
  memset(cytabt, 0, 16 * sizeof(double));

  memset(axtbctt, 0, 8 * sizeof(double));
  memset(aytbctt, 0, 8 * sizeof(double));
  memset(bxtcatt, 0, 8 * sizeof(double));

  memset(bytcatt, 0, 8 * sizeof(double));
  memset(cxtabtt, 0, 8 * sizeof(double));
  memset(cytabtt, 0, 8 * sizeof(double));

  memset(abt, 0, 8 * sizeof(double));
  memset(bct, 0, 8 * sizeof(double));
  memset(cat, 0, 8 * sizeof(double));

  memset(abtt, 0, 4 * sizeof(double));
  memset(bctt, 0, 4 * sizeof(double));
  memset(catt, 0, 4 * sizeof(double));

  adx = (double)(pa[0] - pd[0]);
  bdx = (double)(pb[0] - pd[0]);
  cdx = (double)(pc[0] - pd[0]);
  ady = (double)(pa[1] - pd[1]);
  bdy = (double)(pb[1] - pd[1]);
  cdy = (double)(pc[1] - pd[1]);

  Two_Product(t.m_splitter, bdx, cdy, bdxcdy1, bdxcdy0);
  Two_Product(t.m_splitter, cdx, bdy, cdxbdy1, cdxbdy0);
  Two_Two_Diff(bdxcdy1, bdxcdy0, cdxbdy1, cdxbdy0, bc3, bc[2], bc[1], bc[0]);
  bc[3] = bc3;
  axbclen = triScaleExpansionZeroElim(4, bc, adx, axbc, t);
  axxbclen = triScaleExpansionZeroElim(axbclen, axbc, adx, axxbc, t);
  aybclen = triScaleExpansionZeroElim(4, bc, ady, aybc, t);
  ayybclen = triScaleExpansionZeroElim(aybclen, aybc, ady, ayybc, t);
  alen = triFastExpansionSumZeroElim(axxbclen, axxbc, ayybclen, ayybc, adet);

  Two_Product(t.m_splitter, cdx, ady, cdxady1, cdxady0);
  Two_Product(t.m_splitter, adx, cdy, adxcdy1, adxcdy0);
  Two_Two_Diff(cdxady1, cdxady0, adxcdy1, adxcdy0, ca3, ca[2], ca[1], ca[0]);
  ca[3] = ca3;
  bxcalen = triScaleExpansionZeroElim(4, ca, bdx, bxca, t);
  bxxcalen = triScaleExpansionZeroElim(bxcalen, bxca, bdx, bxxca, t);
  bycalen = triScaleExpansionZeroElim(4, ca, bdy, byca, t);
  byycalen = triScaleExpansionZeroElim(bycalen, byca, bdy, byyca, t);
  blen = triFastExpansionSumZeroElim(bxxcalen, bxxca, byycalen, byyca, bdet);

  Two_Product(t.m_splitter, adx, bdy, adxbdy1, adxbdy0);
  Two_Product(t.m_splitter, bdx, ady, bdxady1, bdxady0);
  Two_Two_Diff(adxbdy1, adxbdy0, bdxady1, bdxady0, ab3, ab[2], ab[1], ab[0]);
  ab[3] = ab3;
  cxablen = triScaleExpansionZeroElim(4, ab, cdx, cxab, t);
  cxxablen = triScaleExpansionZeroElim(cxablen, cxab, cdx, cxxab, t);
  cyablen = triScaleExpansionZeroElim(4, ab, cdy, cyab, t);
  cyyablen = triScaleExpansionZeroElim(cyablen, cyab, cdy, cyyab, t);
  clen = triFastExpansionSumZeroElim(cxxablen, cxxab, cyyablen, cyyab, cdet);

  ablen = triFastExpansionSumZeroElim(alen, adet, blen, bdet, abdet);
  finlength = triFastExpansionSumZeroElim(ablen, abdet, clen, cdet, fin1);

  det = triEstimate(finlength, fin1);
  errbound = t.m_iccerrboundB * permanent;
  if ((det >= errbound) || (-det >= errbound))
    return det;

  Two_Diff_Tail(pa[0], pd[0], adx, adxtail);
  Two_Diff_Tail(pa[1], pd[1], ady, adytail);
  Two_Diff_Tail(pb[0], pd[0], bdx, bdxtail);
  Two_Diff_Tail(pb[1], pd[1], bdy, bdytail);
  Two_Diff_Tail(pc[0], pd[0], cdx, cdxtail);
  Two_Diff_Tail(pc[1], pd[1], cdy, cdytail);
  if ((adxtail == 0.0) && (bdxtail == 0.0) && (cdxtail == 0.0) && (adytail == 0.0) &&
      (bdytail == 0.0) && (cdytail == 0.0))
  {
    return det;
  }

  errbound = t.m_iccerrboundC * permanent + t.m_resulterrbound * fabs(det);
  det +=
    ((adx * adx + ady * ady) * ((bdx * cdytail + cdy * bdxtail) - (bdy * cdxtail + cdx * bdytail)) +
     2.0 * (adx * adxtail + ady * adytail) * (bdx * cdy - bdy * cdx)) +
    ((bdx * bdx + bdy * bdy) * ((cdx * adytail + ady * cdxtail) - (cdy * adxtail + adx * cdytail)) +
     2.0 * (bdx * bdxtail + bdy * bdytail) * (cdx * ady - cdy * adx)) +
    ((cdx * cdx + cdy * cdy) * ((adx * bdytail + bdy * adxtail) - (ady * bdxtail + bdx * adytail)) +
     2.0 * (cdx * cdxtail + cdy * cdytail) * (adx * bdy - ady * bdx));
  if ((det >= errbound) || (-det >= errbound))
    return det;

  finnow = fin1;
  finother = fin2;

  if ((bdxtail != 0.0) || (bdytail != 0.0) || (cdxtail != 0.0) || (cdytail != 0.0))
  {
    Square(t.m_splitter, adx, adxadx1, adxadx0);
    Square(t.m_splitter, ady, adyady1, adyady0);
    Two_Two_Sum(adxadx1, adxadx0, adyady1, adyady0, aa3, aa[2], aa[1], aa[0]);
    aa[3] = aa3;
  }
  if ((cdxtail != 0.0) || (cdytail != 0.0) || (adxtail != 0.0) || (adytail != 0.0))
  {
    Square(t.m_splitter, bdx, bdxbdx1, bdxbdx0);
    Square(t.m_splitter, bdy, bdybdy1, bdybdy0);
    Two_Two_Sum(bdxbdx1, bdxbdx0, bdybdy1, bdybdy0, bb3, bb[2], bb[1], bb[0]);
    bb[3] = bb3;
  }
  if ((adxtail != 0.0) || (adytail != 0.0) || (bdxtail != 0.0) || (bdytail != 0.0))
  {
    Square(t.m_splitter, cdx, cdxcdx1, cdxcdx0);
    Square(t.m_splitter, cdy, cdycdy1, cdycdy0);
    Two_Two_Sum(cdxcdx1, cdxcdx0, cdycdy1, cdycdy0, cc3, cc[2], cc[1], cc[0]);
    cc[3] = cc3;
  }

  if (adxtail != 0.0)
  {
    axtbclen = triScaleExpansionZeroElim(4, bc, adxtail, axtbc, t);
    temp16alen = triScaleExpansionZeroElim(axtbclen, axtbc, 2.0 * adx, temp16a, t);

    axtcclen = triScaleExpansionZeroElim(4, cc, adxtail, axtcc, t);
    temp16blen = triScaleExpansionZeroElim(axtcclen, axtcc, bdy, temp16b, t);

    axtbblen = triScaleExpansionZeroElim(4, bb, adxtail, axtbb, t);
    temp16clen = triScaleExpansionZeroElim(axtbblen, axtbb, -cdy, temp16c, t);

    temp32alen = triFastExpansionSumZeroElim(temp16alen, temp16a, temp16blen, temp16b, temp32a);
    temp48len = triFastExpansionSumZeroElim(temp16clen, temp16c, temp32alen, temp32a, temp48);
    finlength = triFastExpansionSumZeroElim(finlength, finnow, temp48len, temp48, finother);
    finswap = finnow;
    finnow = finother;
    finother = finswap;
  }
  if (adytail != 0.0)
  {
    aytbclen = triScaleExpansionZeroElim(4, bc, adytail, aytbc, t);
    temp16alen = triScaleExpansionZeroElim(aytbclen, aytbc, 2.0 * ady, temp16a, t);

    aytbblen = triScaleExpansionZeroElim(4, bb, adytail, aytbb, t);
    temp16blen = triScaleExpansionZeroElim(aytbblen, aytbb, cdx, temp16b, t);

    aytcclen = triScaleExpansionZeroElim(4, cc, adytail, aytcc, t);
    temp16clen = triScaleExpansionZeroElim(aytcclen, aytcc, -bdx, temp16c, t);

    temp32alen = triFastExpansionSumZeroElim(temp16alen, temp16a, temp16blen, temp16b, temp32a);
    temp48len = triFastExpansionSumZeroElim(temp16clen, temp16c, temp32alen, temp32a, temp48);
    finlength = triFastExpansionSumZeroElim(finlength, finnow, temp48len, temp48, finother);
    finswap = finnow;
    finnow = finother;
    finother = finswap;
  }
  if (bdxtail != 0.0)
  {
    bxtcalen = triScaleExpansionZeroElim(4, ca, bdxtail, bxtca, t);
    temp16alen = triScaleExpansionZeroElim(bxtcalen, bxtca, 2.0 * bdx, temp16a, t);

    bxtaalen = triScaleExpansionZeroElim(4, aa, bdxtail, bxtaa, t);
    temp16blen = triScaleExpansionZeroElim(bxtaalen, bxtaa, cdy, temp16b, t);

    bxtcclen = triScaleExpansionZeroElim(4, cc, bdxtail, bxtcc, t);
    temp16clen = triScaleExpansionZeroElim(bxtcclen, bxtcc, -ady, temp16c, t);

    temp32alen = triFastExpansionSumZeroElim(temp16alen, temp16a, temp16blen, temp16b, temp32a);
    temp48len = triFastExpansionSumZeroElim(temp16clen, temp16c, temp32alen, temp32a, temp48);
    finlength = triFastExpansionSumZeroElim(finlength, finnow, temp48len, temp48, finother);
    finswap = finnow;
    finnow = finother;
    finother = finswap;
  }
  if (bdytail != 0.0)
  {
    bytcalen = triScaleExpansionZeroElim(4, ca, bdytail, bytca, t);
    temp16alen = triScaleExpansionZeroElim(bytcalen, bytca, 2.0 * bdy, temp16a, t);

    bytcclen = triScaleExpansionZeroElim(4, cc, bdytail, bytcc, t);
    temp16blen = triScaleExpansionZeroElim(bytcclen, bytcc, adx, temp16b, t);

    bytaalen = triScaleExpansionZeroElim(4, aa, bdytail, bytaa, t);
    temp16clen = triScaleExpansionZeroElim(bytaalen, bytaa, -cdx, temp16c, t);

    temp32alen = triFastExpansionSumZeroElim(temp16alen, temp16a, temp16blen, temp16b, temp32a);
    temp48len = triFastExpansionSumZeroElim(temp16clen, temp16c, temp32alen, temp32a, temp48);
    finlength = triFastExpansionSumZeroElim(finlength, finnow, temp48len, temp48, finother);
    finswap = finnow;
    finnow = finother;
    finother = finswap;
  }
  if (cdxtail != 0.0)
  {
    cxtablen = triScaleExpansionZeroElim(4, ab, cdxtail, cxtab, t);
    temp16alen = triScaleExpansionZeroElim(cxtablen, cxtab, 2.0 * cdx, temp16a, t);

    cxtbblen = triScaleExpansionZeroElim(4, bb, cdxtail, cxtbb, t);
    temp16blen = triScaleExpansionZeroElim(cxtbblen, cxtbb, ady, temp16b, t);

    cxtaalen = triScaleExpansionZeroElim(4, aa, cdxtail, cxtaa, t);
    temp16clen = triScaleExpansionZeroElim(cxtaalen, cxtaa, -bdy, temp16c, t);

    temp32alen = triFastExpansionSumZeroElim(temp16alen, temp16a, temp16blen, temp16b, temp32a);
    temp48len = triFastExpansionSumZeroElim(temp16clen, temp16c, temp32alen, temp32a, temp48);
    finlength = triFastExpansionSumZeroElim(finlength, finnow, temp48len, temp48, finother);
    finswap = finnow;
    finnow = finother;
    finother = finswap;
  }
  if (cdytail != 0.0)
  {
    cytablen = triScaleExpansionZeroElim(4, ab, cdytail, cytab, t);
    temp16alen = triScaleExpansionZeroElim(cytablen, cytab, 2.0 * cdy, temp16a, t);

    cytaalen = triScaleExpansionZeroElim(4, aa, cdytail, cytaa, t);
    temp16blen = triScaleExpansionZeroElim(cytaalen, cytaa, bdx, temp16b, t);

    cytbblen = triScaleExpansionZeroElim(4, bb, cdytail, cytbb, t);
    temp16clen = triScaleExpansionZeroElim(cytbblen, cytbb, -adx, temp16c, t);

    temp32alen = triFastExpansionSumZeroElim(temp16alen, temp16a, temp16blen, temp16b, temp32a);
    temp48len = triFastExpansionSumZeroElim(temp16clen, temp16c, temp32alen, temp32a, temp48);
    finlength = triFastExpansionSumZeroElim(finlength, finnow, temp48len, temp48, finother);
    finswap = finnow;
    finnow = finother;
    finother = finswap;
  }

  if ((adxtail != 0.0) || (adytail != 0.0))
  {
    if ((bdxtail != 0.0) || (bdytail != 0.0) || (cdxtail != 0.0) || (cdytail != 0.0))
    {
      Two_Product(t.m_splitter, bdxtail, cdy, ti1, ti0);
      Two_Product(t.m_splitter, bdx, cdytail, tj1, tj0);
      Two_Two_Sum(ti1, ti0, tj1, tj0, u3, u[2], u[1], u[0]);
      u[3] = u3;
      negate = -bdy;
      Two_Product(t.m_splitter, cdxtail, negate, ti1, ti0);
      negate = -bdytail;
      Two_Product(t.m_splitter, cdx, negate, tj1, tj0);
      Two_Two_Sum(ti1, ti0, tj1, tj0, v3, v[2], v[1], v[0]);
      v[3] = v3;
      bctlen = triFastExpansionSumZeroElim(4, u, 4, v, bct);

      Two_Product(t.m_splitter, bdxtail, cdytail, ti1, ti0);
      Two_Product(t.m_splitter, cdxtail, bdytail, tj1, tj0);
      Two_Two_Diff(ti1, ti0, tj1, tj0, bctt3, bctt[2], bctt[1], bctt[0]);
      bctt[3] = bctt3;
      bcttlen = 4;
    }
    else
    {
      bct[0] = 0.0;
      bctlen = 1;
      bctt[0] = 0.0;
      bcttlen = 1;
    }

    if (adxtail != 0.0)
    {
      temp16alen = triScaleExpansionZeroElim(axtbclen, axtbc, adxtail, temp16a, t);
      axtbctlen = triScaleExpansionZeroElim(bctlen, bct, adxtail, axtbct, t);
      temp32alen = triScaleExpansionZeroElim(axtbctlen, axtbct, 2.0 * adx, temp32a, t);
      temp48len = triFastExpansionSumZeroElim(temp16alen, temp16a, temp32alen, temp32a, temp48);
      finlength = triFastExpansionSumZeroElim(finlength, finnow, temp48len, temp48, finother);
      finswap = finnow;
      finnow = finother;
      finother = finswap;
      if (bdytail != 0.0)
      {
        temp8len = triScaleExpansionZeroElim(4, cc, adxtail, temp8, t);
        temp16alen = triScaleExpansionZeroElim(temp8len, temp8, bdytail, temp16a, t);
        finlength = triFastExpansionSumZeroElim(finlength, finnow, temp16alen, temp16a, finother);
        finswap = finnow;
        finnow = finother;
        finother = finswap;
      }
      if (cdytail != 0.0)
      {
        temp8len = triScaleExpansionZeroElim(4, bb, -adxtail, temp8, t);
        temp16alen = triScaleExpansionZeroElim(temp8len, temp8, cdytail, temp16a, t);
        finlength = triFastExpansionSumZeroElim(finlength, finnow, temp16alen, temp16a, finother);
        finswap = finnow;
        finnow = finother;
        finother = finswap;
      }

      temp32alen = triScaleExpansionZeroElim(axtbctlen, axtbct, adxtail, temp32a, t);
      axtbcttlen = triScaleExpansionZeroElim(bcttlen, bctt, adxtail, axtbctt, t);
      temp16alen = triScaleExpansionZeroElim(axtbcttlen, axtbctt, 2.0 * adx, temp16a, t);
      temp16blen = triScaleExpansionZeroElim(axtbcttlen, axtbctt, adxtail, temp16b, t);
      temp32blen = triFastExpansionSumZeroElim(temp16alen, temp16a, temp16blen, temp16b, temp32b);
      temp64len = triFastExpansionSumZeroElim(temp32alen, temp32a, temp32blen, temp32b, temp64);
      finlength = triFastExpansionSumZeroElim(finlength, finnow, temp64len, temp64, finother);
      finswap = finnow;
      finnow = finother;
      finother = finswap;
    }
    if (adytail != 0.0)
    {
      temp16alen = triScaleExpansionZeroElim(aytbclen, aytbc, adytail, temp16a, t);
      aytbctlen = triScaleExpansionZeroElim(bctlen, bct, adytail, aytbct, t);
      temp32alen = triScaleExpansionZeroElim(aytbctlen, aytbct, 2.0 * ady, temp32a, t);
      temp48len = triFastExpansionSumZeroElim(temp16alen, temp16a, temp32alen, temp32a, temp48);
      finlength = triFastExpansionSumZeroElim(finlength, finnow, temp48len, temp48, finother);
      finswap = finnow;
      finnow = finother;
      finother = finswap;

      temp32alen = triScaleExpansionZeroElim(aytbctlen, aytbct, adytail, temp32a, t);
      aytbcttlen = triScaleExpansionZeroElim(bcttlen, bctt, adytail, aytbctt, t);
      temp16alen = triScaleExpansionZeroElim(aytbcttlen, aytbctt, 2.0 * ady, temp16a, t);
      temp16blen = triScaleExpansionZeroElim(aytbcttlen, aytbctt, adytail, temp16b, t);
      temp32blen = triFastExpansionSumZeroElim(temp16alen, temp16a, temp16blen, temp16b, temp32b);
      temp64len = triFastExpansionSumZeroElim(temp32alen, temp32a, temp32blen, temp32b, temp64);
      finlength = triFastExpansionSumZeroElim(finlength, finnow, temp64len, temp64, finother);
      finswap = finnow;
      finnow = finother;
      finother = finswap;
    }
  }
  if ((bdxtail != 0.0) || (bdytail != 0.0))
  {
    if ((cdxtail != 0.0) || (cdytail != 0.0) || (adxtail != 0.0) || (adytail != 0.0))
    {
      Two_Product(t.m_splitter, cdxtail, ady, ti1, ti0);
      Two_Product(t.m_splitter, cdx, adytail, tj1, tj0);
      Two_Two_Sum(ti1, ti0, tj1, tj0, u3, u[2], u[1], u[0]);
      u[3] = u3;
      negate = -cdy;
      Two_Product(t.m_splitter, adxtail, negate, ti1, ti0);
      negate = -cdytail;
      Two_Product(t.m_splitter, adx, negate, tj1, tj0);
      Two_Two_Sum(ti1, ti0, tj1, tj0, v3, v[2], v[1], v[0]);
      v[3] = v3;
      catlen = triFastExpansionSumZeroElim(4, u, 4, v, cat);

      Two_Product(t.m_splitter, cdxtail, adytail, ti1, ti0);
      Two_Product(t.m_splitter, adxtail, cdytail, tj1, tj0);
      Two_Two_Diff(ti1, ti0, tj1, tj0, catt3, catt[2], catt[1], catt[0]);
      catt[3] = catt3;
      cattlen = 4;
    }
    else
    {
      cat[0] = 0.0;
      catlen = 1;
      catt[0] = 0.0;
      cattlen = 1;
    }

    if (bdxtail != 0.0)
    {
      temp16alen = triScaleExpansionZeroElim(bxtcalen, bxtca, bdxtail, temp16a, t);
      bxtcatlen = triScaleExpansionZeroElim(catlen, cat, bdxtail, bxtcat, t);
      temp32alen = triScaleExpansionZeroElim(bxtcatlen, bxtcat, 2.0 * bdx, temp32a, t);
      temp48len = triFastExpansionSumZeroElim(temp16alen, temp16a, temp32alen, temp32a, temp48);
      finlength = triFastExpansionSumZeroElim(finlength, finnow, temp48len, temp48, finother);
      finswap = finnow;
      finnow = finother;
      finother = finswap;
      if (cdytail != 0.0)
      {
        temp8len = triScaleExpansionZeroElim(4, aa, bdxtail, temp8, t);
        temp16alen = triScaleExpansionZeroElim(temp8len, temp8, cdytail, temp16a, t);
        finlength = triFastExpansionSumZeroElim(finlength, finnow, temp16alen, temp16a, finother);
        finswap = finnow;
        finnow = finother;
        finother = finswap;
      }
      if (adytail != 0.0)
      {
        temp8len = triScaleExpansionZeroElim(4, cc, -bdxtail, temp8, t);
        temp16alen = triScaleExpansionZeroElim(temp8len, temp8, adytail, temp16a, t);
        finlength = triFastExpansionSumZeroElim(finlength, finnow, temp16alen, temp16a, finother);
        finswap = finnow;
        finnow = finother;
        finother = finswap;
      }

      temp32alen = triScaleExpansionZeroElim(bxtcatlen, bxtcat, bdxtail, temp32a, t);
      bxtcattlen = triScaleExpansionZeroElim(cattlen, catt, bdxtail, bxtcatt, t);
      temp16alen = triScaleExpansionZeroElim(bxtcattlen, bxtcatt, 2.0 * bdx, temp16a, t);
      temp16blen = triScaleExpansionZeroElim(bxtcattlen, bxtcatt, bdxtail, temp16b, t);
      temp32blen = triFastExpansionSumZeroElim(temp16alen, temp16a, temp16blen, temp16b, temp32b);
      temp64len = triFastExpansionSumZeroElim(temp32alen, temp32a, temp32blen, temp32b, temp64);
      finlength = triFastExpansionSumZeroElim(finlength, finnow, temp64len, temp64, finother);
      finswap = finnow;
      finnow = finother;
      finother = finswap;
    }
    if (bdytail != 0.0)
    {
      temp16alen = triScaleExpansionZeroElim(bytcalen, bytca, bdytail, temp16a, t);
      bytcatlen = triScaleExpansionZeroElim(catlen, cat, bdytail, bytcat, t);
      temp32alen = triScaleExpansionZeroElim(bytcatlen, bytcat, 2.0 * bdy, temp32a, t);
      temp48len = triFastExpansionSumZeroElim(temp16alen, temp16a, temp32alen, temp32a, temp48);
      finlength = triFastExpansionSumZeroElim(finlength, finnow, temp48len, temp48, finother);
      finswap = finnow;
      finnow = finother;
      finother = finswap;

      temp32alen = triScaleExpansionZeroElim(bytcatlen, bytcat, bdytail, temp32a, t);
      bytcattlen = triScaleExpansionZeroElim(cattlen, catt, bdytail, bytcatt, t);
      temp16alen = triScaleExpansionZeroElim(bytcattlen, bytcatt, 2.0 * bdy, temp16a, t);
      temp16blen = triScaleExpansionZeroElim(bytcattlen, bytcatt, bdytail, temp16b, t);
      temp32blen = triFastExpansionSumZeroElim(temp16alen, temp16a, temp16blen, temp16b, temp32b);
      temp64len = triFastExpansionSumZeroElim(temp32alen, temp32a, temp32blen, temp32b, temp64);
      finlength = triFastExpansionSumZeroElim(finlength, finnow, temp64len, temp64, finother);
      finswap = finnow;
      finnow = finother;
      finother = finswap;
    }
  }
  if ((cdxtail != 0.0) || (cdytail != 0.0))
  {
    if ((adxtail != 0.0) || (adytail != 0.0) || (bdxtail != 0.0) || (bdytail != 0.0))
    {
      Two_Product(t.m_splitter, adxtail, bdy, ti1, ti0);
      Two_Product(t.m_splitter, adx, bdytail, tj1, tj0);
      Two_Two_Sum(ti1, ti0, tj1, tj0, u3, u[2], u[1], u[0]);
      u[3] = u3;
      negate = -ady;
      Two_Product(t.m_splitter, bdxtail, negate, ti1, ti0);
      negate = -adytail;
      Two_Product(t.m_splitter, bdx, negate, tj1, tj0);
      Two_Two_Sum(ti1, ti0, tj1, tj0, v3, v[2], v[1], v[0]);
      v[3] = v3;
      abtlen = triFastExpansionSumZeroElim(4, u, 4, v, abt);

      Two_Product(t.m_splitter, adxtail, bdytail, ti1, ti0);
      Two_Product(t.m_splitter, bdxtail, adytail, tj1, tj0);
      Two_Two_Diff(ti1, ti0, tj1, tj0, abtt3, abtt[2], abtt[1], abtt[0]);
      abtt[3] = abtt3;
      abttlen = 4;
    }
    else
    {
      abt[0] = 0.0;
      abtlen = 1;
      abtt[0] = 0.0;
      abttlen = 1;
    }

    if (cdxtail != 0.0)
    {
      temp16alen = triScaleExpansionZeroElim(cxtablen, cxtab, cdxtail, temp16a, t);
      cxtabtlen = triScaleExpansionZeroElim(abtlen, abt, cdxtail, cxtabt, t);
      temp32alen = triScaleExpansionZeroElim(cxtabtlen, cxtabt, 2.0 * cdx, temp32a, t);
      temp48len = triFastExpansionSumZeroElim(temp16alen, temp16a, temp32alen, temp32a, temp48);
      finlength = triFastExpansionSumZeroElim(finlength, finnow, temp48len, temp48, finother);
      finswap = finnow;
      finnow = finother;
      finother = finswap;
      if (adytail != 0.0)
      {
        temp8len = triScaleExpansionZeroElim(4, bb, cdxtail, temp8, t);
        temp16alen = triScaleExpansionZeroElim(temp8len, temp8, adytail, temp16a, t);
        finlength = triFastExpansionSumZeroElim(finlength, finnow, temp16alen, temp16a, finother);
        finswap = finnow;
        finnow = finother;
        finother = finswap;
      }
      if (bdytail != 0.0)
      {
        temp8len = triScaleExpansionZeroElim(4, aa, -cdxtail, temp8, t);
        temp16alen = triScaleExpansionZeroElim(temp8len, temp8, bdytail, temp16a, t);
        finlength = triFastExpansionSumZeroElim(finlength, finnow, temp16alen, temp16a, finother);
        finswap = finnow;
        finnow = finother;
        finother = finswap;
      }

      temp32alen = triScaleExpansionZeroElim(cxtabtlen, cxtabt, cdxtail, temp32a, t);
      cxtabttlen = triScaleExpansionZeroElim(abttlen, abtt, cdxtail, cxtabtt, t);
      temp16alen = triScaleExpansionZeroElim(cxtabttlen, cxtabtt, 2.0 * cdx, temp16a, t);
      temp16blen = triScaleExpansionZeroElim(cxtabttlen, cxtabtt, cdxtail, temp16b, t);
      temp32blen = triFastExpansionSumZeroElim(temp16alen, temp16a, temp16blen, temp16b, temp32b);
      temp64len = triFastExpansionSumZeroElim(temp32alen, temp32a, temp32blen, temp32b, temp64);
      finlength = triFastExpansionSumZeroElim(finlength, finnow, temp64len, temp64, finother);
      finswap = finnow;
      finnow = finother;
      finother = finswap;
    }
    if (cdytail != 0.0)
    {
      temp16alen = triScaleExpansionZeroElim(cytablen, cytab, cdytail, temp16a, t);
      cytabtlen = triScaleExpansionZeroElim(abtlen, abt, cdytail, cytabt, t);
      temp32alen = triScaleExpansionZeroElim(cytabtlen, cytabt, 2.0 * cdy, temp32a, t);
      temp48len = triFastExpansionSumZeroElim(temp16alen, temp16a, temp32alen, temp32a, temp48);
      finlength = triFastExpansionSumZeroElim(finlength, finnow, temp48len, temp48, finother);
      finswap = finnow;
      finnow = finother;
      finother = finswap;

      temp32alen = triScaleExpansionZeroElim(cytabtlen, cytabt, cdytail, temp32a, t);
      cytabttlen = triScaleExpansionZeroElim(abttlen, abtt, cdytail, cytabtt, t);
      temp16alen = triScaleExpansionZeroElim(cytabttlen, cytabtt, 2.0 * cdy, temp16a, t);
      temp16blen = triScaleExpansionZeroElim(cytabttlen, cytabtt, cdytail, temp16b, t);
      temp32blen = triFastExpansionSumZeroElim(temp16alen, temp16a, temp16blen, temp16b, temp32b);
      temp64len = triFastExpansionSumZeroElim(temp32alen, temp32a, temp32blen, temp32b, temp64);
      finlength = triFastExpansionSumZeroElim(finlength, finnow, temp64len, temp64, finother);
      finswap = finnow;
      finnow = finother;
      finother = finswap;
    }
  }

  return finnow[finlength - 1];
} // triInCircleAdapt
//------------------------------------------------------------------------------
// FUNCTION triInitTrianglePool
/// \brief  Calculate sizes of the triangle and shell edge data structures
//          and initialize their memory pools.
// NOTES    This routine also computes the `highorderid', `elemattid',
//             indices used to find values within each triangle.
//          highorderid: index in each tri for extra nodes (above three)
//                       associated with high order elements are found
//                       There are 3 ptrs to other tris, 3 pts to corners,
//                       & maybe 3 ptrs to shell edges before the extra nodes
//          elemattid:   index in each tri for its attributes
//                       where the index is measured in double
//------------------------------------------------------------------------------
static void triInitTrianglePool(TriVars& t)
{
#define TRIPERBLOCK 4092 /* # of tris allocated at once */
  int trisize, highorderid, elemattid;
  /* AKZ - we may be able to hack this based on no high order elements */
  highorderid = 6;
  /* # of bytes occupied by a triangle */
  trisize = (3 + (highorderid - 3)) * sizeof(Ttri);
  elemattid = (trisize + sizeof(double) - 1) / sizeof(double);
  /* Initialize triangle pool */
  triPoolInit(&t.m_triangles, trisize, TRIPERBLOCK, TRIPERBLOCK, 4);
  /* Initialize the "outer space" triangle */
  triDummyInit(t.m_triangles.itembytes, t);
} // triInitTrianglePool
//------------------------------------------------------------------------------
// FUNCTION triMakeTriangle
/// \brief  Create a new triangle with orientation zero.
// NOTES
//------------------------------------------------------------------------------
static bool triMakeTriangle(Tedgetype newtedge, TriVars& t)
{
  newtedge->tri = (Ttri*)triPoolAlloc(&t.m_triangles);

  /* Init the adjoining tris to be "outer space" */
  newtedge->tri[0] = (Ttri)t.m_dummytri;
  newtedge->tri[1] = (Ttri)t.m_dummytri;
  newtedge->tri[2] = (Ttri)t.m_dummytri;
  /* Three NULL vertex points. */
  newtedge->tri[3] = nullptr;
  newtedge->tri[4] = nullptr;
  newtedge->tri[5] = nullptr;
  newtedge->orient = 0;
  return true;
} // triMakeNewTriangle
//------------------------------------------------------------------------------
// FUNCTION triMergeHulls
/// \brief  Merge two adjacent Delaunay triangulations into a
//          single Delaunay triangulation.
// NOTES
//  This is similar to the algorithm given by Guibas and Stolfi, but uses
//  a triangle-based, rather than edge-based, data structure.
//
//  The algorithm walks up the gap between the two triangulations, knitting
//  them together.  As they are merged, some of their bounding triangles
//  are converted into real triangles of the triangulation.  The procedure
//  pulls each hull's bounding triangles apart, then knits them together
//  like the teeth of two gears.  The Delaunay property determines, at each
//  step, whether the next "tooth" is a bounding triangle of the left hull
//  or the right.  When a bounding triangle becomes real, its apex is
//  changed from NULL to a real point.
//
//  Only two new triangles need to be allocated.  These become new bounding
//  triangles at the top and bottom of the seam.  They are used to connect
//  the remaining bounding triangles (those that have not been converted
//  into real triangles) into a single fan.
//
//  On entry, `farleft' and `innerleft' are bounding triangles of the left
//  triangulation.  The origin of `farleft' is the leftmost vertex, and
//  the destination of `innerleft' is the rightmost vertex of the
//  triangulation.  Similarly, `innerright' and `farright' are bounding
//  triangles of the right triangulation.  The origin of `innerright' and
//  destination of `farright' are the leftmost and rightmost vertices.
//
//  On completion, the origin of `farleft' is the leftmost vertex of the
//  merged triangulation, and the destination of `farright' is the rightmost
//  vertex.
//------------------------------------------------------------------------------
static bool triMergeHulls(Tedgetype farleft,
                          Tedgetype innerleft,
                          Tedgetype innerright,
                          Tedgetype farright,
                          int axis,
                          TriVars& t)
{
  int changemade, badedge, leftfinished, rightfinished;
  Tedge leftcand, rightcand, baseedge, nextedge;
  Tedge sidecasing, topcasing, outercasing, checkedge;
  Tpt innerleftdest, innerrightorg;
  Tpt innerleftapex, innerrightapex;
  Tpt farleftpt, farrightpt;
  Tpt farleftapex, farrightapex;
  Tpt lowerleft, lowerright;
  Tpt upperleft, upperright;
  Tpt nextapex, checkvertex;
  Ttri ptr; /* Temporary variable used by sym() */
  bool canMakeTriangles = true;

  dest(*innerleft, innerleftdest);
  apex(*innerleft, innerleftapex);
  org(*innerright, innerrightorg);
  apex(*innerright, innerrightapex);
  /* Special treatment for horizontal cuts */
  if (axis == 1)
  {
    org(*farleft, farleftpt);
    apex(*farleft, farleftapex);
    dest(*farright, farrightpt);
    apex(*farright, farrightapex);
    /* The pointers to the extremal points are shifted to point to the */
    /*   topmost and bottommost point of each hull, rather than the    */
    /*   leftmost and rightmost points.                                */
    while (farleftapex[1] < farleftpt[1])
    {
      lnextself(*farleft);
      symself(*farleft);
      farleftpt = farleftapex;
      apex(*farleft, farleftapex);
    }
    sym(*innerleft, checkedge);
    apex(checkedge, checkvertex);
    while (checkvertex[1] > innerleftdest[1])
    {
      lnext(checkedge, *innerleft);
      innerleftapex = innerleftdest;
      innerleftdest = checkvertex;
      sym(*innerleft, checkedge);
      apex(checkedge, checkvertex);
    }
    while (innerrightapex[1] < innerrightorg[1])
    {
      lnextself(*innerright);
      symself(*innerright);
      innerrightorg = innerrightapex;
      apex(*innerright, innerrightapex);
    }
    sym(*farright, checkedge);
    apex(checkedge, checkvertex);
    while (checkvertex[1] > farrightpt[1])
    {
      lnext(checkedge, *farright);
      farrightapex = farrightpt;
      farrightpt = checkvertex;
      sym(*farright, checkedge);
      apex(checkedge, checkvertex);
    }
  }
  /* Find a line tangent to and below both hulls. */
  do
  {
    changemade = 0;
    /* Make innerleftdest the "bottommost" point
     * of the left hull. */
    if (triCounterClockwise(innerleftdest, innerleftapex, innerrightorg, t) > 0.0)
    {
      lprevself(*innerleft);
      symself(*innerleft);
      innerleftdest = innerleftapex;
      apex(*innerleft, innerleftapex);
      changemade = 1;
    }
    /* Make innerrightorg the "bottommost" point
     * of the right hull. */
    if (triCounterClockwise(innerrightapex, innerrightorg, innerleftdest, t) > 0.0)
    {
      lnextself(*innerright);
      symself(*innerright);
      innerrightorg = innerrightapex;
      apex(*innerright, innerrightapex);
      changemade = 1;
    }
  } while (changemade);
  /* Find the 2 candidates to be next "gear tooth" */
  sym(*innerleft, leftcand);
  sym(*innerright, rightcand);
  /* Create the bottom new bounding triangle. */
  canMakeTriangles = triMakeTriangle(&baseedge, t);
  if (!canMakeTriangles)
  {
    return false;
  }
  /* Connect it to the bounding boxes of the left
   * and right triangulations. */
  bond(baseedge, *innerleft);
  lnextself(baseedge);
  bond(baseedge, *innerright);
  lnextself(baseedge);
  setorg(baseedge, innerrightorg);
  setdest(baseedge, innerleftdest);
  /* Apex is intentionally left NULL. */
  /* Fix the extreme triangles if necessary. */
  org(*farleft, farleftpt);
  if (innerleftdest == farleftpt)
  {
    lnext(baseedge, *farleft);
  }
  dest(*farright, farrightpt);
  if (innerrightorg == farrightpt)
  {
    lprev(baseedge, *farright);
  }
  /* The vertices of the current knitting edge. */
  lowerleft = innerleftdest;
  lowerright = innerrightorg;
  /* The candidate vertices for knitting. */
  apex(leftcand, upperleft);
  apex(rightcand, upperright);
  /* Walk up the gap between the two triangulations,
   * knitting them together. */
  for (;;)
  {
    /* Have we reached the top?  (This isn't quite the right question,       */
    /*   because even though the left triangulation might seem finished now, */
    /*   moving up on the right triangulation might reveal a new point of    */
    /*   the left triangulation.  And vice-versa.)                           */
    leftfinished = triCounterClockwise(upperleft, lowerleft, lowerright, t) <= 0.0;
    rightfinished = triCounterClockwise(upperright, lowerleft, lowerright, t) <= 0.0;
    if (leftfinished && rightfinished)
    {
      /* Create the top new bounding triangle. */
      canMakeTriangles = triMakeTriangle(&nextedge, t);
      if (!canMakeTriangles)
      {
        return false;
      }
      setorg(nextedge, lowerleft);
      setdest(nextedge, lowerright);
      /* Apex is intentionally left NULL. */
      /* Connect it to the bounding boxes of the
       * two triangulations. */
      bond(nextedge, baseedge);
      lnextself(nextedge);
      bond(nextedge, rightcand);
      lnextself(nextedge);
      bond(nextedge, leftcand);
      /* Special treatment for horizontal cuts. */
      if (axis == 1)
      {
        org(*farleft, farleftpt);
        apex(*farleft, farleftapex);
        dest(*farright, farrightpt);
        apex(*farright, farrightapex);
        sym(*farleft, checkedge);
        apex(checkedge, checkvertex);
        /* The pointers to the extremal points are restored to the leftmost */
        /*   and rightmost points (rather than topmost and bottommost).     */
        while (checkvertex[0] < farleftpt[0])
        {
          lprev(checkedge, *farleft);
          farleftapex = farleftpt;
          farleftpt = checkvertex;
          sym(*farleft, checkedge);
          apex(checkedge, checkvertex);
        }
        while (farrightapex[0] > farrightpt[0])
        {
          lprevself(*farright);
          symself(*farright);
          farrightpt = farrightapex;
          apex(*farright, farrightapex);
        }
      }
      return true;
    }
    /* Consider eliminating edges from the left
     * triangulation. */
    if (!leftfinished)
    {
      /* What vertex would be exposed if an edge
       * were deleted? */
      lprev(leftcand, nextedge);
      symself(nextedge);
      apex(nextedge, nextapex);
      /* If nextapex is NULL, then no vertex would be exposed; the */
      /*   triangulation would have been eaten right through.      */
      if (nextapex != nullptr)
      {
        /* Check whether the edge is Delaunay. */
        badedge = triInCircle(lowerleft, lowerright, upperleft, nextapex, t) > 0.0;
        while (badedge)
        {
          /* Eliminate the edge with an edge flip.  As a result, the    */
          /*   left triangulation will have one more boundary triangle. */
          lnextself(nextedge);
          sym(nextedge, topcasing);
          lnextself(nextedge);
          sym(nextedge, sidecasing);
          bond(nextedge, topcasing);
          bond(leftcand, sidecasing);
          lnextself(leftcand);
          sym(leftcand, outercasing);
          lprevself(nextedge);
          bond(nextedge, outercasing);
          /* Correct vertices to reflect the edge flip. */
          setorg(leftcand, lowerleft);
          setdest(leftcand, nullptr);
          setapex(leftcand, nextapex);
          setorg(nextedge, nullptr);
          setdest(nextedge, upperleft);
          setapex(nextedge, nextapex);
          /* Consider the newly exposed vertex. */
          upperleft = nextapex;
          /* What vertex would be exposed if another
           * edge were deleted? */
          tedgecopy(sidecasing, nextedge);
          apex(nextedge, nextapex);
          if (nextapex != nullptr)
          {
            /* Check whether the edge is Delaunay. */
            badedge = triInCircle(lowerleft, lowerright, upperleft, nextapex, t) > 0.0;
          }
          else
          {
            /* Avoid eating right through the triangulation */
            badedge = 0;
          }
        }
      }
    }
    /* Consider eliminating edges from the
     * right triangulation. */
    if (!rightfinished)
    {
      /* What vertex would be exposed if an edge
       * were deleted? */
      lnext(rightcand, nextedge);
      symself(nextedge);
      apex(nextedge, nextapex);
      /* If nextapex is NULL, then no vertex would
       * be exposed; the triangulation would have been
       * eaten right through.      */
      if (nextapex != nullptr)
      {
        /* Check whether the edge is Delaunay. */
        badedge = triInCircle(lowerleft, lowerright, upperright, nextapex, t) > 0.0;
        while (badedge)
        {
          /* Eliminate the edge with an edge flip.
           * As a result, the right triangulation will
           *   have one more boundary triangle. */
          lprevself(nextedge);
          sym(nextedge, topcasing);
          lprevself(nextedge);
          sym(nextedge, sidecasing);
          bond(nextedge, topcasing);
          bond(rightcand, sidecasing);
          lprevself(rightcand);
          sym(rightcand, outercasing);
          lnextself(nextedge);
          bond(nextedge, outercasing);
          /* Correct vertices to reflect the edge flip. */
          setorg(rightcand, nullptr);
          setdest(rightcand, lowerright);
          setapex(rightcand, nextapex);
          setorg(nextedge, upperright);
          setdest(nextedge, nullptr);
          setapex(nextedge, nextapex);
          /* Consider the newly exposed vertex. */
          upperright = nextapex;
          /* What vertex would be exposed if another
           * edge were deleted? */
          tedgecopy(sidecasing, nextedge);
          apex(nextedge, nextapex);
          if (nextapex != nullptr)
          {
            /* Check whether the edge is Delaunay. */
            badedge = triInCircle(lowerleft, lowerright, upperright, nextapex, t) > 0.0;
          }
          else
          {
            /* Avoid eating right through the triangulation. */
            badedge = 0;
          }
        }
      }
    }
    if (leftfinished ||
        (!rightfinished && (triInCircle(upperleft, lowerleft, lowerright, upperright, t) > 0.0)))
    {
      /* Knit the triangulations, adding an edge
       * from `lowerleft' to `upperright' */
      bond(baseedge, rightcand);
      lprev(rightcand, baseedge);
      setdest(baseedge, lowerleft);
      lowerright = upperright;
      sym(baseedge, rightcand);
      apex(rightcand, upperright);
    }
    else
    {
      /* Knit the triangulations, adding an edge
       * from `upperleft' to `lowerright' */
      bond(baseedge, leftcand);
      lnext(leftcand, baseedge);
      setorg(baseedge, lowerright);
      lowerleft = upperleft;
      sym(baseedge, leftcand);
      apex(leftcand, upperleft);
    }
  }
} // triMergeHulls
//------------------------------------------------------------------------------
// FUNCTION triNumberNodes
/// \brief  Number the points.
// NOTES    Each point is assigned a marker equal to its number.
//------------------------------------------------------------------------------
static void triNumberNodes(TriVars& t)
{
  int pointnumber;
  Tpt pointloop;

  triTraversalInit(&t.m_points);
  pointloop = triPointTraverse(t);
  pointnumber = 1;
  while (pointloop != nullptr)
  {
    setpointmark(pointloop, pointnumber, t.m_pointmarkindex);
    pointloop = triPointTraverse(t);
    pointnumber++;
  }
} // triNumberNodes
//------------------------------------------------------------------------------
// FUNCTION triPointMedian
/// \brief  An order statistic algorithm, almost.  Shuffles an array
//          of points so that the first `median' points occur
//          lexicographically before the remaining points.
// NOTES
//  Uses the x-coordinate as the primary key if axis == 0; the y-coordinate
//  if axis == 1.  Very similar to the triPointSort() procedure, but runs in
//  randomized linear time.
//------------------------------------------------------------------------------
static void triPointMedian(Tpt* sortarray, int arraysize, int median, int axis)
{
  int left, right, pivot;
  double pivot1, pivot2;
  Tpt temp;
  /* Recursive base case. */
  if (arraysize == 2)
  {
    if ((sortarray[0][axis] > sortarray[1][axis]) ||
        ((sortarray[0][axis] == sortarray[1][axis]) &&
         (sortarray[0][1 - axis] > sortarray[1][1 - axis])))
    {
      temp = sortarray[1];
      sortarray[1] = sortarray[0];
      sortarray[0] = temp;
    }
    return;
  }
  else if (arraysize == 0)
    return;
  /* Choose a random pivot to split the array. */
  pivot = (int)triRandomnation((unsigned int)arraysize);
  pivot1 = sortarray[pivot][axis];
  pivot2 = sortarray[pivot][1 - axis];
  /* Split the array. */
  left = -1;
  right = arraysize;
  while (left < right)
  {
    /* Find a pt whose 'x' is > left */
    do
    {
      left++;
    } while ((left <= right) &&
             ((sortarray[left][axis] < pivot1) ||
              ((sortarray[left][axis] == pivot1) && (sortarray[left][1 - axis] < pivot2))));
    /* Find a pt whose 'x' is < right */
    do
    {
      right--;
    } while ((left <= right) &&
             ((sortarray[right][axis] > pivot1) ||
              ((sortarray[right][axis] == pivot1) && (sortarray[right][1 - axis] > pivot2))));
    if (left < right)
    {
      /* Swap the left and right points. */
      temp = sortarray[left];
      sortarray[left] = sortarray[right];
      sortarray[right] = temp;
    }
  }
  /* Recurse - only one condition can be true */
  if (left > median)
  {
    /* Recursively shuffle the left subset. */
    triPointMedian(sortarray, left, median, axis);
  }
  if (right < median - 1)
  {
    /* Recursively shuffle the right subset. */
    triPointMedian(&sortarray[right + 1], arraysize - right - 1, median - right - 1, axis);
  }
} // triPointMedian
//------------------------------------------------------------------------------
// FUNCTION triPointSort
/// \brief  Sort an array of points by x-coordinate, using the y-coordinate as
//          a secondary key.
// NOTES
//  Uses quicksort.  Randomized O(n log n) time.  No, I did not make any of
//  the usual quicksort mistakes.
//------------------------------------------------------------------------------
static void triPointSort(Tpt* sortarray, int arraysize)
{
  int left, right, pivot;
  double pivotx, pivoty;
  Tpt temp;
  /* Recursive base case. */
  if (arraysize == 2)
  {
    if ((sortarray[0][0] > sortarray[1][0]) ||
        ((sortarray[0][0] == sortarray[1][0]) && (sortarray[0][1] > sortarray[1][1])))
    {
      temp = sortarray[1];
      sortarray[1] = sortarray[0];
      sortarray[0] = temp;
    }
    return;
  }
  else if (arraysize == 0)
    return;
  /* Choose a random pivot to split the array. */
  pivot = (int)triRandomnation((unsigned int)arraysize);
  pivotx = sortarray[pivot][0];
  pivoty = sortarray[pivot][1];
  /* Split the array. */
  left = -1;
  right = arraysize;
  while (left < right)
  {
    /* Find a pt whose 'x' is > left */
    do
    {
      left++;
    } while ((left <= right) && ((sortarray[left][0] < pivotx) || ((sortarray[left][0] == pivotx) &&
                                                                   (sortarray[left][1] < pivoty))));
    /* Find a pt whose 'x' is < right */
    do
    {
      right--;
    } while ((left <= right) &&
             ((sortarray[right][0] > pivotx) ||
              ((sortarray[right][0] == pivotx) && (sortarray[right][1] > pivoty))));
    if (left < right)
    {
      /* Swap the left and right points. */
      temp = sortarray[left];
      sortarray[left] = sortarray[right];
      sortarray[right] = temp;
    }
  }
  if (left > 1)
  {
    /* Recursively sort the left subset. */
    triPointSort(sortarray, left);
  }
  if (right < arraysize - 2)
  {
    /* Recursively sort the right subset. */
    triPointSort(&sortarray[right + 1], arraysize - right - 1);
  }
} // triPointSort
//------------------------------------------------------------------------------
// FUNCTION triPointTraverse
/// \brief  Traverse the points, skipping dead ones.
// NOTES
//------------------------------------------------------------------------------
static Tpt triPointTraverse(TriVars& t)
{
  Tpt newpoint;

  do
  {
    newpoint = (Tpt)triTraverse(&t.m_points);
    if (newpoint == nullptr)
      return nullptr;
  } while (pointmark(newpoint, t.m_pointmarkindex) == -999); // Skip dead ones.
  return newpoint;
} // triPointTraverse
//------------------------------------------------------------------------------
// FUNCTION triPoolAlloc
/// \brief  Allocate space for an item.
// NOTES
//------------------------------------------------------------------------------
static int* triPoolAlloc(Tmemtype pool)
{
  int *newitem, **newblock;
  unsigned long long alignptr;
  /* use dead item before allocating a fresh one */
  if (pool->deaditemstack != nullptr)
  {
    newitem = pool->deaditemstack; /* Take first item in list */
    pool->deaditemstack = *(int**)pool->deaditemstack;
  }
  else
  {
    /* see if a new block must be allocated */
    if (pool->unallocateditems == 0)
    {
      if (*(pool->nowblock) == nullptr)
      {
        /* get a new block, add to linked list of blocks */
        newblock =
          (int**)malloc(pool->itemsperblock * pool->itembytes + sizeof(int*) + pool->alignbytes);
        if (newblock == nullptr)
        {
          std::string s = "malloc failed: ";
          s += __FUNCTION__;
          XM_LOG(xmlog::error, s);
          return nullptr;
        }
        *(pool->nowblock) = (int*)newblock;
        /* next block pointer is NULL. */
        *newblock = nullptr;
      }
      /* Move to the new block. */
      pool->nowblock = (int**)*(pool->nowblock);
      /* Find 1st item in pool. Inc by sizeof(int*) */
      alignptr = (unsigned long long)(pool->nowblock + 1);
      /* Align item on an `alignbytes'-byte boundary */
      pool->nextitem = (int*)(alignptr + (unsigned long long)pool->alignbytes -
                              (alignptr % (unsigned long long)pool->alignbytes));
      /* all items in block are unallocated */
      pool->unallocateditems = pool->itemsperblock;
    }
    /* Allocate a new item */
    newitem = pool->nextitem;
    /* Advance `nextitem' ptr to next item in block */
    pool->nextitem = (int*)((char*)pool->nextitem + pool->itembytes);

    pool->unallocateditems--;
    pool->maxitems++;
  }
  pool->items++;
  return newitem;
} // triPoolAlloc
//------------------------------------------------------------------------------
// FUNCTION triPoolDealloc
/// \brief  Deallocate space for an item.
// NOTES    The deallocated space is stored in a queue for later reuse.
//------------------------------------------------------------------------------
static void triPoolDealloc(Tmemtype pool, int* dyingitem)
{
  /* Push freshly killed item onto stack. */
  *((int**)dyingitem) = pool->deaditemstack;
  pool->deaditemstack = dyingitem;
  pool->items--;
} // triPoolDealloc
//------------------------------------------------------------------------------
// FUNCTION triPoolDeinit
/// \brief  Free to the operating system all memory taken by a pool.
// NOTES
//------------------------------------------------------------------------------
static void triPoolDeinit(Tmemtype pool)
{
  while (pool && pool->firstblock != nullptr)
  {
    pool->nowblock = (int**)*(pool->firstblock);
    free(pool->firstblock);
    pool->firstblock = pool->nowblock;
  }
} // triPoolDeinit
//------------------------------------------------------------------------------
// FUNCTION  triPoolInit
/// \brief   Initialize a pool of memory for allocation of items.
// NOTES     This routine initializes machinery for allocating items.
//           A `pool' is created with records of at least `bytecount' bytes.
//           Items will be allocated in `itemcount'- blocks.
//           Each item is assumed to be a collection of words, and either
//             pointers or floating-point values are assumed to be the
//             "primary" word type.  (The "primary" word type is used
//             to determine alignment of items.)
//           If `alignment' isn't zero, all items will be `alignment'-byte
//             aligned in memory.  `alignment' must be either a multiple or
//             a factor of the primary word size; powers of two are safe.
//           `alignment' is normally used to create a few unused bits at the
//             bottom of each item's pointer to store information.
// Don't change this routine unless you understand it.
//------------------------------------------------------------------------------
static bool triPoolInit(Tmemtype pool,
                        int bytecount,
                        int itemcount,
                        int firstitemcount,
                        int alignment)
{
  /* Find alignment to avoid unaligned access      *
   * Must be at least as large as:                 *
   *  -`alignment'                                 *
   *  - primary word type,                         *
   *  - sizeof(int*), to maintain dead item stack */
  if (alignment > sizeof(int*))
  {
    pool->alignbytes = alignment;
  }
  else
  {
    pool->alignbytes = sizeof(int*);
  }
  pool->itembytes = ((bytecount - 1) / pool->alignbytes + 1) * pool->alignbytes;
  pool->itemsperblock = itemcount;
  if (firstitemcount == 0)
  {
    pool->itemsfirstblock = itemcount;
  }
  else
  {
    pool->itemsfirstblock = firstitemcount;
  }
  /* Allocate block of items. Space for:           *
   *   `itemsperblock' items                       *
   *   1 ptr (to point to next block)              *
   *   space to ensure alignment of the items      */
  pool->firstblock =
    (int**)malloc(pool->itemsfirstblock * pool->itembytes + sizeof(int*) + pool->alignbytes);
  if (pool->firstblock == nullptr)
  {
    std::string s = "malloc failed: ";
    s += __FUNCTION__;
    XM_LOG(xmlog::error, s);
    return false;
  }
  /* Set the next block pointer to NULL. */
  *(pool->firstblock) = nullptr;
  /* Initialize the pool */
  triPoolRestart(pool);
  return true;
} // triPoolInit
//------------------------------------------------------------------------------
// FUNCTION triPoolRestart
/// \brief  Generate a random number between 0 and `choices' - 1.
// NOTES
//  This is a simple linear congruential random number generator.  Hence, it
//  is a bad random number generator, but good enough for most randomized
//  geometric algorithms.
//------------------------------------------------------------------------------
static void triPoolRestart(Tmemtype pool)
{
  unsigned long long alignptr;

  pool->items = 0;
  pool->maxitems = 0;

  /* Set the currently active block. */
  pool->nowblock = pool->firstblock;
  /* Find the first item in the pool.  Increment by the size of (VOID *). */
  alignptr = (unsigned long long)(pool->nowblock + 1);
  /* Align the item on an `alignbytes'-byte boundary. */
  pool->nextitem = (int*)(alignptr + (unsigned long long)pool->alignbytes -
                          (alignptr % (unsigned long long)pool->alignbytes));
  /* There are lots of unallocated items left in this block. */
  pool->unallocateditems = pool->itemsfirstblock;
  /* The stack of deallocated items is empty. */
  pool->deaditemstack = nullptr;

} // triPoolRestart
//------------------------------------------------------------------------------
// FUNCTION triRandomnation
/// \brief  Generate a random number between 0 and `choices' - 1.
// NOTES
//  This is a simple linear congruential random number generator.  Hence, it
//  is a bad random number generator, but good enough for most randomized
//  geometric algorithms.
//------------------------------------------------------------------------------
static unsigned long triRandomnation(unsigned int choices)
{
  static unsigned long randomseed = 1;

  if (choices == 0)
    return 0;
  randomseed = (randomseed * 1366l + 150889l) % 714025l;
  return randomseed / (714025l / choices + 1);
} // triRandomnation
//------------------------------------------------------------------------------
// FUNCTION triRemoveGhosts
/// \brief
// NOTES
//------------------------------------------------------------------------------
static void triRemoveGhosts(Tedgetype startghost, TriVars& t)
{
  Tedge searchedge, dissolveedge, deadtri;
  Tpt markorg;
  Ttri ptr; /* temp variable used by sym() */
            /* Find a starting edge on the convex hull */
  lprev(*startghost, searchedge);
  symself(searchedge);
  t.m_dummytri[0] = encode(searchedge);
  /* Remove bounding box & count convex hull edges */
  tedgecopy(*startghost, dissolveedge);
  do
  {
    lnext(dissolveedge, deadtri);
    lprevself(dissolveedge);
    symself(dissolveedge);
    /* If no PSLG is involved, set the boundary markers of all the points */
    /*   on the convex hull.  If a PSLG is used, this step is done later. */
    /* Watch out for the all collinear input case */
    if (dissolveedge.tri != t.m_dummytri)
    {
      org(dissolveedge, markorg);
      if (pointmark(markorg, t.m_pointmarkindex) == 0)
      {
        setpointmark(markorg, 1, t.m_pointmarkindex);
      }
    }
    /* Remove a bounding tri from a convex hull tri */
    dissolveedge.tri[dissolveedge.orient] = (Ttri)t.m_dummytri;
    /* Find the next bounding triangle */
    sym(deadtri, dissolveedge);
    /* Delete the bounding triangle */
    triTriangleDealloc(deadtri.tri, t);
  } while (!tedgeequal(dissolveedge, *startghost));
} // triRemoveGhosts
//------------------------------------------------------------------------------
// FUNCTION triScaleExpansionZeroElim
/// \brief  Multiply an expansion by a scalar, eliminating zero components from
//          the output expansion.
// NOTES    Sets h = be.  See my Robust Predicates paper for details.
//
//  Maintains the nonoverlapping property.  If round-to-even is used (as
//  with IEEE 754), maintains the strongly nonoverlapping and nonadjacent
//  properties as well.  (That is, if e has one of these properties, so
//  will h.) e and h cannot be the same.
//------------------------------------------------------------------------------
static int triScaleExpansionZeroElim(int elen, double* e, double b, double* h, TriVars& t)
{
  int eindex, hindex;
  double Q, sum;
  double hh;
  double product1, product0;
  double enow;
  double bvirt, avirt, bround, around;
  double c;
  double abig, ahi, alo, bhi, blo;
  double err1, err2, err3;

  Split(t.m_splitter, b, bhi, blo);
  Two_Product_Presplit(t.m_splitter, e[0], b, bhi, blo, Q, hh);
  hindex = 0;
  if (hh != 0)
    h[hindex++] = hh;

  for (eindex = 1; eindex < elen; eindex++)
  {
    enow = e[eindex];
    Two_Product_Presplit(t.m_splitter, enow, b, bhi, blo, product1, product0);
    Two_Sum(Q, product0, sum, hh);
    if (hh != 0)
      h[hindex++] = hh;
    Fast_Two_Sum(product1, sum, Q, hh);
    if (hh != 0)
      h[hindex++] = hh;
  }
  if ((Q != 0.0) || (hindex == 0))
    h[hindex++] = Q;
  return hindex;
} // triScaleExpansionZeroElim
//------------------------------------------------------------------------------
// FUNCTION triTraversalInit
/// \brief  Prepare to traverse the entire list of items.
// NOTES    This routine is used in conjunction with triTraverse().
//------------------------------------------------------------------------------
static void triTraversalInit(Tmemtype pool)
{
  unsigned long long alignptr;
  /* Begin the traversal in the first block */
  pool->pathblock = pool->firstblock;
  /* Find 1st item in block.  Inc by sizeof(int*) */
  alignptr = (unsigned long long)(pool->pathblock + 1);
  /* Align w/item on an `alignbytes'-byte boundary */
  pool->pathitem = (int*)(alignptr + (unsigned long long)pool->alignbytes -
                          (alignptr % (unsigned long long)pool->alignbytes));
  /* Update # of items left in the current block */
  pool->pathitemsleft = pool->itemsfirstblock;
} // triTraversalInit
//------------------------------------------------------------------------------
// FUNCTION triTraverse
/// \brief  Find the next item in the list.
// NOTES    This routine is used in conjunction with triTraversalInit().
//          Be forewarned that this routine successively returns all items in
//          the list, including deallocated ones on the deaditemqueue.  It's up
//          to you to figure out which ones are actually dead.  Why?  I don't
//          want to allocate extra space just to demarcate dead items.  It can
//          usually be done more space-efficiently by a routine that knows
//          something about the structure of the item.
//------------------------------------------------------------------------------
static int* triTraverse(Tmemtype pool)
{
  int* newitem;
  unsigned long long alignptr;
  /* Stop upon exhausting the list of items. */
  if (pool->pathitem == pool->nextitem)
    return nullptr;
  /* look for untraversed items in current block */
  if (pool->pathitemsleft == 0)
  {
    /* Find the next block. */
    pool->pathblock = (int**)*(pool->pathblock);
    /* Find 1st item in block.  Inc by sizeof(int*) */
    alignptr = (unsigned long long)(pool->pathblock + 1);
    /* Align w/item on `alignbytes'-byte boundary */
    pool->pathitem = (int*)(alignptr + (unsigned long long)pool->alignbytes -
                            (alignptr % (unsigned long long)pool->alignbytes));
    /* Update # of items left in the current block */
    pool->pathitemsleft = pool->itemsperblock;
  }
  newitem = pool->pathitem;
  /* Find the next item in the block. */

  pool->pathitem = (int*)((char*)pool->pathitem + pool->itembytes);
  pool->pathitemsleft--;
  return newitem;
} // triTraverse
//------------------------------------------------------------------------------
// FUNCTION triTriangleDealloc
/// \brief  Deallocate space for a triangle, marking it dead.
// NOTES    Makes it possible to detect dead triangles when traversing
//------------------------------------------------------------------------------
static void triTriangleDealloc(Ttri* dyingtriangle, TriVars& t)
{
  /* Set triangle's vertices to NULL */
  dyingtriangle[3] = nullptr;
  dyingtriangle[4] = nullptr;
  dyingtriangle[5] = nullptr;
  triPoolDealloc(&t.m_triangles, (int*)dyingtriangle);
} // triTriangleDealloc
//------------------------------------------------------------------------------
// FUNCTION triTriangleTraverse
/// \brief  Traverse the triangles, skipping dead ones.
// NOTES
//------------------------------------------------------------------------------
static Ttri* triTriangleTraverse(TriVars& t)
{
  Ttri* newtriangle;
  do
  {
    newtriangle = (Ttri*)triTraverse(&t.m_triangles);
    if (newtriangle == nullptr)
      return nullptr;
  } while (newtriangle[3] == nullptr); /* Skip dead ones */
  return newtriangle;
} // triTriangleTraverse

} // namespace unnamed
/// \endcond

namespace xms
{
//----- Class Definitions ------------------------------------------------------

//----- Function Definitions ---------------------------------------------------

//------------------------------------------------------------------------------
/// \brief Do delaunay divide-and-conquer triangulation
///
/// This algorithm can leave triangles that are VERY long and thin.
/// The default ratio should be sufficient to get rid of these (on
/// the exterior only).  If you want to supply a bigger ratio to get
/// rid of more triangles, you can, or you can do it on your own
/// after you call this function.
/// \param[in,out] a_Client: Class used to help do the triangulation.
/// \return true if no errors.
//------------------------------------------------------------------------------
bool trTriangulateIt(TrTriangulator& a_Client)
{
  TriVars t;
  try
  {
    // initialize counters
    t.m_points.maxitems = t.m_triangles.maxitems = 0l;
    t.m_points.itembytes = t.m_triangles.itembytes = 0;

    // initialize exact arithmetic constants
    triExactInit(t);

    // copy points into pool data
    if (!triGetPoints(a_Client, t))
    {
      throw - 1;
    }

    if (t.m_numpoints > 2)
    {
      // initialize the triangle pool
      triInitTrianglePool(t);

      // do the triangulation
      bool ok = triDivConqDelaunay(t);
      if (ok)
      {
        triNumberNodes(t);
        triFillTriList(a_Client, t);

        // free the memory
        triPoolDeinit(&t.m_triangles);
        if (t.m_dummytribase)
        {
          free(t.m_dummytribase);
        }
      }
    }

    // more memory clean up
    triPoolDeinit(&t.m_points);

    a_Client.FinalizeTriangulation();
  }
  catch (...)
  {
    XM_LOG(xmlog::error, "Error: Unable to triangulate.");
    triPoolDeinit(&t.m_triangles);
    triPoolDeinit(&t.m_points);
    return false;
  }
  return true;
} // trTriangulateIt

} // namespace xms

// eof triangulate.cpp
