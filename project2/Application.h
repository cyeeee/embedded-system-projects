/*
 * Application.h
 *
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <HAL/HAL.h>
#include <HAL/Graphics.h>
#include <HAL/Timer.h>

#define TITLE_SCREEN_WAIT   3000  //3 seconds
#define COUNTDOWN_WAIT      1000  //1 seconds

enum _GameState
{
    TITLE_SCREEN, MENU_SCREEN, HowToPlay_SCREEN, HighScore_SCREEN, GAME_SCREEN, RESULT_SCREEN
};
typedef enum _GameState GameState;

enum _Cursor
{
    CURSOR_0 = 0, CURSOR_1 = 1, CURSOR_2 = 2
};
typedef enum _Cursor Cursor;

enum _Recipe
{
    RECIPE_0, RECIPE_1, RECIPE_2, RECIPE_3, RECIPE_4, RECIPE_5, RECIPE_6, RECIPE_7, RECIPE_8, RECIPE_9
};
typedef enum _Recipe Recipe;

enum _Ingredient
{
    Empty, INGREDIENT_1, INGREDIENT_2, INGREDIENT_3, INGREDIENT_4, INGREDIENT_5
};
typedef enum _Ingredient Ingredient;

struct _Application
{

    SWTimer titleTimer;     // General-purpose timer for when screens must disappear
    GameState state;        // Determines which screen is currently shown
    Cursor cursor;          // Cursor tracker variable
    Recipe recipe;          // Determines which recipe to show based on the random number
    SWTimer timer;          // timer for count down
    Ingredient ingredient;  // variable for ingredient selection

    // number of each ingredient selected
    int tomato;
    int patty;
    int pickle;
    int lettuce;
    int cheese;
    // number of each ingredient in recipe
    int tomato_r;
    int patty_r;
    int pickle_r;
    int lettuce_r;
    int cheese_r;

    int countdown;          // count down timer
    int income;             // money earned

    // compare selected ingredient
    int currentSelect;
    int previousSelect;

    int strike;             // number of wrong order
    int success;            // number of correct order
    bool correctOrder;

    // top scores
    int top1;
    int top2;
    int top3;

};
typedef struct _Application Application;

// Called only a single time - inside of main(), where the application is constructed
Application Application_construct(HAL* hal);

// Called once per super-loop of the main application.
void Application_loop(Application* app, HAL* hal);

// Drawing screens
void Application_showTitleScreen(GFX* gfx);
void Application_showMenuScreen(Application* app, GFX* gfx);
void Application_showHowToPlayScreen(GFX* gfx);
void Application_showHighScoreScreen(Application* app, GFX* gfx);
void Application_showGameScreen(Application* app, GFX* gfx, HAL* hal);
void Application_showResultScreen(Application* app, GFX* gfx);

// Handle each screens
void Application_handleTitleScreen(Application* app, HAL* hal);
void Application_handleMenuScreen(Application* app, HAL* hal);
void Application_handleHowToPlayScreen(Application* app, HAL* hal);
void Application_handleHighScoreScreen(Application* app, HAL* hal);
void Application_handleGameScreen(Application* app, HAL* hal);
void Application_handleResultScreen(Application* app, HAL* hal);

// Used to reset the internal game variables
void Application_initGameVariables(Application* app, HAL* hal);
void Application_resetIngredientNum(Application* app);

// Helper functions
void Application_updateMenuScreen(Application* app, GFX* gfx);
void Application_printIncome(Application* app, GFX* gfx);
void Application_drawGameScreen(GFX* gfx);
void Application_drawFrame(GFX* gfx);
void Application_showRecipes(Application* app, GFX* gfx, HAL* hal);
void Application_printRecipe1(Application* app, GFX* gfx);
void Application_printRecipe2(Application* app, GFX* gfx);
void Application_printRecipe3(Application* app, GFX* gfx);
void Application_printRecipe4(Application* app, GFX* gfx);
void Application_printRecipe5(Application* app, GFX* gfx);
void Application_printRecipe6(Application* app, GFX* gfx);
void Application_printRecipe7(Application* app, GFX* gfx);
void Application_printRecipe8(Application* app, GFX* gfx);
void Application_printRecipe9(Application* app, GFX* gfx);
void Application_printRecipe10(Application* app, GFX* gfx);
void Application_updateTimer(Application* app, GFX* gfx);
void Application_interpretIngredient(Application* app, HAL* hal);
void Application_hightlightIngredient(Application* app, HAL* hal);
void Application_addIngredient(Application* app, HAL* hal);
void Application_removeIngredient(Application* app, HAL* hal);
void Application_addIndicator(Application* app, HAL* hal, int x1, int x2, int x3, int y, int q);
void Application_removeIndicator(Application* app, HAL* hal, int x1, int x2, int x3, int y, int q);
void Application_serveOrder(Application* app);
void Application_drawStrike(Application* app, GFX* gfx);
void Application_rankScores(Application* app);

#endif /* APPLICATION_H_ */
