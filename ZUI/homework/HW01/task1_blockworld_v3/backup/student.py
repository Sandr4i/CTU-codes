from queue import PriorityQueue
from blockworld import BlockWorld

class BlockWorldHeuristic(BlockWorld):
	def __init__(self, num_blocks=5, state=None):
		BlockWorld.__init__(self, num_blocks, state)

	def heuristic(self, goal):
		self_state = self.get_state()
		goal_state = goal.get_state()

		# ToDo. Implement the heuristic here.

		#Version 3
		self_list = list(self_state)
		goal_list = list(goal_state)
		blocks_num = 0
		cost = 0

		for block_in_state in self_list:
			blocks_num += len(block_in_state)
			for block_in_goal in goal_list:
				local_i = 1
				while(block_in_state[-local_i] == block_in_goal[-local_i]):
					local_i += 1
					if (local_i > len(block_in_state) or local_i > len(block_in_goal)):
						break
				if local_i > 1:
					cost += local_i
					goal_list.remove(block_in_goal)
					break
		return blocks_num - cost

class AStar():
	def search(self, start, goal):
		# ToDo. Return a list of optimal actions that takes start to goal.
		
		queue = PriorityQueue()
		queue.put((0, start))
		path = {start: (None, None)}
		closed = set()
		total_cost = {start: 0}

		while not queue.empty():
			cost, current = queue.get()
			if current in closed:
				continue
			if current == goal:
				actions = []
				while current != start:
					action, current = path[current]
					actions.append(action)
				actions.reverse()
				return actions
			closed.add(current)
			for action, neighbor in current.get_neighbors():
				if neighbor in closed:
					continue
				new_cost = total_cost[current] + 1
				if neighbor not in total_cost.keys() or new_cost < total_cost[neighbor]:
					total_cost[neighbor] = new_cost
					priority = new_cost + neighbor.heuristic(goal)
					queue.put((priority, neighbor))
					path[neighbor] = (action, current)

if __name__ == '__main__':
	# Here you can test your algorithm. You can try different N values, e.g. 6, 7.
	N = 5

	start = BlockWorldHeuristic(N)
	goal = BlockWorldHeuristic(N)

	print("Searching for a path:")
	print(f"{start} -> {goal}")
	print()

	astar = AStar()
	path = astar.search(start, goal)

	if path is not None:
		print("Found a path:")
		print(path)

		print("\nHere's how it goes:")

		s = start.clone()
		print(s)

		for a in path:
			s.apply(a)
			print(s)

	else:
		print("No path exists.")

	print("Total expanded nodes:", BlockWorld.expanded)