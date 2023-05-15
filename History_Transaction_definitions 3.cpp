#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction( std::string ticker_symbol, unsigned int day_date, unsigned int month_date,
unsigned year_date, bool buy_sell_trans, unsigned int number_shares, double trans_amount
){
  day=day_date;
  month=month_date;
  year=year_date;
  symbol=ticker_symbol;
  trans_type=(buy_sell_trans == true ? "Buy": "Sell");
  shares=number_shares;
  amount=trans_amount;
  trans_id=assigned_trans_id;
  assigned_trans_id++;
  
  acb=0.0;
  acb_per_share=0.0;
  share_balance=0;
  cgl=0.0;
}

// Destructor
// TASK 1
//
Transaction::~Transaction(){

}

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<( Transaction const &other ){
  if(this->get_year()>other.get_year()){
    return false;
  }
  else if(this->get_year()==other.get_year()
  &&this->get_month()>other.get_month()){
    return false;
  }
  else if(this->get_month()==other.get_month()
  &&this->get_day()>other.get_day()){
    return false;
  }
  else if(this->get_day()==other.get_day()
  &&this->get_month()==other.get_month()
  &&this->get_year()==other.get_year()
  &&this->get_trans_id()<=other.get_trans_id()){
    return false;
  }
  else{
    return true;
  }
}


// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//

  // Constructor
	//
	History::History(){
    p_head=nullptr;
  }

	// Destructor.
	//
	History::~History(){
    while(p_head!=nullptr){
      Transaction *temp=p_head;
      p_head=p_head->get_next();
      delete temp;
      temp=nullptr;
    }
  }

	// Read the transaction history from file.
	//
	void History::read_history(){
    ece150::open_file();
    Transaction *p_new_trans;
    while(ece150::next_trans_entry()){
      p_new_trans= new Transaction( ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(),
      ece150::get_trans_year(),ece150::get_trans_type(), ece150::get_trans_shares(),ece150::get_trans_amount());
      insert(p_new_trans);
    }
    ece150::close_file();
  }
	// Insert transaction in linked list.
	//
	void History::insert(Transaction *p_new_trans){
    if(p_head==nullptr){
      p_head=p_new_trans;
     }
     else{
      Transaction *temp=p_head;
      while(temp->get_next()!=nullptr){
          temp=temp->get_next();
      }
      temp->set_next(p_new_trans);
  }
  }
	// Sort using insertion sort.
	//
	void History::sort_by_date(){
   Transaction *p_new_head=p_head;
    p_head = (p_head->get_next());
    p_new_head ->set_next(nullptr);
    if(p_head==nullptr){
      return;
    }
    while(p_head!=nullptr){
      Transaction*p_temp=p_head;
      Transaction*p_current=p_new_head;
      p_head=(p_head->get_next());
      if(*p_temp < *p_new_head){
        p_temp -> set_next(p_new_head);
        p_new_head=p_temp;
      }
      else{
        while((p_current->get_next()!=nullptr)&&(*(p_current->get_next())<*p_temp)){
          p_current=(p_current->get_next());
        }
        p_temp->set_next((p_current->get_next()));
        p_current->set_next(p_temp);
      }
    }
    p_head=p_new_head;
  }

	// Update the acb members in the linked list.
	//
	void History::update_acb_cgl(){
    Transaction*p_temp=p_head;
    double sum_amount=0;
    double sum_shares=0;
    double acb_per_share=0;
    double cgl=0;
    if(p_temp==nullptr){
      return;
    }
    while(p_temp!=nullptr){
      if(p_temp->get_trans_type()){
        sum_amount+=(p_temp->get_amount());
        p_temp->set_acb(sum_amount);
        sum_shares+=(p_temp->get_shares());
        p_temp->set_share_balance(sum_shares);
        acb_per_share=sum_amount/sum_shares;
        p_temp->set_acb_per_share(acb_per_share);
        }
      else{
        sum_amount=sum_amount-((p_temp->get_shares())*acb_per_share);
        p_temp->set_acb(sum_amount);
        sum_shares-=(p_temp->get_shares());
        p_temp->set_share_balance(sum_shares);
        acb_per_share=sum_amount/sum_shares;
        p_temp->set_acb_per_share(acb_per_share);
        cgl=(p_temp->get_amount())-(p_temp->get_shares())*acb_per_share;
        p_temp->set_cgl(cgl);
      }
      p_temp=p_temp->get_next();
    }
  }
	// Compute the ACB, and CGL.
	//
	double History::compute_cgl(unsigned int year){ 
    double sum_cgl=0;
    Transaction*p_temp=p_head;
      while(p_temp!=nullptr){
        if((p_temp->get_year())==year){
          sum_cgl+=(p_temp->get_cgl());
        }
        p_temp=p_temp->get_next();
      }
      return sum_cgl;
  }

	// Print the transaction history.
	//
	void History::print(){
    std::cout<<"========== BEGIN TRANSACTION HISTORY ============"<<std::endl; 
    Transaction *temp2 = p_head;
      while(temp2 !=nullptr){
      temp2->Transaction::print();
      temp2=temp2->get_next();}
    std::cout<<"========== END TRANSACTION HISTORY ============"<<std::endl; 
  }



// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
