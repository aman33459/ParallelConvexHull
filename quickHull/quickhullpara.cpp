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
#define DEPTH 10
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
	cout << "done\n";
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
void serialHull(vector < Points > &P   , int n , Points l ,Points r , int side , vector < Points > &ans){
	int ind =-1;
	int max_dist = 0; 
	for(int i=0; i<n; i++) 
    { 
        int temp = lineDist(l, r, P[i]);
        int k =  findSide(l ,  r, P[i]);
        
        	if( k == side && temp > max_dist) 
	        { 
	            ind = i; 
	            max_dist = temp; 
	        }
	}	    	
	//cout << l.a << " " << l.b << "++\n";
	if (ind == -1) 
	 {
	 	///cout << omp_get_thread_num() << "++\n"; 
       	ans.push_back(l); 
        ans.push_back(r); 
        ///cout << l.a << " " << l.b << " " << r.a << " " << r.b << "++\n";
        return; 
	 }

	serialHull(P, n, P[ind], l, -findSide(P[ind], l, r),ans); 
	serialHull(P, n, P[ind], r, -findSide(P[ind], r, l),ans);
 
	return;
}

void quickHull(vector < Points > &P   , int n , Points l ,Points r , int side , vector < Points > & ans , int depth){
	if(depth  == 0) 
	{
			serialHull(P,n,l,r,side , ans);
	}
	int max_dist = 0;
	int ind = -1;
	
	#pragma omp parallel shared(ind,max_dist,P,side,l,r,n)
	{
		#pragma omp for schedule(dynamic) 
			for(int i=0; i<n; i++) 
		    { 
		    	int tno = omp_get_thread_num();
		        int temp = lineDist(l, r, P[i]);
		        int k =  findSide(l ,  r, P[i]);
		        if(k == side && temp > max_dist){
		        	max_dist=temp;
		        	ind = i;
		        }
	    } 
	}
	if(ind == -1) 
	 {
	 	ans.push_back(l);
	 	ans.push_back(r);
        ///cout << l.a << " " << l.b << " " << r.a << " " << r.b << "++\n";
        return; 
	 }
 #pragma omp task shared(ans)
    {
    	quickHull(P, n, P[ind], l, -findSide(P[ind], l, r),ans,depth-1); 
	}
	#pragma omp task shared(ans)
    {
    	quickHull(P, n, P[ind], r, -findSide(P[ind], r, l),ans,depth-1);
	}
	return;
}
vector < Points >  hullInternal(vector < Points > &P, int n) {
  pair<int , int > minMax = parallel_max_min(P,n);
  int l = minMax.first;
  int r = minMax.second;
  vector < Points > ans;
	#pragma omp task
	{
			quickHull(P,n,P[l],P[r],1,ans,DEPTH);
	}
	#pragma omp task
	{
		quickHull(P,n,P[l],P[r],-1,ans,DEPTH);
	}
	return ans;
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
	vector < Points > ans = hullInternal(point,n);
	 auto stop = high_resolution_clock::now(); 
	 auto duration = duration_cast<microseconds>(stop - start);
/*	for(int tno = 0 ; tno < NUMTHREADS ; tno++){
			for(auto i: ans[tno]) {
			cout << i.a << " " << i.b << "***\n";
		}	
	}*/
	 //for()
	
	 cout << "Time taken by function: "
         << duration.count() << " microseconds\n";
	return 0;
}
