#include "matrizadj/Grafo.h" 
#include <fstream>
using namespace std;

int main (int argc, char **argv) 
{
    ifstream in ("dfs_input.txt");
    Grafo *grafo = new Grafo (in);
    grafo->imprime ();  
    
    grafo->buscaLargura();
    
}
