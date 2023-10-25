#include "Lista.h" 
#include "FPHeapMinIndireto.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <array>
#include <cstring>
#include <float.h>
using namespace std;

	class Grafo {
	public:
		class Aresta {
	  private:
	    int v1, v2, peso; 
	  public: 
	    Aresta (int v1, int v2, int peso) {
	      this->v1 = v1; this->v2 = v2; this->peso = peso;
	    }
	    int _peso () { return this->peso; }
	    int _v1 () { return this->v1; }
	    int _v2 () { return this->v2; }
      bool operator<(const Aresta &p) const{
        return peso<p.peso;
      }
	    ~Aresta(){}
	  };
	private:	
		class Celula {
    friend class Grafo;
    friend ostream& operator<< (ostream& out, const Celula& celula) {
      out << "vertice:" << celula.vertice << endl;
      out << "peso:"    << celula.peso    << endl;
      return out;
    }    
		private:	
	    int vertice, peso;
	  public:
	    Celula (int v, int p) {
	    	this->vertice = v; this->peso = p;
	    }
	    Celula (const Celula& cel) { *this = cel; }      
      bool operator== (const Celula& celula) const {
        return this->vertice == celula.vertice;
      }
      bool operator!= (const Celula& celula) const {
        return this->vertice != celula.vertice;
      }
      const Celula& operator= (const Celula& cel) {     
        this->vertice = cel.vertice; this->peso = cel.peso;
        return *this; // @{\it permite atribui\c{c}\~oes encadeadas}@
      }      
      ~Celula () {}	    
	  }; 
    Lista<Celula> *adj; 
    int numVertices;
  public:
    Grafo( istream &in );
    Grafo (int numVertices);	  
    Grafo (int numVertices, int numArestas);	  
	  void insereAresta (int v1, int v2, int peso);
	  bool existeAresta (int v1, int v2) const;
	  bool listaAdjVazia (int v) const;
    Aresta *lerAresta ();
	  Aresta *primeiroListaAdj (int v);
	  Aresta *proxAdj (int v);
	  Aresta *retiraAresta (int v1, int v2);
	  void imprime () const ;
    void prim(int raiz);
	  int _numVertices () const;
	  Grafo *grafoTransposto ();
    void buscaProfundidade();
    void visitaDFS(int u, int *cor, int *antecessor);
    bool ciclico();
    int numComponentes();
    vector<int> ordemtopologica();
    void buscaLargura();
    void visitaBfs(int u, int *cor, int *dist, int *antecessor);
    ~Grafo ();
    void caminhocurto(int u, int v);
    void kruskal(); 
    int encontrarConjunto(int *conjunto, int a);
    void unirconjunto(int *conjunto, int x, int y);
	};

  Grafo::Grafo( istream &in )
  {
   int v1, v2, peso, numVertices;
   in >> numVertices;
  
   this->adj = new Lista<Celula>[numVertices]; 
   this->numVertices = numVertices; 	  	  	
   
    while (in>>v1>>v2>>peso) {
      Grafo::Aresta *a = new Grafo::Aresta (v1, v2, peso);
      this->insereAresta (a->_v1 (), a->_v2 (), a->_peso ()); 
      delete a;
    }
  }

  Grafo::Grafo (int numVertices) {
  	this->adj = new Lista<Celula>[numVertices]; 
  	this->numVertices = numVertices; 	  	
  }	  
  Grafo::Grafo (int numVertices, int numArestas) {
  	this->adj = new Lista<Celula>[numVertices]; 
  	this->numVertices = numVertices; 	  	
  }	  

  Grafo::Aresta *lerAresta () {
    cout << "Aresta:" << endl;
    cout << "  V1:"; int v1 = 0;
    cin >> v1;
    cout << "  V2:"; int v2 = 0;
    cin >> v2;
    cout << "  Peso:"; int peso = 0;
    cin >> peso;
    return new Grafo::Aresta (v1, v2, peso);
  }

  void Grafo::insereAresta (int v1, int v2, int peso) {
    Celula item (v2, peso); 
    this->adj[v1].insere (item); 
  }
  bool Grafo::existeAresta (int v1, int v2) const {
    Celula item (v2, 0);
    return (this->adj[v1].pesquisa (item) != NULL);
  }
  bool Grafo::listaAdjVazia (int v) const {
		return this->adj[v].vazia ();  
  }	  
  Grafo::Aresta *Grafo::primeiroListaAdj (int v) {
    // @{\it Retorna a primeira aresta que o v\'ertice v participa ou}@
    // @{\it {\bf NULL} se a lista de adjac\^encia de v for vazia}@ 
    Celula *item = this->adj[v]._primeiro ();    
    return item != NULL ? new Aresta(v,item->vertice,item->peso) : NULL;
  }
  Grafo::Aresta *Grafo::proxAdj (int v) {
    // @{\it Retorna a pr\'oxima aresta que o v\'ertice v participa ou}@
    // @{\it {\bf NULL} se a lista de adjac\^encia de v estiver no fim}@
    Celula *item = this->adj[v].proximo ();    
    return item != NULL ? new Aresta(v,item->vertice,item->peso) : NULL;
  }
  Grafo::Aresta *Grafo::retiraAresta (int v1, int v2) {
    Celula chave (v2, 0);
    Celula *item = this->adj[v1].retira (chave);
    Aresta *aresta = item != NULL ? new Aresta(v1,v2,item->peso) : NULL;
    delete item; return aresta;
  }
  void Grafo::imprime () const {
    for (int i = 0; i < this->numVertices; i++) { 
      cout << i << ":";
      Celula *item = this->adj[i]._primeiro ();
      while (item != NULL) {
        cout << "  " << item->vertice << " (" <<item->peso<< ")";
        item = this->adj[i].proximo ();
      }
      cout << endl;
    }
  }
  int Grafo::_numVertices () const { return this->numVertices; }
  Grafo *Grafo::grafoTransposto () {  	
    Grafo *grafoT = new Grafo (this->numVertices); 
    for (int v = 0; v < this->numVertices; v++)
      if (!this->listaAdjVazia (v)) {//
        Aresta *adj = this->primeiroListaAdj (v);
        while (adj != NULL) {
          grafoT->insereAresta (adj->_v2 (), adj->_v1 (), adj->_peso ());
          delete adj;
          adj = this->proxAdj (v);
        }
      }//
    return grafoT;
  }

  void Grafo::prim(int raiz){
    int n = this->_numVertices();
    int *antecessor = new int[n];
    double *peso = new double[n];
    int *vs = new int[n];
    bool *itensHeap = new bool[n];

    for (int i = 0; i < n; i++)
    {
      peso[i] = 99999999;
      antecessor[i] = -1;
      itensHeap[i] = true;
      vs[i++] = i;
    }
      
    peso[raiz] = 0;

    FPHeapMinIndireto Q(peso, vs, n);
    Q.constroi();
    int i = 0;

    while (!Q.vazio())
    {
    
    int u = Q.retiraMin();
    itensHeap[u] = false;
      if (!this->listaAdjVazia (u)) {
        Aresta *adj = this->primeiroListaAdj (u);
        while (adj != NULL) {
          int v = adj->_v2();
          int pesoA = adj->_peso();

          if(itensHeap[v] && pesoA < peso[v]){
            antecessor[v] = u;
            Q.diminuiChave(v, pesoA);
          }
          
          
          delete adj;
          adj = this->proxAdj (u);
        }
      }

    int pesoT = 0;
    for (int i = 0; i < n; i++)
    {
      cout << i << " : ";
      cout << antecessor[i] << "(" << peso[i] << ")" << endl;
      pesoT += peso[i];
    }
      
    }
    
  }


  void Grafo::buscaProfundidade(){
    int *cor = new int[this->_numVertices()];
    int *antecessor = new int[this->_numVertices()];
    for(int u= 0; u < this->_numVertices(); u++){
      cor[u] = 0;
      antecessor[u] = -1;
    }
    for(int v =0; v < this->_numVertices(); v++){
      if (cor[v] == 0) this->visitaDFS(v, cor, antecessor);
    }
  }

  void Grafo::visitaDFS(int u, int *cor, int *antecessor){
    cor[u] = 1;
    cout << "Cinza: " << u <<endl;
    if (!this->listaAdjVazia (u)) {
        Aresta *adj = this->primeiroListaAdj (u);
        while (adj != NULL) {
          int v = adj-> _v2();
          if(cor[v] == 0){
            antecessor[v] = u;
            this->visitaDFS(v, cor, antecessor);
          }
          delete adj;
          adj = this->proxAdj(u);
        }
      }
      cor[u] = 2;
      cout << "Preto: " << u << endl;
  }

  void Grafo::buscaLargura(){
    int *cor = new int[this->_numVertices()];
    int *antecesor = new int[this->_numVertices()];
    int *dist = new int[this->_numVertices()];
    for (int i = 0; i < this->_numVertices();  i++)
    {
        cor[i]= 0;
        antecesor[i] = -1;
        dist[i] = 999;
    }
    for (int i = 0; i < this->numVertices; i++)
    {
      if(cor[i] == 0){
        visitaBfs(i, cor, antecesor, dist);
      }
    }
  }

  void Grafo::visitaBfs(int u, int *cor, int *dist, int *antecessor){
    queue <int> fila;
    dist[u] = 0;
    cor[u] = 0;
    fila.push(u);
    while (!fila.empty())
    {
      u= fila.front();
      fila.pop();
      Aresta *adj = this->primeiroListaAdj(u);
      cout<< "visitou" << endl;
      while (!this->listaAdjVazia(u))
      {
        while (adj != NULL)
        {
          int v = adj->_v2();
          if(cor[v]==0){
            antecessor[u] = 1;
            dist[v]= dist[u]+1;
            cor[v]= 1;
            fila.push(v);
          }
          delete adj;
          adj = this->proxAdj(u);
        }
        cor[u] = 2;
      }
    }      
  }
 

  void Grafo::caminhocurto(int u, int v){
  int *antecessor = new int[this->_numVertices()];
  if(u == v) cout << v << endl;
  
  if(antecessor[v] = -1) cout << "Não existe caminho de U para V" << endl;

  else {
    caminhocurto(u, antecessor[v]);
    cout << "" << endl;
  }

  }

  void Grafo::kruskal(){
    vector<Aresta> S;
    int *conjunto = new int[this->_numVertices()];

    memset(conjunto, -1, sizeof(int)*this->_numVertices());

    vector<Aresta> A;

    for (int v = 0; v < this->numVertices; v++){
      if (!this->listaAdjVazia (v)) {
        Aresta *adj = this->primeiroListaAdj (v);
        while (adj != NULL) {
          A.push_back(*adj);
          delete adj;
          adj = this->proxAdj (v);
        }
      }
    }
    

    sort(A.begin(), A.end());

    for (int i = 0; i <= numVertices; i++)
    {
      if (encontrarConjunto(conjunto, A[i]._v1()) != encontrarConjunto(conjunto, A[i]._v2()))
      {
        S.push_back(A[i]);
        
        unirconjunto(conjunto, A[i]._v1(), A[i]._v2());
      }
    }
    for (int i = 0; i < S.size(); i++)
    {
      cout<< S[i]._v1()<< " " << S[i]._v2() << " " << S[i]._peso() <<endl;
    }
    
      
  
  }


  int Grafo::encontrarConjunto(int *conjunto, int a){
    if (conjunto[a] == -1)
    {
      return a;
    }
    return encontrarConjunto(conjunto, conjunto[a]);
  }


  void Grafo::unirconjunto(int *conjunto, int x, int y){
    int conjuntoX= encontrarConjunto(conjunto, x);
    int conjuntoY = encontrarConjunto(conjunto, y);
    conjunto[conjuntoX] = conjuntoY;

  }

		
