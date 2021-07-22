# Final Report

## Generation Vs. Highest Fitness

The following presents a series of generation vs. highest fitness graphs, where the population size is varied. The data used to render each graph, as provided under each heading, is a series of highest fitness values of the generations. In other words, the line-separated values are the highest fitness values of each generation.

The first value is the highest fitness of the first generation, the second value is the highest fitness of the second generation, and so on. X-axis represents the generation numbers and the y-axis represents the fitness values.

Note that each of the graph covers a 30-generation session.

> Highest fitness is the most fitness ever recorded in a training session.

### 50 Genes

__Data:__ 

```
5.431611
5.431611
5.431611
5.431611
5.431611
5.456646
5.456646
5.456646
5.456646
5.456646
7.658571
7.658571
8.265928
8.271417
8.271417
8.277532
8.277532
8.277532
8.277532
8.277532
8.277532
8.277532
8.284661
8.284661
8.284661
8.284661
8.284661
8.284661
8.294047
8.294047
```

![](https://github.com/nahiyan/darwin/raw/gh-pages/pong-progress/book/images/final-report/50.png)

### 100 Genes

__Data:__

```
5.432813
5.432813
5.432813
5.432813
7.066851
7.066851
8.1442
8.231398
8.278016
8.28744
8.28744
8.28744
8.289681
8.289681
8.289681
8.289681
8.289681
8.289681
8.294307
8.294307
8.294307
8.294307
8.294307
8.294307
8.294307
8.294307
8.294307
8.294307
8.294307
8.294307
```

![](https://github.com/nahiyan/darwin/raw/gh-pages/pong-progress/book/images/final-report/100.png)

### 150 Genes

__Data:__

```
5.431562
5.431562
5.431562
8.260981
8.260981
8.271969
8.271969
8.271969
8.271969
8.281805
8.281805
8.281805
8.281805
8.281805
8.281805
8.288877
8.288877
8.292191
8.292191
8.292191
8.292191
8.292191
8.294148
8.294148
8.294148
8.294497
8.295925
8.295925
8.295925
8.39234
```

![](https://github.com/nahiyan/darwin/raw/gh-pages/pong-progress/book/images/final-report/150.png)

### 200 Genes

__Data:__

```
5.431562
7.052027
7.052027
8.275368
8.277686
8.277686
8.282916
8.282916
8.282986
8.290395
8.290395
8.290395
8.290395
8.290395
8.290395
8.290395
8.290395
8.29338
9.171868
9.171868
9.171868
9.298051
9.298051
9.298051
9.298051
9.298051
9.298051
9.298051
9.298051
9.298051
```

![](https://github.com/nahiyan/darwin/raw/gh-pages/pong-progress/book/images/final-report/200.png)

### 250 Genes

__Data:__

```
5.431562
9.04776
9.04776
9.286324
9.286324
9.286324
9.286324
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.311235
9.334922
9.334922
9.334922
```

![](https://github.com/nahiyan/darwin/raw/gh-pages/pong-progress/book/images/final-report/250.png)

### 300 Genes

__Data:__

```
5.432036
9.185127
9.279955
9.279955
9.279955
9.279955
9.279955
9.279955
9.279955
9.279955
9.279955
9.279955
9.279955
9.279955
9.279955
9.279955
9.279955
9.279955
9.279955
9.284306
9.284306
9.284306
9.284306
9.284306
9.328227
9.328227
9.328227
9.328227
9.328227
9.328227
```

![](https://github.com/nahiyan/darwin/raw/gh-pages/pong-progress/book/images/final-report/300.png)


## Population Size vs. Highest Fitness

The following graph shows population size vs. highest fitness at the end of a 30-generation training session.

__Data:__

```
8.294047
8.294307
8.392340
9.298051
9.334922
9.328227
```

![](https://github.com/nahiyan/darwin/raw/gh-pages/pong-progress/book/images/final-report/population-size-v-fitness.png)


The 50-population training session has a highest fitness of 8.29 after 30 generations while the 300-population training session has a highest fitness of 9.33 after 30 generations.

Another way to look at it is that the graph shows the trend in population size vs. plateau height.

## Trend in Generations Required to Reach A Specific Highest Fitness

As the highest fitness at the end of the 50-population training session is 8.29, the following graph compares the population size with the number of generations taken to reach a highest fitness of 8.29.

__Data:__

```
28
18
17
9
1
1
```

![](https://github.com/nahiyan/darwin/raw/gh-pages/pong-progress/book/images/final-report/generations-required-to-reach-certain-highest-fitness.png)


## Observations

- As population size increases, highest fitness at the end of a same-length training session increases, as evident from the __Population Size vs. Max-Fitness__ graph.
- As population size increases, the amount of generations taken to reach a specific highest fitness decreases, as seen from the __Population Size vs. Amount of Generations__ graph.
- From the graphs, it's clear that the drastic change happens between 150 and 200 population sizes.
- As observed from the visualizations, there is always a steep rise from a highest fitness of ~5.5 to ~8.2 and from ~8.2 to ~9.2, till the population size of ~200.
- From population size of 250 to at least 300, there is a steep rise from a highest fitness of ~5.5 to ~9.2.
- The populations usually stay within a local maximum for quite some time before having a steep rise, eventually nearing the global maximum.