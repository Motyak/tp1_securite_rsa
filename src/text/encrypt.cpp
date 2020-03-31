#include <iostream>     //std::cout
#include <algorithm>    //std::find_if
#include <string>       //std::stoi std::to_string
#include <vector>       //std::vector

using llong = long long;

std::pair<int,llong> getKeyFromString(std::string key);
std::string chiffrer(std::string m, int tailleBloc, std::pair<int,llong> pubKey);
    int chiffrer(int m, std::pair<int,llong> pubKey);
        int modExp(int a, int b, int n);

bool validKeyFormat(std::string key);
bool is_number(const std::string& s);

// g++ -o bin/text/encrypt src/text/encrypt.cpp && bin/text/encrypt test "$(< pub.key)"
int main(int argc, char* argv[])
{
    if(argc != 3 || !validKeyFormat(argv[2]))
    {
        std::cout << "Syntaxe : encrypt *msg* *cle_publique*" << std::endl;
        std::cout << "\t'cle_publique' doit avoir comme format : 'e,n'" << std::endl;
        return -1;
    }

    const int TAILLE_BLOC = 4;  //1 de plus que necessaire, ASCII -> [0-127]
    std::cout << chiffrer(argv[1], TAILLE_BLOC, getKeyFromString(argv[2])) << std::endl;
    // std::cout<< chiffrer(std::stoi(argv[1], getKeyFromString(argv[2]))) << std::endl;

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

//le message clair est un entier
int chiffrer(int m, std::pair<int,llong> pubKey)
{
    int e = pubKey.first, n = pubKey.second;
    if(m >= n)
    {
        std::cout << "Le message à chiffrer doit être plus petit que n=" << n << std::endl;
        std::cout << "(Message actuel = " << m << std::endl;
        return -1;
    }
	return modExp(m, e, n);
}

//le message clair est un string
std::string chiffrer(std::string m, int tailleBloc, std::pair<int,llong> pubKey)
{
    // std::vector<int> blocs;
    std::vector<std::string> blocs;
    std::string strMsg = "";
    for(char c : m)
    {
        std::string asciiCode = std::to_string(c);
        int tailleAscii = 3;
        std::string filled = std::string(tailleAscii - asciiCode.size(), '0') + asciiCode;
        strMsg.append(filled);
    }

    std::string blocTmp = "";
    int i = strMsg.size();
    int c = 0;
    int pos = 0;
    while(i >= 0)
    {
        --i;
        pos = c % tailleBloc;
        // ca fait 0 1 2 3, si pos == 0, alors on fait (++) + une nouvelle chaine, sinon on append
        if(pos == 0 && !blocTmp.empty())
        {
            // ajout du bloc précédent
            blocs.push_back(blocTmp);
            blocTmp = "";
        }
        if(i > -1)
        {
            blocTmp = strMsg[i] + blocTmp;
            ++c;
        }
    }

    // ajout du reste
    if(pos != 0)
        blocs.push_back(blocTmp);

    // on chiffre les blocs et on les append dans une chaine resultat, en les separant par des espaces
    std::string res = "";
    for(int i = blocs.size() - 1 ; i > 0 ; --i)
    {
        int bloc = std::stoi(blocs[i]);
        res.append(std::to_string(chiffrer(bloc, pubKey)) + " ");
    }
    int bloc = std::stoi(blocs[0]);
    res.append(std::to_string(chiffrer(bloc, pubKey)));
    
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