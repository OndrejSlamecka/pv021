/*
 *  To test if the parser is reading the data file correctly
 */


#include<bits/stdc++.h>
#include "parser.cpp"

using namespace std;

int main(){
    
    Parser p("data/data.dat");
    auto data=p.parse();

    for(size_t i=0; i<data.size(); i++){
        cout<<data[i].to_string()<<endl;
    }


}
