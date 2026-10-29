# Bipartite Graph and Suggestion Algorithms in C

A command-line program in C that recommends movies with a bipartite graph. Users and movies are the two sets of nodes; every rating is an edge between a user and a movie, and the rating (1 to 5) is its weight. It was written in December 2024 as a data structures and algorithms project by Hasan Alper Ayaz and corrected in October 2026 (see [Changes since December 2024](#changes-since-december-2024)).

For a chosen user the program recommends movies in five ways:

1. random movies the user has not seen,
2. the most linked movies (rated by the most users) the user has not seen,
3. the highest rated unseen movies of the user who shares the most movies with the chosen user,
4. the movies with the closest weighted distance in the graph (Dijkstra's algorithm with the rating as the weight of an edge; movies at the same distance are ordered by degree, then by movie ID),
5. the highest rated unseen movies of the user with the most similar ratings (at least `MIN_COMMON` movies in common).

Users are stored as linked lists and movies are sorted with bubble sort; the project is about data structures, not about speed. A written report is in [`docs/REPORT.md`](docs/REPORT.md) ([PDF](docs/project_report.pdf)).

## Data

The MovieLens 100K data set (943 users, 1,682 movies, 100,000 ratings) of GroupLens Research, University of Minnesota, is **not** part of this repository. Its terms do not allow redistribution without separate permission, require an acknowledgement, forbid implying an endorsement by the University of Minnesota or GroupLens, and forbid commercial or revenue-bearing use without permission. Download it from <https://grouplens.org/datasets/movielens/100k/>, or run:

```sh
make download-data
```

which fetches `ml-100k.zip` from GroupLens and extracts `u.data` to `data/interactions.txt`. The program reads the first three columns (user, movie, rating) of every line and ignores the rest, so the timestamp column does no harm. More in [`data/README.md`](data/README.md).

F. Maxwell Harper and Joseph A. Konstan. 2015. The MovieLens Datasets: History and Context. ACM Transactions on Interactive Intelligent Systems (TiiS) 5, 4, Article 19 (December 2015), 19 pages. <https://doi.org/10.1145/2827872>

## Build and run

You need a C11 compiler. With Make:

```sh
make                 # builds ./bipartite_recommender
make run             # downloads the data if needed, then starts the program
make test-sample     # starts the program on a tiny synthetic file
```

Or by hand, or with CMake:

```sh
gcc -std=c11 -Wall -Wextra -pedantic -o bipartite_recommender src/main.c
./bipartite_recommender data/interactions.txt
```

Without an argument the program reads `interactions.txt` in the current directory. Lines with a user outside 1 to 943, a movie outside 1 to 1682, a rating outside 1 to 5 or a repeated user-movie pair are skipped with a warning.

Choose a user from 1 to 943, then a method from 1 to 5. Enter `0` to change the user and `-1` to quit. Input is read line by line: anything that is not a whole number is rejected with a message and asked for again, and the end of the input quits the program. The program prints movie IDs; it has no movie titles. A method can return fewer than five movies, or none.

`MAX_RECOMMEND`, `MIN_COMMON`, `USER` and `STEPS` are defined at the top of `src/main.c`.

## Other ways to run it

To see all five methods for the user `USER` instead of the menu: comment out `callMenu();` in `main()` and uncomment the lines below the comment "UNCOMMENT BELOW 13".

To run the random walk, which starts at the user `USER` and takes `STEPS` steps along rating edges (an odd number of steps ends on a movie, an even number on a user): uncomment the call below the comment "UNCOMMENT BELOW 14".

Search for `UNCOMMENT` in the source for debugging prints (`*` marks a long output, `**` a very long one):

| Marker | Function | Prints |
|---|---|---|
| 1, 2* | `recommendMostLinked` | the number of links of the recommended movies / of all movies |
| 3 | `findMostCommonUser` | how many movies the two users share |
| 4 | `recommendFromCommonUser` | the other user's ratings of the recommended movies |
| 5**, 6**, 7*, 8*, 9*, 10 | `findClosestMovies` | the distance computations, the visited users, the distances to all users and to all unseen movies, and to the recommended movies |
| 11, 12 | `recommendFromSimilar` | the most similar user and the average rating difference, and that user's ratings of the recommended movies |

## Changes since December 2024

The code was corrected after its submission. The algorithms and data structures are the same, except for method 4, which is now a complete Dijkstra search. The history of this repository shows every change as its own commit. The corrections were found by running all 943 users through all five methods with AddressSanitizer and UndefinedBehaviorSanitizer and by comparing the results with independent calculations.

- Method 4 was a graph traversal that stopped after 700 iterations and read and wrote outside its arrays (`visited[-1]`, `visitedUsers[-1]`, `userMovies[-1]`) when it reached a dead end, which left some users without recommendations. It is now Dijkstra's algorithm on the whole graph: the distances are exact, and movies at the same distance are ordered by degree and then by movie ID instead of always favouring the lowest movie IDs.
- A non-numeric entry at the user prompt, or the end of the input, made the program loop forever.
- A method that found nothing (method 5 when no user shares `MIN_COMMON` movies) showed the previous method's results; now it says there is nothing to recommend.
- The random method seeded the generator on every call (the same list twice within a second) and could repeat a movie.
- The data file is now validated, its path can be given as an argument, and the memory is freed at exit.
- The random walk took one step too many and read a user outside the array.
- Movie 1682 was left out of every sort, and the code now compiles without warnings.

## Known limitations

- In method 4 the weight of an edge is the rating, so a path through low ratings is short: a close movie is not necessarily one the user will enjoy. Ratings are small whole numbers, so many movies share the same shortest distance (for 97% of the users the fifth nearest unseen movie is only 3 or 4 away on MovieLens 100K). Such ties are ordered by degree, so users with similar histories get similar lists: over all 943 users 122 different movies are recommended. Visiting the whole graph takes a fraction of a second per request.
- The program does not measure the accuracy of its recommendations.
- The numbers of users and movies are fixed (`MAX_USERS`, `MAX_MOVIES`) and match MovieLens 100K.
- Linked lists and bubble sort are slow for much larger catalogs.

## License

The source code is released under the [MIT License](LICENSE). The license does not cover the MovieLens data.
