#include<stdio.h>
#include<math.h>

/*Change the following line based on the PATH/TO/MEX.H on your system*/
#include"mex.h"

/*Input Arguments*/
#define CELLS_IN         prhs[0]
#define P_IN             prhs[1]
#define T_IN             prhs[2]
#define FIXED_PARAMS     prhs[3]

/*Output Arguments*/
#define TC_IDX_OUT       plhs[0]


#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


void compute_max_triangle_adjacency(double *t_in_ptr, double *p_in_ptr, int N_nodes, int N_tri,
				    int LENGTH, int WIDTH, int N_cells, int *MAX_ADJ_TRI)
{

  int i_tri, i_nodes, i_cells;
  int temp_idx_t[3];
  double temp_x[3][2];
  double xleft, yleft, xright, yright;
  int x, y;
  int temp_adj_tri[N_cells];
  int temp_i, temp_j;


  *MAX_ADJ_TRI = 0;

  for (i_cells = 0;i_cells <= N_cells;i_cells++)
    {
      temp_adj_tri[i_cells] =  0;
    }


  for (i_tri = 0;i_tri < N_tri;i_tri++)
    {

      temp_idx_t[0] = *(t_in_ptr + 0*N_tri + i_tri)-1;
      temp_idx_t[1] = *(t_in_ptr + 1*N_tri + i_tri)-1;
      temp_idx_t[2] = *(t_in_ptr + 2*N_tri + i_tri)-1;

      temp_x[0][0] = *(p_in_ptr + 0*N_nodes + temp_idx_t[0]);
      temp_x[0][1] = *(p_in_ptr + 1*N_nodes + temp_idx_t[0]);

      temp_x[1][0] = *(p_in_ptr + 0*N_nodes + temp_idx_t[1]);
      temp_x[1][1] = *(p_in_ptr + 1*N_nodes + temp_idx_t[1]);

      temp_x[2][0] = *(p_in_ptr + 0*N_nodes + temp_idx_t[2]);
      temp_x[2][1] = *(p_in_ptr + 1*N_nodes + temp_idx_t[2]);

      xleft  = max(floor(min(min(temp_x[0][0],temp_x[1][0]),temp_x[2][0])),1);
      xright = min(ceil(max(max(temp_x[0][0],temp_x[1][0]),temp_x[2][0])),WIDTH);

      yleft  = max(floor(min(min(temp_x[0][1],temp_x[1][1]),temp_x[2][1])),1);
      yright = min(ceil(max(max(temp_x[0][1],temp_x[1][1]),temp_x[2][1])),LENGTH);


      for(x=xleft;x<=xright;x++)
	{
	  for(y=yleft;y<=yright;y++)
	    {
	      temp_i = y-1;temp_j = x-1;

	      temp_adj_tri[temp_i + temp_j*LENGTH] =  
		temp_adj_tri[temp_i + temp_j*LENGTH] + 1;
	    }
	}
    }


  for (i_cells = 0;i_cells < N_cells;i_cells++)
    {
      if (temp_adj_tri[i_cells] > *MAX_ADJ_TRI)
	{
	  *MAX_ADJ_TRI = temp_adj_tri[i_cells]; 
	}
    }
}


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  int N_cells;int N_tri;int N_nodes, LENGTH, WIDTH;
  double *cells_in_ptr, *p_in_ptr, *t_in_ptr, *tc_idx_out_ptr;
  double *params_ptr; 
  int cell_idx;
  int MAX_ADJ_TRI;
  int xleft, yleft, xright, yright;
  int i_tri, i_nodes, i_cells;
  int temp_idx_t[3];
  double temp_x[3][2];
  int x, y;
  int temp_i, temp_j;
  int temp_i_adj_tri;
  double *i_adj_tri;

  /*Get the input points, nodes, triangles*/
  cells_in_ptr      = mxGetPr(CELLS_IN);
  p_in_ptr          = mxGetPr(P_IN);
  t_in_ptr          = mxGetPr(T_IN);
  
  /*Get the dimensions*/
  params_ptr  = mxGetPr(FIXED_PARAMS);
  N_cells     = (int)*(params_ptr);
  N_nodes     = (int)*(params_ptr+1);
  N_tri       = (int)*(params_ptr+2);
  LENGTH      = (int)*(params_ptr+3);
  WIDTH       = (int)*(params_ptr+4);
  
  i_adj_tri = mxGetPr(mxCreateDoubleMatrix(N_cells,1,mxREAL));

  compute_max_triangle_adjacency(t_in_ptr,p_in_ptr,N_nodes,N_tri,LENGTH,
				 WIDTH,N_cells,&MAX_ADJ_TRI);
  
  /*Create TC_IDX_OUT matrix*/
  TC_IDX_OUT = mxCreateDoubleMatrix(N_cells,MAX_ADJ_TRI,mxREAL);
  tc_idx_out_ptr = mxGetPr(TC_IDX_OUT);

  for(i_cells = 0;i_cells < N_cells;i_cells++)
    {
      i_adj_tri[i_cells] = 0;
    }  

  for (i_cells = 0;i_cells <= N_cells;i_cells++)
    {
      for (temp_i_adj_tri = 0;temp_i_adj_tri < MAX_ADJ_TRI;temp_i_adj_tri++)
	{
	  *(tc_idx_out_ptr + i_cells + temp_i_adj_tri*N_cells) =  0;
	}
    }


  for (i_tri = 0;i_tri < N_tri;i_tri++)
    {
      
      temp_idx_t[0] = *(t_in_ptr + 0*N_tri + i_tri)-1;
      temp_idx_t[1] = *(t_in_ptr + 1*N_tri + i_tri)-1;
      temp_idx_t[2] = *(t_in_ptr + 2*N_tri + i_tri)-1;

      temp_x[0][0] = *(p_in_ptr + 0*N_nodes + temp_idx_t[0]);
      temp_x[0][1] = *(p_in_ptr + 1*N_nodes + temp_idx_t[0]);

      temp_x[1][0] = *(p_in_ptr + 0*N_nodes + temp_idx_t[1]);
      temp_x[1][1] = *(p_in_ptr + 1*N_nodes + temp_idx_t[1]);

      temp_x[2][0] = *(p_in_ptr + 0*N_nodes + temp_idx_t[2]);
      temp_x[2][1] = *(p_in_ptr + 1*N_nodes + temp_idx_t[2]);

      xleft  = max(floor(min(min(temp_x[0][0],temp_x[1][0]),temp_x[2][0])),1);
      xright = min(ceil(max(max(temp_x[0][0],temp_x[1][0]),temp_x[2][0])),WIDTH);

      yleft  = max(floor(min(min(temp_x[0][1],temp_x[1][1]),temp_x[2][1])),1);
      yright = min(ceil(max(max(temp_x[0][1],temp_x[1][1]),temp_x[2][1])),LENGTH);


      for(x=xleft;x<=xright;x++)
	{
	  for(y=yleft;y<=yright;y++)
	    {
	      temp_i = y-1;temp_j = x-1;
	      cell_idx = temp_i + LENGTH*temp_j;

	      *(tc_idx_out_ptr + cell_idx + (int) i_adj_tri[cell_idx]*N_cells) = i_tri + 1;
	      i_adj_tri[cell_idx] = i_adj_tri[cell_idx] + 1;
	    }
	}
    }

}
  



 