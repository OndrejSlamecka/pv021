/**
 * Copyright 2016 Michal Korbela, Filip Pokryvka, Ondrej Slamecka
 * See the LICENSE file for more information.
 */

#include <bits/stdc++.h>
#include "brca_image.hpp"
#include "MLP.hpp"

using namespace std;

/**
 * Encodes true as {0, 1}, false as {1, 0}.
 */
vector<double> one_hot_bool(bool i) {
    if (i) {
        return {0, 1};
    } else {
        return {1, 0};
    }
}


/**
 * Runs the validation set through the network and computes correctness.
 */
double validate(MLP &nn, vector<BrcaImage> &validation, int n_iterations, double certainty_threshold) {
    int n_correct = 0,
        n_incorrect = 0,
        n_unsure = 0;

    vector<double> result;
    for (unsigned i = 0; i < validation.size(); i++) {
        result = nn.feed(validation[i].data);

        // note that result[0] + result[1] is approximately 1
        if (result[0] > certainty_threshold) {
            if (!validation[i].malignant) {
                n_correct++;
            } else {
                n_incorrect++;
            }
        } else if (result[1] > certainty_threshold) {
            if (validation[i].malignant) {
                n_correct++;
            } else {
                n_incorrect++;
            }
        } else {
            n_unsure++;
        }
    }

    double correctness = (n_correct * 100.0) / validation.size();
    cout << correctness << "%: "
        << validation.size() << " inputs, "
        << n_correct << " correct, "
        << n_incorrect << " incorrect, "
        << n_unsure << " unsure, "
        << n_iterations << " iterations" << endl;

    return correctness;
}


/**
 * Standard dataset normalization.
 */
void normalize(vector<BrcaImage> &input) {
    double a, b;
    for (unsigned i = 0; i<input[0].data.size(); ++i) {
        double min_input = input[0].data[i];
        double max_input = input[0].data[i];
        for (auto & img: input) {
            if (img.data[i]<min_input) {
                min_input = img.data[i];
            }

            if (img.data[i]>max_input) {
                max_input = img.data[i];
            }
        }

        a = (max_input + min_input) / 2;
        b = (max_input - min_input) / 2;
        //cout << "Using normalization constants: (X - " << a << ") / " << b << endl;
        for (auto & img: input) {
            img.data[i] = (img.data[i] - a) / b;
        }
    }

    // test the normalization
    int bad = 0;
    for (auto & img: input) {
        for (auto & data: img.data) {
            if (data < -1.2 || data > 1.2 ) {
                cout << data << " is not in -1 1" << endl;
                bad++;
            }
        }
    }

    if (bad) {
        cout << "Normalization went wrong, "
            << bad << " numbers are not in range <-1,1>!" << endl;
    }
}


int main(int argc, char** argv) {
    if (argc < 3) {
        cerr << "Usage:\t" << argv[0] << " data_path training_set_size\n" << endl;
        cerr << "\tThe program opens the file at the data_path " << endl;
        exit(1);
    }

    unsigned training_size = atoi(argv[2]);

    vector<BrcaImage> all_data = parse_brca_dataset(argv[1]);
    normalize(all_data);

    if (training_size > all_data.size()) {
        cerr << "The given training set size is greater than"
                "the size of the data set." << endl;
        exit(1);
    }

    random_shuffle(all_data.begin(), all_data.end());
    vector<BrcaImage> training, validation;
    for (unsigned i = 0; i < all_data.size(); ++i) {
		if (i < training_size) {
	        training.push_back(all_data[i]);
		} else {
			validation.push_back(all_data[i]);
		}
    }

	// how many times does the network learn each learning instance between
	// validation runs
    unsigned interval = 1000;
    // when to stop
    double threshold = 95;

    // At least how big should the value in the output neuron be to
    // determine a result
    double certainty_threshold = 0.9;

    MLP nn({30,2});
    nn.randomize_weights(-1, 1);

    int n_iterations = 0;
    while (validate(nn, validation, n_iterations, certainty_threshold) < threshold) {
        for (unsigned i = 0; i < interval; i++) {
            for (unsigned j = 0; j < training_size; j++) {
                nn.learn(training[j].data,
                    one_hot_bool(training[j].malignant),
                    0.5, 0.2);
            }
        }
        n_iterations += interval;
    }

    cout << "Learning finished, reached threshold " << threshold << "%" << endl;

    return 0;
}
