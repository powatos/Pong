#include <pdcurses/curses.h>
#include <memory>
#include <sstream>
#include <cmath>

#ifndef NDEBUG
#include <chrono> // DEBUG
#include <iostream> // DEBUG
#include <iomanip> // DEBUG
#endif


#include "Vector2D.hpp"

#include "Ponger.hpp"
#include "Ball.hpp"

#define TARGET_FPS 24

unsigned long long FRAME = 0;
bool FBF = false;

constexpr float MAX_SPEED = 50.f;
constexpr float DRAG_COEF = 3.f;

bool GAME_STARTED = false;
bool GAME_RUNNING = false;
long score = 0l;

std::unique_ptr<Ponger> ponger;
std::unique_ptr<Ball> ball;

float currentAcceleration = 0.f;
int pongerCollisionDelay = 0;

#ifndef NDEBUG
long long lf = 0ll;
static std::chrono::time_point<std::chrono::high_resolution_clock> start;
bool showDebug = false;

std::vector<long long> vals;
void ts(){
    start = std::chrono::high_resolution_clock::now();
}
void te(){
    lf = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count();
    vals.push_back(lf);
}
#endif

bool HandleInput(bool startScreen) {
    int ch = getch();

    if (startScreen) {
        if (ch == 's') { GAME_STARTED = true; GAME_RUNNING = true; }
        
        return true;
    }

    
	currentAcceleration = 0.f;

	if(ch == 'q') {
		GAME_RUNNING = false;
	}

	switch (ch) {
	case 'a':
	case KEY_LEFT:
		currentAcceleration = -1500.f;
		break;

	case 'd':
	case KEY_RIGHT:
		currentAcceleration = 1500.f;
		break;

	#ifndef NDEBUG
	case 'p': // frame by frame
		FBF = !FBF;
		break;

	case 'o': // debug
		showDebug = !showDebug;
		break;
	case 'n':
	    if (FBF) { break; }
	#endif

	default:
		return !FBF; // return false for frame by frame
		break;
	}

	return true;


}

void UpdateLoop(float DeltaTime) {
	if (ponger == nullptr) return;
	if (ball == nullptr) return;
	
	FRAME++;

    if (pongerCollisionDelay > 0) { pongerCollisionDelay--; }

    // define boundaries for less redundancy
    const float min_x = 0.f;
    const float max_x = COLS - Ponger::sizex;
    
    bool at_left_wall = (ponger->Position.x <= min_x);
    bool at_right_wall = (ponger->Position.x >= max_x);

    bool pushing_into_wall = (at_left_wall && currentAcceleration < 0.f) || (at_right_wall && currentAcceleration > 0.f);

    // update velocity
    if (pushing_into_wall) {
        ponger->velocity = 0.f;
    } else {
        // p1 start
        float newVelocity = ponger->velocity + (currentAcceleration * DeltaTime);
        
        // Apply drag ()
        newVelocity += -newVelocity * DRAG_COEF * DeltaTime;
    
        if (std::abs(newVelocity) < 0.1f) {
            newVelocity = 0.f;
        }
    
        if (std::abs(newVelocity) > MAX_SPEED) {
            ponger->velocity = MAX_SPEED * std::copysign(1.f, newVelocity);
        } else {
            ponger->velocity = newVelocity;
        }
    
        
        // p1 end
    }
    // p2 start
    // update position
    ponger->Position.x += ponger->velocity * DeltaTime;
    
    // clamp position
    if (ponger->Position.x > max_x) {
        ponger->Position.x = max_x;
        ponger->velocity = 0.f;
    } else if (ponger->Position.x < min_x) {
        ponger->Position.x = min_x;
        ponger->velocity = 0.f;
    }
    // p2 end

    
    ball->Position = ball->Position + ball->velocity * DeltaTime;
    
    bool bCollidingPonger = 
        (ball->Position.x < ponger->Position.x + Ponger::sizex) && // Ball's left edge is left of Ponger's right edge
        (ball->Position.x + Ball::sizex > ponger->Position.x) &&   // Ball's right edge is right of Ponger's left edge
        (ball->Position.y < ponger->Position.y + Ponger::sizey) && // Ball's top edge is above Ponger's bottom edge
        (ball->Position.y + Ball::sizey > ponger->Position.y);    // Ball's bottom edge is below Ponger's top edge
        
    at_left_wall = (ball->Position.x <= min_x+1);
    at_right_wall = (ball->Position.x >= COLS-Ball::sizex-1);
    bool at_top_wall = (ball->Position.y <= 0.f);
    bool at_bot_wall = (ball->Position.y >= LINES - Ball::sizey);
    
    if (at_bot_wall) { GAME_RUNNING = false; return; }

    if (bCollidingPonger && pongerCollisionDelay == 0) {
        pongerCollisionDelay = 5;
        
        float Pcenter = ponger->Position.x + Ponger::sizex/2;
        float BCenter = ball->Position.x + Ball::sizex/2;
        
        Vector2D newVelocity = ball->velocity;
        
        // reflect based on impact location
        newVelocity.y *=-1;
        newVelocity.x = (BCenter - Pcenter)/(Ponger::sizey/2) * 5.f;
        
        // add slight randomness
        ball->randomizeVelocity(-1, 1);
        ball->velocity.y = -std::abs(ball->velocity.y); // random always points up
        ball->velocity = ball->velocity +  0.9f * (newVelocity - ball->velocity); // lerp vel to newvel with alpha 0.9
        
        ball->velocity = newVelocity.Normalized() * ball->speed;
        
        score++;
        
        
    } else if (at_right_wall || at_left_wall || at_top_wall || at_bot_wall) {
        Vector2D boundNormal(0.f,0.f);
        if (at_left_wall) { boundNormal.x = -1; ball->Position.x = 1.f; }
        else if (at_right_wall) { boundNormal.x = 1; ball->Position.x = COLS-Ball::sizex-1; }
        if (at_top_wall) { boundNormal.y = -1; ball->Position.y = 0.f; }
        else if (at_bot_wall) { boundNormal.y = 1; ball->Position.y = LINES-Ball::sizey; }
        
        boundNormal = boundNormal.Normalized(); // prevents (1,1) normal when hitting corner
        
        // mirror reflect velocity by bound normal
        ball->velocity = (ball->velocity - 2.f*(Vector2D::Dot(ball->velocity, boundNormal)) * boundNormal);
    }
    
    if (std::abs(ball->velocity.Normalized().y) < 0.5f) { // if flying close to flat horizontal
        ball->velocity.y -= 5.f;
    }
    

}

#ifndef NDEBUG
const char* getDBOut(){
    // static stream to reuse buffer
    static std::ostringstream ss;
    ss.str(""); // clear content
    ss.clear(); // clear state

    // static string to hold the result
    // reuse buffer, avoide allocation every frame
    static std::string result_string; 

    static const int _init = [&]() {
        ss << std::fixed << std::setprecision(6) << std::showpos;
        return 0;
    }();

    // debug logs
    static bool PV = true;
    static bool PPos = !true;

    static bool BV = true;
    static bool BPos = true;
    
    static bool Frame = true;
    static bool LF = true;
    
    static const char* sep = " | ";
    bool needsSeparator = false;

    // helper to add separator
    auto addSep = [&]() {
        if (needsSeparator) {
            ss << sep;
        }
        needsSeparator = true;
    };

    // helper to format position directly to the stream
    auto formatPos = [&](float x, float y) {
        ss << "(" << static_cast<int>(std::round(x))
           << " , " << static_cast<int>(std::round(y)) << ')';
    };


    // build string stream
    if (PV) {
        addSep();
        ss << "PV: " << std::setw(10) << ponger->velocity;
    }
    if (BV) {
        addSep();
        ss << "BV: " << ball->velocity.ToString(); 
    }
    if (PPos) {
        addSep();
        ss << "PPos: ";
        formatPos(ponger->Position.x, ponger->Position.y);
    }
    if (BPos) {
        addSep();
        ss << "BPos: ";
        formatPos(ball->Position.x, ball->Position.y);
    }
    if (Frame) {
        addSep();
        ss << "Frame: " << FRAME;
    }
    if (LF) {
        addSep();
        ss << "LF: " << lf;
    }
    
    // copy the stream content into static string.
    result_string = ss.str(); 
    
    // eturn pointer to static string data.
    return result_string.c_str();
}
#endif

void RenderScreen(int screen) {
	if (ponger == nullptr) return;
	if (ball == nullptr) return;

	erase();
	
	if (screen == 0) { // game
	    
    	// bounds
    	for (int i=0; i<LINES; i++){
    	    mvaddch(i, 0, '|');
    	    mvaddch(i, COLS-1, '|');
    	}
    
    
    	auto _ = [=](std::vector<std::vector<char>> texture, const int posX, const int posY) {
    		int i=0;
    		for (const std::vector<char> row : texture) {
    			int j = 0;
    			for (const char pix : row) {
    
    				mvaddch(posY+i, posX+j, pix);
    				j++;
    			}
    			i++;
    		}
    	};
    
    	_(Ponger::texture, ponger->Position.x, ponger->Position.y);
    	_(Ball::texture, ball->Position.x, ball->Position.y);

		#ifndef NDEBUG
    	if (showDebug) { mvprintw(0,0, "%s", getDBOut()); }
		#endif

	} else
	if (screen == 1) {
	    std::string endMsg = "GAME OVER";
	    mvprintw(LINES/2 - 1, (COLS - endMsg.size())/2, "%s", endMsg.c_str());
	} else
	if (screen == 2) {
	    // title
	    std::string titleMsg0 = "PONG";
	    std::string titleMsg1 = "by potato";
	    mvprintw(0, (COLS-titleMsg0.size())/2, "%s", titleMsg0.c_str());
	    mvprintw(1, (COLS-titleMsg1.size())/2, "%s", titleMsg1.c_str());
	    
	    // info
    	std::vector<std::string> infoMessages = {
    	    "This is a terminal-based Pong minigame",
    	    "This program uses C++ 23, utilizing the curses/ncurses library to render text",
    	    R"(github.com/powatos)"
    	};

    	for (int i=0; i < infoMessages.size(); i++) {
    	    mvprintw(LINES/4 + (i - 1), (COLS - infoMessages[i].size())/2, "%s", infoMessages[i].c_str());
    	}
    	
    	// tutorial
    	std::vector<std::string> startMessages = {
    	    "ENSURE BOUNDS FIT TO WINDOW AND RE-RUN",
    	    "DO NOT CHANGE TERMINAL SIZE DURING RUNTIME",
    	    "",
    	    "Use 'a' and 'd' or '<-' and '->' to move ponger",
    	    "Press 'q' to end game",
    	    "",
    	    "PRESS 'S' TO START"
    	};

    	for (int i=0; i < startMessages.size(); i++) {
    	    mvprintw(LINES/2 + (i + 1), (COLS - startMessages[i].size())/2, "%s", startMessages[i].c_str());
    	}
    	
    	// bounds
    	for (int i=0; i<LINES; i++){
    	    mvaddch(i, 0, '|');
    	    mvaddch(i, COLS-1, '|');
    	}
	}
	
	// score
	std::string scoreMsg = "Score: " + std::to_string(score);
    mvprintw(LINES-1, 1, "%s", scoreMsg.c_str());

	refresh();
}


int main()
{
	initscr(); // init ncurses
	cbreak(); // disables line buffering
	noecho(); // disables input feedback
	keypad(stdscr, TRUE); // enables keypad input
	curs_set(0); // disables cursor visibility
	timeout(0); // Make getch() non-blocking IMMEDIATELY
	
	Ponger::LoadTexture();
	Ball::LoadTexture();

	Vector2D pongerSP((COLS-Ponger::sizex)/2, LINES-Ponger::sizey);
	Vector2D ballSP = pongerSP - Vector2D(0, 10);

	ponger = std::make_unique<Ponger>(pongerSP);
	ball = std::make_unique<Ball>(ballSP, 35.f);
	
	while (!GAME_STARTED) {
	    HandleInput(true);
	    RenderScreen(2);
	}
	
	while(GAME_RUNNING) {

		if (!HandleInput(false)) {
			RenderScreen(0);
			continue;
		}

		UpdateLoop(1.f/(float)TARGET_FPS);

		RenderScreen(0);
        
		// Wait for the next frame
		napms(1000/TARGET_FPS);
	}
	

	RenderScreen(1);
	napms(5000);


	
    endwin();

	return 0;
}