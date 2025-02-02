// ---------------------------------------------------------------------
//
// Copyright (C) 2017 - 2021 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE.md at
// the top level directory of deal.II.
//
// ---------------------------------------------------------------------

/**
@page changes_between_9_3_0_and_9_3_1 Changes between Version 9.3.0 and 9.3.1

<p>
This is the list of changes made between the release of deal.II version
9.3.0 and that of 9.3.1. All entries are signed with the names of the
author.
</p>

<!-- ----------- SPECIFIC IMPROVEMENTS ----------------- -->

<a name="930-931-specific"></a>
<h3>Specific improvements</h3>
<ol>

 <li>
  Fixed: Various MPI configuration and compatibility issues have been
  resolved.
  <br>
  (Timo Heister, Matthias Maier, 2021/06/29)
 </li>

 <li>
  Fixed: Added a proper configuration guard for a gcc 11 compiler bug. This
  fixes a compilation issue with Intel compilers versions 18 and 19.
  <br>
  (Matthias Maier, 2021/06/29)
 </li>

 <li>
  Fixed: various gcc 11 warnings, missing instantiations and compilation
  and configuration issues.
  <br>
  (Timo Heister, David Wells, 2021/06/29)
 </li>

 <li>
  Fixed: Removed some pointers in MappingQGeneric and MappingFE that could cause
  segmentation faults at high optimization levels due to compiler assumptions
  about data alignment.
  <br>
  (David Wells, 2021/06/17)
 </li>

 <li>
  Fixed: Reorder grid cells in GridIn::read_exodusii() for a quad/hex mesh.
  This allows the mesh to be used in a
  parallel::distributed::Triangulation.
  <br>
  (Maximilian Bergbauer, 2021/06/07)
 </li>

</ol>

*/
