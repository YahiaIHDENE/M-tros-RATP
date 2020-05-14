#include "Lecture.cpp"


using namespace std;
using namespace travel;

vector<uint64_t> name_to_id(string arret, std::unordered_map<uint64_t, Station> hasmap_station)
{
    vector<uint64_t> path;
    for (auto i : hasmap_station)
    {
        if (i.second.name == arret)
        {
            path.push_back(i.first);
        }
    }
    return path;
}

void id_to_name(uint64_t ident, std::unordered_map<uint64_t, Station> hasmap_station)
{
    string station;
    for (auto i : hasmap_station)
    {
        if (i.first == ident)
            std::cout << i.second.name << "  prenez la ligne [" << i.second.line_name << "] ====> ";
    }
}




     int main()
{

    Lecture carte;
    carte.read_connections("c.csv");
    carte.read_stations("s.csv");

    vector<uint64_t> idaller, idarrivee;
    string aller, arrivee ;
    uint64_t temps_min = numeric_limits<uint64_t>::max();
    std::vector<std::pair<uint64_t, uint64_t>> path_min, path;
    string accord = "yes";



    std::cout << "\t===================================================================" << std::endl;
    std::cout << "\t\t Bienvenue à l'application d'iteniraire RATP!!! :) " << std::endl;
    std::cout << "\t===================================================================" << std::endl;

    while(accord == "yes"){

    std::cout<<"\nsaisissez la station du depart :";
    std::cin>> aller;
    std::cout << endl;
    std::cout << "saisissez la station d'arriver : ";
    std::cin >> arrivee;
    std::cout << endl;


idaller = name_to_id(aller, carte.get_stations_hashmap());
idarrivee = name_to_id(arrivee, carte.get_stations_hashmap());

for (std::vector<uint64_t>::const_iterator i = idaller.begin(); i != idaller.end(); ++i)
{
    for (std::vector<uint64_t>::const_iterator j = idarrivee.begin(); j != idarrivee.end(); ++i)
    {
        path = carte.compute_travel(*i, *j);
        

        if (path.front().second < temps_min)
        {
            temps_min = path.front().second;
            path_min = path;
            
            if((i==idaller.end()-1)&&(j==idarrivee.end()-1))
                std::cout << aller << " prenez la ligne [" << *i << "]===> ";
                for (std::vector<std::pair<uint64_t, uint64_t>>::const_iterator i = path_min.end() - 1; i != path_min.begin(); --i)
                {
                    uint64_t a = i->first;
                    id_to_name(a, carte.get_stations_hashmap());
                }
                std::cout << arrivee << "vous etes arrivé" << endl;

                std::cout << "le temps du trajet est estimé a :" << path.front().second << endl;
            
        }
    }
    }
    /*
    cout << "\t============================================\n";
    cout << "depart: " << aller << "===>";
    for (std::vector<std::pair<uint64_t, uint64_t>>::const_iterator i = path_min.end() - 1; i != path_min.begin(); --i){
        std::cout << i->first << " ===> ";

    }
    cout << arrivee << "  vous êtes arrivé!!" << endl;
     std::cout<<"le temps du trajet est estimé a :"<<path.front().second<<endl;;   
*/

    std::cout
        << "\n**************\tvoullez-vous continuer? tapez **<yes>** voullez-vous quitter? appuez sur n'importe quelle touche\t************** " << endl;
    std::cin >> accord;
    if(accord != "yes")
    {
        std::cout << "\ndeseriez-vous vraiment quitter?\nsi c'est une erreur appuer sur **<yes>** sinon appuez sur n'importe quelle touche\t************** " << '\n';
        std::cin >> accord;
    }
    }
    
    cout << "\n**************\tau revoir!! :)\t**************\n"<<endl;
     return 0;
}
