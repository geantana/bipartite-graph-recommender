#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#define USER 216            // The user for whom the recommendations will be made
#define MAX_USERS 943       // 943 users
#define MAX_MOVIES 1682     // 1682 movies
#define MAX_RECOMMEND 5     // Maximum number of recommendations
#define MIN_COMMON 20       // Number of minimum common movies required to say a pair is similar in recommendFromSimilar
#define STEPS 11            // Number of steps to take in a randomized walk (odd number to arrive a movie)

// Node for linked list to store movies watched by a user
typedef struct MovieNode {
    int movieID;
    int rating;
    int degree;         // To find the movie with most degrees
    int distance;       // To find the distance between a user pair
    struct MovieNode *next;
} MovieNode;

// User structure
typedef struct User {
    int userID;
    MovieNode *movies;
} User;

// Sorting a MovieNode array
// 1 - 2 are for sorting by degree ascending - descending
// 3 - 4 are for sorting by rating ascending - descending
// 5 - 6 are for sorting by movieID ascending - descending
// 7 - 8 are for sorting by distance ascending - descending
void sort(MovieNode arr[], int mode) {
    switch(mode) {
        // Ascending by degree
        case 1:
            for (int i = 0; i < MAX_MOVIES - 1; i++) {
                for (int j = 0; j < MAX_MOVIES - i - 1; j++) {
                    if (arr[j].degree > arr[j + 1].degree) {
                        // Swap the elements
                        MovieNode temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }
        break;
        // Descending by degree
        case 2:
            for (int i = 0; i < MAX_MOVIES - 1; i++) {
                for (int j = 0; j < MAX_MOVIES - i - 1; j++) {
                    if (arr[j].degree < arr[j + 1].degree) {
                        // Swap the elements
                        MovieNode temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }
        break;
        // Ascending by rating
        case 3:
            for (int i = 0; i < MAX_MOVIES - 1; i++) {
                for (int j = 0; j < MAX_MOVIES - i - 1; j++) {
                    if (arr[j].rating > arr[j + 1].rating) {
                        // Swap the elements
                        MovieNode temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }
        break;
        // Descending by rating
        case 4:
            for (int i = 0; i < MAX_MOVIES - 1; i++) {
                for (int j = 0; j < MAX_MOVIES - i - 1; j++) {
                    if (arr[j].rating < arr[j + 1].rating) {
                        // Swap the elements
                        MovieNode temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }
        break;
        // Ascending by movieID
        case 5:
            for (int i = 0; i < MAX_MOVIES - 1; i++) {
                for (int j = 0; j < MAX_MOVIES - i - 1; j++) {
                    if (arr[j].movieID > arr[j + 1].movieID) {
                        // Swap the elements
                        MovieNode temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }
        break;
        // Descending by movieID
        case 6:
            for (int i = 0; i < MAX_MOVIES - 1; i++) {
                for (int j = 0; j < MAX_MOVIES - i - 1; j++) {
                    if (arr[j].movieID < arr[j + 1].movieID) {
                        // Swap the elements
                        MovieNode temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }
        break;
        // Ascending by distance
        case 7:
            for (int i = 0; i < MAX_MOVIES - 1; i++) {
                for (int j = 0; j < MAX_MOVIES - i - 1; j++) {
                    if (arr[j].distance > arr[j + 1].distance) {
                        // Swap the elements
                        MovieNode temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }
        break;
        // Descending by distance
        case 8:
            for (int i = 0; i < MAX_MOVIES - 1; i++) {
                for (int j = 0; j < MAX_MOVIES - i - 1; j++) {
                    if (arr[j].distance < arr[j + 1].distance) {
                        // Swap the elements
                        MovieNode temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }
        break;
        default:
            printf("Invalid mode\n");
    }
}

// The graph itself
User users[MAX_USERS];
int totalUsers = 0;

// Add a movie to a user's watched list
void addMovie(int userID, int movieID, int rating) {
    MovieNode *newMovie = (MovieNode *)malloc(sizeof(MovieNode));
    newMovie->movieID = movieID;
    newMovie->rating = rating;
    newMovie->next = users[userID].movies;
    users[userID].movies = newMovie;
}

// Check if a user has watched a movie
int hasWatched(int userID, int movieID) {
    MovieNode *current = users[userID].movies;
    while (current) {
        if (current->movieID == movieID) return 1;  // watched the movie
        current = current->next;
    }
    return 0;                                       // didn't see the movie
}

// Initialize user array
void initializeUsers() {
    for (int i = 0; i < MAX_USERS; i++) {
        users[i].userID = i;
        users[i].movies = NULL;
    }
}

// Find how many movies the given user has watched (NOT USED)
// int numOfMovies(int userID) {
//     if(userID >= MAX_USERS) return 0;
//     int sum = 0;
//     MovieNode *current = users[userID].movies;
//     while (current) {
//         sum += 1;
//         current = current->next;
//     }
//     return sum;
// }

// Read data from a file (interactions.txt) and populate the graph
void loadInteractions(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int userID, movieID, rating;
        if (sscanf(line, "%d %d %d", &userID, &movieID, &rating) == 3) {
            addMovie(userID - 1, movieID, rating);  // Adjusting userID to 0-based index ! ! !
            if (userID > totalUsers) totalUsers = userID;   // Keep the total number of users in case it is needed
        }
    }

    fclose(file);
}

// Fills the given MovieNode array with the given user's movies and their ratings from that user
// If the size is bigger than the movies seen, the rest are filled with -1
void fillUserMovies(MovieNode arr[], int userID) {
    if(userID >= MAX_USERS) {
        printf("Invalid user ID\n");
        return;
    }
    MovieNode *current = users[userID].movies;
    // Iterate through the movies seen to fill the given array
    for (int i = 0; i < MAX_MOVIES + 1; i++) {
        if(current) {
            arr[i].movieID = current->movieID;
            arr[i].rating = current->rating;
            current = current->next;
        } else {
            arr[i].movieID = -1;
            arr[i].rating = -1;
        }
    }
}

// A global array to keep the movies that are going to be recommended,
// The list is written over with each recommendation algorithm
int recommendations[MAX_RECOMMEND] = {0};
// To reset all elements of the array recommendations to -1
void resetRecommendations() {
    for (int i = 0; i < MAX_RECOMMEND; i++) {
        recommendations[i] = -1;
    }
}

// INSTRUCTION #1
// A function to recommend the given user with required number of random movies
void recommendRandom(int userID) {
    srand(time(NULL));
    // Set the recommendations -1
    resetRecommendations();
    // Fill the array with desired number of movies
    for(int i = 0; i < MAX_RECOMMEND; ) {
        int movieID = rand() % MAX_MOVIES + 1;
        if (!hasWatched(userID, movieID)) {
            recommendations[i] = movieID;
            i++;
        }
    }
}

// INSTRUCTION #2
// A global array to keep the degrees of each movie
// Since the movies will be indexed with their own ids at first, the size of the list is MAX_MOVIES + 1
MovieNode movies[MAX_MOVIES + 1];
// Calculate movie degrees and fill in the movies array
void calculateDegrees() {
    // Set the list elements to -1
    for (int i = 0; i < MAX_MOVIES; i++) {
        movies[i].degree = 0;
        movies[i].movieID = -1;
    }
    // Iterate through all user movies to calculate degree
    for (int i = 0; i < MAX_USERS; i++) {
        MovieNode *current = users[i].movies;
        while (current) {
            movies[current->movieID].degree++;
            movies[current->movieID].movieID = current->movieID;
            current = current->next;
        }
    }
}

// Recommend most linked movie the user hadn't seen
void recommendMostLinked(int userID) {
    // Set the recommendations -1
    resetRecommendations();
    // To find the most linked movies, it sorts movies by their degrees in descending order (mode 2)
    sort(movies, 2);

    // Fill the recommendations with most linked movies
    for (int i = 0, j = 0; j < MAX_RECOMMEND && i < MAX_MOVIES + 1; i++) {
        if(!hasWatched(userID, movies[i].movieID)) {
            recommendations[j] = movies[i].movieID;
            // To see the degrees of the recommended movies, UNCOMMENT BELOW 1
            //printf("The movie %d has %d links.\n", movies[i].movieID, movies[i].degree);
            j++;
        }
    }

    // To see the degrees of all movies, UNCOMMENT BELOW 2
    // printf("\n HERE ARE ALL THE MOVIES AND THEIR DEGREES (DEBUG MODE)\n\n");
    // for (int i = 0; i < MAX_MOVIES + 1; i++) {
    //     if (movies[i].movieID != -1) {
    //         printf("The movie %d has %d links\n", movies[i].movieID, movies[i].degree);
    //     }
    // }
}

// INSTRUCTION #3
// Finds the user with whom the number of common movies is greatest
int findMostCommonUser(int targetUserID) {
    int maxCommon = 0, similarUserID = -1;
    for (int i = 0; i < MAX_USERS; i++) {
        if (i == targetUserID) continue;        // to evade comparing with themselves
        int commonMovies = 0;
        MovieNode *target = users[targetUserID].movies;
        while (target) {
            if (hasWatched(i, target->movieID)) commonMovies++;
            target = target->next;
        }
        if (commonMovies > maxCommon) {
            maxCommon = commonMovies;
            similarUserID = i;
        }
    }
    // To see the number of movies shared between the targetUserId and the user found, UNCOMMENT BELOW 3
    // printf(" The User %d shares %d movies with User %d\n", targetUserID + 1, maxCommon, similarUserID + 1);
    return similarUserID;
}

// Recommendations from the user with whom the most movies are shared
int recommendFromCommonUser(int targetUserID) {
    int similarUserID = findMostCommonUser(targetUserID);
    if (similarUserID == -1) return -1;   // No movies are shared with anyone
    // Set recommendations -1
    resetRecommendations();
    // Create a MovieNode array to keep user's (similarUserID's) movies
    MovieNode user_movies[MAX_MOVIES + 1];
    // Fill the array with their movies and their ratings
    fillUserMovies(user_movies, similarUserID);
    // Sort the user's (similarUserID's) movies based on the rating in descending order (mode 4)
    sort(user_movies, 4);
    // Fill the recommendations with most rated movies
    for(int i = 0, j = 0; j < MAX_RECOMMEND && i < MAX_MOVIES; i++) {
        if(!hasWatched(targetUserID, user_movies[i].movieID)) {
            recommendations[j] = user_movies[i].movieID;
            j++;
            // To see the most similar user's ratings for the recommended movies, UNCOMMENT BELOW 4
            // printf(" Movie %d is rated %d by user %d\n",
            //     user_movies[i].movieID, user_movies[i].rating, similarUserID + 1);
        }
    }
    return similarUserID;
}

// INSTRUCTION #4
// Get the rating of the user of a given movie
int getRating(int userID, int movieID) {
    MovieNode *current = users[userID].movies;
    while (current != NULL && current->movieID != movieID) {
        current = current->next;
    }
    if (current == NULL) {
        return INT_MIN;
    }
    return current->rating;
}

// A utility function to find the movie with minimum distance
int extractMin(MovieNode movies[], int size, int visited[]) {
    int minIndex = -1;
    int minDistance = INT_MAX;

    for (int i = 0; i < size; i++) {
        // Find the closest non-visited node
        if (!visited[movies[i].movieID] && movies[i].rating < minDistance && movies[i].movieID != -1) {
            minDistance = movies[i].rating;
            minIndex = i;
        }
    }
    // Return the index
    return minIndex;
}
// A utility function to find the user with minimum distance
int getMin(int userList[], int size, int visited[]) {
    int minIndex = -1;
    int minDistance = INT_MAX;
    for(int i = 0; i < size; i++) {
        if(!visited[i] && userList[i] < minDistance) {
            minDistance = userList[i];
            minIndex = i;
        }
    }
    return minIndex;
}
// See if visiting of all the movies is done
int isVisitingDone(int visited[], int size) {
    for (int i = 1; i < size; i++) {
        if (!visited[i]) {
            return 0; // Visiting not done
        }
    }
    return 1; // All movies visited
}
// Dijkstra Algorithm to recommend the closest weighted movies
void findClosestMovies(int userID) {
    MovieNode distances[MAX_MOVIES + 1]; // Movie distances
    int userDistances[MAX_USERS];
    int visited[MAX_MOVIES + 1] = {0}; // To mark visited movies
    int visitedUsers[MAX_USERS] = {0}; // To mark visited users

    // Initialize all movie distances to INT_MAX
    for(int i = 0; i < MAX_MOVIES + 1; i++) {
        distances[i].distance = INT_MAX;
        distances[i].movieID = -1;
    }

    // Initialize all user distances besides the userID to INT_MAX
    for(int i = 0; i < MAX_USERS; i++) {
        if(i == userID) {
            userDistances[i] = 0;
            continue;
        }
        userDistances[i] = INT_MAX;
    }


    int currentUser = userID;
    int currentMovie = -1;
    int mode = 0;               // To change modes between a user process and a movie process
    visitedUsers[currentUser] = 1;
    // !isVisitingDone(visited, MAX_MOVIES + 1);
    int iterationNum = 700;     // Number of iterations in graph traversal
    while(iterationNum != 0) {
        if(mode == 0) {
            // If we are currently at a user
            MovieNode userMovies[MAX_MOVIES + 1];
            fillUserMovies(userMovies, currentUser);
            for(int i = 0; i < MAX_MOVIES + 1; i++) {
                if(userMovies[i].movieID != -1) {
                    int movieID = userMovies[i].movieID;
                    int distance = userMovies[i].rating + userDistances[currentUser];
                    // To see the accumulated distance up until a certain user and thus the total distance to
                    // users movies, UNCOMMENT BELOW 5 **
                    // printf("\nFor Movie: %d\nRating: %d\nDistance accumulated: %d\nThus the total distance: %d\n",
                    //     movieID, userMovies[i].rating, userDistances[currentUser], distance);
                    if(distance < distances[movieID].distance) {
                        distances[movieID].distance = distance;
                        distances[movieID].movieID = movieID;
                        // To see the IDs and the total distances of the movies that are being updated, UNCOMMENT BELOW 6
                        // printf("Movie ID: %d, Distance: %d\n", distances[movieID].movieID, distances[movieID].distance);
                    }
                }
            }
            int minIndex = extractMin(userMovies, MAX_USERS, visited);
            int movieID = userMovies[minIndex].movieID;
            visited[movieID] = 1;
            currentMovie = movieID;
            mode = 1;
        } else {
            // If we are currently at a movie
            for(int i = 0; i < MAX_USERS; i++) {
                if(hasWatched(i, currentMovie) && !visitedUsers[i]) {
                    int rating = getRating(i, currentMovie);
                    int currentDistance = distances[currentMovie].distance + rating;
                    if (currentDistance < userDistances[i]) {
                        userDistances[i] = currentDistance;
                    }
                }
            }
            int minIndex = getMin(userDistances, MAX_USERS, visitedUsers);
            visitedUsers[minIndex] = 1;
            // To see the user changes in the traversal, UNCOMMENT BELOW 7
            // printf("\nChanging user from %d to User %d\n", currentUser + 1, minIndex + 1);
            currentUser = minIndex;
            mode = 0;
        }
        iterationNum--;
    }

    // To see all user distances to source user, UNCOMMENT BELOW 8
    // for(int i = 0 ; i < MAX_USERS; i++) {
    //     printf("Distance from User %d to User %d is %d\n", userID + 1, i + 1, userDistances[i]);
    // }

    // Sort by distance in ascending order
    sort(distances, 7);
    // To see all calculated distances to movies that the target user did not watch, UNCOMMENT BELOW 9
    // for (int i = 0; i < MAX_MOVIES + 1; i++) {
    //     if(!hasWatched(userID, distances[i].movieID) && distances[i].distance < INT_MAX) {
    //         printf("The distance from User %d to Movie %d is %d\n",
    //             userID + 1, distances[i].movieID, distances[i].distance);
    //     }
    // }

    for(int i = 0, j = 0; j < MAX_RECOMMEND && i < MAX_MOVIES; i++) {
        if(!hasWatched(userID, distances[i].movieID)) {
            recommendations[j] = distances[i].movieID;
            j++;
            // To see the distances of the movies that are being recommended, UNCOMMENT BELOW 10
            // printf(" The distance of the movie %d from User %d is %d.\n",
            //     distances[i].movieID, userID + 1, distances[i].distance);
        }
    }

}

// INSTRUCTION #5
double calculateSimilarity(int userA, int userB) {
    int commonMovies = 0, diffSum = 0;

    MovieNode *moviesA = users[userA].movies;
    while (moviesA) {
        MovieNode *moviesB = users[userB].movies;       // start from first movie of B
        while (moviesB) {
            if (moviesA->movieID == moviesB->movieID) {
                int diff = abs(moviesA->rating - moviesB->rating);
                diffSum += diff;
                commonMovies++;
                break;
            }
            moviesB = moviesB->next;
        }
        moviesA = moviesA->next;
    }
    // If the number of common movies is small (less than MIN_COMMON) than the comparison doesn't mean much
    if (commonMovies < MIN_COMMON) return INFINITY;

    return (double)diffSum / commonMovies;
}

int recommendFromSimilar(int targetUserID) {
    int mostSimilarUser = -1;
    double minDifference = INFINITY;
    // find the closest user (with min distance)
    for (int i = 0; i < MAX_USERS; i++) {
        if (i != targetUserID) {
            double difference = calculateSimilarity(targetUserID, i);
            if (difference < minDifference) {
                minDifference = difference;
                mostSimilarUser = i;
            }
        }
    }
    // To see the most similar user and the average difference in rating per movie, UNCOMMENT BELOW 11
    // printf("\n User %d and User %d have at least %d common movies.\n", targetUserID + 1, mostSimilarUser + 1, MIN_COMMON);
    // printf(" The average of rating difference is %.3f per common movie\n\n", minDifference);
    if (mostSimilarUser == -1) return -1;

    MovieNode user_movies[MAX_MOVIES + 1];
    fillUserMovies(user_movies, mostSimilarUser);
    // Sort mostSimilarUser's movies by rating in descending order
    sort(user_movies, 4);
    // Fill the recommendations with most rated movies
    for(int i = 0, j = 0; j < MAX_RECOMMEND && i < MAX_MOVIES; i++) {
        if(!hasWatched(targetUserID, user_movies[i].movieID)) {
            recommendations[j] = user_movies[i].movieID;
            j++;
            // To see the most similar user's ratings for the recommended movies, UNCOMMENT BELOW 12
            // printf(" Movie %d is rated %d by user %d\n",
            //     user_movies[i].movieID, user_movies[i].rating, mostSimilarUser + 1);
        }
    }
    return mostSimilarUser;
}

// A function which takes required number of randomized steps from the user given
// An even step number means arriving to a user
// An odd step number means arriving to a movie
int goToRandom(int targetUserID, int step) {
    srand(time(NULL));
    int randomMovie = 0, randomUser = 0;
    int rating = 0, ratingSum = 0;
    int currentUser = targetUserID;
    int mode = 0, i = 0; printf("\n");
    for(i = 0; i < step;) {
        if(mode == 0) {
            randomMovie = rand() % MAX_MOVIES + 1;
            if(hasWatched(currentUser, randomMovie)) {
                MovieNode *current = users[currentUser].movies;
                while(current) {
                    if(current->movieID == randomMovie) {
                        rating = current->rating;
                        ratingSum += rating;
                        break;
                    }
                    current = current->next;
                }
                printf(" We travel from User %d to movie %d through %d weighted distance.\n",
                    currentUser + 1, randomMovie, rating);
                printf(" Total weighted distance is thus %d.\n", ratingSum);
                mode = 1; i++;
            }
        }
        if (mode == 1) {
            randomUser = rand() % MAX_USERS + 1;
            if(hasWatched(randomUser, randomMovie)) {
                currentUser = randomUser;
                MovieNode *current = users[currentUser].movies;
                while(current) {
                    if(current->movieID == randomMovie) {
                        rating = current->rating;
                        ratingSum += rating;
                        break;
                    }
                    current = current->next;
                }
                printf(" We travel from movie %d to User %d through %d weighted distance.\n",
                    randomMovie, currentUser + 1, rating);
                printf(" Total weighted distance is thus %d.\n", ratingSum);
                mode = 0; i++;
            }
        }
    }
    if(i % 2 != 0) {
        printf("\n Finally, the random walk distance from User %d to movie %d in %d steps is %d.",
                targetUserID + 1, randomMovie, i, ratingSum);
    } else {
        printf("\n Since the desired step number was even, we arrive to a user, User %d.\n",
            currentUser + 1);
        printf(" Finally, the random walk distance from User %d to User %d in %d steps is %d.",
                targetUserID + 1, currentUser + 1, i, ratingSum);
    }
    return -1;
}

void printRecommendations() {
    printf("\n");
    for (int i = 0; i < MAX_RECOMMEND; i++) {
        if (recommendations[i] != -1) {
            printf(" - recommendation number %d: %d\n", i + 1, recommendations[i]);
        } else {
            printf("\nWe can only recommend as many, </3\n");
            break;
        }
    }
}
// Function to recommend user with the desired algorithm:
// mode 1 : RANDOM RECOMMENDATIONS FOR SELECTED USER
// mode 2 : MOST LINKED RECOMMENDATIONS FOR SELECTED USER
// mode 3 : RECOMMENDATIONS FROM MOST SIMILAR USER (BASED ON COMMON MOVIE NUMBER)
// mode 4 : RECOMMENDATIONS FROM THE USER WITH CLOSEST WEIGHTED DISTANCE
// mode 5 : RECOMMENDATIONS FROM THE USER WITH WHOM THE TASTE IS MOST SIMILAR
void doRecommend(int user, int mode) {
    switch (mode) {
        case 1:
            // RANDOM RECOMMENDATIONS FOR SELECTED USER
            printf("\n\n*********** RANDOM RECOMMENDATIONS ***********\n\n");
            recommendRandom(user - 1);
            printf(" Random Recommendation for user %d:\n", user);
            printRecommendations();
        break;
        case 2:
            // MOST LINKED RECOMMENDATIONS FOR SELECTED USER
            printf("\n\n*********** MOST LINKED RECOMMENDATIONS ***********\n\n");
            calculateDegrees(); // needed for recommending most linked movie
            recommendMostLinked(user - 1);
            printf(" Most Linked Recommendation for user %d:\n", user);
            printRecommendations();
        break;
        case 3:
            // RECOMMENDATIONS FROM MOST SIMILAR USER (BASED ON COMMON MOVIE NUMBER)
            printf("\n\n*********** FROM SIMILAR USER RECOMMENDATIONS ***********");
            printf("\n***********  (Based on common movie number)   ***********\n\n");
            int mostCommoner = recommendFromCommonUser(user - 1);
            printf(" The User with whom the User %d shares most movies is User %d\n\n", user, mostCommoner + 1);
            printf(" Highest rated movies from the USER %d : \n", mostCommoner + 1);
            printRecommendations();
        break;
        case 4:
            // RECOMMENDATIONS FROM THE USER WITH CLOSEST WEIGHTED DISTANCE
            printf("\n\n*********** FROM CLOSEST USER RECOMMENDATIONS ***********\n\n");
            findClosestMovies(user-1);
            printf(" The closest movies by weighted distance to the User %d\n", user);
            printRecommendations();
        break;
        case 5:
            // RECOMMENDATIONS FROM THE USER WITH WHOM THE TASTE IS MOST SIMILAR
            printf("\n\n*********** FROM SIMILAR USER RECOMMENDATIONS ***********");
            printf("\n***********  (Based on common movie ratings)  ***********\n\n");
            int mostSimilar = recommendFromSimilar(user - 1);
            printf(" The most similar user to User %d based on movie ratings is User %d\n\n", user, mostSimilar + 1);
            printf(" Highest rated movies from the USER %d : \n", mostSimilar + 1);
            printRecommendations();
        break;
        default:
            printf("\n!!The input is invalid for the printEverything function call!!\n\n");
    }
}

// Basic menu utility functions
int getUserID() {
    int user = -1;
    printf("\n Please enter the user ID (1 - %d) for whom you would like to have recommendations: ", MAX_USERS);
    scanf("%d", &user);
    while (user < 1 || user > MAX_USERS) {
        printf("\n That's not a valid user ID! \n\n");
        printf("Please enter a valid user ID (1 - %d): ", MAX_USERS);
        scanf("%d", &user);
    }
    return user;
}
int getMode() {
    int mode = -1;
    printf("\n Please enter your choice: ");
    scanf("%d", &mode);
    while (mode < -1 || mode > 5) {
        printf("\n That's not a valid choice! \n\n");
        printf("Please enter a valid one: ");
        scanf("%d", &mode);
    }
    return mode;
}
void printChoices() {
    printf(" -1 : QUIT\n");
    printf("  0 : CHANGE USER ID\n");
    printf("  1 : RANDOM RECOMMENDATIONS FOR SELECTED USER\n");
    printf("  2 : MOST LINKED RECOMMENDATIONS FOR SELECTED USER\n");
    printf("  3 : RECOMMENDATIONS FROM MOST SIMILAR USER (BASED ON COMMON MOVIE NUMBER)\n");
    printf("  4 : RECOMMENDATIONS FROM THE USER WITH CLOSEST WEIGHTED DISTANCE\n");
    printf("  5 : RECOMMENDATIONS FROM THE USER WITH WHOM THE TASTE IS MOST SIMILAR\n");
}
void callMenu() {
    printf("\n Hello Dear User,\n");
    printf(" We offer a few recommendation algorithms for you,\n");
    printf(" Before diving into the algorithms, lets pick a user");
    int mode = 0, user = 0;
    while (mode != -1) {
        switch (mode) {
            case 0:
                user = getUserID();
                printf("\n OK!, now we can see the algorithms:\n\n");
                printChoices();
                mode = getMode();
            break;
            case 50:
                printf("\n");
                printChoices();
                mode = getMode();
            break;
            default:
                doRecommend(user, mode);
                mode = 50;
        }
    }
    printf("\nGOODBYE!!\n");
}

int main() {

    // PREPARATION BEFORE RECOMMENDATIONS
    initializeUsers();
    loadInteractions("interactions.txt");

    // Call menu to use an interactive interface to see recommendations
    callMenu();

    // TO SEE ALL ALGORITHMS FOR A PREDETERMINED USER WITHOUT A MENU, UNCOMMENT BELOW 13
    // // RANDOM RECOMMENDATIONS FOR SELECTED USER
    // doRecommend(USER, 1);
    //
    // // MOST LINKED RECOMMENDATIONS FOR SELECTED USER
    // doRecommend(USER, 2);
    //
    // // RECOMMENDATIONS FROM MOST SIMILAR USER (BASED ON COMMON MOVIE NUMBER)
    // doRecommend(USER, 3);
    //
    // // RECOMMENDATIONS FROM THE USER WITH CLOSEST WEIGHTED DISTANCE
    // doRecommend(USER, 4);
    //
    // // RECOMMENDATIONS FROM THE USER WITH WHOM THE TASTE IS MOST SIMILAR
    // doRecommend(USER, 5);

    // TO SEE THE ADDITIONAL RANDOM WALK ALGORITHM, UNCOMMENT BELOW 14
    // // ADDITIONAL RANDOM WALK PATHS OF DESIRED STEPS FROM GIVEN USER
    // goToRandom(USER - 1, STEPS);

    printf("\n\n");
    return 0;
}
