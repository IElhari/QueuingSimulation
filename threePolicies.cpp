/*
Name: Ibrahim Adam Elhari
Class: CS 610
Assignment: Module 5 Programming Assignment: Option 2:
    Each service station has its own queue and passenger are dispatched
    to a queue accoding to one of 3 policies:
        1: Round Robin
        2: arriving passengers placed in shortest queue
        3: arriving passengers placed in random queue
*/

#include <iostream>
#include <queue>
#include <string>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cstring>

using namespace std;

int typedef timeType;
int typedef durationType;
int typedef idType;

// Inputs of simulation:

durationType simulationDuration, arrivalRate, serviceRate;

int generateBusyTime(durationType arrivalRate)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    double mean = 10.0 * arrivalRate;
    double stddev = 1.0;

    std::normal_distribution<double> distribution(mean, stddev);

    int randomValue = distribution(gen);

    return randomValue + 1;
}

struct person
{
    timeType arrival;
    idType ID;
};

const int numberOfServiceStations = 5;

queue<person> typedef customerQueueType;
customerQueueType serviceStationQueues[numberOfServiceStations];

struct serviceStation
{
    durationType busytime; // remainaing time of service
    person customer;
};

serviceStation ss[numberOfServiceStations];
double avgbusytime[numberOfServiceStations];

int customerCounter[numberOfServiceStations];

int averageWaitingTime[numberOfServiceStations];
int maxWaitingTime[numberOfServiceStations];

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

    int option;
    cout << "What type of scheduling do you want?" << endl;
    cout << "1: Round Robin" << endl;
    cout << "2: Shortest Queue" << endl;
    cout << "3: Random Queue" << endl;
    cin >> option;

    int maxLength[numberOfServiceStations];
    memset(maxLength, 0, sizeof(maxLength));
    int queueCounter = 0;
    int queueNumber = 0;
    int minQueueSize = 1000;
    while (ongoingSim(timer))
    {

        if (customerPullUp(timer))
        {
            person customer;

            customer.arrival = timer;
            customer.ID = idCounter;
            idCounter++;
            minQueueSize = 1000;

            switch (option)
            {
            // Round Robin
            case 1:
                serviceStationQueues[queueCounter].push(customer);

                maxLength[queueCounter] = max(maxLength[queueCounter], int(serviceStationQueues[queueCounter].size()));
                customerCounter[queueCounter]++;

                queueCounter = (queueCounter + 1) % numberOfServiceStations;
                break;

            // Shortest Queue
            case 2:
                // before pushing, check which queue has minumum size, then push it
                for (int i = 0; i < numberOfServiceStations; i++)
                {
                    if (minQueueSize > int(serviceStationQueues[i].size()))
                    {
                        minQueueSize = int(serviceStationQueues[i].size());
                        queueNumber = i;
                    }
                }
                serviceStationQueues[queueNumber].push(customer);
                customerCounter[queueNumber]++;
                maxLength[queueNumber] = max(maxLength[queueNumber], int(serviceStationQueues[queueNumber].size()));
                break;
            // Random Queue
            case 3:
                int queueNumberRandom = rand() % numberOfServiceStations;

                serviceStationQueues[queueNumberRandom].push(customer);
                customerCounter[queueNumberRandom]++;
                maxLength[queueNumberRandom] = max(maxLength[queueNumberRandom], int(serviceStationQueues[queueNumberRandom].size()));
                break;
            }
        }

        for (int i = 0; i < numberOfServiceStations; i++)
        {
            if (ss[i].busytime >= 1)
            {
                ss[i].busytime--;
            }
        }
        // cout << "There are  " << CQ.size() << " customers in line" << endl;
        for (int i = 0; i < numberOfServiceStations; i++)
        {
            if (ss[i].busytime == 0 && serviceStationQueues[i].size() > 0)
            {
                person temp = serviceStationQueues[i].front();

                serviceStationQueues[i].pop();

                // customerCounter++;

                ss[i].customer.ID = temp.ID;
                ss[i].customer.arrival = temp.arrival;

                averageWaitingTime[i] += timer - temp.arrival;
                maxWaitingTime[i] = max(maxWaitingTime[i], timer - temp.arrival);

                ss[i].busytime = generateBusyTime(arrivalRate);
                avgbusytime[i] += ss[i].busytime;
            }
        }
        timer++;
    }
    // END OF WHILE LOOP

    int leftOver = 0;
    for (int j = 0; j < numberOfServiceStations; j++)
    {
        leftOver += serviceStationQueues[j].size();
    }
    while (leftOver != 0)
    {
        for (int i = 0; i < numberOfServiceStations; i++)
        {
            if (ss[i].busytime >= 1)
            {
                ss[i].busytime--;
            }
            if (ss[i].busytime == 0 && serviceStationQueues[i].size() > 0)
            {
                person temp = serviceStationQueues[i].front();
                serviceStationQueues[i].pop();
                leftOver--;
                ss[i].customer.ID = temp.ID;
                ss[i].customer.arrival = temp.arrival;

                averageWaitingTime[i] += timer - temp.arrival;
                maxWaitingTime[i] = max(maxWaitingTime[i], timer - temp.arrival);

                ss[i].busytime = generateBusyTime(arrivalRate);
                avgbusytime[i] += ss[i].busytime;
            }
        }

        timer++;
    }

    int maxI = 0;
    int totalCustomers = 0;
    for (int i = 0; i < numberOfServiceStations; i++)
    {
        maxI = max(maxI, ss[i].busytime);
        totalCustomers += customerCounter[i];
    }
    timer = timer + maxI;

    for (int i = 0; i < numberOfServiceStations; i++)
    {
        avgbusytime[i] = avgbusytime[i] / double(timer) * 100;
        cout << "avg busy time for service station " << i + 1 << " is " << avgbusytime[i] << "%" << endl;

        cout << "Max Lenght for queue " << i + 1 << " is " << maxLength[i] << endl;

        averageWaitingTime[i] = averageWaitingTime[i] / customerCounter[i];
        cout << "The average customer waiting time for queue " << i + 1 << " is " << averageWaitingTime[i] << endl;

        cout << "The maximum cusotmer waiting time for queue " << i + 1 << " is " << maxWaitingTime[i] << endl;
    }

    cout << "Total cusotmers: " << totalCustomers << endl;
    cout << "Simulation ran for " << timer << " minutes." << endl;

    return 0;
}
