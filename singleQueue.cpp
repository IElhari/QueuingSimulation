/*
Name: Ibrahim Adam Elhari
Option 1: Single queue for all patrons
*/

#include <iostream>
#include <queue>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>

using namespace std;

// Renaming differnt forms of int variable to make lifer easier
int typedef timeType;
int typedef durationType;
int typedef idType;

// Inputs of simulation:

durationType simulationDuration, arrivalRate, serviceRate;

// Generates random busy time
int generateBusyTime(durationType arrivalRate)
{
    // Should be outside of function, passed in.
    random_device rd;
    mt19937 gen(rd());

    double mean = 5.0 * arrivalRate;
    double stddev = 1.0; // Standard Deviation

    // Creates a normal distribution with the specified mean and standard deviation
    normal_distribution<double> distribution(mean, stddev);

    // Generate a random number from the distribution
    int randomValue = distribution(gen);
    return randomValue + 1;
}
// Create a random number generator

struct person
{
    timeType arrival;
    idType ID;
};

queue<person> typedef customerQueueType;
customerQueueType CQ;

struct serviceStation
{
    durationType busytime; // remainaing time of service
    person customer;
};

// serviceStation SS1, SS2, SS3, SS4;
const int numberOfServiceStations = 5;
serviceStation ss[numberOfServiceStations];
double avgbusytime[numberOfServiceStations];

// customerQueueType serviceStationQueues[numberOfServiceStations]
int customerCounter = 0;

int averageWaitingTime;
int maxWaitingTime = 0;

static idType idCounter = 1010;

timeType timer = 0;

durationType maxDuration;

bool ongoingSim(timeType timer)
{
    return timer <= maxDuration;
}

bool customerPullUp(int timer)
{
    return timer % arrivalRate == 0;
}

int main()
{
    cout << "Enter how long you want the simulation to run (minutes): ";

    cin >> maxDuration;

    cout << "Enter average arrival rates of customers (minutes): ";

    cin >> arrivalRate;

    int availableStations = 0;

    while (ongoingSim(timer))
    {
        if (customerPullUp(timer))
        {
            person customer;

            customer.arrival = timer;
            customer.ID = idCounter;

            idCounter++;
            CQ.push(customer);
            customerCounter++;
        }

        for (int i = 0; i < numberOfServiceStations; i++)
        {
            if (ss[i].busytime >= 1)
            {
                ss[i].busytime--;
            }
        }

        if (!CQ.empty())
        {
            for (int i = 0; i < numberOfServiceStations; i++)
            {
                if (ss[i].busytime == 0)
                {
                    person temp = CQ.front();

                    CQ.pop();

                    ss[i].customer.ID = temp.ID;
                    ss[i].customer.arrival = temp.arrival;

                    averageWaitingTime += timer - temp.arrival;
                    maxWaitingTime = max(maxWaitingTime, timer - temp.arrival);

                    ss[i].busytime = generateBusyTime(arrivalRate);
                    avgbusytime[i] += ss[i].busytime;

                    availableStations--;
                }
                if (CQ.empty())
                    break;
            }
        }
        timer++;
    }

    while (!CQ.empty())
    {
        for (int i = 0; i < numberOfServiceStations; i++)
        {
            if (ss[i].busytime >= 1)
            {
                ss[i].busytime--;
            }
            if (ss[i].busytime == 0)
            {
                availableStations++;
            }
        }

        if (!CQ.empty())
        {
            for (int i = 0; i < numberOfServiceStations; i++)
            {
                if (ss[i].busytime == 0)
                {
                    person temp = CQ.front();

                    CQ.pop();

                    ss[i].customer.ID = temp.ID;
                    ss[i].customer.arrival = temp.arrival;

                    averageWaitingTime += timer - temp.arrival;
                    maxWaitingTime = max(maxWaitingTime, timer - temp.arrival);

                    ss[i].busytime = generateBusyTime(arrivalRate);
                    avgbusytime[i] += ss[i].busytime;

                    availableStations--;
                }
                if (CQ.empty())
                    break;
            }
        }
        timer++;
    }

    double avgWaitingTime = averageWaitingTime / customerCounter;

    int maxI = 0;
    for (int i = 0; i < numberOfServiceStations; i++)
    {
        maxI = max(maxI, ss[i].busytime);
    }
    timer = timer + maxI;

    for (int i = 0; i < numberOfServiceStations; i++)
    {
        avgbusytime[i] = avgbusytime[i] / double(timer) * 100;
        cout << "avg busy time for service station " << i + 1 << " is " << avgbusytime[i] << "%" << endl;
    }

    cout << "Total customers: " << customerCounter << endl;
    cout << "The average customer waiting time was " << avgWaitingTime << endl;
    cout << "The maximum cusotmer waiting time was  " << maxWaitingTime << endl;
    cout << "Simulation ran for " << timer << " minutes." << endl;
    return 0;
}
