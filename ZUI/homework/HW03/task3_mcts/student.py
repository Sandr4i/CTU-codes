import random, time
import numpy as np
import math
import ox

class Node:
	def __init__(self, board, action=None, parent=None):
		self.board = board
		self.parent = parent
		self.action = action
		self.children = {} #Children states of current state
		self.visits = 0 #Number of times visited particular state
		self.wins = 0 #Number of possible wins the state leads to

	def is_full(self):
		return len(self.children) == len(self.board.get_actions())

	def best_uct(self):
		best_value = -float("inf")
		best_child = None
		for child in self.children.values():
			if child.visits == 0: return child
			value = child.wins/child.visits + math.sqrt(2*math.log(self.visits)/child.visits)
			if value > best_value:
				best_value = value
				best_child = child
	
		return best_child

class MCTSBot:
	def __init__(self, play_as: int, time_limit: float):
		self.play_as = play_as
		self.time_limit = time_limit * 0.9

	def traverse(self, node):
		while node.is_full():
			node = node.best_uct()

		if not node.board.is_terminal():
			actions = list(node.board.get_actions())
			unexplored = [a for a in actions if a not in node.children]
			action = random.choice(unexplored)
			new_board = node.board.clone()
			new_board.apply_action(action)
			new_node = Node(board=new_board, parent=node, action=action)
			node.children[action] = new_node
		
		return node

	def rollout(self, node):
		board = node.board.clone()
		while not board.is_terminal():
			action = random.choice(list(board.get_actions()))
			board.apply_action(action)

		return board.get_rewards()

	def backpropagade(self, node, result):
		while node is not None:
			node.visits += 1
			node.wins += result[self.play_as]
			node = node.parent

	def best_child(self, node):
		best_visits = -1
		best_child = None
		for child in node.children.values():
			if child.visits > best_visits:
				best_visits = child.visits
				best_child = child
		
		return best_child

	def play_action(self, board):
		start_time = time.time()
		root = Node(board=board.clone())

		while (time.time() - start_time) < self.time_limit:
			current = self.traverse(root)
			result = self.rollout(current)
			self.backpropagade(current, result)

		best_child = self.best_child(root)
		if best_child: return best_child.action
		return random.choice(list(board.get_actions()))

if __name__ == '__main__':
	board = ox.Board(8)  # 8x8
	bots = [MCTSBot(0, 0.1), MCTSBot(1, 1.0)]

	# try your bot against itself
	while not board.is_terminal():
		current_player = board.current_player()
		current_player_mark = ox.MARKS_AS_CHAR[ ox.PLAYER_TO_MARK[current_player] ]

		current_bot = bots[current_player]
		a = current_bot.play_action(board)
		board.apply_action(a)

		print(f"{current_player_mark}: {a} -> \n{board}\n")