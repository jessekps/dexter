#include<math.h>
#include<R.h>
#include<Rmath.h>
#include<Rdefines.h>
#include<Rinternals.h>
#include "roger.h"


// sample responses to one item. Not adapted to include null category
void sampleNRM0(double *theta, double *b, int *a, int *i, int *first, int *last, int *m, int *response)
{
  double *p=NULL, u;
  int k;
  GetRNGstate(); // get seed
  void *_p = realloc(p, ((last[i[0]]-first[i[0]]+2) * sizeof(double)));
  p=(double*)_p;
  for (int pers=0;pers<m[0];pers++)
  {
    p[0]=1;
    k=1;
    for (int j=first[i[0]];j<=last[i[0]];j++)
    {
      p[k]=p[k-1]+b[j]*exp(a[j]*theta[pers]);
      k++;
    }
    u=p[k-1]*runif(0,1);
    k=0;
    while (u>p[k]) {k++;}
    response[pers]=k;
  }
  PutRNGstate(); // put seed
  free(p);
}

// sample responses to one item adapted for use with b_0 and a_0 in
void sampleNRM(double *theta, double *b, int *a, int *i, int *first, int *last, int *m, int *response)
{
  double *p=NULL, u;
  int k;
  GetRNGstate(); // get seed
  void *_p = realloc(p, ((last[i[0]]-first[i[0]]+2) * sizeof(double)));
  p=(double*)_p;
  for (int pers=0;pers<m[0];pers++)
  {
    p[0]=b[first[i[0]]]*exp(a[first[i[0]]]*theta[pers]); 
    k=1;
    for (int j=first[i[0]]+1;j<=last[i[0]];j++)
    {
      p[k]=p[k-1]+b[j]*exp(a[j]*theta[pers]);
      k++;
    }
    u=p[k-1]*runif(0,1);
    k=0;
    while (u>p[k]) {k++;}
    response[pers]=k;
  }
  PutRNGstate(); // put seed
  free(p);
}

void PV0(double *b, int *a, int *first, int *last, double *mu, double *sigma, int *score, int *pop, int *nP,int *nI, int *nPop, double *theta)
{
  double atheta=0.0;
	int x;
	double *ptheta;
 	ptheta=&atheta;
	double *p=NULL, u;
	int maxS=0, k=0;
 	GetRNGstate(); // get seed
	for (int i=0;i<nI[0];i++) {if ((last[i]-first[i]+2)>maxS) {maxS=(last[i]-first[i]+2);}}
	void *_p = realloc(p, ((maxS+1) * sizeof(double)));
	p=(double*)_p;
	for (int person=0;person<nP[0];person++)
	{
		x=-1;
    while (x!=score[person])
		{
			atheta=rnorm(mu[pop[person]],sigma[pop[person]]);
			x=0;
			for (int i=0;i<nI[0];i++)
			{
				p[0]=1;
				k=1;
				for (int j=first[i];j<=last[i];j++)
				{
					p[k]=p[k-1]+b[j]*exp(a[j]*atheta);
					k++;
				}
				u=p[k-1]*runif(0,1);
				k=0;
				while (u>p[k]) {k++;}
				if (k>0) {x+=a[first[i]+k-1];}
			}
		}
		theta[person]=atheta;
	}
  free(p);
 	PutRNGstate(); // put seed
}


// Adapted by timo for inclusion of zero category
/*
void PV1(double *b, int *a, int *first, int *last, double *mu, double *sigma, int *score, int *pop, int *nP,int *nI, int *nPop, double *theta)
{
  double atheta=0.0;
  int x;
  double *ptheta;
  ptheta=&atheta;
  double *p=NULL, u;
  int maxS=0, k=0;
  GetRNGstate(); // get seed
  for (int i=0;i<nI[0];i++) {if ((last[i]-first[i]+2)>maxS) {maxS=(last[i]-first[i]+2);}}
  void *_p = realloc(p, ((maxS+1) * sizeof(double)));
  p=(double*)_p;
  for (int person=0;person<nP[0];person++)
  {
    x=-1;
    while (x!=score[person])
    {
      atheta=rnorm(mu[pop[person]],sigma[pop[person]]);
      x=0;
      for (int i=0;i<nI[0];i++)
      {
        p[0]=b[first[i]]*exp(a[first[i]]*atheta); 
        k=1;
        for (int j=first[i]+1;j<=last[i];j++) // note the +1
        {
          p[k]=p[k-1]+b[j]*exp(a[j]*atheta);
          k++;
        }
        u=p[k-1]*runif(0,1);
        k=0;
        while (u>p[k]) {k++;}
        if (k>0) {x+=a[first[i]+k];} // note that -1 has been deleted
      }
    }
    theta[person]=atheta;
  }
  free(p);
  PutRNGstate(); // put seed
}
 */

// Adapted by timo for inclusion of zero category
// produces nPV plausible values per score
void PV(double *b, int *a, int *first, int *last, double *mu, double *sigma, int *score, int *pop, int *nP, int *nI, int *nPop, int *nPV, double *theta)
{
  double atheta=0.0;
  int x;
  double *ptheta;
  ptheta=&atheta;
  double *p=NULL, u;
  int maxS=0, k=0;
  GetRNGstate(); // get seed
  for (int i=0;i<nI[0];i++) {if ((last[i]-first[i]+2)>maxS) {maxS=(last[i]-first[i]+2);}}
  void *_p = realloc(p, ((maxS+1) * sizeof(double)));
  p=(double*)_p;
  for (int t=0;t<nPV[0];t++)
  {
    for (int person=0;person<nP[0];person++)
    {
      x=-1;
      while (x!=score[person])
      {
        atheta=rnorm(mu[pop[person]],sigma[pop[person]]);
        x=0;
        for (int i=0;i<nI[0];i++)
        {
          p[0]=b[first[i]]*exp(a[first[i]]*atheta); 
          k=1;
          for (int j=first[i]+1;j<=last[i];j++) // note the +1
          {
            p[k]=p[k-1]+b[j]*exp(a[j]*atheta);
            k++;
          }
          u=p[k-1]*runif(0,1);
          k=0;
          while (u>p[k]) {k++;}
          if (k>0) {x+=a[first[i]+k];} // note that -1 has been deleted
        }
      }
      theta[t*nP[0]+person]=atheta;
    }
  }
  free(p);
  PutRNGstate(); // put seed
}

void Escore(double *theta, double *score, double *b, int *a, int *first, int *last, int *n)
{
  double denom, num;
  score[0]=0.0;
  for (int i=0; i<n[0]; i++)
  {
    num=0.0;
    denom=1.0;
    for (int j=first[i]+1;j<=last[i];j++) // note +1
    {
      num  +=a[j]*b[j]*exp(a[j]*theta[0]);
      denom+=     b[j]*exp(a[j]*theta[0]);
    }
    score[0]+=num/denom;
  }
}
