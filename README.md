# Pomegranate PIe
Calculating PI using distributed computing

## Goal
Gain a better understanding of distributed computing and clustering by creating a program that can compute as many digits of pi as fast as possible using an arbitrary number of nodes.

The name comes from the fact that the pomegranate combines many small parts, just like a cluster of CPUs working together to achieve a greater goal.

## Which PI formula?
Due to its easily parallelizable nature, the [BBP formula](https://en.wikipedia.org/wiki/Bailey%E2%80%93Borwein%E2%80%93Plouffe_formula) is a perfect candidate for this project

It allows to compute the nth hexadecimal digit of pi without having to calculate the previous n-1

## Cluster architecture
The cluster will be made of:
- 1 coordinator
- N worker nodes

The worker nodes will connect to the coordinator and receive **work units** to complete, each consisting of a (still to be defined) number of terms of the BBP formula to compute. The coordinator will then be responsible for combining the hexadecimal digits as they come in and converting the result to decimal.

The coordinator will also provide a web interface from which the entire operation can be monitored and controlled, and a display page that shows the current progress (number of digits computed, digits per second, current representation of pi)
