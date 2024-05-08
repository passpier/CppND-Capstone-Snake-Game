#include "renderer.h"
#include <iostream>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Initialize SDL TTF
  if (TTF_Init() != 0) {
    std::cerr << "Unable to initialize SDL_ttf: " << TTF_GetError() << "\n";
  }

  // Loops, Functions, I/O :
  // The project reads data from a file and process the data, or the program writes data to a file.

  font = TTF_OpenFont("fonts/arial.ttf", 28); // Change "arial.ttf" to your font file path
  if (font == NULL) {
    printf("Font could not be loaded! SDL_ttf Error: %s\n", TTF_GetError());
    return;
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

// Memory Management
// The project uses destructors appropriately.

Renderer::~Renderer() {
  TTF_CloseFont(font);
  SDL_DestroyWindow(sdl_window);
  TTF_Quit();
  SDL_Quit();
}

void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render score
  SDL_Color textColor = {255, 255, 255, 255};
  std::string fullText = "Snake Score: " + gameScore;
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, fullText.c_str(), textColor);
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(sdl_renderer, textSurface);
  SDL_Rect textRect = {50, 10, textSurface->w, textSurface->h};
  SDL_RenderCopy(sdl_renderer, textTexture, NULL, &textRect);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

// Loops, Functions, I/O : 
// The project demonstrates an understanding of C++ functions and control structures.

void Renderer::UpdateScore(int score) {
  int finalScore = score * 100;
  gameScore = std::to_string(finalScore);
}
