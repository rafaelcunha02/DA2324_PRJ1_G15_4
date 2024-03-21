#include "App.h"
#include "System.h"
#include <iostream>


void basicServiceMetrics(const System& system);

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
                //displayFilterMenu();
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

void basicServiceMetrics(const System& system) {

    string choice;
    bool back = false;

    while (!back) {
        cout << "\n----------------------------------\n";
        cout << "      Service Metrics Menu      \n";
        cout << "----------------------------------\n";
        cout << "Choose an option:\n";
        cout << "1. Calculate the Maximum water flow \n";
        cout << "2. Check Water Supply Sufficiency and Water deficits\n";
        cout << "3. Balance the Load across the Network \n";
        cout << "4. Count countries served by a specific airport/city\n";
        cout << "b. Go back\n";
        cout << "----------------------------------\n";
        cout << "Your choice: " << endl;

        cin >> choice;

        if (choice.length() != 1){
            choice = "h";
        }

        switch (choice[0]) {
            case '1':
                //displayGlobalNumbers();
                break;
            case '2':
                //displayFlightsFromAirport(graph);
                break;
            case '3':
                //calculateFlights(graph);
                break;
            case '4':
                //displayCountriesFromAirportCity(graph);
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