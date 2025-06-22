#include "order.h"

Order::Order(string id, char type, double limitPrice, int arrivalTime, int quantity) :
    id_(id), type_(type), limitPrice_(limitPrice), arrivalTime_(arrivalTime), quantity_(quantity){

}

Order::~Order(){
    //No dynamic memeory so an empty destructor will suffice.
}


bool operator<(const Order& o1, const Order& o2) {
	//if both of them are market orders. then check based on arrival time
    if (o1.limitPrice_ < 0 && o2.limitPrice_ < 0) return o1.arrivalTime_ < o2.arrivalTime_;
    //o1 is a market order ,keep it at the front (higher priority).
    else if (o1.limitPrice_ < 0) return true;
    //o2 is a market order, return false to move it to the front (higher priority).
    else if (o2.limitPrice_ < 0) return false;

    //if both are limit orders, the less than operator '<' will behave differently for
    //buy and sell orders.
    //
    //Buy orders with higher limitPrice are considered smaller and thus appear earlier
    //in sorting (high priority).
    //Sell orders with lower limitPrice are considered smaller and thus appear earlier
    //when sorting (high priority).
    else{
        if (o1.limitPrice_ > o2.limitPrice_){
            if (o1.type_ == 'B') return true;
            else return false;
        }
        else if (o1.limitPrice_ < o2.limitPrice_){
            if (o1.type_ == 'B') return false;
            else return true;
        }

        //if orders have the same limitPrice, return based on arrival time
        return o1.arrivalTime_ < o2.arrivalTime_;
    }
}
