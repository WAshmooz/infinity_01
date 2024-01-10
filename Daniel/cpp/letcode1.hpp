#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

class FoodRatings 
{
private:
    unordered_map<string, string> foodCuisineMap;
    unordered_map<string, int> foodRatingMap;
    unordered_map<string, map<int, set<string>>> cuisineHighestRatedMap;

public:
    FoodRatings(vector<string>& foods, vector<string>& cuisines, vector<int>& ratings) 
    {
        for (int i = 0; i < foods.size(); ++i) 
        {
            foodCuisineMap[foods[i]] = cuisines[i];
            foodRatingMap[foods[i]] = ratings[i];

            // Update the cuisineHighestRatedMap
            cuisineHighestRatedMap[cuisines[i]][ratings[i]].insert(foods[i]);
        }
    }

    void changeRating(string food, int newRating) 
    {
        string cuisine = foodCuisineMap[food];
        int oldRating = foodRatingMap[food];

        // Remove the food from the old rating set
        cuisineHighestRatedMap[cuisine][oldRating].erase(food);

        // Update food's rating
        foodRatingMap[food] = newRating;

        // Update the cuisineHighestRatedMap with new rating
        cuisineHighestRatedMap[cuisine][newRating].insert(food);
    }

    string highestRated(string cuisine) 
    {
        auto& ratingFoodMap = cuisineHighestRatedMap[cuisine];
        int highestRating = ratingFoodMap.rbegin()->first; // Get the highest rating

        // Get the lexicographically smallest food item with the highest rating
        return *ratingFoodMap[highestRating].begin();
    }
};


string highestRated(string cuisine) {
    map<int, set<string>>& ratingFoodMap = cuisineHighestRatedMap[cuisine];
    int highestRating = ratingFoodMap.rbegin()->first; // Get the highest rating

    // Get the lexicographically smallest food item with the highest rating
    set<string>& highestRatedFoods = ratingFoodMap[highestRating];
    return *highestRatedFoods.begin();
}