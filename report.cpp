#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char** argv){

	//double total=0.0;

	ofstream subtracted;
	subtracted.open("temp.txt");
	ifstream baseline("out1.txt");
	double bD;
	ifstream ith("m3-10.txt");
	double qD, error, utility;
	if(baseline.is_open() && ith.is_open()){
		while(baseline>>bD && ith>>qD){
			//cout<<bD<<" "<<qD<<'\n';
			error=abs(bD-qD)*double(100)/bD;
			utility=double(100)-error;
			//total+=utility;
			subtracted<<utility<<endl;
			
		}
		baseline.close();
		ith.close();
		subtracted.close();
	}

	ifstream subtract("temp.txt");
	double u, t=0.0;
	if(subtract.is_open()){
		while(subtract>>u){
			t+=u;
		}
		subtract.close();
	}
	
	cout<<t/double(5737);

	
}