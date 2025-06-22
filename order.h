#ifndef ORDER_H_
#define ORDER_H_
#include <string>
using std::string;

class Order {
    private:
        //id of order
        std::string id_;
        //type of order i.e. sell/buy
        char type_;
        //limit price if the order is sell/buy
        //for market orders it'll be set to -1
        double limitPrice_;
        //arrival time of order
        int arrivalTime_;
        //amount of shares in order
        int quantity_;
    public:

        //Parameterised constructor.
        //sets all the private fields
        Order(string id, char type, double limitPrice, int arrivalTime, int quantity);

        //Destructor
        ~Order();

        //overloading the less than (<) operator:
        //
        //This will be used later to sort containers of sell and buy orders based on priority.
        //it ensures that the contaners will be sorted in a descending order with orders with
        //high->low priority, which will be used to match orders later.
        //
        //Note that calling this method with orders of different types might produce wrong results.
        friend bool operator<(const Order& o1, const Order& o2);

        //friend class exchange, to have access to private fields since the exchange will be
        //altering orders as they get executed.
        friend class Exchange;

};

#endif




