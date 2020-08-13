/*
Title: Game.cpp
Author: Michael Harrigan
Date: 3/10/20
Description: Majority of the Subnautica game. This is the functions that come from Game.h as well as accesses those in Diver.h/.cpp
*/
#include "Game.h"

//Default Constructor
Game::Game(){
    // prints the game board to begin with the constructing
    GameTitle();

    // Makes the m_materials file filled with the items provided in the proj2_data.txt file
    LoadMaterials();
    
    cout << PROJ2_SIZE << " materials loaded." << endl;

    // initializes the diver to be used
    string diverName; // the name of the diver that the user wants to have
    cout << "What is the name of your diver?" << endl;
    getline(cin, diverName);
    m_myDiver.SetName(diverName);

} // end of Game constructor

void Game::LoadMaterials(){
    // creates temporary Material array and opens the file as Materials
    Material Mat[PROJ2_SIZE];
    ifstream Materials(PROJ2_DATA);

    if (Materials.is_open()){
        for (int i = 0; i < PROJ2_SIZE; i++){
            // sets each of the required variables in Material.h for each item in the file
            getline(Materials, Mat[i].m_name, ',');
            getline(Materials, Mat[i].m_type, ',');
            getline(Materials, Mat[i].m_material1, ',');
            getline(Materials, Mat[i].m_material2, ',');
            // because the depth is to be read as an int it doesn't use getline
            Materials >> Mat[i].m_depth;
            Materials.ignore(256, '\n');
            // sets each item to quantity zero
            Mat[i].m_quantity = 0;
            // sets the private variable m_materials to the temporary array
            m_materials[i] = Mat[i];
        } // end of the for loop

    } // end of if statement

    // closes the file
    Materials.close();

    // fills the array of the materials that the diver will use
    for (int j = 0; j < PROJ2_SIZE; j++){
        m_myDiver.AddMaterial(m_materials[j]);
    } // end of for loop

} // end of LoadMaterials

void Game::StartGame(){
    // Calls MainMenu, as well as the other game functions
    int playerInput = 0; // int for the players choice in what to do
    int devExit; // used to bypass the game and RNG to just test functions
    Material devMat; // used with devExit for getting different materials instantly

    // win condition and game ending choice
    while ((playerInput != QUIT) and (m_myDiver.CalcDepth() < MAX_DEPTH)){
        playerInput = Game::MainMenu();
        switch (playerInput){
            case DISPLAY:
                // user wants to display the diver's materials
                DisplayMaterials();
                break;
            case SEARCH:
                // user wants to search for raw materials
                SearchMaterials();
                break;
            case COMBINE:
                // user wants to merge materials together
                CombineMaterials();
                break;
            case SCORE:
                // user wants to see their score
                CalcScore();
                break;
            case QUIT:
                // user wants to exit the game
                break;
            case DEVEXIT:
                // dev exit to get any item.
                cout << "What item would you like to have?" << endl;
                cin >> devExit;
                devMat = m_myDiver.GetMaterial(devExit);
                m_myDiver.IncrementQuantity(devMat);
                cout << "Material given." << endl;
                break;
            default:
                // if the user inputs something that is not allowed
                cout << "Enter a number 1 through 5 please." << endl;
                cin >> playerInput;
                break;
        } // end of switch statement

    } // end of while loop

    cout << "Thank you for playing Subnautica!" << endl;

} // end of StartGame

void Game::DisplayMaterials(){
    // calls the Diver.cpp DisplayMaterials
    m_myDiver.DisplayMaterials();

} // end of DisplayMaterials

int Game::MainMenu(){
    int playerInput; // the number of the action the player wants to take
    cout << "\nWhat would you like to do?" << endl;
    cout << "1. Display your Diver's Materials" << endl;
    cout << "2. Search for Raw Materials" << endl;
    cout << "3. Attempt to Merge Materials" << endl;
    cout << "4. See Score" << endl;
    cout << "5. Quit" << endl;
    cin >> playerInput;
    
    // dev exit
    if (playerInput == DEVEXIT){
        return playerInput;
    } // end of dev exit

    // input validation that reprints the prompt
    while ((playerInput > QUIT) or (playerInput < DISPLAY)){
        cout << "Please enter something from 1 to 5\n\n";
        cout << "What would you like to do?" << endl;
        cout << "1. Display your Diver's Materials" << endl;
        cout << "2. Search for Raw Materials" << endl;
        cout << "3. Attempt to Merge Materials" << endl;
        cout << "4. See Score" << endl;
        cout << "5. Quit" << endl;
        cin >> playerInput;

    } // end of while loop

    return playerInput;

} // end of MainMenu

void Game::SearchMaterials(){
    // uses a random seed based on the time to choose what items to get and when
    int randIndex; // used as the index for returning the material
    srand(time(NULL));
    // allows only the indexes of 0 through 16 (all the raw materials)
    randIndex = (rand() % RAW_MAX);
    // calls IncrementQuantity using the m_materials list and the index created by the randomizer
    m_myDiver.IncrementQuantity(m_materials[randIndex]);
    // supposed to print out the name of the item that is found
    cout << m_materials[randIndex].m_name << " found!" << endl;

} // end of SearchMaterials

void Game::CombineMaterials(){
    int playerInput1; // variable for what the first item to be combined
    int playerInput2; // variable for the second item to be combined
    Material mat1; // material of the index playerInput1
    Material mat2; // material of the index playerInput2
    int craftingIndex; // index of the item to be crafted
    bool decrementChecker = true; // if true, should decrement, false should not decrement

    // introductory cout statements
    cout << "Which materials would you like to merge? " << endl;
    cout << "To list known materials, enter -1" << endl;
    // takes the input and places it into playerInput1 and playerInput2
    RequestMaterial(playerInput1);
    RequestMaterial(playerInput2);

    // sets the material variables to that of the Diver's material of the specific index
    mat1 = m_myDiver.GetMaterial(playerInput1);
    mat2 = m_myDiver.GetMaterial(playerInput2);

    // if the quantity of each item is enough to be combined, continues, else ends
    if (m_myDiver.CheckQuantity(mat1, mat2) == true){
        // finds the recipe that matches the 2 materials
        craftingIndex = SearchRecipes(mat1.m_name, mat2.m_name);
        // if there is no recipe that uses the materials
        if (craftingIndex == NEG_ONE){
            cout << "There is no matching recipe for these materials." << endl;
            decrementChecker = false;
        } // end of if statement

        // if the index is within the range of the unique items, as well as if the quantity is above 1
        else if ((craftingIndex >= ADVANCED_MAX) and (m_myDiver.GetMaterial(craftingIndex + 1).m_quantity == 1)){
            cout << "You cannot have more than 1 of any unique item at a time." << endl;
            decrementChecker = false;
        } // end of else if statement

        // if decrementChecker has been set to false, it skips this section
        if (decrementChecker == true){
            // decrements each of the 2 materials
            bool mat1Bool = m_myDiver.DecrementQuantity(mat1);
            bool mat2Bool = m_myDiver.DecrementQuantity(mat2);

            // if the index is of the type advanced, runs this section
            if ((craftingIndex >= RAW_MAX) and (craftingIndex < ADVANCED_MAX)){
                if (mat1Bool == false){
                    cout << "You do not have enough of " << mat1.m_name << " to merge." << endl;
                } // end of if statement

                if (mat2Bool == false){
                    cout << "You do not have enough of " << mat2.m_name << " to merge." << endl;
                } // end of if statement

                if ((mat1Bool) and (mat2Bool)){
                    // crafts the material
                    cout << m_materials[craftingIndex].m_name << " crafted!" << endl;
                    m_myDiver.IncrementQuantity(m_materials[craftingIndex]);
                } // end of if statement

            } // end of if statement

            // if the item that is wanted to be made is a unique item, and one is not had already
            else if ((craftingIndex >= ADVANCED_MAX) and (m_materials[craftingIndex].m_quantity == 0)){
                // checks for each items quantity
                if (mat1Bool == false){
                    cout << "You do not have enough of " << mat1.m_name << " to merge." << endl;
                } // end of if statement

                if (mat2Bool == false){
                    cout << "You do not have enough of " << mat2.m_name << " to merge." << endl;
                } // end of if statement

                if ((mat1Bool) and (mat2Bool)){
                    // crafts the material
                    cout << m_materials[craftingIndex].m_name << " crafted!" << endl;
                    m_myDiver.IncrementQuantity(m_materials[craftingIndex]);
                } // end of if statement

            } // end of else if statement

        } // end of if statement

    } // end of if statement

    else{
        cout << "You do not have enough " << mat1.m_name << " or " << mat2.m_name << "." << endl;
    } // end of else statement

} // end of CombineMaterials

void Game::RequestMaterial(int &choice){
    // takes the input and checks for -1
    cin >> choice;
    if (choice == NEG_ONE){
        DisplayMaterials();
        cout << "Which materials would you like to merge? " << endl;
        cin >> choice;
    } // end of if statement

    while ((choice < INPUT_MIN) or (choice > INPUT_MAX)){
        cout << "Please enter an integer from 1 to 50." << endl;
        cin >> choice;
    } // end of while loop

} // end of RequestMaterial

int Game::SearchRecipes(string mat1, string mat2){
    // starts with the first advanced item and checks until the last unique item
    for (int i = RAW_MAX; i < PROJ2_SIZE; i++){
        // if there is a recipe that uses the 2 materials, continues, else returns -1
        if (((m_materials[i].m_material1 == mat1) and (m_materials[i].m_material2 == mat2)) or ((m_materials[i].m_material2 == mat1) and (m_materials[i].m_material1 == mat2))){
            return i;
        } // end of if statement

    } // end of for loop

    // arbitrary value for determining if the recipe works
    return NEG_ONE;

} // end of SearchRecipes

void Game::CalcScore(){
    cout << "This is the Diver " << m_myDiver.GetName() << "'s current depth: " << m_myDiver.CalcDepth() << endl;
    cout << "This is the maximum depth: " << MAX_DEPTH << endl;

} // end of CalcScore