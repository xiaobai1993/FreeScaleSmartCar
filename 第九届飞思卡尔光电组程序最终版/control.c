#include "all.h"
#define L_B 8// 11       
#define R_B  116   //116 
int s_track=30;//直道线宽度 
//extern int set_speed;

extern int start;
char stop_count=0; 
       
extern unsigned char PixelAverage(unsigned char len, unsigned char *data);

extern int g_line_old;
extern int g_line_new;

int b_right_last=0;
int b_left_last=0;

int b_left=0;
int b_right=0;
int b_left_last_flag=0;
int b_right_last_flag=0;
int b_left_flag=0;
int b_right_flag=0;
 /*********ccd2***************/
/*********************************/

int start_check(unsigned char *A,int interval,int yuzhi)
{
	int i,tiaobian=0;
	int last_tiaobian=0;
	int this_tiaobian=0;

	for(i=35;i<95;i++)
	{
		if(A[i]-A[i+interval]>yuzhi||A[i]-A[i+interval]<-yuzhi)
		{
		    this_tiaobian=i;
		    if(this_tiaobian!=last_tiaobian+1) 
		    {
		          tiaobian++;     
		    }       
		    last_tiaobian=this_tiaobian;
		}
   
	}
	 return tiaobian;
}
	/***********************************************************/	
float Calculate_line()
{
  float track;
  
  /******************************************/
  //new add
  if(b_left_last_flag==1&&b_left_flag==1) 
  {
     if(b_left-b_left_last>7||b_left-b_left_last<-7) 
     {
     
         b_left_flag=0;
     }
      
  }
  if(b_right_last_flag==1&&b_right_flag==1) 
  {
     if(b_right-b_right_last>7||b_right-b_right_last<-7) 
     {
     
         b_right_flag=0;
     }
      
  }
  /*********************************************************/  
  if(b_left_last_flag==1&&b_right_last_flag==0) 
  {
       track=g_line_old-b_left_last;
  }
  else if(b_left_last_flag==0&&b_right_last_flag==1) 
  {
       track=b_right_last-g_line_old;
      
  } 
  else if(b_left_last_flag==1&&b_right_last_flag==1)
  {
      s_track=(b_right_last-b_left_last)/2; 
      track=s_track;  
  } 
  else 
  {
       track=s_track;
  }
  if(b_left_flag==0&&b_right_flag==0)
	{
	    PTM_PTM2=0;
	    PTM_PTM5=0;
	    /*if(b_left_last_flag==1&&b_right_last_flag==0) 
	    {
	        if(fabs(b_left_last-40)<10)
	        {
	           return Middle;
	        } 
	        else
	        {
	            return g_line_old;
	        }
	    }
	    else if(b_left_last_flag==0&&b_right_last_flag==1) 
	    {
	        if(fabs(b_right_last-90)<10)
	        {
	           return Middle;
	        } 
	        else
	        {
	            return g_line_old;
	        }
	    } 
	    else if(b_left_last_flag==0&&b_right_last_flag==0) 
	    {
	         g_line_old;
	    } 
	    else if(b_left_last_flag==1&&b_right_last_flag==1) 
	    {
	       return Middle;
	    }  */
	    /*if(b_right_last>R_B-30&&b_left_last>L_B+30) 
	    {
	        return Middle;
	    } 
	    else if(b_right_last<R_B-30&&b_left_last<L_B+30)
	    {
	           return Middle;
	    } 
	    else
	    {
	         return g_line_old;
	    } */
	   return Middle;//*0.2+g_line_old*0.8;
	       
	}
  else if(b_left_flag==1&&b_right_flag==1)
  {
      PTM_PTM2=1;
	    PTM_PTM5=1;
	    /*if(b_right-b_left<30)
	    {
	         b_left_flag=0;
	         b_right_flag=0;
	         return Middle;
	    } 
	    else */
	    {
	         return(b_left+b_right)/2;
	    }
	       
  }
	else if(b_left_flag==1&&b_right_flag==0)
	{
	    PTM_PTM2=1;
	    PTM_PTM5=0;
	    if(b_left_last_flag==1)
	    {
	       return b_left+track;
	    }
	    else 
	    {
	       return g_line_old;
	    }
	  
	} 
	else if(b_left_flag==0&&b_right_flag==1)
  {    
      PTM_PTM2=0;
	    PTM_PTM5=1;
	    
	    if(b_right_last_flag==1)
	    {
	        return b_right-track;
	    }
	    else
	    {
	        return g_line_old;
	    }
	  
	    
  }
	
}

void find(unsigned char *s,int width,int interval,int yuzhi)
{
	int i=0,j=0;
 static int left_min=0;
 static int left_max=0;
 static int right_min=0;
 static int right_max=0;
	
	b_right_flag=0;
	b_left_flag=0;
	if(b_left_last_flag==0&&b_right_last_flag==0)
	{
	    //b_right_flag=-1;
	    //b_left_flag=-1;
		  left_min=L_B;
	    right_max=R_B;
		  right_min=left_max=Middle;
	} 
	else
  {
	  
    if(b_right_last_flag==1)
  	{
  		right_min=b_right_last-width;
  		right_max=b_right_last+width;
  	}
  	/*else if(b_left_last_flag==1&&b_right_last_flag==0)
  	{
  		right_min=b_left_last+width;
  		right_max=R_B;//b_left_last+TRACK+width;
  	}*/ 
    if(b_left_last_flag==1)
  	{
  		left_min=b_left_last-width;
  		left_max=b_left_last+width;
  	}
  /*	else if(b_left_last_flag==0&&b_right_last_flag==1)
  	{
  		left_min=L_B;//b_right_last-TRACK-width;
  		left_max=b_right_last-width-20;//b_right_last-TRACK+width;
  	}  */
  }
  
	if(left_min<L_B)
	{
		left_min=L_B;
	}
	if(right_min<L_B)
	{
		right_min=L_B;
	}
	if(left_max>R_B)
	{
		left_max=R_B;
	}
	if(right_max>R_B)
	{
		right_max=R_B;
	}
	for(i=left_max;i-interval-1>=left_min;i--)
	{
		if(s[i]-s[i-interval]>yuzhi&&s[i]-s[i-interval-1]>yuzhi)
		{
			b_left=i-interval;
			b_left_flag=1;
			break;
		}
	}
	for(j=right_min;j+interval+1<=right_max;j++)
	{
		if(s[j]-s[j+interval]>yuzhi&&s[j]-s[j+interval+1]>yuzhi)
		{
			b_right=j+interval;
			b_right_flag=1;
			break;
		}
	}
	/*if(b_left_last_flag==0&&b_right_last_flag==0)
  {
        if(b_left_flag==1&&b_right_flag==1) 
        {
          s_track=(b_right-b_left)/2;
        }
        else
        {
            b_left_flag=-1;
            b_right_flag=-1;
        }          
  }*/

}
void CCD_P2(unsigned char *s,int interval,int yuzhi)
{
  int i,j;
  int lines=0;
  b_left_flag=0;
  b_right_flag=0;
  
    
  if(b_right_last_flag!=0||b_left_last_flag!=0) 
  {
  
    /*if(b_right_last_flag==1&&b_left_last_flag==1) 
    {
        lines=Middle;     
    }
    else */
    {
      
      if(g_line_old>=L_B&&g_line_old<R_B) 
      {
            lines=g_line_old;
      }
      else if(g_line_old<L_B)
      {
          lines=L_B;
      }
      else if (g_line_old>R_B)
      {  
            lines=R_B;
      } 
    }
  } 
  else
  {
      lines=Middle;
  }
 	for(i=lines;i-interval-1>=L_B;i--)
	{
		if(s[i]-s[i-interval]>yuzhi&&s[i-1]-s[i-interval-1]>yuzhi)
		{
			b_left=i-interval;
			b_left_flag=1;
			break;
		}
	}
	for(j=lines;j+interval+1<=R_B;j++)
	{
		if(s[j]-s[j+interval]>yuzhi&&s[j+1]-s[j+interval+1]>yuzhi)
		{
			b_right=j+interval;
			b_right_flag=1;
			break;
		}
	}
 
}

















