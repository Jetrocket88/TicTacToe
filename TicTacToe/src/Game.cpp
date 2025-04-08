#include "../headers/Game.h"

SpriteRenderer  *Renderer;

Game::Game(unsigned int width, unsigned int height)
    :Width(width), Height(height)
{
}

Game::~Game()
{
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("src/shaders/sprite.vs", "src/shaders/sprite.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("src/textures/empty.png", true, "empty");
    ResourceManager::LoadTexture("src/textures/cross.png", true, "cross");
    ResourceManager::LoadTexture("src/textures/knot.png", true, "knot");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->Squares.push_back(GameObject(glm::vec2((this->Width / 3) * i, (this->Height / 3) * j), glm::vec2(this->Width / 3, this->Height / 3), 
                Empty));
        }
    }
    ResourceManager::LoadTexture("src/textures/background.jpg", false, "background");
}

void Game::ProcessInput(float dt)
{
    glfwPollEvents();
}


void Game::Update(float dt)
{
    // Check for rows, columns, and diagonals
    for (int i = 0; i < 3; i++) {
        // Check rows
        if (this->Squares[i * 3].State == this->Squares[i * 3 + 1].State &&
            this->Squares[i * 3].State == this->Squares[i * 3 + 2].State &&
            this->Squares[i * 3].State != Empty) {
            std::cout << "Row " << i << " win by " << this->Squares[i * 3].State << std::endl;
        }
        // Check columns
        if (this->Squares[i].State == this->Squares[i + 3].State &&
            this->Squares[i].State == this->Squares[i + 6].State &&
            this->Squares[i].State != Empty) {
            std::cout << "Column " << i << " win by " << this->Squares[i].State << std::endl;
        }
    }
    // Check diagonals
    if (this->Squares[0].State == this->Squares[4].State &&
        this->Squares[0].State == this->Squares[8].State &&
        this->Squares[0].State != Empty) {
        std::cout << "Diagonal win by " << this->Squares[0].State << std::endl;
    }
    if (this->Squares[2].State == this->Squares[4].State &&
        this->Squares[2].State == this->Squares[6].State &&
        this->Squares[2].State != Empty) {
        std::cout << "Diagonal win by " << this->Squares[2].State << std::endl;
    }
}

void Game::Render()
{
	// draw background
	Texture2D backgroundTex = ResourceManager::GetTexture("background");
	//Renderer->DrawSprite(backgroundTex, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);

    Texture2D switchTex;
    for (GameObject &obj : this->Squares) {
        switch (obj.State)
        {
        case Cross:
            switchTex = ResourceManager::GetTexture("cross");
            break;
        case Knot:
            switchTex = ResourceManager::GetTexture("knot");
            break;
        case Empty:
            switchTex = ResourceManager::GetTexture("empty");
            break;
        default:
            break;
        }
        Renderer->DrawSprite(switchTex, obj.Position, obj.Size);
    }
}
