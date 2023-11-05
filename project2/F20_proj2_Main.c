/**
 * Starter code for Project 2. Good luck!
 */

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* HAL and Application includes */
#include <Application.h>
#include <HAL/HAL.h>
#include <HAL/Timer.h>
#include <HAL/ADC_HAL.h>

extern const Graphics_Image titleScreen8BPP_UNCOMP;
extern const Graphics_Image menuScreen8BPP_UNCOMP;
extern const Graphics_Image tomato8BPP_UNCOMP;
extern const Graphics_Image patty8BPP_UNCOMP;
extern const Graphics_Image pickle8BPP_UNCOMP;
extern const Graphics_Image lettuce8BPP_UNCOMP;
extern const Graphics_Image cheese8BPP_UNCOMP;
extern const Graphics_Image gameover1BPP_UNCOMP;
extern const Graphics_Image victory8BPP_UNCOMP;

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void InitNonBlockingLED()
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void PollNonBlockingLED()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
}

int main()
{
    WDT_A_holdTimer();

    // Initialize the system clock and background hardware timer
    InitSystemTiming();

    // Initialize the main Application object and HAL object
    HAL hal = HAL_construct();
    Application app = Application_construct(&hal);

    Application_showTitleScreen(&hal.gfx);

    // Do not remove this line. This is your non-blocking check.
    InitNonBlockingLED();
    while (1)
    {
        // Do not remove this line. This is your non-blocking check.
        PollNonBlockingLED();

        // Main super-loop
        // call the main FSM function over and over.
        Application_loop(&app, &hal);
        HAL_refresh(&hal);
    }
}

/**
 * The main constructor for your application. This function should initialize
 * each of the FSMs which implement the application logic of your project.
 *
 * @return a completely initialized Application object
 */
Application Application_construct(HAL* hal)
{
    Application app;

    app.state = TITLE_SCREEN;
    app.titleTimer = SWTimer_construct(TITLE_SCREEN_WAIT);
    SWTimer_start(&app.titleTimer);

    app.cursor = CURSOR_0;

    app.top1 = 0;
    app.top2 = 0;
    app.top3 = 0;

    Application_initGameVariables(&app, hal);

    return app;
}


/**
 * The main super-loop function of the application. We place this inside of a
 * single infinite loop in main. In this way, we can model a polling system of
 * FSMs. Every cycle of this loop function, we poll each of the FSMs one time,
 * followed by refreshing all inputs to the system through a convenient
 * [HAL_refresh()] call.
 *
 * @param app:  A pointer to the main Application object.
 * @param hal:  A pointer to the main HAL object
 */
void Application_loop(Application* app, HAL* hal)
{
    // control screens
    switch (app->state)
    {
    case TITLE_SCREEN:
        Application_handleTitleScreen(app, hal);
        break;

    case MENU_SCREEN:
        Application_handleMenuScreen(app, hal);
        break;

    case HowToPlay_SCREEN:
        Application_handleHowToPlayScreen(app, hal);
        break;

    case HighScore_SCREEN:
        Application_handleHighScoreScreen(app, hal);
        break;

    case GAME_SCREEN:
        Application_handleGameScreen(app, hal);
        break;

    case RESULT_SCREEN:
        Application_handleResultScreen(app, hal);
        break;

    default:
        break;
    }

}

/**
 * Shows Title screen
 */
void Application_showTitleScreen(GFX* gfx)
{

    GFX_clear(gfx);

    GFX_print(gfx, "- The Busy Kitchen - ", 0, 0);

    Graphics_drawImage(&gfx->context, &titleScreen8BPP_UNCOMP, 22, 10);

    GFX_print(gfx, "          Chenyi Wang", 14, 0);
    GFX_print(gfx, "        ECE2564 Proj2", 15, 0);

}

/**
 * Display the main menu screen
 */
void Application_handleTitleScreen(Application* app, HAL* hal)
{
    // When the SW timer expires, show instruction screen
    if (SWTimer_expired(&app->titleTimer))
    {
        app->state = MENU_SCREEN;
        Application_showMenuScreen(app, &hal->gfx);

    }
}

/**
 * Shows Menu screen
 */
void Application_showMenuScreen(Application* app, GFX* gfx)
{
    GFX_clear(gfx);

    GFX_print(gfx, "- The Busy Kitchen - ", 0, 0);

    Graphics_drawImage(&gfx->context, &menuScreen8BPP_UNCOMP, 12, 14);

    GFX_print(gfx, " ---- Main Menu ---- ", 11, 0);
    GFX_print(gfx, "        Start        ", 12, 0);
    GFX_print(gfx, "     High Scores     ", 13, 0);
    GFX_print(gfx, "     How to Play     ", 14, 0);

    // Draw the cursor
    GFX_print(gfx, "   >", 12 + app->cursor, 0);
}

/**
 * Display the user selected screen
 */
void Application_handleMenuScreen(Application* app, HAL* hal)
{
    // If joystick is tapped either up or down, update the cursor
    if ((Joystick_isTappedUp(&hal->joystick)) && (app->cursor > 0)) {
        app->cursor = (Cursor) ((int) app->cursor - 1);
        Application_updateMenuScreen(app, &hal->gfx);
    }
    else if ((Joystick_isTappedDown(&hal->joystick)) && (app->cursor < 2)) {
        app->cursor = (Cursor) ((int) app->cursor + 1);
        Application_updateMenuScreen(app, &hal->gfx);
    }

    // If JSB is pressed, transit to the user selected state
    if (Button_isTapped(&hal->boosterpackJS))
    {
        switch (app->cursor)
        {
        case CURSOR_0: // Game
            app->state = GAME_SCREEN;
            Application_showGameScreen(app, &hal->gfx, hal);
            break;

        case CURSOR_1: // High Scores
            app->state = HighScore_SCREEN;
            Application_showHighScoreScreen(app, &hal->gfx);
            break;

        case CURSOR_2: // How to Play
            app->state = HowToPlay_SCREEN;
            Application_showHowToPlayScreen(&hal->gfx);
            break;

        default:
            break;
        }
    }
}

/**
 * A helper function which updates the main menu screen by redrawing only the
 * positions where the cursor could possibly be updated.
 */
void Application_updateMenuScreen(Application* app, GFX* gfx)
{
    // Clear the cursors from any previous game screen
    GFX_print(gfx, "    ", 12, 0);
    GFX_print(gfx, "    ", 13, 0);
    GFX_print(gfx, "    ", 14, 0);

    // Draw the cursor
    GFX_print(gfx, "   >", 12 + app->cursor, 0);
}

/**
 * Shows How To Play screen
 */
void Application_showHowToPlayScreen(GFX* gfx)
{

    GFX_clear(gfx);

    GFX_print(gfx, "How to Play           ", 0, 0);
    GFX_print(gfx, "-------------         ", 1, 0);
    GFX_print(gfx, "Move joystick to      ", 2, 0);
    GFX_print(gfx, "select the ingredient ", 3, 0);

    GFX_print(gfx, "BB1: add one serving  ", 5, 0);

    GFX_print(gfx, "BB2: remove one       ", 7, 0);
    GFX_print(gfx, "serving               ", 8, 0);

    GFX_print(gfx, "JSB: serve dish       ", 10, 0);

    GFX_print(gfx, "Follow the recipe &   ", 13, 0);
    GFX_print(gfx, "GOOD LUCK!            ", 14, 0);

}

/**
 * Go back the main menu screen
 */
void Application_handleHowToPlayScreen(Application* app, HAL* hal)
{
    // When the joystick is tapped, show main menu screen
    if (Button_isTapped(&hal->boosterpackJS))
    {
        app->state = MENU_SCREEN;
        Application_showMenuScreen(app, &hal->gfx);

    }
}

/**
 * A helper function which resets all the game variables to their unselected
 * states and resets the cursor position.
 */
void Application_initGameVariables(Application* app, HAL* hal)
{
    // Reset ingredient numbers
    Application_resetIngredientNum(app);

    // time is 30 seconds
    app->countdown = 30;
    app->timer = SWTimer_construct(COUNTDOWN_WAIT);
    SWTimer_start(&app->timer);

    // money is $0
    app->income = 0;

    // reset ingredient selection
    app->ingredient = Empty;

    // compare the ingredient selected
    app->currentSelect = (int) Empty;
    app->previousSelect = NULL;

    // number of orders
    app->strike = 0;        // wrong
    app->success = 0;       // correct
    app->correctOrder = false;
}

/**
 * A helper function which resets all the ingredient variables
 */
void Application_resetIngredientNum(Application* app)
{
    // every ingredient starts at zero
    app->tomato = 0;
    app->patty = 0;
    app->pickle = 0;
    app->lettuce = 0;
    app->cheese = 0;

    // reset the number of each ingredient in recipe
    app->tomato_r = 0;
    app->patty_r = 0;
    app->pickle_r = 0;
    app->lettuce_r = 0;
    app->cheese_r = 0;
}

/**
 * Shows Game screen
 */
void Application_showGameScreen(Application* app, GFX* gfx, HAL* hal)
{

    GFX_clear(gfx);

    GFX_print(gfx, "$            0:       ", 0, 0);
    Application_printIncome(app, gfx);
    Application_drawStrike(app, gfx);

    GFX_print(gfx, "Tomato  Patty  Pickle ", 2, 0);
    GFX_print(gfx, "Lettuce        Cheese ", 7, 0);

    // Draw the skeleton of the game graphics
    Graphics_drawImage(&gfx->context, &tomato8BPP_UNCOMP, 4, 25);
    Graphics_drawImage(&gfx->context, &patty8BPP_UNCOMP, 46, 25);
    Graphics_drawImage(&gfx->context, &pickle8BPP_UNCOMP, 89, 25);
    Graphics_drawImage(&gfx->context, &lettuce8BPP_UNCOMP, 4, 65);
    Graphics_drawImage(&gfx->context, &cheese8BPP_UNCOMP, 89, 65);

    Application_drawGameScreen(gfx);

    Application_showRecipes(app, gfx, hal);

}

/**
 * Show money earned and strike numbers
 */
void Application_printIncome(Application* app, GFX* gfx)
{
    // updates the money on screen
    char moneyString[2] = "";
    moneyString[1] = (app->income % 10) + '0';
    moneyString[0] = ((app->income / 10) % 10) + '0';
    GFX_print(gfx, moneyString, 0, 2);      // Print the current money value

}

/**
 * A helper function to update the count down timer
 */
void Application_updateTimer(Application* app, GFX* gfx)
{
    if (app->countdown >= 0) {
        char timerString[2] = "";
        timerString[1] = (app->countdown % 10) + '0';
        timerString[0] = ((app->countdown / 10) % 10) + '0';
        GFX_print(gfx, "  ", 0, 15);             // Clear the previous time value
        GFX_print(gfx, timerString, 0, 15);      // Print the current time value
        app->countdown--;
    }
}

/**
 * Draw the skeleton of the game graphics
 */
void Application_drawGameScreen(GFX* gfx) {

    // frame
    Application_drawFrame(gfx);

    // for tomato
    GFX_drawHollowCircle(gfx, COLUMN_1_1, ROW_1, RADIUS);   // 1 tomato is selected
    GFX_drawHollowCircle(gfx, COLUMN_1_2, ROW_1, RADIUS);   // 2 tomatoes are selected
    GFX_drawHollowCircle(gfx, COLUMN_1_3, ROW_1, RADIUS);   // 3 tomatoes are selected

    // for patty
    GFX_drawHollowCircle(gfx, COLUMN_2_1, ROW_1, RADIUS);
    GFX_drawHollowCircle(gfx, COLUMN_2_2, ROW_1, RADIUS);
    GFX_drawHollowCircle(gfx, COLUMN_2_3, ROW_1, RADIUS);

    // for pickle
    GFX_drawHollowCircle(gfx, COLUMN_3_1, ROW_1, RADIUS);
    GFX_drawHollowCircle(gfx, COLUMN_3_2, ROW_1, RADIUS);
    GFX_drawHollowCircle(gfx, COLUMN_3_3, ROW_1, RADIUS);

    // for lettuce
    GFX_drawHollowCircle(gfx, COLUMN_1_1, ROW_2, RADIUS);
    GFX_drawHollowCircle(gfx, COLUMN_1_2, ROW_2, RADIUS);
    GFX_drawHollowCircle(gfx, COLUMN_1_3, ROW_2, RADIUS);

    // for cheese
    GFX_drawHollowCircle(gfx, COLUMN_3_1, ROW_2, RADIUS);
    GFX_drawHollowCircle(gfx, COLUMN_3_2, ROW_2, RADIUS);
    GFX_drawHollowCircle(gfx, COLUMN_3_3, ROW_2, RADIUS);

}

/**
 * Draw the skeleton of the game graphics (helper function)
 */
void Application_drawFrame(GFX* gfx)
{
    GFX_drawRectangle(gfx, X_MIN, Y_MIN, X_MAX, Y_MAX);
    GFX_drawLineH(gfx, X_MIN, X_MAX, Y_1);
    GFX_drawLineV(gfx, X_1, Y_MIN, Y_MAX);
    GFX_drawLineV(gfx, X_2, Y_MIN, Y_MAX);
}

/**
 * Show recipes based on the random number
 */
void Application_showRecipes(Application* app, GFX* gfx, HAL* hal)
{
    // create a random numbers between 0 and 9
    int randomNumber = hal->joystick.x % 10;
    app->recipe = (Recipe) randomNumber;

    switch (app->recipe)
    {
    case RECIPE_0:
        Application_printRecipe1(app, gfx);
        break;
    case RECIPE_1:
        Application_printRecipe2(app, gfx);
        break;
    case RECIPE_2:
        Application_printRecipe3(app, gfx);
        break;
    case RECIPE_3:
        Application_printRecipe4(app, gfx);
        break;
    case RECIPE_4:
        Application_printRecipe5(app, gfx);
        break;
    case RECIPE_5:
        Application_printRecipe6(app, gfx);
        break;
    case RECIPE_6:
        Application_printRecipe7(app, gfx);
        break;
    case RECIPE_7:
        Application_printRecipe8(app, gfx);
        break;
    case RECIPE_8:
        Application_printRecipe9(app, gfx);
        break;
    case RECIPE_9:
        Application_printRecipe10(app, gfx);
        break;
    default:
        break;
    }


}

/**
 * A helper function to print recipe.
 * Used to break down the showRecipes() function into smaller
 * sub-functions.
 */
void Application_printRecipe1(Application* app, GFX* gfx)
{
    GFX_print(gfx, "The Double            ", 12, 0);
    GFX_print(gfx, "Patty(2), Cheese      ", 13, 0);
    app->patty_r = 2;
    app->cheese_r = 1;
}

/**
 * A helper function to print recipe.
 * Used to break down the showRecipes() function into smaller
 * sub-functions.
 */
void Application_printRecipe2(Application* app, GFX* gfx)
{
    GFX_print(gfx, "The Veggie            ", 12, 0);
    GFX_print(gfx, "Lettuce, Tomato(2),   ", 13, 0);
    GFX_print(gfx, "Pickle(3)             ", 14, 0);
    app->lettuce_r = 1;
    app->tomato_r = 2;
    app->pickle_r = 3;
}

/**
 * A helper function to print recipe.
 * Used to break down the showRecipes() function into smaller
 * sub-functions.
 */
void Application_printRecipe3(Application* app, GFX* gfx)
{
    GFX_print(gfx, "The Original          ", 12, 0);
    GFX_print(gfx, "Patty, Lettuce,       ", 13, 0);
    GFX_print(gfx, "Tomato, Pickle(2),    ", 14, 0);
    GFX_print(gfx, "Cheese                ", 15, 0);
    app->patty_r = 1;
    app->lettuce_r = 1;
    app->tomato_r = 1;
    app->pickle_r = 2;
    app->cheese_r = 1;
}

/**
 * A helper function to print recipe.
 * Used to break down the showRecipes() function into smaller
 * sub-functions.
 */
void Application_printRecipe4(Application* app, GFX* gfx)
{
    GFX_print(gfx, "TCP/1P                ", 12, 0);
    GFX_print(gfx, "Tomato, Cheese        ", 13, 0);
    GFX_print(gfx, "Patty, Pickle         ", 14, 0);
    app->patty_r = 1;
    app->tomato_r = 1;
    app->pickle_r = 1;
    app->cheese_r = 1;
}

/**
 * A helper function to print recipe.
 * Used to break down the showRecipes() function into smaller
 * sub-functions.
 */
void Application_printRecipe5(Application* app, GFX* gfx)
{
    GFX_print(gfx, "ANGRY BURG!           ", 12, 0);
    GFX_print(gfx, "Patty(3), Lettuce(2)  ", 13, 0);
    GFX_print(gfx, "Pickle(2), Cheese(3)  ", 14, 0);
    app->patty_r = 3;
    app->lettuce_r = 2;
    app->pickle_r = 2;
    app->cheese_r = 3;
}

/**
 * A helper function to print recipe.
 * Used to break down the showRecipes() function into smaller
 * sub-functions.
 */
void Application_printRecipe6(Application* app, GFX* gfx)
{
    GFX_print(gfx, "Quarter Pounder       ", 12, 0);
    GFX_print(gfx, "Patty, Cheese(2)      ", 13, 0);
    GFX_print(gfx, "Pickle(2)             ", 14, 0);
    app->patty_r = 1;
    app->pickle_r = 2;
    app->cheese_r = 2;
}

/**
 * A helper function to print recipe.
 * Used to break down the showRecipes() function into smaller
 * sub-functions.
 */
void Application_printRecipe7(Application* app, GFX* gfx)
{
    GFX_print(gfx, "Cheeseburger          ", 12, 0);
    GFX_print(gfx, "Patty, Cheese         ", 13, 0);
    GFX_print(gfx, "Pickle                ", 14, 0);
    app->patty_r = 1;
    app->pickle_r = 1;
    app->cheese_r = 1;
}

/**
 * A helper function to print recipe.
 * Used to break down the showRecipes() function into smaller
 * sub-functions.
 */
void Application_printRecipe8(Application* app, GFX* gfx)
{
    GFX_print(gfx, "Double Cheese         ", 12, 0);
    GFX_print(gfx, "Patty(2), Cheese(2)   ", 13, 0);
    GFX_print(gfx, "Lettuce               ", 14, 0);
    app->patty_r = 2;
    app->cheese_r = 2;
    app->lettuce_r = 1;
}

/**
 * A helper function to print recipe.
 * Used to break down the showRecipes() function into smaller
 * sub-functions.
 */
void Application_printRecipe9(Application* app, GFX* gfx)
{
    GFX_print(gfx, "Triple Burger         ", 12, 0);
    GFX_print(gfx, "Patty(3), Lettuce     ", 13, 0);
    GFX_print(gfx, "Pickle, Tomato        ", 14, 0);
    app->patty_r = 3;
    app->lettuce_r = 1;
    app->pickle_r = 1;
    app->tomato_r = 1;
}

/**
 * A helper function to print recipe.
 * Used to break down the showRecipes() function into smaller
 * sub-functions.
 */
void Application_printRecipe10(Application* app, GFX* gfx)
{
    GFX_print(gfx, "Kids Meal             ", 12, 0);
    GFX_print(gfx, "Patty, Cheese         ", 13, 0);
    app->patty_r = 1;
    app->cheese_r = 1;
}

/**
 * A helper function to interpret which ingredient is select
 */
void Application_interpretIngredient(Application* app, HAL* hal)
{
    app->previousSelect = app->currentSelect;

    if ((hal->joystick.y > UP_THRESHOLD) && (hal->joystick.x < LEFT_THRESHOLD))
        app->ingredient = INGREDIENT_1;

    else if ((hal->joystick.y > UP_THRESHOLD) && (hal->joystick.x > LEFT_THRESHOLD)
            && (hal->joystick.x < RIGHT_THRESHOLD))
        app->ingredient = INGREDIENT_2;

    else if ((hal->joystick.y > UP_THRESHOLD) && (hal->joystick.x > RIGHT_THRESHOLD))
        app->ingredient = INGREDIENT_3;

    else if ((hal->joystick.y < UP_THRESHOLD) && (hal->joystick.x < LEFT_THRESHOLD))
        app->ingredient = INGREDIENT_4;

    else if ((hal->joystick.y < UP_THRESHOLD) && (hal->joystick.x > RIGHT_THRESHOLD))
        app->ingredient = INGREDIENT_5;

    else
        app->ingredient = Empty;

    app->currentSelect = (int) app->ingredient;
}

/**
 * A helper function to highlight the selected ingredient
 */
void Application_hightlightIngredient(Application* app, HAL* hal)
{
    Application_interpretIngredient(app, hal);

    // refresh frame
    if (app->currentSelect != app->previousSelect)
        Application_drawFrame(&hal->gfx);

    switch (app->ingredient)
    {
        case INGREDIENT_1:
            GFX_hightlightRectangle(&hal->gfx, X_MIN, Y_MIN, X_1, Y_1);
            break;
        case INGREDIENT_2:
            GFX_hightlightRectangle(&hal->gfx, X_1, Y_MIN, X_2, Y_1);
            break;
        case INGREDIENT_3:
            GFX_hightlightRectangle(&hal->gfx, X_2, Y_MIN, X_MAX, Y_1);
            break;
        case INGREDIENT_4:
            GFX_hightlightRectangle(&hal->gfx, X_MIN, Y_1, X_1, Y_MAX);
            break;
        case INGREDIENT_5:
            GFX_hightlightRectangle(&hal->gfx, X_2, Y_1, X_MAX, Y_MAX);
            break;
        default:
            break;
    }

}

/**
 * Adds one quantity of the highlighted ingredient when BB1 is pressed
 */
void Application_addIngredient(Application* app, HAL* hal)
{
    switch (app->ingredient)
    {
    case INGREDIENT_1:
        if (app->tomato < 3) {
            app->tomato ++;
            Application_addIndicator(app, hal, COLUMN_1_1, COLUMN_1_2, COLUMN_1_3, ROW_1, app->tomato);
        }
        break;
    case INGREDIENT_2:
        if (app->patty < 3) {
            app->patty ++;
            Application_addIndicator(app, hal, COLUMN_2_1, COLUMN_2_2, COLUMN_2_3, ROW_1, app->patty);
        }
        break;
    case INGREDIENT_3:
        if (app->pickle < 3) {
            app->pickle ++;
            Application_addIndicator(app, hal, COLUMN_3_1, COLUMN_3_2, COLUMN_3_3, ROW_1, app->pickle);
        }
        break;
    case INGREDIENT_4:
        if (app->lettuce < 3) {
            app->lettuce ++;
            Application_addIndicator(app, hal, COLUMN_1_1, COLUMN_1_2, COLUMN_1_3, ROW_2, app->lettuce);
        }
        break;
    case INGREDIENT_5:
        if (app->cheese < 3) {
            app->cheese ++;
            Application_addIndicator(app, hal, COLUMN_3_1, COLUMN_3_2, COLUMN_3_3, ROW_2, app->cheese);
        }
        break;
    default:
        break;
    }
}

/**
 * Removes one quantity of the highlighted ingredient when BB2 is pressed
 */
void Application_removeIngredient(Application* app, HAL* hal)
{
    switch (app->ingredient)
    {
    case INGREDIENT_1:
        if (app->tomato > 0) {
            app->tomato --;
            Application_removeIndicator(app, hal, COLUMN_1_1, COLUMN_1_2, COLUMN_1_3, ROW_1, app->tomato);
        }
        break;
    case INGREDIENT_2:
        if (app->patty > 0) {
            app->patty --;
            Application_removeIndicator(app, hal, COLUMN_2_1, COLUMN_2_2, COLUMN_2_3, ROW_1, app->patty);
        }
        break;
    case INGREDIENT_3:
        if (app->pickle > 0) {
            app->pickle --;
            Application_removeIndicator(app, hal, COLUMN_3_1, COLUMN_3_2, COLUMN_3_3, ROW_1, app->pickle);
        }
        break;
    case INGREDIENT_4:
        if (app->lettuce > 0) {
            app->lettuce --;
            Application_removeIndicator(app, hal, COLUMN_1_1, COLUMN_1_2, COLUMN_1_3, ROW_2, app->lettuce);
        }
        break;
    case INGREDIENT_5:
        if (app->cheese > 0) {
            app->cheese --;
            Application_removeIndicator(app, hal, COLUMN_3_1, COLUMN_3_2, COLUMN_3_3, ROW_2, app->cheese);
        }
        break;
    default:
        break;
    }
}

/**
 * Fill a indicator when ingredient is added
 *
 * @param x1:   x value of the position of the highlighted ingredient's first indicator.
 * @param x2:   x value of the position of the second indicator.
 * @param x2:   x value of the position of the third indicator.
 * @param y:    y value of the position of the indicators.
 * @param q:    current quantity of the highlighted ingredient
 */
void Application_addIndicator(Application* app, HAL* hal, int x1, int x2, int x3, int y, int q)
{
    if (q == 1)
        GFX_drawSolidCircle(&hal->gfx, x1, y, RADIUS);
    else if (q == 2)
        GFX_drawSolidCircle(&hal->gfx, x2, y, RADIUS);
    else if (q == 3)
        GFX_drawSolidCircle(&hal->gfx, x3, y, RADIUS);
}

/**
 * Remove a filled indicator when ingredient is removed
 *
 * @param x1:   x value of the position of the highlighted ingredient's first indicator.
 * @param x2:   x value of the position of the second indicator.
 * @param x2:   x value of the position of the third indicator.
 * @param y:    y value of the position of the indicators.
 * @param q:    current quantity of the highlighted ingredient
 */
void Application_removeIndicator(Application* app, HAL* hal, int x1, int x2, int x3, int y, int q)
{
    if (q == 2){
        GFX_removeSolidCircle(&hal->gfx, x3, y, RADIUS);
        GFX_drawHollowCircle(&hal->gfx, x3, y, RADIUS);
    }
    else if (q == 1) {
        GFX_removeSolidCircle(&hal->gfx, x2, y, RADIUS);
        GFX_drawHollowCircle(&hal->gfx, x2, y, RADIUS);
    }
    else if (q == 0) {
        GFX_removeSolidCircle(&hal->gfx, x1, y, RADIUS);
        GFX_drawHollowCircle(&hal->gfx, x1, y, RADIUS);
    }
}

/**
 * Compare selected ingredients to the recipe
 */
void Application_serveOrder(Application* app)
{
    if ((app->tomato == app->tomato_r) && (app->patty == app->patty_r) && (app->pickle == app->pickle_r)
            && (app->lettuce == app->lettuce_r) && (app->cheese == app->cheese_r))
        app->correctOrder = true;
}

/**
 * If an order is served incorrectly, the player gets a strike
 */
void Application_drawStrike(Application* app, GFX* gfx)
{
    if (app->strike == 1)
        GFX_print(gfx, "X", 0, 18);
    else if (app->strike == 2)
        GFX_print(gfx, "XX", 0, 18);
    else if (app->strike == 3)
        GFX_print(gfx, "XXX", 0, 18);
}

/**
 * Go back the main menu screen
 */
void Application_handleGameScreen(Application* app, HAL* hal)
{
    // When the SW timer (1s) expires, updates the timer
    if (SWTimer_expired(&app->timer)) {
        Application_updateTimer(app, &hal->gfx);
        app->timer = SWTimer_construct(COUNTDOWN_WAIT);
        SWTimer_start(&app->timer);
    }

    // Highlight the selected ingredient
    Application_hightlightIngredient(app, hal);

    // Add one quantity of selected ingredient when BB1 is pressed
    if (Button_isTapped(&hal->boosterpackS1))
        Application_addIngredient(app, hal);

    // Remove one quantity of selected ingredient when BB2 is pressed
    if (Button_isTapped(&hal->boosterpackS2))
        Application_removeIngredient(app, hal);

    // Serve the order when JSB is pressed
    if (Button_isTapped(&hal->boosterpackJS)) {
        Application_serveOrder(app);
        Application_resetIngredientNum(app);
        if (app->correctOrder == true) {
            app->income += 5;
            app->success ++;
            if (app->success % 3 == 0)
                app->countdown += 5;
        }
        else
            app->strike ++;
        Application_showGameScreen(app, &hal->gfx, hal);
        app->correctOrder = false;
    }

    //if time expires or the player receives three strikes, display the result screen
    if ((app->countdown < 0) || (app->strike == 3))
    {
        Application_rankScores(app);
        app->state = RESULT_SCREEN;
        Application_showResultScreen(app, &hal->gfx);
    }

}

/**
 * Display the result screen
 */
void Application_showResultScreen(Application* app, GFX* gfx)
{
    if ((app->income >= app->top3) && (app->income != 0))
        Graphics_drawImage(&gfx->context, &victory8BPP_UNCOMP, 0, 0);  // winner
    else
        Graphics_drawImage(&gfx->context, &gameover1BPP_UNCOMP, 0, 0);  // failure

    GFX_print(gfx, "Your final score:", 12, 2);

    char scoreString[2] = "";
    scoreString[1] = (app->income % 10) + '0';
    scoreString[0] = ((app->income / 10) % 10) + '0';
    GFX_print(gfx, scoreString, 14, 10);      // Print the final score
}

/**
 * Go back the main menu screen when JSB is pressed
 */
void Application_handleResultScreen(Application* app, HAL* hal)
{
    // When the joystick is tapped, show main menu screen
    if (Button_isTapped(&hal->boosterpackJS))
    {
        app->state = MENU_SCREEN;
        Application_showMenuScreen(app, &hal->gfx);
        Application_initGameVariables(app, hal);
    }
}

/**
 * Compare the final score with top 3 scores
 */
void Application_rankScores(Application* app) {

    if (app->income > app->top3) {
        if (app->income > app->top2) {
            if (app->income > app->top1) {
                app->top2 = app->top1;
                app->top1 = app->income;
            }
            else {
                app->top3 = app->top2;
                app->top2 = app->income;
            }
        }
        else {
            app->top3 = app->income;
        }
    }
}

/**
 * Shows High Score screen
 */
void Application_showHighScoreScreen(Application* app, GFX* gfx)
{

    GFX_clear(gfx);

    GFX_print(gfx, "High Scores           ", 0, 0);
    GFX_print(gfx, "-------------         ", 1, 0);

    GFX_print(gfx, "$ ", 2, 0);
    GFX_print(gfx, "$ ", 3, 0);
    GFX_print(gfx, "$ ", 4, 0);

    char top1String[2] = "";
    top1String[1] = (app->top1 % 10) + '0';
    top1String[0] = ((app->top1 / 10) % 10) + '0';
    GFX_print(gfx, top1String, 2, 2);      // Print the current score

    char top2String[2] = "";
    top2String[1] = (app->top2 % 10) + '0';
    top2String[0] = ((app->top2 / 10) % 10) + '0';
    GFX_print(gfx, top2String, 3, 2);

    char top3String[2] = "";
    top3String[1] = (app->top3 % 10) + '0';
    top3String[0] = ((app->top3 / 10) % 10) + '0';
    GFX_print(gfx, top3String, 4, 2);
}

/**
 * Go back the main menu screen when JSB is pressed
 */
void Application_handleHighScoreScreen(Application* app, HAL* hal)
{
    // When the joystick is tapped, show main menu screen
    if (Button_isTapped(&hal->boosterpackJS))
    {
        app->state = MENU_SCREEN;
        Application_showMenuScreen(app, &hal->gfx);
    }
}
