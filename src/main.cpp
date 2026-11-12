#include "../include/menu.h"

int main() {
    int spaceX = 10;
    int spaceY = 5;
    int size = 1;
    Users** userPtr = new Users*[size]{nullptr};

    initMenu(spaceX, spaceY, userPtr, size);

    for (int i = 0; i < size; ++i) {
        if (userPtr[i] != nullptr) {
            delete userPtr[i];
        }
    }
    delete[] userPtr;
    return 0;
}