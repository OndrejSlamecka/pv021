#include<bits/stdc++.h>

using namespace std;

struct Image{
    int id;
    bool value;
    vector<double> data;

    //for debug
    string to_string(){
        stringstream s;
        s<<"id: "<<id<<" value: "<<value<<" data:(";
        s<<setprecision(4)<<fixed;
        for(size_t i=0; i<data.size(); i++)
            s<<data[i]<<" \n"[i==data.size()-1];
        s<<")";
        return s.str();
    }

};


class Parser{

    string filename;
    
    public:
    Parser(string filename): filename(filename) {}

    vector<Image> parse(){
        
        //output vector of data
        vector<Image> res;

        //open file for reading
        ifstream in(filename);
        if(!in){
            throw std::invalid_argument("Can't open input file !");
        }

        //format is ID, value, data1, data2,...,data32
        //assume that input is correct format

        //read whole file
        int id;
        while(in>>id){
            Image I;
            
            //set id
            I.id=id;

            //set value
            char value;
            in>>value;
            I.value=(value=='M');

            //read 30 doubles
            for(int i=0; i<30; i++){
                double x;
                in>>x;
                I.data.push_back(x);
            }
            
            res.push_back(I);
        }

        //return output
        return res;
        

    }
    

};
