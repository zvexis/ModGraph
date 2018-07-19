/* 
 * File:   dpTree.cpp
 * Author: Zvex
 * 
 * Created on July 8, 2015, 8:46 PM
 */

#include "stdafx.h"
#include "dpTree.h"

dpTree::dpTree() {
    mt.seed( time(NULL) );
    dist=std::uniform_real_distribution<float>(0, 1);
}

dpTree::dpTree(const dpTree& orig) {
}

dpTree::~dpTree() {
}

void dpTree::setSellingPrice(double price) {
	sellingPrice = price;
}

void dpTree::readF(std::string _file){
    std::ifstream is(_file.c_str());
    double x,y;
    for(;;){
        is>>x>>y;
        if(is.fail())
            break;
#ifndef SILENT
		std::cout << "adding (" << x << ", " << y << ")" << std::endl;
#endif // SILENT
        f.add(x,y);
    }
    is.close();
	//std::cout<<"Suintegruotos islaidos: "<<f.intValue(f.begin(),650)<<std::endl;
}

void dpTree::makeTree(std::string _file){
    std::ifstream is(_file.c_str());
    std::vector<node2> nodes;
    node2 node;
    int i,n,num;
    while(!is.eof()){
        is
        >>node.n
        >>node.type
        >>node.price_lose
        >>node.time
        >>node.p
        >>n;
#ifndef SILENT
        std::cout<<"read node: "<<node.n<<" "<<node.price_lose<<" "<<node.time<<" "<<node.p<<" : ";
#endif // SILENT
		for(i=0; i<n; i++){
            is>>num;
            node.childs.push_back(num);
            //std::cout<<num<<" ";
        }
        is>>node.cycle;
        is>>node.depth;
        //std::cout<<std::endl;
        nodes.push_back(node);
        node.childs.clear();
    }
    is.close();
    std::cout<<"read is ok"<<std::endl;
	makeTree_advanced(nodes);
}

void dpTree::makeTree_advanced(std::vector<node2> _nodes) {
	if (_nodes.size() < 1)
		return;
	int i, j;
	int nodemaxind = 0;
	for (auto nod : _nodes) {
		if (nod.n > nodemaxind)
			nodemaxind = nod.n;
	}
	std::cout << "making tree ("<< nodemaxind + 1 <<")\n";
	node ** nnodes = new node*[nodemaxind + 1];
	int *indexes = new int[nodemaxind + 1];

	//node ** nnodes = new node*[_nodes.size()];
	//sukuriami medzio elementai
	for (i = 0; i<_nodes.size(); i++) {
		j = _nodes[i].n;
		indexes[j] = i;
		nnodes[j] = new node;
		nnodes[j]->parent = NULL;
		nnodes[j]->price = _nodes[i].price_lose;
		nnodes[j]->p = _nodes[i].p;
		nnodes[j]->time = _nodes[i].time;
		nnodes[j]->type = _nodes[i].type;
		nnodes[j]->success = nnodes[j]->type == 2 ? false : true;
		nnodes[j]->cycle = NULL;
		nnodes[j]->twin = NULL;
		nnodes[j]->depth = _nodes[i].depth;
		nnodes[j]->next = 0;
		nnodes[j]->next2 = 0;
		//nnodes[j]->n = i;
		nnodes[j]->n = _nodes[i].n;
		nnodes[j]->priceTotal = nnodes[j]->profit = 0;
	}
	root = nnodes[_nodes[0].n];
	root->parent = NULL;
	//sujungiami medzio elementai
	for (i = 0; i<_nodes.size(); i++) {
		//std::cout<<"inserting "<<_nodes[i].childs.size()<<std::endl;
		for (j = 0; j<_nodes[i].childs.size(); j++) {
			nnodes[_nodes[i].n]->childs.push_back(nnodes[_nodes[i].childs[j]]);
			nnodes[_nodes[i].childs[j]]->parent = nnodes[_nodes[i].n];
		}
		if (_nodes[i].cycle > 0 && _nodes[i].depth > 0)
			nnodes[_nodes[i].n]->cycle = nnodes[_nodes[i].cycle];
	}
}

void dpTree::Expand(int _depthmax) {
	depth = 0;
	depthmax = _depthmax;
	Expand(root);
}

void dpTree::Expand(node* n) {
	//std::cout<<
	if (n->cycle != NULL) {
		if (depth < depthmax) {
			//std::cout << " test " << n->n<<" with "<< n->cycle->n << std::endl;
			node* newNode = CopyOfSubtree(n->cycle);
			//std::cout << std::endl;
			newNode->p = 1;//(!)
			newNode->parent = n;
			n->childs.push_back(newNode);

			//*/
			depth = depth + 1;
			Expand(newNode);
			depth = depth - 1;

			//newNode->next = 0;
			//n->next = 0;
		}
		else {
			// deletes unneeded fictive node and recalculate the probabilities
			// of parent’s children nodes leaving the same proportions
			node* p = n->parent;
			for (int i = 0; i < p->childs.size(); i++) {
				if (p->childs[i] == n) {
					for (int j = i; j < p->childs.size() - 1; j++)
						p->childs[j] = p->childs[j + 1];
					p->childs.pop_back();
					break;
				}
			}
			//recalculate the probabilities
			double ptot = 0;
			for (auto &q : p->childs)
				ptot += q->p;
			for (auto &q : p->childs)
				q->p /= ptot;
		}
	}
	else {
		for (auto &t : n->childs)
			Expand(t);
	}
}

node* dpTree::CopyOfSubtree(node* _node) {
	node *res = new node;
	*res = *_node;
	//std::cout << res->n << "  ";
	res->childs.clear();//can be optimized ...
	res->next = 0;
	if (_node->cycle == NULL) {
		for (auto & q : _node->childs) {
			res->childs.push_back(CopyOfSubtree(q));
		}
		for (auto & q : res->childs) {
			q->parent = res;
		}
	}
	return res;
}


void dpTree::calcPars() {
	calcPars(root);
}

void dpTree::calcPars(node *n) {
	if (n->parent == NULL) {
		n->timeTotal = n->time;
		n->priceTotal = n->price;
	}
	else {
		n->timeTotal = n->time + n->parent->timeTotal;
		n->priceTotal = n->price + n->parent->priceTotal;;
	}
	for (auto &t : n->childs)
		calcPars(t);
}

void dpTree::calcValues() {
	calcValues(root);
}

void dpTree::calcValues(node *n) {
	for (auto &t : n->childs)
		calcValues(t);
	if (n->childs.size() == 0) {
		//success
		if (n->type != 2) {
			n->value = sellingPrice - (n->priceTotal + f.intValue(f.begin(), f.end()));
		}//not success
		else {
			n->value = -(n->priceTotal + f.intValue(f.begin(), n->timeTotal));
		}
		n->profit = n->value;
	}
	else {
		//investor
		if (n->type == 1){// && n->childs.size() > 0) {
			node *best = n->childs[0];
			for (auto &t : n->childs) {
				t->p = 0;
				if (t->value > best->value)
					best = t;
			}
			std::cout << n->n << " selects " << best->n << "; " << best->value << std::endl;
			best->p = 1;
		}
		n->value = 0;
		for (auto &t : n->childs) {
			n->value += t->p*t->value;
		}
	}
}

void dpTree::calcProbs(std::vector< std::pair <double, double> > &hist) {
	calcProbs(root, hist);
	std::sort(hist.begin(), hist.end());
}

void dpTree::calcProbs(node* n,std::vector< std::pair <double, double> > &hist) {
	if (n->parent == NULL) {
		n->probTotal = 1;
		//std::cout << "NULIS!" << std::endl;
	}
	else {
		n->probTotal = n->p*n->parent->probTotal;
	}
	if (n->childs.size()) {
		for (auto &t : n->childs)
			calcProbs(t, hist);
	}
	else {
		bool check = true;
		for (auto &t : hist) {
			if (t.first == n->value) {
				check = false;
				t.second += n->probTotal;
				break;
			}
		}
		if (check && n->probTotal>0) {
			hist.push_back(std::pair<double, double> (n->value,n->probTotal));
		}
	}
}



void dpTree::makeTree(std::vector<node2> _nodes) {
	if (_nodes.size() < 1)
		return;
	int i, j;
	int nodemaxind = 0;
	for (auto nod : _nodes) {
		if (nod.n > nodemaxind)
			nodemaxind = nod.n;
	}
	std::cout << "making tree ..." << nodemaxind + 1 << "\n";
	node ** nnodes = new node*[nodemaxind + 1];
	int *indexes = new int[nodemaxind + 1];

	//node ** nnodes = new node*[_nodes.size()];
	//sukuriami medzio elementai
	for (i = 0; i < _nodes.size(); i++) {
		indexes[_nodes[i].n] = i;
	}
	for (i = 0; i < _nodes.size(); i++) {
		_nodes[i].n = indexes[_nodes[i].n];
		for (j = 0; j<_nodes[i].childs.size(); j++) {
			_nodes[i].childs[j] = indexes[_nodes[i].childs[j]];
		}
	}
	for (i = 0; i<_nodes.size(); i++) {
		nnodes[i] = new node;
		nnodes[i]->parent = NULL;
		nnodes[i]->price = _nodes[i].price_lose;
		nnodes[i]->p = _nodes[i].p;
		nnodes[i]->time = _nodes[i].time;
		nnodes[i]->type = _nodes[i].type;
		nnodes[i]->success = nnodes[i]->type == 2 ? false : true;
		nnodes[i]->cycle = NULL;
		nnodes[i]->twin = NULL;
		nnodes[i]->depth = _nodes[i].depth;
		nnodes[i]->next = 0;
		nnodes[i]->next2 = 0;
		//nnodes[i]->n = i;
		nnodes[i]->n = _nodes[i].n;
		nnodes[i]->priceTotal = nnodes[i]->profit = 0;
	}
	root = nnodes[0];
	root->parent = NULL;
	//sujungiami medzio elementai
	for (i = 0; i<_nodes.size(); i++) {
		//std::cout<<"inserting "<<_nodes[i].childs.size()<<std::endl;
		for (j = 0; j<_nodes[i].childs.size(); j++) {
			nnodes[i]->childs.push_back(nnodes[_nodes[i].childs[j]]);
			nnodes[_nodes[i].childs[j]]->parent = nnodes[i];
		}
		if (_nodes[i].cycle > 0 && _nodes[i].depth > 0) {
			nnodes[i]->cycle = nnodes[_nodes[i].cycle];
		}
	}
}

void dpTree::makeTree_old(std::vector<node2> _nodes) {
	if (_nodes.size() < 1)
		return;
	std::cout << "making tree ...\n";
	int i, j;
	/*	int nodemaxind = 0;
	for (auto nod : _nodes) {
	if (nod.n > nodemaxind)
	nodemaxind = nod.n;
	}
	//node ** nnodes = new node*[nodemaxind+1];*/
	node ** nnodes = new node*[_nodes.size()];
	//sukuriami medzio elementai
	for (i = 0; i<_nodes.size(); i++) {
		nnodes[i] = new node;
		nnodes[i]->parent = NULL;
		nnodes[i]->price = _nodes[i].price_lose;
		nnodes[i]->p = _nodes[i].p;
		nnodes[i]->time = _nodes[i].time;
		nnodes[i]->type = _nodes[i].type;
		nnodes[i]->success = nnodes[i]->type == 2 ? false : true;
		nnodes[i]->cycle = NULL;
		nnodes[i]->twin = NULL;
		nnodes[i]->depth = _nodes[i].depth;
		nnodes[i]->next = 0;
		nnodes[i]->next2 = 0;
		//nnodes[i]->n = i;
		nnodes[i]->n = _nodes[i].n;
		nnodes[i]->priceTotal = nnodes[i]->profit = 0;
	}
	root = nnodes[0];
	root->parent = NULL;
	//sujungiami medzio elementai
	for (i = 0; i<_nodes.size(); i++) {
		//std::cout<<"inserting "<<_nodes[i].childs.size()<<std::endl;
		for (j = 0; j<_nodes[i].childs.size(); j++) {
			nnodes[i]->childs.push_back(nnodes[_nodes[i].childs[j]]);
			nnodes[_nodes[i].childs[j]]->parent = nnodes[i];
		}
		if (_nodes[i].cycle > 0 && _nodes[i].depth > 0)
			nnodes[i]->cycle = nnodes[_nodes[i].cycle];
	}
}

void dpTree::print() {
	print(root);
	std::cout << std::endl;
}

void dpTree::print_post() {
	print_post(root);
	std::cout << std::endl;
}

void dpTree::print_pre() {
	print_pre(root);
	std::cout << std::endl;
}

void dpTree::print(node* _root) {
	//std::cout<<"go through "<<_root->childs.size()<<std::endl;
	for (int i = 0; i<_root->childs.size(); i++) {
		//std::cout<<"go to "<<_root->childs[i]->n<<std::endl;
		print(_root->childs[i]);
	}
	std::cout << _root->n << "  ";
}

void dpTree::print_post(node* _root) {
	int count = 0;
	while (true) {
		//jeigu yra neaplankytu vaiku
		if (_root->next < _root->childs.size()) {
			_root->next++;
			_root = _root->childs[_root->next - 1];
		}
		else {//visi vaikai aplankyti
			_root->next = 0;
			std::cout << _root->n << "  ";
			count++;
			if (_root->parent == NULL)
				break;
			_root = _root->parent;
		}
	}
	std::cout << "is viso: " << count << std::endl;
}

void dpTree::print_pre(node* _root) {
	while (true) {
		//jeigu lankosi virsuneje pirma karta
		if (!_root->next)
			std::cout << _root->n << "  ";
		//jeigu yra neaplankytu vaiku
		if (_root->next < _root->childs.size()) {
			_root->next++;
			_root = _root->childs[_root->next - 1];
		}
		else {//visi vaikai aplankyti
			_root->next = 0;
			if (_root->parent == NULL)
				break;
			_root = _root->parent;
		}
	}
}


//EXPERIMENTAL FUNCTIONS

void dpTree::solve() {
	solve(root);
}

void dpTree::solve_s() {
	solve_s(root);
}

void dpTree::solve_s(node* _root) {
	int nbest;
	float best;
	float t;
	int i;
	while (true) {
		//jeigu yra neaplankytu vaiku
		if (_root->next < _root->childs.size()) {
			_root->next++;
			_root = _root->childs[_root->next - 1];
		}
		else {//visi vaikai aplankyti
			_root->next = 0;
			//actions begin
			//std::cout<<"type: "<<_root->type<<"  childs: "<<_root->childs.size()<<std::endl;
			if (_root->type == 1 && _root->childs.size()>0) {
				best = Evaluate(_root->childs[0], 100);
				nbest = 0;
				std::cout << "value: " << best << std::endl;
				for (i = 1; i<_root->childs.size(); i++) {
					t = Evaluate(_root->childs[i], 100);
					std::cout << "value: " << t << std::endl;
					if (t>best) {
						best = t;
						nbest = i;
					}
				}
				for (i = 0; i<_root->childs.size(); i++) {
					_root->childs[i]->p = 0;
				}
				_root->childs[nbest]->p = 1;
				std::cout << "best: " << nbest << std::endl;
			}
			//actions end
			if (_root->parent == NULL)
				break;
			_root = _root->parent;
		}
	}
}

void dpTree::Experiment(float money, float &money_left, node*&res) {
	node* n = root;
	while (n->childs.size() > 0) {
		money -= n->price;
		n = chooseChild(n);
	}
	money_left = money;
	res = n;
}

float dpTree::Evaluate(node*_root, int _n) {
	node* n;
	float money = 0;
	int i;
	for (i = 0; i<_n; i++) {
		n = _root;
		money -= n->price;
		while (n->childs.size() > 0) {
			n = chooseChild(n);
			money -= n->price;
		}
	}
	return money / (float)_n;
}

node* dpTree::chooseChild(node* _node) {
	int s = _node->childs.size();
	if (s < 1)
		return NULL;
	float p = dist(mt);
	_node->p;
	float p0 = 0;
	int i;
	s--;
	for (i = 0; i<s; i++) {
		p0 += _node->childs[i]->p;
		if (p < p0)
			return _node->childs[i];
	}
	return _node->childs[i];

}

void dpTree::solve(node* _root) {
	float value;
	float max;
	int i, imax;
	while (true) {
		//jeigu yra neaplankytu vaiku
		if (_root->next < _root->childs.size()) {
			_root->next++;
			_root = _root->childs[_root->next - 1];
		}
		else {//visi vaikai aplankyti
			_root->next = 0;

			if (!_root->childs.size()) {
				_root->value = _root->profit;
			}
			else {
				if (_root->type == 1) {
					max = _root->childs[0]->value;
					imax = 0;
					//value=0;
					//std::cout << max << " ; " << _root->childs[0]->n << std::endl;
					for (i = 0; i<_root->childs.size(); i++) {
						value = _root->childs[i]->value;
						//std::cout << value << " ;; " << _root->childs[i]->n << std::endl;
						if (max < value) {
							imax = i;
							max = value;
						}
					}
					for (i = 0; i<_root->childs.size(); i++) {
						_root->childs[i]->p = 0;
					}
					_root->childs[imax]->p = 1;
					_root->value = _root->childs[imax]->value;
					std::cout << _root->n << " selects " << _root->childs[imax]->n << " ; " << _root->value << std::endl;
				}
				else {
					value = 0;
					for (i = 0; i<_root->childs.size(); i++) {
						value += _root->childs[i]->value * _root->childs[i]->p;
						/*if (_root->n == 3) {
							std::cout << "lala " << _root->childs[i]->value << " " << _root->childs[i]->p << std::endl;
						}*/
					}
					//value /= (double)_root->childs.size();
					_root->value = value;
				}
			}

			if (_root->parent == NULL)
				break;
			_root = _root->parent;
		}
	}
}

void dpTree::calcPars_nr() {
	node *_root = root;
	float tsum = 0;
	float ssum = 0;
	while (true) {
		//jeigu lankosi virsuneje pirma karta
		if (!_root->next) {
			if (_root->parent == NULL) {
				_root->timeTotal = _root->time;
				_root->priceTotal = _root->price;
			}
			else {
				_root->timeTotal = _root->time + _root->parent->timeTotal;
				_root->priceTotal = _root->price + _root->parent->priceTotal;// + f.intValue(_root->timeTotal - _root->time, _root->timeTotal);
			}
		}
		//jeigu nera vaiku, t.y. lapas
		if (!_root->childs.size()) {
			if (_root->type != 2) {
				/*if (!(_root->success && _root->type != 2)) {
				std::cout << "NESAMONE " << _root->n << " " << (int)_root->success << " " << _root->type << std::endl;
				}*/
				_root->profit = sellingPrice - (_root->priceTotal + f.intValue(f.begin(), f.end()));
			}
			else {
				_root->profit = -(_root->priceTotal + f.intValue(f.begin(), _root->timeTotal));
				/*if (_root->n == 25) {
				std::cout << " ROOT 25 " << _root->timeTotal << " " << _root->profit << std::endl;
				}*/
			}
		}
		//jeigu yra neaplankytu vaiku
		if (_root->next < _root->childs.size()) {
			_root->next++;
			_root = _root->childs[_root->next - 1];
		}
		else {//visi vaikai aplankyti
			_root->next = 0;
			if (_root->parent == NULL)
				break;
			_root = _root->parent;
		}
	}
}

void dpTree::calcProbs_nr(std::vector< std::pair <double, double> > &hist) {
	node *_root = root;
	float tsum = 0;
	float ssum = 0;
	hist.clear();
	while (true) {
		//jeigu lankosi virsuneje pirma karta
		if (!_root->next) {
			if (_root->parent == NULL) {
				_root->probTotal = 1;
			}
			else {
				_root->probTotal = _root->parent->probTotal * _root->p;
			}
		}
		//jeigu nera vaiku, t.y. lapas
		if (!_root->childs.size()) {
			if (_root->probTotal > 0) {
				if (_root->type == 2)
					std::cout << "BANKRUPT t=" << _root->timeTotal <<" cost = "<< f.intValue(f.begin(), _root->timeTotal)<< std::endl;
				std::pair <double, double> t(_root->profit, _root->probTotal);
				bool add = true;
				for (int i = 0; i < hist.size();i++) {
					if (t.first == hist[i].first) {
						if(t.second > 0.7)
							std::cout << "tseee " << t.second << std::endl;
						hist[i].second += t.second;
						add = false;
						break;
					}
				}
				if(add)
					hist.push_back(t);
			}
		}
		//jeigu yra neaplankytu vaiku
		if (_root->next < _root->childs.size()) {
			_root->next++;
			_root = _root->childs[_root->next - 1];
		}
		else {//visi vaikai aplankyti
			_root->next = 0;
			if (_root->parent == NULL)
				break;
			_root = _root->parent;
		}
	}
	std::sort(hist.begin(), hist.end());
}

void dpTree::addCycles(){
    adds.clear();
    std::cout<<"addCycles 1"<<std::endl;
    makeCycles(root);
    std::cout<<"addCycles 2"<<std::endl;
    for(int i=0;i<adds.size();i++){
        adds[i]->parent->childs.push_back(adds[i]);
		adds[i]->price = 0;
		adds[i]->time = 0;
	}
    std::cout<<"addCycles 3"<<std::endl;
    renormWeights(root);
    std::cout<<"addCycles 4"<<std::endl;
 }
 
void dpTree::makeCycles(node* _node){
    node* begin = _node;
    while(true){
        //jeigu yra neaplankytu vaiku
        if( _node->next < _node->childs.size()){
            _node->next++;
            _node = _node->childs[_node->next-1];
        }else{//visi vaikai aplankyti
            _node->next=0;
            if(_node->cycle != NULL && _node->depth>0 && _node->childs.size()==0)
                genAndAdd(_node);
            if(_node == begin)
                break;
            _node = _node->parent;
        }
    }
}

void dpTree::renormProbs() {
	renormProbs(root);
}

void dpTree::renormProbs(node* _node) {
	double p = 0;
	for (auto &t : _node->childs)
		p += t->p;
	if (_node->childs.size())
		if (p != 1)
			std::cout << "PROB NOT EQUAL TO 1: " << p << std::endl;
}


void dpTree::renormWeights() {
	renormWeights(root);
}

void dpTree::renormWeights(node* _node){
    node* begin = _node;
    std::cout<<"renormWeights "<<std::endl;
    while(true){
        //jeigu yra neaplankytu vaiku
        if( _node->next < _node->childs.size()){
            _node->next++;
            _node = _node->childs[_node->next-1];
        }else{//visi vaikai aplankyti
            _node->next=0;

			float norm=0;
			int i;
			if (_node->cycle != NULL && _node->childs.size() == 0){
				for (i = 0; i < _node->parent->childs.size(); i++) {
					if (_node->parent->childs[i] == _node) {
						for (int j = i; j < _node->parent->childs.size() - 1; j++)
							_node->parent->childs[j] = _node->parent->childs[j + 1];
						_node->parent->childs.pop_back();
						_node->parent->next--;
						node* temp = _node->parent;
						delete _node;
						_node = temp;
						break;
					}
				}
			}
			else {
				for (i = 0; i < _node->childs.size(); i++) {
					norm += _node->childs[i]->p;
				}
				//std::cout<<"pnorm "<<norm<<std::endl;
				for (i = 0; i < _node->childs.size(); i++) {
					_node->childs[i]->p /= norm;
				}
				if (_node == begin)
					break;
				_node = _node->parent;
			}
        }
    }
}

void dpTree::addCyclesr() {
	addCyclesr(root);
}

void dpTree::addCyclesr(node* _node) {
	for (int i = 0; i<_node->childs.size(); i++) {
		addCyclesr(_node->childs[i]);
	}
	if (_node->cycle != NULL && _node->depth > 0 && _node->childs.size() == 0) {
		genAndAdd(_node);
		/*node *to;
		makeCopy(_node->cycle, to);
		_node->childs.push_back(to);
		addCyclesr(to);*/
	}
}

void dpTree::genAndAdd(node* _node) {
	//    _node->depth--;
	node* subroot = new node;
    makeCopy(_node->cycle, subroot);
    subroot->p = 1;
    subroot->next = 0;
    node* child = subroot;
    node* child2;
    int depth=1;
    while(true){
        //jeigu lankosi virsuneje pirma karta
        if(!child->next){
            //std::cout<<depth<<" < "<<_node->depth<<std::endl;
            if(child->twin->cycle != NULL && depth < _node->depth){

				std::cout << "NEBUNA!" << std::endl;
                //child->depth = 0;
                //child->twin->depth --;
                child2 = new node;
                makeCopy(child->twin->cycle, child2);
                child2->p = 1;
                child2->next = 0;
                child->childs.push_back(child2);
                child2->parent=child;
            }
        }
        //jeigu yra neaplankytu vaiku
        if( child->next < child->childs.size()){
            child->next++;
            if(child->twin->cycle != NULL)
                depth++;
            child = child->childs[child->next-1];
        }else{//visi vaikai aplankyti
            child->next=0;
            if(child == subroot)
                break;
            child = child->parent;
            if(child->twin->cycle != NULL)
                depth--;
        }
    }

    //_node->childs.push_back(subroot);
    subroot->parent = _node;
    adds.push_back(subroot);
}

void dpTree::makeCopy(node* from, node* to) {
	std::cout << "making copy from " << from->n << std::endl;
	int i;
	node* begin = from;
	node *t;
	int next;
	while (true) {
		if (from->next2 == 0) {
			//std::cout<<"1 "<<to->depth<<std::endl;
			to->depth = from->depth;
			to->n = from->n;
			to->next = 0;
			to->price = from->price;
			to->p = from->p;
			to->time = from->time;
			to->type = from->type;
			//to->cycle = NULL;
			to->cycle = from->cycle;
			to->twin = from;
			for (i = 0; i<from->childs.size(); i++) {
				t = new node;
				t->parent = to;
				to->childs.push_back(t);
			}
		}
		//jeigu yra neaplankytu vaiku
		if (from->next2 < from->childs.size()) {
			//std::cout<<"2 "<<std::endl;
			next = from->next2;
			from->next2++;
			from = from->childs[next];
			to = to->childs[next];
		}
		else {//visi vaikai aplankyti
			  //std::cout<<"3 "<<std::endl;
			from->next2 = 0;
			to->next = 0;
			if (from == begin)
				break;
			from = from->parent;
			to = to->parent;
		}
	}
}


/*
 * addCycles(node* _node){
 * if(node->cycle != NULL && node->childs.size()==0)
 *      checkAndGen(node);
 * for(int i=0; i<node->childs.size(); i++){
 *      addCycles(node->childs[i]);
 * }
 * }
 * 
 * checkAndGen(node* _node){
 *  if(_node->depth == 0)
 *  return;
 * _node->depth--;
 * node* child = new node;
 * makeCopy(child _node->cycle);
 * _node->childs.add(child);
 * }
 */

#ifdef USE_CV
using namespace std;
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
void  dpTree::draw(const char* name){
    Mat im = imread(name);
    if (im.empty())
    {
      cout << "Cannot open image!" << endl;
      return;
    }

    imshow("image", im);
    waitKey(0);
}

void dpTree::drawGraph(){
    int i,j,k;
    if(root == NULL || root->childs.size()<1)
        return;
    vector< vector< node* > > Nodes;
    node *tnode = root;
    Nodes.push_back(std::vector< node* >());
    Nodes[0].push_back(root);
    bool check=true;
    for(i=0;check;i++){
        Nodes.push_back(std::vector< node* >());
        check=false;
        for(j=0;j<Nodes[i].size();j++){
            for(k=0;k<Nodes[i][j]->childs.size();k++){
                tnode = Nodes[i][j]->childs[k];
                Nodes[i+1].push_back(tnode);
                if(tnode->childs.size()>0 && !check)
                    check = true;
            }
        }
    }
    IplImage* img;// = cvCloneImage(Map);
    int nsize = 90;
    float xmin,ymin,xmax,ymax,x,y;
    float mapx = 0;
    float mapy = 0;
    mapy = nsize*(1+(Nodes.size()-1)*3)+10;
    int total = 0;
    for(i=0;i<Nodes.size();i++){
        total += Nodes[i].size();
        x = nsize*(1+(Nodes[i].size()-1)*2);
        if(mapx < x)
            mapx = x;
    }
    //if(mapx > 2048*2)mapx = 2048*2;
    //if(mapy > 8196*2)mapy = 8196*2;
    std::cout<<"image size: "<<mapx<<"x"<<mapy<<" , nodes total:"<<total<<std::endl;
    img=cvCreateImage( cvSize(mapx,mapy), 8, 3 );
	cvSet(img, CV_RGB(255, 255, 255));
    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_PLAIN|CV_FONT_ITALIC,2,2,0,1);    
    float resize,r;
    xmin=ymin=0;
    xmax=mapx;
    ymax=mapy;
    resize=1;
    stringstream ss;
    stringstream ss2;
    for(i=0;i<Nodes.size();i++){
        y=nsize*(i*3+1)-nsize/2;
        xmin = (mapx - nsize*Nodes[i].size()*2)/2;
        for(j=0;j<Nodes[i].size();j++){
            x=nsize*(j*2+1);
            Nodes[i][j]->x = x+xmin;
            Nodes[i][j]->y = y;
        }
    }
    xmin=ymin=0;
#define SPALVA cvScalar( 0, 0, 0, 0 )
    for(i=1;i<Nodes.size();i++){
        for(j=0;j<Nodes[i].size();j++){
//            ss.str("");
//            ss<<"("<<(int)v->x<<", "<<(int)v->y<<")";
            x=Nodes[i][j]->x;
            y=Nodes[i][j]->y;
            cvLine( img,
                cvPoint((xmin + x)*resize,
                        (ymin + y)*resize),
                cvPoint((xmin + Nodes[i][j]->parent->x)*resize,
                        (ymin + Nodes[i][j]->parent->y)*resize),
				SPALVA, 2, 0, 0 );
            if(Nodes[i][j]->cycle!=NULL && Nodes[i][j]->depth>0){
                /*cvLine( img,
                cvPoint((xmin + x)*resize,
                        (ymin + y)*resize),
                cvPoint((xmin + Nodes[i][j]->cycle->x)*resize,
                        (ymin + Nodes[i][j]->cycle->y)*resize),
					cvScalar(0, 0, 255, 0), 1, 0, 0 );*/
            }
        }
    }
    for(i=0;i<Nodes.size();i++){
        for(j=0;j<Nodes[i].size();j++){
//            ss.str("");
//            ss<<"("<<(int)v->x<<", "<<(int)v->y<<")";
            x=Nodes[i][j]->x;
            y=Nodes[i][j]->y;
            cvCircle( img,
                    cvPoint((xmin + x)*resize,
                            (ymin + y)*resize),
                    nsize/2,
                    SPALVA,-1,0,0);
/*            cvPutText(img,ss.str().c_str(),
                    cvPoint(mapx - (-xmin + v->x)*resize-15-40,
                            mapy - (-ymin + v->y)*resize-15-5),
                    &font,cvScalar(50,50,255));*/
            ss.str("");
            ss2.str("");
			int cycl;
			if (Nodes[i][j]->cycle == NULL)
				cycl = 0;
			else
				cycl = 1;
			//(Nodes[i][j]->cycle == NULL) ? 0 : 1;
			ss << Nodes[i][j]->n << ";" << cycl;//<<"("<<Nodes[i][j]->p<<")";
            //ss<<Nodes[i][j]->n<<";"<<Nodes[i][j]->timeTotal<<";"<<Nodes[i][j]->priceTotal<<";"<<Nodes[i][j]->value;
			//ss2 << Nodes[i][j]->time << ";" << Nodes[i][j]->price << ";" << Nodes[i][j]->profit;
			ss2 << Nodes[i][j]->value << ";";// << Nodes[i][j]->p << std::endl;// ";" << Nodes[i][j]->profit;
			cvPutText(img,ss.str().c_str(),
                    cvPoint((xmin + x - nsize/2+5)*resize,
                            (ymin + y-7 )*resize),
                    &font,cvScalar(50,50,255));
            cvPutText(img,ss2.str().c_str(),
                    cvPoint((xmin + x - nsize/2+5)*resize,
                            (ymin + y + 15  )*resize),
                    &font,cvScalar(50,50,255));
        }
    }
    cvSaveImage("Graph.PNG",img);
    if(img->imageData!=NULL)
        cvReleaseImage(&img);
    /*list< virsune >::iterator v;
    v = Nodes.begin();
    xmin=xmax=v->x;
    ymin=ymax=v->y;
    for(v++; v != Nodes.end(); v++){
        x = v->x;
        y = v->y;
        if(x<xmin)
            xmin=x;
        else if(x>xmax)
            xmax=x;
        if(y<ymin)
            ymin=y;
        else if(y>ymax)
            ymax=y;
    }
    resize=(mapx-30.)/(xmax-xmin);
    r=(mapy-30.)/(ymax-ymin);
    if(r<resize)
        resize=r;
    //cvScale(Map,img,scale);
    v = Nodes.begin();
    list< briauna >::iterator b;
    for(b = Rels.begin(); b != Rels.end(); b++){
        cvLine( img,
                cvPoint(mapx - (-xmin + b->x->x)*resize-15,
                        mapy - (-ymin + b->x->y)*resize-15),
                cvPoint(mapx - (-xmin + b->y->x)*resize-15,
                        mapy - (-ymin + b->y->y)*resize-15),
                cvScalar( 255, 170, 0, 0 ), 2, 0, 0 );
    }
    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_PLAIN|CV_FONT_ITALIC,1,1,0,1);
    string text;
    stringstream ss;
    for(v=Nodes.begin(); v != Nodes.end(); v++){
        ss.str("");
        ss<<"("<<(int)v->x<<", "<<(int)v->y<<")";
        cvCircle( img,
                cvPoint(mapx - (-xmin + v->x)*resize-15,
                        mapy - (-ymin + v->y)*resize-15),
                12,
                cvScalar(255,170,0,0),-1,0,0);
        cvPutText(img,ss.str().c_str(),
                cvPoint(mapx - (-xmin + v->x)*resize-15-40,
                        mapy - (-ymin + v->y)*resize-15-5),
                &font,cvScalar(50,50,255));
        ss.str("");
        ss<<v->n;
        cvPutText(img,ss.str().c_str(),
                cvPoint(mapx - (-xmin + v->x)*resize-15-6,
                        mapy - (-ymin + v->y)*resize-15+8),
                &font,cvScalar(50,50,255));
    }
    //itoa(cntr,cntr_s,10);
    cout<<"will save now"<<endl;
    cvSaveImage("Graph.png",img);
    if(img->imageData!=NULL)
        cvReleaseImage(&img);*/
}
#endif
