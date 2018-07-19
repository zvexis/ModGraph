/* 
 * File:   dpTree.h
 * Author: Zvex
 *
 * Created on July 8, 2015, 8:46 PM
 */

#ifndef DPTREE_H
#define	DPTREE_H

#include "stdafx.h"
#include "SmartF.h"

#include <vector>
#include <random>

struct node{
    int type;//nurodoma ar yra S tipo = 1, -2 - failure
    float p;//pasirinkimo tikimybė
    float price;//įvykio kaina
    float time;//įvykio laikas
    float profit;//pelnas
	float priceTotal;//akumuliuota kaina nuo pradžios
	float probTotal;//akumuliuota kaina nuo pradžios
	float timeTotal;//akumuliuotas laikas nuo pradžios
    float value;//būsenos vertė
    node* parent;//viršūnės tėvas
    std::vector< node* > childs;//viršūnių vaikų vektorius
    node* cycle;//nuoroda į medžio pošaknį
    bool success;//veliavėlė, nurodanti, kad lapas atitinka sėkmingą scenarijų
    int next;
    int next2;
    node* twin;
    int depth;
    int n;
    int x;
    int y;
};

struct node2{
    float *node;
    int type;//0 - regular node; 1 - decision node; 2 - unsuccess;
    float price_lose;
    float time;
    float p;
    std::vector< int > childs;
    int cycle;
    int depth;
    int n;
	bool success;//veliavėlė, nurodanti, kad lapas atitinka sėkmingą scenarijų
};

class dpTree {
public:
    dpTree();
    dpTree(const dpTree& orig);
    virtual ~dpTree();
    void readF(std::string _file);
	void setSellingPrice(double price);
    void makeTree(std::string _file); 
	void makeTree_advanced(std::vector<node2> _nodes);
	void dpTree::Expand(int depthmax);
	void dpTree::Expand(node* n);
	node* CopyOfSubtree(node* _node);
	void calcPars();
	void calcPars(node* _root);
	void calcValues();
	void calcValues(node* _root);
	void calcProbs(std::vector< std::pair <double, double> > &hist);
	void calcProbs(node* n, std::vector< std::pair <double, double> > &hist);

	void print();
	void print(node* _root);
	void print_post();
	void print_pre();
	void print_post(node* _root);
	void print_pre(node* _root);

	void makeTree(std::vector<node2> _nodes);
	void makeTree_old(std::vector<node2> _nodes);
	float Evaluate(node*_root, int _n);
	node* chooseChild(node* _node);
	void solve(node* _root);
	void solve();
	void calcPars_nr();
	void calcProbs_nr(std::vector< std::pair <double, double> > &hist);

    void solve_s();
    void solve_s(node* _root);
    void Experiment(float money, float &money_left, node*&res);
	void addCycles();
    void makeCycles(node* _node);
	void renormProbs();
	void renormProbs(node* _node);
	void renormWeights();
	void renormWeights(node* _node);
	void addCyclesr();
	void addCyclesr(node* _node);

	void genAndAdd(node* _node); 
	void makeCopy(node* from, node* to);
#ifdef USE_CV
    void draw(const char* name);
    void drawGraph();
#endif
private:
    SmartF f;
    node* root;
	double sellingPrice;
	int depth;
	int depthmax;
	std::vector< node* > adds;
    std::mt19937 mt;
    std::uniform_real_distribution<float> dist;
};

#endif	/* DPTREE_H */

