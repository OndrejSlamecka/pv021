/**
 * Copyright 2016 Michal Korbela, Filip Pokrývka, Ondřej Slámečka
 * See the LICENSE file for more information.
 */

#include <bits/stdc++.h>

//#include "../mnist/include/mnist/mnist_reader.hpp"

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

double validate(MLP &nn, std::vector<BrcaImage> &validation, std::deque<Stats> &q) {
    Stats stats = {0,0,0};
    std::vector<double> r1, r2;
    for(unsigned i = 0; i<validation.size(); i++) {
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
    std::cout << "validation: " << stats.corr << "/" << validation.size() << " = " << (stats.corr*100.0)/validation.size() << "%" << " (" << stats.unsure << " unsure)" << std::endl;
    q.push_back(stats);
    return (stats.corr*100.0)/validation.size();
}

void normalize(std::vector<BrcaImage> &input, double a, double b) {
    double min_input = input[0].data[0];
    double max_input = input[0].data[0];
    for(auto & img: input) 
        for(auto & data: img.data) {
            if (data<min_input)
                min_input = data;
            if (data>max_input)
                max_input = data;
        }
    a = (max_input + min_input) / 2;
    b = (max_input - min_input) / 2;
    std::cout << "using normalisation constants: (X-" << a << ")/" << b << "\n";
    for(auto & img: input) 
        for(auto & data: img.data)
            data = (data-a)/b;

    // test of normalisation
    int bad = 0;
    for(auto & img: input)
        for(auto & data: img.data)
            if(data < -1 || data > 1)
                bad++;
    if(bad) {
        std::cout << "normalization went wrong, " << bad << " numbers are not in range <-1,1>\n";
    }
    else {
        std::cout << "data normalized successfully\n";
    }
}


int main(int argc, char** argv) {
    if (argc != 2 ) {
        throw std::invalid_argument("No data file specified "
                                "please provide it as first parameter");
    }

    MLP nn({30,10,2});

    std::vector<BrcaImage> all_data = parse_brca_dataset(argv[1]);
    double a=0, b=0;
    normalize(all_data, a, b);
    nn.randomize_weights(-1, 1);

    unsigned training_size = 300;
    unsigned validation_size = 0;
    unsigned tot_size = training_size + validation_size;
    if (tot_size > all_data.size())
        throw std::invalid_argument("please decrease training or validation set size");
    
    std::vector<BrcaImage> training, validation;
    for(unsigned i = 0; i<training_size; ++i) {
        training.push_back(all_data[i]);
    }
    if (validation_size == 0)
        validation = all_data;
    else {
        for(unsigned i = training_size; i<tot_size; ++i) 
            validation.push_back(all_data[i]);
    }

    unsigned queue_size = 500;
    unsigned interval = 100;
    std::deque<Stats> efficiency;
    double treshold = 93.0;

    while(validate(nn, validation, efficiency) < treshold) {
        // if efficiency does not grow for queue_size intervals, randomize weights
        while(efficiency.size() > queue_size)
            efficiency.pop_front();
        if(efficiency.size() == queue_size) {
            Stats first = efficiency.front(),
                  last = efficiency.back();
            // lobotomy :)
            if(last.corr <= first.corr) {
                std::cout << "reinitializing weights\n";
                efficiency.clear();
                nn.randomize_weights(-1, 1);
            }
        }
        for(unsigned i = 0; i<interval; i++) {
            for(unsigned j = 0; j<training_size; j++) {
                nn.learn(training[j].data, transform(training[j].malignant), 0.5, 0.1);
            }
        }
    }

    std::cout << "found good efficiency on valid set, here is efficiency on all data:\n";
    validate(nn, all_data, efficiency);

    return 0;
}
