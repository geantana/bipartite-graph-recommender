# Dataset Information & Licensing Terms

## Origin & Licensing Notice
This project uses interaction data from the **MovieLens 100K** dataset, collected and provided by **GroupLens Research** at the University of Minnesota.

* **Dataset Home:** [https://grouplens.org/datasets/movielens/100k/](https://grouplens.org/datasets/movielens/100k/)
* **Official Download URL:** `https://files.grouplens.org/datasets/movielens/ml-100k.zip`

### Terms of Use & Redistribution Restrictions
According to the GroupLens dataset terms of use:
1. **Redistribution:** *"The user may not redistribute the data without separate permission."*
   In strict compliance with this rule, the full raw dataset file (`interactions.txt`) is **not** bundled or committed into this Git repository.
2. **Non-Commercial Research:** The dataset is provided strictly for non-commercial research and educational exploration.
3. **Citation:** Any research or project using this dataset must provide attribution to GroupLens Research.

---

## How to Obtain the Dataset

You can download the full dataset with a single command:

```bash
# From the repository root:
make download-data
```

Or manually:
```bash
./scripts/download_data.sh
```

This will download `ml-100k.zip` directly from GroupLens servers and extract `ml-100k/u.data` to `data/interactions.txt`.

For quick offline verification and CI testing without downloading the full dataset, a synthetic sample file is provided at `data/sample_interactions.txt`.

---

## Academic Citation

If you use this dataset in academic work, cite the foundational paper:

```bibtex
@article{Harper2015MovieLens,
  author    = {Harper, F. Maxwell and Konstan, Joseph A.},
  title     = {The MovieLens Datasets: History and Context},
  journal   = {ACM Transactions on Interactive Intelligent Systems (TiiS)},
  volume    = {5},
  number    = {4},
  pages     = {19:1--19:19},
  year      = {2015},
  publisher = {ACM},
  doi       = {10.1145/2827872}
}
```
