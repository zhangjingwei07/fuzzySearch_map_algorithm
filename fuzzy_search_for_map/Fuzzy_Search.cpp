/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */




#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "m1.h"
#include "m2.h"
#include "Fuzzy_Search.h"
#include "global.h"

using namespace std;

extern global globalVar;
//This may slow the procesing speed, needs to fix
#define FUZZYNAME_DISPLAY_NUM 100 //Number of names to display
#define FUZZYNAME_CHAR_THRESHOLD_first_few 1 //Number of characters to trigger fuzzy search base on first few
#define FUZZYNAME_CHAR_THRESHOLD_Levenshtein 2 //Number of characters to trigger fuzzy search based on Lev

#define FUZZYNAME_TOLERANCE_STREETNAME 3 //allowed steps to change one string to another, based on research
//#define FUSSYNAME_TOLERANCE_INTERSECTIONNAME  10//allowed steps between two intersections names


unsigned LevenshteinDistance(std::string  s1, std::string  s2){
    
    unsigned int len1 = s1.size();
    unsigned int len2 = s2.size();
    int temp = 0;
    std::vector<unsigned> vec(len2 + 1), preVec(len2 + 1);
    
    //This loop stores the distance is number of characters to be deleted from vec
    for(unsigned i = 0; i< preVec.size(); i++)
        preVec[i] = i;  
    
    
    for(unsigned i = 0; i < len1; i++){      
        vec[0] = i + 1;
        
        for(unsigned j = 0; j < len2; j++){
           
            
            if(s1[i] == s2[j])
                temp = 0;
            else
                temp = 1;           
            //preVec[1+j]+1 is deletion cost, the other two are add cost and insertion cost
            vec[j+1] = std::min({preVec[1+j] + 1, vec[j] + 1, preVec[j] + temp});            
        }            
        vec.swap(preVec);
        
    }   
    return preVec[len2];   
    
}


//This return the selected string corresponding to LevenshteinDistance
std::vector<string> return_selected_street_PointsOfInterest_names_LevenshteinDistance(string search_input) {

    //Delete all leading and trailing spaces considering humans searching behaviors
    search_input.erase(search_input.begin(), std::find_if(search_input.begin(), search_input.end(), 
            std::bind1st(std::not_equal_to<char>(), ' ')));
    search_input.erase(std::find_if(search_input.rbegin(), search_input.rend(),
            std::bind1st(std::not_equal_to<char>(), ' ')).base(), search_input.end());
    
    //Transform input into all lower case
    transform(search_input.begin(), search_input.end(), search_input.begin(), ::toupper);
    
    std::vector<string> fuzzy_selected_names;
    std::set<string>::iterator it;

    //Fussy search triggers
    if (search_input.size() >= FUZZYNAME_CHAR_THRESHOLD_Levenshtein) {
        

        //this loop is to ensure that the display of fuzzy names starts from
        //the most similar to different
        for (unsigned tolerance_level = 0; tolerance_level <= FUZZYNAME_TOLERANCE_STREETNAME; tolerance_level++) {
           
            //This is certified that it breaks all loops as display number is full
            if (fuzzy_selected_names.size() > FUZZYNAME_DISPLAY_NUM) break;
            
            //loop through all elements for comparison
            for (it = globalVar.street_pointOfInterest_names_fuzzy_only.begin();
                    it != globalVar.street_pointOfInterest_names_fuzzy_only.end();
                    it++) {
                string testing_name = *it;               
                
                //transform potential street names into all lower case, so that to 
                //compare with search input
                transform(testing_name.begin(), testing_name.end(), testing_name.begin(), ::toupper);
                
                
                //if the vector of selected names still have spaces
                if (fuzzy_selected_names.size() >= FUZZYNAME_DISPLAY_NUM)
                    break;
                
                   //if the street name corresponds to the require tolerance level
                    if (LevenshteinDistance(search_input, testing_name.substr(0,search_input.size())) == tolerance_level) {
                        
                        fuzzy_selected_names.push_back(*it);
                    }
            }
            
                     
        }
    }
    
    //Following code is used to reorder the rudimentary results to cater to 
    //user's experience. Order is based on the matching of the position of the string 
    //characters.
    
    
    unsigned num_of_choice = fuzzy_selected_names.size();
    unsigned smallest_size = 100;
    std::vector<string> final_selected_names;
   
    //pick the smallest size of string
    for(unsigned index = 0; index < num_of_choice; index ++){
        if(fuzzy_selected_names[index].size() < smallest_size)
            smallest_size = fuzzy_selected_names[index].size();
    }
            
    //traverse each selected names character by character.
    //reorder the selected names corresponding to the characters match order
    for(unsigned index_char = 0; index_char < smallest_size; index_char++){
        
       for(unsigned index_name = 0; index_name < num_of_choice; index_name++) {
           
           //if the certain character matches and the final_selected_names do not have
           //duplicated names
           if(fuzzy_selected_names[index_name][index_char] == search_input[index_char]
                   && (std::find (final_selected_names.begin(), final_selected_names.end(), 
                   fuzzy_selected_names[index_name]) == final_selected_names.end()))
               
               final_selected_names.push_back(fuzzy_selected_names[index_name]);
           
           
           //if the final_selected_names size is full, then break
           if(final_selected_names.size() == num_of_choice)
               break;
           
       }
    }
    
    return final_selected_names;
}

//This returns the street names partially match the user's input 

std::vector<string> return_selected_street_PointsOfInterest_names_first_few_match(string search_input) {

    //Delete all leading and trailing spaces considering humans searching behaviors
    search_input.erase(search_input.begin(), std::find_if(search_input.begin(), search_input.end(), 
            std::bind1st(std::not_equal_to<char>(), ' ')));
    search_input.erase(std::find_if(search_input.rbegin(), search_input.rend(),
            std::bind1st(std::not_equal_to<char>(), ' ')).base(), search_input.end());
    
    //Transform input into all lower case
    transform(search_input.begin(), search_input.end(), search_input.begin(), ::toupper);
    
    std::vector<string> fuzzy_selected_names;
    std::set<string>::iterator it;

    //
    if (search_input.size() >= FUZZYNAME_CHAR_THRESHOLD_first_few) {

        //loop through all elements for comparison
            for (it = globalVar.street_pointOfInterest_names_fuzzy_only.begin();
                    it != globalVar.street_pointOfInterest_names_fuzzy_only.end();
                    it++) {
                string testing_name = *it;               
               
                //transform potential street names into all lower case, so that to 
                //compare with search input
                transform(testing_name.begin(), testing_name.end(), testing_name.begin(), ::toupper);
            
            //this bool variable is to judge whether the street name is qualified
            bool match = true;

            //push back the streets names that match exactly the same as input for the
            //size of users inputs
            for (unsigned input_Idx = 0; input_Idx < search_input.size(); input_Idx++) {

                //if within the size of input, dismatch is discovered, then
                if (search_input[input_Idx] != testing_name[input_Idx]) {
                    match = false;
                    break;
                }


            }
            //loop is over, check if the street names match our requirements
            if (match)
                fuzzy_selected_names.push_back(*it);
        }
       
        return fuzzy_selected_names;

    }
    
    return fuzzy_selected_names;
}



vector<string> find_closest_names(string input){
    vector<string> vector1 = return_selected_street_PointsOfInterest_names_first_few_match(input);
    vector<string> vector2 = return_selected_street_PointsOfInterest_names_LevenshteinDistance(input);   
    vector<string>::iterator it_2;
        
    for (it_2 = vector2.begin(); it_2 != vector2.end(); it_2++)
        if(std::find (vector1.begin(), vector1.end(), *it_2) == vector1.end())
            vector1.push_back(*it_2);
    
    return vector1;
}





