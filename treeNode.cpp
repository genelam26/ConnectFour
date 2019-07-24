#ifndef __TREENODE__CPP
#define __TREENODE__CPP

#include "treeNode.h"

// CONSTRUCTORS 
treeNode::treeNode() {
  setParent(NULL);
  for(int i = 0; i < 7; i++) {
    children[i] = NULL;
  }
}
treeNode::treeNode(size_t col, char user) {        
  Board root;
  root.addPiece(col, user);
  possibleBoard = root;
  setParent(NULL);
  for(int i = 0; i < 7; i++) {
    children[i] = NULL;
  }
}
treeNode::treeNode(Board bo){
	for(size_t i=0; i<bo.getRows(); i++){
		for(size_t j=0; i<bo.getCols(); j++){
			possibleBoard.setPiece(i,j,bo.getPiece(i,j));
		}
	}
  setParent(NULL);
  for(int i = 0; i < 7; i++) {
    children[i] = NULL;
  }
}

// ADD/SET DATA
/* addChild: fills designated slot in children array with pointer to new child
  preconditions: pointer to new child and the column in which the new piece was added */
void treeNode::addChild(treeNode* newChildPtr, int i) {
  children[i] = newChildPtr;
}

/* generateChildren function:
- preconditions: have a turn (user or comp) and an empty children array (the children is a list of pointers pointing from the parent
  to its 7 children)
- when you call parent.generateChildren it will fill the array of pointers connected to each Node with pointers to all these children 
- However, if the column is full, that slot in the list will point to NULL
- postconditions: have the list of pointers (children array) pointing to 7 children with full columns pointing to NULL */
void treeNode::generateChildren(char turn) {
  for(int i = 0; i < 7; i++) {
    treeNode* childPtr;
    childPtr = new treeNode;
    Board childBoard;
    childBoard = possibleBoard;
    childPtr->setParent(this); 
    if(!childBoard.fullColumn(i)) {           
      childBoard.addPiece(turn, i);            
      addChild(childPtr, i);
    } else {
      addChild(nullptr, i);         
    }  
    childPtr->setBoard(childBoard);
  }
}

// OVERLOADED OPERATORS                                            
void treeNode::operator=(treeNode source) {
  possibleBoard = source.getBoard();
  parent = source.getParent();
  for(int i = 0; i < 7; i++) {
    children[i] = source.getChild(i);
  }
}

// BFS

void treeNode::BFS(char turn) {
  char thisTurn = turn;
  if(thisTurn == 'O') {
    thisTurn = 'X';
  } else {
    thisTurn = 'O';
  }
  bool isWinner = false;
  Queue<treeNode*> gameTree;          // QUEUE
  Stack<treeNode*> winningPath;
  gameTree.push(this);			// queue is a queue of pointers, not boards sooo push this pointer 
  treeNode* currentNode;
  //cout << "before generate children: " << gameTree.front()->getBoard();       //TEST
  while(!gameTree.empty()) {
    //cout << "BEFORE THE SWITCH THE TURN IS: " << thisTurn << endl;
    if(thisTurn == 'O') {
      thisTurn = 'X';
    } else {
      thisTurn = 'O';
    }
    //cout << "NOW THIS TURN IS: " << thisTurn << endl;
    currentNode = gameTree.front();
    gameTree.pop(); 
    currentNode->generateChildren(thisTurn);
    /* TESTING:
    for(int i = 0; i < 7; i++) {
      cout << currentNode->getChild(i)->getBoard() << endl;
    } */
    for(int i = 0; i < 7; i++) {
      gameTree.push(currentNode->getChild(i)); 
      //cout << "CURRENT PARENT: " << endl << currentNode->getBoard();
      //cout << "current child: " << endl << currentNode->getChild(i)->getBoard() << endl; 
      if(currentNode->getChild(i) == nullptr) {
        continue;
      }      
      if(currentNode->getChild(i)->getBoard().hasWinner()) {
        winningPath.push(currentNode->getChild(i));
        cout << "WINNER HAS BEEN FOUND!" << endl;
        isWinner = true;
        break;
      }
    }
	  if(isWinner) { 
      break; 
    }
   // cout << "====== END TURN ========" << endl;
   // cout << "CURRENT TURN IS: " << thisTurn << endl;
  }
  treeNode* winner = winningPath.top();
  cout << "Winning Board: " << endl << winner->getBoard() << endl;		// PRINT BOARD FUNCTION OR COUT OVERLOADED
  treeNode* rootParent;
  rootParent = winner->getParent();
  cout << "PATH TO WINNING BOARD: " << endl;
  while(rootParent != NULL) {
    winningPath.push(rootParent);
    rootParent = rootParent->getParent();
  }
  cout << "Make this move: " << endl;
  while(!winningPath.empty()) {
    Board path = winningPath.top()->getBoard();
    cout << path << endl;
    winningPath.pop(); 
    cout << "Then, make this next move: " << endl;   
  }
  cout << "^Winning board reached^" << endl << "End of Search." << endl;
} 
// NO GENERATE CHILDREN
bool treeNode::itdfs(char turn, treeNode* currentNode, Stack<treeNode*>& nodeStack,int level, int maxLevel) { // USER PART && CURRENTNODE
  char thisTurn = turn;
  if(thisTurn == 'X') { 
    thisTurn = 'O'; 
  } else {
    thisTurn = 'X';
  }
  if(currentNode->getBoard().hasWinner()) {
    cout << "-----A WINNING BOARD HAS BEEN FOUND-----" << endl;
  	Stack<treeNode*> winningPath;
	  winningPath.push(currentNode);
    cout << "WINNING BOARD: " << endl << currentNode->getBoard() <<endl;
	  while(currentNode->getParent() != NULL) {
	    currentNode = currentNode->getParent();
	    winningPath.push(currentNode);
    }
    treeNode* nextBoard;
    cout << "From the given board, make this following move: " << endl;
    while(!winningPath.empty()) {
      nextBoard = winningPath.top();
      cout << nextBoard->getBoard() << endl << "Then:" << endl;
      winningPath.pop(); 
    }
    cout << "You're done! You've reached the winning Board!" << endl << "End of Search." << endl;
    return true;
  }
  nodeStack.push(currentNode);
  if(level < maxLevel) {
	  for (int i = 0; i < 7; i++) {
      cout << currentNode->getBoard() << endl;
      currentNode->generateChildren(thisTurn);
      nodeStack.push(currentNode->getChild(i));
		  if (currentNode->getChild(i) == nullptr) {     
        continue;
      }
      if(itdfs(turn, currentNode->getChild(i), nodeStack, level + 1, maxLevel)) {  // WORST CASE, turn thisTurn back to turn
			  return true;
		  }
	  }
  }
  nodeStack.pop();
  return false;
}
	  
void treeNode::IT(char turn) { // USER PART
	treeNode* currentNode;
	currentNode = this;
	for(int i= 0; i < 15; i++) {
		Stack<treeNode*> nodeStack;
		if(itdfs(turn, currentNode, nodeStack, 0, i)== true) {
			break;
		}
	}
}

// Searching Algorithms
void DFS(treeNode*& b, char turn, Stack<treeNode*>& childrenStack){
  // Base Case: returns the winning board or tied board if no other solution possible
  if(b->getBoard().hasWinner()||(b->getBoard().boardFull()&&childrenStack.empty())){ // onlytime you want to print is if board has winner or if full, childrenstack empty isnt necessary
    cout<<b->getBoard(); // winning board
    while(b->getParent()!=nullptr){
      cout<<b->getParent()->getBoard();
      b=b->getParent();
	    return;
    }
  }
  /*
   * If the board isn't full, DFS will push in the child nodes into the childrenStack.
   * Then it will perform DFS on the most leftest child node. If the board is full, it will use the 
   * childrenStack to find the next node to perform DFS on.
   */
  else if(!b->getBoard().boardFull()){
    b->generateChildren(turn);
    for(int i=6; i>=0;i--){
      if((b->getChild(i)) == nullptr) { continue; }
      else { childrenStack.push(b->getChild(i)); }
    }
  } 
  treeNode* onTop=childrenStack.top();
  childrenStack.pop();
  if(turn=='X') turn='O';
  else turn='X';
  DFS(onTop,turn,childrenStack);  
}

// computerTurn = 'X; userTurn = 'O'
void generateScore(treeNode* b, size_t level, char turn){
  // initalize the board and score
  Board in=b->getBoard();
  int score=0; level++; size_t consecutive=0; int tempScore=0;
  // adds score based on how many consecutives each board has
	for(size_t i=0; i<3; i++){
		for(size_t j=0; j<7; j++){
		  if(turn==in.getPiece(i,j)==in.getPiece(i+1,j)==in.getPiece(i+2,j)==in.getPiece(i+3,j)){ consecutive=4; }
      else if (turn==in.getPiece(i,j)==in.getPiece(i+1,j)==in.getPiece(i+2,j)){ consecutive=3; }
      else if (turn==in.getPiece(i,j)){ consecutive=2; }
      score+=pow(level,consecutive);
		}
	}
	for(size_t i=0; i<3; i++){
		for(size_t j=0; j<4; j++){
			if(turn==in.getPiece(i,j)==in.getPiece(i,j+1)==in.getPiece(i,j+2)==in.getPiece(i,j+3)){ consecutive=4; }
      else if (turn==in.getPiece(i,j)==in.getPiece(i,j+1)==in.getPiece(i,j+2)){ consecutive=3; }
      else if (turn==in.getPiece(i,j)==in.getPiece(i,j+1)){ consecutive=2; }
      score+=pow(level,consecutive);
		}
	}
	for(size_t i=3; i<6; i++){
		for(size_t j=0; j<4;j++){
      if(turn==in.getPiece(i,j)==in.getPiece(i+1,j+1)==in.getPiece(i+2,j+2)==in.getPiece(i+3,j+3)){ consecutive=4; }
      else if (turn==in.getPiece(i,j)==in.getPiece(i+1,j+1)==in.getPiece(i+2,j+2)){ consecutive=3; }
      else if (turn==in.getPiece(i,j)==in.getPiece(i+1,j+1)){ consecutive=2; }
      score+=pow(level,consecutive);
		}
	}
	for(size_t i=3; i<6; i++){
		for(size_t j=3; j<7;j++){
			if(turn==in.getPiece(i,j)==in.getPiece(i-1,j-1)==in.getPiece(i-2,j-2)==in.getPiece(i-3,j-3)){ consecutive=4; }
    		  else if (turn==in.getPiece(i,j)==in.getPiece(i-1,j-1)==in.getPiece(i-2,j-2)){ consecutive=3; }
      else if (turn==in.getPiece(i,j)==in.getPiece(i-1,j-1)){ consecutive=2; }
      score+=pow(level,consecutive);
		}
	}
  in.setScore(score);
}

void evaluateUp(treeNode* b, char turn){
	int minMax; 
	if(turn=='O') { minMax=100000000; }	
	else { minMax=-100000000; }
	// while your current subtree isn't the root, you compare yourself to your siblings and based on the turn, you minimize or maximize
	while(b->getParent()!= NULL){
		b=b->getParent(); 
		if(b->getChild(0)!=NULL && b->getChild(1)!=NULL && b->getChild(2)!=NULL && b->getChild(3)!=NULL
			 && b->getChild(4)!=NULL && b->getChild(5)!=NULL && b->getChild(6)!=NULL){
			for(size_t i=0; i<7; i++){
				int temp=(b->getChild(i)->getBoard().getScore());
				// if the current turn is the user, you try to minimize
				if(turn=='O' && temp<minMax){ minMax=temp; } 
				// otherwise, you're maximizing
				else if (temp>minMax) { minMax=temp; }
			}
			b->getBoard().setScore(minMax);                                                            
		}
		// if one of your sibling is NULL, you return the function and wait until it gets a score
		else { return; }
	}
	// for printing the possible board combinations out
	while(b->getChildren()!=NULL){
		for(size_t i=0; i<7;i++){
    	if(b->getChild(i)==NULL) { continue; }
			else if(b->getChild(i)->getBoard().getScore()==minMax){                         
				b=b->getChild(i);
				cout<<b->getBoard();
			}
		}
	}
}
/*
 * Precondition: A root containing board b that can still be filled
 * Postcondition: A tree from that board b with 4 levels of possible board choices. Each node has 7 children.
 * 		When the program finishes, each leaf score is calculated with generateScore. At the end,  
 *		evaluateUp takes each leaf score and "miniMaxes" it upward.
 */
void MiniMax(treeNode* b, size_t level, char turn){
  // If the node is null, we generate the score for the parent and stop generating null children
  if(b==NULL){
    if(turn=='O') turn='X';
    else turn='O';
    level++;
    generateScore(b->getParent(),level, turn);
    return;
  }
  if(level==0){
    evaluateUp(b, turn); 
    return;
  }
  // Otherwise, the MiniMax function continues to generate child nodes
  b->generateChildren(turn);
  for(size_t i=0; i<7;i++){
    if(b->getChild(i) == NULL) { continue; }
    if(turn=='O') { turn='X'; }
    else { turn='O'; }
    // for each level, the minimax function will recursively call upon itself to create more children nodes
    if(level>1) {   level--; MiniMax(b->getChild(i),level,turn); }
    // if level is 1, we have to generate child nodes and also their score
    else { level--; generateScore(b->getChild(i),level,turn); }
  }
}
    
#endif
