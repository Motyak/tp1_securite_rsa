#include <iostream>     //std::cout
#include <algorithm>    //std::find_if
#include <string>       //std::stoi std::to_string
#include <vector>       //std::vector

using llong = long long;

std::pair<int,llong> getKeyFromString(std::string key);
std::string dechiffrer(std::string m, int tailleBloc, std::pair<int,llong> privKey);
    int dechiffrer(int m, std::pair<int,llong> privKey);
        int modExp(int a, int b, int n);

        bool validKeyFormat(std::string key);
        bool is_number(const std::string& s);

// g++ -o bin/decrypt src/decrypt.cpp && bin/decrypt "45067 57433 36197" "$(< priv.key)"
int main(int argc, char* argv[])
{
    // if(argc != 3 || !validKeyFormat(argv[2]))
    // {
    //     std::cout << "Syntaxe : encrypt *msg* *cle_privee*" << std::endl;
    //     std::cout << "\t'cle_privee' doit avoir comme format : 'd,n'" << std::endl;
    //     return -1;
    // }

    const int TAILLE_BLOC = 4;  //1 de plus que necessaire, ASCII -> [0-127]
    std::cout << dechiffrer(argv[1], TAILLE_BLOC, getKeyFromString(argv[2])) << std::endl;

    return 0;
}

//check if string is a valid number
bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

//check if argument 2 is valid
bool validKeyFormat(std::string key)
{
    //si aucune virgule ou plus d'une
    if(std::count(key.begin(), key.end(), ',') != 1)
        return false;
    
    //un nombre, une virgule, un autre nombre
    int indexOfComma = key.find(',');
    std::string leftPart = key.substr(0, indexOfComma);
    std::string rightPart = key.substr(indexOfComma + 1, key.size() - 1);
    if(!is_number(leftPart) || !is_number(rightPart))
        return false;

    return true;
}

//a^b mod n
int modExp(int a, int b, int n)
{
	//algo expo rapide/exponential squaring
	
	if (b == 0) 
        return 1; 
  
    if (b == 1) 
        return a % n; 
  
    long long t = modExp(a, b / 2, n); 
    t = (t * t) % n; 
  
    // if exponent is even value 
    if (b % 2 == 0) 
        return t; 
  
    // if exponent is odd value 
    else
        return ((a % n) * t) % n; 
}

int dechiffrer(int x, std::pair<int,llong> privKey)
{
    int d = privKey.first, n = privKey.second;
	return modExp(x,d,n);
}

std::string dechiffrer(std::string m, int tailleBloc, std::pair<int,llong> privKey)
{
    std::string decrypted = "";

    std::string blocTmp = "";
    std::string decBlockTmp;
    for(char c : m)
    {
        if(c == ' ')
        {
            decBlockTmp = std::to_string(dechiffrer(std::stoi(blocTmp), privKey));
            std::string dec_filled = std::string(tailleBloc - decBlockTmp.length(), '0') + decBlockTmp;
            decrypted.append(dec_filled);
            blocTmp = "";
        }
        else
        {
            blocTmp.append(std::to_string(c - 48)); //numbers in ascii starts at 48
        }
        
    }
    decBlockTmp = std::to_string(dechiffrer(std::stoi(blocTmp), privKey));
    std::string dec_filled = std::string(tailleBloc - decBlockTmp.length(), '0') + decBlockTmp;
    decrypted.append(dec_filled);

    std::cout<<decrypted<<std::endl;//debug

//2eme partie


    std::vector<std::string> blocs;
    blocTmp = "";
    int i = decrypted.size();
    int c = 0;
    int pos = 0;
    while(i >= 0)
    {
        --i;
        pos = c % (tailleBloc - 1); //parce qu'on veut des codes ascii => [0;127]
        // ca fait 0 1 2 3, si pos == 0, alors on fait (++) + une nouvelle chaine, sinon on append
        if(pos == 0 && !blocTmp.empty())
        {
            // ajout du bloc précédent
            blocs.push_back(blocTmp);
            blocTmp = "";
        }
        if(i > -1)
        {
            blocTmp = decrypted[i] + blocTmp;
            ++c;
        }
        std::cout<<blocTmp<<std::endl;//debug
    }
    if(!blocTmp.empty())
        blocs.push_back(blocTmp);   //ajout du reste

    //debug affichage du vector blocs
    // for(std::string s : blocs)
    // {
    //     std::cout<<s<<std::endl;
    // }

    std::string res = "";
    for(int i = blocs.size() - 1 ; i >= 0 ; --i)
    {
        std::cout<<blocs[i]<<std::endl;
        char c = std::stoi(blocs[i]);
        if(c != 0)
            res.push_back(c);
    }
    return res;
}

//on suppose que le string est conforme
std::pair<int,llong> getKeyFromString(std::string key)
{
    int indexOfComma = key.find(',');
    return std::make_pair(
        std::stoi(key.substr(0, indexOfComma)),
        std::stoll(key.substr(indexOfComma + 1, key.size() - 1))
    );
}