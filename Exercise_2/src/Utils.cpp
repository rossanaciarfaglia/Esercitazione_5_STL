#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;

namespace PolygonalLibrary{
bool ImportMesh(const string& filepath,
                PolygonalMesh& mesh){
    if(!ImportCell0Ds(filepath +"/Cell0Ds.csv",mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:"<< endl;

        // stampo coppie chiave-valore
        for (auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++){
            cout << "key: "<< it->first << " values: ";
            for (const unsigned int id : it ->second) // stampo lista valori associati alla chiave
                cout<<" "<< id;
            cout<< endl;
        }
    }

    if(!ImportCell1Ds(filepath+ "/Cell1Ds.csv", mesh)){
        return false;
    }
    else
    {
        cout<< "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++){

            cout <<"Key: "<< it-> first << " values: ";
            for(const unsigned int id : it-> second)
                cout <<" " << id;
            cout<< endl;
        }
    }


    /*if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        for (const auto& riga : mesh.EdgesCell2D) {
            // Per ogni riga, iteriamo su ogni elemento e lo stampiamo.
            for (int elemento : riga) {
                cout << elemento << " ";
            }
            // Alla fine di ogni riga, stampiamo una nuova linea.
            cout << endl;
        }
        // Test:
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            vector<unsigned int> edges = mesh.EdgesCell2D[c];

            for(unsigned int e = 0; e < size(mesh.EdgesCell2D[c]); e++)
            {
                //per ogni spigolo della cella 2D si accede ai vertici di origine e di fine
                //e si cerca la presenza di questi vertici tra quelli della cella 2D stessa
                //se non vengono trovati la cella è inconsistente
                const unsigned int origin = mesh.VerticesCell2D[edges[e]][0];  //verifico se vertice inizio e fine coincidono con gli estremi dei lati
                const unsigned int end = mesh.VerticesCell2D[edges[e]][size(mesh.VerticesCell2D[c])];
                cout<<"inizio "<<origin<<endl;
                auto findOrigin = find(mesh.VerticesCell2D[c].begin(), mesh.VerticesCell2D[c].end(), origin);
                cout<<"inizio "<<findOrigin<<endl;
                if(findOrigin == mesh.VerticesCell2D[c].end())
                {
                    cerr << "Wrong mesh qui" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.VerticesCell2D[c].begin(), mesh.VerticesCell2D[c].end(), end);
                if(findEnd == mesh.VerticesCell2D[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }

            }
        }
    }*/



    // call ImportCell2Ds
    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",mesh))
    {
        return false;
    }
}

bool test_area(const PolygonalMesh& mesh, double tol)
{
    for (size_t i=0; i < mesh.idCell1D.size(); ++i) {
    //estrazione dell'Id dei vertici
        int origin = mesh.VerticesCell1D[i][0];
        int end= mesh.VerticesCell1D[i][1];

    //ne trovo le coordinate
        double X_origin = mesh.CoordinatesCell0Ds[origin][0];
        double Y_origin = mesh.CoordinatesCell0Ds[origin][1];
        double X_end = mesh.CoordinatesCell0Ds[end][0];
        double Y_end = mesh.CoordinatesCell0Ds[end][1];

    //distanza euclidea
        double d = sqrt((X_origin-X_end)*(X_origin-X_end) + (Y_origin-Y_end)*(Y_origin-Y_end));
        if(d<tol){
            cerr<<"l'area è nulla"<<endl;
            return false;
        }
        return true;
    }
}



bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh){

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;
    list<string> listLines;
    string line;
    while(getline(file,line))
        listLines.push_back(line); // aggiungere elemento a fine lista
    file.close();

    listLines.pop_front(); // togliere elemento inizio lista
    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "Non ci sono celle 0D" << endl;
        return false;
    }

    // preallocare la memoria
    mesh.idCell0D.reserve(mesh.NumberCell0D);
    mesh.CoordinatesCell0Ds.reserve(mesh.NumberCell0D);

    for (const string& line: listLines)
    {
        istringstream  converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;
        char delimiter;
        converter >> id >> delimiter>>marker>> delimiter >> coord(0)>> delimiter >> coord(1);

        mesh.idCell0D.push_back(id);
        mesh.CoordinatesCell0Ds.push_back(coord);

        if(marker != 0){
            auto ret = mesh.Cell0DMarkers.insert({marker,{id}});
            // si inserisce una nuova coppia chiave-valore
            // ret contiene iteratore posizione nuovo elemento inserito

            if(!ret.second)
                // inserimento non avvenuto con successo
                (ret.first)->second.push_back(id);
        }
    }
    file.close();
    return true;

}








bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);
    if(file.fail())
        return false;
    list<string> listLines;
    string line;
    while (getline(file,line))
        listLines.push_back(line);
    file.close();

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if(mesh.NumberCell1D == 0)
    {
        cerr<< "Non ci sono celle 1D" << endl;
        return false;
    }
    mesh.idCell1D.reserve(mesh.NumberCell1D);
    mesh.VerticesCell1D.reserve(mesh.NumberCell1D);

    for (const string& line: listLines){
        istringstream converter(line);
        unsigned int id;
        unsigned int marker;
        Vector2d vertices;
        char delimiter;
        converter >> id >>delimiter>> marker >>delimiter>> vertices(0) >>delimiter>> vertices(1);

        mesh.idCell1D.push_back(id);
        mesh.VerticesCell1D.push_back(vertices);

        if(marker != 0){
            auto ret = mesh.Cell1DMarkers.insert({marker,{id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();
    return true;
}





bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);
    if(file.fail())
        return false;
    list<string> listLines;
    string line;
    while (getline(file,line))
        listLines.push_back(line);
    file.close();

    listLines.pop_front();

    /*for (const string& elemento : listLines) {
        cout << elemento << endl;
    }*/

    mesh.NumberCell2D = listLines.size();

    if(mesh.NumberCell2D == 0)
    {
        cerr<< "Non ci sono celle 2D" << endl;
        return false;
    }


    mesh.idCell2D.reserve(mesh.NumberCell2D);
    mesh.VerticesCell2D.reserve(mesh.NumberCell2D);
    mesh.EdgesCell2D.reserve(mesh.NumberCell2D);


    bool length_zero = false;
    for (const string& line: listLines){
        istringstream converter(line);
        unsigned int id;
        unsigned int marker;
        unsigned int NumVertic;
        unsigned int NumEdges;
        unsigned int vertice;
        unsigned int lato;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;
        char delimiter;
        converter >> id >>delimiter>> marker >>delimiter>> NumVertic>>delimiter;


        vertices.resize(NumVertic);
        mesh.idCell2D.push_back(id);
        vertices.clear();
        for (unsigned int i = 0; i<NumVertic; i++){
            converter>> vertice>>delimiter;
            vertices.push_back(vertice);

        }
        mesh.VerticesCell2D.push_back(vertices);
        converter>>NumEdges>>delimiter;

        edges.resize(NumEdges);
        edges.clear();
        for (unsigned int i = 0; i<NumEdges; i++){
            converter>>lato>>delimiter;
            edges.push_back(lato);

        }

        for (unsigned int i = 0; i<size(edges)-1; i++) {
            if (norm(edges[i] - edges[i+1]) < 10*numeric_limits<double>::epsilon()){
                bool length_zero = true;
            }
        }

        mesh.EdgesCell2D.push_back(edges);

    }
    cout << "length_zero: " << (length_zero ? "true" : "false") << endl;

    file.close();
    return true;
}
}
