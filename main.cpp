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
		int len=0;
		for( const_iterator cell=begin(); cell!=end(); cell++)
			len = max(len,(int)cell->length());
		return len;
	}
};
class Page_t: public list< Row_t> {
public:
	int get_cellen() const {
		int len = 0;
		for( const_iterator row=begin(); row!=end(); row++)
			len = max(len,row->get_cellen());
		return len;
	}
	int get_maxcol() const {
		int col = 0;
		for( const_iterator row=begin(); row!=end(); row++)
			col = max(col,(int)row->size());
		return col;
	}
	ostream& begin_tabular(ostream& ostr) const {
		const int col = get_maxcol();
		if(!col)
			return ostr;
		ostr<<"\\begin{tabular}{|";
		for(int i=0; i<col; i++)
			ostr<<"r|";
		ostr<<"}";
		return ostr;
	}
	ostream& end_tabular(ostream& ostr) const {
		ostr<<"\\end{tabular}";
		return ostr;
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
	;
	for( Row_t::const_iterator cell=row.begin(); cell!=row.end(); cell++, i++)
		ostr<< (i>0?delim:"")<< setw(len) <<(*cell)<<' ';
	Row_t::cellen=0;
	Row_t::Delimiter='\0';
	return ostr;
}

ostream& operator<<(ostream& ostr, const Page_t& page){
	int len = page.get_cellen();
	int col = page.get_maxcol();
	page.begin_tabular(ostr) <<'\n';
	for( Page_t::const_iterator row=page.begin(); row!=page.end(); row++){
		Row_t::cellen = len, Row_t::Delimiter = '&', ostr<<(*row);
		for(int i= col- row->size(); i>0; i-- ) ostr<<'&'<<setw(len)<<""<<' ';
		ostr<<"\\\\\n";
	}
	Row_t::cellen=0;
	page.end_tabular(ostr)<<'\n';
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
		//cout << line <<endl;
		stringstream wstr(line);
		Row_t row;
		while( getline(wstr,word,',') )
			row.push_back(word);
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