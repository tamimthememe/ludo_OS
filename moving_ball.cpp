//i22449_Tamim_Ahmad    i222409_Eman_Khalid    i221511_Huzaifa_Imran

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
#include <map>
#include <string>

using namespace std;

// Constants
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

struct CircleInCircle {
    int centerX;
    int centerY;
    int outerRadius;
    SDL_Color outerColor;
    int innerRadius;
    SDL_Color innerColor;
};


// Helper function to log SDL errors
void logSDLError(const std::string& msg) {
    std::cerr << msg << " Error: " << SDL_GetError() << std::endl;
}

void drawCircleInCircle(SDL_Renderer* renderer, const CircleInCircle& circle) {
    // Draw the outer circle
    filledCircleRGBA(renderer, circle.centerX, circle.centerY, circle.outerRadius,
                     circle.outerColor.r, circle.outerColor.g, circle.outerColor.b, circle.outerColor.a);
                     
    // Draw the inner circle
    filledCircleRGBA(renderer, circle.centerX, circle.centerY, circle.innerRadius,
                     circle.innerColor.r, circle.innerColor.g, circle.innerColor.b, circle.innerColor.a);
}


// Helper function to draw a circle
void drawCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color) {
    filledCircleRGBA(renderer, x, y, radius, color.r, color.g, color.b, color.a);
}

// Helper function to draw a rounded rectangle
void drawRoundedRectangle(SDL_Renderer* renderer, int x, int y, int w, int h, int radius, SDL_Color color) {
    roundedBoxRGBA(renderer, x, y, x + w, y + h, radius, color.r, color.g, color.b, color.a);
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logSDLError("SDL_Init");
        return 1;
    }

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        logSDLError("IMG_Init");
        SDL_Quit();
        return 1;
    }

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("Static Image Rendering",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        logSDLError("SDL_CreateWindow");
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        logSDLError("SDL_CreateRenderer");
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Load the image
    SDL_Surface* imageSurface = IMG_Load("asset.png");
    if (!imageSurface) {
        logSDLError("IMG_Load");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface); // Free the surface after creating the texture
    if (!imageTexture) {
        logSDLError("SDL_CreateTextureFromSurface");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    map<string, CircleInCircle> circles;
    //Green
    circles["green1"] = {1230, 610, 40, {40, 118, 22, 255}, 25, {40, 176, 21, 255}};
    circles["green2"] = {1355, 820, 40, {40, 118, 22, 255}, 25, {40, 176, 21, 255}};
    circles["green3"] = {1205, 820, 40, {40, 118, 22, 255}, 25, {40, 176, 21, 255}};
    circles["green4"] = {1280, 900, 40, {40, 118, 22, 255}, 25, {40, 176, 21, 255}};
    
    //Blue
    circles["blue1"] = {1280, 130, 40, {7, 146, 238, 255}, 25, {1, 77,128, 255}};
    circles["blue2"] = {1355, 210, 40, {7, 146,238, 255}, 25, {1, 77,128, 255}};
    circles["blue3"] = {1205, 210, 40, {7, 146, 238, 255}, 25, {1, 77,128, 255}};
    circles["blue4"] = {1280, 290, 40, {7, 146, 238, 255}, 25, {1, 77,128, 255}};

    //Red
    circles["red1"] = {640, 130, 40, {255, 0,66, 255}, 25, {94, 1, 25, 255}};
    circles["red2"] = {715, 210, 40, {255, 0,66, 255}, 25, {94, 1, 25, 255}};
    circles["red3"] = {565, 210, 40, {255, 0,66, 255}, 25, {94, 1, 25, 255}};
    circles["red4"] = {640, 290, 40, {255, 0,66, 255}, 25, {94, 1, 25, 255}};

    //yellow
    circles["yellow1"] = {640, 740, 40, {221, 255, 0, 255}, 25, {69, 79, 0, 255}};
    circles["yellow2"] = {715, 820, 40, {221, 255, 0, 255}, 25, {69, 79, 0, 255}};
    circles["yellow3"] = {565, 820, 40, {221, 255, 0, 255}, 25, {69, 79, 0, 255}};
    circles["yellow4"] = {640, 900, 40, {221, 255, 0, 255}, 25, {69, 79, 0, 255}};
   

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        
            // Example: Move "circle1" with arrow keys
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        circles["green1"].centerY -= 90;
                        break;
                    case SDLK_DOWN:
                        circles["green1"].centerY += 90;
                        break;
                    case SDLK_LEFT:
                        circles["green1"].centerX -= 90;
                        break;
                    case SDLK_RIGHT:
                        circles["green1"].centerX += 90;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 42, 40, 84, 0);
        SDL_RenderClear(renderer);

         
        
    
        SDL_Rect destRect = {460, 20, 1000, 1000}; // x, y, width, height
        SDL_RenderCopy(renderer, imageTexture, nullptr, &destRect);

        //Draw tokens
        for (const auto& [name, circle] : circles) {
            drawCircleInCircle(renderer, circle);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16); 
    }

    // Cleanup
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
