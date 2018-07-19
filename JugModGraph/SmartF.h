/* 
 * File:   SmartF.h
 * Author: Zvex
 *
 * Created on September 7, 2015, 7:26 PM
 */

#ifndef SMARTF_H
#define	SMARTF_H

#include "stdafx.h"
#include <vector>

class SmartF {
public:
    SmartF();
    SmartF(const SmartF& orig);
    virtual ~SmartF();
    void add(double t, double f);
    double value(double t);
	double begin();
	double end();
	double intValue(double _t1, double _t2);
private:
    int ind;
    std::vector<double> T;
    std::vector<double> F;
    std::vector<double> K;
};

#endif	/* SMARTF_H */

