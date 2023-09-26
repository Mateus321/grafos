#include "matrizadj/Grafo.h" 
#include <fstream>
using namespace std;

int main (int argc, char **argv) 
{
    ifstream in ("teste.txt");
    Grafo *grafo = new Grafo (in);
    //grafo->imprime ();  

    Grafo *grafoND = grafo->grafoNaoDirecionado();
    grafoND->imprime();

    if(grafoND->completo()) cout<<"Completo" <<endl;
    if(grafoND->regular()) cout<<"Regular" <<endl;
    if(grafoND->euleriano()) cout<<"Euleriano" <<endl;
    if(grafoND->subeuleriano()) cout<<"Subeuleriano" <<endl;

    if(grafoND->direcionado()) cout<<"Direcionado"<<endl;

    //Ex1: criar e imprimir grafo transposto.
    
    


    //Ex2: Criar e imprimir um grafo não orientado.
    /*
    //Ex3: Ler e inserir nova aresta, verificando se ela já existe.
    Grafo::Aresta *a = grafo->lerAresta();
    if(!grafo->existeAresta(a->_v1(), a->_v2()))
        grafo->insereAresta(a->_v1(), a->_v2(), a->_peso());
    else cout<< "Aresta já existente!";
    grafo->imprime();

    //Ex4: Ler e remover uma aresta, verificando se ela já existe.
    Grafo::Aresta *a = grafo->lerAresta();
    if(grafo->existeAresta(a->_v1(), a->_v2()))
        grafo->retiraAresta(a->_v1(), a->_v2());
    else cout<< "Aresta não existente!";
    grafo->imprime();*/
    //Ex5: Imprimir uma lista adjacente de um determinado vértices.
}
