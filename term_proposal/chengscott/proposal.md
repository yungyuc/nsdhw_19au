# nsd PageRank

## Background

PageRank was an algorithm used by Google to rank the importance of resulting web pages from their search engine. PageRank simulates Web surfers, starting at a random page and randomly browsing around, would tend to converge and aggregated, and this process is allowed to iterate many times.

The algorithm accepts a directed graph as the input, where vertices and edges represents web pages and links between, respectively; It outputs PageRank scores for each vertices, which represents the likelihood of a page being visited by random web suffering. The problem can be reformulated in matrix calculations:

Let $d_i$ be the out-degree of vertex $i$. Then, define a column stochastic matrix $M$ as
$$
M_{ji}=
\begin{cases}
\frac{1}{d_i},\ \text{if } i \to j \\
0,\ \text{otherwise.}
\end{cases}
$$
In addition, define Google Matrix as $A = \beta M + (1 - \beta) \left[\frac{1}{N}\right]_{N\times N}$, where $N$ is the number of vertices, and $\beta\in(0,1)$ is a probability to teleport away.

The PageRank equation (matrix version) is:
$$
r = A \cdot r
$$

The calculation requires several passes of iterations to approximate PageRank values.

## Project

In this project, I am going to implement PageRank algorithm with different kinds of BLAS. I expect to get familiar with several BLAS libraries, and compare performance among mine implementations. The code will all be written in modern C++, and be tested with small and large Dataset.


## Reference

- [PageRank Algorithm](https://en.wikipedia.org/wiki/PageRank)
- [Dataset](https://snap.stanford.edu/data/p2p-Gnutella04.html)

