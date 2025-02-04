import numpy as np
from blockworld import BlockWorldEnv
from collections import defaultdict
import random

class QLearning():
	# don't modify the methods' signatures!
	def __init__(self, env: BlockWorldEnv):
		self.env = env
		self.q = {}

	def train(self):
		alpha = 0.8
		gamma = 0.95
		epsilon = 0.2
		min_epsilon = 0.01
		epsilon_decay = 0.995
		
		for ep in range(10000):
			state = self.env.reset()
			done = False
			while not done:
				if np.random.rand() < epsilon:
					action = random.choice(state[0].get_actions())
				else:
					action = self.act(state)
				next_state, reward, done = self.env.step(action)
				max_next_q = max(self.q.get((next_state, a), 0) for a in next_state[0].get_actions())
				current_q = self.q.get((state, action), 0)
				new_q = current_q + alpha * (reward + gamma * max_next_q - current_q)
				self.q[(state, action)] = new_q
				state = next_state
		#epsilon = max(min_epsilon, epsilon * (epsilon_decay))
   
	def act(self, s):
		return max(s[0].get_actions(), key=lambda a: self.q.get((s, a), 0))

if __name__ == '__main__':
	# Here you can test your algorithm. Stick with N <= 4
	N = 4

	env = BlockWorldEnv(N)
	qlearning = QLearning(env)

	# Train
	qlearning.train()

	# Evaluate
	test_env = BlockWorldEnv(N)

	test_problems = 10
	solved = 0
	avg_steps = []

	for test_id in range(test_problems):
		s = test_env.reset()
		done = False

		print(f"\nProblem {test_id}:")
		print(f"{s[0]} -> {s[1]}")

		for step in range(50): 	# max 50 steps per problem
			a = qlearning.act(s)
			s_, r, done = test_env.step(a)

			print(f"{a}: {s[0]}")

			s = s_

			if done:
				solved += 1
				avg_steps.append(step + 1)
				break

	avg_steps = sum(avg_steps) / len(avg_steps)
	print(f"Solved {solved}/{test_problems} problems, with average number of steps {avg_steps}.")