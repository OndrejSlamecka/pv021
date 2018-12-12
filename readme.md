Neural Network Diagnosing Tumorous Cells
========================================

Feed about 80 of the 567 instances of the
[Breast Cancer Wisconsin (Diagnostic) Data Set](http://archive.ics.uci.edu/ml/datasets/Breast+Cancer+Wisconsin+%28Diagnostic%29)
to our own implementation of multilayer perceptron and after short
learning achieve a 93.6 % correctness rate on the validation dataset.

First we normalize the data and then the MLP learns using gradient
descent with backpropagation and momentum. The MLP works very well with
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

    93.3063%: 493 inputs, 460 correct, 11 incorrect, 22 unsure, 7000 iterations

Since the 76 instances are selected at random you may observe some bad
learning if you are unlucky. Or good learning if you are lucky.

Hyperparameter experiments
--------------------------

The program has four main hyperparameters: the number and size of the
hidden layers, learning rate, momentum factor, certainty threshold
(determines how sure the network has to be to report a result instead of
saying it is unsure).

Below we list several well performing network configurations. Most
reasonable configurations achieve good results, the network has to be
set up in a really wrong way to not work (e.g. very low learning rate,
extreme momentum, ten inner layers,...).

    name  topology     learning   momentum  certainty  max certainty  incorrect/unsure
                       rate                 threshold  in %
    nn00  {30,2}       1          0         0.7        96.348         14/4
    nn01  {30,2}       1          0         0.95       92.900         10/25
    nn02  {30,2}       1          0.5       0.7        96.348         14/4
    nn03  {30,2}       1          0.5       0.95       93.509         11/21
    nn04  {30,2}       0.1        0         0.7        96.348         13/5
    nn05  {30,2}       0.1        0         0.95       92.900         10/25
    nn06  {30,2}       0.1        0.5       0.7        96.348         13/5
    nn07  {30,2}       0.1        0.5       0.95       93.712         11/20
    nn08  {30,10,2}    0          0         0.7        96.754         16/0
    nn09  {30,10,2}    0          0         0.95       96.348         15/3
    nn10  {30,10,2}    0          0.5       0.7        96.754         16/0
    nn11  {30,10,2}    0          0.5       0.95       96.146         14/5
    nn12  {30,10,2}    0.1        0         0.7        96.754         16/0
    nn13  {30,10,2}    0.1        0         0.95       96.146         12/7
    nn14  {30,10,2}    0.1        0.5       0.7        96.754         16/0
    nn15  {30,10,2}    0.1        0.5       0.95       96.348         15/3
    nn16  {30,10,4,2}  0          0         0.7        96.957         15/0
    nn17  {30,10,4,2}  0          0         0.95       96.957         15/0
    nn18  {30,10,4,2}  0          0.5       0.7        96.957         15/0
    nn19  {30,10,4,2}  0          0.5       0.95       96.754         15/1
    nn20  {30,10,4,2}  0.1        0         0.7        96.957         15/0
    nn21  {30,10,4,2}  0.1        0         0.95       96.957         15/0
    nn22  {30,10,4,2}  0.1        0.5       0.7        96.957         15/0
    nn23  {30,10,4,2}  0.1        0.5       0.95       96.754         15/1

The networks were trained for as long as one minute on 76 examples. The
configurations with zero momentum need more time to attain their good
results.


Authors
-------

* Michal Korbela, Filip Pokrývka -- hyper-parameter optimization, data
  normalization and processing
* Ondřej Slámečka -- project leader, MLP implementation, data processing
