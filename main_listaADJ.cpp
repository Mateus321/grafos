#include "listaadj/Grafo.h" 
#include <fstream>
using namespace std;

int main (int argc, char **argv) 
{
    ifstream in ("exemplo5.txt");
    Grafo *grafo = new Grafo (in);
    //grafo->imprime ();  
    
    grafo->dijkstra(0);
    
}
