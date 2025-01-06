#include<stdio.h>
#include<math.h>

/*Change the following line based on the PATH/TO/MEX.H on your system*/
#include"mex.h"


/*Input Arguments*/
#define P_IN         prhs[0]
#define T_IN         prhs[1]
#define FIXED_PARAMS prhs[2]
#define ERR          0
/*Output Arguments*/
#define A_OUT  plhs[0]

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  int N_tri;int N_nodes;
  double *p_in_ptr, *t_in_ptr;
  double *params_ptr; 
  double *a_out_ptr;  
  int i_tri;
  int dims[3];
  int temp_idx_t0, temp_idx_t1, temp_idx_t2;
  double x0, y0, x1, y1, x2, y2;
  double temp_delta, temp_delta_0, temp_delta_1, temp_delta_2;

  /*Get the input points, nodes, triangles*/
  p_in_ptr = mxGetPr(P_IN);
  t_in_ptr = mxGetPr(T_IN);

  /*Get the dimensions*/
  params_ptr = mxGetPr(FIXED_PARAMS);
  N_nodes = (int)*(params_ptr+0);
  N_tri   = (int)*(params_ptr+1);

    
  /*Create XT_IDX_OUT matrix*/
  dims[0] = 3;
  dims[1] = 3;
  dims[2] = N_tri;
  A_OUT = mxCreateNumericArray(3,dims,mxDOUBLE_CLASS,mxREAL);
  a_out_ptr = mxGetPr(A_OUT);
  
  
  for(i_tri = 0;i_tri < N_tri;i_tri++)
    {
      /*the point t0*/
      temp_idx_t0 = (int)(*(t_in_ptr + i_tri + 0*N_tri)-1);
      temp_idx_t1 = (int)(*(t_in_ptr + i_tri + 1*N_tri)-1);
      temp_idx_t2 = (int)(*(t_in_ptr + i_tri + 2*N_tri)-1);
      

      x0 = *(p_in_ptr + temp_idx_t0 + 0*N_nodes);y0 = *(p_in_ptr + temp_idx_t0 + 1*N_nodes); 
      x1 = *(p_in_ptr + temp_idx_t1 + 0*N_nodes);y1 = *(p_in_ptr + temp_idx_t1 + 1*N_nodes); 
      x2 = *(p_in_ptr + temp_idx_t2 + 0*N_nodes);y2 = *(p_in_ptr + temp_idx_t2 + 1*N_nodes);

      temp_delta = (x1*y2-x2*y1) + (x0*y1-x0*y2) + (x2*y0-x1*y0) + ERR;
      temp_delta_0 = x1*y2-x2*y1;
      temp_delta_1 = y1-y2;
      temp_delta_2 = x2-x1;

      *(a_out_ptr + 0 + 3*0 + 3*3*i_tri) = temp_delta_0/temp_delta;
      *(a_out_ptr + 0 + 3*1 + 3*3*i_tri) = temp_delta_1/temp_delta;
      *(a_out_ptr + 0 + 3*2 + 3*3*i_tri) = temp_delta_2/temp_delta;

      /*the point t1*/
      temp_idx_t0 = (int)(*(t_in_ptr + i_tri + 1*N_tri)-1);
      temp_idx_t1 = (int)(*(t_in_ptr + i_tri + 0*N_tri)-1);
      temp_idx_t2 = (int)(*(t_in_ptr + i_tri + 2*N_tri)-1);
      

      x0 = *(p_in_ptr + temp_idx_t0 + 0*N_nodes);y0 = *(p_in_ptr + temp_idx_t0 + 1*N_nodes); 
      x1 = *(p_in_ptr + temp_idx_t1 + 0*N_nodes);y1 = *(p_in_ptr + temp_idx_t1 + 1*N_nodes); 
      x2 = *(p_in_ptr + temp_idx_t2 + 0*N_nodes);y2 = *(p_in_ptr + temp_idx_t2 + 1*N_nodes);

      temp_delta = (x1*y2-x2*y1) + (x0*y1-x0*y2) + (x2*y0-x1*y0) + ERR;
      temp_delta_0 = x1*y2-x2*y1;
      temp_delta_1 = y1-y2;
      temp_delta_2 = x2-x1;

      *(a_out_ptr + 1 + 3*0 + 3*3*i_tri) = temp_delta_0/temp_delta;
      *(a_out_ptr + 1 + 3*1 + 3*3*i_tri) = temp_delta_1/temp_delta;
      *(a_out_ptr + 1 + 3*2 + 3*3*i_tri) = temp_delta_2/temp_delta;

      /*the point t2*/
      temp_idx_t0 = (int)(*(t_in_ptr + i_tri + 2*N_tri)-1);
      temp_idx_t1 = (int)(*(t_in_ptr + i_tri + 0*N_tri)-1);
      temp_idx_t2 = (int)(*(t_in_ptr + i_tri + 1*N_tri)-1);
      

      x0 = *(p_in_ptr + temp_idx_t0 + 0*N_nodes);y0 = *(p_in_ptr + temp_idx_t0 + 1*N_nodes); 
      x1 = *(p_in_ptr + temp_idx_t1 + 0*N_nodes);y1 = *(p_in_ptr + temp_idx_t1 + 1*N_nodes); 
      x2 = *(p_in_ptr + temp_idx_t2 + 0*N_nodes);y2 = *(p_in_ptr + temp_idx_t2 + 1*N_nodes);

      temp_delta = (x1*y2-x2*y1) + (x0*y1-x0*y2) + (x2*y0-x1*y0) + ERR;
      temp_delta_0 = x1*y2-x2*y1;
      temp_delta_1 = y1-y2;
      temp_delta_2 = x2-x1;

      *(a_out_ptr + 2 + 3*0 + 3*3*i_tri) = temp_delta_0/temp_delta;
      *(a_out_ptr + 2 + 3*1 + 3*3*i_tri) = temp_delta_1/temp_delta;
      *(a_out_ptr + 2 + 3*2 + 3*3*i_tri) = temp_delta_2/temp_delta;
    }
}
  



 