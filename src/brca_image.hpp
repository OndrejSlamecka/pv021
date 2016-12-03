/**
 * Data structure and parser for
 * http://archive.ics.uci.edu/ml/datasets/Breast+Cancer+Wisconsin+%28Diagnostic%29
 */
#include <bits/stdc++.h>

/**
 * Single screening for BReast CAncer.
 */
struct BrcaImage {
    int id;
    bool malignant;
    std::vector<double> data;

    std::string to_string() {
        std::stringstream s;
        s << "id: " << id << " malignant: " << malignant << " data:(";
        s << std::setprecision(4) << std::fixed;
        for (size_t i = 0; i < data.size(); i++) {
            s << data[i] << " \n"[i == data.size() - 1];
        }
        s << ")";
        return s.str();
    }
};


std::vector<BrcaImage> parse_brca_dataset(std::string filename) {
    // Open file for reading
    std::ifstream input(filename);
    if (!input) {
        throw std::invalid_argument("Can't open input file!");
    }

    // Format is ID, M/B (for malignant/benign), data1, data2, ..., data32
    std::vector<BrcaImage> result;
    int id;
    while (input >> id){
        BrcaImage image;
        image.id = id;

        char value;
        input >> value;
        image.malignant = (value=='M');

        for (int i = 0; i < 30; i++) {
            double x;
            input >> x;
            image.data.push_back(x);
        }

        result.push_back(image);
    }

    return result;
}
