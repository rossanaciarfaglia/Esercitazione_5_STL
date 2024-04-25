//qui costruisco funzioni di import della mesh
#pragma once
#include<iostream>
#include "PolygonalMesh.hpp"

using namespace std;
namespace PolygonalLibrary{



//importo le celle 0D dal file e controllo il corretto inserimento
bool ImportCell0Ds(const string &filepath, PolygonalMesh& mesh);

//importo la mesh ed effettuo un controllo sul corretto inserimento dei dati
bool ImportMesh(const string &filepath, PolygonalMesh& mesh);


//importo le celle 1D dal file e controllo il corretto inserimento
bool ImportCell1Ds(const string &filepath, PolygonalMesh &mesh);

//importo le celle 2D dal file e controllo il corretto inserimento
bool ImportCell2Ds(const string &filepath, PolygonalMesh &mesh);


//test sull'area
bool test_area(PolygonalMesh &mesh, double tol);
//corretto se ritorna true, altrimenti false

}

