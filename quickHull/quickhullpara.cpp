#include<bits/stdc++.h>
#include"omp.h"
using namespace std;
class Points{
public:
	int a,b;
};
#define CHUNKSIZE 100
#define NUMTHREADS 10
int min_x[10],max_x[10];
pair < int ,  int >  parallel_max_min(vector < Points > &P, int n){
	omp_set_num_threads(NUMTHREADS);
	int i;
	for(int i =0 ; i < NUMTHREADS ; i++){
		min_x[i]=0;
		max_x[i]=0;
	}
	#pragma omp parallel shared(P,n,min_x,max_x)
	{
		#pragma omp for schedule(dynamic) nowait
		for(int i = 0 ; i < n ; i++)
		{
			int tno = omp_get_thread_num();
			//cout << i << " " << tno << "++\n";
			if(P[max_x[tno]].a < P[i].a){
				max_x[tno] =i; 
			}
			/*#pragma omp critical
        	{	
				 if(P[max_x].a < P[i].a){
				 		max_x=i;
				 }
			}*/
		}
		
		#pragma omp for schedule(dynamic) nowait
		for(int i = 0 ; i < n ; i++){
			int tno = omp_get_thread_num();
			if(P[min_x[tno]].a > P[i].a){
				min_x[tno] =i; 
			}
			/*#pragma omp critical
        	{
        		if(P[min_x].a > P[i].a){
				 		min_x=i;
				 }
			}*/
		} 
	}
	int a1 = max_x[0],a2 = min_x[0];
	///for(int i  = 0 ; i < NUMTHREADS ; i++ ) cout << max_x[i] << " " << min_x[i] << "--\n";
 	for(int i =0 ; i < NUMTHREADS ; i++){
 		//cout << P[max_x[i]].a << " " << P[a1].a << " " << a1 << "++\n";
		if(P[min_x[i]].a < P[a2].a){
				a2 =min_x[i]; 
			}
		if(P[max_x[i]].a > P[a1].a){
				a1 = max_x[i]; 
			}
	}
	//cout << a2 << " " << a1 << "++\n";
	return  {a2, a1};
}
int findSide(Points p1, Points p2, Points p) 
{ 
    int val = (p.b - p1.b) * (p2.a - p1.a) - 
              (p2.b - p1.b) * (p.a - p1.a); 
  
    if (val > 0) 
        return 1; 
    if (val < 0) 
        return -1; 
    return 0; 
} 
  
int lineDist(Points p1, Points p2, Points p) 
{ 
    return abs ((p.b - p1.b) * (p2.a - p1.a) - 
               (p2.b - p1.b) * (p.a - p1.a)); 
}
vector < Points > ans;
int ind[10];
int max_dist[10];
void quickHull(vector < Points > &P   , int n , Points l ,Points r , int side){

	for(int i = 0; i < NUMTHREADS ; i++) {
		max_dist[i]=0;
		ind[i]=-1;
	}
	//cout << "ok till now\n" << " " << omp_get_num_threads() << "------\n";
	#pragma omp parallel shared(ind,max_dist,P,side,l,r,n)
	{
		#pragma omp for schedule(dynamic) 
			for(int i=0; i<n; i++) 
		    { 
		    	int tno = omp_get_thread_num();
		        int temp = lineDist(l, r, P[i]);
		        int k =  findSide(l ,  r, P[i]);
		        if(k == side && temp > max_dist[tno]){
		        	max_dist[tno]=temp;
		        	ind[tno] = i;
		        }
		        /*#pragma omp critical
		        {
		        	if( k == side && temp > max_dist) 
			        { 
			            ind = i; 
			            max_dist = temp; 
			        }
		    	}*/
		    } 
	}
	//cout << "something went wrong\n";
	int f = 1;
	int m = 0;
	int indfinal = ind[0];
	for(int i = 0; i < NUMTHREADS ; i++) {
        if(max_dist[m] < max_dist[i]){
        	m= i;
        	indfinal = ind[i];
        }
	}
	if(indfinal == -1) 
	 {
	 	///cout << omp_get_thread_num() << "++\n"; 
        ans.push_back(l); 
        ans.push_back(r); 
        ///cout << l.a << " " << l.b << " " << r.a << " " << r.b << "++\n";
        return; 
	 }
    #pragma omp parallel shared(ans)
 	 {
  		#pragma omp sections
  		{
  				#pragma omp section
  				{
  					quickHull(P, n, P[indfinal], l, -findSide(P[indfinal], l, r)); 
				}
				#pragma omp section
				{
					quickHull(P, n, P[indfinal], r, -findSide(P[indfinal], r, l));
				}
		}
	} 
	return;
}
void hullInternal(vector < Points > &P, int n) {
  pair<int , int > minMax = parallel_max_min(P,n);
  int l = minMax.first;
  int r = minMax.second;
  #pragma omp parallel shared(ans)
  {
  		#pragma omp sections
  		{
  				#pragma omp section
  				{
  					quickHull(P,n,P[l],P[r],1);
				}
				#pragma omp section
				{
					quickHull(P,n,P[l],P[r],-1);
				}
		}
	} 
	//return ans;
}
vector < Points > point;
using namespace std::chrono; 
int main(){
	//cout << "ok\n";
	auto start = high_resolution_clock::now(); 
	int n;
	cin >> n;
	for(int i = 0 ; i < n ; i++){
		int tmp1,tmp2;
		cin >> tmp1 >> tmp2;
		Points ok;
		ok.a=tmp1;
		ok.b=tmp2;
		point.push_back(ok);
	}
	hullInternal(point,n);
	 auto stop = high_resolution_clock::now(); 
	 auto duration = duration_cast<microseconds>(stop - start);
	 
	/*for(auto i: ans) {
		cout << i.a << " " << i.b << "***\n";
	}*/
	 cout << "Time taken by function: "
         << duration.count() << " microseconds\n";
	return 0;
}