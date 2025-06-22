#ifndef EXCHANGE_H_
#define EXCHANGE_H_

#include <set>
#include "order.h"
#include <vector>
using std::string;
class Exchange{
    private:
        //the latest price of the exchange 
        double executionPrice_;
        //the name of the file to write the output to.
        std::string outFile_;
        //a vector to store the execution results while the exchange is still running.
        //a vector was choosen as it maintains the order of insertion, which is important for 
        //wrtitng the logs to the output file in the correct order.
        std::vector<string> exchangeLog_;

        //a set container was choosen since the excahnge will be inserting and removing orders
        //frequently and given the efficency of these containers in these operations they were choosen.
        
        //set for buy orders
        std::set<Order> buyOrders_;
        //set for sell orders
        std::set<Order> sellOrders_;


        
    public:
        //default constructor
        Exchange();

        //Parameterised constructor.
        //That Sets the execution price and the name of the 
        //output file for the exchange.
        Exchange(double executionPrice, string outputFile);
        //Destructor
        ~Exchange();

        //Process order
        //A method that takes an order, print the pending orders details
        //and then call the matchOrder function.
        void processOrder(Order& order);

        //A method to print details about pending orders.
        //The method takes the latest order as an argument and will be called in two cases. 
        //1. An order was just added.
        //2. After a match has been made.
        //
        //It determines what state the order is in by checking its remianing 
        //quantity, if it is less than zero, the order will not be included in the print.
        //Otherwise, it will be added to the appropriate set, and after the print it will
        //be removed since this mean we are handling a residual order and further matches will be attempted.
        void printPendingOrders(Order& order);

        //A method that takes the latest order and tries to match it with an old order
        //of the opposite type if certain rules applies.
        //The method has a while loop, and a for loop. It loops through an alias (orders) 
        //of one the (buy/sell) sets and if there's a match it does the following:
        //1. update the execution price of the exchange.
        //2. detrmine the correct quantity.
        //3. update the current order to be the residual order from the match (if it resulted from the old order),
        //otherwise it only updates the quantity of the current order (latest order).
        //4. erase the old order from the set since it's either executed fully, or it executed partially
        //and thus is the current order (step 3).
        //5. save the match details by calling the 'executeTrade' method.
        //6. Print the pending orders in the exchange.
        //7. break
        //At the end of each iteration of the while loop, the method breaks out of the while loop if:
        //1. The current order was executed fully.
        //2. The order was not matched, in this case it adds it to the appropriate set before breaking.
        void matchOrder(Order& order);

        //A method that does two things
        //Print the match deatils to the screen
        //Store the result of the trade or "match" to the vector exchangeLog_.
        void executeTrade(string buyOrderId, string sellOrderId, int quantity, double executionPrice);

        //A method that writes the executed and un executed orders to the output file.
        void saveExchangeOutputs();

};

#endif
