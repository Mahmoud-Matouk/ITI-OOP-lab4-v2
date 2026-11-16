#ifndef MENU_H
#define MENU_H
#include "console.h"
#include "dynamicUsers.h"

void displayMainMenu(int spaceX, int spaceY, int selectedIndex, Color color) {
    string options[3] = {"1. New", "2. Display", "3. Logout"};
    printAt(spaceX, spaceY, "=== Application ===", 0, Color::Default);
    for (int i=1; i<=3; i++) {
        printAt(spaceX, spaceY+i*2, options[i-1],  selectedIndex == i, color);
    }
}

void displayNewUsersMenu(int spaceX, int spaceY, Users** &userPtr, int &ptrSize) {
    clearScreen();
    if (ptrSize <= 0) {
        printAt(spaceX, spaceY, "Invalid size. Press any key to return to the menu.", true, Color::Red);
        getArrowKey();
        return;
    }
    int n = 0;
    while (n < ptrSize && userPtr[n] != nullptr) {
        ++n;
    }
    if (n == ptrSize) {
        ptrSize *= 2;
        Users** newUsers = new Users*[ptrSize]{nullptr};
        for (int i = 0; i < ptrSize/2; i++) {
            newUsers[i] = userPtr[i];
        }
        delete[] userPtr;
        userPtr = newUsers;
    }

    Users* tempUser = new Users();

    char input[MAX_SIZE]{};
    int lineStep{};

    do {
        printAt(spaceX, spaceY + lineStep, "Enter name:");
        goToXY(spaceX, spaceY + lineStep + 1);
        if (!isEscPressed(input, MAX_SIZE)) {
            delete tempUser;
            return;
        }
        if (!tempUser->setName(input)) {
            printAt(spaceX * 3, spaceY + lineStep + 2, "Invalid name (no numbers allowed)!", true, Color::Red);
        }
        lineStep+=2;
    } while (tempUser->getName()[0] == '\0');

    do {
        printAt(spaceX, spaceY + lineStep, "Enter Password:");
        goToXY(spaceX, spaceY + lineStep + 1);
        if (!isEscPressed(input, MAX_SIZE)) {
            delete tempUser;
            return;
        }
        if (!tempUser->setPassword(input)) {
            printAt(spaceX * 3, spaceY + lineStep + 2, "Invalid password (must be 7-99 characters)!", true, Color::Red);
        }
        lineStep+=2;
    } while (!tempUser->isPasswordSet());

    do {
        printAt(spaceX, spaceY + lineStep, "Enter email:");
        goToXY(spaceX, spaceY + lineStep + 1);
        if (!isEscPressed(input, MAX_SIZE)) {
            delete tempUser;
            return;
        }
        if (!tempUser->setEmail(input)) {
            printAt(spaceX * 3, spaceY + lineStep + 2, "Invalid email format (needs @ and domain)!", true, Color::Red);
        }
        lineStep+=2;
    } while (tempUser->getEmail()[0] == '\0');

    int age = 0;
    do {
        printAt(spaceX, spaceY + lineStep, "Enter age: ");
        goToXY(spaceX, spaceY + lineStep + 1);

        if (!isEscPressed(input, MAX_SIZE)) {
            delete tempUser;
            return;
        }
        age = atoi(input);
        if (!tempUser->setAge(age)) {
            printAt(spaceX * 3, spaceY + lineStep + 2, "Invalid age (must be 1-100)!", true, Color::Red);
            lineStep+=2;
        } 
        else {
            printAt(spaceX * 3, spaceY + lineStep + 2, "                                            ");
            lineStep += 3;
            break;
        }

    } while (tempUser->getAge() == 0);

    userPtr[n] = tempUser;
    printAt(spaceX, spaceY + lineStep + 2, "User added successfully! Press ESC to return to menu...", true, Color::Green);
    while (getArrowKey() != 27);
}

void displayUsersDataMenu(int spaceX, int spaceY, Users* userPtr[], int ptrSize) {
    clearScreen();

    if (ptrSize <= 0) {
        printAt(spaceX, spaceY, "Invalid menu size. Press ESC to return.", true, Color::Red);
        while (getArrowKey() != 27);
        return;
    }

    int userCount = 0;
    while (userCount < ptrSize && userPtr[userCount] != nullptr) {
        userCount++;
    }

    if (userCount == 0) {
        printAt(spaceX, spaceY, "No users registered yet!", true, Color::Red);
        printAt(spaceX, spaceY + 2, "Press ESC to return to main menu...", true, Color::White);
        while (getArrowKey() != 27);
        return;
    }

    printAt(spaceX, spaceY, "USERS LIST");

    int currentY = spaceY + 2;

    for (int i = 0; i < userCount; ++i) {
        printAt(spaceX, currentY, ("User #" + to_string(i + 1) + ":").c_str());

        printAt(spaceX + 12, currentY, "Name:");
        printAt(spaceX + 18, currentY, userPtr[i]->getName());

        printAt(spaceX + 38, currentY, "Email:");
        printAt(spaceX + 45, currentY, userPtr[i]->getEmail());

        printAt(spaceX + 72, currentY, "Age:");
        printAt(spaceX + 77, currentY, to_string(userPtr[i]->getAge()).c_str());

        currentY += 2; 
    }

    printAt(spaceX, currentY + 3, "Press ESC to return to menu...", true, Color::Green);
    while (getArrowKey() != 27);
}

void initMenu(int spaceX, int spaceY, Users** &userPtr, int &ptrSize) {
    int selectedIndex{1};
    while(true) {
        clearScreen();
        displayMainMenu(spaceX, spaceY, selectedIndex, Color::Red);

        char key = getArrowKey();
        if (key == 'A' && selectedIndex > 1) {
            selectedIndex = (selectedIndex - 1)%4;
        } else if (key == 'B' && selectedIndex < 3) {
            selectedIndex = (selectedIndex + 1)%4;
        }  else if (key == 'H') {
            selectedIndex = 1;
        } else if (key == 'F') {
            selectedIndex = 3;
        } else if (key == '\n' || key == '\r') {
            switch (selectedIndex) {
                case 1:
                    displayNewUsersMenu(spaceX, spaceY, userPtr, ptrSize);
                    break;
                case 2:
                    displayUsersDataMenu(spaceX, spaceY, userPtr, ptrSize);
                    break;
                case 3:
                    return;
                }
        }
        clearScreen();
    }
}

#endif