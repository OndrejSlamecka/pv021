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

double validate(MLP &nn, std::vector<BrcaImage> &validation, std::deque<Stats> &q) {
    Stats stats = {0,0,0};
    double unsure_min = 1, unsure_max = 0;
    std::vector<double> r;
    for(unsigned i = 0; i<validation.size(); i++) {
        r = nn.feed(validation[i].data);
        if (r[0] >= 0.1 && r[0] <= 0.9) {
            if(r[0] < unsure_min)
                unsure_min = r[0];
            if(r[0] > unsure_max)
                unsure_max = r[0];
            stats.unsure++;
        //    continue;
        }
        bool res=0;
        if(r[0] >= 0.5) res=1;
        if(res == validation[i].malignant) {
            stats.corr++;
        }
        else {
            stats.fail++;
        }
    }
    std::cout << "validation: " << stats.corr << "/" << validation.size() << " = " << (stats.corr*100.0)/validation.size() << "%" << " (" << stats.unsure << " unsure in range " << unsure_min << "-" << unsure_max << ")" << std::endl;
    q.push_back(stats);
    return (stats.corr*100.0)/validation.size();
}


int main(int argc, char** argv) {
    if (argc != 2 ) {
        throw std::invalid_argument("No data file specified "
                                "please provide it as first parameter");
    }

    MLP nn({30,10,1});

    std::vector<BrcaImage> all_data = parse_brca_dataset(argv[1]);
    nn.randomize_weights(-1, 1);

    unsigned training_size = 569;
    unsigned validation_size = 0;
    unsigned tot_size = training_size + validation_size;
    if (tot_size > all_data.size())
        throw std::invalid_argument("please decreraining or validation set size");
    
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
    double treshold = 85.0;

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
                nn.learn(training[j].data, {(double) training[j].malignant}, 0.5, 0.1);
            }
        }
    }

    std::cout << "found good efficiency on valid set, here is efficiency on all data:\n";
    validate(nn, all_data, efficiency);

    return 0;
}
