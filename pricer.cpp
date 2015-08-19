#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <unordered_map>
#include <iomanip>

using namespace std;

// Comparator for bidPriceRec (from high to low)
struct cmpBid {
	bool operator()(const double &a, const double &b) const {
		return a > b;
	}	
};

class Pricer{
	public:
		// Constructor
		Pricer(const string filepath, const unsigned long targetSize){
			this->filepath = filepath;
			this->targetSize = targetSize;
			this->curTime = "";
			this->curBid = 0;
			this->curAsk = 0;
			this->curIncome = 0.0;
			this->curExpence = 0.0;
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

		// Calculate income/expense
		double calResult(bool ask){
			double result = 0;
			unsigned long tmpCur = targetSize;
			map<double, unsigned long>::iterator it=ask?askPriceRec.begin():bidPriceRec.begin();
			for(;tmpCur>0.0;it++){
				if(tmpCur>=it->second){
					result+=(it->first)*(it->second);
					tmpCur-=(it->second);
				}
				else{
					result+=(it->first)*tmpCur;
					tmpCur-=tmpCur;
				}
			}
			return result;
		}

		// Add Order to Book
		void addOrder(const string orderID, const bool ask, const double price, const unsigned long count){
			orderRec[orderID] = make_pair(ask, make_pair(price, count));
			double cur = ask?curExpence:curIncome;
			
			// Update price record
			if(ask){
				askPriceRec[price] += count;
				curAsk+=count;
			}
			else{
				bidPriceRec[price] += count;
				curBid+=count;
			}

			// Check the # of shares
			if(checkTarget(ask)){
				double total = calResult(ask);
				if(cur!=total){
					printResult(curTime, ask, total);
					if(ask)
						curExpence = total;
					else
						curIncome = total;
				}
			}
		}

		// Reduce Order
		void reduceOrder(const string orderID, const unsigned long count){
			//cout<<"orderID: "<<orderID<<" count: "<<count<<endl;
			double rePrice = (orderRec[orderID].second).first;
			bool ask = orderRec[orderID].first;
			bool na;
			double cur = ask?curExpence:curIncome;
			
			// Update price record
			if(ask){
				na = (curAsk<targetSize);
				askPriceRec[rePrice]-=count;
				curAsk-=count;
				if(askPriceRec[rePrice]==0)
					askPriceRec.erase(rePrice);
			}
			else{
				na = (curBid<targetSize);
				bidPriceRec[rePrice]-=count;
				curBid-=count;
				if(bidPriceRec[rePrice]==0)
					bidPriceRec.erase(rePrice);
			}

			// Update order record
			orderRec[orderID].second.second-=count;
			if(orderRec[orderID].second.second==0){
				orderRec.erase(orderID);
			}

			// Check the # of shares
			if(checkTarget(ask)){
				double total = calResult(ask);
				if(cur!=total){
					printResult(curTime, ask, total);
					if(ask)
						curExpence = total;
					else
						curIncome = total;
				}
			}
			else{
				if(!na){
					// Update income/expence
					if(ask)	
						curExpence=0.0;
					else	
						curIncome=0.0;
					printResult(curTime, ask, 0);
				}
			}
		}

		// Check whether current # of share is meet or exceed target size
		bool checkTarget(bool ask){
			return ask?(curAsk>=targetSize):(curBid>=targetSize);
		}

		// Output result
		void printResult(const string times, const bool ask, double total){
			string actionStr = (ask)?"B":"S";
			if(total==0){
				cout<<times<<" "<<actionStr<<" "<<"NA"<<endl;
			}
			else{
				cout<<fixed;
				cout<<times<<" "<<actionStr<<" "<<setprecision(2)<<total<<endl;
			}
		}

		// Split inout string into a set of tokens and then do further analysis.
		// tokens[0]: times
		// tokens[1]: action - A, R
		// tokens[2]: orderID
		// tokens[3]: type - B, S / count (when reduce order)
		// tokens[4]: price 
		// tokens[5]: count
		void analysis(){
			string line;
			targetFile.open(filepath,ios::binary|ios::out);
			if(targetFile.is_open()){
				while(getline(targetFile,line)){
					vector<string> tokens = split(line, ' ');
					curTime = tokens[0];
					if(tokens[1]=="A")
						addOrder(tokens[2],(tokens[3]=="S"), stod(tokens[4]), stoul(tokens[5]));
				
					else
						reduceOrder(tokens[2],  stoul(tokens[3]));
				}
				targetFile.close();
			}
		}

		

	private:
		// # of share
		unsigned long targetSize;

		// path of file
		string filepath;

		// Target file
		ifstream targetFile;

		// Current time
		string curTime;

		// Record current # share
		unsigned long curBid;
		unsigned long curAsk;

		// Record currnt income/expence
		double curIncome;
		double curExpence;

		// Table to record the status of different order_id
		// entry: <order_id, <ask, <price, count> > >
		// e.g. A b S 44.26 100 
		//	=>  insert an entry <"b", <true, <44.26, 100> > > into askOrderRec
		unordered_map< string, pair<bool, pair<double, unsigned long> > > orderRec;

		// Table to record the status of price and its count.
		// entry: <price, count>>
		// order: 	bidPriceRec high->low 
		// 			askPriceRec low->high
		// e.g. A b S 44.26 100 
		//	=>  insert an entry <44.26, 100> into askPriceRec
		map<double, unsigned long ,cmpBid> bidPriceRec;
		map<double, unsigned long> askPriceRec;
};


int main(int argc, char *argv[]){
	if(argc!=2){
		cout<<"Please input target size."<<endl<<"e.g. ./a.out 200"<<endl;
		exit(1);
	}
	unsigned long targetSize = strtoul(argv[1], NULL, 0);
	Pricer* pricer = new Pricer("./pricer.in", targetSize);
	pricer->analysis();
	return 0;
}
