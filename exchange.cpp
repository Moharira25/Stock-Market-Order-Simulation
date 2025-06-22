#include "exchange.h"
#include <iomanip> 
#include<iostream>
#include <fstream>
#include <algorithm>
using namespace std;


Exchange::Exchange(){
    //initializes a default exchange with random defalut parameters.
    executionPrice_ = 1.0;
    outFile_ = "default_output_file.txt";
}

Exchange::Exchange(double executionPrice, string outputFile) : executionPrice_(executionPrice), outFile_(outputFile){

}

Exchange::~Exchange(){
    //No dynamic memeory so an empty destructor will suffice.
}


void Exchange::processOrder(Order& order){
    //print new pending orders
    printPendingOrders(order);
    //try and match the order
    matchOrder(order);

}

void Exchange::matchOrder(Order& order){
    Order currentOrder = order;
    while (true){
        int amountTraded; //shares to be traded
        bool matched = false; 
        string buyOrderId, sellOrderId; 

        set<Order>& orders = (currentOrder.type_ == 'B') ? sellOrders_ : buyOrders_;
        for (auto it = orders.begin(); it != orders.end(); it++){
            if (it->limitPrice_ == -1 || currentOrder.limitPrice_ == -1){
                //at least one is a market order
                matched = true;
                double price = max(it->limitPrice_, currentOrder.limitPrice_);
                //if one of them is a limit order, its price is the execution price
                executionPrice_ = price != -1 ? price : executionPrice_;
            }else if (((it->type_ == 'B') && (it ->limitPrice_ >= currentOrder.limitPrice_)) || ((currentOrder.type_ == 'B') && (currentOrder.limitPrice_ >= it->limitPrice_))){
                //both are limit orders with the buy order having higher than or equal to the limit price than the sell order
                //in this case execution price is for order that arrived earlier.
                executionPrice_ = it->arrivalTime_ < currentOrder.arrivalTime_ ? it->limitPrice_ : currentOrder.limitPrice_;
                matched = true;
            }
            if (matched){
                sellOrderId = currentOrder.type_ == 'B' ? it->id_ : currentOrder.id_;
                buyOrderId  = currentOrder.type_ == 'B' ? currentOrder.id_ : it->id_;
                //checks if the order (*it) will be executed partially
                if (it->quantity_ > currentOrder.quantity_){
                    amountTraded = currentOrder.quantity_;
                    //create a shallow copy of the order (*it)
                    Order residualOrder = *it;
                    residualOrder.quantity_ -= amountTraded;
                    currentOrder = residualOrder;
                }
                //checks if the current order will be executed partially 
                else if (it->quantity_ < currentOrder.quantity_){
                    amountTraded = it->quantity_;
                    currentOrder.quantity_ -= amountTraded;
                } else{ 
                    //quantities are equal which mean full shares are traded
                    
                    amountTraded = currentOrder.quantity_;

                    //set the quantity to -1 as it would be used to
                    //break out of the while loop
                    currentOrder.quantity_ = -1;
                }
                //the order (*it) will be removed because it's either executed fully
                //or it's the current order to be matched.
                orders.erase(*it);
                
                executeTrade(buyOrderId, sellOrderId, amountTraded, executionPrice_);
                printPendingOrders(currentOrder);
                break;
            }
        }
        if (matched){
            //if the shares left is less than 0 the order is fully executed. 
            if (currentOrder.quantity_ < 0) break;
            
        } else{
            //no match, add it to the correct orders' set
            //don't use the set orders as the current order might be of
            //different type in this step.
            if (currentOrder.type_ == 'B') buyOrders_.insert(currentOrder);
            else sellOrders_.insert(currentOrder);
            break;
        }
    }
}

void Exchange::printPendingOrders(Order& order){
    //check if the order is not executed
    if (order.quantity_ > 0){
        //insert the order to the appropriate set before prinitng
        if (order.type_ == 'B') buyOrders_.insert(order);
        else sellOrders_.insert(order);
    }
    
    cout << fixed << setprecision(2);
    cout << "latest Price: "<< executionPrice_ << endl;
    
    cout << setw(25) << left << "buy" << setw(25) << left << "| sell" << endl;
    cout << setw(25) << setfill('-') << "" << setw(25) << setfill('-') << "" << endl;
    cout << setfill(' ');

    auto it1 = buyOrders_.begin(); 
    auto it2 = sellOrders_.begin();

    
    while (it1 != buyOrders_.end() || it2 != sellOrders_.end()){
        // Buy orders' side of the table
        if (it1 != buyOrders_.end()){
            //format the execution price to display two decimal points
            string price = to_string(it1->limitPrice_); 
            if (it1->limitPrice_ != -1) price = price.substr(0, price.find('.') + 3);
            else price = "M";

            string data = it1->id_ + "  " + price + "  " + to_string(it1->quantity_) + " ";
            cout << setw(25) << left << data;
            it1++;
        } else{
            // no buy order
            cout << setw(25) << left << " ";
        }
        //sell orders' side of the table
        if (it2 != sellOrders_.end()){
            string price = to_string(it2->limitPrice_); 
            if (it2->limitPrice_ != -1) price = price.substr(0, price.find('.') + 3);
            else price = "M";
            string data = "| " + it2->id_ + "  " + price + "  " + to_string(it2->quantity_) + " ";
            cout << setw(25) << left << data;
            it2++;
        } else{
            //no sell order
            cout << setw(25) << left << "| ";
        }
        cout << '\n';
    }

    //This if statement is here to prevent unnecessary calls to set.erase()
    if (order.quantity_ > 0){
        //after printing delete the order from the set
        if (order.type_ == 'B') buyOrders_.erase(order);
        else sellOrders_.erase(order);
    }
}


void Exchange::executeTrade(string buyOrderId, string sellOrderId, int quantity, double executionPrice){
    //edit the string executionPrice to display two decimal points
    string price = to_string(executionPrice);
    price = price.substr(0, price.find('.') + 3);

    string bTrade = "order " + buyOrderId+ " " + to_string(quantity) + " shares purchased at price " + price;
    string sTrade = "order " + sellOrderId + " " + to_string(quantity) + " shares sold at price " + price;
    
    exchangeLog_.push_back(bTrade);
    exchangeLog_.push_back(sTrade);
    
    cout << bTrade << endl;
    cout<< sTrade << endl;
    
    
}

void Exchange::saveExchangeOutputs(){
    //create/override output file
    fstream out(outFile_, ios::out);

    if (out.is_open()){
        //write logs of executed orders first
        int logIndex = 1;
        bool ordersRemaining = !sellOrders_.empty() || !buyOrders_.empty();
        for (const string log: exchangeLog_) {
            //add line if it is not the last order or there are 
            //unmatched orders that will come after it
            if (ordersRemaining || (logIndex != exchangeLog_.size())) out << log + '\n';
            else out << log;
            logIndex += 1;
        }

        //a vector to store unmatched sell/buy orders
        vector<Order> unmatchedOrders;
        for (const Order& o: sellOrders_){
            unmatchedOrders.push_back(o);
        }
        for (const Order& o: buyOrders_){
            unmatchedOrders.push_back(o);
        }
        //sort based on arrival time
        sort(unmatchedOrders.begin(), unmatchedOrders.end(), [] (Order& o1, Order& o2){
            return o1.arrivalTime_ < o2.arrivalTime_;
        });
        int unmatchedIndex = 1;
        //write logs of un matched orders.
        for (const Order& o: unmatchedOrders){
            string data = "order " + o.id_ + " " + to_string(o.quantity_) + " shares unexecuted";
            //check if it is the last order
            if (unmatchedIndex == unmatchedOrders.size()) out << data;
            else out << data + '\n';
            unmatchedIndex += 1;
        }
        //close the output file
        out.close();
    }else cout << "Unable to open output file";

}