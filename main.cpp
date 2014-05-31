#include <string>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <cmath>
using namespace::std;


//enums identifying the occupant of a gameboard cell.
enum cellOccupant {
	NO_PLAYER = 0,
	PLAYER_X = 1,
	PLAYER_O = -PLAYER_X
};


//enums describing the current game state.
enum gameState {
	DRAW = 0,
	WIN_X = PLAYER_X,
	WIN_O = PLAYER_O,
	IN_PROGRESS = 2
};


//tic-tac-toe game board class.
class gameBoard {
public:
	//constructors and destructor.
	gameBoard() { memset(cells, 0, 9); }
	gameBoard(const gameBoard& b) { *this = b; }
	~gameBoard() {}


	//helpful operators.
	gameBoard& operator=(const gameBoard& b) { memcpy(cells, b.cells, 9); return *this; }
	char* operator[](unsigned int i) { return &cells[i][0]; }


	//returns the cells contents as an std string.
	string cellsAsStdString() { string str(&(cells[0][0]), 9); return str; }


	//evaulates the current game state.
	int evaluateState() const {
		static const char playerXWinningSum = PLAYER_X * 3;
		static const char playerOWinningSum = PLAYER_O * 3;
		static const char absDrawSum = PLAYER_X * 9;

		//check the board rows.
		for(unsigned int i = 0; i < 3; i++) {
			char rowSum = cells[i][0] + cells[i][1] + cells[i][2];
			if(rowSum == playerXWinningSum)
				return WIN_X;
			else if(rowSum == playerOWinningSum)
				return WIN_O;
		}

		//check the board columns.
		for(unsigned int i = 0; i < 3; i++) {
			char columnSum = cells[0][i] + cells[1][i] + cells[2][i];
			if(columnSum == playerXWinningSum)
				return WIN_X;
			else if(columnSum == playerOWinningSum)
				return WIN_O;
		}
			
		//check the first board diagonal.
		char diagonalSum = cells[0][0] + cells[1][1] + cells[2][2];
		if(diagonalSum == playerXWinningSum)
			return WIN_X;
		else if(diagonalSum == playerOWinningSum)
			return WIN_O;

		//check the second board diagonal.
		diagonalSum = cells[2][0] + cells[1][1] + cells[0][2];
		if(diagonalSum == playerXWinningSum)
			return WIN_X;
		else if(diagonalSum == playerOWinningSum)
			return WIN_O;

		//calculate absolute total sum of all cells.
		char absTotalSum = 0;
		const char* cellsPointer = &cells[0][0];
		for(unsigned int i = 0; i < 9; i++)
			absTotalSum += abs(cellsPointer[i]);

		//check if we have a draw.
		if(absTotalSum == absDrawSum)
			return DRAW;

		//if we couldn't find a win or a draw, our game is in progress.
		return IN_PROGRESS;
	}


	//find all available moves presented by the board.
	int availableMoves(vector<pair<unsigned int, unsigned int> >& v) const {
		//clear result vector.
		v.clear();

		//check if the game is in progress.
		int state = evaluateState();
		if(state != IN_PROGRESS)
			return state;

		//if the game is in progress, check for empty cells.
		for(unsigned int i = 0; i < 3; i++)
			for(unsigned int j = 0; j < 3; j++)
				if(cells[i][j] == NO_PLAYER)
					v.push_back(pair<unsigned int, unsigned int>(i, j));

		return IN_PROGRESS;
	}


	//game board cells.
	char cells[3][3];
};


//tree node class for the tic-tac-toe game tree.
class treeNode {
public:
	//constructors and destructor.
	treeNode(char pId = PLAYER_X, const gameBoard& b = gameBoard(), unsigned int d = 0) { playerId = pId; board = b; depth = d; }
	treeNode(const treeNode& n) { *this = n; }
	~treeNode() {}

	//helpful operator.
	treeNode& operator=(const treeNode& n) { playerId = n.playerId; board = n.board; depth = n.depth; return *this; }

	char playerId;			//player who is about to play.
	gameBoard board;		//game board.
	unsigned int depth;		//node depth.
};


int main(int argc, char *argv[]) {
	//our data.
	set<string> uniqueNodes;
	vector<treeNode> unProcessedNodes;
	map<unsigned int, unsigned int> numNodesAtDepth;
	vector<pair<unsigned int, unsigned int> > availableMoves;

	//various node and game statistics.
	unsigned int numNodes = 0;
	unsigned int numWinsX = 0;
	unsigned int numWinsO = 0;
	unsigned int numDraws = 0;

	//print stuff.
	cout << "\n- initiating tree traversal...";

	//append root node in node stack.
	unProcessedNodes.push_back(treeNode(PLAYER_X, gameBoard(), 0));

	//traverse the tree in a dfs fashion.
	while(unProcessedNodes.size() > 0) {
		//get current node.
		treeNode currentNode(unProcessedNodes.back());
		unProcessedNodes.pop_back();

		//keep track of node statistics.
		uniqueNodes.insert(currentNode.board.cellsAsStdString());
		numNodesAtDepth[currentNode.depth]++;
		numNodes++;

		//check current node state.
		int currentNodeState = currentNode.board.availableMoves(availableMoves);
		if(currentNodeState == DRAW)
			numDraws++;
		else if(currentNodeState == WIN_X)
			numWinsX++;
		else if(currentNodeState == WIN_O)
			numWinsO++;

		//append new nodes in the node stack.
		unsigned int numAvailableMoves = (unsigned int)availableMoves.size();
		for(unsigned int i = 0; i < numAvailableMoves; i++) {
			gameBoard childBoard(currentNode.board);
			childBoard[availableMoves[i].first][availableMoves[i].second] = currentNode.playerId;
			unProcessedNodes.push_back(treeNode(-currentNode.playerId, childBoard, currentNode.depth + 1));
		}
	}


	//print stuff.
	cout << "\n- tree traversal completed successfully...";


	//print node and game statistics.
	cout << "\n\n- total number of nodes: " << numNodes;
	cout << "\n- number of unique nodes: " << (unsigned int)uniqueNodes.size();
	cout << "\n- number of X wins: " << numWinsX;
	cout << "\n- number of O wins: " << numWinsO;
	cout << "\n- number of draws: " << numDraws  << "\n";


	//print number of nodes per tree depth.
	for(unsigned int i = 0; i < (unsigned int)numNodesAtDepth.size(); i++)
		cout << "\n- number of nodes at tree depth " << i << ": " << numNodesAtDepth[i];


	//calculate the tree's average branching factor.
	float averageBranchingFactor = 0.0f;
	for(unsigned int i = 0; i < (unsigned int)numNodesAtDepth.size() - 1; i++)
		averageBranchingFactor += (float(numNodesAtDepth[i+1])) / (float(numNodesAtDepth[i]));
	averageBranchingFactor /= (float)(numNodesAtDepth.size() - 1);

	//print the tree's average branching factor.
	cout << "\n\n- average tree branching factor: " << averageBranchingFactor << "\n\n";


	return 0;
}