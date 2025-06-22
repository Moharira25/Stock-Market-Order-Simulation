#include "exchange.h"
#include "order.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
using namespace std;

int main(int argc, char** argv){
    //check if there's two arguments
    if (argc != 2){
        cout << "Wrong command, use: main <filename>.txt";
        return 1;
    }
    //get the name of the input file.
    string fileName = argv[1];
    string line;
    ifstream in(fileName);

    if (in.is_open()){
        //read the first line to get the execution price
        double executionPrice;
        in >> executionPrice;
        in.ignore(INT_MAX, '\n');
        //generate the output file's name from the input file's name.
        string outputFile = fileName.replace(0, 2, "out");

        Exchange exchange(executionPrice, outputFile);
        int arrivalTime = 0;
        while ( getline (in,line) )
        {   
            stringstream ss(line);
            string id; char type; int quantity;
            double limitPrice = -1; //-1 for market orders

            ss >> id >> type >> quantity;
            if (!(ss >> limitPrice)) limitPrice = -1;

            Order order(id, type, limitPrice, arrivalTime, quantity);
            arrivalTime += 1;
            exchange.processOrder(order);

            cout << "Press 'Enter' to continue..." << endl;
            cin.get();

        }
        //handle un matched orders and close the file
        exchange.saveExchangeOutputs();
        in.close();
    }else{
        cout << "Unable to open file";
        return 1;
    } 

    return 0;
}
