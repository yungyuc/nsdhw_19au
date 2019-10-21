nsd Term Proposal
===
GitHub@stvreumi

## Motivation
- When I interned in NCHC last summer, I've been asked to use some tools like [hyperopt](https://github.com/hyperopt/hyperopt) to tune the hyperparameters of deep learning models. I know how to use the tools to run the experiments to produce possible better hyperparameters, but I don't know what's the math behind these tools.
- **Bayesian Optimization**, is the keyword showing up in almost every optimization tool that I've used or interested in. I want to understand what Bayesian Optimization is by research and implementation in the final project.

## Status
- Actually, I have no idea of Bayesian Optimization. And I've forgotten most things in the probability course.
- But I have found sources called "[Think in Bayes](http://www.greenteapress.com/thinkbayes/thinkbayes.pdf)". The goal of this book is trying to teach readers the concepts of Bayesian Optimization through Python code.
  (Although I only finish preface when I writing this proposal)

## Plan

- My goal of the term project is trying to finish the materials, and implement some core functions of Bayesian Optimization in C++.
- I want to bind the core functions with `pybind11`, and I will write a Python program to call it to generate data and do some analysis on it.

## Resources
- [Think in Bayes](http://www.greenteapress.com/thinkbayes/thinkbayes.pdf)
  - [python2 repo](https://github.com/AllenDowney/ThinkBayes)
  - [python3 repo](https://github.com/AllenDowney/ThinkBayes2)
- [Automated Machine Learning Hyperparameter Tuning in Python](https://towardsdatascience.com/automated-machine-learning-hyperparameter-tuning-in-python-dfda59b72f8a)