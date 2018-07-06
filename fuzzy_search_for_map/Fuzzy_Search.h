/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



#ifndef FUSSY_SEARCH_H
#define FUSSY_SEARCH_H

#include <string>
#include <vector>
#include "StreetsDatabaseAPI.h"
#include "global.h"
using namespace std;


//Algorithm to define the distance between two strings
unsigned LevenshteinDistance(std:: string  s1, std:: string  s2);

//Using LevenshteinDistance algorithm to return street names
std::vector<string> return_selected_street_PointsOfInterest_names_LevenshteinDistance(string search_input);

//Returning street names for exactly matched inputs
std::vector<string> return_selected_street_PointsOfInterest_names_first_few_match(string search_input);



std::vector<string> find_closest_names(string input);


//set<string> return_selected_intersections_name(string search_input);

#endif /* FUSSY_SEARCH_H */

