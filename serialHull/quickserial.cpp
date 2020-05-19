#include<bits/stdc++.h>
using namespace std;
class Points{
public:
	int a,b;
};
pair < int ,  int >  parallel_max_min(vector < Points > &P, int n){
	int i;
	int min_x=0;
	int max_x =0;
	for(int i = 0 ; i < n ; i++)
	{
			 if(P[max_x].a < P[i].a){
			 		max_x=i;
			 }
	}
		
	for(int i = 0 ; i < n ; i++){
    		if(P[min_x].a > P[i].a){
			 		min_x=i;
			 }
		} 
	//cout << min_x << " " << max_x << "++\n";
	return  {min_x , max_x};
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
void quickHull(vector < Points > &P   , int n , Points l ,Points r , int side){
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

	quickHull(P, n, P[ind], l, -findSide(P[ind], l, r)); 
	quickHull(P, n, P[ind], r, -findSide(P[ind], r, l));
 
	return;
}
void hullInternal(vector < Points > &P, int n) {
  pair<int , int > minMax = parallel_max_min(P,n);
  int l = minMax.first;
  int r = minMax.second;
  quickHull(P,n,P[l],P[r],1);
	quickHull(P,n,P[l],P[r],-1);
				
}
vector < Points > point;
using namespace std::chrono; 
int main(){
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

	 cout << "serial program--\n";
	hullInternal(point,n);
	 auto stop = high_resolution_clock::now(); 
	 auto duration = duration_cast<microseconds>(stop - start);
	 /*
	for(auto i: ans) {
		cout << i.a << " " << i.b << "***\n";
	}*/
	 cout << "Time taken by function: "
         << duration.count() << " microseconds\n";
	return 0;
}