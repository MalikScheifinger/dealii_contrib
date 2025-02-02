// ---------------------------------------------------------------------
//
// Copyright (C) 2016 - 2018 by the deal.II authors
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


// Tests vector multiplication and eigenvalues of LAPACKFullMatrix
// copy-paste of lapack/full_matrix_01

#include <deal.II/base/logstream.h>

#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/lapack_full_matrix.h>
#include <deal.II/lac/vector.h>

#include <fstream>
#include <iostream>

/*
 * Eigenvalues and -vectors of this system are
 * lambda = 1     v = (1, 1, 1, 1)
 * lambda = 5     v = (1,-1, 0, 0)
 * lambda = 5     v = (0, 1,-1, 0)
 * lambda = 5     v = (0, 0, 1,-1)
 */
const double symm[4][4] = {{4., -1., -1., -1.},
                           {-1., 4., -1., -1.},
                           {-1., -1., 4., -1.},
                           {-1., -1., -1., 4.}};

const double rect[3][4] = {{4., 3., 2., 1.},
                           {5., 8., 1., -2.},
                           {11., 13., -4., -5}};

using namespace dealii;

void
test_rect(unsigned int m, unsigned int n, const double *values)
{
  std::ostringstream prefix;
  prefix << m << 'x' << n;
  deallog.push(prefix.str());

  FullMatrix<double>       A(m, n, values);
  LAPACKFullMatrix<double> LA(m, n);
  LA = A;

  Vector<double> u(n);
  Vector<double> v1(m);
  Vector<double> v2(m);

  for (unsigned int i = 0; i < u.size(); ++i)
    u(i) = i * i;

  deallog << "operator= (const FullMatrix<number>&) ok" << std::endl;

  A.vmult(v1, u);
  LA.vmult(v2, u);
  v1 -= v2;
  if (v1.l2_norm() < 1.e-14)
    deallog << "vmult ok" << std::endl;
  v1 = v2;

  A.vmult_add(v1, u);
  LA.vmult_add(v2, u);
  v1 -= v2;
  if (v1.l2_norm() < 1.e-14)
    deallog << "vmult_add ok" << std::endl;

  LA.Tvmult(u, v2);
  u *= -1;
  A.Tvmult_add(u, v2);
  if (u.l2_norm() < 1.e-14)
    deallog << "Tvmult ok" << std::endl;

  A.Tvmult(u, v2);
  u *= -1;
  LA.Tvmult_add(u, v2);
  if (u.l2_norm() < 1.e-14)
    deallog << "Tvmult_add ok" << std::endl;

  deallog.pop();
}

int
main()
{
  const std::string logname = "output";
  std::ofstream     logfile(logname);
  logfile.precision(3);
  deallog.attach(logfile);

  test_rect(3, 4, &rect[0][0]);
  test_rect(4, 3, &rect[0][0]);
  test_rect(4, 4, &symm[0][0]);

  // Test symmetric system
  FullMatrix<double>       A(4, 4, &symm[0][0]);
  LAPACKFullMatrix<double> LA(4, 4);
  LA = A;
  LA.compute_eigenvalues();
  for (unsigned int i = 0; i < A.m(); ++i)
    {
      std::complex<double> lambda = LA.eigenvalue(i);
      deallog << "Eigenvalues " << static_cast<int>(lambda.real() + .0001)
              << '\t' << static_cast<int>(lambda.imag() + .0001) << std::endl;
    }
}
