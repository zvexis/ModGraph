#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
using namespace std;

bool B=false;
ifstream is;
ofstream os;

class node{
public:
	node *left;       // rodykle i kaire
    node *right;      // rodykle i desine
	node *previous;
    int data;                // reiksme
	short balance;
	bool was, printed;
	
	                          //Konstruktioriai
	node(){
		left=NULL;
		right=NULL;
		previous=NULL;
		data=NULL;
		balance=NULL;
		was=NULL;
		printed=NULL;
	}

	node(int dat){
		left=NULL;       
		right=NULL;
		previous=NULL;
		data=dat;               
		balance=NULL;
		was=NULL;
		printed=NULL;
	}

	~node(){}

	bool IsEmpty(){
		if(this == NULL ){
			cout<<"Medis tuscias"<<endl;
			return true;
		}return false;
	} 
	                                  // Elemento iterpimo
	void InsertNode(int dat){

		node* t=this;
		while(t != NULL ){
			if(dat < t->data)
				if(t->left==NULL){
					t->left=new node;
					t->left->data=dat;
					t->left->previous=t;
					break;
				}else
					t=t->left;
			else
				if(t->right==NULL){
					t->right=new node;
					t->right->data=dat;
					t->right->previous=t;
					break;
				}else
					t=t->right;
		}
	}
	                                      // Elemento pasalinimo
	node* DeleteNode(node *root, int key){

		node *p,*p2;

		if(root==NULL) return root; 

		if(root->data == key) { 
			if(root->left == root->right){
				delete(root);
				B=true;
				return NULL;
			}

			else if(root->left == NULL) {
				p = root->right;
				p->previous=root->previous;
				delete(root);
				B=true;
				return p;
			}
			else if(root->right == NULL) {
				p = root->left;
				p->previous=root->previous;
				delete(root);
				B=true;
				return p;
			}

			else {
				p2 = root->right;
				p = root->right;
				while(p->left) p = p->left;
				p->left = root->left;
				p->left->previous= root->left->previous;
				p2->previous=root->previous;
				delete(root);
				B=true;
				return p2;
			}
		}
		if(root->data < key) root->right = DeleteNode(root->right, key);
		else root->left = DeleteNode(root->left, key);
		return root;
	}
	                                      // Elementui rasti
	node* FindNode(node* tree, int inf){
		while( tree != NULL && tree->data !=inf){
			if(tree->data < inf)
				tree=tree->right;
			else
				tree=tree->left;
		}
		return tree;
	}
 
	                             // Elementu apejimas infix
	void InOrder(node* v){//////////////////
		if( v != NULL){
			InOrder(v->left);
            cout<<v->data<<", "<<v->balance<<"   ";
			InOrder(v->right);
		}
	}

	bool WasThere(node* n){
		if(n==NULL) return true;
		if(n->was==true) return true;
		return false;
	}
	void NewInOrder(){
		node* temp=this;
		os.open("rezults.txt");
		bool Z;
		while( temp != NULL){
			Z=false;
			while(!Z){
				if(temp->left != NULL && temp->right != NULL){
					if(!temp->left->was) 
						temp=temp->left;
					else if(!temp->right->was){ // pries einant i desine spausdiname kai ne 0
						if(!temp->printed){
							os<<temp->data<<", "<<temp->balance<<"   "<<endl;
							temp->printed=true;
						}
						temp=temp->right;
					}
					if( WasThere(temp->left) && WasThere(temp->right) )// patirkinimas
						Z=true;

				}else if(temp->left != NULL && temp->right == NULL){
					if(!temp->left->was) 
						temp=temp->left;
					else Z=true;


				}else if(temp->left == NULL && temp->right != NULL){
					if(!temp->right->was){
						if(!temp->printed){
							os<<temp->data<<", "<<temp->balance<<"   "<<endl;
							temp->printed=true;
						}
						temp=temp->right;
					}
					else  Z=true;
				}else if(temp->left == NULL && temp->right == NULL) 
					Z=true;
			}

			if(!temp->printed){
				os<<temp->data<<", "<<temp->balance<<"   "<<endl;  // panaikina zymejimus , kad galima butu kartoti infix apejima
				temp->printed=true;
			}

			if(temp->left){
				if(temp->left->left){
					temp->left->left->was=false;
					temp->left->left->printed=false;
				}
				if(temp->left->right){
					temp->left->right->was=false;
					temp->left->right->printed=false;
				}
			}
			if(temp->right){ 
				if(temp->right->left){
					temp->right->left->was=false;
					temp->right->left->printed=false;
				}
				if(temp->right->right){
					temp->right->right->was=false;
					temp->right->right->printed=false;
				}
			}
			temp->was=true;
			if(temp->previous !=NULL) 
				temp=temp->previous;
			else{
				if(temp->left) temp->left->was=false;
				if(temp->left) temp->left->printed=false;

				if(temp->right) temp->right->was=false;
				if(temp->right) temp->right->printed=false;

				temp->was=false;
				temp->printed=false;
				temp=temp->previous;
			}
		}
		os<<endl;
		os.close();
	}

	
	void PostOrderBalance(node* v){

		if( v != NULL){
			PostOrderBalance(v->left);
			PostOrderBalance(v->right);

			if(v->left!=NULL && v->right!=NULL)    //1
				v->balance= Gylis(v->right)- Gylis(v->left);

			if(v->left!=NULL && v->right==NULL)    //2
				v->balance= -Gylis(v->left)-1 ;

			if(v->left==NULL && v->right!=NULL)    //3
				v->balance= Gylis(v->right)+1 ;

			if(v->left==NULL && v->right==NULL)    //4
				v->balance= 0;


			if(abs(v->balance) == 2){
				if(v->previous==NULL){
					v->BalanceTree('H');
					return;
				}else if(v->previous->left == v){
					v->BalanceTree('l');
					return;
				}else if(v->previous->right == v){ 
					v->BalanceTree('r');
					return;
				}
			}

		}
	}
	void PostOrderBalance2(node* v){

		if( v != NULL){
			PostOrderBalance(v->left);
			PostOrderBalance(v->right);

			if(v->left!=NULL && v->right!=NULL)    //1
				v->balance= Gylis(v->right)- Gylis(v->left);

			if(v->left!=NULL && v->right==NULL)    //2
				v->balance= -Gylis(v->left)-1 ;

			if(v->left==NULL && v->right!=NULL)    //3
				v->balance= Gylis(v->right)+1 ;

			if(v->left==NULL && v->right==NULL)    //4
				v->balance= 0;
			}
	}

	void BalanceTree(char side){

		node*t=this;
		if(t->balance==-2 && t->left->balance==-1){
			t->RotateRight(side);
			PostOrderBalance2(t);
			return;
		}

		if(t->balance==2 && t->right->balance==1){
			t->RotateLeft(side);
			PostOrderBalance2(t);
			return;
		}
		if(t->balance==-2 && t->left->balance==1){
			t->left->RotateLeft('l');
			PostOrderBalance2(t);
			t->RotateRight(side);
			PostOrderBalance2(t);
			return;
		}
		if(t->balance==2 && t->right->balance==-1){
			t->right->RotateRight('r');
			PostOrderBalance2(t->right);
			t->RotateLeft(side);
			PostOrderBalance2(t);
			return;
		}
	}


	void RotateLeft(char prev_side){

		if(prev_side=='H'){
			node *temp=this->right->left;
			this->right->left=this;
			this->previous=this->right;
			this->right->previous=NULL;
			this->right=temp;
			if(this->right) this->right->previous=this;

		}else{
			node *temp=this->right->left;
			this->right->left=this;
		    this->right->previous=this->previous;

			if(prev_side=='l')
				this->previous->left=this->right;
			else 
				if(prev_side=='r') 
					this->previous->right=this->right;
			this->previous=this->right;
			this->right=temp;

		}

	}
	void RotateRight(char prev_side){


		if(prev_side=='H'){
			node *temp=this->left->right;
			this->left->right=this;
			this->previous=this->left;
			this->left->previous=NULL;
			this->left=temp;
			if(this->left) this->left->previous=this;

		}else{
			node *temp=this->left->right;
			
			this->left->right=this;
            this->left->previous=this->previous;
			
			if(prev_side=='l')
				this->previous->left=this->left;
			else 
				if(prev_side=='r')
					this->previous->right=this->left;
            this->previous=this->left;
			this->left=temp;
			if(this->left) this->left->previous=this;
			
		}
	}

	int Gylis(node *n){
		int k1=0, k2=0;
		if(n != NULL){
			if(n->left != NULL)
				k1=Zemyn(n->left)+1;
			if(n->right!= NULL)
				k2=Zemyn(n->right)+1;

			if(k1>=k2) 
				return(k1);
			else 
				return(k2);
		}else return 0;
	}
	int Zemyn(node* n){
		if(n->left != NULL)
			return(Zemyn(n->left)+1);
		else if(n->right!= NULL)
			return(Zemyn(n->right)+1);
		else
			return 0;
	}
	
};

int Generate(int a,int b);
void Read(ifstream &is, int a[], int n);
void RandomWrite(ofstream &os,  int n);
void Show(node* v);
double Tyrimas(node* f, int n);
double Tyrimas2(node* f, int m[], int n);

int main(){

	srand(time(0));
	size_t t1, t2;
	node *TreeHead=NULL;
	node* TestTree=NULL;

	cout<<endl<<"---------------------- AVL medis ------------------------------------"<<endl<<endl;

	double L=0;
	int n; 
	int *a;

	cout<<" Iveskite atsitiktinai sugeneruojamu elementu kieki N= ";
	cin>>n;
	a=new int [n];

	RandomWrite(os,n);
	Read(is,a,n);

	TreeHead=new node(a[0]);

    for(int i=1; i<n; i++){
		TreeHead->InsertNode(a[i]);                // Iterpimas

        TreeHead->PostOrderBalance(TreeHead);      // AVL balansavimas

		if(TreeHead->previous != NULL){            // Virsunes keitimas
			while(TreeHead->previous != NULL) 
				TreeHead=TreeHead->previous;
			TreeHead->PostOrderBalance(TreeHead);
		}
	}
	
	
	TreeHead->NewInOrder();
	
	cout<<endl<<" Faile 'data.txt' uzrasyti atsitiktiniai duomenys.\n I txt faila 'results.txt' uzrasyti infix apejimo be rekursijos\n AVL medzio virsunes"<<endl<<endl;
		    
	system("pause");
    cout<<endl;
	cout<<"----------------- Dvejetainis medis -------------------"<<endl;
	L=0;  n=100;
	double R=50;
	for(int i=0; i<R; i++){
		TestTree= new node(Generate(1,2*n));
		L=L+Tyrimas(TestTree, n);
	}
	cout<<"Bendras laikas: "<<L/R<<endl;
	cout<<"N = "<<n<<"; v.p.l.: "<<L/(R*n)<<endl<<endl;
	
	L=0;  n=200;
	for(int i=0; i<R; i++){
		TestTree= new node(Generate(1,2*n));
		L+=Tyrimas(TestTree, n);
	}
	cout<<"Bendras laikas: "<<L/R<<endl;
	cout<<"N = "<<n<<"; v.p.l.: "<<L/(R*n)<<endl<<endl;

	L=0;  n=400;
	for(int i=0; i<R; i++){
		TestTree= new node(Generate(1,2*n));
		L+=Tyrimas(TestTree, n);
	}
	cout<<"Bendras laikas: "<<L/R<<endl;
	cout<<"N = "<<n<<"; v.p.l.: "<<L/(R*n)<<endl<<endl;
	
	cout<<"----------------- AVL medis -------------------"<<endl;
	
	L=0;  n=100;
    int *m=new int [n];

	for(int i=0; i<R; i++){
		TestTree= new node(Generate(1,2*n));
		for(int i=0; i<n; i++){
			m[i]=Generate(1,2*n);
			TestTree->InsertNode(m[i]);
			TestTree->PostOrderBalance(TestTree);    
			if(TestTree->previous != NULL){         
				while(TestTree->previous != NULL) 
					TestTree=TestTree->previous;
				TestTree->PostOrderBalance(TestTree);
			}
		}
		L+=Tyrimas2(TestTree,m, n);
	}
	cout<<"Bendras laikas: "<<L/R<<endl;
	cout<<"N = "<<n<<"; v.p.l.: "<<L/(R*n)<<endl<<endl;
	delete []m;
	
	L=0;  n=200;
    m=new int [n];

	for(int i=0; i<R; i++){
		TestTree= new node(Generate(1,2*n));
		for(int i=0; i<n; i++){
			m[i]=Generate(1,2*n);
			TestTree->InsertNode(m[i]);
			TestTree->PostOrderBalance(TestTree);    
			if(TestTree->previous != NULL){         
				while(TestTree->previous != NULL) 
					TestTree=TestTree->previous;
				TestTree->PostOrderBalance(TestTree);
			}
		}
		L+=Tyrimas2(TestTree,m, n);
	}
	cout<<"Bendras laikas: "<<L/R<<endl;
	cout<<"N = "<<n<<"; v.p.l.: "<<L/(R*n)<<endl<<endl;
	delete []m;
	//--------------------------------------------------
	L=0;  n=400;
	m=new int [n];
	for(int i=0; i<R; i++){
		TestTree= new node(Generate(1,2*n));
		for(int i=0; i<n; i++){
			m[i]=Generate(1,2*n);
			TestTree->InsertNode(m[i]);
			TestTree->PostOrderBalance(TestTree);    
			if(TestTree->previous != NULL){         
				while(TestTree->previous != NULL) 
					TestTree=TestTree->previous;
				TestTree->PostOrderBalance(TestTree);
			}
		}
		L+=Tyrimas2(TestTree,m, n);
	}
	cout<<"Bendras laikas: "<<L/R<<endl;
	cout<<"N = "<<n<<"; v.p.l.: "<<L/(R*n)<<endl<<endl;
	delete []m;
	//--------------------------------------------------------------
	system("pause");
    return 0;
}

int Generate(int a, int b){     
		return(a+rand()%(b-a));
	}

void Read(ifstream &is, int a[], int n){
	is.open("data.txt");
	for(int i=0; i<n; i++)
		is>>a[i];
	is.close();
}

void RandomWrite(ofstream &os, int n){

	os.open("data.txt");
	for(int i=0; i<n; i++)
		os<<Generate(1,2*n)<<" ";
	os.close();
}

double Tyrimas(node* f, int n){
	    node* temp;
		int *m=new int [n];
		size_t t1, t2;
		
		for(int i=0; i<n; i++){
			m[i]=Generate(1,2*n);
			f->InsertNode(m[i]);
		}

		t1 = clock();
		for(int i=0; i<n; i++)
			temp=f->FindNode(f,m[i]);
		t2 = clock();

		return (double)(t2-t1)/(double)CLOCKS_PER_SEC;
	}

double Tyrimas2(node* f, int m[], int n){
	    node* temp;
        size_t t1, t2;

		t1 = clock();
		for(int i=0; i<n; i++)
			temp=f->FindNode(f,m[i]);
		t2 = clock();

		return (double)(t2-t1)/(double)CLOCKS_PER_SEC;
	}