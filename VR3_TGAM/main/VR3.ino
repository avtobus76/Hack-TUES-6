#include "Common.h"

void _initVR3()
{
   myVR.begin(9600); 
}

void _clr()
{
  int res;
  do{
    res = myVR.clear();
  }
  while(res != 0);
}

void _record()
{
  _clr();
  
  for(int com = 0; com < CMD_NUM; com++)
  {
    do{
      myVR.train(com, stat);
    }
    while(stat[4] != 0);
  }
  
  for(int com = 0; com < CMD_NUM; com++)
  {
    do{
      myVR.load(com, stat);
    }
    while(stat[4] != 0);
  }
}
