#include "Game.h"

GraphicsHandler* graphics = GraphicsHandler::GetInstance();
InputHandler* inputs = InputHandler::GetInstance();

SaveSystem::PlayerData* playerData = new SaveSystem::PlayerData();

bool gameRunning = true;
GameState currentState;

#pragma region General Game Functions

void GameLoop()
{
    inputs->ProcessInput();

    graphics->CheckAnimations();
    graphics->CheckRedraw();
}

void SetupInputs()
{
    inputs->AddKey(VK_LEFT, UI::NavigateUI);
    inputs->AddKey(VK_RIGHT, UI::NavigateUI);
    inputs->AddKey(VK_UP, UI::NavigateUI);
    inputs->AddKey(VK_DOWN, UI::NavigateUI);

    inputs->AddKey(VK_RETURN, UI::InteractUI);
}

#pragma endregion

void EndGame() {
    gameRunning = false;
}

void ContinueGame() 
{

}


COORD GetInputPosition()
{
    return ScreenCoord(5, graphics->windowSize.Y - 1);
}

// function that handles transitioning to new game states, including loading sprites and menu buttons
void ChangeState(GameState newState)
{
    graphics->Reset();

    currentState = newState;

    switch (currentState)
    {
    case GameState::Title:
    {
        graphics->state = GraphicsState::MENU;

        Sprite* title = graphics->LoadSprite("Title", "Title.txt", 1, ScreenCoord(1, 1));
        title->position = ScreenCoord((graphics->windowSize.X - title->length) / 2, 4);
        title->SetColours(COLOUR_BRIGHT(COLOUR_CYAN));

        Sprite* bg = graphics->LoadAnimation("Background", "Mountain.txt", 0, ScreenCoord(1, 1));
        bg->SetColours(COLOUR_BRIGHT(COLOUR_BLACK));

        // displays 'continue' button if you can load the save
        if (SaveSystem::CanLoadSave())
        {
            graphics->OrganiseButtons(graphics->GetWindowCentre() + ScreenCoord(0, 6), ScreenCoord(0, 2), std::vector<UI::Button> {
                UI::Button(ScreenCoord(1, 1), " - New Game - ", true, GameState::CharacterSelect),
                    UI::Button(ScreenCoord(1, 1), " - Continue - ", true, ContinueGame),
                    UI::Button(ScreenCoord(1, 1), " - Credits - ", true, GameState::Credits),
                    UI::Button(ScreenCoord(1, 1), " - Exit - ", true, EndGame)
            });
        }
        else
        {
            graphics->OrganiseButtons(graphics->GetWindowCentre() + ScreenCoord(0, 6), ScreenCoord(0, 2), std::vector<UI::Button> {
                UI::Button(ScreenCoord(1, 1), " - New Game - ", true, GameState::CharacterSelect),
                    UI::Button(ScreenCoord(1, 1), " - Credits - ", true, GameState::Credits),
                    UI::Button(ScreenCoord(1, 1), " - Exit - ", true, EndGame)
            });
        }

        graphics->SortSprites();
        break; 
    }
    case GameState::Credits:
    {
        graphics->state = GraphicsState::MENU;

        graphics->Reset();

        Sprite* credits = graphics->LoadSprite("Credits", "Credits.txt", 1, ScreenCoord(1, 1));
        credits->position = graphics->GetWindowCentre() - (ScreenCoord(credits->length, credits->height) / 2);

        graphics->OrganiseButtons(ScreenCoord(credits->position.X, 22), ScreenCoord(0, 0), std::vector<UI::Button>{
            UI::Button(ScreenCoord(1, 1), " - Exit - ", false, GameState::Title)
        });
        break;
    }
    case GameState::CharacterSelect:
    {
        graphics->Reset();
        graphics->state = GraphicsState::INPUT;

        graphics->LoadSprite("Characters", "Characters.txt", 1, ScreenCoord(2, 1));

        graphics->Redraw();

        std::string input;
        inputs->SetInputOptions(std::vector<std::string>{ "warrior", "wizard", "archer" });

        while (true)
        {
            input = inputs->GetInputString();

            if (input == "ERROR")
            {
                graphics->WarnInputError();
            }
            else 
            {
                // setting up player abilities
                if (input == "warrior")
                {
                    playerData->attacks.push_back(PlayerAttacks::Slash);
                    playerData->skills.push_back(PlayerSkills::Heal);
                }
                else if (input == "wizard")
                {
                    playerData->attacks.push_back(PlayerAttacks::Poke);
                    playerData->skills.push_back(PlayerSkills::Lightning);
                }
                else if (input == "archer")
                {
                    playerData->attacks.push_back(PlayerAttacks::Shoot);
                    playerData->skills.push_back(PlayerSkills::Focus);
                }

                break;
            }
        }

        graphics->Reset();
        graphics->state = GraphicsState::TEXT;

        std::string name;

        bool nameSelected = false;

        while (!nameSelected)
        {
            graphics->ClearWordCache();
            graphics->Redraw();

            graphics->WriteLine("Please enter your name:");

            name = inputs->GetRawInputString();

            if (name == "") graphics->WarnInputError();
            else 
            {
                graphics->WriteLine("You are called \033[95m" + name + "\033[0m, is this correct? (yes/no)");
                inputs->SetInputOptions(std::vector<std::string>{ "yes", "no" });

                while (!nameSelected)
                {
                    input = inputs->GetInputString();

                    if (input == "ERROR") graphics->WarnInputError();
                    else 
                    {
                        if (input == "yes")
                            nameSelected = true;
                        else
                            break;
                    }
                }
            }
        }

        playerData->name = name;

        break;
    }
    }
}

void PerformStateActions()
{
    switch (currentState)
    {

    }
}


std::clock_t GetTime()
{
    return std::clock();
}