#include "grid_math.h"
#include <mpi.h>
using namespace std;


// Something like lake michigan
#define dx  (1000./(float) SCALE)
#define dy  (1000./(float) SCALE)
#define dt  (20./(float) SCALE)

#define NX (125.e3 / dx )
#define NY (450.e3 / dy )
#define gee 9.8
#define NSTEP 1000
#define freq  (NSTEP/10)

#define DTYPE float

void step(int nbase, grid2<DTYPE> &u, grid2<DTYPE> &v, grid2<DTYPE> *eta, grid2<DTYPE> &tmpeta, grid2<DTYPE> &h);

int main(int argc, char *argv[]) {
  grid2<DTYPE> u, v, h, eta[3];
  grid2<DTYPE> tmp, tmpeta;
  int i, j, nbase, np;
  ijpt loc, center;
  int nx, ny;
  int global_nx, global_ny;
  char fname[900];
  palette<unsigned char> gg(19, 65);
// if argument, then run for spec'd number of steps:
  int nstep = NSTEP;

// MPI related:
  int myrank, nprocs;

  MPI::Init(argc, argv);
  nprocs = MPI::COMM_WORLD.Get_size();
  myrank = MPI::COMM_WORLD.Get_rank();
  printf("process %5d of %5d\n",myrank, nprocs);

  if (argc == 2) nstep = atoi(argv[1]);

  global_nx = (int) (0.5 + NX);
  global_ny = (int) (0.5 + NY);
  //if (global_nx%nprocs != 0) global_nx += nprocs - global_nx%nprocs ;
  if (global_ny%nprocs != 0) global_ny += nprocs - global_ny%nprocs ;
  printf("run length is %f seconds, %f minutes\n",nstep*dt, nstep*dt/60 );
  printf("global_nx, global_ny = %f %f rounded %d %d\n",NX, NY, global_nx, global_ny);

// Here we would figure out where we are in the stencil and parcel out the
//   correct sizes for each processor.  For now, blindly pass out local=global
  nx = global_nx;
  if (nprocs == 1) {
    ny = global_ny;
  }
  else if (nprocs == 2) {
    ny = global_ny/nprocs ;
  }
  else {
    ny = global_ny / nprocs ;
  }
  printf("processor %5d has grid %d by %d vs globals of %d by %d\n",myrank, nx, ny, global_nx, global_ny);

// Now on with the universalities:

  tmpeta.resize(nx, ny);
  h.resize(nx, ny);
  h.set((DTYPE) 100.);
  u.resize(nx, ny);
  v.resize(nx, ny);
  u.set((DTYPE) 0.);
  v.set((DTYPE) 0.);
  printf("h gridmax %f, max wave speed %f\n",h.gridmax(), sqrt(h.gridmax()*gee) );

  for (i = 0; i < 3; i++) {
    eta[i].resize(nx, ny);
    eta[i].set((DTYPE) 0.);
  }   

// Initial conditions
    int dj;
    center.i = global_nx/2;
    center.j = global_ny/3;  
    printf("center at %d %d dx dy = %f %f nx ny = %d %d\n",center.i, center.j, dx, dy, nx, ny);
    for (loc.j = 0; loc.j < ny; loc.j++) {
      dj = center.j - ny*myrank - loc.j; 
    for (i = -nx/2; i < nx/2; i++) {
      loc.i = center.i + i;
      eta[2][loc] = 1.0 * exp(- (i*i*dx*dx+dj*dj*dy*dy) / (250*60)*(250*60) );
      eta[1][loc] = 1.0 * exp(- (i*i*dx*dx+dj*dj*dy*dy) / (250*60)*(250*60) );
      eta[0][loc] = 1.0 * exp(- (i*i*dx*dx+dj*dj*dy*dy) / (250*60)*(250*60) );
      //printf("%d %d has %e di dj = %d %d\n",
      //     loc.i, loc.j,  exp(- (i*i*dx*dx+dj*dj*dy*dy) / (250*20)*(250*20) ), i, dj );
    }
    }


   for (i = 0; i < 3; i++) {
      printf("proc %5d Initial conditions max step %d is %e\n",myrank, i, eta[i].gridmax());
   }
   fflush(stdout);


// Comm buffers:
   float *sendp, *sendm, *recvp, *recvm;
   ijpt sloc, rloc; // indices
   int psendp = myrank + 1, psendm = myrank - 1; // processors to send to 
   int precvp = myrank + 1, precvm = myrank - 1; // processors to receive from
   int itag1 = 0;
   MPI::Request requests[4];
// Note: 3*nx because we're passing eta, u, v (in that order), rather than just eta
   sendp = new float[nx*3];
   sendm = new float[nx*3];
   recvm = new float[nx*3];
   recvp = new float[nx*3];

// Main loop:
  for (i = 0; i <= nstep; i++) {
    nbase = (i+2)%3;
    np = (nbase + 1)%3;
    step(nbase, u, v, eta, tmpeta, h);

// Communications in here
    if (nprocs == 1 ) { 
       // do nothing
    }
    else {
      if (myrank == 0 ) {
        // swap info with processor myrank + 1
        sloc.j = ny - 2;
        for (sloc.i = 0; sloc.i < nx; sloc.i ++) {
          sendp[sloc.i] = eta[np][sloc];
          sendp[sloc.i+nx] = u[sloc];
          sendp[sloc.i+2*nx] = v[sloc];
        }
        requests[0] = MPI::COMM_WORLD.Isend(sendp, 3*nx, MPI::FLOAT, psendp, itag1);
        requests[1] = MPI::COMM_WORLD.Irecv(recvp, 3*nx, MPI::FLOAT, precvp, itag1);
        requests[0].Waitall(1, &requests[0]);
        requests[1].Waitall(1, &requests[1]);
        
        sloc.j = ny - 1;
        for (sloc.i = 0; sloc.i < nx; sloc.i ++) {
          eta[np][sloc] = recvp[sloc.i]; 
          u[sloc]       = recvp[sloc.i+nx];
          v[sloc]       = recvp[sloc.i+2*nx];
        }
      }
      else if (myrank == (nprocs - 1 ) ) {
        // swap info with processor myrank - 1
        sloc.j = 0;
        for (sloc.i = 0; sloc.i < nx; sloc.i ++) {
          sendm[sloc.i] = eta[np][sloc];
          sendm[sloc.i+nx] = u[sloc];
          sendm[sloc.i+nx*2] = v[sloc];
        }
        requests[0] = MPI::COMM_WORLD.Irecv(recvm, 3*nx, MPI::FLOAT, precvm, itag1);
        requests[1] = MPI::COMM_WORLD.Isend(sendm, 3*nx, MPI::FLOAT, psendm, itag1);
        requests[0].Waitall(1, &requests[0]);
        requests[1].Waitall(1, &requests[1]);
        sloc.j = 1;
        for (sloc.i = 0; sloc.i < nx; sloc.i ++) {
          eta[np][sloc] = recvm[sloc.i];
          u[sloc]       = recvm[sloc.i+nx];
          v[sloc]       = recvm[sloc.i+nx*2];
        }
      }
      else {
        // swap info with processors myrank + 1 and - 1
        // note: not doing all sends/receives and then waiting.  This may be less efficient
        // extract data to send:
        sloc.j = ny - 2;
        for (sloc.i = 0; sloc.i < nx; sloc.i ++) {
          sendp[sloc.i]      = eta[np][sloc];
          sendp[sloc.i+nx]   = u[sloc];
          sendp[sloc.i+nx*2] = v[sloc];
        }
        sloc.j = 0;
        for (sloc.i = 0; sloc.i < nx; sloc.i ++) {
          sendm[sloc.i]      = eta[np][sloc];
          sendm[sloc.i+nx]   = u[sloc];
          sendm[sloc.i+nx*2] = v[sloc];
        }

        // swap info with processor myrank + 1
        requests[0] = MPI::COMM_WORLD.Isend(sendp, 3*nx, MPI::FLOAT, psendp, itag1);
        requests[1] = MPI::COMM_WORLD.Irecv(recvp, 3*nx, MPI::FLOAT, precvp, itag1);

        // swap with processor myrank - 1
        requests[2] = MPI::COMM_WORLD.Irecv(recvm, 3*nx, MPI::FLOAT, precvm, itag1);
        requests[3] = MPI::COMM_WORLD.Isend(sendm, 3*nx, MPI::FLOAT, psendm, itag1);

        requests[0].Waitall(1, &requests[0]);
        requests[1].Waitall(1, &requests[1]);
        requests[2].Waitall(1, &requests[2]);
        requests[3].Waitall(1, &requests[3]);

        // Insert received data:
        sloc.j = ny - 1;
        for (sloc.i = 0; sloc.i < nx; sloc.i ++) {
          eta[np][sloc] = recvp[sloc.i];
          u[sloc]       = recvp[sloc.i+nx];
          v[sloc]       = recvp[sloc.i+nx*2];
        }
        sloc.j = 1;
        for (sloc.i = 0; sloc.i < nx; sloc.i ++) {
          eta[np][sloc] = recvm[sloc.i];
          u[sloc]       = recvm[sloc.i+nx];
          v[sloc]       = recvm[sloc.i+nx*2];
        }

      }

    }
// Done communicating

// Each processor prints out its view of the world:
    if (i >= 0 && (i % freq) == 0) {
      // now start to produce graphics
      tmp = eta[nbase];
      //tmp += 0.50;
      //tmp *= 128;
      tmp.scale();
      //printf("proc %1d tmp max, min %f %f\n",myrank,tmp.gridmax(), tmp.gridmin() );
      printf("proc %5d step %d eta max, min, avg, rms %f %f %f %f\n",
                 myrank,i, 
                eta[nbase].gridmax(), eta[nbase].gridmin(), eta[nbase].average(), eta[nbase].rms() );
      fflush(stdout);
      sprintf(fname,"eta%05d%05d.xpm",i,myrank);
      tmp.xpm(fname, 7, gg); 
    }
    
  }


  MPI::Finalize();
  return 0;
}

void step(int nbase, grid2<DTYPE> &u, grid2<DTYPE> &v, grid2<DTYPE> *eta, grid2<DTYPE> &tmpeta, grid2<DTYPE> &h) {
  int i, n, np, nm;
  ijpt loc, locip, locjp, locim, locjm;
  int nx = h.xpoints(), ny = h.ypoints() ;

  if (nbase == 2) {
    n = 2;
    np = 0;
    nm = 1;
  }
  else if (nbase == 1) {
    n = 1;
    np = 2;
    nm = 0;
  }
  else if (nbase == 0) {
    n = 0;
    np = 1;
    nm = 2;
  }
  else {
    printf("nbase is out of range, = %d\n",nbase);
    return;
  }

  tmpeta = eta[np];

// Centered in time 
  eta[np] = eta[nm];
// Forward in time on u,v

  DTYPE c0 = 2.*dt/2./dx;
  for (loc.j = 1; loc.j < ny - 1; loc.j++) {
    locim.j = loc.j;
    locip.j = loc.j;
    for (loc.i = 1; loc.i < nx - 1; loc.i++) {
      locim.i = loc.i - 1;
      locip.i = loc.i + 1;
      // centered, but ensure we advance eta before u,v as u,v are only
      //   keeping 1 time level, overwriting the previous:
      eta[np][loc] -= c0 * 
         (u[locip]*h[locip] - u[locim]*h[locim]);
     }
  }
  for (loc.j = 1; loc.j < ny - 1; loc.j++) {
    locjm.j = loc.j - 1;
    locjp.j = loc.j + 1;
    for (loc.i = 1; loc.i < nx - 1; loc.i++) {
    //for (loc.i = 1; loc.i < h.xpoints() - 1; loc.i++) {
      locjm.i = loc.i ;
      locjp.i = loc.i ;
      eta[np][loc] -= c0 * 
         (v[locjp]*h[locjp] - v[locjm]*h[locjm])    ;
    }
  }

    // forward in u,v -- overwriting, so must keep in separate loop
  DTYPE c1 = gee * dt / 2. / dx;
  // split loops for mac 9.1 v. 9.8 sec
  for (loc.j = 1; loc.j < ny - 1; loc.j++) {
  //for (loc.j = 1; loc.j < h.ypoints() - 1; loc.j++) {
    locim.j = loc.j;
    locip.j = loc.j;
    for (loc.i = 1; loc.i < nx - 1; loc.i++) {
    //for (loc.i = 1; loc.i < h.xpoints() - 1; loc.i++) {
      locim.i = loc.i - 1;
      locip.i = loc.i + 1;
      u[loc] -= c1 * (eta[n][locip] - eta[n][locim]);
    }
  }

  for (loc.j = 1; loc.j < ny - 1; loc.j++) {
  //for (loc.j = 1; loc.j < h.ypoints() - 1; loc.j++) {
    locjm.j = loc.j - 1;
    locjp.j = loc.j + 1;
    //for (loc.i = 1; loc.i < nx - 1; loc.i++) {
    for (loc.i = 1; loc.i < h.xpoints() - 1; loc.i++) {
      locjm.i = loc.i ;
      locjp.i = loc.i ;
      v[loc] -= c1 * (eta[n][locjp] - eta[n][locjm]);
    }
  }

  eta[nm] = eta[n];
  eta[n] = tmpeta;

  #ifdef DIAGNOSTIC
  i = nbase;
  DTYPE urms = u.rms(), vrms = v.rms(), etarms = eta[i].rms();
  printf("%d gridmax %f %f %f  rms %e %e %e energy %e u-v %e\n", i, u.gridmax(), v.gridmax(), eta[i].gridmax(), urms, vrms, etarms, gee*etarms*etarms + urms*urms + vrms*vrms, urms - vrms);
  //printf("%d rms %e %e %e energy %e u-v %e\n", i, urms, vrms, etarms, gee*etarms*etarms + urms*urms + vrms*vrms, urms - vrms);
  #endif

  return;
}
