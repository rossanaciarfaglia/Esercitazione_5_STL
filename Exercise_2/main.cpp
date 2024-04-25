#include<iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;
int main()
{
    PolygonalMesh mesh;
    string filepath="PolygonalMesh";    //percorso del file

    if(!ImportMesh(filepath,            //viene richiamata la funzione passando il percorso del file e l'oggetto mesh
                    mesh))
    {
        return 1;
    }


    return 0;


}
