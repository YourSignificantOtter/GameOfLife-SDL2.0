#pragma once

typedef enum
{
	CELL_LIVING,
	CELL_DEAD,
} cell_states;

class Cell
{
	public:
		Cell();
		~Cell();

		int GetNeighbors(int xPos, int yPos);
		int TimeStep(void);
		int GetCurrentState(void);
		int SetNextState(cell_states);
	
	private:
		cell_states currentState;
		cell_states nextState;


};