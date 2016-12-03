/*
 *  To test if the parser is reading the data file correctly
 */

#include<bits/stdc++.h>
#include "brca_image.hpp"

using namespace std;

int main(){
    auto data = parse_brca_dataset("data/data.dat");

    for(size_t i=0; i<data.size(); i++){
        cout<<data[i].to_string()<<endl;
    }
}
