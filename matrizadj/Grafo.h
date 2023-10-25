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
	    ~Aresta () {}
	  };
	private:
	  int **mat; // @{\it pesos do tipo inteiro}@
    int numVertices;
    int *pos; // @{\it posi\c{c}\~ao atual ao se percorrer os adjs de um v\'ertice v}@

  public:
    Grafo (int numVertices);
    Grafo(istream &in);
    Grafo(int numVertices, int numArestas);
    void insereAresta(int v1, int v2, int peso);
    bool existeAresta(int v1, int v2) const;
    bool listaAdjVazia(int v) const;
    Aresta *lerAresta();
    Aresta *primeiroListaAdj(int v);
    Aresta *proxAdj(int v);
    Aresta *retiraAresta(int v1, int v2);
    void imprime() const;
    int _numVertices() const;
    Grafo *grafoTransposto();
    Grafo *grafoNaoDirecionado();
    void listaAdj(int v);
    int grauVertice(int v);
    bool completo();
    bool regular();
    bool euleriano();
    bool subeuleriano();
    bool direcionado();
    ~Grafo ();	 
    void buscaLargura();
    void visitaBfs(int u, int *cor, int *dist, int *antecessor);
    void caminhocurto(int u, int v);  
	};

  Grafo::Grafo( istream &in )
  {
   int v1, v2, peso;
   in >> this->numVertices;
   this->mat = new int*[numVertices];
    for (int i = 0; i < numVertices; i++) 
      this->mat[i] = new int[numVertices];
    this->pos = new int[numVertices]; 
    
    for (int i = 0; i < this->numVertices; i++) {
      for (int j = 0; j < this->numVertices; j++) this->mat[i][j] = 0;
      this->pos[i] = -1; 
    }

    while (in>>v1>>v2>>peso) {
      Grafo::Aresta *a = new Grafo::Aresta (v1, v2, peso);
      this->insereAresta (a->_v1 (), a->_v2 (), a->_peso ()); 
      delete a;
    }
  }

  Grafo::Grafo (int numVertices) {
    this->mat = new int*[numVertices];
    for (int i = 0; i < numVertices; i++) 
      this->mat[i] = new int[numVertices];
    this->pos = new int[numVertices]; 
    this->numVertices = numVertices; 
    for (int i = 0; i < this->numVertices; i++) {
      for (int j = 0; j < this->numVertices; j++) this->mat[i][j] = 0;
      this->pos[i] = -1; 
    }
  }	  
  
  Grafo::Grafo (int numVertices, int numArestas) {
    this->mat = new int*[numVertices];
    for (int i = 0; i < numVertices; i++) 
      this->mat[i] = new int[numVertices];
    this->pos = new int[numVertices]; 
    this->numVertices = numVertices; 
    for (int i = 0; i < this->numVertices; i++) {
      for (int j = 0; j < this->numVertices; j++) this->mat[i][j] = 0;
      this->pos[i] = -1; 
    }
  }	  

  void Grafo::insereAresta (int v1, int v2, int peso) {
    this->mat[v1][v2] = peso; 
  }
  bool Grafo::existeAresta (int v1, int v2) const {
    return (this->mat[v1][v2] > 0);
  }
  bool Grafo::listaAdjVazia (int v) const {
    for (int i = 0; i < this->numVertices; i++)
      if (this->mat[v][i] > 0) return false;
    return true;
  }

  Grafo::Aresta *Grafo::lerAresta()
  {
    cout << "Aresta:" << endl;
    cout << "  V1:"; int v1 = 0;
    cin >> v1;
    cout << "  V2:"; int v2 = 0;
    cin >> v2;
    cout << "  Peso:"; int peso = 0;
    cin >> peso;
    return new Aresta (v1, v2, peso);
  }

  Grafo::Aresta *Grafo::primeiroListaAdj (int v) {
    // @{\it Retorna a primeira aresta que o v\'ertice v participa ou}@ 
    // @{\it {\bf NULL} se a lista de adjac\^encia de v for vazia}@
    this->pos[v] = -1; return this->proxAdj (v);
  }
  
  Grafo::Aresta *Grafo::proxAdj (int v) {
    // @{\it Retorna a pr\'oxima aresta que o v\'ertice v participa ou}@ 
    // @{\it {\bf NULL} se a lista de adjac\^encia de v estiver no fim}@ 
    this->pos[v] ++;
    while ((this->pos[v] < this->numVertices) && 
           (this->mat[v][this->pos[v]] == 0)) this->pos[v]++;
    if (this->pos[v] == this->numVertices) return NULL;
    else return new Aresta (v, this->pos[v], this->mat[v][this->pos[v]]);
  }
  
  Grafo::Aresta *Grafo::retiraAresta (int v1, int v2) {
    if (this->mat[v1][v2] == 0) return NULL; // @{\it Aresta n\~ao existe}@
    else {
      Aresta *aresta = new Aresta (v1, v2, this->mat[v1][v2]);
      this->mat[v1][v2] = 0; return aresta;
    }
  }
  
  void Grafo::imprime () const {
    cout << "   ";
    for (int i = 0; i < this->numVertices; i++) 
      cout << i << "   "; 
    cout << endl;
    for (int i = 0; i < this->numVertices; i++) { 
      cout << i <<  "  ";
      for (int j = 0; j < this->numVertices; j++)
        cout << this->mat[i][j] << "   ";
      cout << endl;
    }
  }
  
  int Grafo::_numVertices () const { return this->numVertices; }
  
  Grafo *Grafo::grafoTransposto () {
    Grafo *grafoT = new Grafo (this->numVertices); 
    for (int v = 0; v < this->numVertices; v++)
      if (!this->listaAdjVazia (v)) {
        Aresta *adj = this->primeiroListaAdj (v);
        while (adj != NULL) {
          grafoT->insereAresta (adj->_v2 (), adj->_v1 (), adj->_peso ());
          delete adj;
          adj = this->proxAdj (v);
        }
      }
    return grafoT;
  }

  Grafo *Grafo::grafoNaoDirecionado () {
    Grafo *grafoND = new Grafo (this->numVertices); 
    for (int v = 0; v < this->numVertices; v++)
      if (!this->listaAdjVazia (v)) {
        Aresta *adj = this->primeiroListaAdj (v);
        while (adj != NULL) {
          grafoND->insereAresta (adj->_v2 (), adj->_v1 (), adj->_peso ());
          grafoND->insereAresta (adj->_v1 (), adj->_v2 (), adj->_peso ());
          delete adj;
          adj = this->proxAdj (v);
        }
      }
    return grafoND;
  }

  void Grafo::listaAdj(int v){
    for (int i = 0; i < this->numVertices; i++)
    {
      if(this->mat[v][i]!= 0) cout<<i<<" ";
    }
    
  }

  int Grafo::grauVertice(int v){
    int g = 0;
    for(int j =0; j < this->numVertices; j++){
      if(this->existeAresta(v, j)) g++;
    }
    return g;
  }

  bool Grafo::completo(){
    for (int i = 0; i < this->numVertices; i++){
      if (this->grauVertice(i) != (this->numVertices) - 1){
        return false;
      }
      
    }
    return true;
  }

  bool Grafo::regular(){
    int grau = grauVertice(0);
    for (int i = 0; i < this->numVertices; i++)
    {
      if(grau != grauVertice(i)) return false;
      
      
    }
    return true;
  }

  bool Grafo::euleriano(){
    for (int i = 0; i < this->numVertices; i++)
    {
      if (grauVertice(i)%2 == 0) return true;

      return false; 
    }
  }

  bool Grafo::subeuleriano(){
    int imp = 0;
    for (int i = 0; i < this->numVertices; i++){
      if(grauVertice(i)%2 != 0) imp++;
    }
    if(imp/2 == 0) return true;

    else return false;
  }

  bool Grafo::direcionado(){
    for (int i = 0; i < this->numVertices; i++){
      for (int j = 0; j < this->numVertices; j++){
        if(this->mat[i][j]!= this->mat[j][i]) return false;
      }
    } 
    return true;
  }

  Grafo::~Grafo()
  {
    for (int i = 0; i < numVertices; i++) 
      delete [] this->mat[i];
    delete [] this->mat;
    delete [] this->pos;
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


		
