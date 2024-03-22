#include "App.h"
#include "System.h"
#include <iostream>


void basicServiceMetrics(const System& system);

void MaxFlowMenu(const System &system);

void waterSupplySufficency();

void reliabilityMenu(const System& system);

void ResevoirsMenu(const System &system);

void StationsMenu(const System &system);

void PipesMenu(const System &system);

int mainMenu(){
    cout << "Loading...";

    System system;
    system.readAndParseNodes();
    system.readAndParseEdges();
    system.initialize();

    string choice;
    bool exitMenu = false;
    while (!exitMenu) {
        cout << "\n-----------------------------\n";
        cout << "   Welcome to Main Menu       \n";
        cout << "-----------------------------\n";
        cout << "Enter the number of the option that suits your needs:\n";
        cout << "1. Check the system's Service Metrics \n";
        cout << "2. Test the system's Reliablity and Sensivity to Failures\n";
        cout << "e. Exit\n";
        cout << "-----------------------------\n";
        cout << "Your choice: ";
        cin >> choice;

        if (choice.length() != 1) {
            choice = "0";
        }

        switch (choice[0]) {
            case '1':
                basicServiceMetrics(system);
                break;
            case '2':
                reliabilityMenu(system);
                break;
            case 'e':
                cout << "Exiting menu system...\n";
                exitMenu = true;
                break;
            default:
                cout << "Invalid input. Please choose a valid option.\n";
        }
    }

    return 0;
}



void reliabilityMenu(const System& system) {
    System sistema = system;
    string choice;
    bool back = false;

    while (!back) {
        cout << "\n----------------------------------\n";
        cout << "      Service Metrics Menu      \n";
        cout << "----------------------------------\n";
        cout << "Choose an option:\n";
        cout << "1. Check the impact of one or multiple Reservoirs being out of commission \n";
        cout << "2. Check the impact of temporarily removing one or multiple Pumping Stations\n";
        cout << "3. Check the impact of one or multiple Pipeline failures \n";
        cout << "b. Go back\n";
        cout << "----------------------------------\n";
        cout << "Your choice: " << endl;

        cin >> choice;

        if (choice.length() != 1){
            choice = "h";
        }

        switch (choice[0]) {
            case '1':
                ResevoirsMenu(system);
                break;
            case '2':
                StationsMenu(system);
                break;
            case '3':
                PipesMenu(system);
                break;
            case 'b':
                back = true;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

}

void PipesMenu(const System &system) {

    System sistema = system;
    string choice;
    bool back = false;

    while (!back) {
        cout << "\n----------------------------------\n";
        cout << "      Pipeline Removal Menu      \n";
        cout << "----------------------------------\n";
        cout << "Choose an option:\n";
        cout << "1. Check the impact of removing a chosen Pipeline \n";
        cout << "2. Check the impact of removing a chosen set of Pipelines, one at a time \n";
        cout << "3. Check the impact of removing every Pipeline, one at a time (source-numerical order) \n";
        cout << "b. Go back\n";
        cout << "----------------------------------\n";
        cout << "Your choice: " << endl;

        cin >> choice;

        if (choice.length() != 1){
            choice = "h";
        }

        switch (choice[0]) {
            case '1':
                //sistema.removePipe()
                break;
            case '2':
                StationsMenu(system);
                break;
            case '3':
                PipesMenu(system);
                break;
            case 'b':
                back = true;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

}

void StationsMenu(const System &system) {

    System sistema = system;
    string choice;
    bool back = false;

    while (!back) {
        cout << "\n----------------------------------\n";
        cout << "      Station Removal Menu      \n";
        cout << "----------------------------------\n";
        cout << "Choose an option:\n";
        cout << "1. Check the impact of removing a chosen Station \n";
        cout << "2. Check the impact of removing a chosen set of Stations, one at a time \n";
        cout << "3. Check the impact of removing every Station, one at a time (numerical order) \n";
        cout << "b. Go back\n";
        cout << "----------------------------------\n";
        cout << "Your choice: " << endl;

        cin >> choice;

        if (choice.length() != 1){
            choice = "h";
        }

        switch (choice[0]) {
            case '1':
                //sistema.removeStation()
                break;
            case '2':
                StationsMenu(system);
                break;
            case '3':
                PipesMenu(system);
                break;
            case 'b':
                back = true;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

}

void ResevoirsMenu(const System &system) {

    System sistema = system;
    string choice;
    bool back = false;

    while (!back) {
        cout << "\n----------------------------------\n";
        cout << "      Reservoir Removal Menu      \n";
        cout << "----------------------------------\n";
        cout << "Choose an option:\n";
        cout << "1. Check the impact of removing a chosen Reservoir \n";
        cout << "2. Check the impact of removing a chosen set of Reservoirs, one at a time \n";
        cout << "3. Check the impact of removing every Reservoir, one at a time (numerical order) \n";
        cout << "b. Go back\n";
        cout << "----------------------------------\n";
        cout << "Your choice: " << endl;

        cin >> choice;

        if (choice.length() != 1){
            choice = "h";
        }

        vector<string> reservoirIDs;
        string code;
        stringstream ss;
        string input;

        switch (choice[0]) {
            case '1':
                cout << "Enter comma-separated values for Reservoir IDs in the desired order: " << endl;
                cin >> input;
                sistema.removeReservoir(input);
                break;
            case '2':
                cout << "Enter comma-separated values for Reservoir IDs in the desired order: " << endl;
                cin >> input;

                ss.str(input);

                while (getline(ss, code, ',')) {
                    reservoirIDs.push_back(code);
                }

                sistema.removeReservoirVector(reservoirIDs);
                reservoirIDs.clear();
                break;
            case '3':
                    for (auto v : sistema.getGraph().getVertexSet()){
                        reservoirIDs.push_back(v->getInfo());
                    }
                    sistema.removeReservoirVector(reservoirIDs);
                break;
            case 'b':
                back = true;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

}

void basicServiceMetrics(const System& system) {

    System sistema = system;
    string choice;
    bool back = false;

    while (!back) {
        cout << "\n----------------------------------\n";
        cout << "      Service Metrics Menu      \n";
        cout << "----------------------------------\n";
        cout << "Choose an option:\n";
        cout << "1. Check the Maximum water Flow \n";
        cout << "2. Check Water Supply Sufficiency and Water deficits\n";
        cout << "3. Balance the Load across the Network \n";
        cout << "b. Go back\n";
        cout << "----------------------------------\n";
        cout << "Your choice: " << endl;

        cin >> choice;

        if (choice.length() != 1){
            choice = "h";
        }

        switch (choice[0]) {
            case '1':
                 MaxFlowMenu(system);
                break;
            case '2':
                 sistema.enoughWater();
                break;
            case '3':
                //calculateFlights(graph);
                break;
            case 'b':
                back = true;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

}


void MaxFlowMenu(const System &system) {

    System sistema = system;

    string choice;
    bool back = false;

    while (!back) {
        cout << "\n----------------------------------\n";
        cout << "      Service Metrics Menu      \n";
        cout << "----------------------------------\n";
        cout << "Choose an option:\n";
        cout << "1. Calculate the Maximum Flow for a single city \n";
        cout << "2. Calculate the Maximum Flow for every city\n";
        cout << "3. Calculate the Maximum Flow of the whole Network \n";
        cout << "b. Go back\n";
        cout << "----------------------------------\n";
        cout << "Your choice: " << endl;

        cin >> choice;

        if (choice.length() != 1){
            choice = "h";
        }

        string choice2;

        switch (choice[0]) {
            case '1':
                cout << "Enter the desired City's code:" << endl;
                cin >> choice2;
                sistema.maxFlowSingleCity(choice2);
                break;
            case '2':
                sistema.maxFlowEachCity();
                break;
            case '3':
                sistema.maxFlowSystem();
                break;
            case 'b':
                back = true;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }


}


void App::run() {
    mainMenu();
}
