Neural Network Diagnosing Tumorous Cells
========================================

Feed 76 of the 567 instances of the
[Breast Cancer Wisconsin (Diagnostic) Data Set](http://archive.ics.uci.edu/ml/datasets/Breast+Cancer+Wisconsin+%28Diagnostic%29)
to our own implementation of multilayer perceptron and after quick
learning achieve a 92 % correctness rate on the whole dataset
(on the remaining 8 % the network is unsure but does not misclassify).

With longer training it can achieve up to 95 % correctness. The MLP
works best with just 30 input neurons and two input neurons suggesting
the input maps to output in a linear way (there's a single weight matrix
in the network).

The MLP learns using gradient descent with backpropagation and momentum.

If you want better correctness have a look at radial basis function
networks ([an example application](http://www.acit2k.org/ACIT/2012Proceedings/13233.pdf)).

Run
---

    git clone --recursive git@github.com:OndrejSlamecka/pv021.git
    make
    ./bin/nn data/reorganized.dat 76

Code Style
----------

4 space indent. Please check your contributions with [this linter](https://github.com/google/styleguide/blob/gh-pages/cpplint/cpplint.py).

    python cpplint.py --extensions=cpp,hpp src/*

Authors
-------

* Michal Korbela, Filip Pokrývka -- data processing and finding the
  right network topology for best learning
* Ondřej Slámečka -- MLP with backpropagation
