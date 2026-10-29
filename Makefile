CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2
TARGET = bipartite_recommender
SRC = src/main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o

# Download official MovieLens 100k dataset from GroupLens Research
download-data:
	@bash scripts/download_data.sh

# Run with full dataset (downloads automatically if missing)
run: $(TARGET)
	@if [ ! -f data/interactions.txt ]; then \
		echo "Dataset not found. Downloading MovieLens 100K from official GroupLens repository..."; \
		bash scripts/download_data.sh; \
	fi
	./$(TARGET) data/interactions.txt

# Quick dry-run test using synthetic sample data
test-sample: $(TARGET)
	./$(TARGET) data/sample_interactions.txt

.PHONY: all clean download-data run test-sample
