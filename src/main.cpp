/**
 * Copyright 2016 Michal Korbela, Filip Pokrývka, Ondřej Slámečka
 * See the LICENSE file for more information.
 */

#include <bits/stdc++.h>

#include "brca_image.hpp"

#include "MLP.hpp"

struct Stats {
    int corr;
    int unsure;
    int fail;
};

std::vector<double> transform(bool i) {
    std::vector<double> v;
    if(i) {
        v.push_back(0); v.push_back(1);
    }
    else {
        v.push_back(1); v.push_back(0);
    }
    return v;
}

double validate(MLP &nn, std::vector<BrcaImage> &validation) {
    Stats stats = {0,0,0};
    std::vector<double> r1, r2;
    for (unsigned i = 0; i<validation.size(); i++) {
        r1 = nn.feed(validation[i].data);
        if(r1[0]<0.5) r1[0] = 0;
        else r1[0] = 1;
        if(r1[1]<0.5) r1[1] = 0;
        else r1[1] = 1;
        if(r1 == transform(validation[i].malignant)) {
            stats.corr++;
        }
        else if (r2 == transform(!validation[i].malignant) ){
            stats.fail++;
        }
        else {
            stats.unsure++;
        }
    }
    std::cout << (stats.corr*100.0)/validation.size() << "%: " << stats.corr << "/" << validation.size() << " (" << stats.fail << " incorrect, " << stats.unsure << " unsure)\n";
    return (stats.corr*100.0)/validation.size();
}

void normalize(std::vector<BrcaImage> &input, double a, double b) {
    double min_input = input[0].data[0];
    double max_input = input[0].data[0];
    for (auto & img: input) 
        for (auto & data: img.data) {
            if (data<min_input)
                min_input = data;
            if (data>max_input)
                max_input = data;
        }
    a = (max_input + min_input) / 2;
    b = (max_input - min_input) / 2;
    std::cout << "using normalization constants: (X-" << a << ")/" << b << "\n";
    for (auto & img: input) 
        for (auto & data: img.data)
            data = (data-a)/b;

    // test of normalisation
    int bad = 0;
    for (auto & img: input)
        for (auto & data: img.data) {
            if (data < -1 || data > 1)
                bad++;
        }
    if (bad)
        std::cout << "normalization went wrong, " << bad << " numbers are not in range <-1,1>\n";
    else
        std::cout << "data normalized successfully\n";
}


int main(int argc, char** argv) {
    if (argc < 2)
        throw std::invalid_argument("No data file specified\n"
                                "USAGE: bin/nn traiing_file_name training_set_size"
                                );
    if (argc < 3)
        throw std::invalid_argument("No training set size specified "
                                "USAGE: bin/nn traiing_file_name training_set_size"
                                );
    
    unsigned training_size;
    
    std::stringstream ss;
    ss << argv[2];
    ss >> training_size;

    std::vector<BrcaImage> all_data = parse_brca_dataset(argv[1]);
    double a=0, b=0;
    normalize(all_data, a, b);
    
    if (training_size > all_data.size())
        throw std::invalid_argument("please decrease training set size");

    std::vector<BrcaImage> training, validation;
    for(unsigned i = 0; i<training_size; ++i) 
        training.push_back(all_data[i]);
    
    // how often is validation run
    unsigned interval = 1000;
    // when to stop
    double treshold = 92.5;

    MLP nn({30,2});
    nn.randomize_weights(-1, 1);
    int i = -1;

    while (validate(nn, all_data) < treshold) {
        ++i;
        for (unsigned i = 0; i<interval; i++)
            for (unsigned j = 0; j<training_size; j++)
                nn.learn(training[j].data, transform(training[j].malignant), 0.5, 0.0);
    }

    std::cout << "Learning finished, reached treshold " << treshold << "%" << std::endl;

    return 0;
}
