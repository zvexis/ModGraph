/* 
 * File:   main.cpp
 * Author: Zvex
 *
 * Created on June 1, 2014, 7:09 PM
 */
//#include <cstdlib>
#include "stdafx.h"


using namespace std;

/*
 * 
 */
void ex0(){
    dpTree T;
    T.makeTree("data.txt");
    T.readF("F.txt");
	T.setSellingPrice(8998.2);
	//cout << "Max pelnas = " << 8998.2 - 5508.572753 << endl;

	//T.print_post();

	//T.renormProbs();
	T.Expand(2);
	T.calcPars();
	T.calcValues();
	
	/*T.addCycles();
	T.renormWeights();
	T.calcPars_nr();
	T.solve();*/
	vector< std::pair <double, double> > hist;
	//T.renormProbs();
	T.calcProbs(hist);
	cout << "distribution is:" << endl;
	for (auto i : hist)
		cout << i.first << " & ";
	cout << endl;
	for (auto i : hist)
		cout <<  i.second << " & ";
	cout << endl;
	double s = 0;
	for (auto i : hist)
		s += i.first*i.second;
	cout << "Expect value: " << s << endl;
#ifdef USE_CV
	//T.draw("raz.PNG");
    T.drawGraph();
#endif
	//return;
    //T.print_post();
    //T.print_pre();
}

void ex1(){
    dpTree T;
    T.makeTree("data.txt");
    T.print_post();
    float money;
    node* res;
    int N=10;
    float s=0;
    for(int i=0;i<N;i++){
        T.Experiment(100, money, res);
        cout<<"money left: "<<money<<" node chose: "<<res->n<<endl;
        s+=money;
    }
    cout<<"ex1: N="<<N<<" profit="<<s/(float)N<<endl;
}

void testF(){
    SmartF f;
    f.add(0,1);
    f.add(2,1.5);
    f.add(2,2);
    f.add(6,1);
    f.add(7,3);
    cout<<"values: "<<endl;
    cout<<f.value(0)<<endl;
    cout<<f.value(2)<<endl;
    cout<<f.value(4)<<endl;
    cout<<f.value(1)<<endl;
    cout<<f.value(3)<<endl;
    cout<<"integrals: "<<endl;
    cout<<f.intValue(0,2)<<endl;
    cout<<f.intValue(2,6)<<endl;
    cout<<f.intValue(0,6)<<endl;
    cout<<f.intValue(0,1)+f.intValue(1,1.5)+f.intValue(1.5,3)+f.intValue(3,6)<<endl;
    cout<<f.intValue(0,7)<<endl;
    cout<<f.intValue(0,1)+f.intValue(1,1.5)+f.intValue(1.5,3)+f.intValue(3,7)<<endl;
}

int main(int argc, char** argv) {
    try{
        ex0();
        //T.print();
        //T.solve();
    }catch (std::runtime_error &e){
        std::cout<<e.what()<<std::endl;
    }
	//system("pause");
    return 0;
}
