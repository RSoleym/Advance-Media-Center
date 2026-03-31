//#include <deque>

class SnakeGame {
    
    private:
        struct location{ int x; int y; }; 

        enum direction { up, down, left, right};
        void initGame();

        void moveSnake();
        void drawSnake() const;
        void clearSnake() const;

        void spawnFood();
        void drawFood() const;

        void drawBackground() const;
        
        bool wallCollision() const;
        bool selfCollision() const;
        bool foodCollision() const;
        
				void gameOverScreen() const;

        //std::deque<location> snakeBody; //Can't use this cuz of microLIB
				location snakeBody[20];
        int snakeSize;
				
				location previousTail;

        location foodLocation;

        direction currentDirection;

        bool game_start;
        bool gameOver;
        
				int score;
        
				char scoreDisplay[20];

        static const int boardWidth = 70; //The width of the board ends at 250
        static const int boardHeight = 30; //The height of the board ends at 210
        static const int fullWidth = 15;  //Each block is 12x12 pixels
        static const int fullHeight = 15;  //Each block is 12x12 pixels
    
    public:
        void start();

 
};