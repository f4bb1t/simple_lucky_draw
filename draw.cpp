#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<cstdlib>
#include<utility>
#include<vector>
#include<random>
#include<fstream>
#include<string>
#include<map>

//the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET   "\033[0m"
#define RED     "\033[31m"   
#define BLUE    "\033[34m"    
#define BOLDYELLOW  "\033[1m\033[33m"     
#define BOLDCYAN    "\033[1m\033[36m"  

//Storage
#define POINT_STORAGE_FILE "wallet.txt"
#define DELIMITER ":"

typedef double probability;
typedef std::string name; 

using namespace std;

map<name, int> mapPointBox = {{"bronze", 0}, {"silver", 0}, {"golden", 0}, {"diamond", 0}};


struct Prize {
    string name;   
    double value;  // the price of the prize
    int point_level;  
    int redeem_points;  // how many points can redeem this prize
};


int getRandomNumberInRange(int min, int max){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

bool inRange(int low, int high, int x) {
    return (low <= x && x < high);
}

vector< pair<name, probability> > getRanges(vector< pair<name, probability> > pointDrawProbability) {
    vector< pair<name, probability> > ranges;
    double accumulate = 0;
    for (auto i = pointDrawProbability.begin(); i != pointDrawProbability.end(); ++i) {
        accumulate += i->second;
        pair<name, probability> newPair = std::make_pair(i->first, accumulate);
        ranges.push_back(std::move(newPair));
    }
    return ranges;
}

string drawOnce(vector< pair<name, probability> > pointBox) {
    double random = double(getRandomNumberInRange(0, 99)) / 100;
    for (auto i = pointBox.begin(); i != pointBox.end(); ++i) {
        if (random < i->second) {
            return i->first;
        }
    }
    return "Error";
}

void printPrize(name prize) {
    if (prize == "bronze") {
        std::cout << RED << "bronze " << RESET << "fabbit point" << std::endl;
    } else if (prize == "silver") {
        std::cout << BLUE << "silver " << RESET << "fabbit point" << std::endl;
    } else if (prize == "golden") {
        std::cout << BOLDYELLOW << "Yay! golden fabbit point" << RESET << std::endl;
    } else if (prize == "diamond") {
        std::cout << BOLDCYAN << "🐰: Wow!!! You got a " << prize << " fabbit point! 😍" << RESET << std::endl;
    }
}

auto draw_n_times(int times_to_draw, vector< pair<name, probability> > pointDrawRange) {
    for (int i = 0; i < times_to_draw; ++i) {
        name prize = drawOnce(pointDrawRange);
        printPrize(prize);
        mapPointBox[prize] += 1; 
    }
}

void readFromFile() {
    string line;
    ifstream myfile (POINT_STORAGE_FILE);
    if (!myfile.is_open()) {
        cout << "Unable to open file"; 
        return ;
    }
    std::cout << "Your current points in the 🐰's wallet:" << std::endl;
    std::cout << "======================" << std::endl;
    while (myfile) {
        string s;
        if (!getline(myfile, s)) break;
        name prize = s.substr(0, s.find(DELIMITER));
        int number = stoi(s.substr(s.find(DELIMITER) + 2, s.length() - 1));
        std::cout << prize << " points: " << number << std::endl;
        mapPointBox[prize] = number;
    }
    std::cout << "======================" << std::endl;
    myfile.close();
}

void writeToFile() {
    ofstream myfile;
    myfile.open(POINT_STORAGE_FILE);
    for (auto i = mapPointBox.begin(); i != mapPointBox.end(); ++i) {
        myfile << i->first << ": " << i->second << "\n";
    }
    myfile.close();
}


int main(){
    // Initialization
    // TO-DO: use a file to initialize pointBox
    std::cout << BOLDYELLOW << "\nWelcome to fabbit's lucky draw program.\n" << RESET << std::endl;

    vector< pair<name, probability> > pointDrawProbability;
    pointDrawProbability.push_back(std::make_pair("bronze", 0.6));
    pointDrawProbability.push_back(std::make_pair("silver", 0.3));
    pointDrawProbability.push_back(std::make_pair("golden", 0.09));
    pointDrawProbability.push_back(std::make_pair("diamond", 0.01));

    readFromFile();

    // get input
	int times_to_draw;
	std::cout << "Please input your remaining lucky draw quota today:" << std::endl;
	std::cin >> times_to_draw;
    std::cout << "Start the game..." << std::endl;

    // start game
    auto pointDrawRange = getRanges(pointDrawProbability);
    draw_n_times(times_to_draw, pointDrawRange);
    writeToFile();

    std::cout << "======================" << std::endl;
    std::cout << "Congratulations! Data Saved to wallet." << std::endl;

    return 0;
}
