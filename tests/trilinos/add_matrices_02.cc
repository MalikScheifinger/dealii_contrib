// ---------------------------------------------------------------------
//
// Copyright (C) 2004 - 2018 by the deal.II authors
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



// Test SparseMatrix::add(factor, SparseMatrix) based on different sparsity
// patterns (neither sparsity pattern is more inclusive than the other, but
// the entry in the rhs matrix in the 'offending' position is zero).

#include <deal.II/base/utilities.h>

#include <deal.II/lac/trilinos_sparse_matrix.h>
#include <deal.II/lac/trilinos_sparsity_pattern.h>

#include <iostream>

#include "../tests.h"


void
test()
{
  unsigned int myid    = Utilities::MPI::this_mpi_process(MPI_COMM_WORLD);
  unsigned int numproc = Utilities::MPI::n_mpi_processes(MPI_COMM_WORLD);

  if (myid == 0)
    deallog << "numproc=" << numproc << std::endl;


  // each processor owns 3 indices
  IndexSet local_owned(numproc * 3);
  local_owned.add_range(myid * 3, myid * 3 + 3);

  // Create sparsity patterns
  TrilinosWrappers::SparsityPattern sp1(local_owned, MPI_COMM_WORLD),
    sp2(local_owned, MPI_COMM_WORLD);

  for (unsigned int i = myid * 3; i < myid * 3 + 3; ++i)
    {
      sp1.add(i, i);
      sp2.add(i, i);
    }
  if (myid == 0)
    {
      sp1.add(0, 1);
      sp2.add(1, 0);
    }

  sp1.compress();
  sp2.compress();

  // create matrices by adding some elements into the respective positions
  TrilinosWrappers::SparseMatrix m1(sp1), m2(sp2);
  for (unsigned int i = myid * 3; i < myid * 3 + 3; ++i)
    {
      m1.add(i, i, i + 2);
      m2.add(i, i, 4);
    }
  if (myid == 0)
    m1.add(0, 1, 3);

  m1.compress(VectorOperation::add);
  m2.compress(VectorOperation::add);

  m1.add(2, m2);

  // Check for correctness of entries (all floating point comparisons should
  // be exact)
  for (unsigned int i = myid * 3; i < myid * 3 + 3; ++i)
    Assert(m1.el(i, i) == (double)i + 2 + 8, ExcInternalError());
  if (myid == 0)
    Assert(m1.el(0, 1) == 3., ExcInternalError());

  deallog << "OK" << std::endl;
}



int
main(int argc, char **argv)
{
  initlog();

  Utilities::MPI::MPI_InitFinalize mpi_initialization(
    argc, argv, testing_max_num_threads());

  try
    {
      test();
    }
  catch (const std::exception &exc)
    {
      std::cerr << std::endl
                << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Exception on processing: " << std::endl
                << exc.what() << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;

      return 1;
    }
  catch (...)
    {
      std::cerr << std::endl
                << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Unknown exception!" << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    };
}
