#include<iostream>
#include<math.h>
#include<ctime>
#include<sys/time.h>
#include<pthread.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include<semaphore.h>
#include<unistd.h>
#include "mesh.h"
using namespace std;





int main()
{
    bool Left_Reqd,Left_Avail;
    bool Right_Reqd,Right_Avail;
    bool Up_Reqd,Up_Avail;
    bool Down_Reqd,Down_Avail;
    int x,y;
    cout<<"\nMESH TOPOLOGY SIMULATION\n\n\n";
    cout<<"\nEnter Row Size : ";
    cin>>row;
    cout<<"\nEnter Column Size : ";
    cin>>col;
    
	
    pthread_t threads[row][col];
    threadstatus=READY;
 
    struct args *argtab;
    argtab=(struct args *)malloc(sizeof(struct args));
	simactive=true;
       
                MNode=new MeshNode*[row];
                for(int i=0;i<row;i++)
                {
                    MNode[i]=new MeshNode[col];
                    for(int j=0;j<col;j++)
                    {
                        MNode[i][j].x=i;
                        MNode[i][j].y=j;
                        for(int k=0;k<4;k++)
                        {
                            MNode[i][j].interfaces[k].Connected_Channel=NULL;
                            MNode[i][j].interfaces[k].Interface_Active=false;
                            MNode[i][j].interfaces[k].Channel_Operation=NONE;
                        }
                    }
                }
                list=new Channel[row*col];
                channel_count=0;
                cout<<"Creating connections\n";
                for(int i=0;i<row;i++)
                {
                    for(int j=0;j<col;j++)
                    {
                        Left_Reqd=true;
                        Right_Reqd=true;
                        Up_Reqd=true;
                        Down_Reqd=true;
                        if(j==0)
                        {
                            Left_Reqd=false;
                            Left_Avail=false;
                        }
                        if(i==0)
                        {
                            Up_Reqd=false;
                            Up_Avail=false;
                        }
                        if(j==col-1)
                        {
                            Right_Reqd=false;
                            Right_Avail=false;
                        }
                        if(i==row-1)
                        {
                            Down_Reqd=false;
                            Down_Avail=false;
                        }
                        if(Left_Reqd)
                        {
                            MNode[i][j].interfaces[LEFT].Connected_Channel=MNode[i][j-1].interfaces[RIGHT].Connected_Channel;
                        }
                        if(Right_Reqd)
                        {
                            list[channel_count].REQ=false;
                            list[channel_count].status=EMPTY;
                            list[channel_count].ACK=false;
                            list[channel_count].DATA=false;
                            list[channel_count].CLK=false;
                            list[channel_count].CHOKE=false;
                            list[channel_count].Busy_Bit=false;
                            list[channel_count].Receive_Bit=false;
                            list[channel_count].Transfer_Bit=false;
                            MNode[i][j].interfaces[RIGHT].Connected_Channel=&list[channel_count];
                            channel_count++;
                        }
                        if(Up_Reqd)
                        {
                            MNode[i][j].interfaces[UP].Connected_Channel=MNode[i-1][j].interfaces[DOWN].Connected_Channel;
                        }
                        if(Down_Reqd)
                        {
                            list[channel_count].REQ=false;
                            list[channel_count].status=EMPTY;
                            list[channel_count].ACK=false;
                            list[channel_count].DATA=false;
                            list[channel_count].CLK=false;
                            list[channel_count].CHOKE=false;
                            list[channel_count].Busy_Bit=false;
                            list[channel_count].Receive_Bit=false;
                            list[channel_count].Transfer_Bit=false;
                            MNode[i][j].interfaces[DOWN].Connected_Channel=&list[channel_count];
                            channel_count++;
                        }
                    }
                }
                for(int i=0;i<row;i++)
                    for(int j=0;j<col;j++)
                    {
                        while(threadstatus!=READY);
                        threadstatus=INPROGRESS;
                        argtab->a=i+1;
                        argtab->b=j+1;
                        //cout<<"\nInitiating "<<argtab->a<<","<<argtab->b<<"\n";
                        pthread_create(&threads[i][j],0,call_func,(void *)argtab);
                        for(int k=0;k<4;k++)
                        {
                            MNode[i][j].interfaces[k].Interface_Active=false;
                            MNode[i][j].interfaces[k].Channel_Operation=NONE;
                            if(MNode[i][j].interfaces[k].Connected_Channel!=NULL)
                            {
                                MNode[i][j].interfaces[k].Connected_Channel->REQ=false;
                                MNode[i][j].interfaces[k].Connected_Channel->status=EMPTY;
                            }
                        }
                    }

              int sr,sc,dr,dc;

              cout<<"\nEnter Source Node Details : ";
              cout<<"\nx : ";
              cin>>sr;
              cout<<"\ny : ";
              cin>>sc;
              cout<<"\nEnter Destination Details : ";
              cout<<"\nx : ";
              cin>>dr;
              cout<<"\ny : ";
              cin>>dc;

               MNode[sr][sc].Acquire_Data_Packet(dr,dc);
               while(loop);
		delete MNode;
		delete list;
            
    
	simactive=false;
    return 0;   
}
















