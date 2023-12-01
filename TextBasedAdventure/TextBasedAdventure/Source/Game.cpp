#include "../Headers/Game.h"

#pragma region GameSingletons
GraphicsHandler* graphics = GraphicsHandler::GetInstance();
InputHandler* inputs = InputHandler::GetInstance();

SaveSystem::PlayerData* playerData = new SaveSystem::PlayerData();
bool gameRunning = true;

LevelData* currentLevel = new LevelData;

// variable used for a variety of purposes between levels
int randomLevelData = 0;

#pragma endregion

#if DEBUG_BUILD
void DebugLoadLevel()
{
    graphics->ClearScreen();

    std::string levelName;
    std::cout << "Level Name: ";
    std::getline(std::cin, levelName);

    LoadLevel(levelName);
}

void OpenDebugMenu()
{
    graphics->Reset();

    graphics->OrganiseButtons(graphics->GetWindowCentre(), ScreenCoord(0, 2), std::vector<UI::Button> {
        UI::Button(ScreenCoord(1, 1), " Create Dialogue Tree ", true, Debug::MakeDialogueTree),
            UI::Button(ScreenCoord(1, 1), " Edit Dialogue Tree ", true, Debug::EditDialogueTree),
            UI::Button(ScreenCoord(1, 1), " Create Level ", true, Debug::MakeLevel),
            UI::Button(ScreenCoord(1, 1), " Load Level ", true, DebugLoadLevel)
    });
}
#endif

// function called each loop, changed depending on the current scene, frequently null
void (*onLoop)();

#pragma region General Game Functions
void GameLoop()
{
    ProcessInput();

    graphics->CheckAnimations();
    graphics->CheckRedraw();

    if (onLoop != nullptr)
        ((void(*)())onLoop)();
}

void ProcessInput() { inputs->ProcessInput(); }

void SetupInputs()
{
    inputs->AddKey(VK_LEFT, UI::NavigateUI);
    inputs->AddKey(VK_RIGHT, UI::NavigateUI);
    inputs->AddKey(VK_UP, UI::NavigateUI);
    inputs->AddKey(VK_DOWN, UI::NavigateUI);

    inputs->AddKey(VK_RETURN, UI::InteractUI);

    inputs->AddKey(VK_SPACE, UI::SkipText);
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
    if (playerData->currentLocation == "forestpath")
        playerData->currentLocation = "forestentrance";

    LoadLevel(playerData->currentLocation);
}

COORD GetInputPosition()
{
    return ScreenCoord(5, graphics->windowSize.Y - 1);
}

// function that handles transitioning to new game states, including loading sprites and menu buttons
void ChangeState(GameState newState)
{
    onLoop = nullptr;

    switch (newState)
    {
        // just the basic title scene with the background and title image and menu buttons.
    case GameState::Title:
    {
        graphics->Reset();

        graphics->state = GraphicsState::MENU;

        Sprite* title = graphics->LoadSprite("Title", "UI/Title.txt", 1, graphics->GetWindowCentre() - ScreenCoord(0, 4), true, true);
        title->SetColours(COLOUR_BRIGHT(COLOUR_CYAN));

        Sprite* bg = graphics->LoadAnimation("Background", "UI/Mountain.txt", 0, ScreenCoord(1, 1));
        bg->SetColours(COLOUR_BRIGHT(COLOUR_BLACK));

#if DEBUG_BUILD
        graphics->OrganiseButtons(graphics->GetWindowCentre() + ScreenCoord(0, 6), ScreenCoord(0, 2), std::vector<UI::Button> {
            UI::Button(ScreenCoord(1, 1), " - New Game - ", true, SelectCharacter),
                UI::Button(ScreenCoord(1, 1), " - Continue - ", true, (SaveSystem::CanLoadSave()) ? ContinueGame : nullptr),
                UI::Button(ScreenCoord(1, 1), " - Credits - ", true, GameState::Credits),
                UI::Button(ScreenCoord(1, 1), " - Exit - ", true, EndGame),
                UI::Button(ScreenCoord(1, 1), " ~ DEBUG ~ ", true, OpenDebugMenu)
    });
#else
        // displays 'continue' button if you can load the save
        if (SaveSystem::CanLoadSave() && playerData->currentLocation != "")
        {
            graphics->OrganiseButtons(graphics->GetWindowCentre() + ScreenCoord(0, 6), ScreenCoord(0, 2), std::vector<UI::Button> {
                UI::Button(ScreenCoord(1, 1), " - New Game - ", true, SelectCharacter),
                    UI::Button(ScreenCoord(1, 1), " - Continue - ", true, ContinueGame),
                    UI::Button(ScreenCoord(1, 1), " - Credits - ", true, GameState::Credits),
                    UI::Button(ScreenCoord(1, 1), " - Exit - ", true, EndGame),
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
#endif

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
        graphics->Reset();

        graphics->ClearWordCache();

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

        graphics->WriteLines(lines, sizeof(lines) / sizeof(lines[0]));

        graphics->WriteLine(" - now your journey begins! remember, if you're ever lost, press type 'help' - ");

        Sleep(1000);

        LoadLevel("campfire");

        break;
    }
    case GameState::Dialogue:
    {
        graphics->Reset();
        graphics->ClearWordCache();
        graphics->state = GraphicsState::DIALOGUE;

        DrawNode();

        break;
    }
    case GameState::DefaultLevel:
    {
        graphics->Reset();
        graphics->ClearWordCache();

        graphics->AddLabel(UI::Label(currentLevel->name, ScreenCoord(graphics->GetWindowCentre().X, 2), true));
        graphics->AddDivider(3);

        onLoop = LevelCommand;

        graphics->state = GraphicsState::TEXT;

        inputs->SetInputOptions(std::vector<std::string>{ "look", "go", "talk", "back", "help" });

        graphics->WriteLine(currentLevel->description);

        graphics->WriteLine("please input commands");

        break;
    }

    case GameState::ForestEntrance:
    {
        ChangeState(GameState::DefaultLevel);

        if(playerData->flags["survivedForest"])
            graphics->WriteLine("You remember the forest more now, and you can retrace your steps out (go leave)");

        onLoop = ForestCommands;

        break;
    }
    case GameState::ForestPath:
    {
        if (graphics->state != GraphicsState::TEXT) ChangeState(GameState::DefaultLevel);

        onLoop = ForestCommands;

        graphics->WriteLine(currentLevel->description);

        graphics->WriteLine("please input commands");

        break;
    }
    case GameState::ForestExit:
    {
        ChangeState(GameState::DefaultLevel);

        playerData->flags["survivedForest"] = true;

        break;
    }
    case GameState::Town:
    {
        ChangeState(GameState::DefaultLevel);

        graphics->WriteLine("enter anything to return to the title");

        inputs->GetRawInputString();

        ChangeState(GameState::Title);

        break;
    }
    }

    SaveSystem::SavePlayerData(playerData);
}

void ForestCommands()
{
    graphics->Redraw();

    Command command = inputs->GetCommandString();

    std::string answerSheet[] = { "north", "east", "north", "west" };

    // additional movement commands
    if (command.first == "go")
    {
        if (command.second.size() < 2)
        {
            graphics->WarnInputError();
            return;
        }

        if (randomLevelData == -1)
        {
            graphics->WriteLine("You find yourself getting more and more lost in the tangled branches of the forest, until you're right back where you started. @300");
            LoadLevel("forestentrance");
            randomLevelData = 0;
            return;
        }
        else if (randomLevelData == 0 && playerData->flags["survivedForest"])
        {
            if (command.second[1] == "leave")
            {
                graphics->WriteLine("You retrace your path through the forest out into the clear air again");
                LoadLevel("forestexit");
                return;
            }
        }

        if (randomLevelData > 0)
        {
            int r = rand() % 20;

            if (r == 1 && !playerData->flags["adventurer"])
            {
                LoadDialogue("lostadventurer");
                return;
            }
            else if (r == 2)
            {
                graphics->WriteLine("A great creature, with legs that line the sides of its body like a centipede, stalks past you through the forest. @400 You feel notably more uncomfortable @300");
            }
            else if (r == 3)
            {
                graphics->WriteLine("The brittle bones of an old skeleton crunch beneath your horse with a shock");
            }
        }

        if (answerSheet[randomLevelData] == command.second[1])
        {
            randomLevelData++;

            graphics->WriteLine("*you ride " + command.second[1] + "*");

            if (randomLevelData == sizeof(answerSheet) / sizeof(answerSheet[0]))
            {
                LoadLevel("forestexit");
            }
            else 
            {
                LoadLevel("forestpath");
            }
            return;
        }
        else 
        {
            graphics->WriteLine("you can feel yourself getting lost @200");
            randomLevelData = -1;
            return;
        }
    }

    DefaultLevelCommands(&command);
}

void LoadLevel(std::string levelName)
{
    LoadLevelData(currentLevel, levelName + ".json");

    playerData->currentLocation = levelName;

    if (currentLevel->specialArea)
    {
        ChangeState((GameState)currentLevel->areaCode);
    }
    else 
    {
        ChangeState(GameState::DefaultLevel);
    }
}
void LoadCurrentLevel()
{
    LoadLevel(playerData->currentLocation);
}

void LevelCommand()
{
    graphics->Redraw();

    Command command = inputs->GetCommandString();

    DefaultLevelCommands(&command);
}
void DefaultLevelCommands(Command* c)
{
    if (c->first == "ERROR")
    {
        graphics->WarnInputError();
        return;
    }

    if (c->first == "look")
    {
        if (c->second.size() == 1)
        {
            // default look command with no parameters
            graphics->WriteLine(currentLevel->lookInfo["default"]);
            return;
        }
        else if (currentLevel->lookInfo.count(c->second[1]))
        {
            graphics->WriteLine(currentLevel->lookInfo[c->second[1]]);
            return;
        }
    }
    else if (c->first == "go")
    {
        if (c->second.size() < 2) return;

        for (int i = 0; i < currentLevel->places.size(); i++)
        {
            if (currentLevel->places[i].first == c->second[1])
            {
                LoadLevel(currentLevel->places[i].second);
                return;
            }
        }
    }
    else if (c->first == "talk")
    {
        if (c->second.size() < 2) return;

        for (int i = 0; i < currentLevel->people.size(); i++)
        {
            if (currentLevel->people[i].second == c->second[1])
            {
                LoadDialogue(currentLevel->people[i].first);
                return;
            }
        }
    }
    else if (c->first == "help")
    {
        if (c->second.size() > 1 && c->second[1] == "area")
        {
            graphics->WriteLine(currentLevel->helptext);

            return;
        }
        else
        {
            std::string helpLines[] = {
                "look [something] - take a closer look at something specific, @300 if you input without a specific subject your character will have a general look around",
                "go [place] - go somewhere to do something",
                "talk [person] - talk to someone",
                "help - open this menu",
                "help area - gives a specific hint for your current location"
            };

            graphics->WriteLines(helpLines, 5);

            return;
        }
    }
    else if (c->first == "back")
    {
        if (currentLevel->lastLevelName != "")
        {
            LoadLevel(currentLevel->lastLevelName);
            return;
        }

        graphics->WriteLine("you cant go back now");
        return;
    }

    graphics->WarnInputError();
}

void LoadDialogue(std::string characterName)
{
    currentTree->Clear();
    currentTree->LoadDialogueTree(characterName + ".json");
    ChangeState(GameState::Dialogue);
}

// the process of choosing a character class and naming yourself
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
            playerData->characterClass = input;

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

    graphics->WriteLine("you can use space to skip likes, but use it sparingly - you dont want to miss any information");

    playerData->name = name;

    ChangeState(GameState::OpeningCutscene);
}

void SetPlayerFlags(std::string flag, bool value)
{
    playerData->flags[flag] = value;
}