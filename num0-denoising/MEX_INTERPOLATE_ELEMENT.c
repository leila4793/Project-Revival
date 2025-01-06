#include<stdio.h>
#include<math.h>

/*Change the following line based on the PATH/TO/MEX.H on your system*/
#include"mex.h"


/*Input Arguments*/
#define X_IN         prhs[0]
#define XT_IDX_IN    prhs[1]
#define FP_IN        prhs[2]
#define T_IN         prhs[3]
#define A_IN         prhs[4]
#define FIXED_PARAMS prhs[5]

/*Output Arguments*/
#define FX_OUT       plhs[0]

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  int N_pts;int N_tri;int N_nodes;
  double *x_in_ptr, *xt_idx_in_ptr, *fp_in_ptr, *t_in_ptr, *a_in_ptr;
  double *fx_out_ptr;
  double *params_ptr; 
  int  i_pts, i_tri, i, j;
  int temp_idx_t[3];
  double temp_f[3];
  double temp_x[2];
  double temp_a[3][3];

  /*Get the input points, nodes, triangles*/
  x_in_ptr      = mxGetPr(X_IN);
  xt_idx_in_ptr = mxGetPr(XT_IDX_IN);
  fp_in_ptr     = mxGetPr(FP_IN);
  t_in_ptr      = mxGetPr(T_IN);
  a_in_ptr      = mxGetPr(A_IN);

  /*Get the dimensions*/
  params_ptr = mxGetPr(FIXED_PARAMS);
  N_pts   = (int)*(params_ptr);
  N_nodes = (int)*(params_ptr+1);
  N_tri   = (int)*(params_ptr+2);

    
  /*Create FX_OUT matrix*/
  FX_OUT = mxCreateDoubleMatrix(N_pts,1,mxREAL);
  fx_out_ptr = mxGetPr(FX_OUT);

  
  for(i_pts = 0;i_pts < N_pts;i_pts++)
    {
      if(xt_idx_in_ptr[i_pts] != -1)
	{
	  /*the triangle t0-t1-t2*/
	  i_tri = (int)(xt_idx_in_ptr[i_pts]-1); 
	  
	  /*function values the nodes t0, t1, t2*/
	  temp_idx_t[0] = (int)(*(t_in_ptr + i_tri + 0*N_tri)-1);
	  temp_idx_t[1] = (int)(*(t_in_ptr + i_tri + 1*N_tri)-1);
	  temp_idx_t[2] = (int)(*(t_in_ptr + i_tri + 2*N_tri)-1);
	  
	  temp_f[0] = fp_in_ptr[temp_idx_t[0]];
	  temp_f[1] = fp_in_ptr[temp_idx_t[1]];
	  temp_f[2] = fp_in_ptr[temp_idx_t[2]];
	 
	  /*basis function parameters at nodes t0, t1, t2*/
	  for (i=0;i<3;i++)
	    {
	      for (j=0;j<3;j++)
		{
		  temp_a[i][j] = *(a_in_ptr + i + j*3 + 3*3*i_tri);
		}
	    }
	  
	  /*the point*/
	  temp_x[0] = *(x_in_ptr + i_pts + N_pts*0);
	  temp_x[1] = *(x_in_ptr + i_pts + N_pts*1);
	  
	  /*interpolate and compute the value at (x_in[i_pts][0], x_in[i_pts][1])*/
	  fx_out_ptr[i_pts] = temp_f[0]*(temp_a[0][0] + temp_a[0][1]*temp_x[0] + temp_a[0][2]*temp_x[1]) + 
	    temp_f[1]*(temp_a[1][0] + temp_a[1][1]*temp_x[0] + temp_a[1][2]*temp_x[1]) + 
	    temp_f[2]*(temp_a[2][0] + temp_a[2][1]*temp_x[0] + temp_a[2][2]*temp_x[1]);
	}
      else
	{
	  fx_out_ptr[i_pts] = 0;
	}

    }

}
  



 