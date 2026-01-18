# Mini Plague Simulation
Simple plague simulation on a directed graph.

- Population: N humans
- Each human has directed connections
- Infection spreads via outgoing and incoming edges
- Discrete tick-based simulation
- Written in C++ for quick modeling and completion

This project was intentionally kept small and simple.
```bash
./plague_simulation -totalTick 50 -population 1000 -minEdgeCountPerHuman 4 -maxEdgeCountPerHuman 8 -totalInfectedInStart 1 -plagueInfectSTR 60 -plagueTickForBeCured 7
