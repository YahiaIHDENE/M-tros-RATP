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
                uint64_t id = 0;
                Station stat;
                getline(lire, stat.name, ',');
                getline(lire, stat.line_id, ',');
                try
                {
                    id = stoi(stat.line_id);
                }
                catch (const std::invalid_argument &e){}
                getline(lire, stat.line_name, ',');
                getline(lire, stat.address, '\n');
                if (id != 0)
                    stations_hashmap[id] = stat;
        }
        lire.close();
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
    carte.read_connections("c.csv");
    carte.read_stations("s.csv");
    std::unordered_map<uint64_t, uint64_t> duree;
    std::unordered_map<uint64_t, uint64_t> previous;
    std::vector<std::pair<uint64_t, uint64_t>> path1;

    vector<uint64_t> stations;
    auto comparator = [&](uint64_t left, uint64_t right) { return duree[left] > duree[right]; };

    if(_start==_end)
    {
        cout<<"la duree est 0, vous etes dans arrivee"<<endl;
        exit (1);
    }
    for (auto i : carte.get_connections_hashmap())
    {
        if(i.first == _start)
        {
            duree[i.first]=0; //la distance d'une station avec elle même est de 0 sec
        }
        else
        {
            duree[i.first] = numeric_limits<uint64_t>::max(); //la distance d'une station avec les autre station = oo
        }
        //mettre toutes les tations dans le tas "avec la boucle"
        stations.push_back(i.first);
        push_heap(begin(stations),end(stations),comparator); 
    }

    while (!stations.empty())
    {
        pop_heap(begin(stations), end(stations), comparator); //extraire le dernier element du tas avec la valeur la plus petit et le mettre dans last-1
        uint64_t optimal = stations.back();
        stations.pop_back();

        if (optimal == _end)
        {
            while (previous.find(optimal) != end(previous))
            {
                path1.push_back(std::pair<std::uint64_t, uint64_t>(optimal, duree[optimal]));
                optimal = previous[optimal];
            }

            break;
        }

        if (duree[optimal] == numeric_limits<uint64_t>::max())
        {
            break;
        }

        for (auto j : carte.get_connections_hashmap().at(optimal))
        {
            int time = duree[optimal] + j.second;
            if (time < duree[j.first])
            {
                duree[j.first] = time;
                previous[j.first] = optimal;
                make_heap(begin(stations), end(stations), comparator); // Réorganiser les élements de stations pour former un tas
            }
        }
    }
    
        return path1;
}

//la fonction string_to_id sert a trouver l'id d'une station a partir de son
