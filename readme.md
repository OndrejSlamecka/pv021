Neural Network Diagnosing Tumorous Cells
========================================

Feed about 80 of the 567 instances of the
[Breast Cancer Wisconsin (Diagnostic) Data Set](http://archive.ics.uci.edu/ml/datasets/Breast+Cancer+Wisconsin+%28Diagnostic%29)
to our own implementation of multilayer perceptron and after short
learning achieve a 93.6 % correctness rate on the validation dataset.

First we normalize the data and then the MLP learns using gradient
descent with backpropagation and momentum. The MLP works very good with
just 30 input neurons (one for each number in the input) and two output
neurons (encoding malignant/benign prediction) suggesting the input maps
to output in a linear way (there's a single weight matrix in the
network).

If you want better correctness have a look at radial basis function
networks ([an example
application](http://www.acit2k.org/ACIT/2012Proceedings/13233.pdf)).


Run
---

    git clone --recursive https://github.com/OndrejSlamecka/pv021.git
    make
    ./bin/nn data/data.dat 76


With the above arguments the program will repeatedly train the network 1000 times with 76 instances and then output a line similar to

    92.9701%: 569 inputs, 529 correct, 14 incorrect, 26 unsure, 40000 iterations


Since the 76 instances are selected at random you may observe some bad
learning if you are unlucky. Or good learning if you are lucky.

Hyperparameter experiments
--------------------------

The program has four main hyperparameters: the number and size of thr
hidden layers, learning rate, momentum factor, certainty threshold
(determines how sure the network has to be to report a result instead of
saying it is unsure).

Below we list several well performing network configurations. Most
reasonable configurations achieve good results, the network has to be
set up in a really wrong way to not work (e.g. very low learning rate,
extreme momentum, ten inner layers,...).

WARNING: The certainty below includes the learning set. The table will
be fixed soon.

    name  topology     learning   momentum  certainty  max certainty  incorrect/unsure
                       rate                 threshold  in %
    nn00  {30,2}       1          0.5       0.7        95.0791        23/5
    nn01  {30,2}       1          0.5       0.95       92.6186        12/30
    nn02  {30,2}       1          0         0.7        95.2548        23/4
    nn03  {30,2}       1          0         0.95       92.7944        11/30
    nn04  {30,2}       0.1        0.5       0.7        94.7276        22/8
    nn05  {30,2}       0.1        0.5       0.95       92.2671        8/36
    nn06  {30,2}       0.1        0         0.7        94.9033        22/7
    nn07  {30,2}       0.1        0         0.95       91.7399        7/40
    nn08  {30,10,2}    1          0.5       0.7        95.4306        23/3
    nn09  {30,10,2}    1          0.5       0.95       94.7276        19/11
    nn10  {30,10,2}    1          0         0.7        95.6063        24/1
    nn11  {30,10,2}    1          0         0.95       94.7276        17/13
    nn12  {30,10,2}    0.1        0.5       0.7        95.4306        24/2
    nn13  {30,10,2}    0.1        0.5       0.95       94.3761        20/12
    nn14  {30,10,2}    0.1        0         0.7        95.2548        24/3
    nn15  {30,10,2}    0.1        0         0.95       93.8489        20/15
    nn16  {30,10,4,2}  1          0.5       0.7        96.8366        18/0
    nn17  {30,10,4,2}  1          0.5       0.95       96.8366        16/2
    nn18  {30,10,4,2}  1          0         0.7        97.188         16/0
    nn19  {30,10,4,2}  1          0         0.95       96.8366        16/2
    nn20  {30,10,4,2}  0.1        0.5       0.7        96.8366        16/2
    nn21  {30,10,4,2}  0.1        0.5       0.95       96.8366        16/2
    nn22  {30,10,4,2}  0.1        0         0.7        96.8366        18/0
    nn23  {30,10,4,2}  0.1        0         0.95       96.3093        15/6


The networks were trained for as long as one minute on 80 examples. The
configurations with zero momentum need more time to attain their good
results.


Authors
-------

* Michal Korbela, Filip Pokrývka -- hyper-parameter optimization, data
  normalization and processing
* Ondřej Slámečka -- project leader, MLP implementation, data processing
