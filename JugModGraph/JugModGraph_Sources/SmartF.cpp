/* 
 * File:   SmartF.cpp
 * Author: Zvex
 * 
 * Created on September 7, 2015, 7:26 PM
 */

#include "stdafx.h"
#include "SmartF.h"
#include <iostream>

SmartF::SmartF() {
    ind = 0;
}

SmartF::SmartF(const SmartF& orig) {
}

SmartF::~SmartF() {
}

void SmartF::add(double _t, double _f){
    ind = T.size() - 1;
    if(ind >=0 && _t < T[ind])
        _t = T[ind];
    T.push_back(_t);
    F.push_back(_f);
    if(ind < 0 || T[ind] == T[ind+1])
        K.push_back(0);
    else 
        K.push_back((F[ind+1] - F[ind])/(T[ind+1] - T[ind]));
}

double SmartF::value(double t){
    //std::cout<<"value "<<t<<" ind "<<ind<<std::endl;
    //std::cout<<t<<" < "<<T[ind]<<std::endl;
    while(t < T[ind]){
        if(ind <= 0){
            return F[0];
        }
        --ind;
    }
    //std::cout<<"value ind "<<ind<<std::endl;
    while(T[ind+1] <= t){
        if(ind >= T.size()-2){
            return F[T.size()-1];
        }
        ++ind;
    }
    //std::cout<<"value ind "<<ind<<std::endl;
    /*if(T[ind] == T[ind+1]){
        return T[ind+1];
    }*/
    return F[ind+1]-(T[ind+1]-t)*K[ind+1];
}

double SmartF::begin() {
	return T[0];
}

double SmartF::end() {
	return T[T.size()-1];
}

double SmartF::intValue(double _t1, double _t2){
	if (_t2 > end())
		_t2 = end();
	double temp;
    bool inv = false;
    if(_t1 > _t2){
        temp = _t1;
        _t1 = _t2;
        _t2 = temp;
        inv = true;
    }
    //std::cout<<"value "<<t<<" ind "<<ind<<std::endl;
    //std::cout<<t<<" < "<<T[ind]<<std::endl;
    while(_t1 < T[ind]){
        if(ind <= 0){
            break;
        }
        --ind;
    }
    //std::cout<<"value ind "<<ind<<std::endl;
    while(T[ind+1] <= _t1){
        if(ind >= T.size()-2){
            break;
        }
        ++ind;
    }
    
    int ind2 = ind;
    while(_t2 < T[ind2]){
        if(ind2 <= 0){
            break;
        }
        --ind2;
    }
    //std::cout<<"value ind "<<ind<<std::endl;
    while(T[ind2+1] <= _t2){
        if(ind2 >= T.size()-2){
            break;
        }
        ++ind2;
    }
    double v1 = F[ind+1]-(T[ind+1]-_t1)*K[ind+1];
    double v2 = F[ind2+1]-(T[ind2+1]-_t2)*K[ind2+1];

    if(ind == ind2){
        //std::cout<<"v1,v2: "<<v1<<", "<<v2<<std::endl;
        return (_t2 - _t1)* 0.5 * (v1 + v2);
    }
    double sum = (T[ind+1] - _t1) * 0.5 * (v1 + F[ind+1]) + (_t2 - T[ind2]) * 0.5 * (F[ind2] + v2);
    //std::cout<<"sum: "<<sum <<" = "<<(T[ind+1] - _t1)<<" * 0.5 * "<< (v1 + F[ind+1])<<" + "<<(_t2 - T[ind2])<<" * 0.5 * "<<(v2 + F[ind2])<<std::endl;
    for(int i=ind+1; i<ind2; i++){
        sum += (T[i+1] - T[i]) * 0.5 * (F[i+1] + F[i]);
        //std::cout<<"sum += "<<sum<<" = "<<(T[i+1] - T[i])<<" * 0.5 * "<<(F[i+1] + F[i])<<std::endl;
    }
    return sum;
}
