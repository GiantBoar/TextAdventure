#include "../Headers/Game.h"

#pragma region GameSingletons
GraphicsHandler* graphics = GraphicsHandler::GetInstance();
InputHandler* inputs = InputHandler::GetInstance();

SaveSystem::PlayerData* playerData = new SaveSystem::PlayerData();
bool gameRunning = true;

#pragma endregion

// function called each loop, changed depending on the current scene, frequently null
void (*onLoop)();

#pragma region General Game Functions
void GameLoop()
{
    inputs->ProcessInput();

    graphics->CheckAnimations();
    graphics->CheckRedraw();

    if (onLoop != nullptr)
        ((void(*)())onLoop)();
}

void SetupInputs()
{
    inputs->AddKey(VK_LEFT, UI::NavigateUI);
    inputs->AddKey(VK_RIGHT, UI::NavigateUI);
    inputs->AddKey(VK_UP, UI::NavigateUI);
    inputs->AddKey(VK_DOWN, UI::NavigateUI);

    inputs->AddKey(VK_RETURN, UI::InteractUI);
}

std::clock_t GetTime()
{
    return std::clock();
}
#pragma endregion

void EndGame() {
    gameRunning = false;
}

void ContinueGame() 
{
    SaveSystem::LoadPlayerData(playerData);
    ChangeState(playerData->currentLocation);
}


COORD GetInputPosition()
{
    return ScreenCoord(5, graphics->windowSize.Y - 1);
}

// function that handles transitioning to new game states, including loading sprites and menu buttons
void ChangeState(GameState newState)
{
    graphics->Reset();

    onLoop = nullptr;

    switch (newState)
    {
        // just the basic title scene with the background and title image and menu buttons.
    case GameState::Title:
    {
        graphics->state = GraphicsState::MENU;

        Sprite* title = graphics->LoadSprite("Title", "UI/Title.txt", 1, graphics->GetWindowCentre() - ScreenCoord(0, 4), true, true);
        title->SetColours(COLOUR_BRIGHT(COLOUR_CYAN));

        Sprite* bg = graphics->LoadAnimation("Background", "UI/Mountain.txt", 0, ScreenCoord(1, 1));
        bg->SetColours(COLOUR_BRIGHT(COLOUR_BLACK));

        // displays 'continue' button if you can load the save
        if (SaveSystem::CanLoadSave())
        {
            graphics->OrganiseButtons(graphics->GetWindowCentre() + ScreenCoord(0, 6), ScreenCoord(0, 2), std::vector<UI::Button> {
                UI::Button(ScreenCoord(1, 1), " - New Game - ", true, SelectCharacter),
                    UI::Button(ScreenCoord(1, 1), " - Continue - ", true, ContinueGame),
                    UI::Button(ScreenCoord(1, 1), " - Credits - ", true, GameState::Credits),
                    UI::Button(ScreenCoord(1, 1), " - Exit - ", true, EndGame)
            });
        }
        else
        {
            graphics->OrganiseButtons(graphics->GetWindowCentre() + ScreenCoord(0, 6), ScreenCoord(0, 2), std::vector<UI::Button> {
                UI::Button(ScreenCoord(1, 1), " - New Game - ", true, SelectCharacter),
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

        Sprite* credits = graphics->LoadSprite("Credits", "UI/Credits.txt", 1, graphics->GetWindowCentre(), true, true);

        graphics->OrganiseButtons(ScreenCoord(credits->position.X, 22), ScreenCoord(0, 0), std::vector<UI::Button>{
            UI::Button(ScreenCoord(1, 1), " - Exit - ", false, GameState::Title)
        });
        break;
    }
    case GameState::OpeningCutscene:
    {
        playerData->currentLocation = GameState::OpeningCutscene;
        SaveSystem::SavePlayerData(playerData);

        graphics->state = GraphicsState::TEXT;

        std::string lines[] = {
            "@1000 Far east, @200 a small town sits against the stubborn country rock.",
            "The people pray at a small alter, @200 past the border of the forest and shaded by tall dark trees,",
            "and their humble god churns the earth and rewards their worship with plump carrots and fresh water.",
            "The simple town breathed, @250 and the simple people lived and died and buried their dead.",
            "But one day @100 from a tall wooden tower on the border of the village, @200 smoke was sighted,",
            "smoke that writhed @100 and wriggled @100 and fought the sky for dominance.",
            "When the smoke reached the town, @200 the source trudged forwards,",
            "a " + GraphicsHandler::ColourString("Rider", COLOUR_RED) + ", clad in soot black armor that billowed thick smoke at every seam,",
            "mounted upon a bony, @150 scarred horse, @150 who's hooves cracked and splintered the earth beneath them.",
            "The town's strongest, @250 it's fathers, @100 and mothers, @100 and workers @100 all met the rider at the entrance to the town.",
            "In a flash of heat and a heave of it's great, @150 black @150 blade, @250 the rider carved its path through,",
            "rending arm from shoulder, @200 hips from chest.",
            "The rider continued through the town, @240 leaving charred earth and pollution in its stead.",
            "Their humble shrine, @150 made from wood @150 and whalebone @150 and heaped up shale @100 was no match,",
            "and for what was left of the town, @200 no god churned their soil @100 and gifted them water.",
            "Through their stubborn country rock carved a deep, " + GraphicsHandler::ColourString("rotten", COLOUR_GREEN) + " passage into the land beyond.",
            "A young soldier arose, @150 born from the town and newly orphaned in the rider's wake, @150 atop an old horse, @150 cut loose and mounted from a toppled trading wagon.",
            GraphicsHandler::ColourString(playerData->name, COLOUR_BRIGHT(COLOUR_PURPLE)) + " followed the rider's passage, both of you in search of " + GraphicsHandler::ColourString("The Mountain", COLOUR_BRIGHT(COLOUR_CYAN))
        };

        graphics->WriteLines(lines, sizeof(lines) / sizeof(lines[0]), 400);

        break;
    }
    }
}

// the process of choosing 
void SelectCharacter()
{
    graphics->Reset();
    graphics->state = GraphicsState::INPUT;

    /*
    graphics->LoadSprite("warrior", "WarriorCard.txt", 1, ScreenCoord(3, 3))->SetColours(COLOUR_BRIGHT(COLOUR_RED));
    graphics->LoadSprite("wizard", "WizardCard.txt", 1, ScreenCoord(42, 3))->SetColours(COLOUR_BRIGHT(COLOUR_CYAN));
    graphics->LoadSprite("archer", "ArcherCard.txt", 1, ScreenCoord(81, 3))->SetColours(COLOUR_BRIGHT(COLOUR_GREEN));
    */

    graphics->LoadSprite("warrior", "WarriorCard.txt", 1, ScreenCoord(3, 3));
    graphics->LoadSprite("wizard", "WizardCard.txt", 1, ScreenCoord(42, 3));
    graphics->LoadSprite("archer", "ArcherCard.txt", 1, ScreenCoord(81, 3));

    graphics->AddLabel(UI::Label("~ Choose a Character ~", graphics->GetWindowCentre() - ScreenCoord(0, 13), true));

    graphics->Redraw();

    std::string input; 
    std::vector<std::string> classes{ "warrior", "wizard", "archer" };
    inputs->SetInputOptions(classes);

    while (true)
    {
        input = inputs->GetInputString();

        if (input == "ERROR" || input == "")
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

    for (int i = 0; i < classes.size(); i++)
    {
        if (classes[i] != input) graphics->GetSprite(classes[i])->SetColours(COLOUR_GREY);
    }

    graphics->Redraw();

    Sprite* icon = graphics->GetSprite(input);

    int weights[] = { 0, 1, 0, -1 };
    ScreenCoord cachePos = icon->position;

    for (int i = 0; i < 10; i++)
    {
        icon->position = cachePos + ScreenCoord(0, weights[i % 4]);
        graphics->Redraw();
        Sleep(250);
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

    ChangeState(GameState::OpeningCutscene);
}