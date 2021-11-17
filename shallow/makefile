all : hello shallow

shallow : shallow.C
	#g++ -cpp -DSCALE=5 -I /u/Robert.Grumbine/save/mmablib_v3.3.1/mmablib/include -I /usr/mpi/gcc/openmpi-1.4.6/include shallow.C -o shallow -L /usr/mpi/gcc/openmpi-1.4.6/lib64 -lmpi_cxx
	#mpCC -DSCALE=12 -I /u/Robert.Grumbine/save/mmablib_v3.3.1/mmablib/include shallow.C -o shallow 
	mpiicpc -DSCALE=8 -I /home/Robert.Grumbine/save/mmablib.git/mmablib/include shallow.C -o shallow 

hello : hello.C
	#mpiicc hello.C -o hello 
	mpiicpc hello.C -o hello 

