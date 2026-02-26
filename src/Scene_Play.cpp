#include "Scene_Play.hpp"
#include "Action.hpp"
#include "Assets.hpp"
#include "Components.hpp"
#include "GameEngine.hpp"
#include "Physics.hpp"

#include "Scene_Menu.hpp"
#include <fstream>
#include <iostream>

void Scene_Play::init(const std::string &levelPath)
{
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");   // Toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // Toggle drawing (C)ollision Boxes
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");      // Toggle drawing (G)rid

    registerAction(sf::Keyboard::D, "MOVE_FORWARD");
    registerAction(sf::Keyboard::A, "MOVE_BACKWARD");
    registerAction(sf::Keyboard::S, "MOVE_DOWN");
    registerAction(sf::Keyboard::W, "MOVE_UP");
    registerAction(sf::Keyboard::Space, "JUMP");

    // TODO: Register all other gameplay Actions
    m_gridText.setCharacterSize(14);
    m_gridText.setFont(m_game->assets().getFont("cardot"));

    loadLevel(levelPath);
}

Scene_Play::Scene_Play(GameEngine *gameEngine, const std::string &levelPath)
    : Scene(gameEngine)
    , m_levelPath(levelPath)
{
    init(m_levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    // TODO: This funation takes in a grid (x, y) position and an Entity
    //       Return a Vec2 indicating where the CENTER position of the Entity should be
    //       You must use the Entity's Animation size to position it correctly
    //       The size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
    //       The bottom-left corner of the Animation should align with the bottom left of the grid cell

    // (0, 0) => (32 * 0, height - 64 * 0) =
    // (0, 1) = (0, height - 64 * 1)
    // (0, 2) = (0, height - 64 * 2)
    // ...
    // (n, n) = (64 * n, heigth - 64 * n)
    auto cAnimation = entity->getComponent<CAnimation>();
    Vec2 bottomLeftCell(gridX * 64 + (cAnimation.animation.getSize().x / 2) * 4,
                        m_game->window().getSize().y - (64 * gridY) - (cAnimation.animation.getSize().y / 2) * 4);
    return bottomLeftCell;
}

void Scene_Play::loadLevel(const std::string &filename)
{
    // reset the entity manager every time we load a level
    m_entityManager = EntityManager();

    // TODO: read in the level file and add the appropriate entities
    //       use the PlayerConfig struct m_playerConfig to store player properties
    //       this struct is defined at the top of Scene_Play.h

    // NOTE: all of the code below is sample code which shows you how to
    //       set up and use entities with the new syntax, it should be removed

    std::fstream fin(filename);
    std::string  fpoint;

    while (fin >> fpoint)
    {
        if (fpoint == "Tile")
        {
            std::string name;
            float       gX;
            float       gY;

            fin >> name >> gX >> gY;

            auto entity = m_entityManager.addEntity("tile");
            entity->addComponent<CAnimation>(m_game->assets().getAnimation(name), true);
            entity->addComponent<CTransform>(gridToMidPixel(gX, gY, entity));
            entity->addComponent<CBoungingBox>(m_game->assets().getAnimation(name).getSize() * 4);

            auto &cTransform = entity->getComponent<CTransform>();
            cTransform.scale = Vec2(4.f, 4.f);
        }
        else if (fpoint == "Dec")
        {
            std::string name;
            float       gX;
            float       gY;

            fin >> name >> gX >> gY;

            auto entiy = m_entityManager.addEntity("decoration");
            entiy->addComponent<CAnimation>(m_game->assets().getAnimation(name), true);
            entiy->addComponent<CTransform>(gridToMidPixel(gX, gY, entiy));

            auto &cTransform = entiy->getComponent<CTransform>();
            cTransform.scale = Vec2(4.f, 4.f);
        }
        else if (fpoint == "Player")
        {
            fin >> m_playerConfig.GX >> m_playerConfig.GY >> m_playerConfig.BBW >> m_playerConfig.BBH >>
                m_playerConfig.SPEED >> m_playerConfig.JUMPSPEED >> m_playerConfig.MAXSPEED >> m_playerConfig.GRAVITY >>
                m_playerConfig.WEAPON;

            spawnPlayer();
        }
    }

    // !IMPORTANT: always add the CAnimation component first so that gridToMidPixel can compute correctly
    // brick->addComponent<CAnimation>(m_game->assets().getAnimation("Ground"), true);
    // brick->addComponent<CTransform>(Vec2(96, 480));
    // auto &transform = brick->getComponent<CTransform>();
    // transform.scale = Vec2(4.f, 4.f);
    // brick->getComponent<CTransform>().scale(Vec2(4.0, 4.0));
    // NOTE: You final code should postion the entity with the grid x, y, postion read from the file:
    // brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick));

    // if (brick->getComponent<CAnimation>().animation.getName() == "Ground")
    // {
    //     std::cout << "This could be a good way of identifying if a tile is a brick!\n";
    // }

    // auto block = m_entityManager.addEntity("tile");
    // brick->addComponent<CAnimation>(m_game->assets().getAnimation("Block"), true);
    // brick->addComponent<CTransform>(Vec2(224, 480));
    // add a bounding box, this will now show up if we press the 'C' key
    // block->addComponent<CBoungingBox>(m_game->assets().getAnimation("Block").getSize());

    // auto question = m_entityManager.addEntity("tile");
    // brick->addComponent<CAnimation>(m_game->assets().getAnimation("Question"), true);
    // brick->addComponent<CTransform>(Vec2(224, 480));

    // !NOTE: THIS IS INCREADIBLY IMPORTANT
    //       Components are now returned as references rather than pointers
    //       If you do not specify a reference variable type, it will COPY the component
    //       Here is an example:
    //
    //       This will COPY the transform into the variable 'transform1' - it is INCORRECT
    //       Any changes you make to transform1 will not be changed inside the entity
    //       auto transform1 = entity->get<CTransform>()
    //
    //       This will REFERENCE the transform with the variable 'transform2' - it is CORRECT
    //       Now any changes you make to transform2 will be changed inside the entity
    //       auto& transform2 = entity->get<CTransform>()
}

void Scene_Play::spawnPlayer()
{
    // here is a sample player entity which you can use to construct entities
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Idle"), true);
    m_player->addComponent<CTransform>(gridToMidPixel(m_playerConfig.GX, m_playerConfig.GY, m_player));
    m_player->addComponent<CInput>();
    m_player->addComponent<CState>();
    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
    m_player->addComponent<CBoungingBox>(Vec2(m_playerConfig.BBH, m_playerConfig.BBW));
    auto &transform    = m_player->getComponent<CTransform>();
    transform.velocity = Vec2(m_playerConfig.SPEED, m_playerConfig.JUMPSPEED);
    transform.scale    = Vec2(4.f, 4.f);
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
    // TODO: this should spawn a bullet at the given entiy, going in the direction entity is facing
}

void Scene_Play::update()
{
    m_entityManager.update();

    // TODO: implement pause functionality

    sMovement();
    // sLifespan();
    sCollision();
    sAnimation();
    sRender();
}

void Scene_Play::setPause(bool isPaused)
{
    m_paused = isPaused;
}

void Scene_Play::sMovement()
{
    // TODO: Implement player movement / jumping based on its CInput component
    // TODO: Implement gravity's effect on the player
    // TODO: Implement the maximum player speed in both X and Y directions
    // TODO: Setting an entity's scale.x to -1/1 will make it face to the left/right
    auto &p_Input     = m_player->getComponent<CInput>();
    auto &p_Transform = m_player->getComponent<CTransform>();
    auto &p_Gravity   = m_player->getComponent<CGravity>();
    auto &p_State     = m_player->getComponent<CState>();

    if (p_Input.right)
    {
        p_State.state       = "run";
        p_Transform.prevPos = p_Transform.pos;
        p_Transform.pos.x += p_Transform.velocity.x;
        p_Transform.scale.x = 4;
    }

    if (p_Input.left)
    {
        p_State.state       = "run";
        p_Transform.prevPos = p_Transform.pos;
        p_Transform.pos.x -= p_Transform.velocity.x;
        p_Transform.scale.x = -4;
    }

    if (p_Input.up)
    {
        p_State.state = "jumping";
        if (p_Transform.max_jump_height <= p_Transform.pos.y)
        {
            p_Transform.prevPos = p_Transform.pos;
            p_Transform.pos.y += p_Transform.velocity.y;
        }
        else if (p_Transform.max_jump_height > p_Transform.pos.y)
        {
            p_Input.up = false;
        }
    }
    else
    {
        p_Transform.max_jump_height = p_Transform.pos.y - (m_gridSize.y * 4.7);
    }

    if (p_State.state != "jumping")
    {
        p_Transform.prevPos.y = p_Transform.pos.y;
        p_Transform.pos.y += m_playerConfig.MAXSPEED * p_Gravity.gravity;
    }
}

void Scene_Play::sLifespan()
{
    // TODO: Check lifespan of entities that have them, and destroy them if they go over
}

void Scene_Play::sCollision()
{
    // REMEMBER: SFML's (0,0) position is on the TOP-LEFT corner
    //           This means jumping will have a negative y-component
    //           and gravity will have a positive y-component
    //           Also, something BELOW something else will have a y value GREATER than it
    //           Also, something ABOVE something else will have a y value LESS than it

    // TODO: Implement Physics::GetOverlap() function, use it inside this function

    // TODO: Implement bullet / tile collisions
    //       Destroy the tile if it has a Brick animation
    // TODO: Implement player / tile collisions and resolutions
    //       Update the CState component of the player to store whether
    //       it is currently on the ground or in the air. This will be
    //       used by the Animation system
    // TODO: Check to see if the player has fallen down a hole (y > height())
    // TODO: Don't let the player walk off the left side of the map

    auto &p_Transform = m_player->getComponent<CTransform>();
    auto &p_State     = m_player->getComponent<CState>();
    auto &p_Input     = m_player->getComponent<CInput>();

    if (p_Transform.pos.x <= 0)
    {
        // m_player->getComponent<CTransform>().pos.x += m_player->getComponent<CBoungingBox>().halfSize.x;
        p_Transform.pos.x += p_Transform.velocity.x;
    }

    Physics p;
    for (auto &e : m_entityManager.getEntities("tile"))
    {
        Vec2 overlap      = p.GetOverlap(e, m_player);
        Vec2 prev_overlap = p.GetPreviousOverlap(e, m_player);

        auto &e_Transform = e->getComponent<CTransform>();

        // LEFT-RIGHT COLLISIONS
        if (overlap.x > 0 && prev_overlap.y > 0)
        {
            // RIGHT --> LEFT
            if (p_Transform.pos.x < e_Transform.pos.x)
                p_Transform.pos.x -= overlap.x;
            // LEFT --> RIGHT
            if (p_Transform.pos.x > e_Transform.pos.x)
                p_Transform.pos.x += overlap.x;
        }
        // TOP-BOTTOM COLLISIONS
        if (overlap.y > 0 && prev_overlap.x > 0)
        {
            // TOP --> BOTTOM
            if (p_Transform.pos.y < e_Transform.pos.y)
            {
                p_Transform.pos.y -= overlap.y;
                p_Input.canJump = true;
                p_State.state   = (p_Input.left || p_Input.right) ? "run" : "idle";
            }
            // BOTTOM --> TOP
            if (p_Transform.pos.y > e_Transform.pos.y)
            {

                p_Transform.pos.y += overlap.y;
                p_State.state   = "air";
                p_Input.up      = false;
                p_Input.canJump = false;
            }
        }

        if (overlap.y < 0 && prev_overlap.x < 0)
        {
            if (p_Transform.pos.y < e_Transform.pos.y)
            {
                p_State.state   = "air";
                p_Input.canJump = false;
            }
        }
        // std::cout << "overlap.x: " << overlap.x << std::endl;
        // std::cout << "overlap.prev.x: " << prev_overlap.x << std::endl;
        // std::cout << "overlap.y: " << overlap.y << std::endl;
        // std::cout << "overlap.prev.y: " << prev_overlap.y << std::endl;
    }
}

void Scene_Play::sDoAction(const Action &action)
{
    if (action.type() == "START")
    {
        if (action.name() == "TOGGLE_TEXTURE")
            m_drawTextures = !m_drawTextures;
        else if (action.name() == "TOGGLE_COLLISION")
            m_drawCollision = !m_drawCollision;
        else if (action.name() == "TOGGLE_GRID")
            m_drawGrid = !m_drawGrid;
        else if (action.name() == "PAUSE")
            setPause(!m_paused);
        else if (action.name() == "QUIT")
            onEnd();
        else if (action.name() == "MOVE_FORWARD")
        {
            m_player->getComponent<CInput>().right = true;
        }
        else if (action.name() == "MOVE_BACKWARD")
        {
            m_player->getComponent<CInput>().left = true;
        }
        else if (action.name() == "MOVE_DOWN")
            m_player->getComponent<CInput>().down = true;
        else if (action.name() == "JUMP")
        {
            if (m_player->getComponent<CInput>().canJump)
            {

                m_player->getComponent<CInput>().up = true;
            }
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "MOVE_FORWARD")
        {
            m_player->getComponent<CInput>().right = false;
        }
        else if (action.name() == "MOVE_BACKWARD")
        {
            m_player->getComponent<CInput>().left = false;
        }
        else if (action.name() == "MOVE_UP")
            m_player->getComponent<CInput>().up = false;
        else if (action.name() == "MOVE_DOWN")
            m_player->getComponent<CInput>().down = false;
        else if (action.name() == "JUMP")
        {
            m_player->getComponent<CInput>().up = false;
        }
    }
}

void Scene_Play::sAnimation()
{
    auto &p_state = m_player->getComponent<CState>();

    if (p_state.state == "air")
    {
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Jump"), true);
    }

    if (p_state.state == "run")
    {
        if (m_player->getComponent<CAnimation>().animation.getName() != "Run")
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Run"), true);
    }
    if (p_state.state == "jumping")
    {
        if (m_player->getComponent<CAnimation>().animation.getName() != "Jump")
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Jump"), true);
    }
    if (p_state.state == "idle")
    {
        if (m_player->getComponent<CAnimation>().animation.getName() != "Idle")
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Idle"), true);
    }

    for (auto &e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CAnimation>())
        {
            e->getComponent<CAnimation>().animation.update();
            if (!e->getComponent<CAnimation>().repeat && e->getComponent<CAnimation>().animation.hasEnded())
            {
                e->destroy();
            }
        }
    }

    // TODO: Complete the Animation class code first

    // TODO: set the animation of the player based on its CState component
    // TODO: for each entity with an animation, call entity->getComponent<CAnimation>().animation.update()
    //       if the animation is not repeated, and it has ended, destroy the entity
}

void Scene_Play::sRender()
{
    // color the backgound darker so you know that the game is paused
    if (!m_paused)
    {
        m_game->window().clear(sf::Color(100, 100, 255));
    }
    else
    {
        m_game->window().clear(sf::Color(50, 50, 150));
    }

    // set the viewport of the window to be centered on the player if it's far enough right
    auto    &pPos          = m_player->getComponent<CTransform>().pos;
    float    windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
    sf::View view          = m_game->window().getView();
    view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
    m_game->window().setView(view);

    // draw all Entity textures / animations
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities())
        {
            auto &transform = e->getComponent<CTransform>();
            if (e->hasComponent<CAnimation>())
            {
                auto &animation = e->getComponent<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                m_game->window().draw(animation.getSprite());
            }
        }
    }
    // draw all Entity collision bounding boxes with a rectangleshape
    if (m_drawCollision)
    {
        for (auto e : m_entityManager.getEntities())
        {
            if (e->hasComponent<CBoungingBox>())
            {
                auto              &box       = e->getComponent<CBoungingBox>();
                auto              &transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;

                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color(255, 255, 255, 255));
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }

    if (m_drawGrid)
    {
        // std::cout << m_game->window().getView().getCenter().x << '\n';
        // std::cout << width() / 2 << '\n';
        float leftX = m_game->window().getView().getCenter().x - width() / 2;
        // std::cout << "leftX: " << leftX << '\n';
        float rightX = leftX + width() + m_gridSize.x;
        // std::cout << "rightX: " << rightX << '\n';
        float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);
        // std::cout << "nextGridX: " << nextGridX << '\n';

        // float leftX     = 0;
        // float rightX    = width();
        // float nextGridX = 0;

        for (float x = nextGridX; x < rightX; x += m_gridSize.x)
        {
            drawLine(Vec2(x, 0), Vec2(x, height()));
        }

        for (float y = 0; y < height(); y += m_gridSize.y)
        {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

            for (float x = nextGridX; x < rightX; x += m_gridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
                std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setColor(sf::Color::White);
                m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
                m_game->window().draw(m_gridText);
            }
        }
    }

    m_game->window().display();
}

void Scene_Play::onEnd()
{
    // TODO: When the scene ends, change back to the MENU scene
    //       use m_game->changeScene(correct params);
    m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game), true);
}
