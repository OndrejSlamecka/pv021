/**
 * Copyright 2016 Michal Korbela, Filip Pokrývka, Ondřej Slámečka
 * See the LICENSE file for more information.
 */
#include <iostream>
#include <vector>

#include "../mnist/include/mnist/mnist_reader.hpp"

#include "MLP.hpp"

int main() {
    MLP nn({2, 2, 1});
    nn.randomize_weights(-1, 1);

    for (int i = 0; i < 10000; ++i) {
        nn.learn({0, 0}, {0}, 0.5);
        nn.learn({0, 1}, {1}, 0.5);
        nn.learn({1, 0}, {1}, 0.5);
        nn.learn({1, 1}, {0}, 0.5);
    }

    std::vector<double> r = nn.feed({1, 1});
    std::cout << "exp 0, act " << r[0] << std::endl;

    r = nn.feed({1, 0});
    std::cout << "exp 1, act " << r[0] << std::endl;

    r = nn.feed({0, 1});
    std::cout << "exp 1, act " << r[0] << std::endl;

    r = nn.feed({0, 0});
    std::cout << "exp 0, act "  << r[0] << std::endl;


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
