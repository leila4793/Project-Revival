#include<stdio.h>
#include<math.h>

/*Change the following line based on the PATH/TO/MEX.H on your system*/
#include"mex.h"

/*Input Arguments*/
#define X_IN         prhs[0]
#define P_IN         prhs[1]
#define T_IN         prhs[2]
#define TC_IDX_IN    prhs[3]
#define FIXED_PARAMS prhs[4]

/*Output Arguments*/
#define XT_IDX_OUT       plhs[0]
#define BARY_COORD_OUT   plhs[1]

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

double dot_prod(double *temp_a, double *temp_b)
{
  double temp_a_dot_b;

  temp_a_dot_b= (temp_a[0] * temp_b[0]) + (temp_a[1] * temp_b[1]);
 
  return(temp_a_dot_b);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  int N_pts;int N_tri;int N_nodes, N, LENGTH, WIDTH;
  double *x_in_ptr, *p_in_ptr, *t_in_ptr, *tc_idx_in_ptr;
  double *params_ptr; 
  double *xt_idx_out_ptr; double *bary_coord_out_ptr;  
  int  i_pts, i_tri, i_adj_tri, i;
  double temp_a[2], temp_b[2], temp_x[2];
  int temp_idx_t1, temp_idx_t2, temp_idx_t3;
  double temp_t1[2], temp_t2[2], temp_t3[2];
  double temp_dot_aa, temp_dot_bb, temp_dot_ax, temp_dot_bx, temp_dot_ab;
  double temp_denm;
  double alpha, beta;
  int cell_idx;
  int MAX_ADJ_TRI;
  double temp_x_coord, temp_y_coord;
  int temp_i, temp_j;

  /*Get the input points, nodes, triangles*/
  x_in_ptr      = mxGetPr(X_IN);
  p_in_ptr      = mxGetPr(P_IN);
  t_in_ptr      = mxGetPr(T_IN);
  tc_idx_in_ptr = mxGetPr(TC_IDX_IN);

  /*Get the dimensions*/
  params_ptr  = mxGetPr(FIXED_PARAMS);
  N_pts       = (int)*(params_ptr);
  N_nodes     = (int)*(params_ptr+1);
  N_tri       = (int)*(params_ptr+2);
  LENGTH      = (int)*(params_ptr+3);
  WIDTH       = (int)*(params_ptr+4);
  MAX_ADJ_TRI = (int)*(params_ptr+5);

  N = LENGTH*WIDTH;
    
  /*Create XT_IDX_OUT matrix*/
  XT_IDX_OUT = mxCreateDoubleMatrix(N_pts,1,mxREAL);
  xt_idx_out_ptr = mxGetPr(XT_IDX_OUT);

  /*Create BARY_COORD_OUT matrix*/
  BARY_COORD_OUT = mxCreateDoubleMatrix(N_pts,3,mxREAL);
  bary_coord_out_ptr = mxGetPr(BARY_COORD_OUT);


  for (i = 0;i <N_pts;i++)
    {
      xt_idx_out_ptr[i] = -1;
    }

 
  for(i_pts = 0;i_pts < N_pts;i_pts++)
    {
      
      temp_x_coord =  *(x_in_ptr + i_pts + 0*N_pts);
      temp_y_coord =  *(x_in_ptr + i_pts + 1*N_pts);
      
      temp_i = floor(temp_y_coord)-1;
      temp_j = floor(temp_x_coord)-1;

      cell_idx = temp_j*LENGTH + temp_i;
      
      if (cell_idx >= 0 && cell_idx <= N-1)
	{
	  for(i_adj_tri = 0;i_adj_tri < MAX_ADJ_TRI;i_adj_tri++)
	    {
	      
	      if(*(tc_idx_in_ptr + i_adj_tri*N + cell_idx)-1 != -1)
		{
		  
		  i_tri = *(tc_idx_in_ptr + i_adj_tri*N + cell_idx)-1;
		  
		  temp_idx_t1 = (int)(*(t_in_ptr + i_tri + 0*N_tri)-1);
		  temp_idx_t2 = (int)(*(t_in_ptr + i_tri + 1*N_tri)-1);
		  temp_idx_t3 = (int)(*(t_in_ptr + i_tri + 2*N_tri)-1);
		  
		  /*the triangle t1-t2-t3*/
		  temp_t1[0] = *(p_in_ptr + temp_idx_t1 + 0*N_nodes);temp_t1[1] = 
								       *(p_in_ptr + temp_idx_t1 + 1*N_nodes); 
		  temp_t2[0] = *(p_in_ptr + temp_idx_t2 + 0*N_nodes);temp_t2[1] = 
								       *(p_in_ptr + temp_idx_t2 + 1*N_nodes); 
		  temp_t3[0] = *(p_in_ptr + temp_idx_t3 + 0*N_nodes);temp_t3[1] = 
								       *(p_in_ptr + temp_idx_t3 + 1*N_nodes);
		  
		  /*vector a*/
		  temp_a[0] = temp_t2[0]-temp_t1[0];temp_a[1] = temp_t2[1]-temp_t1[1];
		  /*vector b*/
		  temp_b[0] = temp_t3[0]-temp_t1[0];temp_b[1] = temp_t3[1]-temp_t1[1];
		  /*vector x*/
		  temp_x[0] = *(x_in_ptr + i_pts + 0*N_pts)-temp_t1[0];
		  temp_x[1] = *(x_in_ptr + i_pts + 1*N_pts)-temp_t1[1];
		  
		  /*compute all the dot products*/
		  temp_dot_aa = dot_prod(&temp_a[0],&temp_a[0]);
		  temp_dot_bb = dot_prod(&temp_b[0],&temp_b[0]);
		  temp_dot_ab = dot_prod(&temp_a[0],&temp_b[0]);
		  temp_dot_ax = dot_prod(&temp_a[0],&temp_x[0]);
		  temp_dot_bx = dot_prod(&temp_b[0],&temp_x[0]);
		  
		  /*now check if the point lies inside the triangle t1-t2-t3*/
		  temp_denm = (temp_dot_aa*temp_dot_bb - temp_dot_ab*temp_dot_ab);
		  alpha = (temp_dot_ax*temp_dot_bb - temp_dot_bx*temp_dot_ab)/temp_denm;
		  beta =  (temp_dot_bx*temp_dot_aa - temp_dot_ax*temp_dot_ab)/temp_denm;
		  
		  if(alpha >= 0 && beta >=0 && alpha+beta <=1)
		    {
		      xt_idx_out_ptr[i_pts] = i_tri+1;
		      *(bary_coord_out_ptr + 0*N_pts + i_pts) = 1-alpha-beta;
		      *(bary_coord_out_ptr + 1*N_pts + i_pts) = alpha;
		      *(bary_coord_out_ptr + 2*N_pts + i_pts) = beta;
		      break;
		    }
		}
	      
	    }
	}
    }
}
  



 