#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <unordered_map>

using namespace std;


class OrderBookProgram{
	public:
		// Constructor
		OrderBookProgram(const string filepath, const unsigned long targetSize){
			this->filepath = filepath;
			this->targetSize = targetSize;
			this->curIncome = 0.0;
			this->curExpence = 0.0;
			this->curBid = 0;
			this->curAsk = 0;
		}

		// Tokenize a string according to specific deliminator
		vector<string> split(const string &s, char delim) {
  			vector<string> elems;
  			stringstream ss(s);
  			string item;
  			while (getline(ss, item, delim)) {
    			elems.push_back(item);
  			}
  			return elems;
		}

		// Add Order to Book
		void addOrder(const string orderID, const bool ask, const double price, const unsigned long count){
			cout<<"orderID: "<<orderID<<" ask: "<<ask<<" price: "<<price<<" count: "<<count<<endl; 
		}

		// Reduce Order
		void reduceOrder(const string orderID, const unsigned long count){
			cout<<"orderID: "<<orderID<<" count: "<<count<<endl;
		}

		// Check whether current # of stock is meet or exceed target size
		bool checkTarget(bool ask){
			if(ask){
				return (curAsk>=targetSize);
			}
			else{
				return (curBid>=targetSize);
			}
		}

		// Output result
		void printResult(const string times, const bool ask, double total){
			string askStr = (ask)?"S":"B";
			string totalStr = (total==0)?"NA":to_string(total);
			cout<<times<<" "<<askStr<<" "<<totalStr<<endl;
		}

		// Analysis data
		void analysis(){
			string line;
			cout<<"open "<<filepath<<endl;
			targetFile.open(filepath,ios::binary|ios::out);
			if(targetFile.is_open()){
				while(getline(targetFile,line)){
					cout<<line<<endl;
					// tokens[0]: times
					// tokens[1]: action - A, R
					// tokens[2]: orderID
					// tokens[3]: type - B, S / count (when reduce order)
					// tokens[4]: price 
					// tokens[5]: count
					vector<string> tokens = split(line, ' ');
					if(tokens[1]=="A"){
						cout<<"Add Book Order!"<<endl;
						addOrder(tokens[2],(tokens[3]=="S"), stod(tokens[4]), stoul(tokens[5]));
					}
					else{
						cout<<"Reduce Order!"<<endl;
						reduceOrder(tokens[2],  stoul(tokens[3]));
					}
				}
				targetFile.close();
			}
		}

		

	private:
		// # of stock
		unsigned long targetSize;

		// path of file
		string filepath;

		// Target file
		ifstream targetFile;

		// Record current income/expence
		double curIncome;
		double curExpence; 

		// Record current # stock
		unsigned long curBid;
		unsigned long curAsk;

		// entry: <order_id, <price, count>>
		// e.g. A b S 44.26 100 
		//	=>  insert an entry <"b", <44.26, 100> > into askOrderRec
		unordered_map<string, pair<double, unsigned long> > bidOrderRec;
		unordered_map<string, pair<double, unsigned long> > askOrderRec;
		
		// entry: <price, count>>
		// order: 	bidPriceRec high->low 
		// 			askPriceRec low->high
		// e.g. A b S 44.26 100 
		//	=>  insert an entry <44.26, 100> into askPriceRec
		map<double, unsigned long> bidPriceRec;
		map<double, unsigned long> askPriceRec;
};


int main(){
	unsigned long targetSize = 0;
	cout<<"Please input target size: ";
	cin>>targetSize;
	OrderBookProgram* obp = new OrderBookProgram("./pricer.in", targetSize);
	obp->analysis();
	return 0;
}
