/**
 * Copyright 2016 Michal Korbela, Filip Pokrývka, Ondřej Slámečka
 * See the LICENSE file for more information.
 */
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <cmath>  // exp
#include <random>
#include <stdexcept>


/**
 * Multi Layer Perceptron
 *
 * This implementation is not thread safe!
 */
class MLP {
    /**
     * n_neurons[l] -- the number of neurons in layer l.
     */
    std::vector<size_t> n_neurons;


    /**
     * The number of layers.
     */
    size_t n_layers;


    /**
     * w[l][i][j] -- the weight of the connection between
     * neuron i of layer l and neuron j of layer l + 1
     */
    std::vector<std::vector<std::vector<double>>> w;


    /**
     * ksi[l][i] -- the inner potential of neuron i in layer l
     */
    std::vector<std::vector<double>> ksi;


    /**
     * o[l][i] -- the output of neuron i in layer l
     * (o = map (map sigma) ksi).
     */
    std::vector<std::vector<double>> o;


    /**
     * The activation function.
     */
    double sigma(double z) {
        return 1 / (1 + exp(-z));
    }


    /**
     * The derivative of the activation function.
     */
    double sigma_prime(double z) {
        return sigma(z) * (1 - sigma(z));
    }


 public:
    /**
     * By calling NeuralNetwork(2,3,2,1)
     * create a new network with
     * - an input layer with 2 neurons,
     * - one hidden layer with 3 neurons,
     * - one hidden layer with 2 neurons
     * - and an output layer with 1 neuron.
     *
     * Call randomize_weights to initialize the connection weights!
     *
     * This implementation assumes your network will have at least the
     * input and output layers.
     */
    MLP(std::initializer_list<size_t> il_n_neurons)
        : n_neurons(il_n_neurons),
          n_layers(n_neurons.size()),
          w(n_layers),
          ksi(n_layers), o(n_layers) {
        o[0].resize(n_neurons[0] + 1);
        o[0][n_neurons[0]] = 1;

        for (unsigned int l = 1; l < n_layers; ++l) {
            // +1 for the bias input and weight
            w[l-1].resize(n_neurons[l-1] + 1);
            for (size_t r = 0; r < n_neurons[l-1] + 1; ++r) {
                w[l-1][r].resize(n_neurons[l]);
            }

            ksi[l].resize(n_neurons[l]);
            o[l].resize(n_neurons[l] + 1);
            o[l][n_neurons[l]] = 1;  // bias input
        }

        // The output layer doesn't have bias
        o[n_layers - 1].resize(n_neurons[n_layers - 1]);
    }


    /**
     * Randomizes the weights of the connections.
     * Backpropagation would not work with zero weights
     * (try plugging in zeros to see that).
     */
    void randomize_weights(double min, double max) {
        std::random_device random_device;
        std::mt19937 generator(random_device());
        std::uniform_real_distribution<> distribution(min, max);
        for (size_t l = 0; l < n_layers - 1; ++l) {
            for (size_t i = 0; i < n_neurons[l] + 1; ++i) {
                for (size_t j = 0; j < n_neurons[l + 1]; ++j) {
                    w[l][i][j] = distribution(generator);
                }
            }
        }
    }


    /**
     * Feed the input to the network and return its output.
     */
    const std::vector<double>& feed(std::vector<double> input) {
        // Copy input to the input layer
        for (size_t i = 0; i < input.size(); ++i) {
            o[0][i] = input[i];
        }

        for (size_t l = 1; l < n_layers; ++l) {
            // The inner potential of each neuron is the sum of its
            // weighted inputs
            for (size_t i = 0; i < n_neurons[l]; ++i) {
                ksi[l][i] = 0;
                for (size_t j = 0; j < n_neurons[l-1] + 1; ++j) {
                    ksi[l][i] += w[l-1][j][i] * o[l-1][j];
                }
            }

            // The output of each neuron is the sigma of its inner
            // potential
            for (size_t j = 0; j < ksi[l].size(); ++j) {
                o[l][j] = sigma(ksi[l][j]);
            }
        }

        return o[n_layers - 1];
    }


    /**
     * For given input this method modifies the weights to bring the
     * actual output closer to the expected_output.
     */
    void learn(std::vector<double> input, std::vector<double> expected_output,
        double learning_rate) {
        // A handy shortcut
        int n_last_layer = n_layers - 1;

        // Check the sizes match
        if (expected_output.size() != n_neurons[n_last_layer]) {
            throw std::invalid_argument("The size of expected output "
                    "doesn't match the size of the output layer");
        }

        // Run the input through the network to work out inner potential
        // and output value of each neuron
        feed(input);

        // Compute the error gradient in the output space
        // (backpropagation)
        std::vector<std::vector<double>> Ek_over_y(n_layers);
        Ek_over_y[n_last_layer].resize(n_neurons[n_last_layer]);


        // For output neurons it's just the difference with expected
        // output
        for (size_t j = 0; j < n_neurons[n_last_layer]; ++j) {
            Ek_over_y[n_last_layer][j] =
                o[n_last_layer][j] - expected_output[j];
        }

        // For hidden neurons it's calculated with a recurrent formula
        for (unsigned int l = n_last_layer; l --> 0; ) {
            Ek_over_y[l].resize(n_neurons[l] + 1);

            for (size_t j = 0; j < n_neurons[l] + 1; ++j) {
                Ek_over_y[l][j] = 0;
                for (size_t r = 0; r < n_neurons[l + 1]; ++r) {
                    Ek_over_y[l][j] +=
                        Ek_over_y[l+1][r]
                        * sigma_prime(o[l+1][r])
                        * w[l][j][r];
                }
            }
        }

        // Transform the error gradient to the weight space and update
        // the weights in the opposite direction
        for (unsigned int l = 0; l < n_layers - 1; ++l) {
            for (size_t i = 0; i < n_neurons[l] + 1; ++i) {
                for (size_t j = 0; j < n_neurons[l+1]; ++j) {
                    w[l][i][j] -= learning_rate *
                        Ek_over_y[l+1][j]
                        * sigma_prime(ksi[l+1][j])
                        * o[l][i];
                }
            }
        }
    }
};
