#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <list>
using namespace std;
class Row_t: public list<string> {
public:
	static char Delimiter;
	static int cellen;
	int get_cellen() const {
		int len;
		for( const_iterator cell=begin(); cell!=end(); cell++)
			len = max(len,(int)cell->length());
		return len;
	}
};
class Page_t: public list< Row_t> {
public:
	int get_cellen() const {
		int len;
		for( const_iterator row=begin(); row!=end(); row++)
			len = max(len,row->get_cellen());
		return len;
	}
};
int Row_t::cellen = 0;
char Row_t::Delimiter=0;

ostream& operator<<(ostream& ostr, const Row_t& row){
	int len = Row_t::cellen;
	if(0==len)
		len = row.get_cellen();
	int i=0;
	string delim;
	delim = Row_t::Delimiter;
	for( Row_t::const_iterator cell=row.begin(); cell!=row.end(); cell++, i++)
		ostr<< delim<< setw(len) <<(*cell);
	Row_t::cellen=0;
	Row_t::Delimiter='\0';
	return ostr;
}

ostream& operator<<(ostream& ostr, const Page_t& page){
	int len = page.get_cellen();
	for( Page_t::const_iterator row=page.begin(); row!=page.end(); row++){
		Row_t::cellen = len, Row_t::Delimiter = ',', ostr<<(*row)<<" ";
	}
	Row_t::cellen=0;
	return ostr;
}
int main( int argc, char * argv[] ){
	
	char * ifName, * ofName;
	ifName = argc >= 2? argv[1]: NULL;
	ofName = argc >= 3? argv[2]: NULL;
	if( ! ifName ){
		cout<<"No INPUT specified\n";
		return 1;
	}
	
	ifstream istr(ifName);
	if( ! istr.is_open() ){
		cout<<"Cannot open \'"<<ifName<<"\'\n";
		return 1;
	}

	Page_t Page;
	string line;
	while( getline(istr,line) ){
		string word;
		stringstream wstr(line);
		Row_t row;
		while( getline(wstr,word,',') ){
			row.push_back(word);
		}
		if( row.empty() )
			continue;
		Page.push_back(row);\
		//Page.back().splice(row.begin(), row);
	}
	istr.close();
	if(!ofName){
		cout<<Page;
		return 0;
	}

	ofstream ostr(ofName);
	ostr<<Page;
	ostr.close();
	return 0;
}