#include <mpi.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

int main (int argc, char *argv[])
{
  MPI::Init (argc, argv);
  int task_count = MPI::COMM_WORLD.Get_size ();
  int task_rank = MPI::COMM_WORLD.Get_rank ();
//std::cout << "Hello, world!  This is MPI process "
//          << task_rank
//          << " of "
//          << task_count
//          << "."
//          << std::endl;
  std::ostringstream stream;
  stream << "Hello, world!  This is MPI process "
         << task_rank
         << " of "
         << task_count
         << ".\n";
  std::cout << stream.str ();
  MPI::COMM_WORLD.Barrier ();
  MPI::Finalize ();

  return EXIT_SUCCESS;
}
