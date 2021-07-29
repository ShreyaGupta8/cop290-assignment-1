#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char** argv){

	//double total=0.0;

	ofstream subtracted;
	subtracted.open("temp.txt");
	ifstream baseline("out1.txt");
	double bD, tbD;
	ifstream ith("m3-10.txt");
	double qD, error, utility;
	while(baseline.is_open() && ith.is_open()){
		while(baseline>>bD && ith>>qD){
			//cout<<bD<<" "<<qD<<'\n';
			error=abs(bD-qD);
			utility=bD-error;
			tbD+=bD;
			//total+=utility;
			subtracted<<utility<<endl;
			
		}
		baseline.close();
		ith.close();
		subtracted.close();
	}

	ifstream subtract("temp.txt");
	double u, t=0.0;
	double j=0;
	while(subtract.is_open()){
		while(subtract>>u){
			t+=u;
			j++;
		}
		subtract.close();
	}
	
	cout<<t*double(100)/tbD<<" "<<j;

	
}