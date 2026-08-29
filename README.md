# Milk Collection Route Optimizer

A C++ optimization engine for the Milk Collection Problem with Blending
(MCPwB). It builds a feasible route plan with a greedy heuristic and improves
the result through simulated annealing.

## Why it is interesting

- Models vehicle capacity, milk quality, demand, distance, and route profit.
- Combines constructive heuristics with neighborhood search.
- Includes benchmark instances for reproducible experiments.
- Reports objective quality and execution time for each solution.

## Build

```bash
make build
```

The project requires a C++17-compatible compiler.

## Run

Pass an instance file and the number of simulated-annealing iterations:

```bash
./MCPwB Instancias-20210102/a36.txt 1000
```

You can also build and run in one command:

```bash
make run INSTANCE=a36.txt ITERATIONS=1000
```

## Approach

1. Parse the fleet, collection nodes, milk types, demand, and profitability.
2. Construct a feasible initial solution using a greedy assignment strategy.
3. Generate neighboring route plans and repair infeasible candidates.
4. Accept improving moves and probabilistically accept alternatives according
   to the simulated-annealing temperature.
5. Return the best route plan found within the iteration budget.

## Benchmark snapshot

| Instance | Objective | Runtime |
| --- | ---: | ---: |
| a36 | 29118 | 2.5 s |
| a44 | 38557 | 2.5 s |
| a55 | 24459 | 2.5 s |
| a64 | 23454 | 2.5 s |
| a80 | 29661 | 2.5 s |
| eil22 | 15944.2 | 2.5 s |
| eil23 | 7207 | 2.5 s |
| eil30 | 7099.1 | 2.5 s |
| eil33 | 20352.8 | 2.5 s |
| eil76 | 91224.8 | 2.5 s |

Results depend on random initialization and the selected iteration budget.
