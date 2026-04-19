#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Header.h"
#include "plants.h"
#include "zombies.h"
#include "levels.h"
#include "sun.h"
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <memory>

using namespace sf;
using namespace std;

SunFactory* gSunFactory = nullptr;

class Game
{
public:
    Game() {}

    void run()
    {
        bool levelJustStarted = false;
        bool pendingLevel2Setup = false;
        bool waitingAfterLevel2Bg = false;
        bool pendingLevel3Setup = false;
        bool waitingAfterLevel3Bg = false;
        bool transitioningToLevel2 = false;
        bool showLevel2TransitionMsg = false;
        bool transitioningToLevel3 = false;
        bool showLevel3TransitionMsg = false;
        Clock levelStartClock;
        Clock waitLevel2BgClock;
        Clock waitLevel3BgClock;
        Clock level2TransitionMsgClock;
        Clock level3TransitionMsgClock;

    main_menu:
        {
            Texture homeTexture;
            if (!homeTexture.loadFromFile("PVZ/backgrounds/homescreenpvz.jpg"))
            {
                cout << "Failed to load homescreen texture!" << endl;
            }
            Sprite homeSprite(homeTexture);

            RenderWindow tempWindow(VideoMode(1366, 768), "Plants VS Zombies by MTQ & IBRAHIM");
            tempWindow.clear();
            tempWindow.draw(homeSprite);
            tempWindow.display();

            Clock splashClock;
            while (splashClock.getElapsedTime().asSeconds() < 2.f)
            {
                Event tempEvent;
                while (tempWindow.pollEvent(tempEvent))
                {
                    if (tempEvent.type == Event::Closed)
                    {
                        tempWindow.close();
                        return;
                    }
                }
            }
            tempWindow.close();
        }

        // Main menu logic
        {
            RenderWindow mainMenuWindow(VideoMode(1366, 768), "Plants VS Zombies by MTQ & IBRAHIM");
            Texture menuTexture;
            if (!menuTexture.loadFromFile("PVZ/backgrounds/mainmenu.jpg"))
            {
                cout << "Failed to load main menu texture!" << endl;
            }
            Sprite menuBackground(menuTexture);

            float scaleX = 1366.0f / static_cast<float>(menuTexture.getSize().x);
            float scaleY = 768.0f / static_cast<float>(menuTexture.getSize().y);
            menuBackground.setScale(scaleX, scaleY);

            Font font;
            if (!font.loadFromFile("PVZ/Fonts/Arial.ttf"))
            {
                if (!font.loadFromFile("PVZ/Fonts/Samdan.TTF"))
                {
                    cout << "Failed to load font!" << endl;
                }
            }

            Text title("Zombie WarFrame", font, 60);
            title.setFillColor(Color::White);

            Text startGameText("Start Game", font, 40);
            Text instructionsText("Instructions", font, 40);
            Text exitText("Exit", font, 40);

            auto centerTextX = [&](Text& txt, float y)
                {
                    float textWidth = txt.getLocalBounds().width;
                    txt.setPosition((1366 - textWidth) / 2.f, y);
                };

            centerTextX(title, 120.f);
            centerTextX(startGameText, 300.f);
            centerTextX(instructionsText, 380.f);
            centerTextX(exitText, 460.f);

            bool inInstructions = false;

            Text instructionsHeading("How to Play", font, 50);
            instructionsHeading.setFillColor(Color::White);
            centerTextX(instructionsHeading, 180.f);

            Text instructionsBody(
                "1) Collect sun to plant.\n2) Place plants to defeat zombies.\n3) Don't let zombies cross!",
                font,
                28
            );
            instructionsBody.setFillColor(Color::White);
            instructionsBody.setPosition(200.f, 280.f);

            Text backText("Back", font, 40);

            while (mainMenuWindow.isOpen())
            {
                Vector2i mousePos = Mouse::getPosition(mainMenuWindow);
                float mx = static_cast<float>(mousePos.x);
                float my = static_cast<float>(mousePos.y);

                Event ev;
                while (mainMenuWindow.pollEvent(ev))
                {
                    if (ev.type == Event::Closed)
                    {
                        mainMenuWindow.close();
                        return;
                    }
                    else if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left)
                    {
                        if (inInstructions)
                        {
                            centerTextX(backText, 500.f);
                            FloatRect backRect(backText.getGlobalBounds());
                            if (backRect.contains(mx, my))
                            {
                                inInstructions = false;
                            }
                        }
                        else
                        {
                            FloatRect startRect(startGameText.getGlobalBounds());
                            FloatRect instrRect(instructionsText.getGlobalBounds());
                            FloatRect exitRect(exitText.getGlobalBounds());

                            if (startRect.contains(mx, my))
                            {
                                mainMenuWindow.close();
                            }
                            else if (instrRect.contains(mx, my))
                            {
                                inInstructions = true;
                            }
                            else if (exitRect.contains(mx, my))
                            {
                                mainMenuWindow.close();
                                return;
                            }
                        }
                    }
                }

                if (inInstructions)
                {
                    centerTextX(backText, 500.f);
                    if (backText.getGlobalBounds().contains(mx, my))
                        backText.setFillColor(Color::Yellow);
                    else
                        backText.setFillColor(Color::White);
                }
                else
                {
                    if (startGameText.getGlobalBounds().contains(mx, my))
                        startGameText.setFillColor(Color::Yellow);
                    else
                        startGameText.setFillColor(Color::White);

                    if (instructionsText.getGlobalBounds().contains(mx, my))
                        instructionsText.setFillColor(Color::Yellow);
                    else
                        instructionsText.setFillColor(Color::White);

                    if (exitText.getGlobalBounds().contains(mx, my))
                        exitText.setFillColor(Color::Yellow);
                    else
                        exitText.setFillColor(Color::White);
                }

                mainMenuWindow.clear();
                mainMenuWindow.draw(menuBackground);

                if (inInstructions)
                {
                    mainMenuWindow.draw(instructionsHeading);
                    mainMenuWindow.draw(instructionsBody);
                    mainMenuWindow.draw(backText);
                }
                else
                {
                    mainMenuWindow.draw(title);
                    mainMenuWindow.draw(startGameText);
                    mainMenuWindow.draw(instructionsText);
                    mainMenuWindow.draw(exitText);
                }

                mainMenuWindow.display();
            }
        }

    game_restart:
        GameLevel currentLevel = LEVEL_1;
        std::unique_ptr<Levels> level = std::make_unique<Levels>(currentLevel);

        RenderWindow window(VideoMode(1366, 768), "Plants VS Zombies by MTQ & IBRAHIM");

        Texture bgTexture, peashooter, sunflower, snowpea, wallnut, cherrybomb, repeater;
        bgTexture.loadFromFile("PVZ/backgrounds/level1.png");
        peashooter.loadFromFile("PVZ/Plants/card_peashooter.png");
        snowpea.loadFromFile("PVZ/Plants/card_snowpea.png");
        wallnut.loadFromFile("PVZ/Plants/card_wallnut.png");
        cherrybomb.loadFromFile("PVZ/Plants/card_cherrybomb.png");
        repeater.loadFromFile("PVZ/Plants/card_repeaterpea.png");
        sunflower.loadFromFile("PVZ/Plants/card_sunflower.png");

        Sprite peashooterSprite(peashooter);
        Sprite sunflowerSprite(sunflower);
        Sprite repeaterSprite(repeater);
        Sprite snowpeaSprite(snowpea);
        Sprite cherrybombSprite(cherrybomb);
        Sprite wallnutSprite(wallnut);

        const int Rows = 5;
        const int Cols = 9;
        const int Cell_Size_X = 80;
        const int Cell_Size_Y = 98;
        int Field_Status[Rows][Cols] = { false };

        const int Diversion_X = 260;
        const int Diversion_Y = 174;

        static SunFactory sunFactory;
        gSunFactory = &sunFactory;

        Texture menuTexture;
        menuTexture.loadFromFile("PVZ/backgrounds/mainmenu.jpg");
        Sprite menuBackground(menuTexture);
        float scaleX = 1366.0f / static_cast<float>(menuTexture.getSize().x);
        float scaleY = 768.0f / static_cast<float>(menuTexture.getSize().y);
        menuBackground.setScale(scaleX, scaleY);

        Texture borderTexture;
        borderTexture.loadFromFile("PVZ/backgrounds/border.png");
        Sprite upperBorder(borderTexture), lowerBorder(borderTexture);

        float borderWidth = 1366.0f;
        float borderHeight = 90.0f;

        upperBorder.setScale(borderWidth / borderTexture.getSize().x, borderHeight / borderTexture.getSize().y);
        upperBorder.setPosition(0, -6);
        lowerBorder.setScale(borderWidth / borderTexture.getSize().x, borderHeight / borderTexture.getSize().y);
        lowerBorder.setPosition(0, 768 - borderHeight);

        Texture barTexture;
        barTexture.loadFromFile("PVZ/backgrounds/bar.png");
        Sprite barSprite(barTexture);

        float newBarWidth = 510.0f;
        float newBarHeight = 100.0f;
        float barMarginLeft = 30.0f;
        float barMarginBottom = 18.0f + 70.0f;
        float newBarX = barMarginLeft;
        float newBarY = 768 - borderHeight - newBarHeight + barMarginBottom;

        barSprite.setScale(newBarWidth / barTexture.getSize().x, newBarHeight / barTexture.getSize().y);
        barSprite.setPosition(newBarX, newBarY);

        Texture sunCounterTexture;
        if (!sunCounterTexture.loadFromFile("PVZ/backgrounds/suncounter.png")) {
            cout << "Failed to load suncounter.png" << endl;
        }
        float sunCounterW = 70.0f;
        float sunCounterH = 30.0f;
        Sprite sunCounterSprite(sunCounterTexture);
        sunCounterSprite.setScale(sunCounterW / sunCounterTexture.getSize().x, sunCounterH / sunCounterTexture.getSize().y);

        std::vector<FloatRect> cardSlots;
        float barImageWidth = 402.0f;
        float barImageHeight = 88.0f;
        float sunAreaWidth = 72.0f;
        float slotWidth = (barImageWidth - sunAreaWidth) / 6.0f;
        float slotHeight = barImageHeight;

        float newScaleX = newBarWidth / barImageWidth;
        float newScaleY = newBarHeight / barImageHeight;

        float slotDrawWidth = slotWidth * newScaleX;
        float slotDrawHeight = slotHeight * newScaleY;
        float slotStartX = newBarX + sunAreaWidth * newScaleX;
        float slotY = newBarY;

        for (int i = 0; i < 6; ++i) {
            cardSlots.emplace_back(slotStartX + i * slotDrawWidth, slotY, slotDrawWidth, slotDrawHeight);
        }

        Texture healthBarTexture;
        if (!healthBarTexture.loadFromFile("PVZ/backgrounds/CRAZYDAVEHEALTHBAR.png")) {
            cout << "Failed to load health bar texture!" << endl;
        }
        Sprite healthBarSprite(healthBarTexture);
        float healthBarDisplayWidth = static_cast<float>(healthBarTexture.getSize().x);
        float healthBarDisplayHeight = static_cast<float>(healthBarTexture.getSize().y);
        float healthBarMargin = 24.0f;

        float healthBarScale = 0.6f;
        healthBarSprite.setScale(healthBarScale, healthBarScale);

        float scaledWidth = healthBarDisplayWidth * healthBarScale;
        float scaledHeight = healthBarDisplayHeight * healthBarScale;
        float healthBarX = 1366.0f - scaledWidth - healthBarMargin - 60.0f;
        float healthBarY = 768.0f - scaledHeight - 0.5f * (90.0f - scaledHeight);
        healthBarSprite.setPosition(healthBarX, healthBarY);

        Font zombieshFont;
        if (!zombieshFont.loadFromFile("PVZ/Fonts/Zombiesh.ttf")) {
            if (!zombieshFont.loadFromFile("PVZ/Fonts/Samdan.TTF")) {
                cout << "Failed to load zombiesh font!" << endl;
            }
        }

        Color darkBrown(60, 40, 20);

        Text levelText("Level 1", zombieshFont, 54);
        levelText.setFillColor(darkBrown);
        levelText.setOutlineColor(Color::White);
        levelText.setOutlineThickness(3.f);
        levelText.setStyle(Text::Bold);
        levelText.setPosition((1366 - levelText.getLocalBounds().width) / 2.f, 12.f);

        Text livesText("", zombieshFont, 46);
        livesText.setFillColor(darkBrown);
        livesText.setOutlineColor(Color::White);
        livesText.setOutlineThickness(2.5f);
        livesText.setStyle(Text::Bold);

        Text optionsText("Options", zombieshFont, 40);
        optionsText.setFillColor(darkBrown);
        optionsText.setOutlineColor(Color::White);
        optionsText.setOutlineThickness(2.5f);
        optionsText.setStyle(Text::Bold);
        float margin = 20.0f;
        optionsText.setPosition(1366 - optionsText.getLocalBounds().width - margin, margin);

        Text sunText;
        sunText.setFont(zombieshFont);
        sunText.setCharacterSize(22);
        sunText.setFillColor(Color::Black);
        sunText.setOutlineColor(Color::White);
        sunText.setOutlineThickness(1.2f);
        sunText.setStyle(Text::Bold);

        Font font = zombieshFont;

        Text resumeText("Resume", font, 48);
        Text pauseInstructionsText("Instructions", font, 48);
        Text quitText("Quit", font, 48);

        Text instructionsHeading("How to Play", font, 50);
        instructionsHeading.setFillColor(Color::White);
        float instrHeadY = 180.f;
        instructionsHeading.setPosition((1366 - instructionsHeading.getLocalBounds().width) / 2.f, instrHeadY);

        Text instructionsBody(
            "1) Collect sun to plant.\n2) Place plants to defeat zombies.\n3) Don't let zombies cross!",
            font,
            28
        );
        instructionsBody.setFillColor(Color::White);
        instructionsBody.setPosition(200.f, 280.f);

        Text backText("Back", font, 40);

        // Transition messages: centralized and white
        Text transitionMsg("Congratulations on clearing level 1\nMoving to level 2...", font, 48);
        transitionMsg.setFillColor(Color::White);
        transitionMsg.setOutlineColor(Color::Black);
        transitionMsg.setOutlineThickness(3.f);
        transitionMsg.setStyle(Text::Bold);
        transitionMsg.setPosition(
            (1366 - transitionMsg.getLocalBounds().width) / 2.f,
            (768 - transitionMsg.getLocalBounds().height) / 2.f
        );
        transitionMsg.setOrigin(0, 0);

        Text transitionMsg3("Congratulations on clearing level 2\nMoving to level 3...", font, 48);
        transitionMsg3.setFillColor(Color::White);
        transitionMsg3.setOutlineColor(Color::Black);
        transitionMsg3.setOutlineThickness(3.f);
        transitionMsg3.setStyle(Text::Bold);
        transitionMsg3.setPosition(
            (1366 - transitionMsg3.getLocalBounds().width) / 2.f,
            (768 - transitionMsg3.getLocalBounds().height) / 2.f
        );
        transitionMsg3.setOrigin(0, 0);

        auto centerTransitionMsg = [&]() {
            FloatRect msgBounds = transitionMsg.getLocalBounds();
            float msgX = (1366 - msgBounds.width) / 2.f - msgBounds.left;
            float msgY = (768 - msgBounds.height) / 2.f - msgBounds.top;
            transitionMsg.setPosition(msgX, msgY);
            };
        centerTransitionMsg();

        auto centerTransitionMsg3 = [&]() {
            FloatRect msgBounds = transitionMsg3.getLocalBounds();
            float msgX = (1366 - msgBounds.width) / 2.f - msgBounds.left;
            float msgY = (768 - msgBounds.height) / 2.f - msgBounds.top;
            transitionMsg3.setPosition(msgX, msgY);
            };
        centerTransitionMsg3();

        Clock gridClock;
        int index = 0;
        int sunsCollected = 500;
        int lives = 3;

        bool paused = false;
        bool showPauseInstructions = false;
        int selectedCard = -1;

        GameLevel gameLevel = LEVEL_1;

        while (window.isOpen())
        {
            if (showLevel2TransitionMsg)
            {
                window.clear();
                window.draw(menuBackground);
                centerTransitionMsg();
                window.draw(transitionMsg);
                window.display();

                Event ev;
                while (window.pollEvent(ev))
                {
                    if (ev.type == Event::Closed)
                    {
                        window.close();
                        return;
                    }
                }

                if (level2TransitionMsgClock.getElapsedTime().asSeconds() > 3.0f)
                {
                    showLevel2TransitionMsg = false;
                    pendingLevel2Setup = true;
                }
                continue;
            }

            if (showLevel3TransitionMsg)
            {
                window.clear();
                window.draw(menuBackground);
                centerTransitionMsg3();
                window.draw(transitionMsg3);
                window.display();

                Event ev;
                while (window.pollEvent(ev))
                {
                    if (ev.type == Event::Closed)
                    {
                        window.close();
                        return;
                    }
                }

                if (level3TransitionMsgClock.getElapsedTime().asSeconds() > 3.0f)
                {
                    showLevel3TransitionMsg = false;
                    pendingLevel3Setup = true;
                }
                continue;
            }

            // Do the actual Level 2 setup after the transition message is gone
            if (pendingLevel2Setup)
            {
                gameLevel = LEVEL_2;
                level = std::make_unique<Levels>(LEVEL_2);
                bgTexture.loadFromFile("PVZ/backgrounds/level2.png");
                levelText.setString("Level 2");
                levelText.setPosition((1366 - levelText.getLocalBounds().width) / 2.f, 12.f);
                sunsCollected = 1000;
                lives = 3;
                index = 0;
                memset(Field_Status, 0, sizeof(Field_Status));
                levelJustStarted = false;
                waitLevel2BgClock.restart();
                waitingAfterLevel2Bg = true;
                pendingLevel2Setup = false;
                continue;
            }

            // Do the actual Level 3 setup after the transition message is gone
            if (pendingLevel3Setup)
            {
                gameLevel = static_cast<GameLevel>(3);
                level = std::make_unique<Levels>(gameLevel);
                bgTexture.loadFromFile("PVZ/backgrounds/level3.png");
                levelText.setString("Level 3");
                levelText.setPosition((1366 - levelText.getLocalBounds().width) / 2.f, 12.f);
                sunsCollected = 1000;
                lives = 3;
                index = 0;
                memset(Field_Status, 0, sizeof(Field_Status));
                levelJustStarted = false;
                waitLevel3BgClock.restart();
                waitingAfterLevel3Bg = true;
                pendingLevel3Setup = false;
                continue;
            }

            // Wait 1 second after Level 2 background is shown, then start logic
            if (waitingAfterLevel2Bg)
            {
                window.clear();
                createBack(window, bgTexture);
                window.draw(levelText);
                window.display();

                if (waitLevel2BgClock.getElapsedTime().asSeconds() > 1.0f)
                {
                    levelJustStarted = true;
                    levelStartClock.restart();
                    waitingAfterLevel2Bg = false;
                }
                continue;
            }

            // Wait 1 second after Level 3 background is shown, then start logic
            if (waitingAfterLevel3Bg)
            {
                window.clear();
                createBack(window, bgTexture);
                window.draw(levelText);
                window.display();

                if (waitLevel3BgClock.getElapsedTime().asSeconds() > 1.0f)
                {
                    levelJustStarted = true;
                    levelStartClock.restart();
                    waitingAfterLevel3Bg = false;
                }
                continue;
            }

            if (transitioningToLevel2)
            {
                showLevel2TransitionMsg = true;
                level2TransitionMsgClock.restart();
                transitioningToLevel2 = false;
                continue;
            }

            if (transitioningToLevel3)
            {
                showLevel3TransitionMsg = true;
                level3TransitionMsgClock.restart();
                transitioningToLevel3 = false;
                continue;
            }

            if (levelJustStarted) {
                if (levelStartClock.getElapsedTime().asSeconds() < 1.5f) {
                    gSunFactory->draw(window);
                    level->moveZombies(window, Diversion_Y, Cell_Size_Y, lives);
                    window.display();
                    continue;
                }
                else {
                    levelJustStarted = false;
                }
            }

            window.clear();
            createBack(window, bgTexture);

            window.draw(upperBorder);
            window.draw(lowerBorder);
            window.draw(barSprite);

            float sunCounterX = newBarX + 9.0f + 3.0f;
            float sunCounterY = newBarY + newBarHeight - sunCounterH - 3.0f;
            sunCounterSprite.setPosition(sunCounterX, sunCounterY);
            window.draw(sunCounterSprite);

            sunText.setString(std::to_string(sunsCollected));
            FloatRect sunTextBounds = sunText.getLocalBounds();
            float sunTextX = sunCounterX + (sunCounterW - sunTextBounds.width) / 2.0f - sunTextBounds.left;
            float sunTextY = sunCounterY + (sunCounterH - sunTextBounds.height) / 2.0f - sunTextBounds.top;
            sunText.setPosition(sunTextX, sunTextY);
            window.draw(sunText);

            window.draw(levelText);

            livesText.setString("Lives: " + std::to_string(lives));
            livesText.setPosition(30.f, 18.f);
            window.draw(livesText);

            optionsText.setPosition(1366 - optionsText.getLocalBounds().width - margin, margin);
            window.draw(optionsText);

            // Card selection logic per level
            std::vector<Sprite> cardSprites;
            std::vector<int> availablePlants;
            if (gameLevel == LEVEL_1)
                availablePlants = { 1, 2 };
            else if (gameLevel == LEVEL_2)
                availablePlants = { 1, 2, 3, 4 };
            else // LEVEL_3
                availablePlants = { 1, 2, 3, 4, 5, 6 };

            for (int plantType : availablePlants)
            {
                switch (plantType)
                {
                case 1: cardSprites.push_back(peashooterSprite); break;
                case 2: cardSprites.push_back(sunflowerSprite); break;
                case 3: cardSprites.push_back(snowpeaSprite); break;
                case 4: cardSprites.push_back(repeaterSprite); break;
                case 5: cardSprites.push_back(wallnutSprite); break;
                case 6: cardSprites.push_back(cherrybombSprite); break;
                }
            }

            for (size_t i = 0; i < cardSprites.size() && i < cardSlots.size(); ++i) {
                Sprite& card = cardSprites[i];
                FloatRect slot = cardSlots[i];

                card.setScale(1.f, 1.f);
                card.setColor(Color::White);

                FloatRect bounds = card.getLocalBounds();
                float scaleW = slot.width / bounds.width;
                float scaleH = slot.height / bounds.height;
                float scale = std::min(scaleW, scaleH) * 0.92f;

                card.setScale(scale, scale);

                float cardW = bounds.width * scale;
                float cardH = bounds.height * scale;
                float cardX = slot.left + (slot.width - cardW) / 2.0f;
                float cardY = slot.top + (slot.height - cardH) / 2.0f;
                card.setPosition(cardX, cardY);

                Vector2i mousePos = Mouse::getPosition(window);
                float mx = static_cast<float>(mousePos.x);
                float my = static_cast<float>(mousePos.y);

                if (slot.contains(mx, my)) {
                    card.setColor(Color(255, 255, 128));
                }
                if (static_cast<int>(i) == selectedCard) {
                    card.setColor(Color(200, 255, 200));
                }

                window.draw(card);
            }

            window.draw(healthBarSprite);

            float barSrcX = 87.0f, barSrcY = 17.0f, barW = 417.0f, barH = 40.0f;
            float barDrawX = healthBarX + barSrcX * healthBarScale;
            float barDrawY = healthBarY + barSrcY * healthBarScale;
            float barDrawW = barW * healthBarScale, barDrawH = barH * healthBarScale;

            float frac = 1.0f;
            Color color = Color::Green;
            if (lives == 2) { frac = 2.0f / 3.0f; color = Color(255, 215, 0); }
            else if (lives == 1) { frac = 1.0f / 3.0f; color = Color::Red; }

            float healthBarLeft = barDrawX;
            RectangleShape healthRect(Vector2f(barDrawW * frac, barDrawH));
            healthRect.setPosition(healthBarLeft, barDrawY);
            healthRect.setFillColor(color);
            window.draw(healthRect);

            Event event;
            while (window.pollEvent(event))
            {
                if (lives <= 0 || event.type == Event::Closed
                    || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
                {
                    window.close();
                }
                else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left &&
                    optionsText.getGlobalBounds().contains(static_cast<float>(Mouse::getPosition(window).x), static_cast<float>(Mouse::getPosition(window).y)))
                    || (event.type == Event::KeyPressed && (event.key.code == Keyboard::P)))
                {
                    paused = true;
                    showPauseInstructions = false;
                }
                else if (paused)
                {
                    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                    {
                        Vector2i mousePos = Mouse::getPosition(window);
                        float mx = static_cast<float>(mousePos.x);
                        float my = static_cast<float>(mousePos.y);

                        if (!showPauseInstructions)
                        {
                            float centerX = 1366 / 2.f;
                            float startY = 260.0f;
                            float spacing = 80.0f;
                            resumeText.setPosition(centerX - resumeText.getLocalBounds().width / 2, startY);
                            pauseInstructionsText.setPosition(centerX - pauseInstructionsText.getLocalBounds().width / 2, startY + spacing);
                            quitText.setPosition(centerX - quitText.getLocalBounds().width / 2, startY + 2 * spacing);

                            if (resumeText.getGlobalBounds().contains(mx, my))
                            {
                                paused = false;
                            }
                            else if (pauseInstructionsText.getGlobalBounds().contains(mx, my))
                            {
                                showPauseInstructions = true;
                            }
                            else if (quitText.getGlobalBounds().contains(mx, my))
                            {
                                window.close();
                            }
                        }
                        else
                        {
                            backText.setPosition(1366 / 2.f - backText.getLocalBounds().width / 2, 600.f);
                            if (backText.getGlobalBounds().contains(mx, my))
                            {
                                showPauseInstructions = false;
                            }
                        }
                    }
                }
                else if (!paused && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    int mouseX = Mouse::getPosition(window).x;
                    int mouseY = Mouse::getPosition(window).y;

                    bool sunCollected = false;
                    for (int i = 0; i < 10; i++)
                    {
                        if (gSunFactory->suns[i].handleClick(static_cast<float>(mouseX), static_cast<float>(mouseY)))
                        {
                            sunsCollected += 50;
                            sunCollected = true;
                            break;
                        }
                    }
                    if (sunCollected)
                        continue;

                    for (size_t i = 0; i < cardSprites.size(); ++i) {
                        if (cardSlots[i].contains(mouseX, mouseY)) {
                            selectedCard = static_cast<int>(i);
                        }
                    }

                    if (selectedCard != -1) {
                        int tempX = (mouseX - Diversion_X);
                        int tempY = (mouseY - Diversion_Y);

                        if (tempX > 0 && tempY > 0) {
                            int gridX = (mouseX - Diversion_X) / Cell_Size_X;
                            int gridY = (mouseY - Diversion_Y) / Cell_Size_Y;

                            if ((gridX >= 0 && gridX < Cols && gridY >= 0 && gridY < Rows)
                                && (!Field_Status[gridY][gridX]))
                            {
                                int type = availablePlants[selectedCard];
                                int plantCost = 0;
                                if (type == 2) plantCost = 50;
                                else if (type == 1) plantCost = 100;
                                else if (type == 3) plantCost = 175;
                                else if (type == 4) plantCost = 200;
                                else if (type == 5) plantCost = 50;
                                else if (type == 6) plantCost = 150;

                                // Only allow planting if enough sun is available
                                if (sunsCollected >= plantCost) {
                                    Field_Status[gridY][gridX] = type;
                                    level->createPlant(index, type, Diversion_X + gridX * Cell_Size_X, Diversion_Y + gridY * Cell_Size_Y);
                                    index++;
                                    sunsCollected -= plantCost;
                                    selectedCard = -1;
                                }
                                // else: not enough sun, do not plant or deduct sun
                            }
                        }
                    }
                }
            }

            if (paused)
            {
                window.draw(menuBackground);

                if (showPauseInstructions)
                {
                    window.draw(instructionsHeading);
                    window.draw(instructionsBody);
                    backText.setPosition(1366 / 2.f - backText.getLocalBounds().width / 2, 600.f);
                    window.draw(backText);
                }
                else
                {
                    float centerX = 1366 / 2.f;
                    float startY = 260.0f;
                    float spacing = 80.0f;
                    resumeText.setPosition(centerX - resumeText.getLocalBounds().width / 2, startY);
                    pauseInstructionsText.setPosition(centerX - pauseInstructionsText.getLocalBounds().width / 2, startY + spacing);
                    quitText.setPosition(centerX - quitText.getLocalBounds().width / 2, startY + 2 * spacing);

                    Vector2i mousePos = Mouse::getPosition(window);
                    float mx = static_cast<float>(mousePos.x);
                    float my = static_cast<float>(mousePos.y);

                    if (resumeText.getGlobalBounds().contains(mx, my))
                        resumeText.setFillColor(Color::Yellow);
                    else
                        resumeText.setFillColor(Color::White);

                    if (pauseInstructionsText.getGlobalBounds().contains(mx, my))
                        pauseInstructionsText.setFillColor(Color::Yellow);
                    else
                        pauseInstructionsText.setFillColor(Color::White);

                    if (quitText.getGlobalBounds().contains(mx, my))
                        quitText.setFillColor(Color::Yellow);
                    else
                        quitText.setFillColor(Color::White);

                    window.draw(resumeText);
                    window.draw(pauseInstructionsText);
                    window.draw(quitText);
                }

                window.display();
                continue;
            }

            printGrid(Field_Status, Rows, Cols, gridClock);

            int maxIndex = 45;
            for (int i = 0; i < maxIndex; i++)
            {
                level->draw(window, i);
            }

            // --- GAME WON SCREEN & LEVEL TRANSITION ---
            if (level->allZombiesDead())
            {
                if (gameLevel == LEVEL_1)
                {
                    transitioningToLevel2 = true;
                    continue;
                }
                else if (gameLevel == LEVEL_2)
                {
                    transitioningToLevel3 = true;
                    continue;
                }
                else
                {
                    Texture gameWonTexture;
                    Sprite gameWonSprite;
                    if (gameWonTexture.loadFromFile("PVZ/backgrounds/gamewon.jpg"))
                    {
                        gameWonSprite.setTexture(gameWonTexture);
                        float scaleX = 1366.0f / gameWonTexture.getSize().x;
                        float scaleY = 768.0f / gameWonTexture.getSize().y;
                        gameWonSprite.setScale(scaleX, scaleY);
                        gameWonSprite.setPosition(0, 0);
                    }

                    window.clear();
                    window.draw(gameWonSprite);
                    window.display();

                    Clock winClock;
                    while (winClock.getElapsedTime().asSeconds() < 3.f)
                    {
                        Event winEvent;
                        while (window.pollEvent(winEvent))
                        {
                            if (winEvent.type == Event::Closed)
                            {
                                window.close();
                                return;
                            }
                        }
                    }
                    window.close();
                    return;
                }
            }

            // --- GAME OVER SCREEN WITH MOUSE SELECTION ONLY ---
            if (lives <= 0)
            {
                Texture gameOverTexture;
                Sprite gameOverSprite;
                bool gameOverImageLoaded = false;
                if (gameOverTexture.loadFromFile("PVZ/backgrounds/gameover.jpg"))
                {
                    gameOverSprite.setTexture(gameOverTexture);
                    float scaleX = 1366.0f / gameOverTexture.getSize().x;
                    float scaleY = 768.0f / gameOverTexture.getSize().y;
                    gameOverSprite.setScale(scaleX, scaleY);
                    gameOverSprite.setPosition(0, 0);
                    gameOverImageLoaded = true;
                }

                FloatRect tryAgainBox(190, 647, 459, 119);
                FloatRect mainMenuBox(190 + 459, 647, 451, 121);

                while (window.isOpen())
                {
                    window.clear();
                    if (gameOverImageLoaded)
                        window.draw(gameOverSprite);
                    else
                        window.clear(Color::Black);

                    window.display();

                    Event gameOverEvent;
                    while (window.pollEvent(gameOverEvent))
                    {
                        if (gameOverEvent.type == Event::Closed)
                        {
                            window.close();
                            return;
                        }
                        else if (gameOverEvent.type == Event::MouseButtonPressed && gameOverEvent.mouseButton.button == sf::Mouse::Left)
                        {
                            float mx = static_cast<float>(gameOverEvent.mouseButton.x);
                            float my = static_cast<float>(gameOverEvent.mouseButton.y);

                            if (tryAgainBox.contains(mx, my))
                            {
                                goto game_restart;
                            }
                            else if (mainMenuBox.contains(mx, my))
                            {
                                window.close();
                                goto main_menu;
                            }
                        }
                        else if (gameOverEvent.type == Event::KeyPressed && gameOverEvent.key.code == Keyboard::Escape)
                        {
                            window.close();
                            return;
                        }
                    }
                }
                break;
            }

            Vector2f emptySpot = level->getUpdatedLocation();
            emptySpot.x = emptySpot.x - Diversion_X;
            emptySpot.y = emptySpot.y - Diversion_Y;
            int gridX = static_cast<int>(emptySpot.x) / Cell_Size_X;
            int gridY = static_cast<int>(emptySpot.y) / Cell_Size_Y;
            Field_Status[gridY][gridX] = 0;

            gSunFactory->draw(window);

            level->moveZombies(window, Diversion_Y, Cell_Size_Y, lives);
            window.display();
        }
    }
};