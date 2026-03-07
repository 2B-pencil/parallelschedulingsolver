# ParallelSchedulingSolver

Research code for parallel scheduling problems.

## Compilation

```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel && cmake --install build --config Release --prefix install
```

Setup Python environment to use the Python scripts:
```shell
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

Download data files:
```shell
python scripts/download_data.py
```

Run
```shell
./install/bin/parallelschedulingsolver  --verbosity-level 1  --input ./data/alfieri2021/inst_10_1_1.dat --format alfieri2021  --algorithm greedy-longest-processing-time  --certificate solution.json
```

Visualize solution:
```shell
python scripts/visualize.py solution.json
```

## Implemented algorithms
