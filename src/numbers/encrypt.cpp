#include <iostream>     //std::cout
#include <algorithm>    //std::find_if
#include <string>       //std::stoi std::to_string
#include <vector>       //std::vector

using llong = long long;

std::pair<int,llong> getKeyFromString(std::string key);
int chiffrer(int m, std::pair<int,llong> pubKey);
    int modExp(int a, int b, int n);

bool validKeyFormat(std::string key);
bool is_number(const std::string& s);

// g++ -o bin/numbers/encrypt src/numbers/encrypt.cpp && bin/numbers/encrypt 1000 "$(< pub.key)"
int main(int argc, char* argv[])
{
    if(argc != 3 || !is_number(argv[1]) || !validKeyFormat(argv[2]))
    {
        std::cout << "Syntaxe : encrypt *msg* *cle_publique*" << std::endl;
        std::cout << "\t'msg' doit être un entier positif" << std::endl;
        std::cout << "\t'cle_publique' doit avoir comme format : 'e,n'" << std::endl;
        return -1;
    }

    std::cout<< chiffrer(std::stoi(argv[1]), getKeyFromString(argv[2])) << std::endl;

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

//on suppose que le string est conforme
std::pair<int,llong> getKeyFromString(std::string key)
{
    int indexOfComma = key.find(',');
    return std::make_pair(
        std::stoi(key.substr(0, indexOfComma)),
        std::stoll(key.substr(indexOfComma + 1, key.size() - 1))
    );
}