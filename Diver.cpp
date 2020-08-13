/*
Title: Diver.cpp
Author: Michael Harrigan
Date: 3/10/20
Description: Uses the information from Diver.h to create functions for use in the Subnautica game.
*/
#include "Diver.h"


// Default Constructor
Diver::Diver(){
    // loads the default diver and sets the values of each of the private variables
    m_myName = "Milkman"; 
    m_numMaterials = 0;
    Material m_myMaterials[PROJ2_SIZE] = {};

} // end of default Diver Constructor

Diver::Diver(string name){
    // loads the diver using a default value and the name given by the user
    SetName(name);
    m_numMaterials = 0;
    Material m_myMaterials[PROJ2_SIZE] = {};

} // end of Diver constructor

string Diver::GetName(){
    return m_myName;

} // end of GetName

void Diver::SetName(string name){
    m_myName = name;

} // end of SetName

void Diver::DisplayMaterials(){
    // prints out the materials with the number that represents them as well as the quantity
    for (int i = 0; i < m_numMaterials; i++){
        cout << i + 1 << ". " << m_myMaterials[i].m_name << " " << m_myMaterials[i].m_quantity << endl;
    } // end of for loop

} // end of DisplayMaterials

int Diver::CheckMaterial(Material mat){
    // if mat is in the list of known materials return the index, else returns -1
    for (int i = 0; i < m_numMaterials; i++){
        if (m_myMaterials[i].m_name == mat.m_name){
            return i;
        } // end of if statement

    }  // end of for loop

    return -1;
} // if function returns -1, adds the material to the known materials and sets the quantity to 0

void Diver::AddMaterial(Material mat){
    // adds the material to what the diver knows, ties it to the index m_numMaterials is currently set to
    m_myMaterials[m_numMaterials] = mat;
    m_numMaterials++;

} // end of AddMaterial

void Diver::IncrementQuantity(Material mat){
    m_myMaterials[CheckMaterial(mat)].m_quantity += 1; // the quantity of the material is increased if the diver knows of it

} // end of IncrementQuantity

bool Diver::DecrementQuantity(Material mat){
// the quantity of the material is decreased
// when the diver uses a material, removes one off the quantity
    if (m_myMaterials[CheckMaterial(mat)].m_quantity == 0){
    return false;
    } // end of if statement

    else{
        m_myMaterials[CheckMaterial(mat)].m_quantity -= 1;
        return true;
    } // end of else statement

} // end of DecrementQuantity

bool Diver::CheckQuantity(Material mat1, Material mat2){
    // if the names of each of the materials are the same, checks that there are 2
    if (mat1.m_name == mat2.m_name){
        if (mat1.m_quantity >= 2){
            // if there are 2 or more, returns true.
            // if there are less than 2 it returns false.
            return true;
        } // end of if statement

        else{
            return false;
        } // end of else statement

    } // end of if statement

    else{
        // if both materials have a quantity greater than or equal to 1 returns true.
        // if only one, or neither have 1, then returns false.
        if ((mat1.m_quantity >= 1) and (mat2.m_quantity >= 1)){
            return true;
        } // end of if statement

        else{
            return false;
        } // end of else statement

    } // end of else statement

} // end of CheckQuantity

Material Diver::GetMaterial(int materialInput){
    for (int i = 0; i < m_numMaterials; i++){
        if (materialInput - 1 == i){
            return m_myMaterials[materialInput - 1];
        } // end of if statement

    } // end of for loop

    // arbitrary value for -Wall to stop yelling at me
    return m_myMaterials[0];

} // end of GetMaterial

int Diver::CalcDepth(){
    int sumDepth = 0; // initializes the amount depth that the player begins at
    for (int i = 0; i < m_numMaterials; i++){
        // if the item can increase the depth of the diver, it is tracked here
        if (m_myMaterials[i].m_quantity >= 1){
            sumDepth += m_myMaterials[i].m_depth;
        } // end of if statement
    
    } // end of for loop

    // the sum of the depths of the items that the Diver currently has
    return sumDepth;

} // end of CalcDepth