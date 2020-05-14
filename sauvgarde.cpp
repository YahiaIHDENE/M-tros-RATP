#include <iostream> //std::cout, std::cin...
#include <string>   
#include <fstream>
#include<vector>
#include <unordered_map>
#include <iterator>  //std::
#include <algorithm>
#include <limits>
#include "Lecture.hpp"

using namespace std;
using namespace travel;

 //**********************************************************************************//
 //                        Fonction Read Station                                     //
 //**********************************************************************************//
void Lecture::read_stations(const std::string &_filename)
{
    //ouverture du fichier
    ifstream lire(_filename.c_str());
    if (lire.fail())
        cerr<<"Erreur: impossible de lire le fichier!!"<<endl;
    else
    {
        string ligne;
        getline(lire, ligne); //sauter la premier ligne du fichier csv
        while (!lire.eof())
        {
            int cpt = 0;    //compteur pour deviser la ligne en 4 champs
            Station tran;
            string ligne;
            uint64_t id;
            getline(lire, ligne);
            string::iterator j(ligne.begin()); //iterateur defini le debut du champ a copier
            for (string::iterator i(ligne.begin()); i != ligne.end(); i++)
            {
                if (*i == (','))
                {
                    // 1er champ pour le nom du la statio
                    if (cpt == 0)
                    {
                        copy(j, i, tran.name.begin());
                        j = i + 1;
                    }
                    //2eme champs pour le identifiant du metro
                    if (cpt == 1)
                    {
                        copy(j, i, tran.line_id.begin());
                        j = i + 1;
                        id = stol(tran.line_id, nullptr, 10);
                    }
                    //3eme champ pour  le numero du metro
                    if (cpt == 2)
                    {
                        copy(j, i, tran.line_name.begin());
                        j = i + 1;
                    }
                    //4eme champ pour l'adresse du metro
                    if (cpt == 3)
                    {
                        copy(j, i, tran.address.begin());
                        j = i + 1;
                    }
                    cpt++;

                    if (id != 0) // filtrer l'attribution de 0 a l'id combiner a d'autre champ
                        stations_hashmap[id] = tran;
                }
            }
        }
        lire.close();
        std::cout << "fichier est ferme!" << endl;
    }
}

//**********************************************************************************//
//                       Fonction Read connections                                  //
//**********************************************************************************//
void Lecture::read_connections(const std::string &_filename)
{
    ifstream lire(_filename.c_str());
    if (lire.fail())
        cerr << "Erreur: impossible de lire le fichier!!" << endl;
    else
    {
        string ligne;
        getline(lire, ligne);
        while (!lire.eof())
        {

            string ligne, transit = "            ";
            uint64_t idDepart = 0, idRetour = 0, temps = 0;
            int cpt = 0; //compteur pour deviser la ligne en 3 champs

            getline(lire, ligne); //sauter la 1er ligne du fichier csv
            string::iterator j(ligne.begin());
            for (string::iterator i(ligne.begin()); i != ligne.end(); i++)
            {

                if (*i == (','))
                {
                    // 1er champ pour l'identifiant du depart
                    if (cpt == 0)
                    {
                        transit = "            ";
                        copy(j, i, transit.begin());
                        idDepart = stol(transit, nullptr, 10);
                        j = i + 1;
                    }
                    //2eme champ pour l'identifiant d'arrivee
                    if (cpt == 1)
                    {
                        transit = "            ";
                        copy(j, i, transit.begin());
                        j = i + 1;
                        idRetour = stol(transit, nullptr, 10);
                        cpt++; //sert a forcer le compteur pour prendre en compte le dernier champ sans virgule
                    }
                    // 3eme champs pour le temps du parcour entre les 2 arret
                    if (cpt == 2)
                    {
                        transit = "            ";
                        copy(j, ligne.end(), transit.begin());
                        temps = stol(transit, nullptr, 10);
                    }
                    cpt++;
                }

                if ((idRetour != 0) && (idDepart != 0)) // pour eleminer la combainison avec 0 exemple iddepart =10 id retour = 0 => temps = 0

                    connections_hashmap[idDepart][idRetour] = temps;
            }
        }
        lire.close();
    }
}

//**********************************************************************************//
//                             Compute Travel                                       //
//**********************************************************************************//

std::vector<std::pair<uint64_t, uint64_t>> Lecture :: compute_and_display_travel(uint64_t _start, uint64_t _end)
{
    vector<pair<uint64_t, uint64_t>> v;

    cout << "hiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii" << endl;

    return(v);
}

std::vector<std::pair<uint64_t, uint64_t>> Lecture ::compute_travel(uint64_t _start, uint64_t _end)
{
    Lecture carte;
    carte.read_connections("connections.csv");
    carte.read_stations("stations.csv");

    std::unordered_map<uint64_t, uint64_t> distances;
    std::unordered_map<uint64_t, uint64_t> previous;
    vector<pair<uint64_t, uint64_t>> path1;

    vector<uint64_t> nodes;
    vector<uint64_t> path;
    int trajet;
    auto comparator = [&](uint64_t left, uint64_t right) { return distances[left] > distances[right]; };

    for (auto i : carte.get_connections_hashmap())
    {
        if(i.first == _start)
        {
            distances[i.first]=0; //la distance d'une station avec elle même est de 0 sec
        }
        else
        {
            distances[i.first] = numeric_limits<uint64_t>::max(); //la distance d'une station avec les autre station = oo
        }
        //mettre toutes les tations dans le tas "avec la boucle"
        nodes.push_back(i.first);
        push_heap(begin(nodes),end(nodes),comparator); 
    }

    while (!nodes.empty())
    {
        pop_heap(begin(nodes), end(nodes), comparator); //extraire le dernier element du tas avec la valeur la plus petit et le mettre dans last-1
        uint64_t smallest = nodes.back();
        nodes.pop_back();

        //Affichage du tas de toutes les stations
        /* std::cout << "Open list: ";
        for (std::vector<uint64_t>::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
           std::cout << *i <<' ';
           std::cout<< std::endl;*/

        if (smallest == _end)
        {
            while (previous.find(smallest) != end(previous))
            {
                path.push_back(smallest);
                smallest = previous[smallest];
                //std::cout << "Closed list: ";
               // for (std::vector<uint64_t>::const_iterator i = path.begin(); i != path.end(); ++i)
                   // std::cout << *i << std::endl;
            }

            break;
        }

        if (distances[smallest] == numeric_limits<uint64_t>::max())
        {
            break;
        }

        for (auto j : carte.get_connections_hashmap().at(smallest))
        {
            int alt = distances[smallest] + j.second;
            if (alt < distances[j.first])
            {
                distances[j.first] = alt;
                previous[j.first] = smallest;
                trajet = alt;
                make_heap(begin(nodes), end(nodes), comparator); // Réorganiser les élements de nodes pour former un tas
            }

        }

    }

    cout << "\t============================================\n";
    cout << "depart: " << _start << "===>";
    for (std::vector<uint64_t>::const_iterator i = path.end()-1; i != path.begin(); --i)
        std::cout << *i <<" ===> ";
    cout << _end <<"  vous êtes arrivé!!"<< endl;
    cout<< "Le trajet est estimé à "<<trajet<<" sec"<<endl;
    return path1;
}

    /*
std::vector<std::pair<uint64_t, uint64_t>> Lecture ::compute_travel(uint64_t _start, uint64_t _end)
{
    vector<pair<uint64_t, uint64_t>> path;
    std::unordered_map<uint64_t, uint64_t> actuel;
    std::unordered_map<uint64_t, uint64_t> preced;
    //std::unordered_map<uint64_t, uint64_t> path;
    actuel[_start] = 0;
    preced[_start] = 0;
    double TotalTime = 0;
    for (auto i : preced)
    {
        
        cout<<" station de depart : "<<i.first<<" --- son temps est : "<<i.second<<endl;
    
    }
    Lecture carte;
    carte.read_connections("connections.csv");
    carte.read_stations("stations.csv");

// Reccuperation des Identifiants de toute les corresp
    
    
    for (auto i : preced)
    {
        i.first = station
        while (i.first != _end)
        {
            for (auto i : carte.get_connections_hashmap())
            {
                if ((i.first == _start))
                {
                    for (auto j : i.second)
                    {
                        actuel[j.first] = j.second;
                    }
                }
            }
            
        }
    }

            for (auto i : actuel)
            {
                std::cout << "id station: [" << i.first << "] ||  temps de deplacement: ==> " << i.second << " secondes" << endl;
            }
    return path;
}
*/