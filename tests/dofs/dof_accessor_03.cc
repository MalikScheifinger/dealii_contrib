// ---------------------------------------------------------------------
//
// Copyright (C) 2021 - 2023 by the deal.II authors
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

// Test DoFCellAccessor::distribute_local_to_global() works with
// std::vector<T>::iterators

#include <deal.II/base/mpi.h>

#include <deal.II/distributed/tria.h>

#include <deal.II/dofs/dof_handler.h>

#include <deal.II/fe/fe_q.h>

#include <deal.II/grid/grid_generator.h>
// #include <deal.II/grid/tria.h>

#include <deal.II/lac/la_parallel_vector.h>

#include "../tests.h"

using namespace dealii;

template <int dim, typename Number>
void
do_test()
{
  // create triangulation
  parallel::distributed::Triangulation<dim> tria(MPI_COMM_WORLD);
  GridGenerator::hyper_cube(tria);
  tria.refine_global(2);

  FE_Q<dim>       fe_q(2);
  DoFHandler<dim> dof_handler(tria);
  dof_handler.distribute_dofs(fe_q);

  LinearAlgebra::distributed::Vector<Number> vector1(dof_handler.n_dofs());
  LinearAlgebra::distributed::Vector<Number> vector2(dof_handler.n_dofs());
  std::vector<Number>                        std_vector(fe_q.dofs_per_cell);
  std::iota(std_vector.begin(), std_vector.end(), Number{1});
  for (const auto &cell : dof_handler.active_cell_iterators())
    {
      Vector<Number> dealii_vector(std_vector.begin(), std_vector.end());
      cell->distribute_local_to_global(dealii_vector.begin(),
                                       dealii_vector.end(),
                                       vector1);

      cell->distribute_local_to_global(std_vector.begin(),
                                       std_vector.end(),
                                       vector2);
    }

  // check if both vectors are the same
  vector1.add(Number{-1.0}, vector2);
  assert(vector1.l2_norm() < 1e-12);

  deallog << "OK " << std::endl;
  deallog << std::endl;
}

int
main(int argc, char **argv)
{
  Utilities::MPI::MPI_InitFinalize mpi_initialization(argc, argv, 1);
  MPILogInitAll                    all;
  deallog.precision(8);

  do_test<2, double>();

  return 0;
}
