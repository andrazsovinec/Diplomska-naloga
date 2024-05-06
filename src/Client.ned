package src;

simple Client
{   
    parameters:
        double messageInterval @unit(s) = default(1.0s);
        bool expDistribution = default(true);
        volatile double simulationEndTime @unit(s)= default(60s);

    gates:
        output out;
}
