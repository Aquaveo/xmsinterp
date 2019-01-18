<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>geoms.cpp</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>geoms_8cpp</filename>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="_gm_polygon_8h" name="GmPolygon.h" local="no" imported="no">xmsinterp/geometry/GmPolygon.h</includes>
    <includes id="triangles_8h" name="triangles.h" local="no" imported="no">xmsinterp/triangulate/triangles.h</includes>
    <includes id="_gm_polygon_8t_8h" name="GmPolygon.t.h" local="no" imported="no">xmsinterp/geometry/GmPolygon.t.h</includes>
    <includes id="geoms_8t_8h" name="geoms.t.h" local="no" imported="no">xmsinterp/geometry/geoms.t.h</includes>
    <class kind="class">GmPointInPolyTester_gmPointInPolygon2D</class>
    <member kind="function">
      <type>bool</type>
      <name>gmPointInOrOnBox2d</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a1460ad596726029e888ef3a0c67cb3ec</anchor>
      <arglist>(const Pt3d &amp;a_bMin, const Pt3d &amp;a_bMax, const Pt3d &amp;a_pt)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmBoxesOverlap2d</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a0a8640978d427b51b4ba6c217bb47b12</anchor>
      <arglist>(const Pt3d &amp;a_b1Min, const Pt3d &amp;a_b1Max, const Pt3d &amp;a_b2Min, const Pt3d &amp;a_b2Max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmCalculateNormalizedPlaneCoefficients</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a815c09c53387db67255d3dce98695778</anchor>
      <arglist>(const Pt3d &amp;p1, const Pt3d &amp;p2, const Pt3d &amp;p3, double *a, double *b, double *c, double *d)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmCalculateNormalizedPlaneCoefficients</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ad18b77815c5b85e99429ee9240768473</anchor>
      <arglist>(const Pt3d *p1, const Pt3d *p2, const Pt3d *p3, double *a, double *b, double *c, double *d)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmMiddleZ</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aca28f3227dce2e445d25080fbecbc2c6</anchor>
      <arglist>(const VecPt3d &amp;a_points)</arglist>
    </member>
    <member kind="function">
      <type>PtInOutOrOn_enum</type>
      <name>gmPtInCircumcircle</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ade7879f66ffa765c6ee005573218a4cd</anchor>
      <arglist>(const Pt3d &amp;pt, Pt3d circumcirclePts[3])</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmXyDistanceSquared</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>abfc383f1bd2f48276705d624db7144c8</anchor>
      <arglist>(const Pt3d &amp;pt1, const Pt3d &amp;pt2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmCircumcircleWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a72205354f6d55c7907f6badfbc2e08b0</anchor>
      <arglist>(const Pt3d *pt1, const Pt3d *pt2, const Pt3d *pt3, double *xc, double *yc, double *r2, double tol)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmCartToBary</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ad512367ef48bf109240552ff3c30cc7a</anchor>
      <arglist>(const Pt3d *cart, const Pt3d *orig, double coef[6], int dir, Pt3d *bary)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmBaryPrepare</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a065cd6d71205487312c1e03496f5ca08</anchor>
      <arglist>(const Pt3d *p1, const Pt3d *p2, const Pt3d *p3, const Pt3d *norm, Pt3d *orig, double coef[6], int *dir, bool flag)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmColinearWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a546070340fe9bdf56641efa002a27ac9</anchor>
      <arglist>(const Pt3d &amp;p1, const Pt3d &amp;p2, const Pt3d &amp;p3, const double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmIntersectLineSegmentsWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ac96bb6c54af013c284ac2c3657626c67</anchor>
      <arglist>(const Pt3d &amp;one1, const Pt3d &amp;one2, const Pt3d &amp;two1, const Pt3d &amp;two2, double *xi, double *yi, double *zi1, double *zi2, double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmCounterClockwiseTri</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a098ef608f7092e087f9a456709714fea</anchor>
      <arglist>(const Pt3d &amp;vtx0, const Pt3d &amp;vtx1, const Pt3d &amp;vtx2)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmCross2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a06f4c8c6bc396759e5d8953d85530b46</anchor>
      <arglist>(const double &amp;dx1, const double &amp;dy1, const double &amp;dx2, const double &amp;dy2)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmAngleBetween2DVectors</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aacf14a948fef6fc782b4e7210652ad35</anchor>
      <arglist>(double dxp, double dyp, double dxn, double dyn)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmAngleBetween2DVectors</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ae9ee7df906ccbedcb3b364d1a5571f60</anchor>
      <arglist>(double dxp, double dyp, double dxn, double dyn, double a_magn, double a_magp)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmAngleBetweenEdges</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ad5d19a71ee810d5a16ea601d9ca7dda8</anchor>
      <arglist>(const Pt3d &amp;p1, const Pt3d &amp;p2, const Pt3d &amp;p3)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmAngleBetweenEdges</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a93b051e3a396a16ae9a9a24b4350164c</anchor>
      <arglist>(const Pt2d &amp;p1, const Pt2d &amp;p2, const Pt2d &amp;p3)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmComputeDeviationInDirection</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a7d7bbeb7ef537825868d7e70021d918c</anchor>
      <arglist>(const Pt3d &amp;a_p0, const Pt3d &amp;a_p1, const Pt3d &amp;a_p2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmOnLineWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a45139efe30e550bbc370542f8e69a76f</anchor>
      <arglist>(const Pt3d &amp;p1, const Pt3d &amp;p2, const double x, const double y, const double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmOnLineAndBetweenEndpointsWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a169625a9f18e45ac2b67fac67c5db715</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, const double a_x, const double a_y, double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmAddToExtents</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a7b4416101125e2de1d4301a09f34cc4a</anchor>
      <arglist>(const Pt3d &amp;a_pt, Pt3d &amp;a_min, Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmAddToExtents</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a3d89b95b5652b82ece1415a73f6d102b</anchor>
      <arglist>(const Pt3d &amp;a_pt, Pt2d &amp;a_min, Pt2d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmAddToExtents</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a6c3d405fd7c3488c385b0caa932f9cba</anchor>
      <arglist>(const Pt2d &amp;a_pt, Pt3d &amp;a_min, Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmXyDistance</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a67d8975fe80c8ec9c9b424cf26e5e755</anchor>
      <arglist>(double x1, double y1, double x2, double y2)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmXyDistance</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a9a89d9544f63308d7a3c31e99c8281a9</anchor>
      <arglist>(const Pt3d &amp;pt1, const Pt3d &amp;pt2)</arglist>
    </member>
    <member kind="function">
      <type>Turn_enum</type>
      <name>gmTurn</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a4700adae0c06deec6e84e436c86f276b</anchor>
      <arglist>(const Pt3d &amp;a_v1, const Pt3d &amp;a_v2, const Pt3d &amp;a_v3, double a_angtol)</arglist>
    </member>
    <member kind="function">
      <type>Pt3d</type>
      <name>gmComputeCentroid</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ac462cb8b1fc78478a3f6cad9073a37e9</anchor>
      <arglist>(const VecPt3d &amp;a_points)</arglist>
    </member>
    <member kind="function">
      <type>Pt3d</type>
      <name>gmComputePolygonCentroid</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a728b2937701be72dc73f3f66ac2e7fbf</anchor>
      <arglist>(const VecPt3d &amp;pts)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmLinesIntersect</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>adc56691197d92d60abcb0e77e48df4ea</anchor>
      <arglist>(const Pt3d &amp;one1, const Pt3d &amp;one2, const Pt3d &amp;two1, const Pt3d &amp;two2)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a43bab17fe8d6f6b67de11a49e5127424</anchor>
      <arglist>(const T *a_verts, const size_t a_num, const double a_x, const double a_y, const double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a457f87dda88f474bebe491ec094b693f</anchor>
      <arglist>(const Pt3d *a_verts, size_t a_num, double a_x, double a_y)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aa32ddc6f5892464bfe6d596c1922f7f5</anchor>
      <arglist>(const Pt3d *a_verts, size_t a_num, Pt3d a_pt)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a959d71253e45c5f67e1bf202a5a122fd</anchor>
      <arglist>(const Pt2i *a_verts, size_t a_num, Pt2d a_pt)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aac4a8fdb18373864527f2fad0bb8c433</anchor>
      <arglist>(const Pt2i *a_verts, size_t a_num, Pt3d a_pt)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a3b879e6c80208a8780496137ac51c3c9</anchor>
      <arglist>(const Pt2i *a_verts, size_t a_num, Pt2i a_pt)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aad56bb08be2f1969cd7912f59d49d9e2</anchor>
      <arglist>(const VecPt3d &amp;a_verts, const Pt3d &amp;a_pt)</arglist>
    </member>
    <member kind="function">
      <type>template int</type>
      <name>gmPointInPolygon2D&lt; Pt3d &gt;</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a241b33f3ef73ed56ca73fdbb830e14a3</anchor>
      <arglist>(const Pt3d *a_verts, const size_t a_num, const double a_x, const double a_y, const double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>template int</type>
      <name>gmPointInPolygon2D&lt; Pt2d &gt;</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a84aea859e24101dbd69dc902d4f423f8</anchor>
      <arglist>(const Pt2d *a_verts, const size_t a_num, const double a_x, const double a_y, const double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>template int</type>
      <name>gmPointInPolygon2D&lt; Pt2i &gt;</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a296ceeadef9d9d4e4ba698f52c44c6c9</anchor>
      <arglist>(const Pt2i *a_verts, const size_t a_num, const double a_x, const double a_y, const double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmComputeXyTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a03126d64edc4ecb587a357fbfa5fb08c</anchor>
      <arglist>(const Pt3d &amp;a_mn, const Pt3d &amp;a_mx)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmXyTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a7a33258adddbd27ad36954f6ccdcdef7</anchor>
      <arglist>(bool a_set, double a_value)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmZTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>af0e7dba5dafe577d6345afe25580777a</anchor>
      <arglist>(bool a_set, double a_value)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXY</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a9a226e666728d9fde914f18db63a4714</anchor>
      <arglist>(double x1, double y1, double x2, double y2, double tolerance)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXY</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ac504f8315cad401c34447f06c5b6b07d</anchor>
      <arglist>(double x1, double y1, double x2, double y2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXY</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a427db5d209d5d8d22ec417417de4d702</anchor>
      <arglist>(const Pt2d &amp;a_pt1, const Pt2d &amp;a_pt2, double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXY</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a6724be333a3994b7cb0bbe1ec832f244</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXY</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a878ec06b0e9bbd3afccd2bf70ec54621</anchor>
      <arglist>(const Pt2i &amp;point1, const Pt2i &amp;point2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXYZ</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a8c3a08ccd01c4230e7498d793a9d90cd</anchor>
      <arglist>(double x1, double y1, double z1, double x2, double y2, double z2, double tolerance)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXYZ</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a6e4ba75c670a283360048a5365558916</anchor>
      <arglist>(double x1, double y1, double z1, double x2, double y2, double z2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXYZ</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a326b40120fd0517c654d9a94a963e83e</anchor>
      <arglist>(const Pt3d &amp;pt1, const Pt3d &amp;pt2, double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmPointInTriangleWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a5b2cb41a4510b0ba885d4a4691781506</anchor>
      <arglist>(const Pt3d *p1, const Pt3d *p2, const Pt3d *p3, double x, double y, double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmInsideOrOnLineWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a33f9850dc05d9598dcd1e4d06d7a64a2</anchor>
      <arglist>(const Pt3d *p1, const Pt3d *p2, const Pt3d *inpoint, const double x, const double y, const double tol, double *dist)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmPolygonArea</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a299579fe39539f353e245d1c41c8eb0f</anchor>
      <arglist>(const Pt3d *pts, size_t npoints)</arglist>
    </member>
    <member kind="function">
      <type>VecPt3d</type>
      <name>gmArrayToVecPt3d</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a7877ffa4620ae49b12d0b49de2289135</anchor>
      <arglist>(double *a_array, int a_size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmEnvelopeOfPts</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aeb0fecb3c9d9aa77f4ff42ec6a608f26</anchor>
      <arglist>(const VecPt3d &amp;a_pts, Pt3d &amp;a_min, Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmOrderPointsCounterclockwise</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ac3222874630da8f1f8537f1a6135846f</anchor>
      <arglist>(const VecPt3d &amp;a_pts, VecInt &amp;a_ccwOrder, int a_startindex)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmOrderPointsCounterclockwise</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aed9ee1170ee7004d626347f8886e4943</anchor>
      <arglist>(VecPt3d &amp;a_pts)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmFindClosestPtOnSegment</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ad89c693cc48610a4a7dcf89f22d55118</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, const Pt3d &amp;a_pt, Pt3d &amp;a_newpt, const double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmPtDistanceAlongSegment</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a9f9e75d979ef1bc5c0c217115eb34a04</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, const Pt3d &amp;a_pt, const double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmInsideOfLineWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aa78a4612aa080741e2ce7aa301aab615</anchor>
      <arglist>(const Pt3d &amp;a_vertex1, const Pt3d &amp;a_vertex2, const Pt3d &amp;a_oppositevertex, const double a_x, const double a_y, const double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmExtents2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a38a42e7eea0d9ce1dfebc230e50ad166</anchor>
      <arglist>(const VecPt3d &amp;a_points, Pt2d &amp;a_min, Pt2d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmExtents2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ac21eba0393f3071a3af665e8cfedb7bd</anchor>
      <arglist>(const VecPt3d &amp;a_points, Pt3d &amp;a_min, Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmExtents3D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ac4c2eb4bc4667e6479497b0b529562c8</anchor>
      <arglist>(const VecPt3d &amp;a_points, Pt3d &amp;a_min, Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmPerpendicularAngle</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a8397c11d92c81098edabbc41ab06bffa</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmBisectingAngle</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>afbc887b28d4ef88ce8c14aab5482765a</anchor>
      <arglist>(const Pt3d &amp;a_p1, const Pt3d &amp;a_p2, const Pt3d &amp;a_p3)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmComponentMagnitudes</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a214f89ef03c8dd4f0ca2e4418fc1757d</anchor>
      <arglist>(double *a_x, double *a_y, double *a_mag, double *a_dir, bool a_tomagdir)</arglist>
    </member>
    <member kind="function">
      <type>Pt3d</type>
      <name>gmCreateVector</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ae15d7a7439937df12789f169f594c3df</anchor>
      <arglist>(const Pt3d &amp;a_p1, const Pt3d &amp;a_p2)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmConvertAngleToBetween0And360</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a3de1bfdf51c7b4ee1b3c2a3f1ecb96ba</anchor>
      <arglist>(double a_angle, bool a_InDegrees)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmCross3D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>afff1edecd418a115ea72edd1fffc115e</anchor>
      <arglist>(const Pt3d &amp;a_vec1, const Pt3d &amp;a_vec2, Pt3d *a_vec3)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmDot3D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a02f5cd9ea7e783cdb00ba6ec4f88a357</anchor>
      <arglist>(const Pt3d &amp;a_vec1, const Pt3d &amp;a_vec2)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmIntersectTriangleAndLineSegment</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ab82e28a71a8e0794c21d756c2308fd3b</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, const Pt3d &amp;a_t0, const Pt3d &amp;a_t1, const Pt3d &amp;a_t2, Pt3d &amp;a_IntersectPt)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gm2DDistanceToLineWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a9684415f055c6e1f7b779d3a9e84f609</anchor>
      <arglist>(const Pt3d *a_pt1, const Pt3d *a_pt2, double a_x, double a_y, double a_tol)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>geoms.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>geoms_8h</filename>
    <member kind="enumeration">
      <type></type>
      <name>Turn_enum</name>
      <anchorfile>geoms_8h.html</anchorfile>
      <anchor>a0682e25caba7c1d65216a5680cc0217b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TURN_LEFT</name>
      <anchorfile>geoms_8h.html</anchorfile>
      <anchor>a0682e25caba7c1d65216a5680cc0217babc1acb082c93789cb378bf87597f42dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TURN_RIGHT</name>
      <anchorfile>geoms_8h.html</anchorfile>
      <anchor>a0682e25caba7c1d65216a5680cc0217ba4a1de04425d6c2953fbf40f2b29c15c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TURN_COLINEAR_180</name>
      <anchorfile>geoms_8h.html</anchorfile>
      <anchor>a0682e25caba7c1d65216a5680cc0217badbc22a212895c3de629b927b410a7065</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TURN_COLINEAR_0</name>
      <anchorfile>geoms_8h.html</anchorfile>
      <anchor>a0682e25caba7c1d65216a5680cc0217ba06d649a650f4acb061afb7acd5a2dbd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>PtInOutOrOn_enum</name>
      <anchorfile>geoms_8h.html</anchorfile>
      <anchor>a89f7d814569daefeabe9f5e3b5fbd213</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PT_ERROR</name>
      <anchorfile>geoms_8h.html</anchorfile>
      <anchor>a89f7d814569daefeabe9f5e3b5fbd213a75f63694d4c2fbd0065900d8a51dcf54</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PT_IN</name>
      <anchorfile>geoms_8h.html</anchorfile>
      <anchor>a89f7d814569daefeabe9f5e3b5fbd213a5f2058ee26998db92e7beedea5fd7e1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PT_OUT</name>
      <anchorfile>geoms_8h.html</anchorfile>
      <anchor>a89f7d814569daefeabe9f5e3b5fbd213af6c0802668cf9ce14590690ec216dc6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PT_ON</name>
      <anchorfile>geoms_8h.html</anchorfile>
      <anchor>a89f7d814569daefeabe9f5e3b5fbd213afef40dd1d13df2164ce1a2ba841a3b6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmPointInOrOnBox2d</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a1460ad596726029e888ef3a0c67cb3ec</anchor>
      <arglist>(const Pt3d &amp;a_bMin, const Pt3d &amp;a_bMax, const Pt3d &amp;a_pt)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmBoxesOverlap2d</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a0a8640978d427b51b4ba6c217bb47b12</anchor>
      <arglist>(const Pt3d &amp;a_b1Min, const Pt3d &amp;a_b1Max, const Pt3d &amp;a_b2Min, const Pt3d &amp;a_b2Max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmCalculateNormalizedPlaneCoefficients</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a815c09c53387db67255d3dce98695778</anchor>
      <arglist>(const Pt3d &amp;p1, const Pt3d &amp;p2, const Pt3d &amp;p3, double *a, double *b, double *c, double *d)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmCalculateNormalizedPlaneCoefficients</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ad18b77815c5b85e99429ee9240768473</anchor>
      <arglist>(const Pt3d *p1, const Pt3d *p2, const Pt3d *p3, double *a, double *b, double *c, double *d)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmMiddleZ</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aca28f3227dce2e445d25080fbecbc2c6</anchor>
      <arglist>(const VecPt3d &amp;a_points)</arglist>
    </member>
    <member kind="function">
      <type>PtInOutOrOn_enum</type>
      <name>gmPtInCircumcircle</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ade7879f66ffa765c6ee005573218a4cd</anchor>
      <arglist>(const Pt3d &amp;pt, Pt3d circumcirclePts[3])</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmXyDistanceSquared</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>abfc383f1bd2f48276705d624db7144c8</anchor>
      <arglist>(const Pt3d &amp;pt1, const Pt3d &amp;pt2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmCircumcircleWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a72205354f6d55c7907f6badfbc2e08b0</anchor>
      <arglist>(const Pt3d *pt1, const Pt3d *pt2, const Pt3d *pt3, double *xc, double *yc, double *r2, double tol)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmCartToBary</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ad512367ef48bf109240552ff3c30cc7a</anchor>
      <arglist>(const Pt3d *cart, const Pt3d *orig, double coef[6], int dir, Pt3d *bary)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmBaryPrepare</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a065cd6d71205487312c1e03496f5ca08</anchor>
      <arglist>(const Pt3d *p1, const Pt3d *p2, const Pt3d *p3, const Pt3d *norm, Pt3d *orig, double coef[6], int *dir, bool flag)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmColinearWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a546070340fe9bdf56641efa002a27ac9</anchor>
      <arglist>(const Pt3d &amp;p1, const Pt3d &amp;p2, const Pt3d &amp;p3, const double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmIntersectLineSegmentsWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ac96bb6c54af013c284ac2c3657626c67</anchor>
      <arglist>(const Pt3d &amp;one1, const Pt3d &amp;one2, const Pt3d &amp;two1, const Pt3d &amp;two2, double *xi, double *yi, double *zi1, double *zi2, double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmCounterClockwiseTri</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a098ef608f7092e087f9a456709714fea</anchor>
      <arglist>(const Pt3d &amp;vtx0, const Pt3d &amp;vtx1, const Pt3d &amp;vtx2)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmCross2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a06f4c8c6bc396759e5d8953d85530b46</anchor>
      <arglist>(const double &amp;dx1, const double &amp;dy1, const double &amp;dx2, const double &amp;dy2)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmAngleBetween2DVectors</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aacf14a948fef6fc782b4e7210652ad35</anchor>
      <arglist>(double dxp, double dyp, double dxn, double dyn)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmAngleBetween2DVectors</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ae9ee7df906ccbedcb3b364d1a5571f60</anchor>
      <arglist>(double dxp, double dyp, double dxn, double dyn, double a_magn, double a_magp)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmAngleBetweenEdges</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ad5d19a71ee810d5a16ea601d9ca7dda8</anchor>
      <arglist>(const Pt3d &amp;p1, const Pt3d &amp;p2, const Pt3d &amp;p3)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmAngleBetweenEdges</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a93b051e3a396a16ae9a9a24b4350164c</anchor>
      <arglist>(const Pt2d &amp;p1, const Pt2d &amp;p2, const Pt2d &amp;p3)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmComputeDeviationInDirection</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a7d7bbeb7ef537825868d7e70021d918c</anchor>
      <arglist>(const Pt3d &amp;a_p0, const Pt3d &amp;a_p1, const Pt3d &amp;a_p2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmOnLineAndBetweenEndpointsWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a169625a9f18e45ac2b67fac67c5db715</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, const double a_x, const double a_y, double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmOnLineWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a45139efe30e550bbc370542f8e69a76f</anchor>
      <arglist>(const Pt3d &amp;p1, const Pt3d &amp;p2, const double x, const double y, const double tol)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmAddToExtents</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a7b4416101125e2de1d4301a09f34cc4a</anchor>
      <arglist>(const Pt3d &amp;a_pt, Pt3d &amp;a_min, Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmAddToExtents</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a3d89b95b5652b82ece1415a73f6d102b</anchor>
      <arglist>(const Pt3d &amp;a_pt, Pt2d &amp;a_min, Pt2d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmAddToExtents</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a6c3d405fd7c3488c385b0caa932f9cba</anchor>
      <arglist>(const Pt2d &amp;a_pt, Pt3d &amp;a_min, Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmXyDistance</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a67d8975fe80c8ec9c9b424cf26e5e755</anchor>
      <arglist>(double x1, double y1, double x2, double y2)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmXyDistance</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a9a89d9544f63308d7a3c31e99c8281a9</anchor>
      <arglist>(const Pt3d &amp;pt1, const Pt3d &amp;pt2)</arglist>
    </member>
    <member kind="function">
      <type>Turn_enum</type>
      <name>gmTurn</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a4700adae0c06deec6e84e436c86f276b</anchor>
      <arglist>(const Pt3d &amp;a_v1, const Pt3d &amp;a_v2, const Pt3d &amp;a_v3, double a_angtol)</arglist>
    </member>
    <member kind="function">
      <type>Pt3d</type>
      <name>gmComputeCentroid</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ac462cb8b1fc78478a3f6cad9073a37e9</anchor>
      <arglist>(const VecPt3d &amp;a_points)</arglist>
    </member>
    <member kind="function">
      <type>Pt3d</type>
      <name>gmComputePolygonCentroid</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a728b2937701be72dc73f3f66ac2e7fbf</anchor>
      <arglist>(const VecPt3d &amp;pts)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmLinesIntersect</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>adc56691197d92d60abcb0e77e48df4ea</anchor>
      <arglist>(const Pt3d &amp;one1, const Pt3d &amp;one2, const Pt3d &amp;two1, const Pt3d &amp;two2)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a457f87dda88f474bebe491ec094b693f</anchor>
      <arglist>(const Pt3d *a_verts, size_t a_num, double a_x, double a_y)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aa32ddc6f5892464bfe6d596c1922f7f5</anchor>
      <arglist>(const Pt3d *a_verts, size_t a_num, Pt3d a_pt)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a959d71253e45c5f67e1bf202a5a122fd</anchor>
      <arglist>(const Pt2i *a_verts, size_t a_num, Pt2d a_pt)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a3b879e6c80208a8780496137ac51c3c9</anchor>
      <arglist>(const Pt2i *a_verts, size_t a_num, Pt2i a_pt)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aac4a8fdb18373864527f2fad0bb8c433</anchor>
      <arglist>(const Pt2i *a_verts, size_t a_num, Pt3d a_pt)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aad56bb08be2f1969cd7912f59d49d9e2</anchor>
      <arglist>(const VecPt3d &amp;a_verts, const Pt3d &amp;a_pt)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmPointInPolygon2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a43bab17fe8d6f6b67de11a49e5127424</anchor>
      <arglist>(const T *a_verts, const size_t a_num, const double a_x, const double a_y, const double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmComputeXyTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a03126d64edc4ecb587a357fbfa5fb08c</anchor>
      <arglist>(const Pt3d &amp;a_mn, const Pt3d &amp;a_mx)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmXyTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a7a33258adddbd27ad36954f6ccdcdef7</anchor>
      <arglist>(bool a_set, double a_value)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmZTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>af0e7dba5dafe577d6345afe25580777a</anchor>
      <arglist>(bool a_set, double a_value)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXY</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ac504f8315cad401c34447f06c5b6b07d</anchor>
      <arglist>(double x1, double y1, double x2, double y2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXY</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a9a226e666728d9fde914f18db63a4714</anchor>
      <arglist>(double x1, double y1, double x2, double y2, double tolerance)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXY</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a878ec06b0e9bbd3afccd2bf70ec54621</anchor>
      <arglist>(const Pt2i &amp;point1, const Pt2i &amp;point2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXY</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a427db5d209d5d8d22ec417417de4d702</anchor>
      <arglist>(const Pt2d &amp;a_pt1, const Pt2d &amp;a_pt2, double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXY</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a6724be333a3994b7cb0bbe1ec832f244</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXYZ</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a6e4ba75c670a283360048a5365558916</anchor>
      <arglist>(double x1, double y1, double z1, double x2, double y2, double z2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXYZ</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a8c3a08ccd01c4230e7498d793a9d90cd</anchor>
      <arglist>(double x1, double y1, double z1, double x2, double y2, double z2, double tolerance)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmEqualPointsXYZ</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a326b40120fd0517c654d9a94a963e83e</anchor>
      <arglist>(const Pt3d &amp;pt1, const Pt3d &amp;pt2, double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmPointInTriangleWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a5b2cb41a4510b0ba885d4a4691781506</anchor>
      <arglist>(const Pt3d *p1, const Pt3d *p2, const Pt3d *p3, double x, double y, double tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmInsideOrOnLineWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a33f9850dc05d9598dcd1e4d06d7a64a2</anchor>
      <arglist>(const Pt3d *p1, const Pt3d *p2, const Pt3d *inpoint, const double x, const double y, const double tol, double *dist)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmPolygonArea</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a299579fe39539f353e245d1c41c8eb0f</anchor>
      <arglist>(const Pt3d *pts, size_t npoints)</arglist>
    </member>
    <member kind="function">
      <type>VecPt3d</type>
      <name>gmArrayToVecPt3d</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a7877ffa4620ae49b12d0b49de2289135</anchor>
      <arglist>(double *a_array, int a_size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmEnvelopeOfPts</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aeb0fecb3c9d9aa77f4ff42ec6a608f26</anchor>
      <arglist>(const VecPt3d &amp;a_pts, Pt3d &amp;a_min, Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmOrderPointsCounterclockwise</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ac3222874630da8f1f8537f1a6135846f</anchor>
      <arglist>(const VecPt3d &amp;a_pts, VecInt &amp;a_ccwOrder, int a_startindex)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmOrderPointsCounterclockwise</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aed9ee1170ee7004d626347f8886e4943</anchor>
      <arglist>(VecPt3d &amp;a_pts)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmFindClosestPtOnSegment</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ad89c693cc48610a4a7dcf89f22d55118</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, const Pt3d &amp;a_pt, Pt3d &amp;a_newpt, const double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmPtDistanceAlongSegment</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a9f9e75d979ef1bc5c0c217115eb34a04</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, const Pt3d &amp;a_pt, const double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gmInsideOfLineWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>aa78a4612aa080741e2ce7aa301aab615</anchor>
      <arglist>(const Pt3d &amp;a_vertex1, const Pt3d &amp;a_vertex2, const Pt3d &amp;a_oppositevertex, const double a_x, const double a_y, const double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmExtents2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a38a42e7eea0d9ce1dfebc230e50ad166</anchor>
      <arglist>(const VecPt3d &amp;a_points, Pt2d &amp;a_min, Pt2d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmExtents2D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ac21eba0393f3071a3af665e8cfedb7bd</anchor>
      <arglist>(const VecPt3d &amp;a_points, Pt3d &amp;a_min, Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmExtents3D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ac4c2eb4bc4667e6479497b0b529562c8</anchor>
      <arglist>(const VecPt3d &amp;a_points, Pt3d &amp;a_min, Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmPerpendicularAngle</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a8397c11d92c81098edabbc41ab06bffa</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmBisectingAngle</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>afbc887b28d4ef88ce8c14aab5482765a</anchor>
      <arglist>(const Pt3d &amp;a_p1, const Pt3d &amp;a_p2, const Pt3d &amp;a_p3)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmComponentMagnitudes</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a214f89ef03c8dd4f0ca2e4418fc1757d</anchor>
      <arglist>(double *a_x, double *a_y, double *a_mag, double *a_dir, bool a_tomagdir)</arglist>
    </member>
    <member kind="function">
      <type>Pt3d</type>
      <name>gmCreateVector</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ae15d7a7439937df12789f169f594c3df</anchor>
      <arglist>(const Pt3d &amp;a_p1, const Pt3d &amp;a_p2)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmConvertAngleToBetween0And360</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a3de1bfdf51c7b4ee1b3c2a3f1ecb96ba</anchor>
      <arglist>(double a_angle, bool a_InDegrees)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmCross3D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>afff1edecd418a115ea72edd1fffc115e</anchor>
      <arglist>(const Pt3d &amp;a_vec1, const Pt3d &amp;a_vec2, Pt3d *a_vec3)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gmDot3D</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a02f5cd9ea7e783cdb00ba6ec4f88a357</anchor>
      <arglist>(const Pt3d &amp;a_vec1, const Pt3d &amp;a_vec2)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>gmIntersectTriangleAndLineSegment</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>ab82e28a71a8e0794c21d756c2308fd3b</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, const Pt3d &amp;a_t0, const Pt3d &amp;a_t1, const Pt3d &amp;a_t2, Pt3d &amp;a_IntersectPt)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>gm2DDistanceToLineWithTol</name>
      <anchorfile>geoms_8cpp.html</anchorfile>
      <anchor>a9684415f055c6e1f7b779d3a9e84f609</anchor>
      <arglist>(const Pt3d *a_pt1, const Pt3d *a_pt2, double a_x, double a_y, double a_tol)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>geoms.t.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>geoms_8t_8h</filename>
    <class kind="class">GeomsXmsngUnitTests</class>
    <class kind="class">GmPointInPolyUnitTests</class>
    <class kind="class">GeomsXmsngIntermediateTests</class>
  </compound>
  <compound kind="file">
    <name>GmBoostTypes.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_boost_types_8h</filename>
    <member kind="typedef">
      <type>boost::geometry::model::d2::point_xy&lt; double &gt;</type>
      <name>GmBstPt2d</name>
      <anchorfile>_gm_boost_types_8h.html</anchorfile>
      <anchor>a5f1677620ca85b6bcc1065b757f3e337</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>boost::geometry::model::polygon&lt; GmBstPt2d &gt;</type>
      <name>GmBstPoly2d</name>
      <anchorfile>_gm_boost_types_8h.html</anchorfile>
      <anchor>a93e3d438c33ced49f413cd84323982d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>boost::geometry::model::polygon&lt; Pt3d &gt;</type>
      <name>GmBstPoly3d</name>
      <anchorfile>_gm_boost_types_8h.html</anchorfile>
      <anchor>a05c9c43c740eaf5b56f436ec992dca5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>boost::geometry::model::box&lt; Pt3d &gt;</type>
      <name>GmBstBox3d</name>
      <anchorfile>_gm_boost_types_8h.html</anchorfile>
      <anchor>af7ac61e434c47abf90a1349336f7e8b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>boost::geometry::model::linestring&lt; Pt3d &gt;</type>
      <name>GmBstLine3d</name>
      <anchorfile>_gm_boost_types_8h.html</anchorfile>
      <anchor>a9bb6715848b64f01474619d28cd619d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>boost::geometry::ring_type&lt; GmBstPoly3d &gt;::type</type>
      <name>GmBstRing3d</name>
      <anchorfile>_gm_boost_types_8h.html</anchorfile>
      <anchor>a4d04b2e2f10cd613e47211b9ac69784e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>GmExtents.cpp</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_extents_8cpp</filename>
    <includes id="_gm_extents_8h" name="GmExtents.h" local="no" imported="no">xmsinterp/geometry/GmExtents.h</includes>
    <includes id="_gm_extents_8t_8h" name="GmExtents.t.h" local="no" imported="no">xmsinterp/geometry/GmExtents.t.h</includes>
  </compound>
  <compound kind="file">
    <name>GmExtents.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_extents_8h</filename>
    <class kind="class">xms::GmExtents2d</class>
    <class kind="class">xms::GmExtents3d</class>
  </compound>
  <compound kind="file">
    <name>GmExtents.t.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_extents_8t_8h</filename>
    <class kind="class">GmExtents3dUnitTests</class>
  </compound>
  <compound kind="file">
    <name>GmMultiPolyIntersectionSorter.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_multi_poly_intersection_sorter_8h</filename>
    <class kind="class">xms::GmMultiPolyIntersectionSorter</class>
  </compound>
  <compound kind="file">
    <name>GmMultiPolyIntersectionSorterTerse.cpp</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_multi_poly_intersection_sorter_terse_8cpp</filename>
    <includes id="_gm_multi_poly_intersection_sorter_terse_8h" name="GmMultiPolyIntersectionSorterTerse.h" local="no" imported="no">xmsinterp/geometry/GmMultiPolyIntersectionSorterTerse.h</includes>
    <includes id="_gm_multi_poly_intersector_data_8h" name="GmMultiPolyIntersectorData.h" local="no" imported="no">xmsinterp/geometry/GmMultiPolyIntersectorData.h</includes>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
  </compound>
  <compound kind="file">
    <name>GmMultiPolyIntersectionSorterTerse.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_multi_poly_intersection_sorter_terse_8h</filename>
    <includes id="_gm_multi_poly_intersection_sorter_8h" name="GmMultiPolyIntersectionSorter.h" local="no" imported="no">xmsinterp/geometry/GmMultiPolyIntersectionSorter.h</includes>
    <class kind="class">xms::GmMultiPolyIntersectionSorterTerse</class>
  </compound>
  <compound kind="file">
    <name>GmMultiPolyIntersector.cpp</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_multi_poly_intersector_8cpp</filename>
    <includes id="_gm_multi_poly_intersector_8h" name="GmMultiPolyIntersector.h" local="no" imported="no">xmsinterp/geometry/GmMultiPolyIntersector.h</includes>
    <includes id="_gm_boost_types_8h" name="GmBoostTypes.h" local="no" imported="no">xmsinterp/geometry/GmBoostTypes.h</includes>
    <includes id="_gm_multi_poly_intersection_sorter_8h" name="GmMultiPolyIntersectionSorter.h" local="no" imported="no">xmsinterp/geometry/GmMultiPolyIntersectionSorter.h</includes>
    <includes id="_gm_multi_poly_intersector_data_8h" name="GmMultiPolyIntersectorData.h" local="no" imported="no">xmsinterp/geometry/GmMultiPolyIntersectorData.h</includes>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="_gm_multi_poly_intersector_8t_8h" name="GmMultiPolyIntersector.t.h" local="no" imported="no">xmsinterp/geometry/GmMultiPolyIntersector.t.h</includes>
    <includes id="_gm_multi_poly_intersection_sorter_terse_8h" name="GmMultiPolyIntersectionSorterTerse.h" local="no" imported="no">xmsinterp/geometry/GmMultiPolyIntersectionSorterTerse.h</includes>
    <includes id="triangles_8h" name="triangles.h" local="no" imported="no">xmsinterp/triangulate/triangles.h</includes>
    <class kind="class">xms::GmMultiPolyIntersectorImpl</class>
    <member kind="typedef">
      <type>std::pair&lt; GmBstBox3d, int &gt;</type>
      <name>ValueBox</name>
      <anchorfile>_gm_multi_poly_intersector_8cpp.html</anchorfile>
      <anchor>ad371b9317cd9cab40e2625cb4e35df05</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>bgi::rtree&lt; ValueBox, bgi::quadratic&lt; 8 &gt; &gt;</type>
      <name>RtreeBox</name>
      <anchorfile>_gm_multi_poly_intersector_8cpp.html</anchorfile>
      <anchor>aa547c53279be0830c2afc7536f557044</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>GmMultiPolyIntersector.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_multi_poly_intersector_8h</filename>
    <class kind="class">xms::GmMultiPolyIntersector</class>
    <member kind="enumeration">
      <type></type>
      <name>GmMultiPolyIntersectorQueryEnum</name>
      <anchorfile>_gm_multi_poly_intersector_8h.html</anchorfile>
      <anchor>af3e9dae61009fe77b79327ee8ebe9b2e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>GmMultiPolyIntersector.t.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_multi_poly_intersector_8t_8h</filename>
    <class kind="class">GmMultiPolyIntersectorUnitTests</class>
    <class kind="class">GmMultiPolyIntersector2IntermediateTests</class>
  </compound>
  <compound kind="file">
    <name>GmMultiPolyIntersectorData.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_multi_poly_intersector_data_8h</filename>
    <class kind="class">xms::ix</class>
    <class kind="struct">xms::GmMultiPolyIntersectorData</class>
  </compound>
  <compound kind="file">
    <name>GmPolygon.cpp</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_polygon_8cpp</filename>
    <includes id="_gm_polygon_8h" name="GmPolygon.h" local="no" imported="no">xmsinterp/geometry/GmPolygon.h</includes>
    <includes id="_gm_boost_types_8h" name="GmBoostTypes.h" local="no" imported="no">xmsinterp/geometry/GmBoostTypes.h</includes>
    <includes id="_gm_polygon_8t_8h" name="GmPolygon.t.h" local="no" imported="no">xmsinterp/geometry/GmPolygon.t.h</includes>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="geoms_8t_8h" name="geoms.t.h" local="no" imported="no">xmsinterp/geometry/geoms.t.h</includes>
    <class kind="class">xms::GmPolygonImpl</class>
    <class kind="class">GmPointInPolyTester_GmPolygon</class>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>iDistanceToRing</name>
      <anchorfile>_gm_polygon_8cpp.html</anchorfile>
      <anchor>a99db03c7ac0beff008fd1d9b5795f7cb</anchor>
      <arglist>(const GmBstRing3d &amp;a_ring, const Pt3d &amp;a_pt)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>GmPolygon.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_polygon_8h</filename>
    <class kind="class">xms::GmPolygon</class>
  </compound>
  <compound kind="file">
    <name>GmPolygon.t.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_polygon_8t_8h</filename>
    <class kind="class">GmPolygonUnitTests</class>
    <class kind="class">GmPolygonIntermediateTests</class>
  </compound>
  <compound kind="file">
    <name>GmPolyLinePtRedistributer.cpp</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_poly_line_pt_redistributer_8cpp</filename>
    <includes id="_gm_poly_line_pt_redistributer_8h" name="GmPolyLinePtRedistributer.h" local="no" imported="no">xmsinterp/geometry/GmPolyLinePtRedistributer.h</includes>
    <includes id="_gm_poly_line_pt_redistributer_8t_8h" name="GmPolyLinePtRedistributer.t.h" local="no" imported="no">xmsinterp/geometry/GmPolyLinePtRedistributer.t.h</includes>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <class kind="class">xms::GmPolyLinePtRedistributerImpl</class>
  </compound>
  <compound kind="file">
    <name>GmPolyLinePtRedistributer.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_poly_line_pt_redistributer_8h</filename>
    <class kind="class">xms::GmPolyLinePtRedistributer</class>
  </compound>
  <compound kind="file">
    <name>GmPolyLinePtRedistributer.t.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_poly_line_pt_redistributer_8t_8h</filename>
    <class kind="class">GmPolyLinePtRedistributerUnitTests</class>
  </compound>
  <compound kind="file">
    <name>GmPtSearch.cpp</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_pt_search_8cpp</filename>
    <includes id="_gm_pt_search_8h" name="GmPtSearch.h" local="no" imported="no">xmsinterp/geometry/GmPtSearch.h</includes>
    <includes id="_gm_pt_search_8t_8h" name="GmPtSearch.t.h" local="no" imported="no">xmsinterp/geometry/GmPtSearch.t.h</includes>
    <class kind="class">xms::fSatisfies</class>
    <class kind="class">xms::idx_pt</class>
    <class kind="class">xms::GmPtSearchImpl</class>
    <member kind="define">
      <type>#define</type>
      <name>_TS_ASSERT_POINTS_FOUND</name>
      <anchorfile>_gm_pt_search_8cpp.html</anchorfile>
      <anchor>a8fbe8336b8c27d45710889fd77f4f9c6</anchor>
      <arglist>(a_file, a_line, a_required, a_optional, a_found)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TS_ASSERT_POINTS_FOUND</name>
      <anchorfile>_gm_pt_search_8cpp.html</anchorfile>
      <anchor>ab1484d89355833010a52b160bb7f1e64</anchor>
      <arglist>(a_required, a_optional, a_found)</arglist>
    </member>
    <member kind="typedef">
      <type>bg::model::box&lt; bPt &gt;</type>
      <name>box</name>
      <anchorfile>_gm_pt_search_8cpp.html</anchorfile>
      <anchor>af3aa99ee2ee766e1f9c1480f8f4fa2f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>size_t</type>
      <name>value</name>
      <anchorfile>_gm_pt_search_8cpp.html</anchorfile>
      <anchor>ac246bb2df10f44751d78d40e43d88da2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>bgi::quadratic&lt; 8 &gt;</type>
      <name>qRtree</name>
      <anchorfile>_gm_pt_search_8cpp.html</anchorfile>
      <anchor>a528acc66ce7a7889c76de85b6d5d5a23</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BOOST_GEOMETRY_REGISTER_POINT_3D</name>
      <anchorfile>_gm_pt_search_8cpp.html</anchorfile>
      <anchor>a1c1e59c4b2db3b2f5756f01c668924bb</anchor>
      <arglist>(bPt, double, cs::cartesian, x, y, z)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>iGetPoints2</name>
      <anchorfile>_gm_pt_search_8cpp.html</anchorfile>
      <anchor>a63c7b825306d0e4094c4f4a7cfca60da</anchor>
      <arglist>(std::vector&lt; Pt3d &gt; &amp;a_pts, std::vector&lt; float &gt; &amp;a_scalar, std::vector&lt; Pt3d &gt; &amp;a_iPts)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>iAssertPointsFound</name>
      <anchorfile>_gm_pt_search_8cpp.html</anchorfile>
      <anchor>ad94d7d97e0ce5c2bd382b874e657391b</anchor>
      <arglist>(const char *a_file, int a_line, const std::vector&lt; int &gt; &amp;a_required, const std::vector&lt; int &gt; &amp;a_optional, const std::vector&lt; int &gt; &amp;a_found)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>iSetupPts</name>
      <anchorfile>_gm_pt_search_8cpp.html</anchorfile>
      <anchor>a5a1426a86969ec4ea6e1ebb3bddf6809</anchor>
      <arglist>(std::vector&lt; Pt3d &gt; &amp;pts, bool a_2d)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>iSetupPtsOctant</name>
      <anchorfile>_gm_pt_search_8cpp.html</anchorfile>
      <anchor>a5a5d4b2dca9706e51c1c427ce4f8d058</anchor>
      <arglist>(std::vector&lt; Pt3d &gt; &amp;pts)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>GmPtSearch.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_pt_search_8h</filename>
    <class kind="class">xms::GmPtSearch</class>
  </compound>
  <compound kind="file">
    <name>GmPtSearch.t.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_pt_search_8t_8h</filename>
    <class kind="class">PtSearchUnitTests</class>
  </compound>
  <compound kind="file">
    <name>GmTriSearch.cpp</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_tri_search_8cpp</filename>
    <includes id="_gm_tri_search_8h" name="GmTriSearch.h" local="no" imported="no">xmsinterp/geometry/GmTriSearch.h</includes>
    <includes id="_gm_boost_types_8h" name="GmBoostTypes.h" local="no" imported="no">xmsinterp/geometry/GmBoostTypes.h</includes>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="_gm_pt_search_8h" name="GmPtSearch.h" local="no" imported="no">xmsinterp/geometry/GmPtSearch.h</includes>
    <includes id="_gm_tri_search_8t_8h" name="GmTriSearch.t.h" local="no" imported="no">xmsinterp/geometry/GmTriSearch.t.h</includes>
    <includes id="_tr_tin_8h" name="TrTin.h" local="no" imported="no">xmsinterp/triangulate/TrTin.h</includes>
    <includes id="_tr_tin_8t_8h" name="TrTin.t.h" local="no" imported="no">xmsinterp/triangulate/TrTin.t.h</includes>
    <class kind="class">xms::idx_tri</class>
    <class kind="struct">xms::BarycentricVals</class>
    <class kind="class">xms::GmTriSearchImpl</class>
    <member kind="define">
      <type>#define</type>
      <name>BARYTOL</name>
      <anchorfile>_gm_tri_search_8cpp.html</anchorfile>
      <anchor>a3dc37a63219351ec125b2545bbdcdda3</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>iCartToBary</name>
      <anchorfile>_gm_tri_search_8cpp.html</anchorfile>
      <anchor>af5d39c8118d6484eca6f5dc22cfe082a</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, const Pt3d &amp;a_pt3, BarycentricVals &amp;a_b)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>iGetBarycentricCoords</name>
      <anchorfile>_gm_tri_search_8cpp.html</anchorfile>
      <anchor>a4e909346571623b3e32310fd0877dea6</anchor>
      <arglist>(const Pt3d &amp;a_p, BarycentricVals &amp;a_b, Pt3d &amp;weights)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>GmTriSearch.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_tri_search_8h</filename>
    <class kind="class">xms::GmTriSearch</class>
  </compound>
  <compound kind="file">
    <name>GmTriSearch.t.h</name>
    <path>/home/conan/xmsinterp/geometry/</path>
    <filename>_gm_tri_search_8t_8h</filename>
    <class kind="class">TriSearchUnitTests</class>
  </compound>
  <compound kind="file">
    <name>AnisotropicInterpolator.cpp</name>
    <path>/home/conan/xmsinterp/interpolate/detail/</path>
    <filename>_anisotropic_interpolator_8cpp</filename>
    <includes id="_anisotropic_interpolator_8h" name="AnisotropicInterpolator.h" local="no" imported="no">xmsinterp/interpolate/detail/AnisotropicInterpolator.h</includes>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="_anisotropic_interpolator_8t_8h" name="AnisotropicInterpolator.t.h" local="no" imported="no">xmsinterp/interpolate/detail/AnisotropicInterpolator.t.h</includes>
    <member kind="variable">
      <type>const double</type>
      <name>kVERTICAL</name>
      <anchorfile>_anisotropic_interpolator_8cpp.html</anchorfile>
      <anchor>a98defd88c83c4631fe9ab59cf5ad208f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>AnisotropicInterpolator.h</name>
    <path>/home/conan/xmsinterp/interpolate/detail/</path>
    <filename>_anisotropic_interpolator_8h</filename>
    <class kind="struct">xms::SNResult</class>
    <class kind="struct">xms::LineParameters</class>
    <class kind="class">xms::AnisotropicInterpolator</class>
    <member kind="typedef">
      <type>std::vector&lt; SNResult &gt;</type>
      <name>VecSNResult</name>
      <anchorfile>_anisotropic_interpolator_8h.html</anchorfile>
      <anchor>a6cd6a805924ff382dbcbc74fc9e34b9c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>AnisotropicInterpolator.t.h</name>
    <path>/home/conan/xmsinterp/interpolate/detail/</path>
    <filename>_anisotropic_interpolator_8t_8h</filename>
    <class kind="class">AnisotropicInterpolatorUnitTests</class>
  </compound>
  <compound kind="file">
    <name>InterpCt.cpp</name>
    <path>/home/conan/xmsinterp/interpolate/detail/</path>
    <filename>_interp_ct_8cpp</filename>
    <includes id="_interp_ct_8h" name="InterpCt.h" local="no" imported="no">xmsinterp/interpolate/detail/InterpCt.h</includes>
    <includes id="_nodal_func_8h" name="NodalFunc.h" local="no" imported="no">xmsinterp/interpolate/detail/NodalFunc.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>SQRT3</name>
      <anchorfile>_interp_ct_8cpp.html</anchorfile>
      <anchor>ae42978afd835c3a1f70d409a1b5f5a39</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>InterpCt.h</name>
    <path>/home/conan/xmsinterp/interpolate/detail/</path>
    <filename>_interp_ct_8h</filename>
    <class kind="class">xms::InterpCt</class>
  </compound>
  <compound kind="file">
    <name>InterpNatNeigh.cpp</name>
    <path>/home/conan/xmsinterp/interpolate/detail/</path>
    <filename>_interp_nat_neigh_8cpp</filename>
    <includes id="_interp_nat_neigh_8h" name="InterpNatNeigh.h" local="no" imported="no">xmsinterp/interpolate/detail/InterpNatNeigh.h</includes>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="_gm_tri_search_8h" name="GmTriSearch.h" local="no" imported="no">xmsinterp/geometry/GmTriSearch.h</includes>
    <includes id="_nodal_func_8h" name="NodalFunc.h" local="no" imported="no">xmsinterp/interpolate/detail/NodalFunc.h</includes>
    <includes id="_interp_nat_neigh_8t_8h" name="InterpNatNeigh.t.h" local="no" imported="no">xmsinterp/interpolate/detail/InterpNatNeigh.t.h</includes>
    <class kind="struct">xms::nnOuterEdgeStruct</class>
    <class kind="class">xms::InterpNatNeighImpl</class>
  </compound>
  <compound kind="file">
    <name>InterpNatNeigh.h</name>
    <path>/home/conan/xmsinterp/interpolate/detail/</path>
    <filename>_interp_nat_neigh_8h</filename>
    <class kind="class">xms::InterpNatNeigh</class>
  </compound>
  <compound kind="file">
    <name>InterpNatNeigh.t.h</name>
    <path>/home/conan/xmsinterp/interpolate/detail/</path>
    <filename>_interp_nat_neigh_8t_8h</filename>
    <class kind="class">InterpNatNeighUnitTests</class>
  </compound>
  <compound kind="file">
    <name>NodalFunc.cpp</name>
    <path>/home/conan/xmsinterp/interpolate/detail/</path>
    <filename>_nodal_func_8cpp</filename>
    <includes id="_nodal_func_8h" name="NodalFunc.h" local="no" imported="no">xmsinterp/interpolate/detail/NodalFunc.h</includes>
    <includes id="_gm_pt_search_8h" name="GmPtSearch.h" local="no" imported="no">xmsinterp/geometry/GmPtSearch.h</includes>
    <includes id="_interp_util_8h" name="InterpUtil.h" local="no" imported="no">xmsinterp/interpolate/InterpUtil.h</includes>
    <includes id="_interp_nat_neigh_8h" name="InterpNatNeigh.h" local="no" imported="no">xmsinterp/interpolate/detail/InterpNatNeigh.h</includes>
    <includes id="matrix_8h" name="matrix.h" local="no" imported="no">xmsinterp/matrices/matrix.h</includes>
    <includes id="_thread_loop_8h" name="ThreadLoop.h" local="no" imported="no">xmsinterp/thread/ThreadLoop.h</includes>
    <includes id="_thread_mgr_8h" name="ThreadMgr.h" local="no" imported="no">xmsinterp/thread/ThreadMgr.h</includes>
    <includes id="_nodal_func_8t_8h" name="NodalFunc.t.h" local="no" imported="no">xmsinterp/interpolate/detail/NodalFunc.t.h</includes>
    <class kind="class">xms::NodalFuncImpl</class>
    <class kind="class">xms::NodalFuncImpl::NfThread</class>
  </compound>
  <compound kind="file">
    <name>NodalFunc.h</name>
    <path>/home/conan/xmsinterp/interpolate/detail/</path>
    <filename>_nodal_func_8h</filename>
    <class kind="class">xms::NodalFunc</class>
  </compound>
  <compound kind="file">
    <name>NodalFunc.t.h</name>
    <path>/home/conan/xmsinterp/interpolate/detail/</path>
    <filename>_nodal_func_8t_8h</filename>
    <class kind="class">NodalFuncUnitTests</class>
  </compound>
  <compound kind="file">
    <name>InterpAnisotropic.cpp</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_anisotropic_8cpp</filename>
    <includes id="_interp_anisotropic_8h" name="InterpAnisotropic.h" local="no" imported="no">xmsinterp/interpolate/InterpAnisotropic.h</includes>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="_anisotropic_interpolator_8h" name="AnisotropicInterpolator.h" local="no" imported="no">xmsinterp/interpolate/detail/AnisotropicInterpolator.h</includes>
    <includes id="_interp_anisotropic_8t_8h" name="InterpAnisotropic.t.h" local="no" imported="no">xmsinterp/interpolate/InterpAnisotropic.t.h</includes>
    <class kind="class">xms::InterpAnisotropicImpl</class>
  </compound>
  <compound kind="file">
    <name>InterpAnisotropic.h</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_anisotropic_8h</filename>
    <class kind="class">xms::InterpAnisotropic</class>
  </compound>
  <compound kind="file">
    <name>InterpAnisotropic.t.h</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_anisotropic_8t_8h</filename>
    <class kind="class">InterpAnisotropicUnitTests</class>
  </compound>
  <compound kind="file">
    <name>InterpBase.h</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_base_8h</filename>
    <class kind="class">xms::InterpBase</class>
  </compound>
  <compound kind="file">
    <name>InterpIdw.cpp</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_idw_8cpp</filename>
    <includes id="_interp_idw_8h" name="InterpIdw.h" local="no" imported="no">xmsinterp/interpolate/InterpIdw.h</includes>
    <includes id="_gm_pt_search_8h" name="GmPtSearch.h" local="no" imported="no">xmsinterp/geometry/GmPtSearch.h</includes>
    <includes id="_interp_signals_8h" name="InterpSignals.h" local="no" imported="no">xmsinterp/interpolate/InterpSignals.h</includes>
    <includes id="_interp_util_8h" name="InterpUtil.h" local="no" imported="no">xmsinterp/interpolate/InterpUtil.h</includes>
    <includes id="_nodal_func_8h" name="NodalFunc.h" local="no" imported="no">xmsinterp/interpolate/detail/NodalFunc.h</includes>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="_thread_loop_8h" name="ThreadLoop.h" local="no" imported="no">xmsinterp/thread/ThreadLoop.h</includes>
    <includes id="_thread_mgr_8h" name="ThreadMgr.h" local="no" imported="no">xmsinterp/thread/ThreadMgr.h</includes>
    <includes id="_interp_idw_8t_8h" name="InterpIdw.t.h" local="no" imported="no">xmsinterp/interpolate/InterpIdw.t.h</includes>
    <class kind="class">xms::InterpIdwImpl</class>
    <class kind="class">xms::InterpIdwImpl::InterpThread</class>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>iGetPoints</name>
      <anchorfile>_interp_idw_8cpp.html</anchorfile>
      <anchor>a5ecbe3f746652c820934c79ac25de71a</anchor>
      <arglist>(VecPt3d &amp;a_pts, VecFlt &amp;a_scalar, VecPt3d &amp;a_iPts)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>InterpIdw.h</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_idw_8h</filename>
    <includes id="_interp_base_8h" name="InterpBase.h" local="no" imported="no">xmsinterp/interpolate/InterpBase.h</includes>
    <class kind="class">xms::InterpIdw</class>
  </compound>
  <compound kind="file">
    <name>InterpIdw.t.h</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_idw_8t_8h</filename>
    <class kind="class">InterpIdwUnitTests</class>
  </compound>
  <compound kind="file">
    <name>InterpLinear.cpp</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_linear_8cpp</filename>
    <includes id="_interp_linear_8h" name="InterpLinear.h" local="no" imported="no">xmsinterp/interpolate/InterpLinear.h</includes>
    <includes id="_gm_pt_search_8h" name="GmPtSearch.h" local="no" imported="no">xmsinterp/geometry/GmPtSearch.h</includes>
    <includes id="_gm_tri_search_8h" name="GmTriSearch.h" local="no" imported="no">xmsinterp/geometry/GmTriSearch.h</includes>
    <includes id="_interp_ct_8h" name="InterpCt.h" local="no" imported="no">xmsinterp/interpolate/detail/InterpCt.h</includes>
    <includes id="_interp_nat_neigh_8h" name="InterpNatNeigh.h" local="no" imported="no">xmsinterp/interpolate/detail/InterpNatNeigh.h</includes>
    <includes id="_nodal_func_8h" name="NodalFunc.h" local="no" imported="no">xmsinterp/interpolate/detail/NodalFunc.h</includes>
    <includes id="_interp_util_8h" name="InterpUtil.h" local="no" imported="no">xmsinterp/interpolate/InterpUtil.h</includes>
    <includes id="_interp_linear_8t_8h" name="InterpLinear.t.h" local="no" imported="no">xmsinterp/interpolate/InterpLinear.t.h</includes>
    <class kind="class">xms::InterpLinearImpl</class>
  </compound>
  <compound kind="file">
    <name>InterpLinear.h</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_linear_8h</filename>
    <includes id="_interp_base_8h" name="InterpBase.h" local="no" imported="no">xmsinterp/interpolate/InterpBase.h</includes>
    <class kind="class">xms::InterpLinear</class>
  </compound>
  <compound kind="file">
    <name>InterpLinear.t.h</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_linear_8t_8h</filename>
    <class kind="class">InterpLinearUnitTests</class>
  </compound>
  <compound kind="file">
    <name>InterpSignals.cpp</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_signals_8cpp</filename>
    <includes id="_interp_signals_8h" name="InterpSignals.h" local="no" imported="no">xmsinterp/interpolate/InterpSignals.h</includes>
    <member kind="function">
      <type>boost::signals2::signal&lt; void(int)&gt; &amp;</type>
      <name>SetDataIndex</name>
      <anchorfile>_interp_signals_8cpp.html</anchorfile>
      <anchor>ab7b85b7e5ce60931d531baf5ca711659</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>boost::signals2::signal&lt; void(int, double)&gt; &amp;</type>
      <name>FillWtArray</name>
      <anchorfile>_interp_signals_8cpp.html</anchorfile>
      <anchor>aa928e314d18dd5ff6f135327da040c0b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>inSetDataIndex</name>
      <anchorfile>_interp_signals_8cpp.html</anchorfile>
      <anchor>a505f1a8118b4e0725a0430e343c5065c</anchor>
      <arglist>(int val)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>inFillWtArray</name>
      <anchorfile>_interp_signals_8cpp.html</anchorfile>
      <anchor>ac15113d908c8bfab21332a25f76cd786</anchor>
      <arglist>(int id, double wt)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>InterpSignals.h</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_signals_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>inSetDataIndex</name>
      <anchorfile>_interp_signals_8cpp.html</anchorfile>
      <anchor>a505f1a8118b4e0725a0430e343c5065c</anchor>
      <arglist>(int val)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>inFillWtArray</name>
      <anchorfile>_interp_signals_8cpp.html</anchorfile>
      <anchor>ac15113d908c8bfab21332a25f76cd786</anchor>
      <arglist>(int id, double wt)</arglist>
    </member>
    <member kind="function">
      <type>boost::signals2::connection</type>
      <name>SetDataIndex</name>
      <anchorfile>_interp_signals_8h.html</anchorfile>
      <anchor>a4fbf2e18074156b7bd66a4f5cee78088</anchor>
      <arglist>(const boost::signals2::signal&lt; void(int)&gt;::slot_type &amp;a_slot)</arglist>
    </member>
    <member kind="function">
      <type>boost::signals2::connection</type>
      <name>FillWtArray</name>
      <anchorfile>_interp_signals_8h.html</anchorfile>
      <anchor>ac93c018be7a62399d42b12a0ff565526</anchor>
      <arglist>(const boost::signals2::signal&lt; void(int, double)&gt;::slot_type &amp;a_slot)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>InterpUtil.cpp</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_util_8cpp</filename>
    <includes id="_interp_util_8h" name="InterpUtil.h" local="no" imported="no">xmsinterp/interpolate/InterpUtil.h</includes>
    <member kind="function">
      <type>void</type>
      <name>inNodalFuncSetUpMatrixAndVector</name>
      <anchorfile>_interp_util_8cpp.html</anchorfile>
      <anchor>a596dc1ea330f8f25b64d8b50ba61498f</anchor>
      <arglist>(double xk, double yk, double fk, const std::vector&lt; InterpPtInfo &gt; &amp;closest, double **M, double *VV)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>inNodalFuncSetUpMatrixAndVector3d</name>
      <anchorfile>_interp_util_8cpp.html</anchorfile>
      <anchor>aaa75bc946f62af5bc6cb7faaf0d9cbdc</anchor>
      <arglist>(double xk, double yk, double zk, double fk, const std::vector&lt; InterpPtInfo &gt; &amp;closest, double **M, double *vv)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>inDistanceSquared</name>
      <anchorfile>_interp_util_8cpp.html</anchorfile>
      <anchor>a45a66123476449da3230fca568f2efbb</anchor>
      <arglist>(const Pt3d &amp;a_pt, const std::vector&lt; int &gt; &amp;a_ptIdxs, const std::vector&lt; Pt3d &gt; &amp;a_ptLoc, bool a_2d, std::vector&lt; double &gt; &amp;a_d2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>inIdwWeights</name>
      <anchorfile>_interp_util_8cpp.html</anchorfile>
      <anchor>a51eb9958e3615ed089902e284df1b839</anchor>
      <arglist>(const std::vector&lt; double &gt; &amp;a_distSquare, double a_power, bool a_modifiedShepardWeights, std::vector&lt; double &gt; &amp;a_w)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>inAllScalarsEqual</name>
      <anchorfile>_interp_util_8cpp.html</anchorfile>
      <anchor>a0067a46b6f467053aeede80a6701afa0</anchor>
      <arglist>(const std::vector&lt; float &gt; &amp;a_scalars, const DynBitset &amp;a_act)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>InterpUtil.h</name>
    <path>/home/conan/xmsinterp/interpolate/</path>
    <filename>_interp_util_8h</filename>
    <class kind="struct">xms::InterpPtInfo</class>
    <member kind="function">
      <type>void</type>
      <name>inNodalFuncSetUpMatrixAndVector</name>
      <anchorfile>_interp_util_8cpp.html</anchorfile>
      <anchor>a596dc1ea330f8f25b64d8b50ba61498f</anchor>
      <arglist>(double xk, double yk, double fk, const std::vector&lt; InterpPtInfo &gt; &amp;closest, double **M, double *VV)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>inNodalFuncSetUpMatrixAndVector3d</name>
      <anchorfile>_interp_util_8cpp.html</anchorfile>
      <anchor>aaa75bc946f62af5bc6cb7faaf0d9cbdc</anchor>
      <arglist>(double xk, double yk, double zk, double fk, const std::vector&lt; InterpPtInfo &gt; &amp;closest, double **M, double *vv)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>inDistanceSquared</name>
      <anchorfile>_interp_util_8cpp.html</anchorfile>
      <anchor>a45a66123476449da3230fca568f2efbb</anchor>
      <arglist>(const Pt3d &amp;a_pt, const std::vector&lt; int &gt; &amp;a_ptIdxs, const std::vector&lt; Pt3d &gt; &amp;a_ptLoc, bool a_2d, std::vector&lt; double &gt; &amp;a_d2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>inIdwWeights</name>
      <anchorfile>_interp_util_8cpp.html</anchorfile>
      <anchor>a51eb9958e3615ed089902e284df1b839</anchor>
      <arglist>(const std::vector&lt; double &gt; &amp;a_distSquare, double a_power, bool a_modifiedShepardWeights, std::vector&lt; double &gt; &amp;a_w)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>inAllScalarsEqual</name>
      <anchorfile>_interp_util_8cpp.html</anchorfile>
      <anchor>a0067a46b6f467053aeede80a6701afa0</anchor>
      <arglist>(const std::vector&lt; float &gt; &amp;a_scalars, const DynBitset &amp;a_act)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>matrix.cpp</name>
    <path>/home/conan/xmsinterp/matrices/</path>
    <filename>matrix_8cpp</filename>
    <includes id="matrix_8h" name="matrix.h" local="no" imported="no">xmsinterp/matrices/matrix.h</includes>
    <member kind="function">
      <type>int</type>
      <name>mxLUDecomp</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>abafef2cc31ff622207da56d429af5a54</anchor>
      <arglist>(double **mat, int n, int *indx, double *d)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>mxLUBcksub</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>af4f904018cc928a778a7167b13fee704</anchor>
      <arglist>(double **mat, int n, const int *indx, double *b)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>mxiLudcmp3</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a83378019fa1d29c6f64e692118946a30</anchor>
      <arglist>(double mat[3][3], int indx[3], double *d)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxiLubksb3</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a1417150425cc14d57c2749d4f55a2945</anchor>
      <arglist>(const double mat[3][3], const int indx[3], double b[3])</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>mxSolveNxN</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a6b98843e685aab775941c3b89baa1d05</anchor>
      <arglist>(double **A, double *x, double *b, int n)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>mxSolveBandedEquations</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a6db76770e358ca141899b2a7c49d0a4d</anchor>
      <arglist>(double **a, double *x, double *b, int numeqs, int halfbandwidth)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>mxSolve3x3</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>aeed277bbc30a62f34e143baf5e6b04d4</anchor>
      <arglist>(double A[3][3], double x[3], double b[3])</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>mxInvert4x4</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>ac76184db2a6a79c7a900ccf094f2bc5d</anchor>
      <arglist>(const double matrix[4][4], double inv[4][4])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxPointMult</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a5e1a072632bb9cbda8df1442f70aa79a</anchor>
      <arglist>(Pt3d *pt, const double ctm[4][4])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxCopy4x4</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>ad30f45f6b87cb251f568c5afd565ac0f</anchor>
      <arglist>(double copy[4][4], const double matrix[4][4])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxIdent4x4</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a817c4c41acd08d13bfc16e7f2982b5c2</anchor>
      <arglist>(double matrix[4][4])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxMult4x4</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a5af15ec8654dced904d8c0b5b1e0ec99</anchor>
      <arglist>(double product[4][4], const double matrix1[4][4], const double matrix2[4][4])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxRotate4x4</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>ab8a23ab457d1a300fee82511b9dcccad</anchor>
      <arglist>(double xrot, double yrot, double zrot, double rMatt[4][4])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxTranslate4x4</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>ad8e2621b9cbd6773239bd1c9ad604e0a</anchor>
      <arglist>(const Pt3d &amp;a_translation, double a_mx[4][4])</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>matrix.h</name>
    <path>/home/conan/xmsinterp/matrices/</path>
    <filename>matrix_8h</filename>
    <member kind="function">
      <type>int</type>
      <name>mxLUDecomp</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>abafef2cc31ff622207da56d429af5a54</anchor>
      <arglist>(double **mat, int n, int *indx, double *d)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>mxLUBcksub</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>af4f904018cc928a778a7167b13fee704</anchor>
      <arglist>(double **mat, int n, const int *indx, double *b)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>mxiLudcmp3</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a83378019fa1d29c6f64e692118946a30</anchor>
      <arglist>(double mat[3][3], int indx[3], double *d)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxiLubksb3</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a1417150425cc14d57c2749d4f55a2945</anchor>
      <arglist>(const double mat[3][3], const int indx[3], double b[3])</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>mxSolveNxN</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a6b98843e685aab775941c3b89baa1d05</anchor>
      <arglist>(double **A, double *x, double *b, int n)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>mxSolveBandedEquations</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a6db76770e358ca141899b2a7c49d0a4d</anchor>
      <arglist>(double **a, double *x, double *b, int numeqs, int halfbandwidth)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>mxSolve3x3</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>aeed277bbc30a62f34e143baf5e6b04d4</anchor>
      <arglist>(double A[3][3], double x[3], double b[3])</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>mxInvert4x4</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>ac76184db2a6a79c7a900ccf094f2bc5d</anchor>
      <arglist>(const double matrix[4][4], double inv[4][4])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxPointMult</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a5e1a072632bb9cbda8df1442f70aa79a</anchor>
      <arglist>(Pt3d *pt, const double ctm[4][4])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxCopy4x4</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>ad30f45f6b87cb251f568c5afd565ac0f</anchor>
      <arglist>(double copy[4][4], const double matrix[4][4])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxIdent4x4</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a817c4c41acd08d13bfc16e7f2982b5c2</anchor>
      <arglist>(double matrix[4][4])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxMult4x4</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>a5af15ec8654dced904d8c0b5b1e0ec99</anchor>
      <arglist>(double product[4][4], const double matrix1[4][4], const double matrix2[4][4])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxRotate4x4</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>ab8a23ab457d1a300fee82511b9dcccad</anchor>
      <arglist>(double xrot, double yrot, double zrot, double rMatt[4][4])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mxTranslate4x4</name>
      <anchorfile>matrix_8cpp.html</anchorfile>
      <anchor>ad8e2621b9cbd6773239bd1c9ad604e0a</anchor>
      <arglist>(const Pt3d &amp;a_translation, double a_mx[4][4])</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ThreadLoop.cpp</name>
    <path>/home/conan/xmsinterp/thread/</path>
    <filename>_thread_loop_8cpp</filename>
    <includes id="_thread_loop_8h" name="ThreadLoop.h" local="no" imported="no">xmsinterp/thread/ThreadLoop.h</includes>
    <class kind="class">xms::ThreadLoop::impl</class>
  </compound>
  <compound kind="file">
    <name>ThreadLoop.h</name>
    <path>/home/conan/xmsinterp/thread/</path>
    <filename>_thread_loop_8h</filename>
    <class kind="class">xms::ThreadLoop</class>
  </compound>
  <compound kind="file">
    <name>ThreadMgr.cpp</name>
    <path>/home/conan/xmsinterp/thread/</path>
    <filename>_thread_mgr_8cpp</filename>
    <includes id="_thread_mgr_8h" name="ThreadMgr.h" local="no" imported="no">xmsinterp/thread/ThreadMgr.h</includes>
    <includes id="_thread_loop_8h" name="ThreadLoop.h" local="no" imported="no">xmsinterp/thread/ThreadLoop.h</includes>
    <class kind="class">xms::ThreadMgrImpl</class>
    <class kind="struct">xms::ThreadMgrImpl::myThread</class>
  </compound>
  <compound kind="file">
    <name>ThreadMgr.h</name>
    <path>/home/conan/xmsinterp/thread/</path>
    <filename>_thread_mgr_8h</filename>
    <class kind="class">xms::ThreadMgr</class>
  </compound>
  <compound kind="file">
    <name>TrAutoFixFourTrianglePts.cpp</name>
    <path>/home/conan/xmsinterp/triangulate/detail/</path>
    <filename>_tr_auto_fix_four_triangle_pts_8cpp</filename>
    <includes id="_tr_auto_fix_four_triangle_pts_8h" name="TrAutoFixFourTrianglePts.h" local="no" imported="no">xmsinterp/triangulate/detail/TrAutoFixFourTrianglePts.h</includes>
    <includes id="_tr_tin_8h" name="TrTin.h" local="no" imported="no">xmsinterp/triangulate/TrTin.h</includes>
    <includes id="_tr_auto_fix_four_triangle_pts_8t_8h" name="TrAutoFixFourTrianglePts.t.h" local="no" imported="no">xmsinterp/triangulate/detail/TrAutoFixFourTrianglePts.t.h</includes>
    <includes id="_tr_triangulator_points_8h" name="TrTriangulatorPoints.h" local="no" imported="no">xmsinterp/triangulate/TrTriangulatorPoints.h</includes>
    <class kind="class">xms::TrAutoFixFourTrianglePtsImpl</class>
  </compound>
  <compound kind="file">
    <name>TrAutoFixFourTrianglePts.h</name>
    <path>/home/conan/xmsinterp/triangulate/detail/</path>
    <filename>_tr_auto_fix_four_triangle_pts_8h</filename>
    <class kind="class">xms::TrAutoFixFourTrianglePts</class>
  </compound>
  <compound kind="file">
    <name>TrAutoFixFourTrianglePts.t.h</name>
    <path>/home/conan/xmsinterp/triangulate/detail/</path>
    <filename>_tr_auto_fix_four_triangle_pts_8t_8h</filename>
    <class kind="class">TrAutoFixFourTrianglePtsUnitTests</class>
  </compound>
  <compound kind="file">
    <name>triangulate.cpp</name>
    <path>/home/conan/xmsinterp/triangulate/detail/</path>
    <filename>triangulate_8cpp</filename>
    <includes id="triangulate_8h" name="triangulate.h" local="no" imported="no">xmsinterp/triangulate/detail/triangulate.h</includes>
    <includes id="_tr_triangulator_8h" name="TrTriangulator.h" local="no" imported="no">xmsinterp/triangulate/TrTriangulator.h</includes>
    <member kind="function">
      <type>bool</type>
      <name>trTriangulateIt</name>
      <anchorfile>triangulate_8cpp.html</anchorfile>
      <anchor>ae7e3768a7e434634406ea7ee25bc0648</anchor>
      <arglist>(TrTriangulator &amp;a_Client)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>triangulate.h</name>
    <path>/home/conan/xmsinterp/triangulate/detail/</path>
    <filename>triangulate_8h</filename>
    <member kind="function">
      <type>bool</type>
      <name>trTriangulateIt</name>
      <anchorfile>triangulate_8cpp.html</anchorfile>
      <anchor>ae7e3768a7e434634406ea7ee25bc0648</anchor>
      <arglist>(TrTriangulator &amp;a_Client)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>TrOuterTriangleDeleter.cpp</name>
    <path>/home/conan/xmsinterp/triangulate/detail/</path>
    <filename>_tr_outer_triangle_deleter_8cpp</filename>
    <includes id="_tr_outer_triangle_deleter_8h" name="TrOuterTriangleDeleter.h" local="no" imported="no">xmsinterp/triangulate/detail/TrOuterTriangleDeleter.h</includes>
    <includes id="_tr_tin_8h" name="TrTin.h" local="no" imported="no">xmsinterp/triangulate/TrTin.h</includes>
    <includes id="triangles_8h" name="triangles.h" local="no" imported="no">xmsinterp/triangulate/triangles.h</includes>
    <includes id="_tr_outer_triangle_deleter_8t_8h" name="TrOuterTriangleDeleter.t.h" local="no" imported="no">xmsinterp/triangulate/detail/TrOuterTriangleDeleter.t.h</includes>
    <includes id="_tr_tin_8t_8h" name="TrTin.t.h" local="no" imported="no">xmsinterp/triangulate/TrTin.t.h</includes>
    <class kind="class">xms::TrOuterTriangleDeleterImpl</class>
  </compound>
  <compound kind="file">
    <name>TrOuterTriangleDeleter.h</name>
    <path>/home/conan/xmsinterp/triangulate/detail/</path>
    <filename>_tr_outer_triangle_deleter_8h</filename>
    <class kind="class">xms::TrOuterTriangleDeleter</class>
  </compound>
  <compound kind="file">
    <name>TrOuterTriangleDeleter.t.h</name>
    <path>/home/conan/xmsinterp/triangulate/detail/</path>
    <filename>_tr_outer_triangle_deleter_8t_8h</filename>
    <class kind="class">TrOuterTriangleDeleterUnitTests</class>
  </compound>
  <compound kind="file">
    <name>TrBreaklineAdder.cpp</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>_tr_breakline_adder_8cpp</filename>
    <includes id="_tr_breakline_adder_8h" name="TrBreaklineAdder.h" local="no" imported="no">xmsinterp/triangulate/TrBreaklineAdder.h</includes>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="_gm_boost_types_8h" name="GmBoostTypes.h" local="no" imported="no">xmsinterp/geometry/GmBoostTypes.h</includes>
    <includes id="_gm_pt_search_8h" name="GmPtSearch.h" local="no" imported="no">xmsinterp/geometry/GmPtSearch.h</includes>
    <includes id="_gm_multi_poly_intersector_8h" name="GmMultiPolyIntersector.h" local="no" imported="no">xmsinterp/geometry/GmMultiPolyIntersector.h</includes>
    <includes id="_gm_multi_poly_intersection_sorter_terse_8h" name="GmMultiPolyIntersectionSorterTerse.h" local="no" imported="no">xmsinterp/geometry/GmMultiPolyIntersectionSorterTerse.h</includes>
    <includes id="triangles_8h" name="triangles.h" local="no" imported="no">xmsinterp/triangulate/triangles.h</includes>
    <includes id="_tr_tin_8h" name="TrTin.h" local="no" imported="no">xmsinterp/triangulate/TrTin.h</includes>
    <includes id="_tr_breakline_adder_8t_8h" name="TrBreaklineAdder.t.h" local="no" imported="no">xmsinterp/triangulate/TrBreaklineAdder.t.h</includes>
    <includes id="_tr_triangulator_points_8h" name="TrTriangulatorPoints.h" local="no" imported="no">xmsinterp/triangulate/TrTriangulatorPoints.h</includes>
    <includes id="_tr_tin_8t_8h" name="TrTin.t.h" local="no" imported="no">xmsinterp/triangulate/TrTin.t.h</includes>
    <class kind="struct">xms::edgerecord</class>
    <class kind="class">xms::TrBreaklineAdderImpl</class>
    <member kind="typedef">
      <type>std::vector&lt; edgerecord &gt;</type>
      <name>VecEdge</name>
      <anchorfile>_tr_breakline_adder_8cpp.html</anchorfile>
      <anchor>a7541b74c2b86c70e57e863b57304bdcc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>TrBreaklineAdder.h</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>_tr_breakline_adder_8h</filename>
    <class kind="class">xms::TrBreaklineAdder</class>
  </compound>
  <compound kind="file">
    <name>TrBreaklineAdder.t.h</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>_tr_breakline_adder_8t_8h</filename>
    <class kind="class">TrBreaklineAdderUnitTests</class>
  </compound>
  <compound kind="file">
    <name>triangles.cpp</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>triangles_8cpp</filename>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="triangles_8h" name="triangles.h" local="no" imported="no">xmsinterp/triangulate/triangles.h</includes>
    <member kind="function">
      <type>double</type>
      <name>trArea</name>
      <anchorfile>triangles_8cpp.html</anchorfile>
      <anchor>a3ab795fd4e5d3ebcedb00230e73392dc</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, const Pt3d &amp;a_pt3)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>trBuildGridTrianglePolys</name>
      <anchorfile>triangles_8cpp.html</anchorfile>
      <anchor>a176da2f77435c7baf90a2507e5841362</anchor>
      <arglist>(int rows, int cols, VecPt3d &amp;a_points, VecInt2d &amp;a_polys)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>trBuildGridPolys</name>
      <anchorfile>triangles_8cpp.html</anchorfile>
      <anchor>a5c5d655291110f3f2ff13fa3a733b689</anchor>
      <arglist>(int rows, int cols, VecPt3d &amp;pts, VecInt2d &amp;polys)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>triangles.h</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>triangles_8h</filename>
    <member kind="function">
      <type>double</type>
      <name>trArea</name>
      <anchorfile>triangles_8cpp.html</anchorfile>
      <anchor>a3ab795fd4e5d3ebcedb00230e73392dc</anchor>
      <arglist>(const Pt3d &amp;a_pt1, const Pt3d &amp;a_pt2, const Pt3d &amp;a_pt3)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>trBuildGridPolys</name>
      <anchorfile>triangles_8cpp.html</anchorfile>
      <anchor>a5c5d655291110f3f2ff13fa3a733b689</anchor>
      <arglist>(int rows, int cols, VecPt3d &amp;pts, VecInt2d &amp;polys)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>trBuildGridTrianglePolys</name>
      <anchorfile>triangles_8cpp.html</anchorfile>
      <anchor>a176da2f77435c7baf90a2507e5841362</anchor>
      <arglist>(int rows, int cols, VecPt3d &amp;a_points, VecInt2d &amp;a_polys)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>trIncrementIndex</name>
      <anchorfile>triangles_8h.html</anchorfile>
      <anchor>a3522d0baa47f21d6355b87f5c7173260</anchor>
      <arglist>(int i)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>trDecrementIndex</name>
      <anchorfile>triangles_8h.html</anchorfile>
      <anchor>a5b6d3f9afa689aac0cfcdf1097ae62cf</anchor>
      <arglist>(int i)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>TrTin.cpp</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>_tr_tin_8cpp</filename>
    <includes id="_tr_tin_8h" name="TrTin.h" local="no" imported="no">xmsinterp/triangulate/TrTin.h</includes>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="triangles_8h" name="triangles.h" local="no" imported="no">xmsinterp/triangulate/triangles.h</includes>
    <includes id="_tr_tin_8t_8h" name="TrTin.t.h" local="no" imported="no">xmsinterp/triangulate/TrTin.t.h</includes>
    <includes id="_tr_triangulator_points_8h" name="TrTriangulatorPoints.h" local="no" imported="no">xmsinterp/triangulate/TrTriangulatorPoints.h</includes>
    <class kind="class">xms::TrTinImpl</class>
    <member kind="function">
      <type></type>
      <name>BOOST_CLASS_EXPORT</name>
      <anchorfile>_tr_tin_8cpp.html</anchorfile>
      <anchor>a7244a742c140134d2c13f8714f1af76c</anchor>
      <arglist>(xms::TrTinImpl)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>trRenumberOnDelete</name>
      <anchorfile>_tr_tin_8cpp.html</anchorfile>
      <anchor>a66eebefbcd706efc80c4ca1cdceb05d3</anchor>
      <arglist>(const SetInt &amp;a_deleting, VecInt &amp;a_vec)</arglist>
    </member>
    <member kind="function">
      <type>BSHP&lt; TrTin &gt;</type>
      <name>trBuildTestTin</name>
      <anchorfile>_tr_tin_8cpp.html</anchorfile>
      <anchor>a0c925ae34b78a1dac382e317833dbec1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>BSHP&lt; TrTin &gt;</type>
      <name>trBuildTestTin2</name>
      <anchorfile>_tr_tin_8cpp.html</anchorfile>
      <anchor>a9ce87b91b5012a999ddbc87ad8820029</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>BSHP&lt; TrTin &gt;</type>
      <name>trBuildTestTin6</name>
      <anchorfile>_tr_tin_8cpp.html</anchorfile>
      <anchor>a60fe241cf170639ac6a6c0d2b342821c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>BSHP&lt; TrTin &gt;</type>
      <name>trBuildTestTin7</name>
      <anchorfile>_tr_tin_8cpp.html</anchorfile>
      <anchor>a068ae1567736bd473d51b406c9ade227</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>BSHP&lt; TrTin &gt;</type>
      <name>trBuildTestTin8</name>
      <anchorfile>_tr_tin_8cpp.html</anchorfile>
      <anchor>a310ee9097c8a7ff014c12621714077fa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>BSHP&lt; TrTin &gt;</type>
      <name>trBuildTestTin9</name>
      <anchorfile>_tr_tin_8cpp.html</anchorfile>
      <anchor>a386420cc59bd28676fe65c2b57f17127</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>TrTin.h</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>_tr_tin_8h</filename>
    <class kind="class">xms::TrTin</class>
    <member kind="function">
      <type>void</type>
      <name>trRenumberOnDelete</name>
      <anchorfile>_tr_tin_8cpp.html</anchorfile>
      <anchor>a66eebefbcd706efc80c4ca1cdceb05d3</anchor>
      <arglist>(const SetInt &amp;a_deleting, VecInt &amp;a_vec)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>TrTin.t.h</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>_tr_tin_8t_8h</filename>
    <class kind="class">TrTinUnitTests</class>
    <member kind="function">
      <type>boost::shared_ptr&lt; xms::TrTin &gt;</type>
      <name>trBuildTestTin</name>
      <anchorfile>_tr_tin_8t_8h.html</anchorfile>
      <anchor>af323a53c7f6fbba3a645dcd33a8ac450</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>boost::shared_ptr&lt; xms::TrTin &gt;</type>
      <name>trBuildTestTin2</name>
      <anchorfile>_tr_tin_8t_8h.html</anchorfile>
      <anchor>a1eda7b31829ff99f4d5b15c2188a997b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>boost::shared_ptr&lt; xms::TrTin &gt;</type>
      <name>trBuildTestTin6</name>
      <anchorfile>_tr_tin_8t_8h.html</anchorfile>
      <anchor>ac4f1ed3125ba5cd229fc1d04bb9fefe3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>boost::shared_ptr&lt; xms::TrTin &gt;</type>
      <name>trBuildTestTin7</name>
      <anchorfile>_tr_tin_8t_8h.html</anchorfile>
      <anchor>ae9477606ec0f20782f1bc2dbe94213d4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>boost::shared_ptr&lt; xms::TrTin &gt;</type>
      <name>trBuildTestTin8</name>
      <anchorfile>_tr_tin_8t_8h.html</anchorfile>
      <anchor>a1492a1c9c30f02dc6dccad115531126b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>boost::shared_ptr&lt; xms::TrTin &gt;</type>
      <name>trBuildTestTin9</name>
      <anchorfile>_tr_tin_8t_8h.html</anchorfile>
      <anchor>a36ccf1dbfe10bcde1d187234d0e8e619</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>TrTriangulator.cpp</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>_tr_triangulator_8cpp</filename>
    <includes id="_tr_triangulator_8h" name="TrTriangulator.h" local="no" imported="no">xmsinterp/triangulate/TrTriangulator.h</includes>
    <includes id="triangulate_8h" name="triangulate.h" local="no" imported="no">xmsinterp/triangulate/detail/triangulate.h</includes>
  </compound>
  <compound kind="file">
    <name>TrTriangulator.h</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>_tr_triangulator_8h</filename>
    <class kind="class">xms::TrTriangulator</class>
  </compound>
  <compound kind="file">
    <name>TrTriangulatorPoints.cpp</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>_tr_triangulator_points_8cpp</filename>
    <includes id="_tr_triangulator_points_8h" name="TrTriangulatorPoints.h" local="no" imported="no">xmsinterp/triangulate/TrTriangulatorPoints.h</includes>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="triangles_8h" name="triangles.h" local="no" imported="no">xmsinterp/triangulate/triangles.h</includes>
    <includes id="_tr_triangulator_points_8t_8h" name="TrTriangulatorPoints.t.h" local="no" imported="no">xmsinterp/triangulate/TrTriangulatorPoints.t.h</includes>
    <includes id="triangulate_8h" name="triangulate.h" local="no" imported="no">xmsinterp/triangulate/detail/triangulate.h</includes>
  </compound>
  <compound kind="file">
    <name>TrTriangulatorPoints.h</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>_tr_triangulator_points_8h</filename>
    <includes id="_tr_triangulator_8h" name="TrTriangulator.h" local="no" imported="no">xmsinterp/triangulate/TrTriangulator.h</includes>
    <class kind="class">xms::TrTriangulatorPoints</class>
  </compound>
  <compound kind="file">
    <name>TrTriangulatorPoints.t.h</name>
    <path>/home/conan/xmsinterp/triangulate/</path>
    <filename>_tr_triangulator_points_8t_8h</filename>
    <class kind="class">TrTriangulatorPointsUnitTests</class>
  </compound>
  <compound kind="file">
    <name>TutInterpolation.cpp</name>
    <path>/home/conan/xmsinterp/tutorial/</path>
    <filename>_tut_interpolation_8cpp</filename>
    <includes id="geoms_8h" name="geoms.h" local="no" imported="no">xmsinterp/geometry/geoms.h</includes>
    <includes id="_tut_interpolation_8t_8h" name="TutInterpolation.t.h" local="no" imported="no">xmsinterp/tutorial/TutInterpolation.t.h</includes>
    <includes id="_interp_idw_8h" name="InterpIdw.h" local="no" imported="no">xmsinterp/interpolate/InterpIdw.h</includes>
    <includes id="_interp_linear_8h" name="InterpLinear.h" local="no" imported="no">xmsinterp/interpolate/InterpLinear.h</includes>
    <includes id="_interp_anisotropic_8h" name="InterpAnisotropic.h" local="no" imported="no">xmsinterp/interpolate/InterpAnisotropic.h</includes>
  </compound>
  <compound kind="file">
    <name>TutInterpolation.t.h</name>
    <path>/home/conan/xmsinterp/tutorial/</path>
    <filename>_tut_interpolation_8t_8h</filename>
    <class kind="class">TutInterpolationIntermediateTests</class>
  </compound>
  <compound kind="class">
    <name>xms::AnisotropicInterpolator</name>
    <filename>classxms_1_1_anisotropic_interpolator.html</filename>
    <member kind="function">
      <type></type>
      <name>AnisotropicInterpolator</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>addce3b33f2e24362650ebeb3abbba4b6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetPoints</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a5db002e619acbc0475829c4d3af005fa</anchor>
      <arglist>(const VecPt3d &amp;a_centerlinePts, const VecPt3d &amp;a_interpolationPts, bool a_pickClosest=false)</arglist>
    </member>
    <member kind="function">
      <type>const VecPt3d &amp;</type>
      <name>GetInterpolationPoints</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a45947abe52e2dd7a419faec933f5b346</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>InterpolatePoint</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a5f990a1912def1febbb07dcd95282424</anchor>
      <arglist>(const Pt3d &amp;a_target, const VecPt3d &amp;a_interpolationPoints, double a_xScale, double a_IDWPower=2, double a_tol=1.0e-5)</arglist>
    </member>
    <member kind="function">
      <type>VecPt3d</type>
      <name>ComputeInterpolationPoints</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>aca09aeaceab3be5c1904066945bdbbe8</anchor>
      <arglist>(const VecPt3d &amp;a_interpolationPts, bool a_pickClosest)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>Distance</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a76b97e4fa7f0e2beb567a385e181e2a1</anchor>
      <arglist>(const Pt3d &amp;a_p0, const Pt3d &amp;a_p1) const</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>AnisotropicDistanceSquared</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a208cd08d32b9c9f29ef3c1899bc4e0a4</anchor>
      <arglist>(const Pt3d &amp;a_p0, const Pt3d &amp;a_p1, double a_xScale)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>CalculateIDW</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a5ac4d79af84045b2c57c57ff9cb1a3f7</anchor>
      <arglist>(double a_xScale, const VecPt3d &amp;a_points, const Pt3d &amp;a_target, double a_power, double a_tol=1.0e-5)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>CrossProduct</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a6c93ec40dbd10628fc0044ade6a26b23</anchor>
      <arglist>(size_t a_segmentIndex, const Pt3d &amp;a_p) const</arglist>
    </member>
    <member kind="function">
      <type>LineParameters</type>
      <name>GetLineParameters</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a26ac5ed96dfab3ebfa0f3b51deda9768</anchor>
      <arglist>(const Pt3d &amp;a_p0, const Pt3d &amp;a_p1) const</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>GetParameterInSegment</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a89d69f475b58b91c66af9da678986320</anchor>
      <arglist>(size_t a_segmentIndex, const Pt3d &amp;a_p) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>GetIntersectionOfSegmentWithPoint</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>afbbf2018849b77d8257a8859a7e3f1bd</anchor>
      <arglist>(size_t a_segmentIndex, const Pt3d &amp;a_p, Pt3d &amp;a_intersection, double &amp;a_param) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GetNormalPointParameter</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a2029e95c27c05e502ec0f4a084ff1585</anchor>
      <arglist>(size_t a_segmentIndex, const Pt3d &amp;a_p, double &amp;a_lastParam, VecSNResult &amp;a_results) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GetAllNormalPointParameters</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a1a66b1b43199caf4469a9b6db2209c8b</anchor>
      <arglist>(const Pt3d &amp;a_p, VecSNResult &amp;a_results)</arglist>
    </member>
    <member kind="function">
      <type>VecPt3d</type>
      <name>TransformPoint</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a7c4fe1fdd8425e77f15f6a6f7c5471f4</anchor>
      <arglist>(const Pt3d &amp;a_p, bool a_onlyClosest=false)</arglist>
    </member>
    <member kind="variable">
      <type>VecPt3d</type>
      <name>m_centerLinePts</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>ad2bc45326e615ab8a69514005d4d0704</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VecDbl</type>
      <name>m_accLengths</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>a40fd171a794add8ea68679c73b06abb4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; LineParameters &gt;</type>
      <name>m_lineParams</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>af7b575de43d220d619683a832a0dfec1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VecPt3d</type>
      <name>m_SNPoints</name>
      <anchorfile>classxms_1_1_anisotropic_interpolator.html</anchorfile>
      <anchor>ae7be19a8211aa36923314f93831c6011</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>AnisotropicInterpolatorUnitTests</name>
    <filename>class_anisotropic_interpolator_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testSimple</name>
      <anchorfile>class_anisotropic_interpolator_unit_tests.html</anchorfile>
      <anchor>a21219da1c1b1790e5423cbc4c3d1638d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testCrossSectionThroughPoint</name>
      <anchorfile>class_anisotropic_interpolator_unit_tests.html</anchorfile>
      <anchor>a486e729faa316b029abb46d39b39ce8a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testAmbiguity</name>
      <anchorfile>class_anisotropic_interpolator_unit_tests.html</anchorfile>
      <anchor>af802cd8a7b64cacbf72c70fad9e4be7f</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>xms::BarycentricVals</name>
    <filename>structxms_1_1_barycentric_vals.html</filename>
    <member kind="variable">
      <type>Pt3d</type>
      <name>orig</name>
      <anchorfile>structxms_1_1_barycentric_vals.html</anchorfile>
      <anchor>ae88c2de12a80a534faf78addd416f38c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>coef</name>
      <anchorfile>structxms_1_1_barycentric_vals.html</anchorfile>
      <anchor>aebed3bc5a1c410ee503863972249b13b</anchor>
      <arglist>[6]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>dir</name>
      <anchorfile>structxms_1_1_barycentric_vals.html</anchorfile>
      <anchor>acb8bf1c437d7b0141c565730738b297a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>xms::edgerecord</name>
    <filename>structxms_1_1edgerecord.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>pt1</name>
      <anchorfile>structxms_1_1edgerecord.html</anchorfile>
      <anchor>a628ba61db2c8c725a8b5a603620f35cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>pt2</name>
      <anchorfile>structxms_1_1edgerecord.html</anchorfile>
      <anchor>a37a472bed46ccd144b62eb755ad8770e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Pt3d</type>
      <name>intersection</name>
      <anchorfile>structxms_1_1edgerecord.html</anchorfile>
      <anchor>a34b4f43309c69d116fad70ebe6cf0dfb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::fSatisfies</name>
    <filename>classxms_1_1f_satisfies.html</filename>
    <member kind="function">
      <type></type>
      <name>fSatisfies</name>
      <anchorfile>classxms_1_1f_satisfies.html</anchorfile>
      <anchor>ab363650297804f81ac7f6fa1179b6bc0</anchor>
      <arglist>(size_t a_nVals)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>classxms_1_1f_satisfies.html</anchorfile>
      <anchor>aba3fb24836d4e09c49644fd394fe191e</anchor>
      <arglist>(value const &amp;a_) const</arglist>
    </member>
    <member kind="variable">
      <type>DynBitset</type>
      <name>m_bits</name>
      <anchorfile>classxms_1_1f_satisfies.html</anchorfile>
      <anchor>abb625b439183cff97cde9dede033f7aa</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GeomsXmsngIntermediateTests</name>
    <filename>class_geoms_xmsng_intermediate_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>test_gmPointInPolygon2D</name>
      <anchorfile>class_geoms_xmsng_intermediate_tests.html</anchorfile>
      <anchor>a06e3aae3a26f0358d21daf3ae69ccdf2</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GeomsXmsngUnitTests</name>
    <filename>class_geoms_xmsng_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>test_gmComputePolygonCentroid</name>
      <anchorfile>class_geoms_xmsng_unit_tests.html</anchorfile>
      <anchor>a55f61c470b2c64974edeedda4523d500</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test_gmComputeCentroid</name>
      <anchorfile>class_geoms_xmsng_unit_tests.html</anchorfile>
      <anchor>a6221ee23de11b511e5920baa908cb6a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test_gmPointInPolygon2D_Speed</name>
      <anchorfile>class_geoms_xmsng_unit_tests.html</anchorfile>
      <anchor>a3c7aa86a05eb6c65edbfd2739adae675</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmExtents2d</name>
    <filename>classxms_1_1_gm_extents2d.html</filename>
    <member kind="function">
      <type></type>
      <name>GmExtents2d</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a7713120ed10fdf972a7d9cd4727695e7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GmExtents2d</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>ac6ca00863adaed8c44fdf229ba55cee2</anchor>
      <arglist>(const GmExtents3d &amp;a_extents)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GmExtents2d</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a41f7becb48eb6566a0d8446c0af744e8</anchor>
      <arglist>(const Pt2d &amp;a_min, const Pt2d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GmExtents2d</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a59954fe5dcff4393c789b9b5310d3b00</anchor>
      <arglist>(const Pt3d &amp;a_min, const Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~GmExtents2d</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a199a9513a66ab4e8ca031c93a60502ce</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>operator+=</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>ae42c798434698c97fdd26298ac26f6e0</anchor>
      <arglist>(const GmExtents2d &amp;a_rhs)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>AddToExtents</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a961a392986a503fd2a08b2dcd4c19644</anchor>
      <arglist>(const Pt3d &amp;pt)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Add</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>ae5cc44c6efd560195817f1d1d32cc17c</anchor>
      <arglist>(ItrType a_begin, ItrType a_end)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsValid</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a8808cc09d89e82f4ab38c6fc649a026d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Overlap</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a78e80fd3786ba85959a85df3896c5c13</anchor>
      <arglist>(const GmExtents2d &amp;a_b) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Clear</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a60780f3b3471ce34744412fed1938f47</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GetExtents</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a12db96218f2b4f7c1ff5242ba8bab3d7</anchor>
      <arglist>(double *a_MinX, double *a_MaxX, double *a_MinY, double *a_MaxY)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GetExtents</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>abdadfa5471353d9186adf9118790a95a</anchor>
      <arglist>(Pt3d &amp;a_dMin, Pt3d &amp;a_dMax)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>GetMinX</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a77bf9cf316f868751b5beb1266a2b75f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>GetMaxX</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>ab46d3290c8986ddaf08562d0fad62caf</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>GetMinY</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a2149daf43a184dd06ade5b9c78d4645b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>GetMaxY</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>aa8f7bfbeaba83b82e321f72c562addca</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Pt2d</type>
      <name>GetMin</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>af1491a3798bcb600e4cd47f1ef333d2f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Pt2d</type>
      <name>GetMax</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>ada4543fe674250cab6f1e582c6f091af</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Set</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a65c98975328fe846704c7fc91a006e1e</anchor>
      <arglist>(const Pt2d &amp;a_min, const Pt2d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>serialize</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>aa7a591f9d935ef8aea0402d746c56fc9</anchor>
      <arglist>(Archive &amp;archive, const unsigned int version)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>SetTolerance</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>af91bf63e31dc46c63a0165d267f7ad19</anchor>
      <arglist>(double a_Tol)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>GetTolerance</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a7db9964a1b49f1bd8e3003a5c592dbc0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Pt2d</type>
      <name>m_min</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>aff08e09f489359e779cc9a068fed293a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Pt2d</type>
      <name>m_max</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>aee80084ccb1fc4d9aa069c232678ff0d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static double</type>
      <name>m_tolerance</name>
      <anchorfile>classxms_1_1_gm_extents2d.html</anchorfile>
      <anchor>a96ea5046f0d2a697ab852e2e6e37b0de</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmExtents3d</name>
    <filename>classxms_1_1_gm_extents3d.html</filename>
    <member kind="function">
      <type></type>
      <name>GmExtents3d</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>aee049dcde88efdf2266b9ddac3700626</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GmExtents3d</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>aee5d872568e8585f8e24db8678c5a246</anchor>
      <arglist>(const GmExtents2d &amp;a_extents)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GmExtents3d</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a1954ff9b989d3e13f414e142dd5404d1</anchor>
      <arglist>(const Pt3d &amp;a_min, const Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GmExtents3d</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a11ec30b976bd5ddfde9a9ecdf3248702</anchor>
      <arglist>(const Pt2d &amp;a_min, const Pt2d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>operator+=</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>ac0a55b7e31742d3dfff48cbc321ccca2</anchor>
      <arglist>(const GmExtents3d &amp;a_rhs)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>AddToExtents</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a1f41c66ac6b67c60982044093ada302e</anchor>
      <arglist>(const Pt3d &amp;pt)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsValid</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a078c56a4d36e3db9fd038082418964fc</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Overlap</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a5eab9f9515caa8e15d344bca95d3a783</anchor>
      <arglist>(GmExtents3d &amp;a_b) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Clear</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>af7b8f4955c9068c0853354ab1f34f297</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPointWithin</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a902eb81668d4f9cf77dd6ddc558b40a7</anchor>
      <arglist>(const Pt3d &amp;pt) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GetExtents</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a1afb775183a2020c66e937d9d8df5ede</anchor>
      <arglist>(double *a_MinX, double *a_MaxX, double *a_MinY, double *a_MaxY, double *a_MinZ, double *a_MaxZ) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GetExtents</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>ac77ae8b5172628c3266bc774951c29d6</anchor>
      <arglist>(Pt3d &amp;a_dMin, Pt3d &amp;a_dMax) const</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>GetMinX</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a9a3f36f9da0e09ba773590d6ff36da60</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>GetMaxX</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>ab5827886d151465fbb902eff452bb978</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>GetMinY</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>ac7a8ae51c57f09c4f8487d851fcf31a8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>GetMaxY</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>ad932aba529c8cfb6bbe73e2f9eaa6f49</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>GetMinZ</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a4863256f0eed7d7b6b71b08e1178c8de</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>GetMaxZ</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a3ef27d65b4d3931937f647486059fc02</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Pt3d</type>
      <name>GetMin</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a608b517961d6745d92f15d6278a8e5e4</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Pt3d</type>
      <name>GetMax</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>ad9558c795fc8b7350567f178e27eca8a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Set</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a79314a735a92ef987b6412fcf404beea</anchor>
      <arglist>(const Pt3d &amp;a_min, const Pt3d &amp;a_max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>serialize</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a0b0221f18c9e5c9262606c3ac89aff67</anchor>
      <arglist>(Archive &amp;archive, const unsigned int version)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>SetTolerance</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a6b4934250f1e3b40181595cf45ec52cf</anchor>
      <arglist>(double a_Tol)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>GetTolerance</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>accc7b2a33b04d17b07af0db56df8b328</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Pt3d</type>
      <name>m_min</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>af9468191aa5196a5013db9c0e14b1d71</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Pt3d</type>
      <name>m_max</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>ad4235d1ff4b577bcfad2dbfe0524afdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static double</type>
      <name>m_tolerance</name>
      <anchorfile>classxms_1_1_gm_extents3d.html</anchorfile>
      <anchor>a7a1ee26cd395da7b0d36e605571f8ea5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GmExtents3dUnitTests</name>
    <filename>class_gm_extents3d_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testSinglePt</name>
      <anchorfile>class_gm_extents3d_unit_tests.html</anchorfile>
      <anchor>adedf8a67b016f09f4e95672841cead5c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testMultiplePts</name>
      <anchorfile>class_gm_extents3d_unit_tests.html</anchorfile>
      <anchor>a1c9c9993c46633c84692110933f34e41</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testNoPts</name>
      <anchorfile>class_gm_extents3d_unit_tests.html</anchorfile>
      <anchor>ac38da518f4b94df3f4cd1cf49d847a5e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testOverlapping</name>
      <anchorfile>class_gm_extents3d_unit_tests.html</anchorfile>
      <anchor>aa3e1b36f174ffc0f1607c3cb31b8c4d0</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmMultiPolyIntersectionSorter</name>
    <filename>classxms_1_1_gm_multi_poly_intersection_sorter.html</filename>
  </compound>
  <compound kind="class">
    <name>xms::GmMultiPolyIntersectionSorterTerse</name>
    <filename>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</filename>
    <base>xms::GmMultiPolyIntersectionSorter</base>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>Sort</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>aab32ba927a9062f1c95c5a2e1bedb62d</anchor>
      <arglist>(GmMultiPolyIntersectorData &amp;a_data, std::vector&lt; int &gt; &amp;polyids, std::vector&lt; double &gt; &amp;tvalues, std::vector&lt; Pt3d &gt; &amp;a_pts, double a_tol) override</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>RemoveCornerTouches</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>ac86036c81726449c8121c1a05fd73db3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>RemoveDuplicateEdges</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>a1ef5deaea8e21cd99a88376758bc6c64</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>SwapAdjacents</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>a230088635ebb08581d8701911068be0a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>IntersectionsToPolyIdsAndTValuesFor2</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>a6b310a7f9545a6629edbc8250eefcb24</anchor>
      <arglist>(std::vector&lt; int &gt; &amp;polyids, std::vector&lt; double &gt; &amp;tvalues, std::vector&lt; Pt3d &gt; &amp;a_pts) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>IntersectionsToPolyIdsAndTValuesFor3OrMore</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>ae3c73099c3b23d94416fc14601b91ee5</anchor>
      <arglist>(std::vector&lt; int &gt; &amp;polyids, std::vector&lt; double &gt; &amp;tvalues, std::vector&lt; Pt3d &gt; &amp;a_pts) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>IntersectionsToPolyIdsAndTValues</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>a9cd8f532b56f11e5600f2910051daaff</anchor>
      <arglist>(std::vector&lt; int &gt; &amp;polyids, std::vector&lt; double &gt; &amp;tvalues, std::vector&lt; Pt3d &gt; &amp;a_pts) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>FixArrays</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>af5a86f40ed9c1962c81950074ac9a598</anchor>
      <arglist>(std::vector&lt; int &gt; &amp;polyids, std::vector&lt; double &gt; &amp;tvalues, std::vector&lt; Pt3d &gt; &amp;a_pts) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>AddToPolyIdsAndTValues</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>a0cb9995b0273c1953428f420f12eaffb</anchor>
      <arglist>(const ix &amp;a_ix, std::vector&lt; int &gt; &amp;polyids, std::vector&lt; double &gt; &amp;tvalues, std::vector&lt; Pt3d &gt; &amp;a_pts) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>FindPreviousNextNeither</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>ab8b8a8153fae932c73a83a639f8ac7ad</anchor>
      <arglist>(const std::vector&lt; int &gt; &amp;tChange, const int idx, std::vector&lt; int &gt; *inPrev, std::vector&lt; int &gt; *inNext, std::vector&lt; int &gt; *inNeither) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>Swap</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>a484addfe624bb88f1980496ccb922692</anchor>
      <arglist>(int a, int b)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>FindWhereTValuesChange</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>ab19ab5f34e20a4cce4d5ef18e6e914f3</anchor>
      <arglist>(std::vector&lt; int &gt; &amp;tChange) const</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>GmMultiPolyIntersectorData *</type>
      <name>m_d</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>a7bacf816a4c3b4eae33cd5a36007655a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>m_tol</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersection_sorter_terse.html</anchorfile>
      <anchor>afe1b7c2e44e9d9837c446666d4995d48</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmMultiPolyIntersector</name>
    <filename>classxms_1_1_gm_multi_poly_intersector.html</filename>
    <member kind="function" static="yes">
      <type>static boost::shared_ptr&lt; GmMultiPolyIntersector &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector.html</anchorfile>
      <anchor>a225f2e577e682f6fbd603fa56dce2253</anchor>
      <arglist>(const std::vector&lt; Pt3d &gt; &amp;a_points, const std::vector&lt; std::vector&lt; int &gt; &gt; &amp;a_polys, boost::shared_ptr&lt; GmMultiPolyIntersectionSorter &gt; a_sorter, int a_startingId=1)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GmMultiPolyIntersector2IntermediateTests</name>
    <filename>class_gm_multi_poly_intersector2_intermediate_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testLargeNumPolys</name>
      <anchorfile>class_gm_multi_poly_intersector2_intermediate_tests.html</anchorfile>
      <anchor>af3432feef6f5f052b418fb4e8aeb5868</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testLargeNumPolysAndSegments</name>
      <anchorfile>class_gm_multi_poly_intersector2_intermediate_tests.html</anchorfile>
      <anchor>accf890a54adf98d863387d7d6c714109</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>xms::GmMultiPolyIntersectorData</name>
    <filename>structxms_1_1_gm_multi_poly_intersector_data.html</filename>
    <member kind="variable">
      <type>std::vector&lt; Pt3d &gt;</type>
      <name>m_points</name>
      <anchorfile>structxms_1_1_gm_multi_poly_intersector_data.html</anchorfile>
      <anchor>aafe89c90dca39c321b5a71af95256c2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; std::vector&lt; int &gt; &gt;</type>
      <name>m_polys</name>
      <anchorfile>structxms_1_1_gm_multi_poly_intersector_data.html</anchorfile>
      <anchor>a4b6f906b82daa91ae96450abf8071509</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::set&lt; int &gt;</type>
      <name>m_polys1</name>
      <anchorfile>structxms_1_1_gm_multi_poly_intersector_data.html</anchorfile>
      <anchor>a34a35f421f49e17ec2e1f3a2b4d678a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::set&lt; int &gt;</type>
      <name>m_polys2</name>
      <anchorfile>structxms_1_1_gm_multi_poly_intersector_data.html</anchorfile>
      <anchor>aca483d0445e27801f910b40c93d22c96</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; ix &gt;</type>
      <name>m_ixs</name>
      <anchorfile>structxms_1_1_gm_multi_poly_intersector_data.html</anchorfile>
      <anchor>a3e30a6edc79fdb107f575508b14e5ea5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmMultiPolyIntersectorImpl</name>
    <filename>classxms_1_1_gm_multi_poly_intersector_impl.html</filename>
    <base>xms::GmMultiPolyIntersector</base>
    <member kind="function">
      <type></type>
      <name>GmMultiPolyIntersectorImpl</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>ac5a0b775408588b9d96a228d7185f5cd</anchor>
      <arglist>(const VecPt3d &amp;a_points, const VecInt2d &amp;a_polys, BSHP&lt; GmMultiPolyIntersectionSorter &gt; a_sorter, int a_startingId=1)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetQuery</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>ad44f26ecf4069c3406c27806dabf13f8</anchor>
      <arglist>(GmMultiPolyIntersectorQueryEnum a_query) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>TraverseLineSegment</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>ac609bd361ea9ad842dcbb3b242cbda93</anchor>
      <arglist>(double x1, double y1, double x2, double y2, VecInt &amp;polyids, VecDbl &amp;tvalues) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>TraverseLineSegment</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a76d72475dbe448c625990126fd5a9f68</anchor>
      <arglist>(double x1, double y1, double x2, double y2, VecInt &amp;polyids) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>TraverseLineSegment</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a2ab2a192cfceda0596d9a5d9a0c1129e</anchor>
      <arglist>(double a_x1, double a_y1, double a_x2, double a_y2, VecInt &amp;a_polyids, std::vector&lt; Pt3d &gt; &amp;a_pts) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>PolygonFromPoint</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a704347586a5fa15a9f66fd088f8d7353</anchor>
      <arglist>(const Pt3d &amp;a_pt) override</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>CalculateBuffer</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a535aad59183701eae8532bcb4c2ed2f0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>BufferTheBox</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>ae6d9d3489cc6f4ffad6bbdbaefadddf5</anchor>
      <arglist>(GmBstBox3d &amp;box) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>GmBstPoly3d &amp;</type>
      <name>GetBoostPoly</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a9205979073697853ffdc62c0e5a87cac</anchor>
      <arglist>(int a_polyIdx)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>BuildBoostPoly</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a34751018aeada5236adb5c55eb2e7c07</anchor>
      <arglist>(int a_polyIdx, GmBstPoly3d &amp;a_boostPoly) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>BuildRTree</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>af1174b0ddef285f93ba48e5aaadd6bbb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>CreateLine</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a3c3d8a9dd135301536427efe7e08a21a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>GetPolysForPoint</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>aeeca34e061a08d03164142275666c15d</anchor>
      <arglist>(Pt3d pt, SetInt &amp;poly)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>EnsureEndPointsRepresented</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a0d3c6c5b4daa721d5fb2edec53d28dbb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>IntersectEachPolyWithLine</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>ae78756aa2652d524131bb6259534df04</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>ComputeTValues</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>ab1525012537a7fe8d974de6c1c7c1080</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>SortIntersections</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>ad8a8d896823f5f70b3ffc250b453434c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>OffsetPolyIds</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>ad7d468b9c1de82c05149fc6bf429dab1</anchor>
      <arglist>(VecInt &amp;polyIds) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>PointsOnSegment</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a425a8860d5df3720cee68cdd32f1bb9f</anchor>
      <arglist>(const GmBstPoly3d &amp;a_poly, const GmBstLine3d &amp;a_line, std::deque&lt; Pt3d &gt; &amp;a_output)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>TraverseLineSegmentAll</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>aab2618742a6a374cde9c4d8ea2cd8e6a</anchor>
      <arglist>(double a_x1, double a_y1, double a_x2, double a_y2, VecInt &amp;a_polyids, VecDbl &amp;a_tvalues, std::vector&lt; Pt3d &gt; &amp;a_pts)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>GmMultiPolyIntersectorData</type>
      <name>m_d</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a6deb7afa1a20058c93d83ef912ee890a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Pt3d</type>
      <name>m_pt1</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>aab0dadb74472f2aa11c1f2838c5b18b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Pt3d</type>
      <name>m_pt2</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a165515a3528656302865abe9b060d052</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtreeBox *</type>
      <name>m_rtree</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a79c692d08b0a79ba2823a92a98402d04</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>GmBstLine3d</type>
      <name>m_line</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>ac8c9bc9edcf78e66b2b19b429fdd748b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>m_buffer</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>aece8a77964a59bb161d95a8d0f36e598</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>m_startingId</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>af8d56b114f4feb8359cccfb904429dd8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; GmBstPoly3d &gt;</type>
      <name>m_boostPolys</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>a0bf794580c7158c6295b62dc6d359308</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; GmMultiPolyIntersectionSorter &gt;</type>
      <name>m_sorter</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>aee888465df13dc924b99fc607071223e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>GmMultiPolyIntersectorQueryEnum</type>
      <name>m_query</name>
      <anchorfile>classxms_1_1_gm_multi_poly_intersector_impl.html</anchorfile>
      <anchor>ae25a886c347f59cc97cdd4d09d0f2ecf</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GmMultiPolyIntersectorUnitTests</name>
    <filename>class_gm_multi_poly_intersector_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>test1OutOut</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a9bc1d133022a08f727d0c14d5ffc8c5a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1OutIn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a7d9bca80f562c2dae434abaea557dd24</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1InOut</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a50cb9ccfc4935064d0a2506841d57a23</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1InIn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>aa5baa916d12383da4dbe7877af0a956e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1OnOn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a47ee3ffbab44e06feb633d5b15724cee</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1OnIn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a056d2db6357740fadb0a5a2a6aa6219a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1InOn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>ae3cc91a790599b69ab905e1a24936abf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1OutOn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a0e343e3648cc68f7cb30e4a51df5dd0d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1OnOut</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a04ad35a38490fd441747be17c7d06898</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1EdgeInIn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a846dba3e81acd4b114248e867c5fbf17</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1EdgePtPt</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>ac3df56c4be517755e6e1a5080a4a35e8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1EdgeOutOut</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>af82efa33125fcbf89662cf346daa4bcf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1EdgeOutIn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a78fd1c347b853f365d36bde2a1164208</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1EdgeInOut</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>ac81760456d448049194f1b0b1dd06d92</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1OutPt</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>aa9d101ac50cce7df1ad548edbe558d25</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1OutPtOut</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>ae32cf8a87707053e10443d7373795450</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1AllOut</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a25a7d6c55a01f1877e31fed5f09cebe5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1PtIn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a97587425f176cde3ee00ce7828987a85</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test1InPt</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>ae42d2fac80e867f29ba2b22c3a005ab6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test2OnOn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>abc34eb869b0922314dae22aa2a164f7f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test2InOn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>aa6ebb5373b16aea4f77837dd3f3b7091</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test2OnIn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>ab02b7de0c41d00d91983178ed2cc1ba9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test2OutOut</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>aa41bf64c185e5d23e7fb2081cb4cf315</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test2OutIn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a6d4b387f9a12ed3a3e4992b4f11d6ef6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test2InOut</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>ac6698da2b89c2110c5044f65faf474e5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test2InIn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>aba39959cfea6f33feb66b19ae4864cc1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test2InPt</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>aab29e414a3c777ea2e6a6ab6d4842c5f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test2PtIn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a9666cf0a6ed89edd4c3e36739516c54b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test2InEdgePt</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a00fe4843d3173ee19212b6d58d61f8fb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test2InOutIn</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>abc2aacee320789fcd0548cb3a9244afa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testInsideToInside</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a72a7ad9e8759e3a4cf0b9c3675a64c3e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testOutsideToOutside</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a3fb7e8f8e878592caa011cc15e769587</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testTouchesVertex</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>abe52de87107ad86a5af6cd0430952b5d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testTouchesEdge</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a679656ec0fe0368c1f25b35fc345ccca</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testAlongEdgesInsideToInside</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>ac913a37cc74a0b363c4b4d5647dc194d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testAlongEdgesOutsideToOutside</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>ac5d233ab7cbde987ca5f5ad5e6f37c97</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testEdgeThroughOppositeVertexAtAngle</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>af54ca54fb1ec6a2495e9fb0fb127aaa3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testStartAtEdgeThroughAdjacent</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a89bac4f7c03a3d6ffaad8d7776613bcf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testInsideToEdgeThenThroughAdjacent</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a704778a5c7167a957eeba146415930f2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testEndAtEdgeFromAdjacent</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>ae3b9191a3d4e3af3866b04c3458b778d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testSmsCase1</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>aa22cd3c8a7facdc51f8767d8185bbb12</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testMap2MfBug</name>
      <anchorfile>class_gm_multi_poly_intersector_unit_tests.html</anchorfile>
      <anchor>a05ca020d15aa52adaeb42df71db2c748</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GmPointInPolyTester_gmPointInPolygon2D</name>
    <filename>class_gm_point_in_poly_tester__gm_point_in_polygon2_d.html</filename>
    <base>GmPointInPolyUnitTests</base>
    <member kind="function">
      <type></type>
      <name>GmPointInPolyTester_gmPointInPolygon2D</name>
      <anchorfile>class_gm_point_in_poly_tester__gm_point_in_polygon2_d.html</anchorfile>
      <anchor>a139a56adbb9597d361b30e0ceb65905d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" virtualness="virtual">
      <type>virtual void</type>
      <name>CheckPoint</name>
      <anchorfile>class_gm_point_in_poly_tester__gm_point_in_polygon2_d.html</anchorfile>
      <anchor>a49d08071fc94eaade3a90093804d1583</anchor>
      <arglist>(const xms::Pt3d &amp;a_pt) override</arglist>
    </member>
    <member kind="function" protection="private" virtualness="virtual">
      <type>virtual double</type>
      <name>MaxTime</name>
      <anchorfile>class_gm_point_in_poly_tester__gm_point_in_polygon2_d.html</anchorfile>
      <anchor>a922ad92f9b9b0fad1f3167b111abed16</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>XM_DISALLOW_COPY_AND_ASSIGN</name>
      <anchorfile>class_gm_point_in_poly_tester__gm_point_in_polygon2_d.html</anchorfile>
      <anchor>a93106a8027d1f7a54029804e94a478c3</anchor>
      <arglist>(GmPointInPolyTester_gmPointInPolygon2D)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GmPointInPolyTester_GmPolygon</name>
    <filename>class_gm_point_in_poly_tester___gm_polygon.html</filename>
    <base>GmPointInPolyUnitTests</base>
    <member kind="function">
      <type></type>
      <name>GmPointInPolyTester_GmPolygon</name>
      <anchorfile>class_gm_point_in_poly_tester___gm_polygon.html</anchorfile>
      <anchor>af73511361067f8a19790783068097aa9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" virtualness="virtual">
      <type>virtual void</type>
      <name>Setup</name>
      <anchorfile>class_gm_point_in_poly_tester___gm_polygon.html</anchorfile>
      <anchor>ac0cf4568292ffeb7626320453460a1ae</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="private" virtualness="virtual">
      <type>virtual void</type>
      <name>CheckPoint</name>
      <anchorfile>class_gm_point_in_poly_tester___gm_polygon.html</anchorfile>
      <anchor>a6266bdf8e4c1276bda6063a2c419576c</anchor>
      <arglist>(const xms::Pt3d &amp;a_pt) override</arglist>
    </member>
    <member kind="function" protection="private" virtualness="virtual">
      <type>virtual double</type>
      <name>MaxTime</name>
      <anchorfile>class_gm_point_in_poly_tester___gm_polygon.html</anchorfile>
      <anchor>a7c2b20442710d8f70394600de6fd9342</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>XM_DISALLOW_COPY_AND_ASSIGN</name>
      <anchorfile>class_gm_point_in_poly_tester___gm_polygon.html</anchorfile>
      <anchor>a55c128adfb44a4c87abd9f00a9ebd806</anchor>
      <arglist>(GmPointInPolyTester_GmPolygon)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; xms::GmPolygon &gt;</type>
      <name>m_poly</name>
      <anchorfile>class_gm_point_in_poly_tester___gm_polygon.html</anchorfile>
      <anchor>ab36a65f45d348e61c2c54700187523c3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GmPointInPolyUnitTests</name>
    <filename>class_gm_point_in_poly_unit_tests.html</filename>
    <member kind="function">
      <type></type>
      <name>GmPointInPolyUnitTests</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>a242ee9e55d08b8eab4105d7592bf1dd1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DoTest</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>aad892797953c9679e60aacab95c59a27</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual void</type>
      <name>CheckPoint</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>a6c8f3ccf1f1fc9b26f4354bea316c6a2</anchor>
      <arglist>(const xms::Pt3d &amp;a_pt)=0</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual double</type>
      <name>MaxTime</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>ab3908f443552a3365b065a8c9a8cf0c9</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>Setup</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>a4a90399854ed590bb00b89598880f443</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>CheckPoints</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>a9fbcdd807e9e110065b9b44fe07755c0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>GetResults</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>a6faf1c86377a32334873eb63bcb6cfad</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; xms::Pt3d &gt;</type>
      <name>m_outPoly</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>ae4af81c2000f795508bdff86bd95891c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; xms::Pt3d &gt;</type>
      <name>m_inPoly</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>a8d9672a8e39a9b6e40b45e9ee4d6d4a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>boost::timer::cpu_timer</type>
      <name>m_timer</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>abb3f853c60745ea6285525833cf5a1ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>m_count</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>a1e67ce3271a3d8cee046bbc18ee040f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>m_status</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>a1ed8b155abbce252e8bacc00d91d0e36</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>XM_DISALLOW_COPY_AND_ASSIGN</name>
      <anchorfile>class_gm_point_in_poly_unit_tests.html</anchorfile>
      <anchor>ad7cbb8b88a9b3d04a585dac034e7d5e3</anchor>
      <arglist>(GmPointInPolyUnitTests)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmPolygon</name>
    <filename>classxms_1_1_gm_polygon.html</filename>
    <member kind="function">
      <type></type>
      <name>GmPolygon</name>
      <anchorfile>classxms_1_1_gm_polygon.html</anchorfile>
      <anchor>a5c9b3f01094c446f998eb5d898ff5ff4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~GmPolygon</name>
      <anchorfile>classxms_1_1_gm_polygon.html</anchorfile>
      <anchor>a044fc1abedbc680be3610e0078383b9c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static BSHP&lt; GmPolygon &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_gm_polygon.html</anchorfile>
      <anchor>ad8093f52a1162981b9385f02ab74f020</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmPolygonImpl</name>
    <filename>classxms_1_1_gm_polygon_impl.html</filename>
    <base>xms::GmPolygon</base>
    <member kind="function">
      <type></type>
      <name>GmPolygonImpl</name>
      <anchorfile>classxms_1_1_gm_polygon_impl.html</anchorfile>
      <anchor>af30376ca5d7e9312c1ac5d9c7be5fb0d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>Setup</name>
      <anchorfile>classxms_1_1_gm_polygon_impl.html</anchorfile>
      <anchor>af66ebecefe6f91f7cb9d79a1937dfa34</anchor>
      <arglist>(const VecPt3d &amp;a_poly, const VecPt3d2d &amp;a_inPolys) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>GetPoints</name>
      <anchorfile>classxms_1_1_gm_polygon_impl.html</anchorfile>
      <anchor>ad1a1750096fbfda938910e3ec19d573a</anchor>
      <arglist>(VecPt3d &amp;a_poly, VecPt3d2d &amp;a_inPolys) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>CoveredBy</name>
      <anchorfile>classxms_1_1_gm_polygon_impl.html</anchorfile>
      <anchor>ab532f128e5d4ac3c9d5084d639d0c3bf</anchor>
      <arglist>(Pt3d a_point) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>Within</name>
      <anchorfile>classxms_1_1_gm_polygon_impl.html</anchorfile>
      <anchor>ab4fb31f5ae09331f8abb148e9374a0fa</anchor>
      <arglist>(Pt3d a_point) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual double</type>
      <name>MinDistanceToBoundary</name>
      <anchorfile>classxms_1_1_gm_polygon_impl.html</anchorfile>
      <anchor>a9fc163819fdbfc8e49b6a1ad1716661f</anchor>
      <arglist>(Pt3d a_pt) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>Intersection</name>
      <anchorfile>classxms_1_1_gm_polygon_impl.html</anchorfile>
      <anchor>a3dbbbff726ffd85dfeca5b5bd0a1e81e</anchor>
      <arglist>(const GmPolygon &amp;a_, std::vector&lt; BSHP&lt; GmPolygon &gt;&gt; &amp;a_out) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>Union</name>
      <anchorfile>classxms_1_1_gm_polygon_impl.html</anchorfile>
      <anchor>ae30032a3f50f76e8698cd3279c1441db</anchor>
      <arglist>(const GmPolygon &amp;a_, std::vector&lt; BSHP&lt; GmPolygon &gt;&gt; &amp;a_out) const override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>GmBstPoly3d</type>
      <name>m_poly</name>
      <anchorfile>classxms_1_1_gm_polygon_impl.html</anchorfile>
      <anchor>a1b8f4cc90f8c30f3f2b8ede8b5403054</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GmPolygonIntermediateTests</name>
    <filename>class_gm_polygon_intermediate_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testSpeed</name>
      <anchorfile>class_gm_polygon_intermediate_tests.html</anchorfile>
      <anchor>af601eb8418e0b14047243e8fe948a757</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GmPolygonUnitTests</name>
    <filename>class_gm_polygon_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testNoHoles</name>
      <anchorfile>class_gm_polygon_unit_tests.html</anchorfile>
      <anchor>a0978e7f7e86b26f1a4e971b3e5d8a31a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testWithHoles</name>
      <anchorfile>class_gm_polygon_unit_tests.html</anchorfile>
      <anchor>aebc8f3e95304faa3551b291634bcfa40</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testMinDistanceToBoundary</name>
      <anchorfile>class_gm_polygon_unit_tests.html</anchorfile>
      <anchor>a7bc270e0a221a9f869972d6fe695feff</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testIntersection</name>
      <anchorfile>class_gm_polygon_unit_tests.html</anchorfile>
      <anchor>a06d62122ec097576fbe8e2831c434259</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testUnion</name>
      <anchorfile>class_gm_polygon_unit_tests.html</anchorfile>
      <anchor>a41d0b777472b559dcada412c42ee2f3d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>SetUpPolyWithHoles1</name>
      <anchorfile>class_gm_polygon_unit_tests.html</anchorfile>
      <anchor>aadbd64575fc6bf90021c7ddaa10b3e28</anchor>
      <arglist>(xms::VecPt3d &amp;a_outPoly, xms::VecPt3d2d &amp;a_inPolys, xms::VecPt3d &amp;a_ptsToTest)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>SetUpPolyWithHole</name>
      <anchorfile>class_gm_polygon_unit_tests.html</anchorfile>
      <anchor>abb784f9e329491d2e69532a3834a8226</anchor>
      <arglist>(xms::VecPt3d &amp;a_outPoly, xms::VecPt3d &amp;a_inPolys)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmPolyLinePtRedistributer</name>
    <filename>classxms_1_1_gm_poly_line_pt_redistributer.html</filename>
    <member kind="function" static="yes">
      <type>static BSHP&lt; GmPolyLinePtRedistributer &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_gm_poly_line_pt_redistributer.html</anchorfile>
      <anchor>a058c1ce2832a7e0b76aaa6586199cc2d</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmPolyLinePtRedistributerImpl</name>
    <filename>classxms_1_1_gm_poly_line_pt_redistributer_impl.html</filename>
    <base>xms::GmPolyLinePtRedistributer</base>
    <member kind="function" virtualness="virtual">
      <type>virtual VecPt3d</type>
      <name>Redistribute</name>
      <anchorfile>classxms_1_1_gm_poly_line_pt_redistributer_impl.html</anchorfile>
      <anchor>a0fc65a7270b63dd43103de7051d82f44</anchor>
      <arglist>(const VecPt3d &amp;a_input, double a_size) override</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>PolyLineLengths</name>
      <anchorfile>classxms_1_1_gm_poly_line_pt_redistributer_impl.html</anchorfile>
      <anchor>a1d78dd1114e60e22ba262a26d3fa364f</anchor>
      <arglist>(const VecPt3d &amp;a_pts, VecDbl &amp;a_lengths)</arglist>
    </member>
    <member kind="function">
      <type>VecPt3d</type>
      <name>RedistPolyLineWithNumSeg</name>
      <anchorfile>classxms_1_1_gm_poly_line_pt_redistributer_impl.html</anchorfile>
      <anchor>a9d3e8de1f6238353c11aa3e72f136696</anchor>
      <arglist>(const VecPt3d &amp;a_pts, double a_totalLength, const VecDbl &amp;a_lengths, int a_nSeg)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GmPolyLinePtRedistributerUnitTests</name>
    <filename>class_gm_poly_line_pt_redistributer_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testCreateClass</name>
      <anchorfile>class_gm_poly_line_pt_redistributer_unit_tests.html</anchorfile>
      <anchor>a408aa5befac9d84f88fe51828a343ddd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testRedistribute</name>
      <anchorfile>class_gm_poly_line_pt_redistributer_unit_tests.html</anchorfile>
      <anchor>a35539a5b291d522f2e6cfc597c368028</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testRedistribute1</name>
      <anchorfile>class_gm_poly_line_pt_redistributer_unit_tests.html</anchorfile>
      <anchor>ae730c8c2625bc12b21001743446a9ce8</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmPtSearch</name>
    <filename>classxms_1_1_gm_pt_search.html</filename>
    <member kind="function" static="yes">
      <type>static BSHP&lt; GmPtSearch &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_gm_pt_search.html</anchorfile>
      <anchor>a883ee1e554d92caaed23923ebb39f4f8</anchor>
      <arglist>(bool a_2dSearch)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmPtSearchImpl</name>
    <filename>classxms_1_1_gm_pt_search_impl.html</filename>
    <base>xms::GmPtSearch</base>
    <member kind="function">
      <type></type>
      <name>GmPtSearchImpl</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>ac5241a77c7018ec67738437fc0117549</anchor>
      <arglist>(bool a_2dSearch)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>PtsToSearch</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>aab9e1586fb5cdb066d25e8e63d4ab10e</anchor>
      <arglist>(BSHP&lt; std::vector&lt; Pt3d &gt;&gt; a_pts) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>VectorThatGrowsToSearch</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a8397d8d68e7de98085af3ebc3e198308</anchor>
      <arglist>(BSHP&lt; std::vector&lt; Pt3d &gt;&gt; a_) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>AddPtToVectorIfUnique</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>ae4a7236045f1e28491e378f727faea78</anchor>
      <arglist>(const Pt3d &amp;a_, double a_tol, int &amp;a_ptIdx) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>NearestPtsToPt</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>acb07d6a4f730c02e2044c52ae9877f9e</anchor>
      <arglist>(const Pt3d &amp;a_pt, int a_numPtsToFind, bool a_quad_oct_Search, std::vector&lt; int &gt; &amp;a_nearest) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>NearestPtsToPtInRtree</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a1bcb75c2e478882e10daacb9768be4f0</anchor>
      <arglist>(int a_ptIdx, const Pt3d &amp;a_pt, int a_numPtsToFind, bool a_quad_oct_Search, std::vector&lt; int &gt; &amp;a_nearest) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>PtInRTree</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>abc2c132b03146ef72209c914418ebf2a</anchor>
      <arglist>(const Pt3d &amp;a_pt, const double a_tol) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>PtsWithinDistanceToPtInRtree</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a7c3348725a96a2bbef9bc9ed28742c03</anchor>
      <arglist>(int a_ptIdx, const Pt3d &amp;a_pt, double a_dist, std::vector&lt; int &gt; &amp;a_nearest) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>NearestPtsToPt</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a798982ff06d8f2b9d702e8c524770448</anchor>
      <arglist>(const Pt3d &amp;a_pt, int a_numPtsToFind, bool a_quad_oct_Search, std::vector&lt; int &gt; &amp;a_nearest, fSatisfies *a_fsat) const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetActivity</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>af92eb3184ddbb6ff6844f6e5c4563912</anchor>
      <arglist>(DynBitset &amp;a_activity) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DynBitset</type>
      <name>GetActivity</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a6e2738c96014e542d8ba2977ce996eac</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const BSHP&lt; VecPt3d &gt;</type>
      <name>GetPointsPtr</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a501ca6b9d117c035425e3ad64de30edf</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>Is2D</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a106b0a5f562d15608e27969a8fed1f5b</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual std::string</type>
      <name>ToString</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a4b7acbdee40b84ef7d8f4b695a04ce4f</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>UpdateMinMax</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a025de6d0fd6c227670e92acef587bfb1</anchor>
      <arglist>(const Pt3d *a_pts, size_t a_npts)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CreateOctants</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>aae9c54c626d2860a7be4fc122fb917ae</anchor>
      <arglist>(const Pt3d &amp;a_pt, std::vector&lt; box &gt; &amp;a_boxes) const</arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_2dSearch</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>acc23253ca6b707d8a84854c23c1e6914</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Pt3d</type>
      <name>m_min</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a2d87fb7faa5dc23ad3cb7a2a5fd3c451</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Pt3d</type>
      <name>m_max</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>aab93492b32a85bd6ce63b6266bea5b3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DynBitset</type>
      <name>m_activity</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a97f39190bddeb60d645f96a140898553</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bgi::rtree&lt; value, qRtree, idx_pt &gt; *</type>
      <name>m_rTree</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a412dbf1256bcb8d0666aa1a493737350</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; VecPt3d &gt;</type>
      <name>m_bshpPt3d</name>
      <anchorfile>classxms_1_1_gm_pt_search_impl.html</anchorfile>
      <anchor>a4d1a3ea0ffb1a2ca48d8d5e00146e750</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmTriSearch</name>
    <filename>classxms_1_1_gm_tri_search.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~GmTriSearch</name>
      <anchorfile>classxms_1_1_gm_tri_search.html</anchorfile>
      <anchor>ac56072f01e9da1702618ee7255bb91c0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static BSHP&lt; GmTriSearch &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_gm_tri_search.html</anchorfile>
      <anchor>a2df03859534cd0296e57bb87b7d418a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>GmTriSearch</name>
      <anchorfile>classxms_1_1_gm_tri_search.html</anchorfile>
      <anchor>ae944fa5ccf34141894a79b803b78fadc</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::GmTriSearchImpl</name>
    <filename>classxms_1_1_gm_tri_search_impl.html</filename>
    <base>xms::GmTriSearch</base>
    <member kind="function">
      <type></type>
      <name>GmTriSearchImpl</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a9b2ed0cf16ebef3d93ad379d21438d35</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~GmTriSearchImpl</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a611da2725a598042a153313f8042e4e5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>TrisToSearch</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>ab12d667cf4931879e49158ca27a7d0da</anchor>
      <arglist>(BSHP&lt; std::vector&lt; Pt3d &gt;&gt; a_pts, BSHP&lt; std::vector&lt; int &gt;&gt; a_tris) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetPtActivity</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a8f22b0237cb737b390ba5c4fc1512d64</anchor>
      <arglist>(DynBitset &amp;a_activity) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetTriActivity</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a70c35b1902abf36b4b13a6d1eebbf3cc</anchor>
      <arglist>(DynBitset &amp;a_activity) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DynBitset</type>
      <name>GetPtActivity</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a7ef7b01bc917d68bc9cbe258b6e6a9b4</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DynBitset</type>
      <name>GetTriActivity</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a07303c6ed2b3ddc399cd218d439c0446</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ActiveTri</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>ac281a7bee912f5a25c2f29596d85c8c7</anchor>
      <arglist>(int a_idx) const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>TriContainingPt</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a57e36ef11703b604721b833587277e3b</anchor>
      <arglist>(const Pt3d &amp;a_pt) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>TriEnvelopsContainingPt</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a2499bd15424d39940d7a22c0ee90862a</anchor>
      <arglist>(const Pt3d &amp;a_pt, std::vector&lt; int &gt; &amp;a_tris) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>TriEnvelopesOverlap</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a884a53233410d94455ac562ae22d783f</anchor>
      <arglist>(const Pt3d &amp;a_pMin, const Pt3d &amp;a_pMax, std::vector&lt; int &gt; &amp;a_tris) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>InterpWeights</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a89161f03d360c52d22a4d0995e1a5869</anchor>
      <arglist>(const Pt3d &amp;a_pt, std::vector&lt; int &gt; &amp;a_idxs, std::vector&lt; double &gt; &amp;a_wts) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>InterpWeightsTriangleIdx</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a6f831e143724ccc2a55c0daa7995e449</anchor>
      <arglist>(const Pt3d &amp;a_pt, int &amp;a_triangleIdx, std::vector&lt; int &gt; &amp;a_idxs, std::vector&lt; double &gt; &amp;a_wts) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const BSHP&lt; VecPt3d &gt;</type>
      <name>GetPoints</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a681a886b30b96254f6b3ab5e9cf1b1fc</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const BSHP&lt; VecInt &gt;</type>
      <name>GetTriangles</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>ac3f2d17dc8d0568e1f67b9049d3cb827</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual std::string</type>
      <name>ToString</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>ab61cf81f803d0d87fcbf6e2410ae2d25</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CreateRTree</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>ae9759f6aa25a8897ecc21e3f7edb80d7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>PointIdxesFromTriIdx</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>ab3a89a592db07b714e4a5eb8d12037c6</anchor>
      <arglist>(int a_triIdx, int a_ptIdxes[3])</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>FindTriangle</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a040343fd456349661bbbf82dda01e9c5</anchor>
      <arglist>(const Pt3d &amp;a_pt, int ix[3], Pt3d &amp;weights)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GetTriBarycentricVals</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a21de9e3170fcd10467c14aeea25d74be</anchor>
      <arglist>(int a_idx, int a_ix[3], BarycentricVals &amp;a_b)</arglist>
    </member>
    <member kind="function">
      <type>DynBitset</type>
      <name>PointActivityFromTriActivity</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>aa3ae01b7bd0dfc9648e51550dfc8d8b9</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>BSHP&lt; GmPtSearch &gt;</type>
      <name>CreatePtSearch</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a61ab221cb9ff1755cf300597fbc10600</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>Pt3d</type>
      <name>m_min</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>ab5dafd8842cf0404cc3ea8aa0d1aa610</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Pt3d</type>
      <name>m_max</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a5abff431d736788f27904778ea9ef0cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; std::vector&lt; Pt3d &gt; &gt;</type>
      <name>m_pts</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a34d64d02d4d92142cb47b368c86ec739</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; std::vector&lt; int &gt; &gt;</type>
      <name>m_tris</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a500ef32dbd0fc06a8384a67656b5e6a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DynBitset</type>
      <name>m_triActivity</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>aad9adb0543bcb70198143b35a71bc950</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>boost::unordered_map&lt; size_t, BarycentricVals &gt;</type>
      <name>m_cache</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a02245069dba9182140afe37cbc08d6f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bgi::rtree&lt; value, qRtree, idx_tri &gt; *</type>
      <name>m_rTree</name>
      <anchorfile>classxms_1_1_gm_tri_search_impl.html</anchorfile>
      <anchor>a91899c759fe0fd8b1e230c1efb650138</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::idx_pt</name>
    <filename>classxms_1_1idx__pt.html</filename>
    <member kind="typedef">
      <type>const bPt</type>
      <name>result_type</name>
      <anchorfile>classxms_1_1idx__pt.html</anchorfile>
      <anchor>ad127b38c3e9fb6c1d652fa2b6f75880d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>idx_pt</name>
      <anchorfile>classxms_1_1idx__pt.html</anchorfile>
      <anchor>a2b7929bfcd2024966dd873f3e952a345</anchor>
      <arglist>(const Pt3d *a_, bool a_2d)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>idx_pt</name>
      <anchorfile>classxms_1_1idx__pt.html</anchorfile>
      <anchor>a709964a4e328dd7f2e693eb9833031c1</anchor>
      <arglist>(BSHP&lt; std::vector&lt; Pt3d &gt;&gt; a_, bool a_2d)</arglist>
    </member>
    <member kind="function">
      <type>result_type</type>
      <name>operator()</name>
      <anchorfile>classxms_1_1idx__pt.html</anchorfile>
      <anchor>ab7d7c2492883daab3ed8211b1a4468c2</anchor>
      <arglist>(size_t i) const</arglist>
    </member>
    <member kind="variable">
      <type>const Pt3d *</type>
      <name>m_</name>
      <anchorfile>classxms_1_1idx__pt.html</anchorfile>
      <anchor>a0187e9f5054b0c3388e299239311646b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_2d</name>
      <anchorfile>classxms_1_1idx__pt.html</anchorfile>
      <anchor>aec9584b8b51e6795e4e0106187b68a5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; std::vector&lt; Pt3d &gt; &gt;</type>
      <name>m_v</name>
      <anchorfile>classxms_1_1idx__pt.html</anchorfile>
      <anchor>a90678c68a3faa676fb06b081a6e070c6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::idx_tri</name>
    <filename>classxms_1_1idx__tri.html</filename>
    <member kind="typedef">
      <type>const GmBstBox3d</type>
      <name>result_type</name>
      <anchorfile>classxms_1_1idx__tri.html</anchorfile>
      <anchor>a88c4b0524e400a243b939e69b13fa9fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>idx_tri</name>
      <anchorfile>classxms_1_1idx__tri.html</anchorfile>
      <anchor>a18b7dba501483fa3ebb141f439497dfc</anchor>
      <arglist>(const Pt3d *a_pts, const int *a_tris, double a_tol)</arglist>
    </member>
    <member kind="function">
      <type>result_type</type>
      <name>operator()</name>
      <anchorfile>classxms_1_1idx__tri.html</anchorfile>
      <anchor>acb9ee91c68bfa303474a193eb7769614</anchor>
      <arglist>(size_t i) const</arglist>
    </member>
    <member kind="variable">
      <type>const Pt3d *</type>
      <name>m_pts</name>
      <anchorfile>classxms_1_1idx__tri.html</anchorfile>
      <anchor>a13730645297b2eb5f6ef6e657d878dc4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const int *</type>
      <name>m_tris</name>
      <anchorfile>classxms_1_1idx__tri.html</anchorfile>
      <anchor>a2f6cabf2a7d2ac6ae5fe7c349262fcf9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>m_tol</name>
      <anchorfile>classxms_1_1idx__tri.html</anchorfile>
      <anchor>ad89989a3a633daba4bcea58b7168ab79</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::ThreadLoop::impl</name>
    <filename>classxms_1_1_thread_loop_1_1impl.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>m_start</name>
      <anchorfile>classxms_1_1_thread_loop_1_1impl.html</anchorfile>
      <anchor>a886105abdaebf1d3eb819f1df3093074</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>m_nIter</name>
      <anchorfile>classxms_1_1_thread_loop_1_1impl.html</anchorfile>
      <anchor>ac209d4aea4fe92e75135fa17c89d1426</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>m_curIdx</name>
      <anchorfile>classxms_1_1_thread_loop_1_1impl.html</anchorfile>
      <anchor>aec808dae7359abc58fd3eac8e437aff7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int *</type>
      <name>m_cntPtr</name>
      <anchorfile>classxms_1_1_thread_loop_1_1impl.html</anchorfile>
      <anchor>aa5e9e6aa7aa2b80772259d9a74d2cd2b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::InterpAnisotropic</name>
    <filename>classxms_1_1_interp_anisotropic.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~InterpAnisotropic</name>
      <anchorfile>classxms_1_1_interp_anisotropic.html</anchorfile>
      <anchor>af29171e702091bf6fe0cb2a4447066c9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static BSHP&lt; InterpAnisotropic &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_interp_anisotropic.html</anchorfile>
      <anchor>a06efc4ab20831217af942a2e5d3502a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>InterpAnisotropic</name>
      <anchorfile>classxms_1_1_interp_anisotropic.html</anchorfile>
      <anchor>a0432a313b7e41c6a519ab06b3dbb96a0</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::InterpAnisotropicImpl</name>
    <filename>classxms_1_1_interp_anisotropic_impl.html</filename>
    <base>xms::InterpAnisotropic</base>
    <member kind="function">
      <type></type>
      <name>InterpAnisotropicImpl</name>
      <anchorfile>classxms_1_1_interp_anisotropic_impl.html</anchorfile>
      <anchor>a73893769a24ae33fc3665b52b7024f15</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~InterpAnisotropicImpl</name>
      <anchorfile>classxms_1_1_interp_anisotropic_impl.html</anchorfile>
      <anchor>a0a131930bf485ac5fe2fc0c1370054b7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetPoints</name>
      <anchorfile>classxms_1_1_interp_anisotropic_impl.html</anchorfile>
      <anchor>a111eff2131d6245b30eeacf547510afe</anchor>
      <arglist>(const VecPt3d &amp;a_centerlinePts, const VecPt3d &amp;a_interpolationPts, bool a_pickClosest=false) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual float</type>
      <name>InterpToPt</name>
      <anchorfile>classxms_1_1_interp_anisotropic_impl.html</anchorfile>
      <anchor>a3f734cddb991436291d6280434b75b79</anchor>
      <arglist>(const Pt3d &amp;a_pt) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>InterpToPts</name>
      <anchorfile>classxms_1_1_interp_anisotropic_impl.html</anchorfile>
      <anchor>a383209d6e6374ac6ee7d940ba2c2bb29</anchor>
      <arglist>(const VecPt3d &amp;a_pts, VecFlt &amp;a_result) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>GetInterpolationPts</name>
      <anchorfile>classxms_1_1_interp_anisotropic_impl.html</anchorfile>
      <anchor>ab56e497decc246f046d546659879b038</anchor>
      <arglist>(VecPt3d &amp;a_interpPoints) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>GetTransformedPts</name>
      <anchorfile>classxms_1_1_interp_anisotropic_impl.html</anchorfile>
      <anchor>a854d6ca9a691e4cdcd6f691f77a0a6d6</anchor>
      <arglist>(const VecPt3d &amp;a_points, bool a_pickClosest, VecPt3d &amp;a_transformed) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetPower</name>
      <anchorfile>classxms_1_1_interp_anisotropic_impl.html</anchorfile>
      <anchor>ab5165013804292714faded5175c70c71</anchor>
      <arglist>(double a_power) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetXScale</name>
      <anchorfile>classxms_1_1_interp_anisotropic_impl.html</anchorfile>
      <anchor>a092e613fb20cc880ff79c50a4af3dcd4</anchor>
      <arglist>(double a_xScale) override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>AnisotropicInterpolator</type>
      <name>m_anisoInterp</name>
      <anchorfile>classxms_1_1_interp_anisotropic_impl.html</anchorfile>
      <anchor>a4bc52baf23405f3f287b500e76fe62eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>m_xScale</name>
      <anchorfile>classxms_1_1_interp_anisotropic_impl.html</anchorfile>
      <anchor>ab162ebee3d218bf67cc5b75835ae49fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>m_power</name>
      <anchorfile>classxms_1_1_interp_anisotropic_impl.html</anchorfile>
      <anchor>a5d43d06b84d3c8238060addec5863712</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>InterpAnisotropicUnitTests</name>
    <filename>class_interp_anisotropic_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testComplex</name>
      <anchorfile>class_interp_anisotropic_unit_tests.html</anchorfile>
      <anchor>ab198746391825b86ccc5d90157ca72e9</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::InterpBase</name>
    <filename>classxms_1_1_interp_base.html</filename>
  </compound>
  <compound kind="class">
    <name>xms::InterpCt</name>
    <filename>classxms_1_1_interp_ct.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>RegionEnum</name>
      <anchorfile>classxms_1_1_interp_ct.html</anchorfile>
      <anchor>adc36a58e814df9d4398adbb2fd30864b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>InterpCt</name>
      <anchorfile>classxms_1_1_interp_ct.html</anchorfile>
      <anchor>a4a7e480d5f0c0a7edc21ff4e4bb22e6a</anchor>
      <arglist>(const std::vector&lt; Pt3d &gt; &amp;a_pts, BSHP&lt; NodalFunc &gt; a_n)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ComputeCtCoeff</name>
      <anchorfile>classxms_1_1_interp_ct.html</anchorfile>
      <anchor>a13ff777750202447aba55dc6d99e00b0</anchor>
      <arglist>(int *a_ptIdxs)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>InterpToPt</name>
      <anchorfile>classxms_1_1_interp_ct.html</anchorfile>
      <anchor>a1e5b862fa5cd4643ae2a8aad5e55f782</anchor>
      <arglist>(const Pt3d &amp;a_pt)</arglist>
    </member>
    <member kind="function">
      <type>RegionEnum</type>
      <name>DetermineRegion</name>
      <anchorfile>classxms_1_1_interp_ct.html</anchorfile>
      <anchor>aa13b461213f3eefd295cb3fcd9d7bc86</anchor>
      <arglist>(double e, double n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RecalcNodalFunc</name>
      <anchorfile>classxms_1_1_interp_ct.html</anchorfile>
      <anchor>a5d84a1d18d663c54aef7c8fbfc9a94e6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>ToString</name>
      <anchorfile>classxms_1_1_interp_ct.html</anchorfile>
      <anchor>a0600e22511fc11edd0f9aec4aa9e83cf</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="variable">
      <type>const std::vector&lt; Pt3d &gt; &amp;</type>
      <name>m_pts</name>
      <anchorfile>classxms_1_1_interp_ct.html</anchorfile>
      <anchor>a1fa33c8155599c39722851b1bd27b994</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; NodalFunc &gt;</type>
      <name>m_nodalFunc</name>
      <anchorfile>classxms_1_1_interp_ct.html</anchorfile>
      <anchor>a2e0e35634b527fd3c85f4bab5a074383</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::InterpIdw</name>
    <filename>classxms_1_1_interp_idw.html</filename>
    <base>xms::InterpBase</base>
    <member kind="enumeration">
      <type></type>
      <name>WeightEnum</name>
      <anchorfile>classxms_1_1_interp_idw.html</anchorfile>
      <anchor>a75db3e802021d0171f8a8e0077d9d0fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>NodalFuncEnum</name>
      <anchorfile>classxms_1_1_interp_idw.html</anchorfile>
      <anchor>a9f0d45e41829c4a94b53dd336d086f4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~InterpIdw</name>
      <anchorfile>classxms_1_1_interp_idw.html</anchorfile>
      <anchor>ac4decabe930f9ef4075e54e3f9e07e48</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static BSHP&lt; InterpIdw &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_interp_idw.html</anchorfile>
      <anchor>ada220d36126708676e57d6b9eac2f3d7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>InterpIdw</name>
      <anchorfile>classxms_1_1_interp_idw.html</anchorfile>
      <anchor>a4a3e66a3a072673a38ab47fd74618899</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::InterpIdwImpl</name>
    <filename>classxms_1_1_interp_idw_impl.html</filename>
    <base>xms::InterpIdw</base>
    <class kind="class">xms::InterpIdwImpl::InterpThread</class>
    <member kind="function">
      <type></type>
      <name>InterpIdwImpl</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>abe90be24339d7ffd104efc2b29bdcc63</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetPtsTris</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>af241f63a703b1ec13fe0409710f74b1e</anchor>
      <arglist>(BSHP&lt; VecPt3d &gt; a_pts, BSHP&lt; VecInt &gt; a_tris) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetScalars</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a4b3657d29033724bb6f585a52a9c6b45</anchor>
      <arglist>(const float *a_scalar, size_t a_n) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetScalars</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>ab2a3b3a57efec2541b44a71b50e65eb1</anchor>
      <arglist>(BSHP&lt; VecFlt &gt; a_scalar) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetPts</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>ac6c7a272f475452822ad8cf6e0f4b0f6</anchor>
      <arglist>(BSHP&lt; VecPt3d &gt; a_pts, bool a_2d) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual float</type>
      <name>InterpToPt</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a0b6241d646bacd9de44bcf1e346e094d</anchor>
      <arglist>(const Pt3d &amp;a_pt) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>InterpToPts</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a9262e2a2544a0b094aaca1e155f67dae</anchor>
      <arglist>(const VecPt3d &amp;a_pts, VecFlt &amp;a_scalars) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetPtActivity</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>af17b9db69cd62e21e2a566f38463491d</anchor>
      <arglist>(DynBitset &amp;a_activity) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetTriActivity</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a46b0f8615ccad8d9b07053285275ecfb</anchor>
      <arglist>(DynBitset &amp;) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const BSHP&lt; VecPt3d &gt;</type>
      <name>GetPts</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>aa55c4098a59244f43ea7a76193f2b478</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const BSHP&lt; VecInt &gt;</type>
      <name>GetTris</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a04cdf9e0213da80deaf52c4a991ffffd</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const BSHP&lt; VecFlt &gt;</type>
      <name>GetScalars</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a92b2dcf7c9a01c3d4cf617482e09007f</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DynBitset</type>
      <name>GetPtActivity</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a639ecc0af8c0e7888ae3540aa14066f3</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DynBitset</type>
      <name>GetTriActivity</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a7f3c4ccce6f66f5c549e2f73374be805</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetTrunc</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a3360ed1fa84ea3addbb31580c4cfbb77</anchor>
      <arglist>(double a_sMax, double a_sMin) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>GetTruncateInterpolatedValues</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a9b6489912d08bf9e6eebcd572b61027a</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual double</type>
      <name>GetTruncMin</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>acb44608fff49391648e5058a27f112a4</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual double</type>
      <name>GetTruncMax</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>ac34746d9bb3585c8ca9d2232d9f46a9d</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual double</type>
      <name>GetPower</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>aa5a09993da45364d389ac578ccdce504</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>GetSearchOptsNumNearestPts</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a42e768e1973f2f17748fcf55068f2be7</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>GetSearchOptsUseQuadrantSearch</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a9752e659a67f10c05b9c224bab215d97</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual WeightEnum</type>
      <name>GetWeightCalcMethod</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>af06e10333da8f56bd14a325ad968dd6c</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual NodalFuncEnum</type>
      <name>GetNodalFunctionType</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a31a55840e5bab818f92ecf9adf7b9d52</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>GetNodalFunctionNumNearestPts</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a8d161c9fff76798e02e881b1560d7829</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>GetNodalFunctionUseQuadrantSearch</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a6c3a6a5cc8879319ef34119adbdef1dc</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetSaveWeights</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>ad8e7e3a8ff23af9ca36cec3a9deed4f8</anchor>
      <arglist>(bool a_) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>InterpWeights</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>abe0bcead4d8ba888b08fa19bb188a03b</anchor>
      <arglist>(const Pt3d &amp;a_pt, VecInt &amp;a_idx, VecDbl &amp;a_wt) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetMultiThreading</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a95ea5690b1c3e9de099ce4478e138cca</anchor>
      <arglist>(bool a_) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual std::string</type>
      <name>ToString</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>aca966e59b934c8ccf1301fa8ed1d36b4</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetPower</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a88a8bc3e2e027ca134877f3aa88b23d0</anchor>
      <arglist>(double a_power) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RecalcNodalFunc</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>aae5e7543751ab81d1d46648a9acffec1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetSearchOpts</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a467e27bca94097b2f08214d8695c3125</anchor>
      <arglist>(int a_nNearestPoints, bool a_quad_oct_Search) override</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>InterpToPt</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a08b2d3d1b3115930ab67a0d8aee0c1bf</anchor>
      <arglist>(const Pt3d &amp;a_pt, int a_idx) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetObserver</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a9e778c77863691c71b4fcc952bdedd97</anchor>
      <arglist>(BSHP&lt; Observer &gt; a_prog) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ValFromWeights</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>abcca11a941582407c4764175f2306339</anchor>
      <arglist>(VecDbl &amp;a_w, VecInt &amp;a_nPts, float &amp;a_val, int a_ptIdx, const Pt3d &amp;a_pt) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetWeightCalcMethod</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a152be78d49b473101a31a8645a34073d</anchor>
      <arglist>(InterpIdw::WeightEnum a_) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetNodalFunction</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a95e713404bb178081c0c4f98ef541e8d</anchor>
      <arglist>(NodalFuncEnum a_, int a_nNearest, bool a_quad_oct, BSHP&lt; Observer &gt; a_p) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CreateNodalFunctionPtr</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a208c6b6ab2b19093e1f1d207b4b85091</anchor>
      <arglist>(NodalFuncEnum a_, int a_nNearest, bool a_quad_oct, BSHP&lt; Observer &gt; a_p)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>ScalarFromNodalFunc</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>acd590521fc17684d7e24b29a3064808a</anchor>
      <arglist>(int a_ptIdx, const Pt3d &amp;a_loc) const</arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_2d</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a89798e64a57bd3674aa0191b05a1bfb6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_quadOctSearch</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>aedfc573a93a865157cba7d62e94c8396</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_modifiedShepardWeights</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a29d8491cf7291c3aeaf0556a1d25bd06</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>m_nNearestPts</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a3b5231a2f3ae69e877a135e1c1631561</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>m_power</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a725548983086139fc68612438afe1b4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; GmPtSearch &gt;</type>
      <name>m_ptSearch</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a9f2e2caf2bf4fa2e2449c4e6ad314ddf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; VecPt3d &gt;</type>
      <name>m_pts</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>ad2362456e5e398a247b1abfbe92b6ad0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; VecInt &gt;</type>
      <name>m_tris</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>acdfeb655e85c0ca17712df838d668271</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; VecFlt &gt;</type>
      <name>m_scalarFrom</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>aab3aed24914630596b37ab676194425c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; NodalFunc &gt;</type>
      <name>m_nodalFunc</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a2fe74f3c6076182c96f80756c94efa9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; Observer &gt;</type>
      <name>m_prog</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a90d7651f0110ac2aed65074dadc65fc4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_saveWeights</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>aef1ca833d7e5738d07a7ca05a22411c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VecInt2d</type>
      <name>m_ptIdx</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a965e0cf697e00394cb1317076b4b80cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VecDbl2d</type>
      <name>m_weights</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>ad182cdbf040e9b2b7703feed5b27c26e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_trunc</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a3602605e9366da768f4b21196e9fb1ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>m_truncMax</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a8e6a28fea5b353155205dbd1686257e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>m_truncMin</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a5eb5d0d1fd76f234cef91aa4fd493020</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_multiThread</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>aae59e8cfcd72425119a286bc7b52a207</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::string</type>
      <name>m_idString</name>
      <anchorfile>classxms_1_1_interp_idw_impl.html</anchorfile>
      <anchor>a031fa6f2aeca5ae3e15cb01c42b8dae6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>InterpIdwUnitTests</name>
    <filename>class_interp_idw_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testCreateClass</name>
      <anchorfile>class_interp_idw_unit_tests.html</anchorfile>
      <anchor>a8396f3f0730a12892a07e3349b41f01b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testInterpToPts</name>
      <anchorfile>class_interp_idw_unit_tests.html</anchorfile>
      <anchor>a96249279e0569b4deaf1d68aa3a330cb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testInterp2d_a</name>
      <anchorfile>class_interp_idw_unit_tests.html</anchorfile>
      <anchor>a7f2d074fef918ec9a5c6308ea59f3960</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testInterp2d_b</name>
      <anchorfile>class_interp_idw_unit_tests.html</anchorfile>
      <anchor>a4fac354cd0c7e881cd236e88daa9831c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testInterp2d_c</name>
      <anchorfile>class_interp_idw_unit_tests.html</anchorfile>
      <anchor>a51c2aa14e0e672fb206bac8321be1072</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testInterp2d_d</name>
      <anchorfile>class_interp_idw_unit_tests.html</anchorfile>
      <anchor>a702b7e88f13f12a3bd67f63c72a0e36e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testInterp2d_e</name>
      <anchorfile>class_interp_idw_unit_tests.html</anchorfile>
      <anchor>ac84b4f83b6203959642968681bce102d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testInterp2d_f</name>
      <anchorfile>class_interp_idw_unit_tests.html</anchorfile>
      <anchor>afe6cf9c03c045a8b9082af6cea786b30</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testInterp3d</name>
      <anchorfile>class_interp_idw_unit_tests.html</anchorfile>
      <anchor>aa60647607d12677033b9a06101ca8626</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testErrors</name>
      <anchorfile>class_interp_idw_unit_tests.html</anchorfile>
      <anchor>a948550d5bea2c912f62ac7fb2ff2ec35</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testErrors2</name>
      <anchorfile>class_interp_idw_unit_tests.html</anchorfile>
      <anchor>aaa8031ea95bf6c80acf5c5cf0fa0c2cb</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::InterpLinear</name>
    <filename>classxms_1_1_interp_linear.html</filename>
    <base>xms::InterpBase</base>
    <member kind="function" static="yes">
      <type>static BSHP&lt; InterpLinear &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_interp_linear.html</anchorfile>
      <anchor>a8e04b96fee6e50961ba278d892373911</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::InterpLinearImpl</name>
    <filename>classxms_1_1_interp_linear_impl.html</filename>
    <base>xms::InterpLinear</base>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetPtsTris</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a4b52543644d517892757bf2363fea064</anchor>
      <arglist>(BSHP&lt; VecPt3d &gt; a_, BSHP&lt; VecInt &gt; a_tris) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetScalars</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>abaa3cc7cb47464c967ab8f1d3b8bee44</anchor>
      <arglist>(const float *a_scalar, size_t a_n) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetScalars</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>ad286a6c5141735b1752177f248488a82</anchor>
      <arglist>(BSHP&lt; VecFlt &gt; a_scalar) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetPtActivity</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>aa302f97d61fbd90a98fd0283585578e2</anchor>
      <arglist>(DynBitset &amp;a_activity) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetTriActivity</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>acc52e28b25affec198ba0d32b869ae15</anchor>
      <arglist>(DynBitset &amp;a_activity) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const BSHP&lt; VecPt3d &gt;</type>
      <name>GetPts</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>ac01c636f694f3a957257e4668a5564d4</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const BSHP&lt; VecInt &gt;</type>
      <name>GetTris</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a846ac7162509c7331389570e7c3c637b</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const BSHP&lt; VecFlt &gt;</type>
      <name>GetScalars</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>af60b3d1f6512e52d93793b847ffeb9aa</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DynBitset</type>
      <name>GetPtActivity</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>abbbcf1e86f2e49326f4481e7392690fa</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DynBitset</type>
      <name>GetTriActivity</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>aa2441661030f3dc05ca12addee8f04e2</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual VecInt</type>
      <name>GetExtrapolationPointIndexes</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>ab007495c2c27769e269461063d6e6614</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual float</type>
      <name>InterpToPt</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>ada8409ec023fe78c4df3de1d2f133dc7</anchor>
      <arglist>(const Pt3d &amp;a_pt) override</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>InterpToPtImpl</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>aee7cddf436ed89f9997f9c2396a16a9c</anchor>
      <arglist>(const Pt3d &amp;a_pt, int a_ptIdx)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>InterpToPts</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>affebfe1c64d7b27d8fd721b169910884</anchor>
      <arglist>(const VecPt3d &amp;a_pts, VecFlt &amp;a_scalars) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>TriContainingPt</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>ac8107c6869e504f2e0add2769898953c</anchor>
      <arglist>(const Pt3d &amp;a_pt) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>TriEnvelopsContainingPt</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a991ed7ef9ad25e4eaf257e052fffd04d</anchor>
      <arglist>(const Pt3d &amp;a_pt, VecInt &amp;a_tris) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>InterpWeights</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>aea728571e8f169e0491abcbe8219272c</anchor>
      <arglist>(const Pt3d &amp;a_pt, VecInt &amp;a_idxs, VecDbl &amp;a_wts) override</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>CloughTocherInterp</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>aa64567760169c6f7b7fbec3a764aceef</anchor>
      <arglist>(VecInt &amp;a_idxs, const Pt3d &amp;a_pt)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>NatNeighInterp</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a6904e3056dca241fbe8b494b7e3fe967</anchor>
      <arglist>(const Pt3d &amp;a_pt)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetExtrapVal</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a817fbaba11a0b7f67404612aa9a554dc</anchor>
      <arglist>(double a_val) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetTrunc</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a3c6db349f54ec7f7a27a1118057a1ad7</anchor>
      <arglist>(double a_sMax, double a_sMin) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetUseCloughTocher</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a0b726a09f9dda616956380ddf9ce16ce</anchor>
      <arglist>(bool a_, BSHP&lt; Observer &gt; a_prog) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetUseNatNeigh</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a98827037d28e5c1ce5c245c59bde6e01</anchor>
      <arglist>(bool a_, int a_nodalFunc, int a_ndFuncOpt, int a_ndFuncNumNearestPts, bool a_blendWeights, BSHP&lt; Observer &gt; a_prog) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual double</type>
      <name>GetExtrapVal</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a2c13d0e38c81562431d316c428c3ba92</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>GetTruncateInterpolatedValues</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a64db6f5289b66ac723e21a8b3f79899a</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual double</type>
      <name>GetTruncMin</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a682a5b07aecd35657dec5746d6db29e9</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual double</type>
      <name>GetTruncMax</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a2f7551359232cf033d6c88f46e4b79c4</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>GetUseCloughTocher</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a2f4d051923ca71ef2c93c90726405ee6</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>GetUseNatNeigh</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a3836e65e2ba21fcdd3a9b5424e2aa848</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>GetNatNeighNodalFunc</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a8a5e80c6e561079c6aac8fe5ce67fe74</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>GetNatNeighNodalFuncNearestPtsOption</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>ad4add3b5504a96121bc50282f164cfd3</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>GetNatNeighNodalFuncNumNearestPts</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a8535e41ffce0ff3002a7600812317126</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>GetNatNeighBlendWeights</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>ac70c2aaf75bc6b9ac2dfb2b2a99a5783</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function">
      <type>BSHP&lt; GmPtSearch &gt;</type>
      <name>CreatePtSearch</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>ae8bd94a12f04721c20c08baff8bd2539</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RecalcNodalFunc</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a023ce8de9ef4823edac0e79c53e435b9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual std::string</type>
      <name>ToString</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a0145d7a1f9d28fc67ced0479694cf6a6</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BSHP&lt; GmTriSearch &gt;</type>
      <name>m_triSearch</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>aaec9b921882fb7e57e73c0d855d2b8eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BSHP&lt; VecPt3d &gt;</type>
      <name>m_pts</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>af3defed57aebcdbe8b58c86f718d435c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BSHP&lt; VecInt &gt;</type>
      <name>m_tris</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a823f8452db44719d7787a1bba0fce5fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BSHP&lt; VecFlt &gt;</type>
      <name>m_scalar</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a43a3d6e5037f1a441c9bbc254c6cb67e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>m_extrap</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a77a30e0d6dd654968ab1ff9c87044785</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_trunc</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a6a486b7adee474ef1bf20eb4e494817a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>m_truncMax</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a777236727c5375297f073804ce9da539</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>m_truncMin</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>ab46216b258cf56418244375df862b829</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_cloughTocher</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>ae33a253fc4514d80918726b397bcb12f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_natNeigh</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a1fd0cf5252afe3d4795fe7c8596916cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BSHP&lt; NodalFunc &gt;</type>
      <name>m_nodal</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a4a86948e6e0d052eb7cf71b6297aa9ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BSHP&lt; InterpCt &gt;</type>
      <name>m_ct</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>a8eb5d3ec92019fd28b1fe889fb244bb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BSHP&lt; InterpNatNeigh &gt;</type>
      <name>m_nn</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>adfa5e75e3a8ba28e1bf0074c0fccf910</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>VecInt</type>
      <name>m_extrapolationPointIndexes</name>
      <anchorfile>classxms_1_1_interp_linear_impl.html</anchorfile>
      <anchor>ab8cc560a805fa06fb7d49ae11132bc42</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>InterpLinearUnitTests</name>
    <filename>class_interp_linear_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testToString</name>
      <anchorfile>class_interp_linear_unit_tests.html</anchorfile>
      <anchor>a102ac6f3cccff6e6b84b07b04346f852</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testOneTriangle</name>
      <anchorfile>class_interp_linear_unit_tests.html</anchorfile>
      <anchor>af51ed7a3211df29fd89ef4db71e4638b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testInterp</name>
      <anchorfile>class_interp_linear_unit_tests.html</anchorfile>
      <anchor>adeee0cb0d05b95d458a08bcea9cc6480</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testCt</name>
      <anchorfile>class_interp_linear_unit_tests.html</anchorfile>
      <anchor>a9794c0623d2513c0d7fec09330ba057b</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::InterpNatNeigh</name>
    <filename>classxms_1_1_interp_nat_neigh.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~InterpNatNeigh</name>
      <anchorfile>classxms_1_1_interp_nat_neigh.html</anchorfile>
      <anchor>a0008ef8bc9164fc76d47172a925a3032</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static boost::shared_ptr&lt; InterpNatNeigh &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_interp_nat_neigh.html</anchorfile>
      <anchor>af70e19215c8fa50f09d09e0c5a5b63fa</anchor>
      <arglist>(const std::vector&lt; Pt3d &gt; &amp;a_pts, const std::vector&lt; int &gt; &amp;a_tris, const std::vector&lt; float &gt; &amp;a_scalar, GmTriSearch *a_triSearch)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>InterpNatNeigh</name>
      <anchorfile>classxms_1_1_interp_nat_neigh.html</anchorfile>
      <anchor>a72b529ca7b1f5d75181607ba062f4fba</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::InterpNatNeighImpl</name>
    <filename>classxms_1_1_interp_nat_neigh_impl.html</filename>
    <base>xms::InterpNatNeigh</base>
    <member kind="function">
      <type></type>
      <name>InterpNatNeighImpl</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a3d41e682d03b412b386dfa4ee73c1e39</anchor>
      <arglist>(const std::vector&lt; xms::Pt3d &gt; &amp;a_pts, const std::vector&lt; int &gt; &amp;a_tris, const std::vector&lt; float &gt; &amp;a_scalar, GmTriSearch *a_triSearch)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual float</type>
      <name>InterpToPt</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>abe776b9c426c2a81e149b77914b940c0</anchor>
      <arglist>(const xms::Pt3d &amp;a_pt) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>GetNeighbors</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a4a3119e9fba57aee3289facdbfc62f15</anchor>
      <arglist>(int a_ptIdx, std::vector&lt; int &gt; &amp;a_neigh) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetNodalFunc</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>ae5c52020e47377cd9e78878e92f1ee92</anchor>
      <arglist>(BSHP&lt; NodalFunc &gt; a_) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>RecalcNodalFunc</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a4a24e0c2e33a3e1049d7511c2bf8e974</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetBlendWeights</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a722cb830e57088a15fb8ff92c96d9c77</anchor>
      <arglist>(bool a_) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual std::string</type>
      <name>ToString</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a7ae42c38794abd0c03558e865b54e6e5</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>FillEdgeMap</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>aa1db768aa56eec91c68568f94cc81be9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>FillCenterVec</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a700fb9ea8c7fc39629974cc0d1821294</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>EdgesFromTri</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>aad6c40b2274ba9a16d9aeb78b3034eff</anchor>
      <arglist>(int a_triIdx, std::pair&lt; int, int &gt; a_edges[3])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>NeighTriFromTriIdx</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>ae66dd4911135580befe82729782efa69</anchor>
      <arglist>(int a_triIdx, std::vector&lt; int &gt; &amp;a_tris)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GetNatNeighTriangles</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>ac46f2c036f82ef26ddfd4c99ae2b8d9d</anchor>
      <arglist>(const xms::Pt3d &amp;a_pt, std::vector&lt; int &gt; &amp;a_tris)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>BlendFunc</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>affbd9a0395c7a13efda1f5371cfd586e</anchor>
      <arglist>(double a_x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>ScalarFromNodalFunc</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>aebffde9d3540011eedde61cfbdb4ba5c</anchor>
      <arglist>(int a_ptIdx, const xms::Pt3d &amp;a_loc)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>HaleNnInterp</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a3e849d6d8a92cbf2f072d36e30b81e19</anchor>
      <arglist>(const xms::Pt3d &amp;a_pt)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HaleNnVisitNeighbors</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>abb51645e8c36e354d63075aa87aba47d</anchor>
      <arglist>(int a_tIdx, const xms::Pt3d &amp;a_pt, std::vector&lt; nnOuterEdgeStruct &gt; &amp;a_outerEdges, std::map&lt; int, double &gt; &amp;a_weights)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>AdjacentTriangle</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a13915edcf34f12baff5fbaa1b2f50a38</anchor>
      <arglist>(std::pair&lt; int, int &gt; &amp;a_edge, int a_triIdx)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>PtInTriCircumCircle</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a72433c742e06d389c64f0f942927ec27</anchor>
      <arglist>(const xms::Pt3d &amp;a_pt, int a_tri)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HaleNnAddOuterEdge</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a6db34b8c69e7eb48db1a8f86a7c922d6</anchor>
      <arglist>(std::vector&lt; nnOuterEdgeStruct &gt; &amp;a_, int a_tIdx, int a_ptIdx0, int a_ptIdx1, const xms::Pt3d &amp;a_pt)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HaleNnSortOuterEdges</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>aa6d419c410a6f1528ff62fd00bb71709</anchor>
      <arglist>(std::vector&lt; nnOuterEdgeStruct &gt; &amp;a_)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HaleNnOuterEdgesToWeights</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>aecf1952fc3d6a365f1195da8850229b2</anchor>
      <arglist>(std::vector&lt; nnOuterEdgeStruct &gt; &amp;a_outerEdges, std::map&lt; int, double &gt; &amp;a_weights)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>NormalizeWeights</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a0ed2f7b5f27004480482f5a3e249c4b9</anchor>
      <arglist>(std::map&lt; int, double &gt; &amp;a_weights)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>BlendWeights</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>ab39f57fb320a9b51460a2b01a92dd912</anchor>
      <arglist>(std::map&lt; int, double &gt; &amp;a_weights)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>ScalarFromWeights</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a91d250cdc4296e924f6b80f84d4e2068</anchor>
      <arglist>(std::map&lt; int, double &gt; &amp;a_weights, const xms::Pt3d &amp;a_pt)</arglist>
    </member>
    <member kind="typedef" protection="private">
      <type>boost::unordered_map&lt; std::pair&lt; int, int &gt;, std::pair&lt; int, int &gt; &gt;</type>
      <name>MapEdges</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a1e9f040080051c5ff390579d0b127308</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>const std::vector&lt; xms::Pt3d &gt; &amp;</type>
      <name>m_pts</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>add2e2d2dc003f234c0ef842bbb693480</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>const std::vector&lt; int &gt; &amp;</type>
      <name>m_tris</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>ac30d548362e775112455e38aa6d66143</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>const std::vector&lt; float &gt; &amp;</type>
      <name>m_scalar</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>adc81d178dab197a91fe89a6ebb67d953</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>m_tol</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a45862983f5e78e60a70b183e279c683e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>MapEdges</type>
      <name>m_edges</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>ac038753bedc0f9b8e39476a6dbd53159</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::vector&lt; xms::Pt3d &gt;</type>
      <name>m_centers</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>af5803c144d15b8a6cf552a20aee06125</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; NodalFunc &gt;</type>
      <name>m_nf</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a61256a944d364b4896d35d5fceedfd2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>GmTriSearch *</type>
      <name>m_triSearch</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>a7f6e55c27170b28f2e927bbe95a71764</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>m_blendWeights</name>
      <anchorfile>classxms_1_1_interp_nat_neigh_impl.html</anchorfile>
      <anchor>aafbfbfc22a279d45f09db4010c0aa13d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>InterpNatNeighUnitTests</name>
    <filename>class_interp_nat_neigh_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testCreateClass</name>
      <anchorfile>class_interp_nat_neigh_unit_tests.html</anchorfile>
      <anchor>a250cc7b0868d886bfe62a879f2751dc9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testGetTris</name>
      <anchorfile>class_interp_nat_neigh_unit_tests.html</anchorfile>
      <anchor>a19ff24a728bfbe73adb5e3a41a6662cf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testGetNeighbors</name>
      <anchorfile>class_interp_nat_neigh_unit_tests.html</anchorfile>
      <anchor>a891daf9245fc4102170df8230450f724</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testHaleNnSortOuterEdges</name>
      <anchorfile>class_interp_nat_neigh_unit_tests.html</anchorfile>
      <anchor>afe6136a18245213de34dc1e0a8667eda</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testHaleNnInterp</name>
      <anchorfile>class_interp_nat_neigh_unit_tests.html</anchorfile>
      <anchor>ad581e74eade4d9e7407824071228fb30</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>xms::InterpPtInfo</name>
    <filename>structxms_1_1_interp_pt_info.html</filename>
    <member kind="variable">
      <type>Pt3d</type>
      <name>m_loc</name>
      <anchorfile>structxms_1_1_interp_pt_info.html</anchorfile>
      <anchor>a975cbe4662b02d73509546283b7f06f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>m_scalar</name>
      <anchorfile>structxms_1_1_interp_pt_info.html</anchorfile>
      <anchor>a4d2d67d8785d196c6444261c965e83de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>m_weight</name>
      <anchorfile>structxms_1_1_interp_pt_info.html</anchorfile>
      <anchor>a98786093d872060474647f82be4d90eb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::InterpIdwImpl::InterpThread</name>
    <filename>classxms_1_1_interp_idw_impl_1_1_interp_thread.html</filename>
    <base>xms::ThreadLoop</base>
    <member kind="function">
      <type></type>
      <name>InterpThread</name>
      <anchorfile>classxms_1_1_interp_idw_impl_1_1_interp_thread.html</anchorfile>
      <anchor>aad5946619307257a6f66fe8d89d8c115</anchor>
      <arglist>(InterpIdwImpl &amp;a_, VecFlt &amp;a_s, const VecPt3d &amp;a_p)</arglist>
    </member>
    <member kind="function">
      <type>BSHP&lt; ThreadLoop &gt;</type>
      <name>CreateForNewThread</name>
      <anchorfile>classxms_1_1_interp_idw_impl_1_1_interp_thread.html</anchorfile>
      <anchor>ac39d2c4e619bf951402d98d204503446</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="variable">
      <type>InterpIdwImpl &amp;</type>
      <name>m_interp</name>
      <anchorfile>classxms_1_1_interp_idw_impl_1_1_interp_thread.html</anchorfile>
      <anchor>a492a852f3c52ebaac0642a0bd77f8bc7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VecFlt &amp;</type>
      <name>m_scalarTo</name>
      <anchorfile>classxms_1_1_interp_idw_impl_1_1_interp_thread.html</anchorfile>
      <anchor>a67f759105b3e65aaf54abfdef2f9a8a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const VecPt3d &amp;</type>
      <name>m_pts</name>
      <anchorfile>classxms_1_1_interp_idw_impl_1_1_interp_thread.html</anchorfile>
      <anchor>a26bb5b1a02b05e20ca148e1da7a3b109</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>Worker</name>
      <anchorfile>classxms_1_1_interp_idw_impl_1_1_interp_thread.html</anchorfile>
      <anchor>a7f58cfe04f020eac04cac0f9a684be89</anchor>
      <arglist>() override</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::ix</name>
    <filename>classxms_1_1ix.html</filename>
    <member kind="function">
      <type></type>
      <name>ix</name>
      <anchorfile>classxms_1_1ix.html</anchorfile>
      <anchor>ab86596c140cee4b9ad0bc1e0865c3c2a</anchor>
      <arglist>(Pt3d a_pt, int a_i, double a_t)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classxms_1_1ix.html</anchorfile>
      <anchor>ad7ebc7d3dfc72319be03ff49294aae00</anchor>
      <arglist>(const ix &amp;rhs)</arglist>
    </member>
    <member kind="variable">
      <type>Pt3d</type>
      <name>m_pt</name>
      <anchorfile>classxms_1_1ix.html</anchorfile>
      <anchor>a6a4aaf5674e38b0b1ff89f00afc9c32e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>m_i</name>
      <anchorfile>classxms_1_1ix.html</anchorfile>
      <anchor>a4185fe410fd0c5b45dc440483e745b15</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>m_t</name>
      <anchorfile>classxms_1_1ix.html</anchorfile>
      <anchor>ab40238d7edca655589518e4688d1aa49</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>xms::LineParameters</name>
    <filename>structxms_1_1_line_parameters.html</filename>
    <member kind="function">
      <type></type>
      <name>LineParameters</name>
      <anchorfile>structxms_1_1_line_parameters.html</anchorfile>
      <anchor>a7e915686586e522bed5f9f964e72a6cb</anchor>
      <arglist>(double a_slope, double a_intercept)</arglist>
    </member>
    <member kind="function">
      <type>LineParameters</type>
      <name>NormalThrough</name>
      <anchorfile>structxms_1_1_line_parameters.html</anchorfile>
      <anchor>a45f24450c943080ec56602b443158d4e</anchor>
      <arglist>(const Pt3d &amp;a_p) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Intersection</name>
      <anchorfile>structxms_1_1_line_parameters.html</anchorfile>
      <anchor>ab25d0448ce5f80ef8bed1426f45ee8d6</anchor>
      <arglist>(const LineParameters &amp;a_other, Pt3d &amp;a_p) const</arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>m_slope</name>
      <anchorfile>structxms_1_1_line_parameters.html</anchorfile>
      <anchor>a9864a83271e3d87978d9f09e8cd15d57</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>m_intercept</name>
      <anchorfile>structxms_1_1_line_parameters.html</anchorfile>
      <anchor>a5281ad9802bfec2292dcba95d1735167</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>xms::ThreadMgrImpl::myThread</name>
    <filename>structxms_1_1_thread_mgr_impl_1_1my_thread.html</filename>
    <member kind="function">
      <type></type>
      <name>myThread</name>
      <anchorfile>structxms_1_1_thread_mgr_impl_1_1my_thread.html</anchorfile>
      <anchor>a2e0b89c1fae0e51dadceb45c3df8a6b9</anchor>
      <arglist>(BSHP&lt; ThreadLoop &gt; a_)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>operator()</name>
      <anchorfile>structxms_1_1_thread_mgr_impl_1_1my_thread.html</anchorfile>
      <anchor>a3e1888f5f377a0724358131da243dcd4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; ThreadLoop &gt;</type>
      <name>m_</name>
      <anchorfile>structxms_1_1_thread_mgr_impl_1_1my_thread.html</anchorfile>
      <anchor>a3059d4865c38dd28f5c4b858c0e09c9a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::NodalFuncImpl::NfThread</name>
    <filename>classxms_1_1_nodal_func_impl_1_1_nf_thread.html</filename>
    <base>xms::ThreadLoop</base>
    <member kind="function">
      <type></type>
      <name>NfThread</name>
      <anchorfile>classxms_1_1_nodal_func_impl_1_1_nf_thread.html</anchorfile>
      <anchor>a0ad689fa7a423908f8cc245674721e7e</anchor>
      <arglist>(NodalFuncImpl *a_nf, BSHP&lt; GmPtSearch &gt; a_ptSearch)</arglist>
    </member>
    <member kind="function">
      <type>BSHP&lt; ThreadLoop &gt;</type>
      <name>CreateForNewThread</name>
      <anchorfile>classxms_1_1_nodal_func_impl_1_1_nf_thread.html</anchorfile>
      <anchor>a48e40653b8aff223723d2fdea0031e4a</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="variable">
      <type>NodalFuncImpl *</type>
      <name>m_nf</name>
      <anchorfile>classxms_1_1_nodal_func_impl_1_1_nf_thread.html</anchorfile>
      <anchor>aaf4e4077095b15cc51f02f37e598c79b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; GmPtSearch &gt;</type>
      <name>m_ptSearch</name>
      <anchorfile>classxms_1_1_nodal_func_impl_1_1_nf_thread.html</anchorfile>
      <anchor>ac3828ba7770e0a562d8c5a94bbcb2d26</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VecInt</type>
      <name>m_nearestPts</name>
      <anchorfile>classxms_1_1_nodal_func_impl_1_1_nf_thread.html</anchorfile>
      <anchor>a6a1e51f110d431e9eaac23e356bbfc9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; InterpPtInfo &gt;</type>
      <name>m_matrixPts</name>
      <anchorfile>classxms_1_1_nodal_func_impl_1_1_nf_thread.html</anchorfile>
      <anchor>ad3dd39caa688417eeee54aadf9e1422b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VecDbl</type>
      <name>m_d2</name>
      <anchorfile>classxms_1_1_nodal_func_impl_1_1_nf_thread.html</anchorfile>
      <anchor>a93e6f4289084da50c07004180daf8006</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VecDbl</type>
      <name>m_w</name>
      <anchorfile>classxms_1_1_nodal_func_impl_1_1_nf_thread.html</anchorfile>
      <anchor>adbfce94f57bd501ef4332558777c1e4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>Worker</name>
      <anchorfile>classxms_1_1_nodal_func_impl_1_1_nf_thread.html</anchorfile>
      <anchor>a2135ef69d46980306ea28267f2bacdfa</anchor>
      <arglist>() override</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>xms::nnOuterEdgeStruct</name>
    <filename>structxms_1_1nn_outer_edge_struct.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>m_triIdx</name>
      <anchorfile>structxms_1_1nn_outer_edge_struct.html</anchorfile>
      <anchor>a3d5179ff3a6017913fd62f8349bf9dae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>m_edge</name>
      <anchorfile>structxms_1_1nn_outer_edge_struct.html</anchorfile>
      <anchor>a3442f650122e9cce8b564ac9ac890996</anchor>
      <arglist>[2]</arglist>
    </member>
    <member kind="variable">
      <type>xms::Pt3d</type>
      <name>m_cc</name>
      <anchorfile>structxms_1_1nn_outer_edge_struct.html</anchorfile>
      <anchor>ae243edefe043641b6bbe7ec0ca8b56bc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::NodalFunc</name>
    <filename>classxms_1_1_nodal_func.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NodalFunc</name>
      <anchorfile>classxms_1_1_nodal_func.html</anchorfile>
      <anchor>a4ba5c3cfd1fca52345242fff0edca824</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static BSHP&lt; NodalFunc &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_nodal_func.html</anchorfile>
      <anchor>a5c1c96d0322f3905f7e65f9eb88483f1</anchor>
      <arglist>(int a_type, bool a_2d, boost::shared_ptr&lt; GmPtSearch &gt; a_ptSearch, const std::vector&lt; Pt3d &gt; &amp;a_pts, const std::vector&lt; float &gt; &amp;a_scalar, int a_nNearest, bool a_quad_oct, double a_power, bool a_modifiedShepardWeights, boost::shared_ptr&lt; Observer &gt; a_p, InterpNatNeigh *a_natNeigh)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>NodalFunc</name>
      <anchorfile>classxms_1_1_nodal_func.html</anchorfile>
      <anchor>aa406b56b931d0965e7f5062123e6f41b</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::NodalFuncImpl</name>
    <filename>classxms_1_1_nodal_func_impl.html</filename>
    <base>xms::NodalFunc</base>
    <class kind="class">xms::NodalFuncImpl::NfThread</class>
    <member kind="function">
      <type></type>
      <name>NodalFuncImpl</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>ae2f80821ad38874764378301c647d133</anchor>
      <arglist>(const std::vector&lt; Pt3d &gt; &amp;a_pts, const std::vector&lt; float &gt; &amp;a_scalar)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NodalFuncImpl</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>af57d9fc660672342f749b619f35c1125</anchor>
      <arglist>(int a_type, bool a_2d, BSHP&lt; GmPtSearch &gt; a_ptSearch, const std::vector&lt; Pt3d &gt; &amp;a_pts, const std::vector&lt; float &gt; &amp;a_scalar, int a_nNearest, bool a_quad_oct, double a_power, bool a_modifiedShepardWeights, BSHP&lt; Observer &gt; a_p, InterpNatNeigh *a_natNeigh)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual double</type>
      <name>ScalarFromPtIdx</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a912b59daf958deda11c01087d266ba3c</anchor>
      <arglist>(int a_ptIdx, const Pt3d &amp;a_loc) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>GradientFromPtIdx</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a9a544aa910ac1dd607f644452f88b340</anchor>
      <arglist>(int a_ptIdx, Pt3d &amp;a_grad) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>ComputeNodalFuncs</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a895415072c66edf9e0c4dfa3073c2668</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual std::string</type>
      <name>ToString</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a1855ac8e5977ff2558c3698c026bba76</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>GetType</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>ae093d72ba657eeb6255652f867edcc48</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>GetNearestPointsOption</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a2a1ea07c742d21f31bc9dd3dc72b26c5</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>GetNumNearestPoints</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a7cdb2bf0f8a4cc76cf6c8d1579b8cd66</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>GetUseModifiedShepardWeights</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a9c2a699704a0561479a1f2b346349101</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>GetUseQuadrantSearch</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a4532088b9206c487a189f549926d4865</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>NfForPt</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a7874db0e02ba0a2047c8f98989953c53</anchor>
      <arglist>(int a_ptIdx, BSHP&lt; GmPtSearch &gt; a_s, std::vector&lt; int &gt; &amp;nearestPts, std::vector&lt; InterpPtInfo &gt; &amp;matrixPts, std::vector&lt; double &gt; &amp;d2, std::vector&lt; double &gt; &amp;w)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>NodalFuncForPtFromMatrixPts</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a926e856467accafb0788e5832eab4ca3</anchor>
      <arglist>(int a_ptIdx, const std::vector&lt; InterpPtInfo &gt; &amp;a_pts)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>NodalFunc2d</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a5bed8882884fcc613d1c7f8503641dbe</anchor>
      <arglist>(int a_ptIdx, const std::vector&lt; InterpPtInfo &gt; &amp;a_pts, double *a_A)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>NodalFunc3d</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>ad5bca559accbb7fab5f9f35bd86e0d22</anchor>
      <arglist>(int a_ptIdx, const std::vector&lt; InterpPtInfo &gt; &amp;a_pts, double *a_A)</arglist>
    </member>
    <member kind="function">
      <type>Pt3d</type>
      <name>ComputeGradientForPoint</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>ae7bfeeb352a366f5155fcd95cbd66fad</anchor>
      <arglist>(int a_ptIdx, const std::vector&lt; InterpPtInfo &gt; &amp;a_pts)</arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_2d</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>adefb0dbcd8b07abb3203943ba969a771</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>m_type</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a821df3becc5f9fabdfa42e84edf9bea9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>m_nNearest</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a8383d483c888ff2cfe5b763e9e00c366</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_quadOct</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>acf8b255a2dbf430d7017674024970155</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const std::vector&lt; Pt3d &gt; &amp;</type>
      <name>m_pts</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>aedca4d19a36b52bd68d1f026fa5d0980</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const std::vector&lt; float &gt; &amp;</type>
      <name>m_scalar</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a8c6a234116c62f8a5227199a7bff83d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; GmPtSearch &gt;</type>
      <name>m_ptSearch</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a80133a89c3f9375edfe3fcc058719d9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; Pt3d &gt;</type>
      <name>m_gradient</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>aa76232c34d0e9ebb9d16ee146f759a02</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VecDbl2d</type>
      <name>m_quadratic</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>ababfd07ebe7d4987e36fdeefd08c6145</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>m_power</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a60204ebaab033e0d89a1ce1e518bf08c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_modifiedShepardWeights</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a480337950c43a1445630fc6cc7918506</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; int &gt;</type>
      <name>m_nearestAll</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a9b048afa38d47724284946fc75362d34</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; Observer &gt;</type>
      <name>m_prog</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a83f4f8791fcecae4e7b7fe7f724cdef6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_errorReport</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a63cde125aef43a7f2f889d13b0504601</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>m_debugTest</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>ae67f53de253c8e003e025da872249dca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>InterpNatNeigh *</type>
      <name>m_natNeigh</name>
      <anchorfile>classxms_1_1_nodal_func_impl.html</anchorfile>
      <anchor>a0b2d06450b244c3c2368a62d015f8e5f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NodalFuncUnitTests</name>
    <filename>class_nodal_func_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testCreateClass</name>
      <anchorfile>class_nodal_func_unit_tests.html</anchorfile>
      <anchor>a986761f15bb7b983843743da333736eb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testComputeGradientForPoint</name>
      <anchorfile>class_nodal_func_unit_tests.html</anchorfile>
      <anchor>af68cd137ce30435b72961072fa464b6c</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>PtSearchUnitTests</name>
    <filename>class_pt_search_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testCreateClass</name>
      <anchorfile>class_pt_search_unit_tests.html</anchorfile>
      <anchor>a8e322359ace1af03b7ed680f5295693b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testTest2d</name>
      <anchorfile>class_pt_search_unit_tests.html</anchorfile>
      <anchor>a73d2a85aa0fbc316a421dcf912981fa8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testTest2dTutData</name>
      <anchorfile>class_pt_search_unit_tests.html</anchorfile>
      <anchor>add78e0a8851b7ac7f7a2410a1b1fa5b1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testTest3d</name>
      <anchorfile>class_pt_search_unit_tests.html</anchorfile>
      <anchor>a7f493292989b1e6e6c5d01516d5b2d64</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testTest3dOct</name>
      <anchorfile>class_pt_search_unit_tests.html</anchorfile>
      <anchor>a5d78214e1fef2096a0c61d36399c7584</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testActivity2d</name>
      <anchorfile>class_pt_search_unit_tests.html</anchorfile>
      <anchor>a9c68362b8c6522297a6b4f987428fb74</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testActivity3d</name>
      <anchorfile>class_pt_search_unit_tests.html</anchorfile>
      <anchor>a5070b8bb3b9419bbd813a56e42899455</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testPtsWithinDist</name>
      <anchorfile>class_pt_search_unit_tests.html</anchorfile>
      <anchor>a24c64ac92355da4e5384c20b9279acc3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testVectorThatGrows</name>
      <anchorfile>class_pt_search_unit_tests.html</anchorfile>
      <anchor>ac74456c2b966531869f579c2a7875740</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>xms::SNResult</name>
    <filename>structxms_1_1_s_n_result.html</filename>
    <member kind="function">
      <type></type>
      <name>SNResult</name>
      <anchorfile>structxms_1_1_s_n_result.html</anchorfile>
      <anchor>ac995b76bc0bed4171b89a2e158dc289e</anchor>
      <arglist>(size_t a_index, double a_param, double a_cross, const Pt3d &amp;a_position)</arglist>
    </member>
    <member kind="variable">
      <type>Pt3d</type>
      <name>m_position</name>
      <anchorfile>structxms_1_1_s_n_result.html</anchorfile>
      <anchor>a5ad5bfedd78d0686db2718907d1568d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>m_param</name>
      <anchorfile>structxms_1_1_s_n_result.html</anchorfile>
      <anchor>acf9f91cebed6f3f5f60ec8998688cedb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>m_cross</name>
      <anchorfile>structxms_1_1_s_n_result.html</anchorfile>
      <anchor>ad5af71be9de7cff2196047e0b566d7cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>m_index</name>
      <anchorfile>structxms_1_1_s_n_result.html</anchorfile>
      <anchor>ad154a7139ad6738b9a2ec7a72a437eab</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::ThreadLoop</name>
    <filename>classxms_1_1_thread_loop.html</filename>
    <class kind="class">xms::ThreadLoop::impl</class>
    <member kind="function">
      <type>void</type>
      <name>SetStartNumIterCnt</name>
      <anchorfile>classxms_1_1_thread_loop.html</anchorfile>
      <anchor>a2ce8e0b2789803849f6b93312aeb4eaf</anchor>
      <arglist>(int a_startIdx, int a_numIter, int *a_cntPtr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DoWork</name>
      <anchorfile>classxms_1_1_thread_loop.html</anchorfile>
      <anchor>a85da861fe57a764ce110a1f5abf395ce</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual BSHP&lt; ThreadLoop &gt;</type>
      <name>CreateForNewThread</name>
      <anchorfile>classxms_1_1_thread_loop.html</anchorfile>
      <anchor>abf5ce6acd9882022efc6f41923e367e8</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>int</type>
      <name>CurrIdx</name>
      <anchorfile>classxms_1_1_thread_loop.html</anchorfile>
      <anchor>afebcb169439af245b14329587e6768f1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" virtualness="pure">
      <type>virtual void</type>
      <name>Worker</name>
      <anchorfile>classxms_1_1_thread_loop.html</anchorfile>
      <anchor>a75b531bc79726b856f67c8cc11385543</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>XM_DISALLOW_COPY_AND_ASSIGN</name>
      <anchorfile>classxms_1_1_thread_loop.html</anchorfile>
      <anchor>a78ca96fb2d551350de931e7de3a3aa6b</anchor>
      <arglist>(ThreadLoop)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; impl &gt;</type>
      <name>m_p</name>
      <anchorfile>classxms_1_1_thread_loop.html</anchorfile>
      <anchor>af5d2d1bbae67f9daa85d7027f2de1d80</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::ThreadMgr</name>
    <filename>classxms_1_1_thread_mgr.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>SetThreadLoopClass</name>
      <anchorfile>classxms_1_1_thread_mgr.html</anchorfile>
      <anchor>a16f72cdde8894c836771e7da292a5da0</anchor>
      <arglist>(BSHP&lt; ThreadLoop &gt; a_)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>ExplicitlySetNumThreads</name>
      <anchorfile>classxms_1_1_thread_mgr.html</anchorfile>
      <anchor>a2621c25b999c657c64082161eb36931e</anchor>
      <arglist>(int a_nThreads)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>RunThreads</name>
      <anchorfile>classxms_1_1_thread_mgr.html</anchorfile>
      <anchor>a6ce6f8ca944cc8ed7944b276e1ccd34d</anchor>
      <arglist>(int a_nIter)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>SetObserver</name>
      <anchorfile>classxms_1_1_thread_mgr.html</anchorfile>
      <anchor>ad523516e40e779ea4f58ef4432d5ef64</anchor>
      <arglist>(BSHP&lt; Observer &gt; a_prog)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual std::vector&lt; BSHP&lt; ThreadLoop &gt; &gt;</type>
      <name>Threads</name>
      <anchorfile>classxms_1_1_thread_mgr.html</anchorfile>
      <anchor>a925946129f98cb3d2cd63797e22b30dc</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static BSHP&lt; ThreadMgr &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_thread_mgr.html</anchorfile>
      <anchor>af20f5ccc34e6e699a6ffc63bd683470d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>ThreadMgr</name>
      <anchorfile>classxms_1_1_thread_mgr.html</anchorfile>
      <anchor>a9b3e8c11763fd8fff8b0ad659e608457</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>XM_DISALLOW_COPY_AND_ASSIGN</name>
      <anchorfile>classxms_1_1_thread_mgr.html</anchorfile>
      <anchor>ab391ca89f70260edff7883f785239673</anchor>
      <arglist>(ThreadMgr)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::ThreadMgrImpl</name>
    <filename>classxms_1_1_thread_mgr_impl.html</filename>
    <base>xms::ThreadMgr</base>
    <class kind="struct">xms::ThreadMgrImpl::myThread</class>
    <member kind="function">
      <type></type>
      <name>ThreadMgrImpl</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a44a610f1cff66a35a1fc14f836942ffc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetThreadLoopClass</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a21af9b43c5397c043e93d76b27808364</anchor>
      <arglist>(BSHP&lt; ThreadLoop &gt; a_) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ExplicitlySetNumThreads</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a51734bab808fe6e5d8d3f69466e40a13</anchor>
      <arglist>(int a_nThreads) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RunThreads</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a2ce51491b83dc2225dd205cff2e7cdbe</anchor>
      <arglist>(int a_nIter) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetObserver</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>adb747e7ebd68cf2f2a143de15d75a969</anchor>
      <arglist>(BSHP&lt; Observer &gt; a_prog) override</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; BSHP&lt; ThreadLoop &gt; &gt;</type>
      <name>Threads</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a242f47dba9999a759701bdbb81b47f17</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetupThreads</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a644e8117ad62493b7f0f2970e9024d86</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>MonitorThreads</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>afb22f2e593b8bf696f24df957281fd76</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>FinishThreads</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a0697ffced10a2454deb4c37a2a858974</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; ThreadLoop &gt;</type>
      <name>m_threader</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a37ff4e07fd978b9bae05bca828daf31e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BSHP&lt; Observer &gt;</type>
      <name>m_prog</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a0cd7355aa09fde767f79afb17994f041</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>m_nThread</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a2214c95c674e392ad112006f208c7f01</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>m_nIter</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>af07e3fe8c5c2a3f0060b60d493c274a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VecInt</type>
      <name>m_beg</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>ac442b3638d9f2842da1905e2b014e966</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VecInt</type>
      <name>m_thrdNumIter</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a63d634ef172e6702c328c18e9cb223b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>VecInt</type>
      <name>m_counts</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a5cd96dbeb73212fea8ff96832279ac80</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; myThread &gt;</type>
      <name>m_vMyThread</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>a14b40af408bb79b1d6f4eaf12702fb2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; boost::thread &gt;</type>
      <name>m_vThreads</name>
      <anchorfile>classxms_1_1_thread_mgr_impl.html</anchorfile>
      <anchor>ad85e6817fe4a86146c05df7390913e42</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::TrAutoFixFourTrianglePts</name>
    <filename>classxms_1_1_tr_auto_fix_four_triangle_pts.html</filename>
    <member kind="function" static="yes">
      <type>static BSHP&lt; TrAutoFixFourTrianglePts &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts.html</anchorfile>
      <anchor>a58773a2774574706c01b09c1d1ac3048</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::TrAutoFixFourTrianglePtsImpl</name>
    <filename>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</filename>
    <base>xms::TrAutoFixFourTrianglePts</base>
    <member kind="function">
      <type></type>
      <name>TrAutoFixFourTrianglePtsImpl</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a0f4600beeb16d689a3e0b32230506cff</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>Fix</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>af147444a1e2e287a72e382f30692cf0c</anchor>
      <arglist>(BSHP&lt; TrTin &gt; a_tin) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetObserver</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a9e38e41ebbd28ca092257ea1809ea731</anchor>
      <arglist>(BSHP&lt; Observer &gt; a_) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetUndeleteablePtIdxs</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a0c5da7045cd27850ac698f3f06f48379</anchor>
      <arglist>(VecInt &amp;a_ptIdx) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>FillAdjTrisIfNeeded</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a7bafc03b8a5a97ba3f8fea1ce5238aa8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GetPtsAndTrisToDelete</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a295a366f04639d09928b3d8295fd8e2c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>std::pair&lt; int, int &gt;</type>
      <name>EdgeOppositePt</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a68551fa49d119b1e7d6ed1a47b69ca9c</anchor>
      <arglist>(int a_ptIdx, int a_tri, VecInt &amp;a_tris)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>MakeTwoNewTriangles</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a98dd5e7d4469d59171b8497e99463d25</anchor>
      <arglist>(VecPt3d &amp;a_pts, std::map&lt; int, int &gt; a_edges, int a_tris[2][3])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ReplaceTriangle</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a4ddad45d4285e669efb1e7b5881ad957</anchor>
      <arglist>(int &amp;a_tIdx, int a_newTri[3], VecInt &amp;a_tris)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RenumberPts</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>aae03f8c6f46aea41f01595b0e83d565a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RenumberTris</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a1c685b02542d9985b962f6dfc04be6c5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RemovePts</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a8c1abb56f3123fc5a41e7ac155e4a886</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>RemoveTris</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a94350c394e0cab1a5491210257567dff</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ClearAdjTrisIfCreated</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a359ac2134d3f2bc3b4c0c551f1cd27d7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; TrTin &gt;</type>
      <name>m_tin</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a6df5fba0dfca93533acc9febafc732b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; Observer &gt;</type>
      <name>m_observer</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>af0f22a3314f8a951657a112e3dc61a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>m_removeAdjTris</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a28c1a4f2b4c06c2d3e5915499fbce30a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>SetInt</type>
      <name>m_ptsToDelete</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>aecab7c30b6be74785e7708750940ea81</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>SetInt</type>
      <name>m_trisToDelete</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>a8decce1f1994328ae11f20eb08efe780</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>SetInt</type>
      <name>m_noDelete</name>
      <anchorfile>classxms_1_1_tr_auto_fix_four_triangle_pts_impl.html</anchorfile>
      <anchor>ab1b475420cf47491c7600270b02dbee5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TrAutoFixFourTrianglePtsUnitTests</name>
    <filename>class_tr_auto_fix_four_triangle_pts_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>test1</name>
      <anchorfile>class_tr_auto_fix_four_triangle_pts_unit_tests.html</anchorfile>
      <anchor>ab21bd9cb12084ff3dc1eb734f2626081</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::TrBreaklineAdder</name>
    <filename>classxms_1_1_tr_breakline_adder.html</filename>
    <member kind="function" static="yes">
      <type>static boost::shared_ptr&lt; TrBreaklineAdder &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_tr_breakline_adder.html</anchorfile>
      <anchor>abdc62fc265ddc89580ad12885076c9c9</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::TrBreaklineAdderImpl</name>
    <filename>classxms_1_1_tr_breakline_adder_impl.html</filename>
    <base>xms::TrBreaklineAdder</base>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetObserver</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a1efc7eeb292ed74fb20c505f8c63245c</anchor>
      <arglist>(BSHP&lt; Observer &gt; a_) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetTin</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a14da3950a8bad04e99e01118ff098c67</anchor>
      <arglist>(BSHP&lt; TrTin &gt; a_tin, double a_tol=-1) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>AddBreakline</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a7190431ec5a664f390974a1b9e6a7e16</anchor>
      <arglist>(const VecInt &amp;a_breakline) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>AddBreaklines</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>afdc00ed49ed8eebd1d0d40085158ee8a</anchor>
      <arglist>(const VecInt2d &amp;a_breakline) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual std::string</type>
      <name>ErrorMessage</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>aa4036d3ba5e6ab3e3b9eba71cae6dbc3</anchor>
      <arglist>(int) const override</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>GetExtents</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a330e8badc1b3f6f14c95c07d66ba709c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>ComputeTolerance</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>ae71390adafd684e944c4e5ba132eb1a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>CrossesBoundary</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a239990bf3ccb0b3039d1e02ce7fb10f6</anchor>
      <arglist>(int a_blpt1, int a_blpt2)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>ProcessSegmentBySwapping</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a741b37559be486816ba50cf36ea75b7e</anchor>
      <arglist>(int a_vtx1, int a_vtx2)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>GetIntersectingEdges</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>acb8d69c9277b12db44ff851377264652</anchor>
      <arglist>(int a_blpt1, int a_blpt2, VecEdge &amp;a_edges)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>FindIntersectingEdgeFromPoint</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a3cb98a64932a06a92665236dceb7b1fe</anchor>
      <arglist>(int a_blpt1, int a_blpt2, int *a_intpt1, int *a_intpt2, double *a_x, double *a_y, double *a_z1, double *a_z2)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>FindIntersectingEdgeFromEdge</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a25e0d6e4454a500d1720681e023aecb3</anchor>
      <arglist>(int a_ept1, int a_ept2, int a_blpt1, int a_blpt2, int *a_intpt1, int *a_intpt2, double *a_x, double *a_y, double *a_z1, double *a_z2)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; TrTin &gt;</type>
      <name>m_tin</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a3e1ef5566a50db9c950ad3f25f87b7eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; Observer &gt;</type>
      <name>m_observer</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a4d101ef2d3e4e463803db9f05373ab13</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>m_xyTol</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a78e61b3d04bb68dc283c2b4170270e11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>VecInt *</type>
      <name>m_tris</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a4832c9d590ee0eade459297b359e4636</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>VecPt3d *</type>
      <name>m_pts</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a0f1ea456ca21a815d85be9befa601b84</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>VecInt2d *</type>
      <name>m_trisAdjToPts</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a6255379baf587e613b1d8a4f25f9f690</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Pt3d</type>
      <name>m_mn</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a75db23f8349bd1002d3981d2786e4cad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Pt3d</type>
      <name>m_mx</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>aafff781bcf0960594d71e1ef7acad578</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; GmMultiPolyIntersector &gt;</type>
      <name>m_multiPolyIntersector</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>a73d62e228718e67c814d3669eef1cf27</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; GmPtSearch &gt;</type>
      <name>m_searcher</name>
      <anchorfile>classxms_1_1_tr_breakline_adder_impl.html</anchorfile>
      <anchor>ae928c362b7672a133aee9b3babf50637</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TrBreaklineAdderUnitTests</name>
    <filename>class_tr_breakline_adder_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>test1</name>
      <anchorfile>class_tr_breakline_adder_unit_tests.html</anchorfile>
      <anchor>aed5fc7ad1065440d05eb8e4387a19d6a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test2</name>
      <anchorfile>class_tr_breakline_adder_unit_tests.html</anchorfile>
      <anchor>a29102789708793c51d7204d464c31e34</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testCrossingBoundary</name>
      <anchorfile>class_tr_breakline_adder_unit_tests.html</anchorfile>
      <anchor>af46bc719d5a27acd4a02f4f6ba2ea27d</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TriSearchUnitTests</name>
    <filename>class_tri_search_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>testCreateClass</name>
      <anchorfile>class_tri_search_unit_tests.html</anchorfile>
      <anchor>ab3d5e1da43f73a8071876dc0516fead6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testInterpWeights</name>
      <anchorfile>class_tri_search_unit_tests.html</anchorfile>
      <anchor>a1364caa71954c0da28fe0be43b104c02</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testPtActivity</name>
      <anchorfile>class_tri_search_unit_tests.html</anchorfile>
      <anchor>ad3db5f8baf65200980ce9ce64cf4e076</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testTriActivity</name>
      <anchorfile>class_tri_search_unit_tests.html</anchorfile>
      <anchor>aaafe145f2ebb470f3604248b2a26c3fd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testSmsCase1</name>
      <anchorfile>class_tri_search_unit_tests.html</anchorfile>
      <anchor>a5d1aa17f366ee0dddf326af59b775b8e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testTouch</name>
      <anchorfile>class_tri_search_unit_tests.html</anchorfile>
      <anchor>af0c83cdfee2822cd88b5e784e5d646ea</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testTriEnvelopesOverlap</name>
      <anchorfile>class_tri_search_unit_tests.html</anchorfile>
      <anchor>a9115acc6d9662f1d757bb082cb730450</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::TrOuterTriangleDeleter</name>
    <filename>classxms_1_1_tr_outer_triangle_deleter.html</filename>
    <member kind="function" static="yes">
      <type>static boost::shared_ptr&lt; TrOuterTriangleDeleter &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_tr_outer_triangle_deleter.html</anchorfile>
      <anchor>a0c647ff8f268d6f6cf8d46f2327a7f8f</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::TrOuterTriangleDeleterImpl</name>
    <filename>classxms_1_1_tr_outer_triangle_deleter_impl.html</filename>
    <base>xms::TrOuterTriangleDeleter</base>
    <member kind="enumeration">
      <type></type>
      <name>BoundaryEnum</name>
      <anchorfile>classxms_1_1_tr_outer_triangle_deleter_impl.html</anchorfile>
      <anchor>af029069b0274826a71ae5677357d8041</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>TrOuterTriangleDeleterImpl</name>
      <anchorfile>classxms_1_1_tr_outer_triangle_deleter_impl.html</anchorfile>
      <anchor>a71d9018e45d0db42a74cfdaf97c9d108</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>Delete</name>
      <anchorfile>classxms_1_1_tr_outer_triangle_deleter_impl.html</anchorfile>
      <anchor>ae68038220e16a9ad8cfb09088ec18b05</anchor>
      <arglist>(const VecInt2d &amp;a_polys, BSHP&lt; TrTin &gt; a_tin) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetObserver</name>
      <anchorfile>classxms_1_1_tr_outer_triangle_deleter_impl.html</anchorfile>
      <anchor>abc04a864841bfbb4d75978de497eaed0</anchor>
      <arglist>(BSHP&lt; Observer &gt; a_) override</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>FlagTrianglesAlongPolygon</name>
      <anchorfile>classxms_1_1_tr_outer_triangle_deleter_impl.html</anchorfile>
      <anchor>af243755c164dc8b6af8504aa6a037b8d</anchor>
      <arglist>(const VecInt &amp;a_poly, VecInt &amp;a_flags)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>MarkNeighbors</name>
      <anchorfile>classxms_1_1_tr_outer_triangle_deleter_impl.html</anchorfile>
      <anchor>a98c87038a67b2dbad2caa84d97700b1a</anchor>
      <arglist>(VecInt &amp;a_flags)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>VecInt2d</type>
      <name>m_polys</name>
      <anchorfile>classxms_1_1_tr_outer_triangle_deleter_impl.html</anchorfile>
      <anchor>a4c2c88e77f16bda161d8ce8811688fb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; TrTin &gt;</type>
      <name>m_tin</name>
      <anchorfile>classxms_1_1_tr_outer_triangle_deleter_impl.html</anchorfile>
      <anchor>acf5005baf48c97e177782c0926d4644b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; Observer &gt;</type>
      <name>m_observer</name>
      <anchorfile>classxms_1_1_tr_outer_triangle_deleter_impl.html</anchorfile>
      <anchor>ac7c0d7113b1bcc08e14becfbf66fb449</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TrOuterTriangleDeleterUnitTests</name>
    <filename>class_tr_outer_triangle_deleter_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>test1</name>
      <anchorfile>class_tr_outer_triangle_deleter_unit_tests.html</anchorfile>
      <anchor>a294ae02e69ef1ba5ee88016a54af2882</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::TrTin</name>
    <filename>classxms_1_1_tr_tin.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~TrTin</name>
      <anchorfile>classxms_1_1_tr_tin.html</anchorfile>
      <anchor>acbac3ff7b8a488bec68feace756d1234</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static BSHP&lt; TrTin &gt;</type>
      <name>New</name>
      <anchorfile>classxms_1_1_tr_tin.html</anchorfile>
      <anchor>ab1227a106b70c3061bd03cb3fb9833a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>TrTin</name>
      <anchorfile>classxms_1_1_tr_tin.html</anchorfile>
      <anchor>a13e29683d761b3f8e663c2a69098bfd3</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::TrTinImpl</name>
    <filename>classxms_1_1_tr_tin_impl.html</filename>
    <base>xms::TrTin</base>
    <member kind="function">
      <type></type>
      <name>TrTinImpl</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>adec0a5b6a63e3dd34c1871b1c7d8b52f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~TrTinImpl</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a73d34d1e66565922cbe1b440094b128a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetPoints</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a0c46d52ba123ea89d983831713de21de</anchor>
      <arglist>(BSHP&lt; VecPt3d &gt; a_pts) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetTriangles</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>ad93d04787d4dba68f1bdb521cfbc8486</anchor>
      <arglist>(BSHP&lt; VecInt &gt; a_tris) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetTrianglesAdjacentToPoints</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a3ece59ead0d9335a0039c1d0cec4f5f7</anchor>
      <arglist>(BSHP&lt; VecInt2d &gt; a_trisAdjToPts) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetGeometry</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a4a7c309bad7babf5b2e0e7e46ee626a5</anchor>
      <arglist>(BSHP&lt; VecPt3d &gt; a_pts, BSHP&lt; VecInt &gt; a_tris, BSHP&lt; VecInt2d &gt; a_trisAdjToPts) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual VecPt3d &amp;</type>
      <name>Points</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a4722de3f7981842b5fbfc5f48dc72c95</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual VecInt &amp;</type>
      <name>Triangles</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>abaddc41c2727beca5ffc08a94aa9b6e1</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual VecInt2d &amp;</type>
      <name>TrisAdjToPts</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a8dbab5f6b33aeb379c704a7b2343269a</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const VecPt3d &amp;</type>
      <name>Points</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a0db58a1aa456297fd3bd5a6807dd355d</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const VecInt &amp;</type>
      <name>Triangles</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a5560841bc5a87539eb17a6af8bdd3839</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const VecInt2d &amp;</type>
      <name>TrisAdjToPts</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a477102512bf2d7c9b15c5c6eb49d75f5</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual BSHP&lt; VecPt3d &gt;</type>
      <name>PointsPtr</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a1b1b98c5dd65471d090946399f31770b</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual BSHP&lt; VecInt &gt;</type>
      <name>TrianglesPtr</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a64478436f8fdc5482ce9e9fcea8b42fb</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>NumPoints</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a376cc601aa1f16cffd49c459fc9c9f7f</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>NumTriangles</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a5195a84d6cea132e8066316fbd5ba320</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>TriangleFromEdge</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>ade7ecff4a4b370270258659328575c0f</anchor>
      <arglist>(int a_pt1, int a_pt2, int &amp;a_tri, int &amp;a_idx1, int &amp;a_idx2) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>TriangleAdjacentToEdge</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a388567825f27fe8d6b183125af0cef1c</anchor>
      <arglist>(int a_pt1, int a_pt2) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>LocalIndex</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>ac60ab20aba505a86b7eaca1a6935f540</anchor>
      <arglist>(int a_tri, int a_pt) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>GlobalIndex</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>af2da41ae14938890b4fa3b6f1a96ca89</anchor>
      <arglist>(int a_triIdx, int a_localVtxIdx) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>VerticesAreAdjacent</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a70f4eb6dd87eb85eaca86e34f11da4fc</anchor>
      <arglist>(int a_pt1, int a_pt2) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>CommonEdgeIndex</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>afffd7788db43b8ef041f2719fb8d8431</anchor>
      <arglist>(int a_tri, int a_adjTri) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>AdjacentTriangle</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>ae50a008ac967323421d9574df798d69b</anchor>
      <arglist>(int a_triIdx, int a_edgeIdx) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual Pt3d</type>
      <name>TriangleCentroid</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a48c546057a0e95fd7ebe06f7a6aca7de</anchor>
      <arglist>(int a_tri) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual double</type>
      <name>TriangleArea</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a5c88f3679d65d282b0a67c6aff1341c6</anchor>
      <arglist>(int a_tri) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>NextBoundaryPoint</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>ac145283448f45f0b1ac4e5435421a8a2</anchor>
      <arglist>(int a_point) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>PreviousBoundaryPoint</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a798082ec079562c1616f0b0a509567a1</anchor>
      <arglist>(int a_point) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>GetBoundaryPoints</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a03191327872b758392b0dd2c4fa1f60c</anchor>
      <arglist>(VecInt &amp;a_boundaryPoints) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>GetBoundaryPolys</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a8b2ebdfcba14a1d76554c9cd077a7026</anchor>
      <arglist>(VecInt2d &amp;a_polys) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>GetExtents</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a7ec5b4c0b8cf8d22eb0251a66f8c3eac</anchor>
      <arglist>(Pt3d &amp;a_mn, Pt3d &amp;a_mx) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>ExportTinFile</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a89fad78d3d1f79fbfff7684e9323cc0b</anchor>
      <arglist>(std::ostream &amp;a_os) const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>SwapEdge</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a1466863471faaca3ac55bf4101fa972f</anchor>
      <arglist>(int a_triA, int a_triB, bool a_checkAngle=true) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>DeleteTriangles</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a0a160157240d033a400f55c730bb18f6</anchor>
      <arglist>(const SetInt &amp;a_trisToDelete) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>DeletePoints</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>af26463b8da49e0aa9bf2577468c28182</anchor>
      <arglist>(const SetInt &amp;a_points) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>OptimizeTriangulation</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a3edf221597262055cb3847430bd36ab8</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>RemoveLongThinTrianglesOnPerimeter</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a6539e41ceec60374722e504d8e38dfb2</anchor>
      <arglist>(const double a_ratio) override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>Clear</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>ac56cedd2e8a867bb3122716f8d30e1b8</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>BuildTrisAdjToPts</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>ae287535a7320100651aa21e10edead5b</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual std::string</type>
      <name>ToString</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a2cdc1c1c647cbb7ee180e55e5add6835</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>FromString</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a1b7c1dacb8e701e5f5229f8a9624c47f</anchor>
      <arglist>(const std::string &amp;) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>serialize</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a83741ee7a8ad569412bb6715f500f3df</anchor>
      <arglist>(Archive &amp;archive, const unsigned int version)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>InsertAdjacentTriangle</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a0d1478852f239878bae47c9e685e2453</anchor>
      <arglist>(int a_pt, int a_tri)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>DeleteAdjacentTriangle</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a472a843922d633fdf17061006fce3868</anchor>
      <arglist>(int a_pt, int a_tri)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>TriIndexFound</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a253a5434ce056758375e7ee7b565c12e</anchor>
      <arglist>(const int &amp;a_triPt) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>PointIndexFound</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a8947cb759381812e9b2b906b48874553</anchor>
      <arglist>(const Pt3d &amp;a_point) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>AdjacentIndexFound</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>ab66a31615263d8ea599d92d20316d329</anchor>
      <arglist>(const VecInt &amp;a_point) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>CheckAndSwap</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a2283ff7bbf56262e0ba0d4ecbfb15b35</anchor>
      <arglist>(int a_triA, int a_triB, bool a_propagate, const VecInt &amp;a_flags)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>PointIsInCircumcircle</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a208635ddaf084e5baae40c2538ff1dd1</anchor>
      <arglist>(int a_tri1, int a_tri2, int id)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>BuildTrisAdjToPtsConst</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a92fa71c48a02cb66695487cf01fef1cc</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>CheckTriangle</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a819d105df8de79aae59791e7316ee209</anchor>
      <arglist>(const int a_tri, const int a_index, const double a_ratio, VecInt &amp;a_flags, SetInt &amp;a_trisToDelete) const</arglist>
    </member>
    <member kind="function" protection="private">
      <type>int</type>
      <name>AdjacentTriangleIndex</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a5fd85293103096659f5d7311c1e5f252</anchor>
      <arglist>(const int a_currTri, const int a_adjTri) const</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; VecPt3d &gt;</type>
      <name>m_pts</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a30b990ac9bc359b54d476992c140db4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; VecInt &gt;</type>
      <name>m_tris</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a68fa36caa1358e96e27dcdf47023c1aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; VecInt2d &gt;</type>
      <name>m_trisAdjToPts</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>a9f129e46ee51dbc0b3d3fef0cb794b0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>boost::unordered_set&lt; int &gt;</type>
      <name>m_toDelete</name>
      <anchorfile>classxms_1_1_tr_tin_impl.html</anchorfile>
      <anchor>ae197cb5a74cb139418af07ea12161ab6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TrTinUnitTests</name>
    <filename>class_tr_tin_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>test1</name>
      <anchorfile>class_tr_tin_unit_tests.html</anchorfile>
      <anchor>a198e55ca6fc0831488240809cffddea2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testOptimizeTriangulation</name>
      <anchorfile>class_tr_tin_unit_tests.html</anchorfile>
      <anchor>a29a07efdba13d58663f8a61fd5404d4f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testSwap</name>
      <anchorfile>class_tr_tin_unit_tests.html</anchorfile>
      <anchor>ab978361fc941ce007bfa7d419a0bce92</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testBoundaries</name>
      <anchorfile>class_tr_tin_unit_tests.html</anchorfile>
      <anchor>a9a7adf7f4b8ab36ef62d3d7e55bf5c92</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testDeleteTriangles</name>
      <anchorfile>class_tr_tin_unit_tests.html</anchorfile>
      <anchor>a390a1322e40b297601fccea53fad2472</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testDeletePoints</name>
      <anchorfile>class_tr_tin_unit_tests.html</anchorfile>
      <anchor>a609dc373196faee1173a89d9955d1559</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testRemoveLongThinTrianglesOnPerimeter</name>
      <anchorfile>class_tr_tin_unit_tests.html</anchorfile>
      <anchor>a2393a141eb2f1bc3d75b8f45094e915d</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::TrTriangulator</name>
    <filename>classxms_1_1_tr_triangulator.html</filename>
    <member kind="function">
      <type></type>
      <name>TrTriangulator</name>
      <anchorfile>classxms_1_1_tr_triangulator.html</anchorfile>
      <anchor>ac54bc210a1de778f7b6350170e31da26</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~TrTriangulator</name>
      <anchorfile>classxms_1_1_tr_triangulator.html</anchorfile>
      <anchor>abfb9ab61fc0a609f964751b493275366</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Triangulate</name>
      <anchorfile>classxms_1_1_tr_triangulator.html</anchorfile>
      <anchor>a76a62ce7b12c2cc317d8725dba3cef16</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xms::TrTriangulatorPoints</name>
    <filename>classxms_1_1_tr_triangulator_points.html</filename>
    <base>xms::TrTriangulator</base>
    <member kind="function">
      <type></type>
      <name>TrTriangulatorPoints</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>a06ec7a3c688accca4f7c72a3354e1400</anchor>
      <arglist>(const VecPt3d &amp;a_pts, VecInt &amp;a_tris, VecInt2d *a_trisAdjToPts=nullptr)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>SetObserver</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>a9a4225d2e6a3b30d020e0bffffb6351b</anchor>
      <arglist>(BSHP&lt; Observer &gt; a_)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>GetID</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>a5a2ab0245a79750a3212e2400edca40a</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual Pt3d</type>
      <name>GetLocation</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>adde88d87e5db9f9be2c13e9075da6eca</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>GetNPoints</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>adbc913759a325fd2f01283e8112a8c0e</anchor>
      <arglist>() const override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>IncrementPoint</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>ab1dcfd044e79eadc9c91a6feebbb5726</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>PrepareToReceiveTriangles</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>a5e04e8e6077b2456782889f83170123b</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>FinalizeTriangulation</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>a91c8c4eb53d2b252c816b168d0b5f7e5</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>ReceiveTriangle</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>ab4fb455a7ae86c84b8e29b5d03db916d</anchor>
      <arglist>(int a_id1, int a_id2, int a_id3) override</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>UpdateAreaTolerance</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>a172065079973143ab239b0b983b01074</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>const VecPt3d &amp;</type>
      <name>m_pts</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>aaa180402a34969a95e0b3db430d731ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>m_idx</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>ab14bc4456e99495852a63020ee1c644f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>VecInt &amp;</type>
      <name>m_tris</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>ade8f4a814000a26fede635f25b2d4d41</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>VecInt2d *</type>
      <name>m_trisAdjToPts</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>a39ab26cea9e757db19322c7393fbfdfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>m_areaTol</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>af1b6f3704855069f790350cd0e77ebd0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BSHP&lt; Observer &gt;</type>
      <name>m_observer</name>
      <anchorfile>classxms_1_1_tr_triangulator_points.html</anchorfile>
      <anchor>a051d44b473938bc5da0b4810bc3d912d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TrTriangulatorPointsUnitTests</name>
    <filename>class_tr_triangulator_points_unit_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>test1</name>
      <anchorfile>class_tr_triangulator_points_unit_tests.html</anchorfile>
      <anchor>a33422cef521b757882239d403bc6569d</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TutInterpolationIntermediateTests</name>
    <filename>class_tut_interpolation_intermediate_tests.html</filename>
    <member kind="function">
      <type>void</type>
      <name>test_Example_IDW</name>
      <anchorfile>class_tut_interpolation_intermediate_tests.html</anchorfile>
      <anchor>a07da59ac97a03923d1894fc0db7ebd46</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test_Example_Linear</name>
      <anchorfile>class_tut_interpolation_intermediate_tests.html</anchorfile>
      <anchor>a8a81dfb33fad9290a29c89a73febadfa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test_Example_NaturalNeighbor</name>
      <anchorfile>class_tut_interpolation_intermediate_tests.html</anchorfile>
      <anchor>a4016c266625b88129df8654a1d84df5d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test_Example_Anisotropic</name>
      <anchorfile>class_tut_interpolation_intermediate_tests.html</anchorfile>
      <anchor>a938de7e5b7abc72bbeaeb2d014542e46</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>md_Interpolation_Tutorial</name>
    <title>Interpolation_Tutorial</title>
    <filename>md__interpolation__tutorial</filename>
    <docanchor file="md__interpolation__tutorial" title="Interpolation Tutorial">Interpolation_Tutorial</docanchor>
    <docanchor file="md__interpolation__tutorial" title="Introduction">Intro_Interpolation</docanchor>
    <docanchor file="md__interpolation__tutorial" title="Example - Inverse Distance Weighted (IDW) Interpolation">Example_IDW</docanchor>
    <docanchor file="md__interpolation__tutorial" title="Example - Linear Interpolation">Example_Linear</docanchor>
    <docanchor file="md__interpolation__tutorial" title="Example - Natural Neighbor Interpolation">Example_NatNeigh</docanchor>
    <docanchor file="md__interpolation__tutorial" title="Example - Anisotropic Interpolation">Example_Anisotropic</docanchor>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>xmsinterp</title>
    <filename>index</filename>
    <docanchor file="index" title="xmsinterp ">xmsinterp</docanchor>
    <docanchor file="index" title="Introduction ">XmsinterpIntroduction</docanchor>
    <docanchor file="index" title="License ">XmsinterpLicense</docanchor>
    <docanchor file="index" title="Features ">XmsinterpFeatures</docanchor>
    <docanchor file="index" title="Testing ">XmsinterpTesting</docanchor>
    <docanchor file="index" title="The Code ">XmsinterpTheCode</docanchor>
    <docanchor file="index" title="Namespaces">XmsinterpNamespaces</docanchor>
    <docanchor file="index" title="Interface pattern">XmsinterpInterfacePattern</docanchor>
    <docanchor file="index" title="&quot;xmsinterp&quot; Name ">xmsinterpName</docanchor>
  </compound>
</tagfile>
