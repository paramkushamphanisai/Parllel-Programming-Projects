kernel void ArrayMult( global const float *dA, global const float *dB, local float *prods, global float *dC)
{
	int ni = get_local_size( 0 );
	int lid = get_local_id( 0 );
	int numwg  = get_group_id( 0 );
	int gid = get_global_id( 0 );
	prods[ lid ] =  dA[ gid ] * dB[ gid ];
	for( int i = 1; i < ni; i *= 2 )
	{
		int t = 2*i - 1;
		barrier( CLK_LOCAL_MEM_FENCE );  
		if((lid & t) == 0)		
		{								
			prods[lid] += prods[lid + i];
		}
	}
	barrier( CLK_LOCAL_MEM_FENCE );
	if( lid == 0 ){
		dC[ numwg ] = prods[0];
	}
}
