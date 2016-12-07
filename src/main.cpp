/**
 * Copyright 2016 Michal Korbela, Filip Pokrývka, Ondřej Slámečka
 * See the LICENSE file for more information.
 */
#include <iostream>
#include <vector>
#include <queue>

#include <algorithm>
#include <ctime>        
#include <cstdlib> 

//#include "../mnist/include/mnist/mnist_reader.hpp"

#include "brca_image.hpp"

#include "MLP.hpp"

double validate(MLP &nn, std::vector<BrcaImage> &validation, std::queue<double> &q, double treshold) {
    int corr = 0;
    int unsure = 0;
    std::vector<double> r;
    for(int i = 0; i<validation.size(); i++) {
        r = nn.feed(validation[i].data);
        if (r[0] >0.1 && r[0] < 0.9) {
            unsure++;
            continue;
        }
        bool res=0;
        if(r[0] > 0.9) res=1;
        if(res == validation[i].malignant) {
            corr++;
        }
    }
    std::cout << "validation: " << corr << "/" << validation.size() << " = " << (corr*100.0)/validation.size() << "%" << " (" << unsure << " unsure)" << std::endl;
    if (unsure == 100) std::cout << "unsure: " << r[0] << std::endl;

    return (corr*100.0)/validation.size();
}


int main(int argc, char** argv) {
    if (argc != 2 ) {
        throw std::invalid_argument("No data file specified "
                                "please provide it as first parameter");
    }

    MLP nn({30,90,1});

    std::vector<BrcaImage> all_data = parse_brca_dataset(argv[1]);
    nn.randomize_weights(-1, 1);
    std::queue<double>q;
    while(validate(nn, all_data, q, 0.2) > 1.0) {
        std::cout << "reinitializing weights" << std::endl;
        nn.randomize_weights(-1, 1);
    } 

    //std::srand(unsigned(std::time(0)));
    //std::random_shuffle(all_data.begin(), all_data.end());
/*    int iter = 30000;
    for (int z = 0; z < iter; ++z) {
        if(z%(iter/100) == 0) {
            std::cout << z*100/iter << "%" << std::endl;
        }
        for(int i = 0, k=0; i<50; ++k) {
            if(all_data[k].malignant) {
                nn.learn(all_data[k].data, {all_data[k].malignant}, 0.5, 0.1);
                i++;
            }
        }
        for(int i = 0, k=0; i<50; ++k) {
            if(!all_data[k].malignant) {
                nn.learn(all_data[k].data, {all_data[k].malignant}, 0.5, 0.1);
                i++;
            }
        }
    }*/
    

    int training_size = 50;
    int validation_size = 100;
    int tot_size = (training_size + validation_size) / 2;
    std::vector<BrcaImage> training, validation;
    for(int i = 0, k=0; i<tot_size; ++k) {
        if(all_data[k].malignant) {
            if(i<training_size/2)
                training.push_back(all_data[k]);
            else
                validation.push_back(all_data[k]);
            i++;
        }
    }
    for(int i = 0, k=0; i<tot_size; ++k) {
        if(!all_data[k].malignant) {
            if(i<training_size/2)
                training.push_back(all_data[k]);
            else
                validation.push_back(all_data[k]);
            i++;
        }
    }

    int queue_size = 5;
    int interval = 1000;
    std::queue<double> efficiency;
    double treshold = 95.0;
    for(int i = 0; i<queue_size; i++) efficiency.push(0);
    

//    while(validate(nn, validation, efficiency, treshold) < treshold) {
    while(validate(nn, all_data, efficiency, treshold) < treshold) {
        for(int i = 0; i<interval; i++) {
            for(int j = 0; j<training_size; j++) {
                nn.learn(training[j].data, {(double) training[j].malignant}, 0.5, 0.1);
            }
        }
    }

    /*
    for (int z = 0; z < iter; ++z) {
        if(z%(iter/100) == 0) {
            std::cout << z*100/iter << "%" << std::endl;
        }
        for(int i = 0, k=0; i<50; ++k) {
            if(all_data[k].malignant) {
                nn.learn(all_data[k].data, {all_data[k].malignant}, 0.5, 0.1);
                i++;
            }
        }
        for(int i = 0, k=0; i<50; ++k) {
            if(!all_data[k].malignant) {
                nn.learn(all_data[k].data, {all_data[k].malignant}, 0.5, 0.1);
                i++;
            }
        }
    }*/
     
    int corrm=0, corrn=0;
    for (int i = 0; i < all_data.size(); i++) {
        std::vector<double> r = nn.feed(all_data[i].data);
        if(all_data[i].malignant && r[0]>0.5) corrm++;
        if(!all_data[i].malignant && r[0]<0.5) corrn++;
        std:: cout << "exp " << all_data[i].malignant << ", act " << r[0] << std::endl;
    }
    std::cout << corrm << " " << corrn << " " << corrn+corrm << "/" << all_data.size() << " = " << ((double)corrm+corrn)/all_data.size() <<  "%" << std::endl;
/*
    std::vector<double> r = nn.feed({1, 1});
    std::cout << "exp 0, act " << r[0] << std::endl;

    r = nn.feed({1, 0});
    std::cout << "exp 1, act " << r[0] << std::endl;

    r = nn.feed({0, 1});
    std::cout << "exp 1, act " << r[0] << std::endl;

    r = nn.feed({0, 0});
    std::cout << "exp 0, act "  << r[0] << std::endl; */


    // auto dataset =
    //     mnist::read_dataset<std::vector, std::vector, uint8_t, uint8_t>();

    // for (int i = 255; i --> 0; ) {
        // if (dataset.training_images[5][i] > 0) {
            // std::cout << i << std::endl;
            // std::cout << dataset.training_images[5][i] << std::endl;
            // break;
        // }
    // }


    return 0;
}
